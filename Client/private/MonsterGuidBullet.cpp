#include "MonsterGuidBullet.h"
#include "MonsterMissile.h"
#include "Particle_Define.h"
#include "CameraManager.h"


CMonsterGuidBullet::CMonsterGuidBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CMonsterGuidBullet::CMonsterGuidBullet(const CMonsterGuidBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CMonsterGuidBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonsterGuidBullet::Initialize(void* pArg)
{
	/* 플레이어를 알고 있어라 */
	m_pTargetPlayer = GET_PLAYER;
	Safe_AddRef(m_pTargetPlayer);

	/* 카메라를 알고 있어라 */
	m_pCamera = CAMERA_MANAGER;
	Safe_AddRef(m_pCamera);

	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("PC_Generic");
	m_szBufferType = TEXT("Rect");

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	/* [ 방향 설정 ] */
	m_vInitDir.Normalize();
	m_vCurrentDir = m_vInitDir;

	//미사일 파티클
	CPSystem::DESC Missile_iDesc{};
	Missile_iDesc.vPosition = { 0.f, 0.f, 0.f };
	Missile_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	Missile_iDesc.iParticleNums = 100;
	Missile_iDesc.fSize = 12.f;
	Missile_iDesc.fMaxFrame = 20.f;
	Missile_iDesc.fNum = 40.f;
	
	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterMissile"),
		LEVEL_GAMEPLAY, L"Layer_Monster", ppOut, &Missile_iDesc)))
		return E_FAIL;
	
	m_pMissile = *ppOut;
	return S_OK;
}

HRESULT CMonsterGuidBullet::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	//셰이더 장착
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* 폭발 사운드 */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Explorsion"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	if (pArg != nullptr)
	{
		//값 가져올 것 있으면 여기서 ↓
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_vInitDir = pDesc->vDir;
	}

	/* 콜라이드 컴포넌트 */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = {};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_BOSS;
	ColliderDesc.iColliderID = CI_BOSS_GUIDBULLET;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

void CMonsterGuidBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMonsterGuidBullet::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	/* [ 타겟 설정 ] */
	SetTargetDir();

	//따라오는 파티클
	if (m_pMissile)
		static_cast<CMonsterMissile*>(m_pMissile)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	// 플레이어까지 향하는 방향 벡터
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToTarget = m_vTargetPos - vMyPos;
	vToTarget.Normalize();

	// ★ 현재 방향 벡터를 타겟 방향으로 보간 → 점점 꺾이는 방향
	
	// 시간 누적
	m_fPreChaseTime += fTimeDelta;
	// 0 ~ maxTurnSpeed 사이로 점점 커지는 회전 속도
	float fTurnSpeed = EaseInCubic(m_fPreChaseTime, 0.8f, 10.f);
	m_vCurrentDir = LerpVec(m_vCurrentDir, vToTarget, fTimeDelta * fTurnSpeed);
	m_vCurrentDir.Normalize();

	// 이동
	vMyPos += m_vCurrentDir * 300.f * fTimeDelta;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vMyPos);

	return __super::Update(fTimeDelta);
}

void CMonsterGuidBullet::Late_Update(_float fTimeDelta)
{
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

#include "FXMgr.h"
void CMonsterGuidBullet::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	m_pSoundCom->SetVolume("explosion_02", 0.8f);
	m_pSoundCom->Play("explosion_02");

	if (m_pMissile)
	{
		static_cast<CMonsterMissile*>(m_pMissile)->SetDead();
		m_pMissile = nullptr;
	}

	m_bDead = TRUE;

	if (OtherColliderID == CI_BLOCK_COMMON)
	{
		FX_MGR->SpawnExplosion2(CCollider::Get_Last_Collision_Pos(), m_eLevelID);
		m_pCamera->Shake_Camera(0.4f, 0.4f);
	}

	if (CI_WEAPON(OtherColliderID))
		FX_MGR->SpawnExplosion2(CCollider::Get_Last_Collision_Pos(), m_eLevelID);

}

HRESULT CMonsterGuidBullet::Render()
{
	SetUp_RenderState();

	//텍스처 셰이더로 넘기기
	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", 0);

	//메트릭스 셰이더로 넘기기
	_float4x4 maxView, maxProj;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &maxView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &maxProj);

	_float4x4 matBillboard = m_pTransformCom->Billboard();
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &matBillboard)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &maxView)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &maxProj)))
		return E_FAIL;

	m_fAccTime += 1.0f / 60.0f;
	m_pShaderCom->SetFloat("g_fTime", m_fAccTime);
	m_pShaderCom->SetFloat("g_fEmissivePower", 2.5f);

	//셰이더 시작
	m_pShaderCom->Begin(CShader::BOSSMISSILE);

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;


	/* ------------------------------------------------------------------------- */

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	/* ------------------------------------------------------------------------- */

	m_pShaderCom->End();


	Release_RenderState();

	return S_OK;
}

HRESULT CMonsterGuidBullet::SetUp_RenderState()
{
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	return S_OK;
}

HRESULT CMonsterGuidBullet::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	return S_OK;
}

void CMonsterGuidBullet::SetTargetDir()
{
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	// 약간의 랜덤 오차를 추가
	m_vTargetPos.x += GetRandomFloat(-50.f, 50.f);
	m_vTargetPos.z += GetRandomFloat(-50.f, 50.f);
}

float CMonsterGuidBullet::EaseInCubic(float currentTime, float duration, float maxValue)
{
	float t = currentTime / duration;
	t = clamp(t, 0.f, 1.f); // 안정성 확보
	return t * t * t * maxValue;
}

CMonsterGuidBullet* CMonsterGuidBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonsterGuidBullet* pInstance = new CMonsterGuidBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonsterGuidBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonsterGuidBullet::Clone(void* pArg)
{
	CMonsterGuidBullet* pInstance = new CMonsterGuidBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMonsterGuidBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterGuidBullet::Free()
{
	__super::Free();

	Safe_Release(m_pTargetPlayer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCamera);
	Safe_Release(m_pSoundCom);
}
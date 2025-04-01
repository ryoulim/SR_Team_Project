#include "MonsterBullet.h"
#include "MonsterMissile.h"
#include "Particle_Define.h"
#include "CameraManager.h"

CMonsterBullet::CMonsterBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CMonsterBullet::CMonsterBullet(const CMonsterBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CMonsterBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonsterBullet::Initialize(void* pArg)
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

	/* [ 타겟 설정 ] */
	SetTargetDir();

	//미사일 파티클
	CPSystem::DESC Missile_iDesc{};
	Missile_iDesc.vPosition = { 0.f, 0.f, 0.f };
	Missile_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	Missile_iDesc.iParticleNums = 1000;
	Missile_iDesc.fSize = 10.f;
	Missile_iDesc.fMaxFrame = 20.f;
	Missile_iDesc.fNum = 60.f;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterMissile"),
		LEVEL_GAMEPLAY, L"Layer_Monster", ppOut, &Missile_iDesc)))
		return E_FAIL;

	m_pMissile = *ppOut;
	return S_OK;
}

HRESULT CMonsterBullet::Ready_Components(void* pArg)
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

	if (pArg != nullptr)
	{
		//값 가져올 것 있으면 여기서 ↓
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(pDesc->vScale);
	}

	/* 콜라이드 컴포넌트 */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = {};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MBULLET;
	ColliderDesc.iColliderID = CI_MONSTER_BULLET;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

void CMonsterBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMonsterBullet::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;
	
	if (m_pMissile)
		static_cast<CMonsterMissile*>(m_pMissile)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	
	//플레이어로 향해 날라가라
	m_pTransformCom->ChaseCustom(m_vTargetPos, fTimeDelta, 1.f, 1000.f);

	return __super::Update(fTimeDelta);
}

void CMonsterBullet::Late_Update(_float fTimeDelta)
{
	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

#include "FXMgr.h"
void CMonsterBullet::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (m_pMissile)
	{
		static_cast<CMonsterMissile*>(m_pMissile)->SetDead();
		m_pMissile = nullptr;
	}

	m_bDead = TRUE;
	
	if (OtherColliderID == CI_BLOCK_COMMON)
	{
		FX_MGR->SpawnExplosion2(CCollider::Get_Last_Collision_Pos(), m_eLevelID);
		m_pCamera->Shake_Camera();
	}
}

HRESULT CMonsterBullet::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;
	//m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard());
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	
	return S_OK;
}

void CMonsterBullet::SetTargetDir()
{
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos.y = 0.f;

	// 약간의 랜덤 오차를 추가
	m_vTargetPos.x += GetRandomFloat(-200.f, 250.f);
	m_vTargetPos.z += GetRandomFloat(-200.f, 250.f);
}

CMonsterBullet* CMonsterBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonsterBullet* pInstance = new CMonsterBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonsterBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonsterBullet::Clone(void* pArg)
{
	CMonsterBullet* pInstance = new CMonsterBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMonsterBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterBullet::Free()
{
	__super::Free();

	Safe_Release(m_pTargetPlayer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCamera);
}
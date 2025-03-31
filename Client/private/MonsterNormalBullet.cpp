#include "MonsterNormalBullet.h"
#include "MonsterMissile.h"
#include "Particle_Define.h"
#include "CameraManager.h"

CMonsterNormalBullet::CMonsterNormalBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CMonsterNormalBullet::CMonsterNormalBullet(const CMonsterNormalBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CMonsterNormalBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonsterNormalBullet::Initialize(void* pArg)
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
	Missile_iDesc.fSize = 0.07f;
	Missile_iDesc.fMaxFrame = 20.f;
	Missile_iDesc.fLifeTime = GetRandomFloat(1.f, 3.f);

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterMissile"),
		LEVEL_GAMEPLAY, L"Layer_Monster", ppOut, &Missile_iDesc)))
		return E_FAIL;

	m_pMissile = *ppOut;
	return S_OK;
}

HRESULT CMonsterNormalBullet::Ready_Components(void* pArg)
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

void CMonsterNormalBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if ((vMyPos - m_vTargetPos).Length() <= 10.f)
		m_bDead = true;
}

EVENT CMonsterNormalBullet::Update(_float fTimeDelta)
{
	if (m_bDead)
	{
		if (m_pMissile)
		{
			static_cast<CMonsterMissile*>(m_pMissile)->SetDead();
			m_pMissile = nullptr;
		}
		return EVN_DEAD;
	}

	if (m_pMissile)
		static_cast<CMonsterMissile*>(m_pMissile)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	//플레이어로 향해 날라가라
	m_pTransformCom->ChaseCustom(m_vTargetPos, fTimeDelta, 1.f, 1000.f);

	return __super::Update(fTimeDelta);
}

void CMonsterNormalBullet::Late_Update(_float fTimeDelta)
{
	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

#include "FXMgr.h"
void CMonsterNormalBullet::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (m_pMissile)
	{
		static_cast<CMonsterMissile*>(m_pMissile)->SetDead();
		m_pMissile = nullptr;
	}

	m_bDead = true;

	if (OtherColliderID == CI_BLOCK_COMMON)
	{
		//FX_MGR->SpawnExplosion2(CCollider::Get_Last_Collision_Pos(), m_eLevelID);
		//m_pCamera->Shake_Camera(0.1f, 0.1f, 2.f, 1.f);
	}
	if (CI_PLAYER(OtherColliderID))
	{
		m_bDead = true;
	}
}

HRESULT CMonsterNormalBullet::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
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

void CMonsterNormalBullet::SetTargetDir()
{
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos.y = 30.f;
	_float3 vDir = vMyPos - m_vTargetPos;
	vDir.Normalize();
	m_vTargetPos -= vDir * 400.f;

	// 약간의 랜덤 오차를 추가
	m_vTargetPos.x += GetRandomFloat(0.f, 25.f);
	m_vTargetPos.y += GetRandomFloat(0.f, 25.f);
	m_vTargetPos.z += GetRandomFloat(0.f, 25.f);
}

CMonsterNormalBullet* CMonsterNormalBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonsterNormalBullet* pInstance = new CMonsterNormalBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonsterNormalBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonsterNormalBullet::Clone(void* pArg)
{
	CMonsterNormalBullet* pInstance = new CMonsterNormalBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMonsterNormalBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterNormalBullet::Free()
{
	__super::Free();

	Safe_Release(m_pTargetPlayer);
	Safe_Release(m_pCamera);
}
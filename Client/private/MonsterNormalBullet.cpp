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

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

//#define FLESHJUMPPOWER 20.f
	/* [ 타겟 설정 ] */
	SetTargetDir();

	if (m_bFlesh)
		m_pGravityCom->Jump(0.05f * m_fTargetDistance);

	//미사일 파티클
	CPSystem::DESC Missile_iDesc{};
	Missile_iDesc.vPosition = { 0.f, 0.f, 0.f };
	Missile_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	Missile_iDesc.iParticleNums = 1000;
	Missile_iDesc.fMaxFrame = 20.f;
	Missile_iDesc.fLifeTime = GetRandomFloat(1.f, 3.f);

	if (m_bFlesh)
		Missile_iDesc.fSize = 0.7f;
	else
		Missile_iDesc.fSize = 0.4f;

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
	if (pArg == nullptr)
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;


	//값 가져올 것 있으면 여기서 ↓
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
	m_pTransformCom->Scaling(pDesc->vScale);
	//m_bGravity = pDesc->bGravity;
	//m_bAnimation = pDesc->bAnimation;
	m_szTextureID = pDesc->szTextureID;
	m_fSpeed = pDesc->fSpeedPerSec;
	m_bBlueFire = pDesc->bBlueFire;
	m_bFlesh = pDesc->bFlesh;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* 콜라이드 컴포넌트 */
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = {};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MBULLET;
	ColliderDesc.iColliderID = pDesc->iColliderID;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	if (m_bFlesh)
	{
		/* For.Com_Gravity */
		CGravity::DESC GravityDesc{};
		GravityDesc.pTransformCom = m_pTransformCom;

		GravityDesc.fTimeIncreasePerSec = 3.6f;
		GravityDesc.fMaxFallSpeedPerSec = 400.f;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Gravity"),
			TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void CMonsterNormalBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fLifetime += fTimeDelta;
	if (m_fLifetime >= 2.5f)
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
	if (m_bFlesh)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else
		m_pTransformCom->ChaseCustom(m_vTargetPos, fTimeDelta, 1.f, m_fSpeed);

	return __super::Update(fTimeDelta);
}

void CMonsterNormalBullet::Late_Update(_float fTimeDelta)
{
	if (m_bFlesh)
		m_pGravityCom->Update(fTimeDelta);

	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;


	if (m_bFlesh || m_bBlueFire)
		FrameUpdate(fTimeDelta, 8.f, 15.f, true);
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
	m_bDead = true; // 뭐가 됐든 없어지는게 맞는 것 같음

}

HRESULT CMonsterNormalBullet::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	if (m_bBlueFire)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	__super::Render();
	
	if (m_bBlueFire)
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CMonsterNormalBullet::SetTargetDir()
{
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	//m_vTargetPos.y = 30.f;
	_float3 vDir = vMyPos - m_vTargetPos;
	m_fTargetDistance = vDir.Length();
	vDir.Normalize();
	m_vTargetPos -= vDir * 400.f;

	// 약간의 랜덤 오차를 추가

	if (m_bFlesh)
	{
		//m_vTargetPos.y += 50.f;
		m_pTransformCom->LookAt(m_vTargetPos);
	
	}
	else
	{
		m_vTargetPos.x += GetRandomFloat(0.f, 25.f);
		m_vTargetPos.y += GetRandomFloat(0.f, 25.f);
		m_vTargetPos.z += GetRandomFloat(0.f, 25.f);
	}
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
	Safe_Release(m_pGravityCom);
}
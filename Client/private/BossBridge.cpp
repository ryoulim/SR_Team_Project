// 내 클래스 이름 : BossBridge
// 부모 클래스 이름 : Map

#include "BossBridge.h"
#include "GameInstance.h"
#include "CameraManager.h"
#include "FXMgr.h"

CBossBridge::CBossBridge(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CBossBridge::CBossBridge(const CBossBridge& Prototype)
	: CMap(Prototype)
{
}

HRESULT CBossBridge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossBridge::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFallTime = 15.f;

	return S_OK;
}

void CBossBridge::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBossBridge::Update(_float fTimeDelta)
{
	if (m_bTrigger)
	{
		m_fTimeaAcc += fTimeDelta;
		if (m_fFallTime <= m_fTimeaAcc)
			m_bTrigger = FALSE; // 어짜피 트리거는 켜지면서 죽으니까 다시 트루될 일 없음
		
		m_pTransformCom->Go_Down(fTimeDelta);
		m_pColliderCom->Update_Collider();

		if(!m_bDoOnce)
		{
			//소용돌이(중앙)
			FX_MGR->SpawnSpher(_float3{ 1430.f, 0.f, 1480.f }, LEVEL_GAMEPLAY);
			m_bDoOnce = true;
		}
	}

	return __super::Update(fTimeDelta);
}

void CBossBridge::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBossBridge::Render()
{
	return __super::Render();
}

HRESULT CBossBridge::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_BLOCK;
	ColliderDesc.iColliderID = CI_BLOCK_COMMON;

	auto& vAngle = static_cast<DESC*>(pArg)->vAngle;
	_wstring ColliderTag = vAngle.x == 0 && vAngle.y == 0 && vAngle.z == 0 ?
		TEXT("Prototype_Component_Collider_AABB_Cube") :
		TEXT("Prototype_Component_Collider_OBB_Cube");

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ColliderTag,
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

inline void CBossBridge::On_Trigger() {
	m_bTrigger = TRUE;
}

CBossBridge* CBossBridge::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossBridge* pInstance = new CBossBridge(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBossBridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossBridge::Clone(void* pArg)
{
	CBossBridge* pInstance = new CBossBridge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBossBridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossBridge::Free()
{
	__super::Free();
}
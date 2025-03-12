// 내 클래스 이름 : MyCube
// 부모 클래스 이름 : Statue

#include "MyCube.h"
#include "GameInstance.h"

CMyCube::CMyCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue{ pGraphic_Device }
{
}

CMyCube::CMyCube(const CMyCube& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CMyCube::Reset(void* pArg)
{
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->LookAt(pDesc->vInitPos+pDesc->vLook);
		m_pTransformCom->Scaling(pDesc->vScale);
	}

	return m_pTransformCom->Initialize(pArg);
}

HRESULT CMyCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMyCube::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("MyCube");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->LookAt(pDesc->vLook);
	}

	return S_OK;
}

void CMyCube::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMyCube::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);

	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > 3.f)
	{
		m_fTimeAcc = 0.f;
		m_pGameInstance->Deactive_Object(TEXT("ObjectPool_MyCube"), this);
		return EVN_OFF;
	}

	return __super::Update(fTimeDelta);
}

void CMyCube::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMyCube::Render()
{
	return __super::Render();
}

CMyCube* CMyCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMyCube* pInstance = new CMyCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMyCube::Clone(void* pArg)
{
	CMyCube* pInstance = new CMyCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMyCube::Free()
{
	__super::Free();
}
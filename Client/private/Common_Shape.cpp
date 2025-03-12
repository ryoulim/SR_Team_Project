// 내 클래스 이름 : Common_Shape
// 부모 클래스 이름 : Statue

#include "Common_Shape.h"
#include "GameInstance.h"

CCommon_Shape::CCommon_Shape(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue{ pGraphic_Device }
{
}

CCommon_Shape::CCommon_Shape(const CCommon_Shape& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CCommon_Shape::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCommon_Shape::Initialize(void* pArg)
{
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_eLevelID = pDesc->eLevelID;
		m_szTextureID = pDesc->szTextureID;
		m_szBufferType = pDesc->szBufferType;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCommon_Shape::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CCommon_Shape::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CCommon_Shape::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCommon_Shape::Render()
{
	return __super::Render();
}

CCommon_Shape* CCommon_Shape::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCommon_Shape* pInstance = new CCommon_Shape(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCommon_Shape");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCommon_Shape::Clone(void* pArg)
{
	CCommon_Shape* pInstance = new CCommon_Shape(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCommon_Shape");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_Shape::Free()
{
	__super::Free();
}
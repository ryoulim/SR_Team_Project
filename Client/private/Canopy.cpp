#include "Canopy.h"
#include "GameInstance.h"
#include "Player.h"
#include "Client_Defines.h"
#include "FXMgr.h"

CCanopy::CCanopy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CCanopy::CCanopy(const CCanopy& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CCanopy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCanopy::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Canopy");
	m_szBufferType = TEXT("Canopy");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCanopy::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CCanopy::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CCanopy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCanopy::Render()
{
	MultiTextureShaderRender(2);

	return S_OK;
}

CCanopy* CCanopy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCanopy* pInstance = new CCanopy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCanopy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCanopy::Clone(void* pArg)
{
	CCanopy* pInstance = new CCanopy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCanopy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCanopy::Free()
{
	__super::Free();
}

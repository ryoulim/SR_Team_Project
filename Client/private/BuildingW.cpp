#include "BuildingW.h"
#include "GameInstance.h"
#include "Shader.h"

CBuildingW::CBuildingW(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CBuildingW::CBuildingW(const CBuildingW& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CBuildingW::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuildingW::Initialize(void* pArg)
{
	m_szTextureID = TEXT("BuildingW");
	m_szBufferType = TEXT("BuildingW");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBuildingW::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBuildingW::Update(_float fTimeDelta)
{
	m_pTransformCom->Move({ 0.f,0.f,-RACE_SPEED_PER_SEC }, fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CBuildingW::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBuildingW::Render()
{
	//__super::LightOn();
	
	/**************************************************************/
	m_pShaderCom->Begin(CShader::COLOR);
	if (FAILED(__super::Render()))
		return E_FAIL;
	m_pShaderCom->End();
	return S_OK;
	/**************************************************************/

	//return __super::Render();
}

CBuildingW* CBuildingW::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuildingW* pInstance = new CBuildingW(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBuildingW");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuildingW::Clone(void* pArg)
{
	CBuildingW* pInstance = new CBuildingW(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBuildingW");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuildingW::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
}

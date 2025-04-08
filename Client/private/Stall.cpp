#include "Stall.h"
#include "GameInstance.h"

CStall::CStall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CStall::CStall(const CStall& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CStall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStall::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Stall");
	m_szBufferType = TEXT("Stall");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CStall::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CStall::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CStall::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CStall::Render()
{
	MultiTextureShaderRender(2);
	return S_OK;
}

CStall* CStall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStall* pInstance = new CStall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStall::Clone(void* pArg)
{
	CStall* pInstance = new CStall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CStall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStall::Free()
{
	__super::Free();
}

#include "Trapezoid.h"
#include "GameInstance.h"

CTrapezoid::CTrapezoid(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CTrapezoid::CTrapezoid(const CTrapezoid& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CTrapezoid::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrapezoid::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Trapezoid");
	m_szBufferType = TEXT("Trapezoid");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	return S_OK;
}

void CTrapezoid::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CTrapezoid::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CTrapezoid::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTrapezoid::Render()
{
	return __super::Render();
}

HRESULT CTrapezoid::Render_Rect()
{
	return E_NOTIMPL;
}

CTrapezoid* CTrapezoid::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrapezoid* pInstance = new CTrapezoid(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrapezoid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrapezoid::Clone(void* pArg)
{
	CTrapezoid* pInstance = new CTrapezoid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTrapezoid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrapezoid::Free()
{
	__super::Free();
}

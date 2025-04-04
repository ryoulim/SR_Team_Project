// 내 클래스 이름 : TelephonePole
// 부모 클래스 이름 : Map

#include "TelephonePole.h"
#include "GameInstance.h"

CTelephonePole::CTelephonePole(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CTelephonePole::CTelephonePole(const CTelephonePole& Prototype)
	: CMap(Prototype)
{
}

HRESULT CTelephonePole::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTelephonePole::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTelephonePole::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTelephonePole::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CTelephonePole::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTelephonePole::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CTelephonePole* CTelephonePole::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTelephonePole* pInstance = new CTelephonePole(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTelephonePole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTelephonePole::Clone(void* pArg)
{
	CTelephonePole* pInstance = new CTelephonePole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTelephonePole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTelephonePole::Free()
{
	__super::Free();
}
// 내 클래스 이름 : Ladder
// 부모 클래스 이름 : Map

#include "Ladder.h"
#include "GameInstance.h"

CLadder::CLadder(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CLadder::CLadder(const CLadder& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CLadder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLadder::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLadder::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLadder::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CLadder::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLadder::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CLadder* CLadder::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLadder* pInstance = new CLadder(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLadder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLadder::Clone(void* pArg)
{
	CLadder* pInstance = new CLadder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLadder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadder::Free()
{
	__super::Free();
}
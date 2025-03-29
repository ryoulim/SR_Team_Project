// 내 클래스 이름 : AlphaRect
// 부모 클래스 이름 : Map

#include "AlphaRect.h"
#include "GameInstance.h"

CAlphaRect::CAlphaRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CAlphaRect::CAlphaRect(const CAlphaRect& Prototype)
	: CMap(Prototype)
{
}

HRESULT CAlphaRect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphaRect::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eNextLevel;
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAlphaRect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CAlphaRect::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CAlphaRect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAlphaRect::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CAlphaRect* CAlphaRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAlphaRect* pInstance = new CAlphaRect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAlphaRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAlphaRect::Clone(void* pArg)
{
	CAlphaRect* pInstance = new CAlphaRect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CAlphaRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAlphaRect::Free()
{
	__super::Free();
}
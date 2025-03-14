// 내 클래스 이름 : Font_ItemDialog
// 부모 클래스 이름 : Font

#include "Font_TinyBlue.h"
#include "GameInstance.h"

CFont_TinyBlue::CFont_TinyBlue(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CFont{ pGraphic_Device }
{
}

CFont_TinyBlue::CFont_TinyBlue(const CFont_TinyBlue& Prototype)
	: CFont(Prototype)
{
}

HRESULT CFont_TinyBlue::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFont_TinyBlue::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Font_TinyBlue");
	//m_szBufferType = TEXT("Rect");
	m_eFontType = TINYBLUE;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFont_TinyBlue::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFont_TinyBlue::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFont_TinyBlue::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont_TinyBlue::Render()
{
	return __super::Render();
}

CFont_TinyBlue* CFont_TinyBlue::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFont_TinyBlue* pInstance = new CFont_TinyBlue(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFont_TinyBlue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFont_TinyBlue::Clone(void* pArg)
{
	CFont_TinyBlue* pInstance = new CFont_TinyBlue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFont_TinyBlue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont_TinyBlue::Free()
{
	__super::Free();
}
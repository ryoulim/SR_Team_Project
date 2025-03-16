// 내 클래스 이름 : Font_ItemDialog
// 부모 클래스 이름 : Font

#include "Font_MediumBlue.h"
#include "GameInstance.h"

CFont_MediumBlue::CFont_MediumBlue(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CFont{ pGraphic_Device }
{
}

CFont_MediumBlue::CFont_MediumBlue(const CFont_MediumBlue& Prototype)
	: CFont(Prototype)
{
}

HRESULT CFont_MediumBlue::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFont_MediumBlue::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Font_MediumBlue");
	//m_szBufferType = TEXT("Rect");
	m_eFontType = MEDIUMBLUE;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFont_MediumBlue::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFont_MediumBlue::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFont_MediumBlue::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont_MediumBlue::Render()
{
	return __super::Render();
}

CFont_MediumBlue* CFont_MediumBlue::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFont_MediumBlue* pInstance = new CFont_MediumBlue(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFont_MediumBlue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFont_MediumBlue::Clone(void* pArg)
{
	CFont_MediumBlue* pInstance = new CFont_MediumBlue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFont_MediumBlue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont_MediumBlue::Free()
{
	__super::Free();
}
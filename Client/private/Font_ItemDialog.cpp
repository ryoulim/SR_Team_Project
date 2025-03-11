// 내 클래스 이름 : Font_ItemDialog
// 부모 클래스 이름 : Font

#include "Font_ItemDialog.h"
#include "GameInstance.h"

CFont_ItemDialog::CFont_ItemDialog(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CFont{ pGraphic_Device }
{
}

CFont_ItemDialog::CFont_ItemDialog(const CFont_ItemDialog& Prototype)
	: CFont(Prototype)
{
}

HRESULT CFont_ItemDialog::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFont_ItemDialog::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Font_ItemDialog");
	//m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFont_ItemDialog::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CFont_ItemDialog::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CFont_ItemDialog::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont_ItemDialog::Render()
{
	return __super::Render();
}

CFont_ItemDialog* CFont_ItemDialog::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFont_ItemDialog* pInstance = new CFont_ItemDialog(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFont_ItemDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFont_ItemDialog::Clone(void* pArg)
{
	CFont_ItemDialog* pInstance = new CFont_ItemDialog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFont_ItemDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont_ItemDialog::Free()
{
	__super::Free();
}
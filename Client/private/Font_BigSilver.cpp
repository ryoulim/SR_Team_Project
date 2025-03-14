// 내 클래스 이름 : Font_ItemDialog
// 부모 클래스 이름 : Font

#include "Font_BigSilver.h"
#include "GameInstance.h"

CFont_BigSilver::CFont_BigSilver(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CFont{ pGraphic_Device }
{
}

CFont_BigSilver::CFont_BigSilver(const CFont_BigSilver& Prototype)
	: CFont(Prototype)
{
}

HRESULT CFont_BigSilver::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFont_BigSilver::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Font_BigSilver");
	//m_szBufferType = TEXT("Rect");
	m_eFontType = BIGSILVER;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFont_BigSilver::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFont_BigSilver::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFont_BigSilver::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont_BigSilver::Render()
{
	return __super::Render();
}

CFont_BigSilver* CFont_BigSilver::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFont_BigSilver* pInstance = new CFont_BigSilver(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFont_BigSilver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFont_BigSilver::Clone(void* pArg)
{
	CFont_BigSilver* pInstance = new CFont_BigSilver(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFont_BigSilver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont_BigSilver::Free()
{
	__super::Free();
}
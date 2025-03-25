// 내 클래스 이름 : AnimeRect
// 부모 클래스 이름 : BackGround

#include "AnimeRect.h"
#include "GameInstance.h"

CAnimeRect::CAnimeRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBackGround{ pGraphic_Device }
{
}

CAnimeRect::CAnimeRect(const CAnimeRect& Prototype)
	: CBackGround(Prototype)
{
}

HRESULT CAnimeRect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimeRect::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAnimeRect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CAnimeRect::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CAnimeRect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAnimeRect::Render()
{
	return __super::Render();
}

CAnimeRect* CAnimeRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimeRect* pInstance = new CAnimeRect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAnimeRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAnimeRect::Clone(void* pArg)
{
	CAnimeRect* pInstance = new CAnimeRect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CAnimeRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimeRect::Free()
{
	__super::Free();
}
// 내 클래스 이름 : AnimeRect
// 부모 클래스 이름 : Map

#include "AnimeRect.h"
#include "GameInstance.h"

CAnimeRect::CAnimeRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CAnimeRect::CAnimeRect(const CAnimeRect& Prototype)
	: CMap(Prototype),
	  m_fMaxFrame {Prototype.m_fMaxFrame}
{
}

HRESULT CAnimeRect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimeRect::Initialize(void* pArg)
{

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

void CAnimeRect::Move_Frame(_float fTimeDelta)
{
	m_fTextureIdx += m_fMaxFrame * fTimeDelta;

	if (m_fTextureIdx >= m_fMaxFrame)
		m_fTextureIdx = 0.f;
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
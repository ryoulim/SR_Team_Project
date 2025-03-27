// 내 클래스 이름 : Lava
// 부모 클래스 이름 : AnimeRect

#include "Lava.h"
#include "GameInstance.h"

CLava::CLava(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAnimeRect{ pGraphic_Device }
{
}

CLava::CLava(const CLava& Prototype)
	: CAnimeRect(Prototype)
{
}

HRESULT CLava::Initialize_Prototype()
{
	m_fMaxFrame = 16.f;

	return S_OK;
}

HRESULT CLava::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Lava");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLava::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLava::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CLava::Late_Update(_float fTimeDelta)
{
	Move_Frame(fTimeDelta);
	__super::Late_Update(fTimeDelta);
}

HRESULT CLava::Render()
{
	return __super::Render();
}

CLava* CLava::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLava* pInstance = new CLava(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLava");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLava::Clone(void* pArg)
{
	CLava* pInstance = new CLava(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLava");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLava::Free()
{
	__super::Free();
}
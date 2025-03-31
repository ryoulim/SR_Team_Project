// 내 클래스 이름 : Water
// 부모 클래스 이름 : AnimeRect

#include "Water.h"
#include "GameInstance.h"

CWater::CWater(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAnimeRect{ pGraphic_Device }
{
}

CWater::CWater(const CWater& Prototype)
	: CAnimeRect(Prototype)
{
}

HRESULT CWater::Initialize_Prototype()
{
	m_fMaxFrame = 16.f;
	return S_OK;
}

HRESULT CWater::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Water");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWater::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWater::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CWater::Late_Update(_float fTimeDelta)
{
	Move_Frame(fTimeDelta);
	__super::Late_Update(fTimeDelta);
}

HRESULT CWater::Render()
{
	return __super::Render();
}

CWater* CWater::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWater* pInstance = new CWater(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWater::Clone(void* pArg)
{
	CWater* pInstance = new CWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();
}
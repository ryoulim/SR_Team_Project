// 내 클래스 이름 : Loading_ToIn
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_ToIn.h"
#include "GameInstance.h"

CLoading_ToIn::CLoading_ToIn(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_ToIn::CLoading_ToIn(const CLoading_ToIn& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_ToIn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_ToIn::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_ToIn");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLoading_ToIn::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_ToIn::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CLoading_ToIn::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLoading_ToIn::Render()
{
	return __super::Render();
}

CLoading_ToIn* CLoading_ToIn::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_ToIn* pInstance = new CLoading_ToIn(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_ToIn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_ToIn::Clone(void* pArg)
{
	CLoading_ToIn* pInstance = new CLoading_ToIn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_ToIn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_ToIn::Free()
{
	__super::Free();
}
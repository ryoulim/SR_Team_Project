// 내 클래스 이름 : Loading_ToIn
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_ToIn.h"
#include "GameInstance.h"
#include "UI_Manager.h"

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
	// Ready_ShaderComponent(); 필요할 시 추가

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

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
	Render_Background();
	return S_OK;
}

HRESULT CLoading_ToIn::Render_Background()
{
	if (FAILED(m_pTextureCom->Bind_Resource(_int(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
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
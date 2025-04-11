// 내 클래스 이름 : Loading_ToOut
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_ToOut.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_ToOut::CLoading_ToOut(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_ToOut::CLoading_ToOut(const CLoading_ToOut& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_ToOut::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_ToOut::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_ToOut");
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


void CLoading_ToOut::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_ToOut::Update(_float fTimeDelta)
{
	//if (m_fLoadingGauge < m_fCurLoadingGauge)		// 실제 로딩 게이지 비례해 진행할 경우 사용
	//	m_fLoadingGauge += fTimeDelta * 0.8f;

	return __super::Update(fTimeDelta);
}

void CLoading_ToOut::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLoading_ToOut::Render()
{
	Render_Background();
	return S_OK;
}

HRESULT CLoading_ToOut::Render_Background()
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

CLoading_ToOut* CLoading_ToOut::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_ToOut* pInstance = new CLoading_ToOut(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_ToOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_ToOut::Clone(void* pArg)
{
	CLoading_ToOut* pInstance = new CLoading_ToOut(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_ToOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_ToOut::Free()
{
	__super::Free();
}
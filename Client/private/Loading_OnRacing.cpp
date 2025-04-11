// 내 클래스 이름 : Loading_OnRacing
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_OnRacing.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_OnRacing::CLoading_OnRacing(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_OnRacing::CLoading_OnRacing(const CLoading_OnRacing& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_OnRacing::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_OnRacing::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_OnRacing");
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


void CLoading_OnRacing::Priority_Update(_float fTimeDelta)
{
	if (m_fCurLoadingGauge >= 1.f)
	{
		m_isLoadingFinished = true;
		m_isReadyToChangeLevel = true;
	}
	//__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_OnRacing::Update(_float fTimeDelta)
{
	//if (m_fLoadingGauge < m_fCurLoadingGauge)		// 실제 로딩 게이지 비례해 진행할 경우 사용
	//	m_fLoadingGauge += fTimeDelta * 0.8f;

	return __super::Update(fTimeDelta);
}

void CLoading_OnRacing::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLoading_OnRacing::Render()
{
	Render_Background();
	return S_OK;
}

HRESULT CLoading_OnRacing::Render_Background()
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

CLoading_OnRacing* CLoading_OnRacing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_OnRacing* pInstance = new CLoading_OnRacing(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_OnRacing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_OnRacing::Clone(void* pArg)
{
	CLoading_OnRacing* pInstance = new CLoading_OnRacing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_OnRacing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_OnRacing::Free()
{
	__super::Free();
}
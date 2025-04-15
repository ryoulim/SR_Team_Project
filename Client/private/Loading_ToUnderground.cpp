// 내 클래스 이름 : Loading_ToUnderground
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_ToUnderground.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_ToUnderground::CLoading_ToUnderground(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_ToUnderground::CLoading_ToUnderground(const CLoading_ToUnderground& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_ToUnderground::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_ToUnderground::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_ToUnderground");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	// Ready_ShaderComponent(); 필요할 시 추가

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Loading_ToUnderground"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	m_pSoundCom->SetVolume("Wind_LowLoop2", 0.3f);
	m_pSoundCom->Play("Wind_LowLoop2");

	return S_OK;
}


void CLoading_ToUnderground::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_ToUnderground::Update(_float fTimeDelta)
{
	//if (m_fLoadingGauge < m_fCurLoadingGauge)		// 실제 로딩 게이지 비례해 진행할 경우 사용
	//	m_fLoadingGauge += fTimeDelta * 0.8f;

	return __super::Update(fTimeDelta);
}

void CLoading_ToUnderground::Late_Update(_float fTimeDelta)
{
	if (m_fCurLoadingGauge >= 1.f)
	{
		m_isLoadingFinished = true;
	}
	if (m_isLoadingFinished)
	{
		m_fAnimationFrame += fTimeDelta * 1.f;
		if (m_fAnimationFrame >= 3.f)
		{
			m_fAnimationFrame = 2.f;
			m_isReadyToChangeLevel = true;
		}
	}
	CUI::Late_Update(fTimeDelta);
}

HRESULT CLoading_ToUnderground::Render()
{
	Render_Background();
	return S_OK;
}

HRESULT CLoading_ToUnderground::Render_Background()
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

CLoading_ToUnderground* CLoading_ToUnderground::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_ToUnderground* pInstance = new CLoading_ToUnderground(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_ToUnderground");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_ToUnderground::Clone(void* pArg)
{
	CLoading_ToUnderground* pInstance = new CLoading_ToUnderground(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_ToUnderground");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_ToUnderground::Free()
{
	__super::Free();
}
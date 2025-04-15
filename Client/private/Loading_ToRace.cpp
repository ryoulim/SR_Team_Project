// 내 클래스 이름 : Loading_ToRace
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_ToRace.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_ToRace::CLoading_ToRace(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_ToRace::CLoading_ToRace(const CLoading_ToRace& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_ToRace::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_ToRace::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_ToRace");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	// Ready_ShaderComponent(); 필요할 시 추가

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

	m_fDepth = UI_PRIORITY;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Loading_ToRace"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	m_pSoundCom->Play("FanLoop02");
	m_pSoundCom->SetVolume("FanLoop02", 0.4f);
	m_pSoundCom->Play("shelly_007_that_isnt_ominous_alt1");
	m_pSoundCom->SetVolume("shelly_007_that_isnt_ominous_alt1", 0.4f);
	m_pSoundCom->SetVolume("vehicle_start", 0.5f);

	return S_OK;
}


void CLoading_ToRace::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_ToRace::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CLoading_ToRace::Late_Update(_float fTimeDelta)
{
	m_fTextureNum += fTimeDelta * 0.25f;
	if (m_fTextureNum >= 1.f && m_bVehicleSoundTrigger == false)
	{
		m_pSoundCom->Play("vehicle_start");
		m_bVehicleSoundTrigger = true;
	}
	if (m_fTextureNum > 2.f)
	{
		m_fTextureNum = 1.f;
		m_isReadyToChangeLevel = true;
	}
	CUI::Late_Update(fTimeDelta);
}

HRESULT CLoading_ToRace::Render()
{
	Render_Background();
	return S_OK;
}

HRESULT CLoading_ToRace::Render_Background()
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

CLoading_ToRace* CLoading_ToRace::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_ToRace* pInstance = new CLoading_ToRace(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_ToRace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_ToRace::Clone(void* pArg)
{
	CLoading_ToRace* pInstance = new CLoading_ToRace(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_ToRace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_ToRace::Free()
{
	__super::Free();
}
// 내 클래스 이름 : Loading_ToBoss
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_ToBoss.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_ToBoss::CLoading_ToBoss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_ToBoss::CLoading_ToBoss(const CLoading_ToBoss& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_ToBoss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_ToBoss::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_ToBoss");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	// Ready_ShaderComponent(); 필요할 시 추가

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);


	m_fAnimationStartFrame = TEX_SMALLNOISE;
	m_fAnimationMaxFrame = TEX_SMALLNOISE_END;
	m_fAnimationSpeed = 35.f;
	m_iTexRepeat = 2;

	return S_OK;
}

void CLoading_ToBoss::Priority_Update(_float fTimeDelta)
{
	if (m_bNextSequence)
	{
		m_bNextSequence = false;
		Init_Sequence();
	}
	if (KEY_DOWN(DIK_C))
		int a = 0;
	Update_Animation(fTimeDelta);
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_ToBoss::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CLoading_ToBoss::Late_Update(_float fTimeDelta)
{
	CUI::Late_Update(fTimeDelta);
}

HRESULT CLoading_ToBoss::Render()
{
	Render_Background();
	
	return S_OK;
}

HRESULT CLoading_ToBoss::Render_Background()
{
	if (FAILED(m_pTextureCom->Bind_Resource(_int(m_fAnimationFrame))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
}

void CLoading_ToBoss::Init_Sequence()
{
	switch (m_eSequence)
	{
	case Client::CLoading_ToBoss::SEQUENCE_SMALLNOISE:
		m_fAnimationStartFrame = TEX_SMALLNOISE;
		m_fAnimationFrame = m_fAnimationStartFrame;
		m_fAnimationMaxFrame = TEX_SMALLNOISE_END;
		m_fAnimationSpeed = 30.f;
		m_iTexRepeat = 2;
		break;

	case Client::CLoading_ToBoss::SEQUENCE_SMALLBREAKINGNEWS:
		m_fAnimationStartFrame = TEX_SMALLBREAKINGNEWS;
		m_fAnimationFrame = m_fAnimationStartFrame;
		m_fAnimationMaxFrame = TEX_SMALLBREAKINGNEWS;
		m_fAnimationSpeed = 1.f;
		m_iTexRepeat = 3;
		break;

	case Client::CLoading_ToBoss::SEQUENCE_SMALLNEWS:
		m_fAnimationStartFrame = TEX_SMALLNEWS;
		m_fAnimationFrame = m_fAnimationStartFrame;
		m_fAnimationMaxFrame = TEX_SMALLNEWS_END;
		m_fAnimationSpeed = 3.f;
		m_iTexRepeat = 3;
		break;

	case Client::CLoading_ToBoss::SEQUENCE_WIDENOISE:
		m_fAnimationStartFrame = TEX_WIDENOISE;
		m_fAnimationFrame = m_fAnimationStartFrame;
		m_fAnimationMaxFrame = TEX_WIDENOISE_END;
		m_fAnimationSpeed = 30.f;
		m_iTexRepeat = 2;
		break;

	case Client::CLoading_ToBoss::SEQUENCE_WIDEBOSS:
		m_fAnimationStartFrame = TEX_WIDEBOSS;
		m_fAnimationFrame = m_fAnimationStartFrame;
		m_fAnimationMaxFrame = TEX_WIDEBOSS;
		m_fAnimationSpeed = 1.f;
		m_iTexRepeat = 3;
		break;

	case Client::CLoading_ToBoss::SEQUENCE_WIDEREDBOSS:
		m_fAnimationStartFrame = TEX_WIDEREDBOSS;
		m_fAnimationFrame = m_fAnimationStartFrame;
		m_fAnimationMaxFrame = TEX_WIDEREDBOSS;
		m_fAnimationSpeed = 1.f;
		m_iTexRepeat = 3;
		break;

	case Client::CLoading_ToBoss::SEQUENCE_END:
		m_fAnimationStartFrame = TEX_BREAK_END;
		m_fAnimationFrame = m_fAnimationStartFrame;
		m_fAnimationMaxFrame = TEX_BREAK_END;
		m_fAnimationSpeed = 1.f;
		m_iTexRepeat = 2;
		break;

	default:
		break;
	}
}

void CLoading_ToBoss::Update_Animation(_float fTimeDelta)
{
	m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
	if (m_fAnimationFrame > m_fAnimationMaxFrame + 1)
	{
		--m_iTexRepeat;
		m_fAnimationFrame = m_fAnimationStartFrame;
		if (m_iTexRepeat < 0)
		{
			if (m_eSequence == SEQUENCE_END)
			{
				m_isReadyToChangeLevel = true;
				return;
			}
			m_eSequence = static_cast<SEQUENCE>(m_eSequence + 1);
			m_bNextSequence = true;
		}
	}
}

CLoading_ToBoss* CLoading_ToBoss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_ToBoss* pInstance = new CLoading_ToBoss(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_ToBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_ToBoss::Clone(void* pArg)
{
	CLoading_ToBoss* pInstance = new CLoading_ToBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_ToBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_ToBoss::Free()
{
	__super::Free();
}
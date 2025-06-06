﻿// 내 클래스 이름 : Portrait
// 부모 클래스 이름 : UI

#include "Portrait.h"
#include "GameInstance.h"
#include "UI_Manager.h"


CPortrait::CPortrait(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CPortrait::CPortrait(const CPortrait& Prototype)
	: CUI(Prototype)
{
}

HRESULT CPortrait::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortrait::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Portrait");
	m_szBufferType = TEXT("Rect");
	return __super::Initialize(pArg);
}

HRESULT CPortrait::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	return S_OK;
}

void CPortrait::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CPortrait::Update(_float fTimeDelta)
{
	if (m_pPlayerInfo != nullptr)
	{
		m_uiHP = m_pPlayerInfo->iHP;
		if (m_pPlayerInfo->iHP > 80)
			m_eHPStatus = HP100;
		else if (m_uiHP > 40)
			m_eHPStatus = HP80;
		else if (m_uiHP > 25)
			m_eHPStatus = HP40;
		else if (m_uiHP > 10)
			m_eHPStatus = HP25;
		else
			m_eHPStatus = HP10;
	}
	Change_Face(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CPortrait::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_LOGO ||
		m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_LOADING)
		return;
	__super::Late_Update(fTimeDelta);
}

HRESULT CPortrait::Render()
{
	if (m_pPlayerInfo != nullptr)
	{
		RENDER_TEXT_BOL(m_pPlayerInfo->iHP,
			-(g_iWinSizeX / 2.f) + m_vSize.x - 5.f,
			-(g_iWinSizeY / 2.f) + m_vSize.y / 2.f - 3.f, 1.1f);
	}
	return __super::Render();
}

void CPortrait::Change_Face(_float fTimeDelta)
{
	// player->isdamaged? : m_eFace = ANGER; && m_fAnimTick = 0.f;
	m_fAnimTick += m_pGameInstance->Get_TimeDelta(L"Timer_60"); 

	switch (m_eFace)
	{
	case Client::CPortrait::PORTRAIT_IDLE:
		if (m_fAnimTick > 3.f)
		{
			if (m_eHPStatus != HP25 && m_eHPStatus != HP10 && m_eHPStatus != HP0)
				m_eFace = PORTRAIT_LOOKL;
			m_fAnimTick = 0.f;
		}
		break;

	case Client::CPortrait::PORTRAIT_LOOKL:
		if (m_fAnimTick > 0.5f)
		{
			m_fAnimTick = 0.f;
			m_eFace = PORTRAIT_LOOKR;
		}
		break;

	case Client::CPortrait::PORTRAIT_LOOKR:
		if (m_fAnimTick > 0.5f)
		{
			m_fAnimTick = 0.f;
			m_eFace = PORTRAIT_IDLE;
		}
		break;

	case Client::CPortrait::PORTRAIT_ANGER:
		if (m_fAnimTick > 1.0f)
		{
			m_eFace = PORTRAIT_IDLE;
			m_fAnimTick = 0.f;
		}
		break;

	case Client::CPortrait::PORTRAIT_SMILE:
		// until voice ends
		if (m_fAnimTick > 1.0f)
		{
			m_eFace = PORTRAIT_IDLE;
			m_fAnimTick = 0.f;
		}
		break;
	case Client::CPortrait::PORTRAIT_DEAD:
		break;
	case Client::CPortrait::PORTRAIT_HYPER:
		if (m_fAnimTick > 1.f)
		{
			m_eFace = PORTRAIT_IDLE;
			m_fAnimTick = 0.f;
		}
		break;
	default:
		break;
	}

	m_fTextureNum = static_cast<_float>(m_eHPStatus + m_eFace);
	if (m_eFace == PORTRAIT_HYPER)
		m_fTextureNum = static_cast<_float>(PORTRAIT_HYPER);
}

CPortrait* CPortrait::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPortrait* pInstance = new CPortrait(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CPortrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPortrait::Clone(void* pArg)
{
	CPortrait* pInstance = new CPortrait(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPortrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortrait::Free()
{
	__super::Free();
}
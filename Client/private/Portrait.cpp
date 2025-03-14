// 내 클래스 이름 : Portrait
// 부모 클래스 이름 : UI

#include "Portrait.h"
#include "GameInstance.h"

#define RENDER_ITEMDIALOG(message, fposY) Render_Text(message, CFont::MEDIUMBLUE, CFont::LEFT, -(g_iWinSizeX / 2.f) + 20.f, fposY)

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

	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Portrait");
	m_szBufferType = TEXT("Rect");

	m_pFont = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_ItemDialog")));
	if (nullptr == m_pFont)
		return E_FAIL;



	DESC Desc{};
	Desc.vScale = _float3(80.f, 80.f, 1.f); // 고정
	Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + Desc.vScale.x / 2.f - 10.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 1.f);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CPortrait::Priority_Update(_float fTimeDelta)
{

#pragma region 테스트용
	if (GetKeyState('1') & 0x8000)
	{
		m_eFace = PORTRAIT_ANGER;
		m_fAnimTick = 0.f;
	}
	if (GetKeyState('2') & 0x8000)
	{
		m_eFace = PORTRAIT_SMILE;
		m_fAnimTick = 0.f;
	}
	if (GetKeyState('3') & 0x8000)
	{
		if (m_eHPStatus == HP100)
			m_eHPStatus = HP80;
		else if (m_eHPStatus == HP80)
			m_eHPStatus = HP40;
		else if (m_eHPStatus == HP40)
			m_eHPStatus = HP25;
		else if (m_eHPStatus == HP25)
			m_eHPStatus = HP10;
		else if (m_eHPStatus == HP10)
			m_eHPStatus = HP100;

		m_fAnimTick = 0.f;
	}
#pragma endregion

	__super::Priority_Update(fTimeDelta);
}

EVENT CPortrait::Update(_float fTimeDelta)
{
	// Player->GetHP() : m_eHPStatus Set

	Change_Face(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CPortrait::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPortrait::Render()
{
	
	m_pFont->RENDER_ITEMDIALOG("armor fragment x4", g_iWinSizeY / 2.f - 20.f);
	return __super::Render();
}

void CPortrait::Change_Face(_float fTimeDelta)
{
	// player->isdamaged? : m_eFace = ANGER; && m_fAnimTick = 0.f;
	m_fAnimTick += fTimeDelta;


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
		if (m_fAnimTick > 1.5f)
		{
			m_eFace = PORTRAIT_IDLE;
			m_fAnimTick = 0.f;
		}
		break;

	case Client::CPortrait::PORTRAIT_SMILE:
		// until voice ends
		if (m_fAnimTick > 1.5f)
		{
			m_eFace = PORTRAIT_IDLE;
			m_fAnimTick = 0.f;
		}
		break;
	case Client::CPortrait::PORTRAIT_DEAD:
		break;
	default:
		break;
	}

	m_fTextureNum = static_cast<_float>(m_eHPStatus + m_eFace);
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
	Safe_Release(m_pFont);
}
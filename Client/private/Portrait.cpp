// 내 클래스 이름 : Portrait
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

	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Portrait");
	m_szBufferType = TEXT("Rect");

	//m_pFont = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_MediumBlue")));
	//if (nullptr == m_pFont)
	//	return E_FAIL;



	DESC Desc{};
	Desc.vScale = _float3(80.f, 80.f, 1.f); // 고정
	Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + Desc.vScale.x / 2.f - 10.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 1.f);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;
	m_uiHP = 100;
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
	if (GetKeyState('1') & 0x8000)
		m_uiHP++;
	
	if (GetKeyState('2')&0x8000)
	{
		if (m_uiHP != 0)
			m_uiHP--;
	}
	//HP100 = 0,
	//	HP80 = 5,
	//	HP40 = 10,
	//	HP25 = 15,
	//	HP10 = 18,
	//	HP0 = 21,
	if (m_uiHP > 80)
		m_eHPStatus = HP100;
	else if (m_uiHP > 40)
		m_eHPStatus = HP80;
	else if (m_uiHP > 25)
		m_eHPStatus = HP40;
	else if (m_uiHP > 10)
		m_eHPStatus = HP25;
	else
		m_eHPStatus = HP10;
	Change_Face(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CPortrait::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPortrait::Render()
{
	//CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text("armor fragment x4", CFont::LEFT, CFont::MEDIUMBLUE, -(g_iWinSizeX / 2.f) + 20.f, g_iWinSizeY / 2.f - 20.f);
	CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text(
		"Hello World!!",
		CFont::MEDIUMBLUE, 
		CFont::LEFT, 
		(float)(-(g_iWinSizeX / 2.f) + 20.f),
		(float)(g_iWinSizeY / 2.f - 20.f));

	RENDER_TEXT_BOL(m_uiHP, 
		-(g_iWinSizeX / 2.f) + m_vSize.x-5.f,
		-(g_iWinSizeY / 2.f) + m_vSize.y / 2.f - 3.f, 1.1f);
	
	
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
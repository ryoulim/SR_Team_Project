// 내 클래스 이름 : Loading_Ending
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_Ending.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_Ending::CLoading_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_Ending::CLoading_Ending(const CLoading_Ending& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_Ending::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_Ending::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_Ending");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;

	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

	Ready_ShaderComponent();
	for (size_t i = 0; i < 6; i++)
		m_bSoundTrigger[i] = false;
	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Loading_Ending"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	m_pSoundCom->SetVolume("seekandhide", 0.9f);
	m_pSoundCom->SetVolume("fire_loop_002_0", 0.2f);
	m_pSoundCom->SetVolume("switch_poweroff_1", 0.5f);
	m_pSoundCom->SetVolume("lever_heavy01_2", 0.5f);
	m_pSoundCom->SetVolume("bigsmash2_3", 0.4f);
	m_pSoundCom->SetVolume("shot_003_4", 0.8f);
	m_pSoundCom->SetVolume("aircraft_missile_5", 0.5f);
	m_pSoundCom->Play("seekandhide");
	m_pSoundCom->Play("fire_loop_002_0");

	m_fOpacity = 1.f;
	m_fDepth = UI_PRIORITY;

	return S_OK;
}


void CLoading_Ending::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_Ending::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RETURN))
	{
		m_fTextureNum += 1.f;
		m_fOpacity = 0.f;
		if (m_fTextureNum > 8.f)
		{
			m_fTextureNum = 8.f;
			m_fOpacity = 1.f;
		}
	}

	if (KEY_DOWN(DIK_SPACE))
	{
		if (m_fTextureNum < 8.f)
		{
			m_fTextureNum = 8.f;
			m_fOpacity = 1.f;
			// 게임 종료?
		}
		if (m_fTextureNum > 7.f)
			PostQuitMessage(0);

	}
	m_fOpacity += fTimeDelta * 3.f;
	if (m_fOpacity > 1.f)
		m_fOpacity = 1.f;
	return CUI::Update(fTimeDelta);
}

void CLoading_Ending::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < 6; i++)
	{
		if (m_bSoundTrigger[i] == false && _uint(m_fTextureNum) == i)
		{
			switch (i)
			{
			case 1:
				m_pSoundCom->Play("switch_poweroff_1");
				break;
			case 2:
				m_pSoundCom->Play("lever_heavy01_2");
				break;
			case 3:
				m_pSoundCom->Play("bigsmash2_3");
				break;
			case 4:
				m_pSoundCom->Play("shot_003_4");
				break;
			case 5:
				m_pSoundCom->Play("aircraft_missile_5");
				break;
			default:
				break;
			}
			m_bSoundTrigger[i] = true;
		}
	}
	CUI::Late_Update(fTimeDelta);
}

HRESULT CLoading_Ending::Render()
{
	Render_Background();
	CUI_Manager::Get_Instance()->Render_Text("Press Enter ...", CFont::MEDIUMBLUE, CFont::CENTER, 0.f, g_iWinSizeY * -0.5f + 30.f, 0.8f);
	return S_OK;
}

HRESULT CLoading_Ending::Render_Background()
{
	if (_int(m_fTextureNum) - 1 >= 0)
	{
 		if (FAILED(m_pTextureCom->Bind_Resource(_int(m_fTextureNum) - 1)))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;
		
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", static_cast<_uint>(m_fTextureNum));
	m_pShaderCom->SetFloat("opacity", m_fOpacity);
	m_pShaderCom->Begin(CShader::ALPHA);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	m_pShaderCom->End();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}

CLoading_Ending* CLoading_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_Ending* pInstance = new CLoading_Ending(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_Ending::Clone(void* pArg)
{
	CLoading_Ending* pInstance = new CLoading_Ending(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Ending::Free()
{
	__super::Free();
}
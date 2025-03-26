// 내 클래스 이름 : FadeUI
// 부모 클래스 이름 : UI

#include "FadeUI.h"
#include "GameInstance.h"

CFadeUI::CFadeUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CFadeUI::CFadeUI(const CFadeUI& Prototype)
	: CUI(Prototype)
{
}

HRESULT CFadeUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFadeUI::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("FadeUI");
	m_szBufferType = TEXT("Rect");

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		if (pDesc->m_isFadeIn)
		{
			m_isFadeIn = pDesc->m_isFadeIn;	
			m_fFadeOpacity = 1.f;
		}
		else
		{
			m_isFadeOut = true;
			m_fFadeOpacity = 0.f;
		}
	}
	m_isRenderOn = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	__super::Ready_ShaderComponent();

	m_fDepth = 0.f;

	m_fAnimationFrame = 0.f;

	return S_OK;
}

void CFadeUI::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFadeUI::Update(_float fTimeDelta)
{
	if (m_isRenderOn)
	{
		m_fFadeOpacity += fTimeDelta * 5.f* (m_isFadeIn?-1.f:1.f);
		if (m_fFadeOpacity > 1.f || m_fFadeOpacity < 0.f)
		{
			//m_isRenderOn = false;
			/*CGameObject* res = { nullptr };
			m_pGameInstance->Detach_GameObject(&res, LEVEL_STATIC, TEXT("Layer_FadeUI"));
			if (res != this)
				return EVN_END;*/
			m_fAnimationFrame += fTimeDelta;
		}
	}
	if (m_fAnimationFrame > 0.7f)
		return EVN_DEAD;
	return __super::Update(fTimeDelta);
}

void CFadeUI::Late_Update(_float fTimeDelta)
{
	if (m_isRenderOn)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
			return;
	}
}

HRESULT CFadeUI::Render()
{
 	if (FAILED(m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", 0)))
		return E_FAIL;
	m_pShaderCom->SetFloat("opacity", m_fFadeOpacity);
 	m_pShaderCom->Begin(CShader::ALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (FAILED(__super::Render()))
		return E_FAIL;
	m_pShaderCom->End();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CFadeUI::Fade_In()
{
	m_isRenderOn = true;
	m_isFadeIn = true;
	m_fFadeOpacity = 1.f;
	//m_pGameInstance->Add_GameObject_Existing(this, LEVEL_STATIC, TEXT("Layer_FadeUI"));
}

void CFadeUI::Fade_Out()
{
	m_isRenderOn = true;
	m_isFadeOut = true;
	m_fFadeOpacity = 0.f;
	//m_pGameInstance->Add_GameObject_Existing(this, LEVEL_STATIC, TEXT("Layer_FadeUI"));
}

CFadeUI* CFadeUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFadeUI* pInstance = new CFadeUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFadeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFadeUI::Clone(void* pArg)
{
	CFadeUI* pInstance = new CFadeUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFadeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFadeUI::Free()
{
	__super::Free();
}
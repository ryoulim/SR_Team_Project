// 내 클래스 이름 : Loading_ToMain
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_ToMain.h"
#include "GameInstance.h"
#include <UI_Manager.h>

CLoading_ToMain::CLoading_ToMain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_ToMain::CLoading_ToMain(const CLoading_ToMain& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_ToMain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_ToMain::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_ToMain");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	Ready_ShaderComponent();
	Initialize_LoadingAnims();
	if (FAILED(m_pTextureCom->Get_TextureSize(10, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);
	m_pAnimationTransformCom->Set_State(CTransform::STATE_POSITION, { (g_iWinSizeX * 0.5f - 130.f), -(g_iWinSizeY * 0.5f) + 130.f, 0.1f });
	m_pAnimationTransformCom->Scaling({ 128.f, 128.f, 1.f });
	m_pBarTransformCom->Set_State(CTransform::STATE_POSITION, { (g_iWinSizeX * 0.5f - 130.f), -(g_iWinSizeY * 0.5f) + 50.f, 0.1f });


	return S_OK;
}

HRESULT CLoading_ToMain::Initialize_LoadingAnims()
{
	CTransform::DESC pArg{};

	pArg.fRotationPerSec = 1.f;
	pArg.fSpeedPerSec = 1.f;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Anim"), reinterpret_cast<CComponent**>(&m_pAnimationTransformCom), &pArg)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Bar"), reinterpret_cast<CComponent**>(&m_pBarTransformCom), &pArg)))
		return E_FAIL;
	return S_OK;
}

void CLoading_ToMain::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_ToMain::Update(_float fTimeDelta)
{
	if (m_fTextureNum < 1.f)
		m_fTextureNum += fTimeDelta;
	else
		m_fTextureNum += fTimeDelta * 10;
	if (m_fTextureNum >= _float(TEX_BARBACK))
		m_fTextureNum = _float(TEX_ANIM);

	if (m_fLoadingGauge < m_fCurLoadingGauge)
	{
		m_fLoadingGauge += fTimeDelta * 0.8f;
	}

	return __super::Update(fTimeDelta);
}

void CLoading_ToMain::Late_Update(_float fTimeDelta)
{
	if (m_fCurLoadingGauge >= 1.f)
	{
		m_isLoadingFinished = true;
	}
	if (m_isLoadingFinished)
	{
		m_fFinished_WaitingTime += fTimeDelta;
		if (m_fFinished_WaitingTime >= 1.f)
		{
			m_isReadyToChangeLevel = true;
		}
	}
	CUI::Late_Update(fTimeDelta);
}

HRESULT CLoading_ToMain::Render()
{
	Render_Background();
	Render_LoadingText();
	Render_LoadingBar();
	Render_LoadingAnimation();
	return S_OK;
}

HRESULT CLoading_ToMain::Render_Background()
{
	if (FAILED(m_pTextureCom->Bind_Resource(TEX_BACKGROUND)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoading_ToMain::Render_LoadingAnimation()
{
	if (FAILED(m_pTextureCom->Bind_Resource(_int(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pAnimationTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoading_ToMain::Render_LoadingBar()
{
	/* 로딩 바 뒷부분 */

	m_pBarTransformCom->Scaling({ 180.f, 28.f, 1.f });
	if (FAILED(m_pBarTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_Resource(TEX_BARBACK)))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	/* 로딩 바 */
	m_pBarTransformCom->Scaling({ 126.f, 6.f, 1.f });
	if (FAILED(m_pBarTransformCom->Bind_Resource())) return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", static_cast<_uint>(TEX_BAR));
	m_pShaderCom->SetFloat("LoadPercent", m_fLoadingGauge);
	m_pShaderCom->Begin(CShader::LOADINGMASKING);
	if (FAILED(m_pVIBufferCom->Bind_Buffers())) return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))		return E_FAIL;
	m_pShaderCom->End();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

void CLoading_ToMain::Render_LoadingText()
{
	if (m_fLoadingGauge > 0.2f)
		CUI_Manager::Get_Instance()->Render_Text("Loading ... \"ION FURY\"", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 1));
	if (m_fLoadingGauge > 0.4f)
		CUI_Manager::Get_Instance()->Render_Text("HINT : Press LCtrl to jump higher.", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 2));
	if (m_fLoadingGauge > 0.6f)
		CUI_Manager::Get_Instance()->Render_Text("REAL TIME LOADING!! @>-----", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 3));
	if (m_fLoadingGauge > 0.8f)
		CUI_Manager::Get_Instance()->Render_Text("You can't skip this loading...", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 4));
	if (m_fLoadingGauge > 1.f)
		CUI_Manager::Get_Instance()->Render_Text("Loading Complete !! ", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 5));

}

CLoading_ToMain* CLoading_ToMain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_ToMain* pInstance = new CLoading_ToMain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_ToMain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_ToMain::Clone(void* pArg)
{
	CLoading_ToMain* pInstance = new CLoading_ToMain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_ToMain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_ToMain::Free()
{
	Safe_Release(m_pAnimationTransformCom);
	Safe_Release(m_pBarTransformCom);
	__super::Free();
}
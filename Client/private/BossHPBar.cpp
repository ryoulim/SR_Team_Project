// 내 클래스 이름 : BossHPBar
// 부모 클래스 이름 : UI

#include "BossHPBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define HPBARMAX 191.f

CBossHPBar::CBossHPBar(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CBossHPBar::CBossHPBar(const CBossHPBar& Prototype)
	: CUI(Prototype)
{
}

HRESULT CBossHPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossHPBar::Initialize(void* pArg)
{
	m_szTextureID = TEXT("BossHPBar");
	m_szBufferType = TEXT("Rect");
	m_vBackSize = _float3(270.f, 42.f, 1.f);
	m_vBarSize = _float3(192.f, 12.f, 1.f);
	m_iBossMaxHP = 1500;

	// 180 28 1? 뒷면 
	// 191 11 1? 바 
	return __super::Initialize(pArg);
}

HRESULT CBossHPBar::Ready_Components(void* pArg)
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
	
	__super::Ready_ShaderComponent();

	return S_OK;
}

void CBossHPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBossHPBar::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_Z))
	{
		*m_pBossHP[0] -= 5;
	}
	if (KEY_DOWN(DIK_X))
	{
		if (m_eLevelID == LEVEL_GAMEPLAY)
			*m_pBossHP[1] -= 5;
	}

	return __super::Update(fTimeDelta);
}

void CBossHPBar::Late_Update(_float fTimeDelta)
{
	if (m_eIsFirstTime == DONT_RENDER)
		return;
	__super::Late_Update(fTimeDelta);
}

HRESULT CBossHPBar::Render()
{
	switch (m_eIsFirstTime)
	{
	case Client::CBossHPBar::DONT_RENDER:
		return S_OK;
	case Client::CBossHPBar::FIRST_RENDER:
		Render_FirstTime();
		break;
	case Client::CBossHPBar::DONE:
		if (m_eLevelID == LEVEL_GAMEPLAY)
		{
			Render_Ttakkeun_I();
		}
		else if (m_eLevelID == LEVEL_RACEFIRST || m_eLevelID == LEVEL_RACESECOND || m_eLevelID == LEVEL_RACETHIRD)
		{
			Render_RacingBoss();
		}
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CBossHPBar::Render_FirstTime()
{
	m_eIsFirstTime = DONE;
	return S_OK;
}

HRESULT CBossHPBar::Render_Ttakkeun_I()
{
	CUI_Manager::Get_Instance()->Render_Text_Colored("THE ALMIGHTY MAGMATRON", CFont::MEDIUMBLUE, CFont::CENTER, 320.f, g_iWinSizeY * 0.5f - 18.5f, 0.37f, 0.88f);
	CUI_Manager::Get_Instance()->Render_Text_Colored("THE ABSOLUTE SCORPOTRON", CFont::MEDIUMBLUE, CFont::CENTER, -280.f, g_iWinSizeY * 0.5f - 18.5f, 0.37f, 0.88f);
	int neg = -1;
	for (int i = 0; i < 2; i ++)
	{

		m_pTransformCom->Scaling(m_vBackSize);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(300.f * neg, g_iWinSizeY * 0.5f - 50.f, 0.1f));
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pShaderCom->SetFloat("opacity", 0.8f);
		m_pShaderCom->Begin(CShader::ALPHA);

		__super::Render();
		m_pShaderCom->End();

		_float fHPPercent = *m_pBossHP[i] / static_cast<_float>(m_iBossMaxHP);
		if (fHPPercent >= 1.f)
			fHPPercent = 1.f;

		m_pTransformCom->Scaling(m_vBarSize);
		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", 1);
		m_pShaderCom->SetFloat("HPPercent", fHPPercent);
		m_pShaderCom->Begin(CShader::HPMASKING);

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
		m_pShaderCom->End();
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		neg *= -1;
	}
	return S_OK;
}

HRESULT CBossHPBar::Render_RacingBoss()
{
	CUI_Manager::Get_Instance()->Render_Text_Colored("URBAN ERADICATOR", CFont::MEDIUMBLUE, CFont::CENTER, 0.f, g_iWinSizeY * 0.5f - 18.5f, 0.37f, 0.85f);

		m_pTransformCom->Scaling(m_vBackSize);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, g_iWinSizeY * 0.5f - 50.f, 0.1f));
		__super::Render();

		_float fHPPercent = *m_pBossHP[0] / static_cast<_float>(m_iBossMaxHP);
		if (fHPPercent >= 1.f)
			fHPPercent = 1.f;

		m_pTransformCom->Scaling(m_vBarSize);
		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", 1);
		m_pShaderCom->SetFloat("HPPercent", fHPPercent);
		m_pShaderCom->Begin(CShader::HPMASKING);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
		m_pShaderCom->End();
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CBossHPBar* CBossHPBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossHPBar* pInstance = new CBossHPBar(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBossHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossHPBar::Clone(void* pArg)
{
	CBossHPBar* pInstance = new CBossHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBossHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossHPBar::Free()
{

	__super::Free();
}
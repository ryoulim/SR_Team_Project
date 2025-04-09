// 내 클래스 이름 : RacingPanel
// 부모 클래스 이름 : UI

#include "RacingPanel.h"
#include "GameInstance.h"
#include "Font_Racing.h"
#define MULTIPLIER 1.5f

CRacingPanel::CRacingPanel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CRacingPanel::CRacingPanel(const CRacingPanel& Prototype)
	: CUI(Prototype)
{
}

HRESULT CRacingPanel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRacingPanel::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RacingPanel");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	CGameObject* pObj = { nullptr };
	pObj = m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_UI_RacingFont"));
	if (pObj == nullptr)
	{
		CUI::DESC Desc{};
		Desc.eLevelID = LEVEL_STATIC;
		Desc.fDepth = _float(UI_FONT);
		Desc.vScale = _float3(1.f, 1.f, 1.f);
		Desc.vInitPos = _float3(0.1f, 0.1f, 0.1f);
		if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_Font_Racing"),
			LEVEL_STATIC, TEXT("Layer_UI_RacingFont"), &pObj, &Desc)))
			return E_FAIL;
		m_iCurSpeed = 140;
		m_iOrigSpeed = 140;
		m_iTargetSpeed = 140;
	}
	m_pFont_Racing = dynamic_cast<CFont_Racing*>(pObj);
	if (nullptr == m_pFont_Racing)
		return E_FAIL;

	m_vPos = _float3(0.f, g_iWinSizeY * -0.5f + 60.f, 0.1f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
	m_vSize = _float3(105.f * MULTIPLIER, 76.f * MULTIPLIER, 1.f);
	m_pTransformCom->Scaling(m_vSize);

	m_pArrowTransformCom->Set_State(CTransform::STATE_POSITION, _float3{ m_vPos.x, m_vPos.y - 20.f, m_vPos.z });
	m_pArrowTransformCom->Scaling(_float3(13.f * MULTIPLIER, 80.f * MULTIPLIER, 1.f));

	return S_OK;
}

HRESULT CRacingPanel::Ready_Components(void* pArg)
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

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Arrow"), reinterpret_cast<CComponent**>(&m_pArrowTransformCom), pArg)))
		return E_FAIL;

	Ready_ShaderComponent();

	return S_OK;
}

void CRacingPanel::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRacingPanel::Update(_float fTimeDelta)
{
	Acc_CurSpeed(fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CRacingPanel::Late_Update(_float fTimeDelta)
{
	Calculate_ArrowAngle();
	__super::Late_Update(fTimeDelta);
}

HRESULT CRacingPanel::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pTransformCom->Scaling(m_vSize);
	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", 0); // 텍스쳐 바인딩
	m_pShaderCom->SetFloat("opacity", 0.9f);
	m_pShaderCom->Begin(CShader::ALPHA);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();

	m_pFont_Racing->Render_Number(m_iCurSpeed); // 숫자

	Render_Arrow();								// 계기판 바늘 	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	return S_OK;
}

void CRacingPanel::Render_Boosts()
{

}

void CRacingPanel::Render_Arrow()
{
	m_pArrowTransformCom->Rotation(_float3{ 0.f, 0.f, 1.f }, D3DXToRadian(m_fArrowAngle));

	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", 1); // 텍스쳐 바인딩
	m_pShaderCom->SetFloat("opacity", 0.9f);
	m_pShaderCom->Begin(CShader::ALPHA);
	if (FAILED(m_pArrowTransformCom->Bind_Resource()))
		return ;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return ;

	if (FAILED(m_pVIBufferCom->Render()))
		return ;
	m_pShaderCom->End();

}

void CRacingPanel::Calculate_ArrowAngle()
{
	// 회전 각도 계산
	if (m_iCurSpeed > 300.f)
		m_fArrowAngle = -90.f;
	else if (m_iCurSpeed < 0.f)	
		m_fArrowAngle = 90.f;
	else
		m_fArrowAngle = 90.f - (m_iCurSpeed / 300.f) * 180.f;

}

void CRacingPanel::Acc_CurSpeed(_float fTimeDelta)
{
	_float fCurSpeed = _float(m_iCurSpeed);
	fCurSpeed += (m_iTargetSpeed - fCurSpeed) * fTimeDelta * 10.f;
	if (abs(m_iTargetSpeed - fCurSpeed) < 1)
		m_iCurSpeed = m_iTargetSpeed;
	m_iCurSpeed = _int(fCurSpeed);
}

CRacingPanel* CRacingPanel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRacingPanel* pInstance = new CRacingPanel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRacingPanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRacingPanel::Clone(void* pArg)
{
	CRacingPanel* pInstance = new CRacingPanel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRacingPanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRacingPanel::Free()
{
	Safe_Release(m_pArrowTransformCom);
	__super::Free(); 
}
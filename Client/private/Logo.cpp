// 내 클래스 이름 : Logo
// 부모 클래스 이름 : UI

#include "Logo.h"
#include "GameInstance.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CLogo::CLogo(const CLogo& Prototype)
	: CUI(Prototype)
{
}

HRESULT CLogo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogo::Initialize(void* pArg)
{
	//m_eLevelID = LEVEL_LOGO;
	m_szTextureID = TEXT("Logo");
	m_szBufferType = TEXT("Rect");



	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	/* For.Com_Shader */
	__super::Ready_ShaderComponent();

	Ready_Component_For_Shadow(pArg);

	m_pTextureCom->Get_TextureSize(0, &(m_vSize));
	m_pTransformCom->Scaling(m_vSize);

	return S_OK;
}

void CLogo::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLogo::Update(_float fTimeDelta)
{
	// 시간 따라 opacity 바꾸는 코드,
	m_fOpacity *= 1.f + fTimeDelta * 1.f * m_fOpacityChangeVar;
	if (m_fOpacity > 0.99f)
	{
		m_fAnimTick += fTimeDelta;
		m_fOpacity = 0.99f;
		if (m_fAnimTick > 0.7f)
		{
			m_fOpacityChangeVar = 1.f;
			m_fOpacityChangeVar *= -1.f;
			m_fAnimTick = 0.f;
		}
	}
	if (m_fOpacity < 0.75f)
		m_fOpacityChangeVar *= -3.f;

	
	return __super::Update(fTimeDelta);
}

void CLogo::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLogo::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom_For_Shadow->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom_For_Shadow->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom_For_Shadow->Render()))
		return E_FAIL;


	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", 0);
	m_pShaderCom->SetFloat("opacity", m_fOpacity);
	m_pShaderCom->Begin(CShader::ALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	__super::Render();
	
	m_pShaderCom->End();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return EVN_NONE;
}

HRESULT CLogo::Ready_Component_For_Shadow(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + L"Logo_Shadow",
		TEXT("Com_Texture_Shadow"), reinterpret_cast<CComponent**>(&m_pTextureCom_For_Shadow), pArg)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer_Shadow"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_For_Shadow), pArg)))
		return E_FAIL;

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLogo* pInstance = new CLogo(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLogo::Clone(void* pArg)
{
	CLogo* pInstance = new CLogo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogo::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom_For_Shadow);
	Safe_Release(m_pVIBufferCom_For_Shadow);
}
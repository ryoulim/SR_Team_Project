// 내 클래스 이름 : BossHPBar
// 부모 클래스 이름 : UI

#include "BossHPBar.h"
#include "GameInstance.h"

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
	m_vBackSize = _float3(240.f, 42.f, 1.f);
	m_vBarSize = _float3(192.f, 12.f, 1.f);
	m_iBossMaxHP = 100;
	m_pBossHP = new int(100);
	// 254 40 1? 뒷면
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
		*m_pBossHP -= 10;
	}

	return __super::Update(fTimeDelta);
}

void CBossHPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBossHPBar::Render()
{
	m_pTransformCom->Scaling(m_vBackSize);
	__super::Render();

	_float fHPPercent = *m_pBossHP / static_cast<_float>(m_iBossMaxHP);
	if (fHPPercent >= 1.f)
		fHPPercent = 1.f;
	//_float fsize = HPBARMAX * fHPPercent;

	////염병UI는 중점이 0,0임 샹
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	//	{ HPBARMAX * fHPPercent * 0.5f, // 현재 HP 비율만큼의 크기 중 중간 지점만큼 +된 위치
	//	(g_iWinSizeY * 0.5f) - 45.f,
	//	0.01f });

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




	//if (FAILED(m_pTextureCom->Bind_Resource(1)))
	//	return E_FAIL;



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
	delete m_pBossHP;
	__super::Free();
}
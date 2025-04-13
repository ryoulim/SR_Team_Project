// 내 클래스 이름 : Aim
// 부모 클래스 이름 : UI

#include "Aim.h"
#include "GameInstance.h"

CAim::CAim(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CAim::CAim(const CAim& Prototype)
	: CUI(Prototype)
{
}

HRESULT CAim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAim::Initialize(void* pArg)
{
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;
	m_szTextureID = TEXT("Aim");
	m_szBufferType = TEXT("Rect");

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_vPos = pDesc->vInitPos;
		m_vPos.z = 0.99f;
		m_vSize = pDesc->vScale;
		m_fDepth = pDesc->fDepth;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
	}
	m_pTextureCom->Get_TextureSize(0, &m_vSize);
	m_pTransformCom->Scaling(m_vSize);
	__super::Ready_ShaderComponent();

	return S_OK;
}

HRESULT CAim::Ready_Components(void* pArg)
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

void CAim::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CAim::Update(_float fTimeDelta)
{
	if (LEVEL_RACING(m_eLevelID))
		return EVN_NONE;
	if (m_pAmmoInfo != nullptr)
 		Calc_Magazine(m_pAmmoInfo->iMaxAmmo, m_pAmmoInfo->iReloadedAmmo);

	return __super::Update(fTimeDelta);
}

void CAim::Late_Update(_float fTimeDelta)
{
	if (LEVEL_RACING(m_pGameInstance->Get_CurrentLevelIndex()) ||
		m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_LOGO ||
		m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_LOADING)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CAim::Render()
{
	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", 0);
	m_pShaderCom->SetFloat("maskingDistance", m_fMaskingDist);
	m_pShaderCom->Begin(CShader::MASKING);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (FAILED(__super::Render()))
		return E_FAIL;
	m_pShaderCom->End();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}

void CAim::Calc_Magazine(_uint iMax, _uint iCur)
{
	if (iMax < iCur)
		return;
	if (iCur < 3)
		m_fMaskingDist = iCur * 0.2f;
	else
		m_fMaskingDist = 1.f;
}

CAim* CAim::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAim* pInstance = new CAim(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAim::Clone(void* pArg)
{
	CAim* pInstance = new CAim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CAim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAim::Free()
{
	__super::Free();
}
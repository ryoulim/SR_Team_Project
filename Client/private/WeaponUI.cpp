#include "WeaponUI.h"
#include "GameInstance.h"
#include <UI_Manager.h>
#include "Weapon.h"

CWeaponUI::CWeaponUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CWeaponUI::CWeaponUI(const CWeaponUI& Prototype)
	: CUI(Prototype)
{
}

HRESULT CWeaponUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeaponUI::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("WeaponUI");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSelectPos);
	m_pTextureCom->Get_TextureSize(0, &m_vSize);
	m_pTransformCom->Scaling(m_vSize);
	return S_OK;
}

HRESULT CWeaponUI::Ready_Components(void* pArg)
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

	Ready_ShaderComponent();

	return S_OK;
}

void CWeaponUI::Priority_Update(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_fRenderTime += fTimeDelta;
		if (m_fRenderTime >= 0.6f)
		{
			m_fSizeMul = 1.f - ((m_fRenderTime - 0.6f) / 0.1f);
		}

		if (m_fRenderTime >= 0.7f)
		{
			m_fRenderTime = 0.f;
			m_fSizeMul = 1.f;
			m_bRender = false;
		}
	}

	__super::Priority_Update(fTimeDelta);
}

EVENT CWeaponUI::Update(_float fTimeDelta)
{
	Update_WeaponSettings(); // 플레이어 키 인풋이 지금 Priority에 있으므로 Update에 위치해둠


	return __super::Update(fTimeDelta);
}

void CWeaponUI::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_LOGO || 
		m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_LOADING ||
		!m_bRender)
		return;
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeaponUI::Render()
{
	vector<CWeapon*> tmp = *m_pWeapons;
	switch (m_iMaxWeaponIndex) // 1개일 때 2개일 때 3개이상일 때. 
	{
	case 0:
		m_fTextureNum = 0;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSelectPos);
		m_pTextureCom->Get_TextureSize((_uint)m_fTextureNum, &m_vSize);
		m_pTransformCom->Scaling(m_vSize * m_fSizeMul);
		Render_WeaponUI();
		CUI_Manager::Get_Instance()->Render_Text(tmp[m_iCurWeaponIndex]->Get_Info()->iCurAmmo, CFont::BIGORANGE, CFont::CENTER, m_vSelectPos.x + 40.f, m_vSelectPos.y - 15.f, 0.4f * m_fSizeMul);
		break;
	case 1:
		m_fTextureNum = _float(m_iCurWeaponIndex);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSelectPos);
		m_pTextureCom->Get_TextureSize((_uint)m_fTextureNum, &m_vSize);
		m_pTransformCom->Scaling(m_vSize * m_fSizeMul);
		Render_WeaponUI();
		CUI_Manager::Get_Instance()->Render_Text(tmp[m_iCurWeaponIndex]->Get_Info()->iCurAmmo, CFont::BIGORANGE, CFont::CENTER, m_vSelectPos.x + 40.f, m_vSelectPos.y - 15.f, 0.4f * m_fSizeMul);

		m_fTextureNum = _float(m_iCurWeaponIndex + 1);
		if (m_fTextureNum > _float(m_iMaxWeaponIndex))
			m_fTextureNum = 0;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSecondPos);
		m_pTextureCom->Get_TextureSize((_uint)m_fTextureNum, &m_vSize);
		m_pTransformCom->Scaling(m_vSize * 0.8f * m_fSizeMul);
		Render_WeaponUI();
		CUI_Manager::Get_Instance()->Render_Text(tmp[_uint(m_fTextureNum)]->Get_Info()->iCurAmmo, CFont::BIGORANGE, CFont::CENTER, m_vSecondPos.x + 20.f, m_vSecondPos.y - 10.f, 0.4f * m_fSizeMul);
		break;
	case 2:
		m_fTextureNum = _float(m_iCurWeaponIndex);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSelectPos);
		m_pTextureCom->Get_TextureSize((_uint)m_fTextureNum, &m_vSize);
		m_pTransformCom->Scaling(m_vSize * m_fSizeMul);
		Render_WeaponUI();
		CUI_Manager::Get_Instance()->Render_Text(tmp[m_iCurWeaponIndex]->Get_Info()->iCurAmmo, CFont::BIGORANGE, CFont::CENTER, m_vSelectPos.x + 40.f, m_vSelectPos.y - 15.f, 0.4f * m_fSizeMul);

		m_fTextureNum = _float(m_iCurWeaponIndex + 1);
		if (m_fTextureNum > _float(m_iMaxWeaponIndex))
			m_fTextureNum = 0;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSecondPos);
		m_pTextureCom->Get_TextureSize((_uint)m_fTextureNum, &m_vSize);
		m_pTransformCom->Scaling(m_vSize * 0.8f * m_fSizeMul);
		Render_WeaponUI();
		CUI_Manager::Get_Instance()->Render_Text(tmp[_uint(m_fTextureNum)]->Get_Info()->iCurAmmo, CFont::BIGORANGE, CFont::CENTER, m_vSecondPos.x + 20.f, m_vSecondPos.y - 10.f, 0.4f * m_fSizeMul);

		m_fTextureNum = _float(m_iCurWeaponIndex - 1);
		if (m_fTextureNum < 0)
			m_fTextureNum = _float(m_iMaxWeaponIndex);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vThirdPos);
		m_pTextureCom->Get_TextureSize((_uint)m_fTextureNum, &m_vSize);
		m_pTransformCom->Scaling(m_vSize * 0.8f * m_fSizeMul);
		Render_WeaponUI();
		CUI_Manager::Get_Instance()->Render_Text(tmp[_uint(m_fTextureNum)]->Get_Info()->iCurAmmo, CFont::BIGORANGE, CFont::CENTER, m_vThirdPos.x + 20.f, m_vThirdPos.y - 10.f, 0.4f * m_fSizeMul);
		break;
	default:
		MSG_BOX("당신은 멍청이 입니다");
		break;
	}
	return S_OK;
}

HRESULT CWeaponUI::Update_WeaponSettings()
{

	return S_OK;
}

HRESULT CWeaponUI::Render_WeaponUI()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", _uint(m_fTextureNum));

	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_float fOpacity = 1.f;
	if (m_iCurWeaponIndex != _int(m_fTextureNum))
		fOpacity = 0.5f;
	m_pShaderCom->SetFloat("opacity", fOpacity);
	m_pShaderCom->Begin(CShader::ALPHA);

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return E_NOTIMPL;
}

HRESULT CWeaponUI::Set_Transform(_int iTexNum)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(g_iWinSizeX * 0.5f - 60.f, 0.f, 0.99f));

	return E_NOTIMPL;
}

CWeaponUI* CWeaponUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeaponUI* pInstance = new CWeaponUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeaponUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeaponUI::Clone(void* pArg)
{
	CWeaponUI* pInstance = new CWeaponUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeaponUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeaponUI::Free()
{
	__super::Free();
}
// 내 클래스 이름 : Armor
// 부모 클래스 이름 : UI

#include "Armor.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CArmor::CArmor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CArmor::CArmor(const CArmor& Prototype)
	: CUI(Prototype)
{
}

HRESULT CArmor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArmor::Initialize(void* pArg)
{
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;
	m_szTextureID = TEXT("Armor");
	m_szBufferType = TEXT("Rect");
	m_fTextureNum = static_cast<_float>(m_eArmorType);
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
		m_pTransformCom->Scaling(m_vSize);
	}

	return S_OK;
}

HRESULT CArmor::Ready_Components(void* pArg)
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

void CArmor::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CArmor::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CArmor::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_LOGO ||
		m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_LOADING)
		return;
	__super::Late_Update(fTimeDelta);
}

HRESULT CArmor::Render()
{
	_float fFontPosX = -(g_iWinSizeX / 2.f) + m_vSize.x * 4.9f;
	_float fFontPosY = -(g_iWinSizeY / 2.f) + m_vSize.y / 2.f - 3.f;

	if (m_pPlayerInfo != nullptr)
	{
		RENDER_TEXT_BOL(m_pPlayerInfo->iArmor, fFontPosX, fFontPosY, 1.1f);
	}

	if (FAILED(m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &m_vSize)))
		return E_FAIL;

	m_pTransformCom->Scaling(m_vSize);
	

	return __super::Render();
}

CArmor* CArmor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArmor* pInstance = new CArmor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArmor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CArmor::Clone(void* pArg)
{
	CArmor* pInstance = new CArmor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CArmor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArmor::Free()
{
	__super::Free();
}
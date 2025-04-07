// 내 클래스 이름 : InteractPromptUI
// 부모 클래스 이름 : UI

#include "InteractPromptUI.h"
#include "GameInstance.h"

#include "UI_Manager.h"
#include "DoorSecurity.h"

CInteractPromptUI::CInteractPromptUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CInteractPromptUI::CInteractPromptUI(const CInteractPromptUI& Prototype)
	: CUI(Prototype)
{
}

HRESULT CInteractPromptUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInteractPromptUI::Initialize(void* pArg)
{
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CInteractPromptUI::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CInteractPromptUI::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CInteractPromptUI::Late_Update(_float fTimeDelta)
{
	/* 여기서 피킹 됐을 때만 띄워주도록 해주고싶은데? */
	/* 이 코드 개 에바긴함 */
	list<CGameObject*>* pDoorSecuritylist = m_pGameInstance->Find_Objects(m_eLevelID, TEXT("Layer_DoorSecurity"));
	if (nullptr == pDoorSecuritylist)
		return;

	for (auto& pDoorSecurity : *pDoorSecuritylist)
	{
		if(dynamic_cast<CDoorSecurity*>(pDoorSecurity)->Get_IsPicked())
			__super::Late_Update(fTimeDelta);
	}
}

HRESULT CInteractPromptUI::Render()
{
	CUI_Manager::Get_Instance()->Render_Text(
		"Press USE [E] to unlock terminal with Access Card.",
		CFont::MEDIUMBLUE,
		CFont::CENTER,
		(float)(0.f),
		(float)(-100.f));

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInteractPromptUI::Ready_Components(void* pArg)
{
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

CInteractPromptUI* CInteractPromptUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInteractPromptUI* pInstance = new CInteractPromptUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInteractPromptUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInteractPromptUI::Clone(void* pArg)
{
	CInteractPromptUI* pInstance = new CInteractPromptUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CInteractPromptUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInteractPromptUI::Free()
{
	__super::Free();
}
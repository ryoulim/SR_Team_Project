// 내 클래스 이름 : LevelLoadingMenu
// 부모 클래스 이름 : UI

#include "LevelLoadingMenu.h"
#include "GameInstance.h"
#include <UI_Manager.h>

CLevelLoadingMenu::CLevelLoadingMenu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CLevelLoadingMenu::CLevelLoadingMenu(const CLevelLoadingMenu& Prototype)
	: CUI(Prototype)
{
}

HRESULT CLevelLoadingMenu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLevelLoadingMenu::Initialize(void* pArg)
{
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;
	m_szTextureID = TEXT("LevelLoadingMenu");
	m_szBufferType = TEXT("Rect");


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


	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_vPos = pDesc->vInitPos;
		m_vPos.z = 0.99f;
		m_vSize = pDesc->vScale;
		m_fDepth = pDesc->fDepth;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
	}

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);


	return S_OK;
}

void CLevelLoadingMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLevelLoadingMenu::Update(_float fTimeDelta)
{
	if (m_fCurLoadingGauge > 0.25f)
		m_fTextureNum = 1.f;
	if (m_fCurLoadingGauge > 0.50f)
		m_fTextureNum = 2.f;
	if (m_fCurLoadingGauge > 0.75f)
		m_fTextureNum = 3.f;
	if (m_fCurLoadingGauge > 0.9f)
	{
		if (!m_isFinished)
		{
			CUI_Manager::Get_Instance()->Fade_Out();
			m_isFinished = true;
		}
	}

	return __super::Update(fTimeDelta);
}

void CLevelLoadingMenu::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLevelLoadingMenu::Render()
{
	return __super::Render();
}

CLevelLoadingMenu* CLevelLoadingMenu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevelLoadingMenu* pInstance = new CLevelLoadingMenu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLevelLoadingMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLevelLoadingMenu::Clone(void* pArg)
{
	CLevelLoadingMenu* pInstance = new CLevelLoadingMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLevelLoadingMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevelLoadingMenu::Free()
{
	__super::Free();
}
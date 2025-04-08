// 내 클래스 이름 : RacingPanel
// 부모 클래스 이름 : UI

#include "RacingPanel.h"
#include "GameInstance.h"
#include "Font_Racing.h"

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
	CUI::DESC Desc{};
	Desc.eLevelID = LEVEL_STATIC;
	Desc.fDepth = _float(UI_FONT);
	Desc.vScale = _float3(1.f, 1.f, 1.f);
	Desc.vInitPos = _float3(0.1f, 0.1f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_Font_Racing"),
		LEVEL_STATIC, TEXT("Layer_UI"), &pObj, &Desc)))
		return E_FAIL;
	m_iCurSpeed = 140;
	m_iOrigSpeed = 140;
	m_iTargetSpeed = 140;
	m_pFont_Racing = dynamic_cast<CFont_Racing*>(pObj);
	if (nullptr == m_pFont_Racing)
		return E_FAIL;

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
	__super::Render();							// 계기판 배경

	m_pFont_Racing->Render_Number(m_iCurSpeed); // 숫자

	Render_Arrow();								// 계기판 바늘 	
	Render_Boosts();							// 잘 모름 밑에 화살표..

	return S_OK;
}

void CRacingPanel::Render_Boosts()
{

}

void CRacingPanel::Render_Arrow()
{

}

void CRacingPanel::Calculate_ArrowAngle()
{
	// 회전 각도 계산
	if (m_iCurSpeed > 250.f)
		m_fArrowAngle = 180.f;
	else if (m_iCurSpeed < 0.f)	
		m_fArrowAngle = 0.f;
	else
		m_fArrowAngle = (m_iCurSpeed / 250.f) * 180.f;
}

void CRacingPanel::Acc_CurSpeed(_float fTimeDelta)
{
	m_iCurSpeed += _int((m_iTargetSpeed - m_iCurSpeed) * fTimeDelta * 15.f);
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
	__super::Free(); 
}
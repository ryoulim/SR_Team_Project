// 내 클래스 이름 : LoadingCutscene
// 부모 클래스 이름 : UI

#include "LoadingCutscene.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoadingCutscene::CLoadingCutscene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CLoadingCutscene::CLoadingCutscene(const CLoadingCutscene& Prototype)
	: CUI(Prototype)
{
}

HRESULT CLoadingCutscene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingCutscene::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingCutscene::Ready_Components(void* pArg)
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


	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_vPos = pDesc->vInitPos;
		m_vPos.z = 0.99f;
		m_vSize = pDesc->vScale;
		m_fDepth = pDesc->fDepth;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
	}

	/* 배경에 쓸 놈은 쓰도록. */
	//// 세로비 대로 이미지 맞춤
	//if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
	//	return E_FAIL;
	//m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	//m_pTransformCom->Scaling(m_vSize);

	return S_OK;
}

void CLoadingCutscene::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

EVENT CLoadingCutscene::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_SPACE))
	{
		if (m_isLoadingFinished)
		{
			//CUI_Manager::Get_Instance()->Fade_In();
			m_isReadyToChangeLevel = true;
		}
	}
	return __super::Update(fTimeDelta);
}

void CLoadingCutscene::Late_Update(_float fTimeDelta)
{
	if (m_fCurLoadingGauge >= 1.f)
	{
		m_isLoadingFinished = true;
	}
	if (m_isLoadingFinished)
	{
		m_fTextureNum += fTimeDelta * 1.f;
		if (m_fTextureNum >= 4.f)
		{
			m_fTextureNum = 3.f;
			m_isReadyToChangeLevel = true;
		}
	}
	__super::Late_Update(fTimeDelta);
}

HRESULT CLoadingCutscene::Render()
{
	return __super::Render();
}

CLoadingCutscene* CLoadingCutscene::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoadingCutscene* pInstance = new CLoadingCutscene(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoadingCutscene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoadingCutscene::Clone(void* pArg)
{
	CLoadingCutscene* pInstance = new CLoadingCutscene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoadingCutscene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingCutscene::Free()
{
	__super::Free();
}


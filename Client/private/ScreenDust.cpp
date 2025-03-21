#include "ScreenDust.h"
#include "GameInstance.h"

CScreenDust::CScreenDust(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CScreenDust::CScreenDust(const CScreenDust& Prototype)
	: CUI(Prototype)
{
}

HRESULT CScreenDust::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScreenDust::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_LOGO;
	m_szTextureID = TEXT("Screen_Dust");
	m_szBufferType = TEXT("Rect");
	m_fDepth = 5.f;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CScreenDust::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CScreenDust::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	FrameUpdate(fTimeDelta, 49.f, 25.f, true);
	return EVN_NONE;
}

void CScreenDust::Late_Update(_float fTimeDelta)
{
	
	__super::Late_Update(fTimeDelta);
}

HRESULT CScreenDust::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);



	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fAnimationFrame), &m_vSize)))
		return E_FAIL;


	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;


	m_pTransformCom->Scaling(m_vSize);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CScreenDust* CScreenDust::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScreenDust* pInstance = new CScreenDust(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CScreenDust");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CScreenDust::Clone(void* pArg)
{
	CScreenDust* pInstance = new CScreenDust(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CScreenDust");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScreenDust::Free()
{
	__super::Free();

}
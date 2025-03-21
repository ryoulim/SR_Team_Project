#include "Flatform.h"
#include "GameInstance.h"
#include "Particle_Define.h"

CFlatform::CFlatform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CFlatform::CFlatform(const CFlatform& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CFlatform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFlatform::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CFlatform::DESC* SpriteDESC = static_cast<DESC*>(pArg);

	m_eLevelID = LEVEL_STATIC;
	m_fMaxFrame = SpriteDESC->fMaxFrame;
	m_szTextureID = SpriteDESC->szTextureTag;
	m_bLoop = SpriteDESC->bLoop;
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), RADIAN(90.f));

	return S_OK;
}

void CFlatform::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFlatform::Update(_float fTimeDelta)
{
	if (m_fTextureNum > m_fMaxFrame)
	{
		if (m_bLoop)
			m_fTextureNum = 0;
		else
			return EVN_NONE;
	}


	//m_fTextureNum += 15.f * fTimeDelta;


	return __super::Update(fTimeDelta);
}

void CFlatform::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CFlatform::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return EVN_NONE;
}

CFlatform* CFlatform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFlatform* pInstance = new CFlatform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFlatform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFlatform::Clone(void* pArg)
{
	CFlatform* pInstance = new CFlatform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFlatform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlatform::Free()
{
	__super::Free();

}
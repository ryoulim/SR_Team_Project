#include "Sprite.h"
#include "GameInstance.h"
#include "Particle_Define.h"

CSprite::CSprite(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CSprite::CSprite(const CSprite& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CSprite::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSprite::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CSprite::DESC* SpriteDESC = static_cast<DESC*>(pArg);

	m_eLevelID = LEVEL_STATIC;
	m_fAnimationMaxFrame = SpriteDESC->fMaxFrame;
	m_szTextureID = SpriteDESC->szTextureTag;
	m_bLoop = SpriteDESC->bLoop;
	m_szBufferType = TEXT("Rect");
	m_fAnimationFrame = SpriteDESC->fStartFrame;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if(m_bLoop)
		m_fAnimationFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);

	return S_OK;
}

void CSprite::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CSprite::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, 15.f, m_bLoop);
	return __super::Update(fTimeDelta);
}

void CSprite::Late_Update(_float fTimeDelta)
{
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (m_pGameInstance->IsInFrustum(vPos, m_pTransformCom->Get_Radius()))
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CSprite::Render()
{
	if (m_bLoop)
	{
		return __super::Render();
	}
	else
	{
		if (m_fAnimationFrame > m_fAnimationMaxFrame)
		{
			m_bDead = true;
			return EVN_NONE;
		}
		return __super::Render();
	}

	return EVN_NONE;
}

CSprite* CSprite::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSprite* pInstance = new CSprite(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSprite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSprite::Clone(void* pArg)
{
	CSprite* pInstance = new CSprite(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CSprite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSprite::Free()
{
	__super::Free();

}
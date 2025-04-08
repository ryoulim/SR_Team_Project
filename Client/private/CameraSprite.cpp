#include "CameraSprite.h"
#include "GameInstance.h"
#include "Particle_Define.h"

CCameraSprite::CCameraSprite(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CCameraSprite::CCameraSprite(const CCameraSprite& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CCameraSprite::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCameraSprite::Initialize(void* pArg)
{
	CCameraSprite::DESC* SpriteDESC = static_cast<DESC*>(pArg);
	
	//뷰포트 크기
	D3DVIEWPORT9            ViewportDesc{};
	m_pGraphic_Device->GetViewport(&ViewportDesc);

	//멤버변수 저장
	m_eLevelID				= LEVEL_STATIC;
	m_fAnimationMaxFrame	= SpriteDESC->fMaxFrame;
	m_szTextureID			= SpriteDESC->szTextureTag;
	//m_bActive				= SpriteDESC->bActive;
	m_fAnimationSpeed		= SpriteDESC->fAniSpeed;
	m_eEffectType			= SpriteDESC->eEffectType;
	m_szBufferType			= TEXT("Rect");

	m_fX					= SpriteDESC->vInitPos.x;
	m_fY					= SpriteDESC->vInitPos.y;
	m_fSizeX				= SpriteDESC->vScale.x;
	m_fSizeY				= SpriteDESC->vScale.y;

	/* 컴포넌트를 붙이자 */
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	/* 스크린에 붙이자 */
	m_pTransformCom->Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(SpriteDESC->vInitPos.x - ViewportDesc.Width / 2,
		-(SpriteDESC->vInitPos.y - ViewportDesc.Height / 2),
		SpriteDESC->vInitPos.z));

	/* 시작 프레임 랜덤셋팅 */
	if (SpriteDESC->bRandom)
		m_fAnimationFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);
	else
		m_fAnimationFrame = 0;


	/* 이 스크린의 뎁스를 정해주세요 */
	if (m_eEffectType == eEffectType::FIRE)
		m_fDepth = 0.001f;
	if (m_eEffectType == eEffectType::GUNFIRE)
		m_fDepth = 0.05f;
	if (m_eEffectType == eEffectType::BULLETTRACER)
		m_fDepth = 0.1f;
	if (m_eEffectType == eEffectType::CHAINGUN || m_eEffectType == eEffectType::SHOTGUN)
		m_fDepth = 8.f;
	if (m_eEffectType == eEffectType::SHOTGUNFIRE)
		m_fDepth = 7.5f;


	return S_OK;
}

HRESULT CCameraSprite::Reset(void* pArg)
{
	m_fAnimationFrame = 0.f;
	return S_OK;
}

void CCameraSprite::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CCameraSprite::Update(_float fTimeDelta)
{
	FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, m_fAnimationSpeed, false);
	if (m_fAnimationMaxFrame <= m_fAnimationFrame)
		return EVN_DEAD;

	return __super::Update(fTimeDelta);
}

void CCameraSprite::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CCameraSprite::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}

HRESULT CCameraSprite::Render()
{	
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;


	SetUp_RenderState();
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	Release_RenderState();


	return EVN_NONE;
}

HRESULT CCameraSprite::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CCameraSprite* CCameraSprite::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCameraSprite* pInstance = new CCameraSprite(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCameraSprite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCameraSprite::Clone(void* pArg)
{
	CCameraSprite* pInstance = new CCameraSprite(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCameraSprite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCameraSprite::Free()
{
	__super::Free();

}
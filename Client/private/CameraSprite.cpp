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
	
	//뷰 행렬 초기화
	D3DXMatrixIdentity(&m_ViewMatrix);

	//뷰포트 크기
	D3DVIEWPORT9            ViewportDesc{};
	m_pGraphic_Device->GetViewport(&ViewportDesc);

	//직교투영 행렬
	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)ViewportDesc.Width, (_float)ViewportDesc.Height, 0.f, 1.f);


	//멤버변수 저장
	m_eLevelID				= LEVEL_STATIC;
	m_fAnimationMaxFrame	= SpriteDESC->fMaxFrame;
	m_szTextureID			= SpriteDESC->szTextureTag;
	m_bActive				= SpriteDESC->bActive;
	m_szBufferType			= TEXT("Rect");

	m_fX					= SpriteDESC->vInitPos.x;
	m_fY					= SpriteDESC->vInitPos.y;
	m_fSizeX				= SpriteDESC->vScale.x;
	m_fSizeY				= SpriteDESC->vScale.y;

	//기본 설정
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(SpriteDESC->vInitPos.x - ViewportDesc.Width / 2,
		-(SpriteDESC->vInitPos.y - ViewportDesc.Height / 2),
		SpriteDESC->vInitPos.z));

	//랜덤셋팅
	m_fAnimationFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);

	return S_OK;
}

void CCameraSprite::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	//_float4x4 matCamWorld;
	//
	////카메라의 위치저장
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	//D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	//_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };
	//
	//
	////카메라 라이트,업,룩 벡터 저장
	//_float3 vCameraRight = { matCamWorld._11, matCamWorld._12, matCamWorld._13 };
	//_float3 vCameraUp = { matCamWorld._21, matCamWorld._22, matCamWorld._23 };
	//_float3 vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };
	//
	////카메라가 바라보는 화면의 오른쪽 아래에서 시작
	//_float3 vScreen = vCameraPos;
	//vScreen += vCameraRight * m_fScreenX; // 오른쪽으로 이동
	//vScreen += vCameraUp * m_fScreenY; // 아래쪽으로 이동
	//vScreen += vCameraLook * m_fScreenZ; // 아래쪽으로 이동
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vScreen);

}

EVENT CCameraSprite::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return EVN_NONE;


	return __super::Update(fTimeDelta);
}

void CCameraSprite::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCameraSprite::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	return S_OK;
}

HRESULT CCameraSprite::Render()
{
	if (!m_bActive)
		return EVN_NONE;
	
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

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

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
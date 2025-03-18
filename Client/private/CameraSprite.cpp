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
	if (nullptr == pArg)
		return E_FAIL;

	CCameraSprite::DESC* SpriteDESC = static_cast<DESC*>(pArg);

	m_eLevelID = LEVEL_STATIC;
	m_fMaxFrame = SpriteDESC->fMaxFrame;
	m_szTextureID = SpriteDESC->szTextureTag;
	m_fScreenX = SpriteDESC->fSceenX;
	m_fScreenY = SpriteDESC->fSceenY;
	m_fScreenZ = SpriteDESC->fSceenZ;
	m_bActive = SpriteDESC->bActive;
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fTextureNum = GetRandomFloat(0.f, m_fMaxFrame);

	return S_OK;
}

void CCameraSprite::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	_float4x4 matCamWorld;

	//카메라의 위치저장
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };


	//카메라 라이트,업,룩 벡터 저장
	_float3 vCameraRight = { matCamWorld._11, matCamWorld._12, matCamWorld._13 };
	_float3 vCameraUp = { matCamWorld._21, matCamWorld._22, matCamWorld._23 };
	_float3 vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };

	//파티클 위치 설정 (카메라가 바라보는 화면의 오른쪽 아래에서 시작)
	_float3 vScreen = vCameraPos;
	vScreen += vCameraRight * m_fScreenX; // 오른쪽으로 이동
	vScreen += vCameraUp * m_fScreenY; // 아래쪽으로 이동
	vScreen += vCameraLook * m_fScreenZ; // 아래쪽으로 이동
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vScreen);

}

EVENT CCameraSprite::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return EVN_NONE;

	if (m_fTextureNum > m_fMaxFrame)
	{
		m_fTextureNum = 0;
	}


	m_fTextureNum += 15.f * fTimeDelta;


	return __super::Update(fTimeDelta);
}

void CCameraSprite::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCameraSprite::Render()
{
	if (!m_bActive)
		return EVN_NONE;
	
	__super::Render();

	return EVN_NONE;
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
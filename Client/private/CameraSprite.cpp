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
	m_bLoop = SpriteDESC->bLoop;
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCameraSprite::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	_float4x4 matCamWorld;

	//ī�޶��� ��ġ����
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };


	//ī�޶� ����Ʈ,��,�� ���� ����
	_float3 vCameraRight = { matCamWorld._11, matCamWorld._12, matCamWorld._13 };
	_float3 vCameraUp = { matCamWorld._21, matCamWorld._22, matCamWorld._23 };
	_float3 vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };

	//��ƼŬ ��ġ ���� (ī�޶� �ٶ󺸴� ȭ���� ������ �Ʒ����� ����)
	_float3 vScreen = vCameraPos;
	vScreen += vCameraRight * 1.5f; // ���������� �̵�
	vScreen += vCameraUp * -1.0f; // �Ʒ������� �̵�
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vScreen);
}

EVENT CCameraSprite::Update(_float fTimeDelta)
{
	if (m_fTextureNum > m_fMaxFrame)
	{
		if (m_bLoop)
			m_fTextureNum = 0;
		else
			return EVN_NONE;
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
	if (m_bLoop)
	{
		return __super::Render();
	}
	else
	{
		if (m_fTextureNum > m_fMaxFrame)
		{
			return EVN_NONE;
		}
		return __super::Render();
	}
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
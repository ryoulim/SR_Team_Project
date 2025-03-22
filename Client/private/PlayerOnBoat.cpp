#include "PlayerOnBoat.h"
#include "CameraManager.h"

CPlayerOnBoat::CPlayerOnBoat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn { pGraphic_Device }
{
}

CPlayerOnBoat::CPlayerOnBoat(const CPlayerOnBoat& Prototype)
	: CPawn (Prototype)
{
}

HRESULT CPlayerOnBoat::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerOnBoat::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_RACE;
	m_szTextureID = TEXT("PlayerOnBoat");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	////카메라 매니저를 가져온다
	//m_pCameraManager = static_cast<CCameraManager*>(m_pGameInstance->Find_Object(m_eLevelID, TEXT("Layer_Camera")));
	//Safe_AddRef(m_pCameraManager);

	////매니저를 통해 특정 카메라를 가져온다
	//CGameObject* FPS_Camera = m_pCameraManager->Get_Camera(CCameraManager::FPS);

	//m_pCameraTransform = static_cast<CTransform*>(FPS_Camera->Find_Component(TEXT("Com_Transform")));
	//Safe_AddRef(m_pCameraTransform);

	return S_OK;
}

void CPlayerOnBoat::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayerOnBoat::Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);

	//m_pCameraTransform->Set_State(CTransform::STATE_POSITION, *m_pTransformCom->Get_State(CTransform::STATE_POSITION));


	return __super::Update(fTimeDelta);
}

void CPlayerOnBoat::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayerOnBoat::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CPlayerOnBoat::Key_Input(_float fTimeDelta)
{
	if (KEY_PRESSING(DIK_UP))
	{
		//m_pGravityCom->Go_Straight_On_Terrain(fTimeDelta);
		m_pTransformCom->Go_Straight(fTimeDelta);
		//const _float3* vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	}
	if (KEY_PRESSING(DIK_DOWN))
	{
		//m_pGravityCom->Go_Backward_On_Terrain(fTimeDelta);
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_LEFT))
	{
		//m_pGravityCom->Go_Left_On_Terrain(fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_RIGHT))
	{
		//m_pGravityCom->Go_Right_On_Terrain(fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

CPlayerOnBoat* CPlayerOnBoat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerOnBoat* pInstance = new CPlayerOnBoat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayerOnBoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerOnBoat::Clone(void* pArg)
{
	CPlayerOnBoat* pInstance = new CPlayerOnBoat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPlayerOnBoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerOnBoat::Free()
{
	__super::Free();

	Safe_Release(m_pCameraManager);
	Safe_Release(m_pCameraTransform);
}

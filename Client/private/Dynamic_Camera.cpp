#include "Dynamic_Camera.h"
#include "Client_Defines.h"
#include "GameInstance.h"

CDynamic_Camera::CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDevice)
    :CCamera{ pGraphicDevice }
{
}

CDynamic_Camera::CDynamic_Camera(const CDynamic_Camera& Prototype)
    : CCamera(Prototype)
{
}

HRESULT CDynamic_Camera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDynamic_Camera::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_fMouseSensor =  pDesc->fMouseSensor;

	ShowCursor(FALSE);

    return S_OK;
}

void CDynamic_Camera::Priority_Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
	if (m_bBouseFixMod)
	{
		Mouse_Move();
		Mouse_Fix();
	}
}
#include "Mycube.h"

EVENT CDynamic_Camera::Update(_float fTimeDelta)
{
	if (KEY_DOWN(VK_SPACE))
	{
		CMyCube::DESC CameraDesc{};
		CameraDesc.vInitPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		CameraDesc.vScale = { 5.f,5.f,5.f };
		CameraDesc.vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		CameraDesc.fSpeedPerSec = 500.f;
		m_pGameInstance->Active_Object(TEXT("ObjectPool_MyCube"), LEVEL_GAMEPLAY,
			TEXT("Layer_Object"), &CameraDesc);
	}

	return __super::Update(fTimeDelta);
}

void CDynamic_Camera::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CDynamic_Camera::Render() // 기본오브젝트 랜더 셋팅
{
	if (FAILED(Bind_Resource()))
		return E_FAIL;
	if(FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;
	return S_OK;
}

void CDynamic_Camera::Key_Input(_float fTimeDelta)
{
	if (KEY_PRESSING('A'))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	else if (KEY_PRESSING('D'))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	else if (KEY_PRESSING('W'))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else if (KEY_PRESSING('S'))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	else if (KEY_DOWN(VK_TAB))
	{
		m_bBouseFixMod = !m_bBouseFixMod;
	}
}

void CDynamic_Camera::Mouse_Move()
{
	_float		fMouseMoveX = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_X)) };
	_float		fMouseMoveY = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_Y)) };
	
	_float3		vRotationAxis = (*m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * fMouseMoveY)
		+ (*m_pTransformCom->Get_State(CTransform::STATE_UP) * fMouseMoveX);

	_float fAngle = RADIAN(_float3(fMouseMoveX, fMouseMoveY, 0).Length() * m_fMouseSensor);

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	_float4x4 matRot{ vRotationAxis,fAngle };
	vLook.TransformNormal(matRot);
	m_pTransformCom->LookAt(*m_pTransformCom->Get_State(CTransform::STATE_POSITION) + vLook);
}

void CDynamic_Camera::Mouse_Fix()
{
	POINT		ptMouse{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamic_Camera::Update_Projection_Matrix()
{
	m_ProjMatrix.MakePerspectiveProjMat(m_fFov, m_fAspect, m_fNear, m_fFar);
}

HRESULT CDynamic_Camera::Ready_Components(void* pArg)
{
	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	return S_OK;
}


CDynamic_Camera* CDynamic_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDynamic_Camera* pInstance = new CDynamic_Camera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamic_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera* CDynamic_Camera::Clone(void* pArg)
{
	CDynamic_Camera* pInstance = new CDynamic_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDynamic_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamic_Camera::Free()
{
	__super::Free();
}

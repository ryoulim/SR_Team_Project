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

	m_CameraDesc = *static_cast<DESC*>(pArg);
	m_CameraDesc.fFovy = D3DXToRadian(m_CameraDesc.fFovy);
	if (FAILED(Ready_Components(&m_CameraDesc)))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);
	m_pTransform->LookAt(m_CameraDesc.vAt);

	if(FAILED(m_pTransform->Bind_View_Transform()))
		return E_FAIL;

	if (FAILED(Bind_Projection_Transform()))
		return E_FAIL;

	ShowCursor(FALSE);

    return S_OK;
}

void CDynamic_Camera::Priority_Update(_float fTimeDelta)
{
}

void CDynamic_Camera::Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
	Mouse_Move();
	Mouse_Fix();
}

void CDynamic_Camera::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CDynamic_Camera::Render() // 기본오브젝트 랜더 셋팅
{
	if(FAILED(m_pTransform->Bind_View_Transform()))
		return E_FAIL;
	if (FAILED(Bind_Projection_Transform()))
		return E_FAIL;
	if(FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;
	return S_OK;
}

void CDynamic_Camera::Key_Input(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing('A'))
	{
		m_pTransform->Go_Left(fTimeDelta);
	}
	else if (m_pGameInstance->Key_Pressing('D'))
	{
		m_pTransform->Go_Right(fTimeDelta);
	}
	else if (m_pGameInstance->Key_Pressing('W'))
	{
		m_pTransform->Go_Straight(fTimeDelta);
	}
	else if (m_pGameInstance->Key_Pressing('S'))
	{
		m_pTransform->Go_Backward(fTimeDelta);
	}
	else if (m_pGameInstance->Key_Down('R'))
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);
		m_pTransform->LookAt(m_CameraDesc.vAt);
	}
}

void CDynamic_Camera::Mouse_Move()
{
	_float		dwMouseMoveY = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_Y)) };
	_float		dwMouseMoveX = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_X)) };
	
	_float3		vRotationAxis = (*m_pTransform->Get_State(CTransform::STATE_RIGHT) * dwMouseMoveY)
		+ (*m_pTransform->Get_State(CTransform::STATE_UP) * dwMouseMoveX);

	_float3		vTmp = { dwMouseMoveX, dwMouseMoveY, 0 };

	_float fAngle = D3DXToRadian(D3DXVec3Length(&vTmp) * 0.1f);

	_float3 vLook = *m_pTransform->Get_State(CTransform::STATE_LOOK);

	 _float4x4 matRot;
	 D3DXMatrixRotationAxis(&matRot, &vRotationAxis, fAngle);
	 D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
	 m_pTransform->LookAt(*m_pTransform->Get_State(CTransform::STATE_POSITION) + vLook);
}

void CDynamic_Camera::Mouse_Fix()
{
	POINT		ptMouse{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

HRESULT CDynamic_Camera::Ready_Components(void* pArg)
{
	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), pArg)))
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
		MSG_BOX("Failed to Cloned : CDynamic_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamic_Camera::Free()
{
	__super::Free();
}

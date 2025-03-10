#include "..\Public\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CCamera::CCamera(const CCamera & Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
}

void CCamera::Update(_float fTimeDelta)
{
}

void CCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Bind_Projection_Transform()
{
	_float4x4 ProjMat{};

	D3DXMatrixPerspectiveFovLH(&ProjMat,
		m_CameraDesc.fFovy,
		m_CameraDesc.fAspect,
		m_CameraDesc.fNear,
		m_CameraDesc.fFar);

	return m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMat);
}

void CCamera::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
}

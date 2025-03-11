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
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    if (nullptr == m_pTransformCom)
        return E_FAIL;

    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vEye);
    m_pTransformCom->LookAt(pDesc->vAt);

    m_fFov = RADIAN(pDesc->fFov);
    m_fNear = pDesc->fNear;
    m_fFar = pDesc->fFar;

    D3DVIEWPORT9            ViewportDesc{};
    m_pGraphic_Device->GetViewport(&ViewportDesc);

    m_fAspect = static_cast<_float>(ViewportDesc.Width) / ViewportDesc.Height;

	Update_Projection_Matrix();
	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
}

EVENT CCamera::Update(_float fTimeDelta)
{
    return EVN_NONE;
}

void CCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Bind_Resource()
{
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrix_Inverse(m_ViewMatrix));

	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
}

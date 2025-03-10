#include "Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent(Prototype)
	, m_WorldMatrix{ Prototype.m_WorldMatrix }
{
}

HRESULT CTransform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_fSpeedPerSec = pDesc->fSpeedPerSec;
		m_fRotationPerSec = pDesc->fRotationPerSec;
	}

	return S_OK;
}

HRESULT CTransform::Bind_Resource()
{
	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
}

HRESULT CTransform::Bind_View_Transform()
{
	_float4x4 matView = {};

	return m_pGraphic_Device->SetTransform(D3DTS_VIEW, &_float4x4().MakeInverseMat(m_WorldMatrix));
}

_float3 CTransform::Compute_Scaled() const
{
	return _float3(Get_State(STATE_RIGHT)->Length(),
		Get_State(STATE_RIGHT)->Length(),
		Get_State(STATE_RIGHT)->Length());
}

void CTransform::Scaling(_float Ratio)
{
	Set_State(STATE_RIGHT, *Get_State(STATE_RIGHT) * Ratio);
	Set_State(STATE_UP, *Get_State(STATE_UP) * Ratio);
	Set_State(STATE_LOOK, *Get_State(STATE_LOOK) * Ratio);
}

void CTransform::Scaling(_float3 Ratio)
{
	Set_State(STATE_RIGHT, *Get_State(STATE_RIGHT) * Ratio.x);
	Set_State(STATE_UP, *Get_State(STATE_UP) * Ratio.y);
	Set_State(STATE_LOOK, *Get_State(STATE_LOOK) * Ratio.z);
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);
	_float3		vLook = *Get_State(STATE_LOOK);

	vPosition += vLook.Normalize() * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);
	_float3		vLook = *Get_State(STATE_LOOK);

	vPosition -= vLook.Normalize() * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);
	_float3		vRight = *Get_State(STATE_RIGHT);

	vPosition -= vRight.Normalize() * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);
	_float3		vRight = *Get_State(STATE_RIGHT);

	vPosition += vRight.Normalize() * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::LookAt(const _float3& vTargetPos)
{
	_float3		vScaled = Compute_Scaled();

	_float3		vLook = vTargetPos - *Get_State(STATE_POSITION);

	_float3		vRight = _float3{ 0.f, 1.f, 0.f }.Cross(vLook);

	_float3		vUp = vLook.Cross(vRight);

	Set_State(STATE_RIGHT, vRight.Normalize() * vScaled.x);
	Set_State(STATE_UP, vUp.Normalize() * vScaled.y);
	Set_State(STATE_LOOK, vLook.Normalize() * vScaled.z);
}

void CTransform::Chase(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance)
{
	_float3		vPosition = *Get_State(STATE_POSITION);

	_float3		vMoveDir = vTargetPos - vPosition;

	if (fMinDistance <= vMoveDir.Length())
	{
		vPosition += vMoveDir.Normalize() * m_fSpeedPerSec * fTimeDelta;
	}

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Turn(const _float3& vAxis, _float fTimeDelta)
{
	_float3			vRight = *Get_State(STATE_RIGHT);
	_float3			vUp = *Get_State(STATE_UP);
	_float3			vLook = *Get_State(STATE_LOOK);

	_float4x4		RotationMatrix{ vAxis, m_fRotationPerSec * fTimeDelta };

	Set_State(STATE_RIGHT, vRight.TransformNormal(RotationMatrix));
	Set_State(STATE_UP, vUp.TransformNormal(RotationMatrix));
	Set_State(STATE_LOOK, vLook.TransformNormal(RotationMatrix));
}

void CTransform::Turn_Immediately(const _float3& vAxis, _float fRadian)
{
	_float3			vRight = *Get_State(STATE_RIGHT);
	_float3			vUp = *Get_State(STATE_UP);
	_float3			vLook = *Get_State(STATE_LOOK);

	// D3DXVec4Transform();
	// D3DXVec3TransformCoord();

	_float4x4		RotationMatrix{ vAxis,fRadian };

	Set_State(STATE_RIGHT, vRight.TransformNormal(RotationMatrix));
	Set_State(STATE_UP, vUp.TransformNormal(RotationMatrix));
	Set_State(STATE_LOOK, vLook.TransformNormal(RotationMatrix));
}

void CTransform::Rotation(const _float3& vAxis, _float fRadian)
{
	_float3		vScaled = Compute_Scaled();

	_float3			vRight = _float3(1.f, 0.f, 0.f) * vScaled.x;
	_float3			vUp = _float3(0.f, 1.f, 0.f) * vScaled.y;
	_float3			vLook = _float3(0.f, 0.f, 1.f) * vScaled.z;

	_float4x4		RotationMatrix{ vAxis,fRadian };

	Set_State(STATE_RIGHT, vRight.TransformNormal(RotationMatrix));
	Set_State(STATE_UP, vUp.TransformNormal(RotationMatrix));
	Set_State(STATE_LOOK, vLook.TransformNormal(RotationMatrix));
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}

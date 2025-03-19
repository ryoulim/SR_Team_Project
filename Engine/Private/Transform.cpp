#include "Transform.h"

_float4x4		CTransform::m_Return{};

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

const _float4x4& CTransform::Billboard() const
{
	//객체 스케일
	_float3	vScaled = Compute_Scaled();
	//객체 포지션
	_float3	vPosition = *Get_State(CTransform::STATE_POSITION);

	//카메라 포지션
	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	//카메라를 등지는 룩벡터
	_float3		vLook = vPosition - vCameraPos;
	_float3		vUp = { 0.f, 1.f, 0.f };
	_float3		vRight = vUp.Cross(vLook);

	//각 축을 노말라이즈 x 스케일값으로 세
	vRight.Normalize();
	vRight *= vScaled.x;

	vUp.Normalize();
	vUp *= vScaled.y;

	vLook.Normalize();
	vLook *= vScaled.z;

	memcpy(&m_Return._11, &vRight, sizeof _float3);
	memcpy(&m_Return._21, &vUp, sizeof _float3);
	memcpy(&m_Return._31, &vLook, sizeof _float3);
	memcpy(&m_Return._41, &vPosition, sizeof _float3);

	return m_Return;
}

_float4x4* CTransform::Billboard(_float4x4* _Out_ pOut) const
{
	//객체 스케일
	_float3	vScaled = Compute_Scaled();
	//객체 포지션
	_float3	vPosition = *Get_State(CTransform::STATE_POSITION);

	//카메라 포지션
	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	//카메라를 등지는 룩벡터
	_float3		vLook = vPosition - vCameraPos;
	_float3		vUp = { 0.f, 1.f, 0.f };
	_float3		vRight = vUp.Cross(vLook);

	//각 축을 노말라이즈 x 스케일값으로 세
	vRight.Normalize();
	vRight *= vScaled.x;

	vUp.Normalize();
	vUp *= vScaled.y;

	vLook.Normalize();
	vLook *= vScaled.z;

	memcpy(&pOut->_11, &vRight, sizeof _float3);
	memcpy(&pOut->_21, &vUp, sizeof _float3);
	memcpy(&pOut->_31, &vLook, sizeof _float3);
	memcpy(&pOut->_41, &vPosition, sizeof _float3);

	return pOut;
}

_float3 CTransform::Compute_Scaled() const
{
	return _float3(Get_State(STATE_RIGHT)->Length(),
		Get_State(STATE_UP)->Length(),
		Get_State(STATE_LOOK)->Length());
}

void CTransform::Scaling(_float Ratio)
{
	_float3 vRight{ *Get_State(STATE_RIGHT) };
	_float3 vUp{ *Get_State(STATE_UP) };
	_float3 vLook{ *Get_State(STATE_LOOK) };

	Set_State(STATE_RIGHT, vRight.Normalize() * Ratio);
	Set_State(STATE_UP, vUp.Normalize() * Ratio);
	Set_State(STATE_LOOK, vLook.Normalize() * Ratio);
}

void CTransform::Scaling(const _float3& Ratio)
{
	_float3 vRight{ *Get_State(STATE_RIGHT) };
	_float3 vUp{ *Get_State(STATE_UP) };
	_float3 vLook{ *Get_State(STATE_LOOK) };

	Set_State(STATE_RIGHT, vRight.Normalize() * Ratio.x);
	Set_State(STATE_UP, vUp.Normalize() * Ratio.y);
	Set_State(STATE_LOOK, vLook.Normalize() * Ratio.z);
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

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);

	vPosition += _float3(0.f,1.f,0.f) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);

	vPosition -= _float3(0.f, 1.f, 0.f) * m_fSpeedPerSec * fTimeDelta;

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

	_float3		vRight = _float3{ 1.f, 0.f, 0.f } * vScaled.x;
	_float3		vUp = _float3{ 0.f, 1.f, 0.f } * vScaled.y;
	_float3		vLook = _float3{ 0.f, 0.f, 1.f } * vScaled.z;

	_float4x4		RotationMatrix{ vAxis,fRadian };

	Set_State(STATE_RIGHT, vRight.TransformNormal(RotationMatrix));
	Set_State(STATE_UP, vUp.TransformNormal(RotationMatrix));
	Set_State(STATE_LOOK, vLook.TransformNormal(RotationMatrix));
}

void CTransform::Rotation_Reset()
{
	_float3		vScaled = Compute_Scaled();

	Set_State(STATE_RIGHT, _float3{ 1.f, 0.f, 0.f } *vScaled.x);
	Set_State(STATE_UP, _float3{ 0.f, 1.f, 0.f } *vScaled.y);
	Set_State(STATE_LOOK, _float3{ 0.f, 0.f, 1.f } *vScaled.z);

}

void CTransform::Move(const _float3& vDirectionVector)
{
	_float3 vPos = *Get_State(CTransform::STATE_POSITION) + vDirectionVector;
	Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Move(const _float3& vDirectionVector, _float fTimeDelta)
{
	_float3 vPos = *Get_State(CTransform::STATE_POSITION) + vDirectionVector * fTimeDelta;
	Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Quaternion_Turn(const _float3& vAngle)
{
	_float3			vRight = *Get_State(STATE_RIGHT);
	_float3			vUp = *Get_State(STATE_UP);
	_float3			vLook = *Get_State(STATE_LOOK);

	// 사원수 회전 생성
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationYawPitchRoll(&Qur, vAngle.y, vAngle.x, vAngle.z);

	// 최종 벡터 상태 저장
	Set_State(STATE_RIGHT, RotateVectorByQuaternion(vRight, Qur));
	Set_State(STATE_UP, RotateVectorByQuaternion(vUp, Qur));
	Set_State(STATE_LOOK, RotateVectorByQuaternion(vLook, Qur));
}


void CTransform::Quaternion_Rotation(const _float3& vAngle)
{
	_float3 vScaled = Compute_Scaled();

	_float3 vRight = _float3{ 1.f, 0.f, 0.f } *vScaled.x;
	_float3 vUp = _float3{ 0.f, 1.f, 0.f } *vScaled.y;
	_float3 vLook = _float3{ 0.f, 0.f, 1.f } *vScaled.z;

	// 사원수 회전 생성
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationYawPitchRoll(&Qur, vAngle.y, vAngle.x, vAngle.z);

	Set_State(STATE_RIGHT, RotateVectorByQuaternion(vRight, Qur));
	Set_State(STATE_UP, RotateVectorByQuaternion(vUp, Qur));
	Set_State(STATE_LOOK, RotateVectorByQuaternion(vLook, Qur));
}

void CTransform::Quaternion_Revolution(const _float3& vAxis, const _float3& vCenter, _float fAngle)
{
	_float3 vPos = *Get_State(STATE_POSITION);
	Set_State(STATE_POSITION, vPos - vCenter);

	// 사원수 회전 생성
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationAxis(&Qur, &vAxis, fAngle);

	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &Qur);

	m_WorldMatrix *= matRotation;

	vPos = *Get_State(STATE_POSITION);
	Set_State(STATE_POSITION, vPos + vCenter);
}

void CTransform::Quaternion_Revolution_Pos(const _float3& vAxis, const _float3& vCenter, _float fAngle)
{
	_float3 vPos = *Get_State(STATE_POSITION);

	// 사원수 회전 생성
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationAxis(&Qur, &vAxis, fAngle);

	vPos -= vCenter; // 1. 중심을 원점으로 이동
	vPos = RotateVectorByQuaternion(vPos, Qur); // 2. 회전
	vPos += vCenter;

	Set_State(STATE_POSITION, vPos);
}

inline _float3 CTransform::RotateVectorByQuaternion(const _float3& v, const D3DXQUATERNION& q)
{
	D3DXQUATERNION qVec{ v.x, v.y, v.z, 0.0f };
	D3DXQUATERNION qConj;
	D3DXQuaternionInverse(&qConj, &q); // 켤레(역 사원수)

	// 회전 연산: v' = q * v * q^-1
	D3DXQUATERNION qResult, temp;
	D3DXQuaternionMultiply(&temp, &q, &qVec);
	D3DXQuaternionMultiply(&qResult, &temp, &qConj);

	return _float3{ qResult.x, qResult.y, qResult.z };
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
		MSG_BOX("Failed to Clone : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}

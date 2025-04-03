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

const _float4x4& CTransform::Billboard_Inverse() const
{
	//媛앹껜 ?ㅼ???
	_float3	vScaled = Compute_Scaled();
	//媛앹껜 ?ъ???
	_float3	vPosition = *Get_State(CTransform::STATE_POSITION);

	//移대찓???ъ???
	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	//移대찓?쇰? ?깆???猷⑸깹??
	_float3		vLook = vPosition - vCameraPos;
	_float3		vUp = { 0.f, 1.f, 0.f };
	_float3		vRight = vUp.Cross(vLook);

	//媛?異뺤쓣 ?몃쭚?쇱씠利?x ?ㅼ??쇨컪?쇰줈 ??
	vRight.Normalize();
	vRight *= -vScaled.x;

	vUp.Normalize();
	vUp *= vScaled.y;

	vLook.Normalize();
	vLook *= vScaled.z;
	vLook.y = 0.f;

	memcpy(&m_Return._11, &vRight, sizeof _float3);
	memcpy(&m_Return._21, &vUp, sizeof _float3);
	memcpy(&m_Return._31, &vLook, sizeof _float3);
	memcpy(&m_Return._41, &vPosition, sizeof _float3);

	return m_Return;
}

const _float4x4& CTransform::Billboard() const
{
	_float3	vScaled = Compute_Scaled();
	_float3	vPosition = *Get_State(CTransform::STATE_POSITION);

	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	_float3 vRight = reinterpret_cast<_float3*>(&matCamWorld.m[0][0])->Normalize() * vScaled.x;
	//_float3 vUp = reinterpret_cast<_float3*>(&matCamWorld.m[1][0])->Normalize() * vScaled.y;
	_float3 vLook = reinterpret_cast<_float3*>(&matCamWorld.m[2][0])->Normalize() * vScaled.z;

	memcpy(&m_Return._11, &vRight, sizeof _float3);
	memcpy(&m_Return._21, Get_State(CTransform::STATE_UP), sizeof _float3);
	memcpy(&m_Return._31, &vLook, sizeof _float3);
	memcpy(&m_Return._41, &vPosition, sizeof _float3);

	return m_Return;
}

const _float4x4& CTransform::Billboard_Y() const
{
	_float3	vScaled = Compute_Scaled();
	_float3	vPosition = *Get_State(CTransform::STATE_POSITION);

	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	_float3 vRight = reinterpret_cast<_float3*>(&matCamWorld.m[0][0])->Normalize() * vScaled.x;
	_float3 vUp = reinterpret_cast<_float3*>(&matCamWorld.m[1][0])->Normalize() * vScaled.y;
	_float3 vLook = reinterpret_cast<_float3*>(&matCamWorld.m[2][0])->Normalize() * vScaled.z;

	memcpy(&m_Return._11, &vRight, sizeof _float3);
	memcpy(&m_Return._21, &vUp, sizeof _float3);
	memcpy(&m_Return._31, &vLook, sizeof _float3);
	memcpy(&m_Return._41, &vPosition, sizeof _float3);

	return m_Return;
}

_float4x4* CTransform::Billboard(_float4x4* _Out_ pOut) const
{
	_float3	vScaled = Compute_Scaled();
	_float3	vPosition = *Get_State(CTransform::STATE_POSITION);

	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	_float3 vRight = reinterpret_cast<_float3*>(&matCamWorld.m[0][0])->Normalize() * vScaled.x;
	//_float3 vUp = reinterpret_cast<_float3*>(&matCamWorld.m[1][0])->Normalize() * vScaled.y;
	_float3 vLook = reinterpret_cast<_float3*>(&matCamWorld.m[2][0])->Normalize() * vScaled.z;

	memcpy(&(pOut->_11), &vRight, sizeof _float3);
	memcpy(&(pOut->_21), Get_State(CTransform::STATE_UP), sizeof _float3);
	memcpy(&(pOut->_31), &vLook, sizeof _float3);
	memcpy(&(pOut->_41), &vPosition, sizeof _float3);

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

void CTransform::Go_LeftOnRace(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);
	_float3		vRight = *Get_State(STATE_RIGHT);

	vPosition -= vRight.Normalize() * m_fSpeedPerSec * fTimeDelta;

	if (vPosition.x < 315.f)
		return;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);
	_float3		vRight = *Get_State(STATE_RIGHT);

	vPosition += vRight.Normalize() * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_RightOnRace(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);
	_float3		vRight = *Get_State(STATE_RIGHT);

	vPosition += vRight.Normalize() * m_fSpeedPerSec * fTimeDelta;

	if (vPosition.x > 585.f)
		return;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = *Get_State(STATE_POSITION);

	vPosition += _float3(0.f,1.f,0.f) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

bool CTransform::Go_UpCustom(_float fTimeDelta, _float fSpeed, _float fMaxHight)
{
	_float3		vPosition = *Get_State(STATE_POSITION);

	if (fMaxHight > vPosition.y)
	{
		vPosition += _float3(0.f, 1.f, 0.f) * fSpeed * fTimeDelta;
		Set_State(STATE_POSITION, vPosition);
		return false;
	}
	else
	{
		return true;
	}
}

void CTransform::Go_DownCustom(_float fTimeDelta, _float fSpeed)
{
	_float3		vPosition = *Get_State(STATE_POSITION);

	vPosition -= _float3(0.f, 1.f, 0.f) * fSpeed * fTimeDelta;

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

bool CTransform::RotateToDirection(const _float3& vCurLook, const _float3& vTargetDir, _float fTurnSpeed, _float dt)
{
	// 현재 방향 정리
	_float3 vLook = vCurLook;
	vLook.y = 0.f;
	vLook = vLook.Normalize();

	// 목표 방향 정리
	_float3 vTarget = vTargetDir;
	vTarget.y = 0.f;
	vTarget = vTarget.Normalize();

	// ✅ 회전 완료 체크 먼저
	float fDot = vLook.Dot(vTarget);
	if (fDot >= 0.999f) // 거의 같은 방향이면
	{
		// 강제로 정확히 맞춤
		_float3 vRight = _float3(0.f, 1.f, 0.f).Cross(vTarget).Normalize();
		_float3 vUp = vTarget.Cross(vRight).Normalize();
		_float3 vScaled = Compute_Scaled();

		Set_State(STATE_RIGHT, vRight * vScaled.x);
		Set_State(STATE_UP, vUp * vScaled.y);
		Set_State(STATE_LOOK, vTarget * vScaled.z);
		return true; // ✅ 회전 완료
	}

	// 보간하여 부드럽게 회전
	_float3 vNewLook = Lerp(vLook, vTarget, dt * fTurnSpeed).Normalize();

	_float3 vRight = _float3(0.f, 1.f, 0.f).Cross(vNewLook).Normalize();
	_float3 vUp = vNewLook.Cross(vRight).Normalize();
	_float3 vScaled = Compute_Scaled();

	Set_State(STATE_RIGHT, vRight * vScaled.x);
	Set_State(STATE_UP, vUp * vScaled.y);
	Set_State(STATE_LOOK, vNewLook * vScaled.z);

	return false; // ✅ 아직 회전 중

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


bool CTransform::ChaseCustom(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance, _float fSpeed)
{
	_float3		vPosition = *Get_State(STATE_POSITION);

	_float3		vMoveDir = vTargetPos - vPosition;

	//최소거리보다 길때는 포지션 갱신
	if (fMinDistance <= vMoveDir.Length())
	{
		vPosition += vMoveDir.Normalize() * fSpeed * fTimeDelta;
		Set_State(STATE_POSITION, vPosition);
	}
	else
	{
		return true;
	}
	return false;
}

bool CTransform::ChaseWithOutY(_float3& vTargetPos, _float fTimeDelta, _float fMinDistance, _float fSpeed)
{
	_float3		vPosition = *Get_State(STATE_POSITION);
	vTargetPos.y = vPosition.y;

	_float3		vMoveDir = vTargetPos - vPosition;

	//최소거리보다 길때는 포지션 갱신
	if (fMinDistance <= vMoveDir.Length())
	{
		vPosition += vMoveDir.Normalize() * fSpeed * fTimeDelta;
		Set_State(STATE_POSITION, vPosition);
	}
	else
	{
		return true;
	}
	return false;
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

void CTransform::TurnCustom(const _float3& vAxis, _float _Rotation, _float fTimeDelta)
{
	_float3			vRight = *Get_State(STATE_RIGHT);
	_float3			vUp = *Get_State(STATE_UP);
	_float3			vLook = *Get_State(STATE_LOOK);

	_float4x4		RotationMatrix{ vAxis, _Rotation * fTimeDelta };

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

void CTransform::Turn_BulletMark(const _float3& vTargetLook)
{
	_float3	vScaled = Compute_Scaled();
	_float3 vLook = vTargetLook.Normalize();

	// 월드 업 벡터 (Y축 위 방향)
	_float3 vWorldUp = { 0.f, 1.f, 0.f };

	// Look과 World Up이 거의 평행한 경우를 피하기 위한 처리
	if (fabs(vLook.Dot(vWorldUp)) > 0.99f)
	{
		vWorldUp = { 0.f, 0.f, 1.f }; // 다른 축으로 보정
	}

	_float3 vRight = vWorldUp.Cross(vLook).Normalize();
	_float3 vUp = vLook.Cross(vRight).Normalize();

	// 세 방향 벡터를 바로 설정
	Set_State(STATE_RIGHT, vRight * vScaled.x);
	Set_State(STATE_UP, vUp * vScaled.y);
	Set_State(STATE_LOOK, vLook * vScaled.z);
}

void CTransform::Move_Forward_ByLook(_float fDistance)
{
	_float3 vLook = *Get_State(STATE_LOOK);
	_float3 vPosition = *Get_State(STATE_POSITION);

	// 방향은 정규화한 다음 거리만큼 이동
	vPosition += vLook.Normalize() * fDistance;

	Set_State(STATE_POSITION, vPosition);
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

void CTransform::HarmonicMoveY(_float fWaveHegiht, _float fStdheight, _float fTimeDelta)
{
	_float vPosX = Get_State(CTransform::STATE_POSITION)->x;
	_float vPosZ = Get_State(CTransform::STATE_POSITION)->z;

	//시간 누적
	m_fHarmonicTime += fTimeDelta;

	Set_State(CTransform::STATE_POSITION, _float3(vPosX, fWaveHegiht * sinf(RADIAN(m_fSpeedPerSec * m_fHarmonicTime)) + fStdheight, vPosZ));
}

void CTransform::Quaternion_Turn(const _float3& vAngle)
{
	_float3			vRight = *Get_State(STATE_RIGHT);
	_float3			vUp = *Get_State(STATE_UP);
	_float3			vLook = *Get_State(STATE_LOOK);

	// ?ъ썝???뚯쟾 ?앹꽦
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationYawPitchRoll(&Qur, vAngle.y, vAngle.x, vAngle.z);

	// 理쒖쥌 踰≫꽣 ?곹깭 ???
	Set_State(STATE_RIGHT, RotateVectorByQuaternion(vRight, Qur));
	Set_State(STATE_UP, RotateVectorByQuaternion(vUp, Qur));
	Set_State(STATE_LOOK, RotateVectorByQuaternion(vLook, Qur));
}

void CTransform::Quaternion_Rotation(const _float3& vAngle)
{
	_float3 vScaled = Compute_Scaled();

	_float3 vRight = _float3{ 1.f, 0.f, 0.f };
	_float3 vUp = _float3{ 0.f, 1.f, 0.f };
	_float3 vLook = _float3{ 0.f, 0.f, 1.f };

	// ?ъ썝???뚯쟾 ?앹꽦
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationYawPitchRoll(&Qur, vAngle.y, vAngle.x, vAngle.z);

	Set_State(STATE_RIGHT, RotateVectorByQuaternion(vRight, Qur) * vScaled.x);
	Set_State(STATE_UP, RotateVectorByQuaternion(vUp, Qur) * vScaled.y);
	Set_State(STATE_LOOK, RotateVectorByQuaternion(vLook, Qur) * vScaled.z);
}

void CTransform::QurternionRotation(const _float3& vAngle)
{
	_float3		vScaled = Compute_Scaled();

	_float3		vRight = _float3{ 1.f, 0.f, 0.f } *vScaled.x;
	_float3		vUp = _float3{ 0.f, 1.f, 0.f } *vScaled.y;
	_float3		vLook = _float3{ 0.f, 0.f, 1.f } *vScaled.z;

	D3DXQUATERNION Qur{};
	_float4x4 RotationMatrix{};
	D3DXQuaternionRotationYawPitchRoll(&Qur, vAngle.y, vAngle.x, vAngle.z);
	D3DXMatrixRotationQuaternion(&RotationMatrix, &Qur);

	Set_State(STATE_RIGHT, vRight.TransformNormal(RotationMatrix));
	Set_State(STATE_UP, vUp.TransformNormal(RotationMatrix));
	Set_State(STATE_LOOK, vLook.TransformNormal(RotationMatrix));
}

void CTransform::Quaternion_Revolution(const _float3& vAxis, const _float3& vCenter, _float fAngle)
{
	_float3 vPos = *Get_State(STATE_POSITION);
	Set_State(STATE_POSITION, vPos - vCenter);

	// ?ъ썝???뚯쟾 ?앹꽦
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

_float3 CTransform::Lerp(const _float3& start, const _float3& end, _float t)
{
	return start + (end - start) * t;
}

inline _float3 CTransform::RotateVectorByQuaternion(const _float3& v, const D3DXQUATERNION& q)
{
	D3DXQUATERNION qVec{ v.x, v.y, v.z, 0.0f };
	D3DXQUATERNION qConj;
	D3DXQuaternionInverse(&qConj, &q); // 耳ㅻ젅(???ъ썝??

	// ?뚯쟾 ?곗궛: v' = q * v * q^-1
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

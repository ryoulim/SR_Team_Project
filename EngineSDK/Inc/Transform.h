#pragma once

#include "Component.h"

/* 월드변환을 위한 월드 행렬을 보관한다. */
/* 월드행렬의 변환을 수행한다. == 월드공간에서의 상태를 변환한다 == 모델의 월드공간에서의 움직임을 제어한다. */


BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	friend class CGravity;
	enum TRANSFORMSTATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION };

	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}DESC;
private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	const _float3* Get_State(TRANSFORMSTATE eState) const {
		return reinterpret_cast<const _float3*>(m_WorldMatrix.m[eState]);
	}
	const _float4x4& Get_WorldMatrix_Inverse() const {
		return m_Return.MakeInverseMat(m_WorldMatrix);
	}
	const _float4x4& Get_WorldMatrix_Inverse(_float4x4& rhsOut) const {
		return rhsOut.MakeInverseMat(m_WorldMatrix);
	}
	const _float4x4* Get_WorldMatrix() const { 
		return &m_WorldMatrix; 
	}
	void Set_WorldMatrix(const _float4x4* pWorld)
	{
		m_WorldMatrix = *pWorld;
	}

	void Set_State(TRANSFORMSTATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof vState);
	}
	void Set_SpeedPerSec(_float fSpeedPerSec) {
		m_fSpeedPerSec = fSpeedPerSec;
	}
	void Set_RotationPerSec(_float fRotationPerSec) {
		m_fRotationPerSec = fRotationPerSec;
	}
	void Mul_SpeedPerSec(_float Mul) {
		m_fSpeedPerSec *= Mul;
	}
	_float Get_Radius() {
		return m_fRadius;
	}
	// Z회전만 존재할 때
	_float Get_ZRot_Radian() {
		return atan2f(m_WorldMatrix._21, m_WorldMatrix._11);
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT			Bind_Resource();
	const _float4x4& Billboard_Inverse() const;
	const _float4x4& Billboard() const;
	const _float4x4& Billboard_Y() const;
	_float4x4* Billboard(_float4x4* _Out_ pOut) const;
	_float3 Compute_Scaled() const;
	void Scaling(_float Ratio);
	void Scaling(const _float3& Ratio);
	void Go_Straight(_float fTimeDelta);
	void Go_StraightWithoutY(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_LeftOnRace(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_RightOnRace(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	bool Go_UpCustom(_float fTimeDelta, _float fSpeed, _float fMaxHight);
	void Go_DownCustom(_float fTimeDelta, _float fSpeed);
	void Go_Down(_float fTimeDelta);
	void Go_Dir(const _float3& vMoveDir, _float fTimeDelta);

	void LookAt(const _float3& vTargetPos);
	bool RotateToDirection(const _float3& vCurLook, const _float3& vTargetDir, _float fTurnSpeed, _float dt);
	void Chase(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance = 0.1f);
	bool ChaseCustom(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance, _float fSpeed);
	bool ChaseWithOutY(_float3& vTargetPos, _float fTimeDelta, _float fMinDistance, _float fSpeed);
	void Turn(const _float3& vAxis, _float fTimeDelta);
	void TurnCustom(const _float3& vAxis, _float _Rotation, _float fTimeDelta);
	void Turn_Immediately(const _float3& vAxis, _float fRadian);
	void Turn_BulletMark(const _float3& vTargetLook);
	void Turn_BossRazer(_float fRadian);
	void Move_Forward_ByLook(_float fDistance);
	void Rotation(const _float3& vAxis, _float fRadian);
	void Rotation_Reset();

	void Move(const _float3& vDirectionVector);
	void Move(const _float3& vDirectionVector, _float fTimeDelta);
	void HarmonicMoveY(_float fWaveHegiht, _float fStdheight, _float fTimeDelta);
	void BossBulletLook();

	void Quaternion_Turn(const _float3& vAngle);
	void Quaternion_Rotation(const _float3& vAngle);
	void QurternionRotation(const _float3& vAngle);
	void Quaternion_Revolution(const _float3& vAxis, const _float3& vCenter, _float fAngle);
	void Quaternion_Revolution_Pos(const _float3& vAxis, const _float3& vCenter, _float fAngle);

	_float3 Lerp(const _float3& start, const _float3& end, _float t);

private:
	_float4x4				m_WorldMatrix = {};

	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};
	_float					m_fRadius{ 0.866f}; // 항등행렬 상태의 반지름 길이
	static _float4x4		m_Return;
	_float					m_fHarmonicTime = {};
	
private:
	_float3 RotateVectorByQuaternion(const _float3& v, const D3DXQUATERNION& q);

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
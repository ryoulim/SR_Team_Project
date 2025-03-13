#pragma once

#include "Component.h"

/* ���庯ȯ�� ���� ���� ����� �����Ѵ�. */
/* ��������� ��ȯ�� �����Ѵ�. == ������������� ���¸� ��ȯ�Ѵ� == ���� ������������� �������� �����Ѵ�. */


BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
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

	const _float4x4& Get_WorldMatrix_Inverse(_float4x4& rhsOut) {
		return rhsOut.MakeInverseMat(m_WorldMatrix);
	}
	const _float4x4* Get_WorldMatrix() { return &m_WorldMatrix; }

	void Set_State(TRANSFORMSTATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof vState);
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT			Bind_Resource();

	_float3 Compute_Scaled() const;
	void Scaling(_float Ratio);
	void Scaling(_float3 Ratio);
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);

	void LookAt(const _float3& vTargetPos);
	void Chase(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance = 0.1f);
	void Turn(const _float3& vAxis, _float fTimeDelta);
	void Turn_Immediately(const _float3& vAxis, _float fRadian);
	void Rotation(const _float3& vAxis, _float fRadian);

private:
	_float4x4				m_WorldMatrix;

	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
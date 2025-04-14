#pragma once
#include "Component.h"

//#define _GRV_TER
#define GRV_MIN_HEIGHT		-30.f // �� �ּ� ����

BEGIN(Engine)

class ENGINE_DLL CGravity final : public CComponent
{
public:
	typedef struct CGravityDesc
	{
		// �̰��� Ʈ�������� ������ _42 (y�� ������)�� ���� �׷���Ƽ�� ���� �����մϴ�.
		class CTransform* pTransformCom;
		// �ʴ� �����ϴ� �ð� (������ ������ Time)
		_float	fTimeIncreasePerSec;
		// �ʴ� �ִ�� ������ �� �ִ� �ӵ�
		_float	fMaxFallSpeedPerSec;
	}DESC;

private:
	CGravity(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGravity(const CGravity& Prototype);
	virtual ~CGravity() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	void Update(_float fTimeDelta);
	static void Add_StandableObjLayerTag(_uint ID) {
		m_StandableColliderGroupID.push_back(ID);
	}

	void Go_Straight_On_Terrain(_float fTimedelta);
	void Go_Backward_On_Terrain(_float fTimedelta);
	void Go_Left_On_Terrain(_float fTimedelta);
	void Go_Right_On_Terrain(_float fTimedelta);

	_float3 Get_Terrain_Front_Dir();
	_float3 Get_Terrain_Right_Dir();

	void Jump(_float fJumpPower);
	void Stop_Jump() {
		m_bJump = FALSE;
	}
	_bool isJump() {
		return m_bJump;
	}
	// �ʱ�ȭ���� ���� �ɼ��� �ٲٴ� �ɼ�.
	void Set_JumpOption(_float	fTimeIncreasePerSec, _float	fMaxFallSpeedPerSec) {
		m_fTimeIncreasePerSec = fTimeIncreasePerSec;
		m_fMaxFallSpeedperSec = fMaxFallSpeedPerSec;
	}

	// ���̸� ������ �����Ͽ� ����ϴ�.
	void Update_Height();
	// ���̸� ������ �����մϴ�.
	void Set_Height(_float fHeight) {
		m_fHalfHeight = fHeight * 0.5f;
	}
	// �� �Լ��� ������ Ʈ�������� _42�� ������ �ٴڿ� ���ڰ� �ٽ��ϴ�.
	_float Get_FloorY() {
		return m_fFloorY;
	}
	_bool isJumpEnd() {
		return m_bJumpEndFlag;
	}
	
private:
	class CTransform*		m_pTransformCom{ nullptr };
	_float					m_fHalfHeight{};
	static vector<_uint>	m_StandableColliderGroupID;

/// <summary>
/// ���� ���� ����
/// </summary>
	_bool	m_bForceSetFloor{};
	_bool	m_bJump{};
	_float	m_fFloorY{};
	_float	m_fStartY{};
	_float	m_fJumpPower{};
	_float	m_fTime{};
	_float	m_fTimeIncreasePerSec{};
	_float	m_fMaxFallSpeedperSec{};

	_bool	m_bNormalChanged{};
	_float3	m_vCurNormal{};

	_bool	m_bJumpEndFlag{};

#ifdef _GRV_TER
	// �ʱ�ȭ���� �ƴ�, �ͷ��� ������ �Է����ֱ� ���� ����ü�Դϴ�. (�ʱ�ȭ�ÿ��� Transform�� �־��ּ���)
	typedef struct tagGravityDesc
	{
		const _float3* pTerrainVtxPos;
		_uint			iTerrainVtxNumX;
		_uint			iTerrainVtxNumZ;
		_float3			vTerrainScale;
	}DESC;
	static const _float3*	m_pTerrainVtxPos;
	static _uint			m_iTerrainVtxNumX;
	static _uint			m_iTerrainVtxNumZ;
	static _float3			m_vTerrainScale;

	static void Set_GravityStaticInfo(const DESC& Desc);
	void Check_Terrain();
#endif


public: //����������
	void Raycast_StandAble_Obj();

private:
	void Jumping(_float fTimeDelta);
	void Update_NormalVector(const D3DXPLANE& Plane);

public:
	static CGravity* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
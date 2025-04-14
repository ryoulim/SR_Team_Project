#pragma once
#include "Component.h"

//#define _GRV_TER
#define GRV_MIN_HEIGHT		-30.f // 맵 최소 높이

BEGIN(Engine)

class ENGINE_DLL CGravity final : public CComponent
{
public:
	typedef struct CGravityDesc
	{
		// 이곳에 트랜스폼을 넣으면 _42 (y축 포지션)에 대해 그래비티가 직접 관여합니다.
		class CTransform* pTransformCom;
		// 초당 증가하는 시간 (포물선 공식의 Time)
		_float	fTimeIncreasePerSec;
		// 초당 최대로 떨어질 수 있는 속도
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
	// 초기화시의 점프 옵션을 바꾸는 옵션.
	void Set_JumpOption(_float	fTimeIncreasePerSec, _float	fMaxFallSpeedPerSec) {
		m_fTimeIncreasePerSec = fTimeIncreasePerSec;
		m_fMaxFallSpeedperSec = fMaxFallSpeedPerSec;
	}

	// 높이를 지금의 스케일에 맞춥니다.
	void Update_Height();
	// 높이를 강제로 조정합니다.
	void Set_Height(_float fHeight) {
		m_fHalfHeight = fHeight * 0.5f;
	}
	// 이 함수의 리턴을 트랜스폼의 _42에 넣으면 바닥에 예쁘게 붙습니다.
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
/// 점프 관련 변수
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
	// 초기화용이 아닌, 터레인 정보를 입력해주기 위한 구조체입니다. (초기화시에는 Transform을 넣어주세요)
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


public: //범죄은닉중
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
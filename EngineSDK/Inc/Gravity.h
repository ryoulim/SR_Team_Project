#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGravity final : public CComponent
{
public:
	// 초기화용이 아닌, 터레인 정보를 입력해주기 위한 구조체입니다. (초기화시에는 Transform을 넣어주세요)
	typedef struct tagGravityDesc
	{
		const _float3*	pTerrainVtxPos;
		_uint			iTerrainVtxNumX;
		_uint			iTerrainVtxNumZ;
		_float3			vTerrainScale;
	}DESC;

	typedef struct tagJumpPack
	{
		_float	fTimeIncreasePerSec;
		_float	fMaxFallSpeedPerSec;
	}JUMPDESC;

private:
	CGravity(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGravity(const CGravity& Prototype);
	virtual ~CGravity() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	void Update(_float fTimeDelta);
	static void Set_GravityStaticInfo(const DESC& Desc);
	static void Add_StandableObjLayerTag(_uint ID) {
		m_StandableColliderGroupID.push_back(ID);
	}

	void Go_Straight_On_Terrain(_float fTimedelta);
	void Go_Backward_On_Terrain(_float fTimedelta);
	void Go_Left_On_Terrain(_float fTimedelta);
	void Go_Right_On_Terrain(_float fTimedelta);

	void Set_JumpOption(const JUMPDESC& JumpDESC);
	void Parabolic_Motion(const _float3* vDirection);
	void Force_Set_FloorY(_float fFloorY);
	void Jump(_float fJumpPower);
	void Stop_Jump() {
		m_bJump = FALSE;
	}
	_bool isJump() {
		return m_bJump;
	}

private:
	class CTransform*		m_pTransformCom{ nullptr };
	_float					m_fHalfHeight{};

	static const _float3*	m_pTerrainVtxPos;
	static _uint			m_iTerrainVtxNumX;
	static _uint			m_iTerrainVtxNumZ;
	static _float3			m_vTerrainScale;
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

private:
	void Check_Terrain();
	void Raycast_StandAble_Obj();
	void Jumping(_float fTimeDelta);
	void Update_NormalVector(const D3DXPLANE& Plane);

public:
	static CGravity* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGravity final : public CComponent
{
public:
	// 초기화용이 아닌, 터레인 정보를 입력해주기 위한 구조체입니다.
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
	static void Set_TerrainInfo(const DESC& Desc);

	void Go_Straight_On_Terrain(_float fTimedelta);
	void Go_Backward_On_Terrain(_float fTimedelta);

	void Set_JumpOption(const JUMPDESC& JumpDESC);
	void Update(_float fTimeDelta);
	void Jump(_float fJumpPower);

private:
	class CTransform*		m_pTransformCom{ nullptr };
	_float					m_fHeight{};

	static const _float3*	m_pTerrainVtxPos;
	static _uint			m_iTerrainVtxNumX;
	static _uint			m_iTerrainVtxNumZ;
	static _float3			m_vTerrainScale;

/// <summary>
/// 점프 관련 변수
/// </summary>
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
	void Jumping(_float fTimeDelta);
	void Update_NormalVector(const D3DXPLANE& Plane);

public:
	static CGravity* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <chrono>
#include <random>

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

using namespace std;
using namespace std::chrono;

class CHitBox : public CGameObject
{
public:
	typedef struct tagMonsterDesc : public CTransform::DESC
	{
		const TCHAR* szTextureTag;
		LEVEL		eLevel;
		_float3		vPosition;
		_float3		vScale;
		_float		fMaxTime;
		_bool		bHitDead;

		COLLIDER_ID eID;
	}DESC;

protected:
	CHitBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHitBox(const CHitBox& Prototype);
	virtual ~CHitBox() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;

public:
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

protected:
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Render();
	virtual HRESULT Release_RenderState();

protected:
	virtual HRESULT Ready_Components(void* pArg);

protected: // 충돌함수
	virtual void		On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public: // 디버깅
	void	ToggleDebugMode() { m_bDebug = !m_bDebug; }

public: // 나눈이유가 있음
	_bool	GetDead() { return m_bDead; }

public:
	static CHitBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();


protected: //기본정보
	LEVEL	m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };
	const _tchar* m_szBufferType = { nullptr };

protected: //컴포넌트
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pCollider = { nullptr };

protected: //속성
	_float3	m_vScale = { 0.f, 0.f, 0.f };
	_float3 m_vPosition = { 0.f, 0.f, 0.f };

protected: //플레이어
	CGameObject* m_pTargetPlayer = nullptr;

protected: // 작동변수
	_bool			m_bHitDead = false;
	_bool			m_bDead = false;

	_float			m_fStartTime = 0.f;
	_float			m_fMaxTime = 0.f;

protected: // 디버깅
	_bool			m_bDebug = false;
};

END

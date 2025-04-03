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

class CHitBox abstract : public CGameObject
{
public:
	typedef struct tagMonsterDesc : public CTransform::DESC
	{
		LEVEL		eLevel;
		_float3		vPosition;
		_float3		vScale;

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

protected: // 난수 생성
	float GetRandomFloat(float min, float max)
	{
		if (min > max) std::swap(min, max);
		uniform_real_distribution<float> dist(min, max);
		return dist(m_Gen);
	}
	int GetRandomInt(int min, int max)
	{
		if (min > max) std::swap(min, max);
		std::uniform_int_distribution<int> dist(min, max); // min 이상 max 이하 정수
		return dist(m_Gen);
	}

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
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

protected: // 랜덤 난수 생성변수
	random_device m_Rd;
	mt19937 m_Gen{ std::random_device{}() };
	uniform_real_distribution<float> m_Dist;

protected: // 작동변수
	_bool			m_bDead = false;

protected: // 디버깅
	_bool			m_bDebug = false;
};

END

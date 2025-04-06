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

protected: // �浹�Լ�
	virtual void		On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public: // �����
	void	ToggleDebugMode() { m_bDebug = !m_bDebug; }

public: // ���������� ����
	_bool	GetDead() { return m_bDead; }

public:
	static CHitBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();


protected: //�⺻����
	LEVEL	m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };
	const _tchar* m_szBufferType = { nullptr };

protected: //������Ʈ
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pCollider = { nullptr };

protected: //�Ӽ�
	_float3	m_vScale = { 0.f, 0.f, 0.f };
	_float3 m_vPosition = { 0.f, 0.f, 0.f };

protected: //�÷��̾�
	CGameObject* m_pTargetPlayer = nullptr;

protected: // �۵�����
	_bool			m_bHitDead = false;
	_bool			m_bDead = false;

	_float			m_fStartTime = 0.f;
	_float			m_fMaxTime = 0.f;

protected: // �����
	_bool			m_bDebug = false;
};

END

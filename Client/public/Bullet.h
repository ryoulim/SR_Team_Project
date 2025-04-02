#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CShader;
END

BEGIN(Client)

class CBullet abstract : public CGameObject
{
public:
	typedef struct tagBulletDesc : public CTransform::DESC
	{
		_float3 vPosition;
		_float3 vScale;
		_uint iColliderID;
	}DESC;

protected:
	CBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBullet(const CBullet& Prototype);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Reset(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void FrameUpdate(_float timeDelta, _float _MaxFrame, _float fSpeed, _bool isLoop);
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

protected:
	virtual HRESULT Ready_Components(void* pArg);

protected:
	LEVEL m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };
	const _tchar* m_szBufferType = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pCollider = { nullptr };
	_bool m_bDead{ FALSE };
	_float m_fTimeAcc{};
	_float m_fTimeLimit{ 3.f };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

protected: // �ִϸ��̼�
	_float	m_fAnimationFrame = 0;
	_float	m_fAnimationMaxFrame = 0;
	_float	m_fAnimationSpeed = 0;
};

END

#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"


BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CEffect abstract : public CGameObject
{
public:
	typedef struct tagEffectDesc : public CTransform::DESC
	{
		_float3 vInitPos;
		_float3 vScale;

	}DESC;

protected:
	CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect(const CEffect& Prototype);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

public: //·»´õ¸µ
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Render();
	virtual HRESULT Release_RenderState();

protected:
	virtual HRESULT Ready_Components(void* pArg);


public:
	void SetPosition(_float3 _Pos);

protected:
	LEVEL m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };
	const _tchar* m_szBufferType = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

};

END

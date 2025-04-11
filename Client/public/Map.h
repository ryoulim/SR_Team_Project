#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

#define BOSSSCALE 22.f
#define INDOORSCALE 14.f
#define OUTDOORSCALE 10.f
#define UNDERGROUNDSCALE 18.f

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CShader;
END

BEGIN(Client)

class CMap abstract : public CGameObject
{
public:
	typedef struct tagMapDesc : public CTransform::DESC
	{
		_float3 vInitPos;
		_float3 vScale;
		_float3 vAngle;
		_bool   bCollision;
		_float  fTextureIdx;
		LEVEL	eLevelID;
	}DESC;

protected:
	CMap(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMap(const CMap& Prototype);
	virtual ~CMap()override;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;

	virtual void Priority_Update(_float fTimeDelta)override;
	virtual EVENT Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

protected:
	virtual HRESULT Ready_Components(void* pArg);

public: /* [ 셰이더를 활용한 렌더링 ] */
	virtual HRESULT DefaultShaderRender();
	virtual HRESULT MultiShaderRender(_float _fTexNum, _int _iSetting);
	virtual HRESULT MultiTextureShaderRender(int _iNums);
	virtual HRESULT BillboardShaderRender();

protected:
	LEVEL m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };
	const _tchar* m_szBufferType = { nullptr };
	_float	  m_fTextureIdx = {};
	//static _bool m_bRenderMode{};

	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

protected: /* [ 안개 셰이더 조절용 ] */
	LEVEL m_eShadingLevel = LEVEL_END;
	_float m_fShaderTime = 0.f;
	_float m_fFlashTime = 0.f;
	_float4 m_vFlashColor = { 0.486f, 0.584f, 0.918f , 1.f };
	_float4 m_vFogColor = { 1.f, 1.f, 1.f, 1.f };
};

END
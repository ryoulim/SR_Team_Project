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

class CStatue abstract : public CGameObject
{
public:
	typedef struct tagStatueDesc : public CTransform::DESC
	{
		_float3 vInitPos;
		_float3 vScale;
		_float3 vAngle;
		_bool   bCollision;
		LEVEL	eLevelID;
	}DESC;

protected:
	CStatue(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStatue(const CStatue& Prototype);
	virtual ~CStatue() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: /* [ 셰이더 렌더링 함수 ] */
	HRESULT DefaultShaderRender();
	HRESULT MultiShaderRender(_float _fTexNum, _int _iSetting);
	HRESULT MultiTextureShaderRender(int _iNums);
	HRESULT BillboardShaderRender();

protected:
	virtual HRESULT Ready_Components(void* pArg);

	void			LightOn();

protected:
	LEVEL m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };
	const _tchar* m_szBufferType = { nullptr };
	_float	  m_fTextureNum = {};

	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

protected: /* [ 안개 셰이더 조절용 ] */
	LEVEL m_eShadingLevel = LEVEL_END;
	_float m_fShaderTime = 0.f;
	_float m_fTextureIdx = {};
	_float m_fThunderRange = 1000.f;
	_float4 m_vFlashColor = { 0.486f, 0.584f, 0.918f , 1.f};
	_float4 m_vFogColor = { 1.f, 1.f, 1.f, 1.f };
};

END

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

class CUI abstract : public CGameObject
{
public:
	//enum TYPE { PRIORITY, EFFECT, BUTTON, OBJECT, BACKGROUND };

	typedef struct tagUIDesc : public CTransform::DESC
	{
		_float3 vInitPos;
		_float3 vScale;
		_float	fDepth;
		LEVEL	eLevelID;
	}DESC;

protected:
	CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI(const CUI& Prototype);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT	Ready_ShaderComponent();
    virtual void Update_Rect();

protected:
	LEVEL           m_eLevelID = { LEVEL_END };
	const _tchar*   m_szTextureID = { nullptr };
	const _tchar*   m_szBufferType = { nullptr };
	CTexture*       m_pTextureCom = { nullptr };
	CVIBuffer*      m_pVIBufferCom = { nullptr };
	CTransform*     m_pTransformCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	_float3         m_vPos = {};
    _float3         m_vSize = {};
    RECT            m_tRect = {};

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();
};

END

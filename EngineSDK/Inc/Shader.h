#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	enum TYPE {COLOR, ALPHA, MASKING, SHADE, SANDEVISTAN};
	enum DH_TYPE {SMOKE, BOSSMISSILE, DEBUG, FIREATTECK, HIT, HEAL ,WATER, FOG};
private:
	CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader(const CShader& Prototype);
	virtual ~CShader() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize(void* pArg) override;

	HRESULT Bind_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture);
	HRESULT Bind_Matrix(D3DXHANDLE hParameter, const _float4x4* pMatrix);
	HRESULT SetFloat(D3DXHANDLE hParameter, _float fValue);
	HRESULT SetVector(D3DXHANDLE hParameter, D3DXVECTOR4* fVector);
	HRESULT GetTexture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9* fVector);
	HRESULT SetFloatArray(D3DXHANDLE hParameter, const _float* fValueArray, _uint iNum);


public:
	void Begin(_uint iPassIndex);
	void End();


private:
	LPD3DXEFFECT				m_pEffect = { nullptr };

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};


END
#pragma once

#include "Component.h"
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_CUBE, TYPE_2D, TYPE_END };

private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures, TYPE eType = TYPE_2D);
	virtual HRESULT Initialize(void* pArg) override;
public:
	HRESULT Bind_Resource(_uint iIndex);
	HRESULT	Get_TextureSize(_uint iIndex, _float3* pOut);

	// Effect컴객체(m_pEffect), GetParameterByName으로 받아온 D3DXHANDLE 핸들, 바인딩 할 텍스쳐 인덱스
	//HRESULT Bind_Shader_To_Texture(LPD3DXEFFECT pEffect, D3DXHANDLE hTexParameter, _uint iIndex);
	HRESULT Bind_Shader_To_Texture(CShader* pShader, D3DXHANDLE hTexParameter, _uint iIndex);
	HRESULT Bind_Shader_To_Reset(CShader* pShader, D3DXHANDLE hTexParameter);
	IDirect3DBaseTexture9* Get_Texture(_uint iIndex){return m_Textures[iIndex];}

private:
	_uint							m_iNumTextures = { };
	vector<LPDIRECT3DTEXTURE9>		m_Textures;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTextureFilePath, _uint iNumTextures, TYPE eType = TYPE_2D);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
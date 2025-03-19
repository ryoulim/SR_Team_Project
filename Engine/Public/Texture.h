#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;
public:
	HRESULT Bind_Resource(_uint iIndex);
	HRESULT	Get_TextureSize(_uint iIndex, _float3* ppOut);

	// Effect컴객체(m_pEffect), GetParameterByName으로 받아온 D3DXHANDLE 핸들, 바인딩 할 텍스쳐 인덱스
	HRESULT Bind_Shader_To_Texture(LPD3DXEFFECT pEffect, D3DXHANDLE hTexParameter, _uint iIndex);

private:
	_uint							m_iNumTextures = { };
	vector<LPDIRECT3DTEXTURE9>		m_Textures;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
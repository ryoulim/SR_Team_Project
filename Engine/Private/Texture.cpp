#include "Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent { pGraphic_Device }
{

}

CTexture::CTexture(const CTexture& Prototype)
    : CComponent(Prototype)
	, m_iNumTextures { Prototype.m_iNumTextures }
	, m_Textures { Prototype.m_Textures }
{
	for (auto& pTexture : m_Textures)	
		Safe_AddRef(pTexture);	
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures, TYPE eType)
{
	m_iNumTextures = iNumTextures;

	for (size_t i = 0; i < m_iNumTextures; i++)
	{
		LPDIRECT3DTEXTURE9		pTexture = { nullptr };

		_tchar			szTextureFileName[MAX_PATH] = {};

		wsprintf(szTextureFileName, pTextureFilePath, i);


		if (eType == TYPE_2D)
		{
			D3DXIMAGE_INFO imageInfo;

			// 먼저 이미지 정보를 가져옴
			if (FAILED(D3DXGetImageInfoFromFile(szTextureFileName, &imageInfo)))
				return E_FAIL; // 이미지 정보를 가져오지 못한 경우

			// 비정규 텍스처 크기를 유지하도록 텍스처 생성
			if (FAILED(D3DXCreateTextureFromFileEx(
				m_pGraphic_Device,
				szTextureFileName,
				imageInfo.Width,  // 원본 너비
				imageInfo.Height, // 원본 높이
				D3DX_DEFAULT,
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				&imageInfo,
				nullptr,
				&pTexture
			))) {
				return E_FAIL; // 텍스처 생성 실패
			}
		}
		else
		{
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szTextureFileName, reinterpret_cast<IDirect3DCubeTexture9**>(&pTexture))))
				return E_FAIL;
		}
		//if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szTextureFileName, &pTexture)))
		//	return E_FAIL;

		m_Textures.push_back(pTexture);
	}

    return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CTexture::Bind_Resource(_uint iIndex)
{
	if (iIndex >= m_iNumTextures)
		return E_FAIL;

	if (nullptr == m_Textures[iIndex])
		MSG_BOX("살려다오");

	return m_pGraphic_Device->SetTexture(0, m_Textures[iIndex]);		
}

HRESULT CTexture::Get_TextureSize(_uint iIndex, _float3* pOut)
{
 	if (iIndex >= m_iNumTextures)
		return E_FAIL;

	D3DSURFACE_DESC Desc{};
	if (FAILED(m_Textures[iIndex]->GetLevelDesc(0, &Desc)))
		return E_FAIL;
	

	pOut->x = static_cast<_float>(Desc.Width);
	pOut->y = static_cast<_float>(Desc.Height);

	return S_OK;
}

//HRESULT CTexture::Bind_Shader_To_Texture(LPD3DXEFFECT pEffect, D3DXHANDLE hTexParameter, _uint iIndex)
//{
//	return pEffect->SetTexture(hTexParameter, m_Textures[iIndex]);
//}


HRESULT CTexture::Bind_Shader_To_Texture(CShader* pShader, D3DXHANDLE hTexParameter, _uint iIndex)
{
	return pShader->Bind_Texture(hTexParameter, m_Textures[iIndex]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTextureFilePath, _uint iNumTextures, TYPE eType)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures, eType)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
    __super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
}

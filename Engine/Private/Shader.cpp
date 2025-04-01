#include "Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent { pGraphic_Device }
{
}

CShader::CShader(const CShader& Prototype)
    : CComponent( Prototype )
    , m_pEffect { Prototype.m_pEffect }
{
    Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, pShaderFilePath, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, nullptr)))
		return E_FAIL;

    return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CShader::Bind_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture)
{
	return m_pEffect->SetTexture(hParameter, pTexture);
}

HRESULT CShader::Bind_Matrix(D3DXHANDLE hParameter, const _float4x4* pMatrix)
{
	return m_pEffect->SetMatrix(hParameter, pMatrix);
	
}

HRESULT CShader::SetFloat(D3DXHANDLE hParameter, _float fValue)
{
	return m_pEffect->SetFloat(hParameter, fValue);
}

HRESULT CShader::SetVector(D3DXHANDLE hParameter, D3DXVECTOR4* fVector)
{
	return m_pEffect->SetVector(hParameter, fVector);
}

HRESULT CShader::GetTexture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9* fVector)
{
	m_pEffect->GetTexture(hParameter, fVector);
	return E_NOTIMPL;
}

HRESULT CShader::SetFloatArray(D3DXHANDLE hParameter, const _float* fValueArray, _uint iNum)
{
	return m_pEffect->SetFloatArray(hParameter, fValueArray, iNum);
}

void CShader::Begin(_uint iPassIndex)
{
	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(iPassIndex);
}

void CShader::End()
{
	m_pEffect->EndPass();
	m_pEffect->End();
}

CShader* CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath)
{
	CShader* pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
    __super::Free();

    Safe_Release(m_pEffect);
}

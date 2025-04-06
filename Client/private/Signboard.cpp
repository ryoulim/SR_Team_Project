#include "Signboard.h"

CSignboard::CSignboard(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue { pGraphic_Device }
{
}

CSignboard::CSignboard(const CSignboard& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CSignboard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSignboard::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Signboard");
	m_szBufferType = TEXT("Signboard");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSignboard::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CSignboard::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CSignboard::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSignboard::Render()
{
	for (int i = 0; i < 5; i++)
	{
		SignBoard_Render(static_cast<float>(i), i);
	}
	return S_OK;
}

HRESULT CSignboard::SignBoard_Render(_float _fTexNum, _int _iSetting)
{
	if (m_eShadingLevel == LEVEL_RACEFIRST || m_eShadingLevel == LEVEL_RACESECOND || m_eShadingLevel == LEVEL_RACETHIRD)
	{
		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(_fTexNum))))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render(_iSetting)))
			return E_FAIL;
	}
	else
	{
		/* [ 텍스처 셰이더로 넘기기 ] */
		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", (_uint)_fTexNum);

		/* [ 메트릭스 셰이더로 넘기기 ] */
		_float4x4 matWorld, maxView, maxProj;
		matWorld = *m_pTransformCom->Get_WorldMatrix();

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &maxView);
		m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &maxProj);

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &matWorld)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &maxView)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &maxProj)))
			return E_FAIL;

		/* [ 카메라 위치 넘기기 ] */
		_float3 vCamPos;
		_float4x4 invView;
		D3DXMatrixInverse(&invView, nullptr, &maxView);
		vCamPos = *(_float3*)&invView._41;

		if (FAILED(m_pShaderCom->SetFloatArray("g_CameraPos", (_float*)&vCamPos, 3)))
			return E_FAIL;

		/* [ 안개 거리 설정 ] */
		_float fFogStart = 0.f;
		_float fFogEnd = 0.f;

		if (m_eShadingLevel == LEVEL_OUTDOOR)
		{
			fFogStart = 0.f;
			fFogEnd = 700.f;
		}
		else if (m_eShadingLevel == LEVEL_GAMEPLAY)
		{
			fFogStart = 1000.f;
			fFogEnd = 2500.f;
		}
		else if (m_eShadingLevel == LEVEL_INDOOR)
		{
			fFogStart = 300.f;
			fFogEnd = 1500.f;
		}
		else
		{
			fFogStart = 0.f;
			fFogEnd = 700.f;
		}

		if (FAILED(m_pShaderCom->SetFloat("g_FogStart", fFogStart)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetFloat("g_FogEnd", fFogEnd)))
			return E_FAIL;

		/* [ 안개 색상 설정 ] */
		_float4 vFogColor = _float4(0.059f, 0.067f, 0.082f, 1.f);
		if (FAILED(m_pShaderCom->SetVector("g_FogColor", &vFogColor)))
			return E_FAIL;

		//셰이더 시작
		m_pShaderCom->Begin(CShader::FOG);

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;


		/* ------------------------------------------------------------------------- */

		if (FAILED(m_pVIBufferCom->Render(_iSetting)))
			return E_FAIL;

		/* ------------------------------------------------------------------------- */

		m_pShaderCom->End();
	}
	return S_OK;
}

CSignboard* CSignboard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSignboard* pInstance = new CSignboard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSignboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSignboard::Clone(void* pArg)
{
	CSignboard* pInstance = new CSignboard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CSignboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSignboard::Free()
{
	__super::Free();
}

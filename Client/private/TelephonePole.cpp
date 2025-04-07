// 내 클래스 이름 : TelephonePole
// 부모 클래스 이름 : Map

#include "TelephonePole.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "Player.h"
#include "FXMgr.h"

CTelephonePole::CTelephonePole(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CTelephonePole::CTelephonePole(const CTelephonePole& Prototype)
	: CMap(Prototype)
{
}

HRESULT CTelephonePole::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTelephonePole::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTelephonePole::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTelephonePole::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CTelephonePole::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTelephonePole::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (!static_cast<CPlayer*>(GET_PLAYER)->GetbFog())
	{
		if (FAILED(m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard())))
			return E_FAIL;

		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureIdx))))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		return S_OK;
	}

	if (m_eShadingLevel == LEVEL_RACEFIRST || m_eShadingLevel == LEVEL_RACESECOND || m_eShadingLevel == LEVEL_RACETHIRD)
	{
		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}
	else
	{
		/* [ 텍스처 셰이더로 넘기기 ] */
		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", (_uint)m_fTextureIdx);

		/* [ 메트릭스 셰이더로 넘기기 ] */
		_float4x4 matWorld, maxView, maxProj;
		matWorld = *m_pTransformCom->Get_WorldMatrix();

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &maxView);
		m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &maxProj);

		_float4x4 matBillboard = m_pTransformCom->Billboard();
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &matBillboard)))
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

		m_fShaderTime += m_pGameInstance->Get_Scaled_TimeDelta(TEXT("Timer_60"));
		if (FAILED(m_pShaderCom->SetFloat("g_fTime", m_fShaderTime)))
			return E_FAIL;

		D3DXVECTOR4 ThunderPos = { FX_MGR->GetThunderPos(), 0.f };
		if (FAILED(m_pShaderCom->SetVector("g_LightningPos", &ThunderPos)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetFloat("g_FlashIntensity", 0.5f)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetFloat("g_LightRange", 600.f)))
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

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		/* ------------------------------------------------------------------------- */

		m_pShaderCom->End();
	}


	return S_OK;

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

#ifdef _COLLIDERRENDER
	if (m_pColliderCom)
		m_pColliderCom->Render();
#endif

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CTelephonePole* CTelephonePole::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTelephonePole* pInstance = new CTelephonePole(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTelephonePole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTelephonePole::Clone(void* pArg)
{
	CTelephonePole* pInstance = new CTelephonePole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTelephonePole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTelephonePole::Free()
{
	__super::Free();
}
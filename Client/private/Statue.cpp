#include "Statue.h"
#include "FXMgr.h"

CStatue::CStatue(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CStatue::CStatue(const CStatue& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CStatue::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStatue::Initialize(void* pArg)
{
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CStatue::Priority_Update(_float fTimeDelta)
{
}

EVENT CStatue::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CStatue::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CStatue::Render()
{
	DefaultShaderRender();

	return S_OK;
}

HRESULT CStatue::DefaultShaderRender()
{
	if (!g_FogTrigger || LEVEL_RACING(m_eShadingLevel))
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

		return S_OK;
	}
	else
	{
		/* [ 텍스처 셰이더로 넘기기 ] */
		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", (_uint)m_fTextureNum);

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

		if (m_eShadingLevel == LEVEL_INDOOR)
		{
			fFogStart = INDOOR_START_FOG;
			fFogEnd = INDOOR_END_FOG;
		}
		else if (m_eShadingLevel == LEVEL_OUTDOOR)
		{
			fFogStart = OUTDOOR_START_FOG;
			fFogEnd = OUTDOOR_END_FOG;
			if (g_FogCustom <= 1995.f)
			{
				/* g_FogCustom 은 0까지 줄어든다 */
				fFogStart = g_FogCustom;
				fFogEnd = g_FogCustom + 700;
			}
		}
		else if (m_eShadingLevel == LEVEL_GAMEPLAY)
		{
			fFogStart = GAMEPLAY_START_FOG;
			fFogEnd = GAMEPLAY_END_FOG;
		}
		else if (m_eShadingLevel == LEVEL_UNDERGROUND)
		{
			fFogStart = UNDERGROUND_START_FOG;
			fFogEnd = UNDERGROUND_END_FOG;
		}
		else
		{
			fFogStart = 10000.f;
			fFogEnd = 15000.f;
		}

		if (FAILED(m_pShaderCom->SetFloat("g_FogStart", fFogStart)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetFloat("g_FogEnd", fFogEnd)))
			return E_FAIL;

		m_fShaderTime += m_pGameInstance->Get_Scaled_TimeDelta(TEXT("Timer_60"));
		if (FAILED(m_pShaderCom->SetFloat("g_fTime", m_fShaderTime)))
			return E_FAIL;

		/* [ 플래쉬 설정 ] */
		D3DXVECTOR4 ThunderPos;
		if (m_eLevelID == LEVEL_UNDERGROUND)
		{
			if (FX_MGR->IsFlashing())
			{
				ThunderPos = { *GET_PLAYER_TRANSFORM->Get_State(CTransform::STATE_POSITION), 0.f };
			}
			else
			{
				ThunderPos = { 0.f, 10000.f, 0.f, 0.f };
			}
		}
		if (m_eLevelID == LEVEL_OUTDOOR)
		{
			if (FX_MGR->IsThunder())
			{
				ThunderPos = { FX_MGR->GetThunderPos(), 0.f };
			}
			else
			{
				ThunderPos = { 0.f, 10000.f, 0.f, 0.f };
			}
		}

		if (m_eLevelID != LEVEL_OUTDOOR && m_eLevelID != LEVEL_UNDERGROUND)
			ThunderPos = { 0.f, 10000.f, 0.f, 0.f };

		if (FAILED(m_pShaderCom->SetVector("g_LightningPos", &ThunderPos)))
			return E_FAIL;


		/* [ 조명 세기 설정 ] */
		_float LigtIntensity = 0.5f;
		if (m_eLevelID == LEVEL_UNDERGROUND)
			LigtIntensity = 1.f;
		if (FAILED(m_pShaderCom->SetFloat("g_FlashIntensity", LigtIntensity)))
			return E_FAIL;


		/* [ 조명 거리 설정 ] */
		_float LigtRange = m_fThunderRange;
		if (m_eLevelID == LEVEL_UNDERGROUND)
			LigtRange = 350.f;

		if (FAILED(m_pShaderCom->SetFloat("g_LightRange", LigtRange)))
			return E_FAIL;
		/* [ 안개 색상 설정 ] */
		m_vFogColor = _float4(0.059f, 0.067f, 0.082f, 1.f);
		if (m_eLevelID == LEVEL_UNDERGROUND)
			m_vFogColor = _float4(0.10f, 0.11f, 0.13f, 1.f);
		if (FAILED(m_pShaderCom->SetVector("g_FogColor", &m_vFogColor)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetVector("g_FlashColor", &m_vFlashColor)))
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
}
HRESULT CStatue::MultiShaderRender(_float _fTexNum, _int _iSetting)
{
	if (!g_FogTrigger || LEVEL_RACING(m_eShadingLevel))
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

		return S_OK;
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

		if (m_eShadingLevel == LEVEL_INDOOR)
		{
			fFogStart = INDOOR_START_FOG;
			fFogEnd = INDOOR_END_FOG;
		}
		else if (m_eShadingLevel == LEVEL_OUTDOOR)
		{
			fFogStart = OUTDOOR_START_FOG;
			fFogEnd = OUTDOOR_END_FOG;
			if (g_FogCustom <= 1995.f)
			{
				/* g_FogCustom 은 0까지 줄어든다 */
				fFogStart = g_FogCustom;
				fFogEnd = g_FogCustom + 700;
			}
		}
		else if (m_eShadingLevel == LEVEL_GAMEPLAY)
		{
			fFogStart = GAMEPLAY_START_FOG;
			fFogEnd = GAMEPLAY_END_FOG;
		}
		else if (m_eShadingLevel == LEVEL_UNDERGROUND)
		{
			fFogStart = UNDERGROUND_START_FOG;
			fFogEnd = UNDERGROUND_END_FOG;
		}
		else
		{
			fFogStart = 10000.f;
			fFogEnd = 15000.f;
		}

		if (FAILED(m_pShaderCom->SetFloat("g_FogStart", fFogStart)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetFloat("g_FogEnd", fFogEnd)))
			return E_FAIL;

		m_fShaderTime += m_pGameInstance->Get_Scaled_TimeDelta(TEXT("Timer_60"));
		if (FAILED(m_pShaderCom->SetFloat("g_fTime", m_fShaderTime)))
			return E_FAIL;

		/* [ 플래쉬 설정 ] */
		D3DXVECTOR4 ThunderPos;
		if (m_eLevelID == LEVEL_UNDERGROUND)
		{
			if (FX_MGR->IsFlashing())
			{
				ThunderPos = { *GET_PLAYER_TRANSFORM->Get_State(CTransform::STATE_POSITION), 0.f };
			}
			else
			{
				ThunderPos = { 0.f, 10000.f, 0.f, 0.f };
			}
		}
		if (m_eLevelID == LEVEL_OUTDOOR)
		{
			if (FX_MGR->IsThunder())
			{
				ThunderPos = { FX_MGR->GetThunderPos(), 0.f };
			}
			else
			{
				ThunderPos = { 0.f, 10000.f, 0.f, 0.f };
			}
		}

		if (m_eLevelID != LEVEL_OUTDOOR && m_eLevelID != LEVEL_UNDERGROUND)
			ThunderPos = { 0.f, 10000.f, 0.f, 0.f };

		if (FAILED(m_pShaderCom->SetVector("g_LightningPos", &ThunderPos)))
			return E_FAIL;


		/* [ 조명 세기 설정 ] */
		_float LigtIntensity = 0.5f;
		if (m_eLevelID == LEVEL_UNDERGROUND)
			LigtIntensity = 1.f;
		if (FAILED(m_pShaderCom->SetFloat("g_FlashIntensity", LigtIntensity)))
			return E_FAIL;


		/* [ 조명 거리 설정 ] */
		_float LigtRange = m_fThunderRange;
		if (m_eLevelID == LEVEL_UNDERGROUND)
			LigtRange = 350.f;

		if (FAILED(m_pShaderCom->SetFloat("g_LightRange", LigtRange)))
			return E_FAIL;
		/* [ 안개 색상 설정 ] */
		m_vFogColor = _float4(0.059f, 0.067f, 0.082f, 1.f);
		if (m_eLevelID == LEVEL_UNDERGROUND)
			m_vFogColor = _float4(0.10f, 0.11f, 0.13f, 1.f);
		if (FAILED(m_pShaderCom->SetVector("g_FogColor", &m_vFogColor)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetVector("g_FlashColor", &m_vFlashColor)))
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
HRESULT CStatue::MultiTextureShaderRender(int _iNums)
{
	for (int i = 0; i < _iNums; i++)
	{
		MultiShaderRender(static_cast<float>(i), i);
	}
	return S_OK;
}
HRESULT CStatue::BillboardShaderRender()
{
	if (!g_FogTrigger || LEVEL_RACING(m_eShadingLevel))
	{
		if (FAILED(m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard())))
			return E_FAIL;

		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		return S_OK;
	}
	else
	{
		/* [ 텍스처 셰이더로 넘기기 ] */
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", (_uint)m_fTextureNum);

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

		if (m_eShadingLevel == LEVEL_INDOOR)
		{
			fFogStart = INDOOR_START_FOG;
			fFogEnd = INDOOR_END_FOG;
		}
		else if (m_eShadingLevel == LEVEL_OUTDOOR)
		{
			fFogStart = OUTDOOR_START_FOG;
			fFogEnd = OUTDOOR_END_FOG;
			if (g_FogCustom <= 1995.f)
			{
				/* g_FogCustom 은 0까지 줄어든다 */
				fFogStart = g_FogCustom;
				fFogEnd = g_FogCustom + 700;
			}
		}
		else if (m_eShadingLevel == LEVEL_GAMEPLAY)
		{
			fFogStart = GAMEPLAY_START_FOG;
			fFogEnd = GAMEPLAY_END_FOG;
		}
		else if (m_eShadingLevel == LEVEL_UNDERGROUND)
		{
			fFogStart = UNDERGROUND_START_FOG;
			fFogEnd = UNDERGROUND_END_FOG;
		}
		else
		{
			fFogStart = 10000.f;
			fFogEnd = 15000.f;
		}

		if (FAILED(m_pShaderCom->SetFloat("g_FogStart", fFogStart)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetFloat("g_FogEnd", fFogEnd)))
			return E_FAIL;

		m_fShaderTime += m_pGameInstance->Get_Scaled_TimeDelta(TEXT("Timer_60"));
		if (FAILED(m_pShaderCom->SetFloat("g_fTime", m_fShaderTime)))
			return E_FAIL;

		/* [ 플래쉬 설정 ] */
		D3DXVECTOR4 ThunderPos;
		if (m_eLevelID == LEVEL_UNDERGROUND)
		{
			if (FX_MGR->IsFlashing())
			{
				ThunderPos = { *GET_PLAYER_TRANSFORM->Get_State(CTransform::STATE_POSITION), 0.f };
			}
			else
			{
				ThunderPos = { 0.f, 10000.f, 0.f, 0.f };
			}
		}
		if (m_eLevelID == LEVEL_OUTDOOR)
		{
			if (FX_MGR->IsThunder())
			{
				ThunderPos = { FX_MGR->GetThunderPos(), 0.f };
			}
			else
			{
				ThunderPos = { 0.f, 10000.f, 0.f, 0.f };
			}
		}

		if (m_eLevelID != LEVEL_OUTDOOR && m_eLevelID != LEVEL_UNDERGROUND)
			ThunderPos = { 0.f, 10000.f, 0.f, 0.f };

		if (FAILED(m_pShaderCom->SetVector("g_LightningPos", &ThunderPos)))
			return E_FAIL;


		/* [ 조명 세기 설정 ] */
		_float LigtIntensity = 0.5f;
		if (m_eLevelID == LEVEL_UNDERGROUND)
			LigtIntensity = 1.f;
		if (FAILED(m_pShaderCom->SetFloat("g_FlashIntensity", LigtIntensity)))
			return E_FAIL;


		/* [ 조명 거리 설정 ] */
		_float LigtRange = m_fThunderRange;
		if (m_eLevelID == LEVEL_UNDERGROUND)
			LigtRange = 350.f;

		if (FAILED(m_pShaderCom->SetFloat("g_LightRange", LigtRange)))
			return E_FAIL;
		/* [ 안개 색상 설정 ] */
		m_vFogColor = _float4(0.059f, 0.067f, 0.082f, 1.f);
		if (m_eLevelID == LEVEL_UNDERGROUND)
			m_vFogColor = _float4(0.10f, 0.11f, 0.13f, 1.f);
		if (FAILED(m_pShaderCom->SetVector("g_FogColor", &m_vFogColor)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetVector("g_FlashColor", &m_vFlashColor)))
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
}


HRESULT CStatue::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	//셰이더 장착
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_pTransformCom->QurternionRotation(pDesc->vAngle);
		m_eShadingLevel = pDesc->eLevelID;
	}

	return S_OK;
}

void CStatue::LightOn()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

#pragma region 조명 설치

#pragma region 조명의 속성 정의
	D3DLIGHT9 LightDesc{};

	// 광원의 타입 : 점 광원
	LightDesc.Type = D3DLIGHT_POINT;

	// 광원의 위치
	LightDesc.Position = _float3(500.f, 200.f, 500.f);

	// 조명이 미치는 범위
	LightDesc.Range = 400.f;

	// 광원으로부터 멀어질 때의 감쇄값
	LightDesc.Attenuation1 = 0.00000001f;

	// 기본 설정
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
#pragma endregion

	// 0번 광원을 설치한다.
	m_pGraphic_Device->SetLight(0, &LightDesc);

#pragma region 재질 설정
	D3DMATERIAL9		MtrlDesc{};
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
#pragma endregion

#pragma endregion

	// 장치에 재질을 설정한다
	m_pGraphic_Device->SetMaterial(&MtrlDesc);

	// 0번 광원을 켠다
	m_pGraphic_Device->LightEnable(0, true);
}

void CStatue::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
}
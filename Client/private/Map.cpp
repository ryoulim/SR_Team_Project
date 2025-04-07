#include "Map.h"
#include "FXMgr.h"
#include "Client_Defines.h"
#include "Player.h"

CMap::CMap(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{pGraphic_Device}
{
}

CMap::CMap(const CMap& Prototype)
	:CGameObject(Prototype)
{
}

CMap::~CMap()
{
}

HRESULT CMap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMap::Initialize(void* pArg)
{
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMap::Priority_Update(_float fTimeDelta)
{
}

EVENT CMap::Update(_float fTimeDelta)
{
    return EVN_NONE;
}

void CMap::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CMap::Render()
{
	if (!static_cast<CPlayer*>(GET_PLAYER)->GetbFog())
	{
		if (FAILED(m_pTransformCom->Bind_Resource()))
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




#ifdef _COLLIDERRENDER
	if(m_pColliderCom)
		m_pColliderCom->Render();
#endif

	return S_OK;
}

void CMap::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{

	if (OtherColliderID == CI_LOVERBOY ||
		OtherColliderID == CI_CHAINGUN ||
		OtherColliderID == CI_DISPENSOR_SHELL)
	{
		/* [ 충돌한 면의 노말 벡터 ] */
		_float3 vLook = CCollider::Get_Last_Collision_Depth().Normalize();


		/* 콘크리트 총알 자국_0 */
		if (m_fTextureIdx == 98 || m_fTextureIdx == 62)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 0);
		}
		/* 유리 총알 자국 */
		else if (m_fTextureIdx == 116)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 1);
		}
		/* 콘크리트 총알 자국_1 */
		else if (m_fTextureIdx == 93 || m_fTextureIdx == 99 || m_fTextureIdx == 127)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 2);
		}
		/* 콘크리트 총알 자국_2 */
		else if (m_fTextureIdx == 108 || m_fTextureIdx == 109)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 3);
		}
		/* 나무 총알 자국 */
		else if (m_fTextureIdx == 32 || m_fTextureIdx == 128 || m_fTextureIdx == 130 || m_fTextureIdx == 132 || m_fTextureIdx == 133)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 4);
		}
		/* 콘크리트 총알 자국_3 */
		else if (m_fTextureIdx == 0 || m_fTextureIdx == 18 || m_fTextureIdx == 118)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 5);
		}
		/* 금속 총알 자국 */
		else if (m_fTextureIdx == 20 || m_fTextureIdx == 137 || m_fTextureIdx == 2 || m_fTextureIdx == 23 || m_fTextureIdx == 25 ||
			m_fTextureIdx == 110 || m_fTextureIdx == 114 || m_fTextureIdx == 115 || m_fTextureIdx == 120)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 6);
		}
		else
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 0);
		}

	}







}

HRESULT CMap::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (TEXT("Terrain") == m_szBufferType || TEXT("RaceTerrain") == m_szBufferType)
	{
		if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;
	}

	else
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
		m_pTransformCom->QurternionRotation(pDesc->vAngle);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (pDesc->vInitPos));

		m_fTextureIdx = pDesc->fTextureIdx;
		m_eShadingLevel = pDesc->eLevelID;
	}

	return S_OK;
}

void CMap::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
}

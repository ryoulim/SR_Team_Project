#include "Statue.h"
#include "Player.h"
#include "Client_Defines.h"

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

	if (!static_cast<CPlayer*>(GET_PLAYER)->GetbFog())
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
		/* [ �ؽ�ó ���̴��� �ѱ�� ] */
		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", (_uint)m_fTextureNum);

		/* [ ��Ʈ���� ���̴��� �ѱ�� ] */
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

		/* [ ī�޶� ��ġ �ѱ�� ] */
		_float3 vCamPos;
		_float4x4 invView;
		D3DXMatrixInverse(&invView, nullptr, &maxView);
		vCamPos = *(_float3*)&invView._41;

		if (FAILED(m_pShaderCom->SetFloatArray("g_CameraPos", (_float*)&vCamPos, 3)))
			return E_FAIL;

		/* [ �Ȱ� �Ÿ� ���� ] */
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

		/* [ �Ȱ� ���� ���� ] */
		_float4 vFogColor = _float4(0.059f, 0.067f, 0.082f, 1.f);
		if (FAILED(m_pShaderCom->SetVector("g_FogColor", &vFogColor)))
			return E_FAIL;

		//���̴� ����
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

	//���̴� ����
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CStatue::LightOn()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

#pragma region ���� ��ġ

#pragma region ������ �Ӽ� ����
	D3DLIGHT9 LightDesc{};

	// ������ Ÿ�� : �� ����
	LightDesc.Type = D3DLIGHT_POINT;

	// ������ ��ġ
	LightDesc.Position = _float3(500.f, 200.f, 500.f);

	// ������ ��ġ�� ����
	LightDesc.Range = 400.f;

	// �������κ��� �־��� ���� ���Ⱚ
	LightDesc.Attenuation1 = 0.00000001f;

	// �⺻ ����
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
#pragma endregion

	// 0�� ������ ��ġ�Ѵ�.
	m_pGraphic_Device->SetLight(0, &LightDesc);

#pragma region ���� ����
	D3DMATERIAL9		MtrlDesc{};
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
#pragma endregion

#pragma endregion

	// ��ġ�� ������ �����Ѵ�
	m_pGraphic_Device->SetMaterial(&MtrlDesc);

	// 0�� ������ �Ҵ�
	m_pGraphic_Device->LightEnable(0, true);
}

void CStatue::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
}
#include "Effect.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CEffect::CEffect(const CEffect& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->Scaling(pDesc->vScale);
	}

	return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{
}

EVENT CEffect::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CEffect::Late_Update(_float fTimeDelta)
{
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CEffect::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);


	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	m_pTransformCom->Billboard();
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CEffect::Ready_Components(void* pArg)
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

	return S_OK;
}
void CEffect::SetPosition(_float3 _Pos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _Pos);
}

void CEffect::Billboard()
{
	//객체 스케일
	_float3	vScaled = m_pTransformCom->Compute_Scaled();
	//객체 포지션
	_float3	vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//카메라 포지션
	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	//카메라를 바라보는 룩벡터
	_float3		vLook = vCameraPos - vPosition;

	//라이트
	_float3		vRight = {};
	_float3		vUpDir{ 0.f, 1.f, 0.f };
	D3DXVec3Cross(&vRight, &vUpDir, &vLook);

	//업벡터를 구함
	_float3		vUp = {0.f, 1.f, 0.f};

	//각 축을 노말라이즈 x 스케일값으로 세팅

	_float3 vecRight = *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x;
	_float3 vecUp = *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x;
	_float3 vecLook = *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScaled.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScaled.z);
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
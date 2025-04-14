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

HRESULT CEffect::SetUp_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//if(LEVEL_RACING(m_pGameInstance->Get_CurrentLevelIndex()))
	//	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CEffect::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD,&m_pTransformCom->Billboard());

	SetUp_RenderState();
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	Release_RenderState();

	return S_OK;
}

HRESULT CEffect::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	if (LEVEL_RACING(m_pGameInstance->Get_CurrentLevelIndex()))
		m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

void CEffect::FrameUpdate(_float timeDelta, _float _MaxFrame, _float fSpeed, _bool isLoop)
{
	if (isLoop)
	{
		//현재 프레임이 맥스프레임보다 크면 ? 0 으로 바꿔라.
		if (_MaxFrame <= m_fAnimationFrame)
		{
			m_fAnimationFrame -= _MaxFrame;
		}
	}
	else
	{
		//반복이 아니면 그냥 리턴 
		if (_MaxFrame <= m_fAnimationFrame)
			return;
	}

	m_fAnimationFrame += fSpeed * timeDelta;

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

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
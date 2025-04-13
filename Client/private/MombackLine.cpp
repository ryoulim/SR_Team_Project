#include "MombackLine.h"
#include "GameInstance.h"
#include "RaceBossBomb.h"
#include "FXMgr.h"

CMombackLine::CMombackLine(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CMombackLine::CMombackLine(const CMombackLine& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CMombackLine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMombackLine::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RedPad");
	m_szBufferType = TEXT("Rect");

	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
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

	m_pPlayer = GET_PLAYER;
	Safe_AddRef(m_pPlayer);

	m_fTextureNum = 0.f;

	return S_OK;
}

void CMombackLine::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMombackLine::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	/* [ 프레임 업데이트 ] */
	if (!m_bFrame)
	{
		m_fTextureNum += 5.f * fTimeDelta;
		if (10.f <= m_fTextureNum)
		{
			m_bFrame = true;
		}
	}
	else if(m_bFrame)
	{
		m_fTextureNum -= 10.f * fTimeDelta;
		if (0.f >= m_fTextureNum)
		{
			return EVN_DEAD;
		}
	}

	return __super::Update(fTimeDelta);
}

void CMombackLine::Late_Update(_float fTimeDelta)
{
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CMombackLine::SetUp_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}


HRESULT CMombackLine::Render()
{
	SetUp_RenderState();
	return __super::Render();
	Release_RenderState();
}

HRESULT CMombackLine::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	return S_OK;
}

CMombackLine* CMombackLine::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMombackLine* pInstance = new CMombackLine(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMombackLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMombackLine::Clone(void* pArg)
{
	CMombackLine* pInstance = new CMombackLine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMombackLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMombackLine::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
}

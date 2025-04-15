#include "Item.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CItem::CItem(const CItem& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	m_szTextureID = static_cast<DESC*>(pArg)->szTextureID;
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;
	m_fTextureNum = static_cast<DESC*>(pArg)->fTextureNum;
	m_eColID = static_cast<DESC*>(pArg)->eColID;

	m_szBufferType = TEXT("Rect");

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	DESC* Desc = static_cast<DESC*>(pArg);

	m_pTransformCom->Set_SpeedPerSec(300.f);

	m_fStdHeight = Desc->vInitPos.y;

	return S_OK;
}

void CItem::Priority_Update(_float fTimeDelta)
{
}

EVENT CItem::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	//ÇÔ¼ö µþ±ï
	//HarmonicMoveY(_float fWaveHegiht, _float fStdheight, _float fTimeDelta);
	m_pTransformCom->HarmonicMoveY(2.f, m_fStdHeight, fTimeDelta);

	return EVN_NONE;
}

void CItem::Late_Update(_float fTimeDelta)
{
	m_pCollider->Update_Collider();

	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (m_pGameInstance->IsInFrustum(vPos, m_pTransformCom->Get_Radius()))
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CItem::Render()
{
	//if (FAILED(m_pTransformCom->Bind_Resource()))
	//	return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard());

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

void CItem::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	m_bDead = true;
}

HRESULT CItem::Ready_Components(void* pArg)
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

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->Scaling(pDesc->vScale);
	}

	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Sphere::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = pDesc->vScale;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_ITEM;
	ColliderDesc.iColliderID = m_eColID;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}
CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem* pInstance = new CItem(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem::Clone(void* pArg)
{
	CItem* pInstance = new CItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CItem::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
}
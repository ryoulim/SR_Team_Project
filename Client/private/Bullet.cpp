#include "Bullet.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBullet::CBullet(const CBullet& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBullet::Priority_Update(_float fTimeDelta)
{
}

EVENT CBullet::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CBullet::Late_Update(_float fTimeDelta)
{
	m_pCollider->Update_Collider(m_pTransformCom);
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CBullet::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CBullet::On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag)
{
	m_bDead = TRUE;
	pCollisionedObject->On_Collision(this,TEXT("Layer_Bullet"));
}

HRESULT CBullet::Ready_Components(void* pArg)
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
		_float4x4 mat{};
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &mat);
		m_pTransformCom->Set_WorldMatrix(&mat.MakeInverseMat(mat));
		m_pTransformCom->Scaling(pDesc->vScale);
	}

	return S_OK;
}
void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
}
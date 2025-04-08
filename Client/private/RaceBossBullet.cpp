#include "RaceBossBullet.h"

CRaceBossBullet::CRaceBossBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet { pGraphic_Device }
{
}

CRaceBossBullet::CRaceBossBullet(const CRaceBossBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CRaceBossBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceBossBullet::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RaceBossBullet");
	m_szBufferType = TEXT("Rect");

	Ready_Components(pArg);

	return S_OK;
}

void CRaceBossBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceBossBullet::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	m_pTransformCom->Go_Straight(fTimeDelta);
	
	return __super::Update(fTimeDelta);
}

void CRaceBossBullet::Late_Update(_float fTimeDelta)
{
	//m_pTransformCom->Scaling(_float3(m_fScale, m_fScale, m_fScale));

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceBossBullet::Render()
{
	/*if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;*/

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard_Y());

	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
		
	return S_OK;
}
 
void CRaceBossBullet::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	m_bDead = true;
}

HRESULT CRaceBossBullet::Ready_Components(void* pArg)
{
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

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_pTransformCom->LookAt(pDesc->vPosition + pDesc->vLook);
	}

	DESC* pDesc = static_cast<DESC*>(pArg);

	CCollider_Sphere::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = pDesc->vScale;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderID = pDesc->iColliderID;
	ColliderDesc.iColliderGroupID = CG_MBULLET;
	
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CRaceBossBullet* CRaceBossBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceBossBullet* pInstance = new CRaceBossBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceBossBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceBossBullet::Clone(void* pArg)
{
	CRaceBossBullet* pInstance = new CRaceBossBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceBossBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceBossBullet::Free()
{
	__super::Free();
}

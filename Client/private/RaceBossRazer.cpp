#include "RaceBossRazer.h"

CRaceBossRazer::CRaceBossRazer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue { pGraphic_Device }
{
}

CRaceBossRazer::CRaceBossRazer(const CRaceBossRazer& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CRaceBossRazer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceBossRazer::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RaceBossRazer");
	m_szBufferType = TEXT("Cube");
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;
	Ready_Components(pArg);

	return S_OK;
}

void CRaceBossRazer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceBossRazer::Update(_float fTimeDelta)
{
	/*if (m_bDead)
		return EVN_DEAD;*/
	
	//양의 방향으로 돌아야한다.
	m_vQu += _float3(D3DXToRadian(1.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	m_pTransformCom->QurternionRotation(m_vQu);

	//m_pTransformCom->Rotation(*m_pTransformCom->Get_State(CTransform::STATE_RIGHT), D3DXToRadian(45.f));

	return __super::Update(fTimeDelta);
}

void CRaceBossRazer::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceBossRazer::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
		
	return S_OK;
}
 
void CRaceBossRazer::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	
}

HRESULT CRaceBossRazer::Ready_Components(void* pArg)
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
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_pTransformCom->QurternionRotation(pDesc->vAngle);
		m_eShadingLevel = pDesc->eLevelID;
	}

	DESC* pDesc = static_cast<DESC*>(pArg);

	CCollider_OBB_Cube::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = pDesc->vScale;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderID = pDesc->iColliderID;
	ColliderDesc.iColliderGroupID = CG_MBULLET;
	
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CRaceBossRazer* CRaceBossRazer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceBossRazer* pInstance = new CRaceBossRazer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceBossRazer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceBossRazer::Clone(void* pArg)
{
	CRaceBossRazer* pInstance = new CRaceBossRazer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceBossRazer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceBossRazer::Free()
{
	__super::Free();

	Safe_Release(m_pCollider);
}

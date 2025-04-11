#include "RaceBossBomb.h"

CRaceBossBomb::CRaceBossBomb(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet { pGraphic_Device }
{
}

CRaceBossBomb::CRaceBossBomb(const CRaceBossBomb& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CRaceBossBomb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceBossBomb::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RaceBossBomb");
	m_szBufferType = TEXT("Rect");

	Ready_Components(pArg);

	//자신의 속도와 시작지점을 저장해 놓는다.
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_fSpeed = pDesc->fSpeedPerSec;
	m_vStartPos = pDesc->vPosition;

	return S_OK;
}

void CRaceBossBomb::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceBossBomb::Update(_float fTimeDelta)
{
	/*if (m_bDead)
		return EVN_DEAD;*/

	m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);
	m_pTransformCom->Go_Down(fTimeDelta);
	/*m_fTimeAcc += fTimeDelta;

	_uint iTimeAcc = static_cast<_uint>(m_fTimeAcc);
	_float t = m_fTimeAcc - iTimeAcc;
;
	m_pTransformCom->Lerp(m_fStartPoint, m_fEndPoint, m_fTimeAcc);*/
	
	return __super::Update(fTimeDelta);
}

void CRaceBossBomb::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceBossBomb::Render()
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
 
void CRaceBossBomb::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	m_bDead = true;
}

HRESULT CRaceBossBomb::Ready_Components(void* pArg)
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
		m_pTransformCom->LookAt(pDesc->vLook);
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

CRaceBossBomb* CRaceBossBomb::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceBossBomb* pInstance = new CRaceBossBomb(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceBossBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceBossBomb::Clone(void* pArg)
{
	CRaceBossBomb* pInstance = new CRaceBossBomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceBossBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceBossBomb::Free()
{
	__super::Free();
}

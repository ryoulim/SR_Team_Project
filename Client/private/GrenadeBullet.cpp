// 내 클래스 이름 : GrenadeBullet
// 부모 클래스 이름 : Bullet

#include "GrenadeBullet.h"

CGrenadeBullet::CGrenadeBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CGrenadeBullet::CGrenadeBullet(const CGrenadeBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CGrenadeBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrenadeBullet::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("GrenadeBullet");
	m_szBufferType = TEXT("Rect");
	m_fTimeLimit = 10.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//m_pGravityCom->Jump(30.f);

	return S_OK;
}

void CGrenadeBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CGrenadeBullet::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > m_fTimeLimit ||
		m_bDead)
		return EVN_DEAD;

	m_pTransformCom->Go_Straight(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CGrenadeBullet::Late_Update(_float fTimeDelta)
{



	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
	m_pGravityCom->Update(fTimeDelta);
	if (!m_pGravityCom->isJump())
	{
		int a = 1;
		m_pGravityCom->Jump(30.f);
	}
	__super::Late_Update(fTimeDelta);
}

HRESULT CGrenadeBullet::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	__super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	return S_OK;
}

HRESULT CGrenadeBullet::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = pDesc->vScale;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = COL_PBULLET;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Gravity */

	CGravity::DESC GravityDesc{};
	GravityDesc.pTransformCom = m_pTransformCom;
	GravityDesc.fTimeIncreasePerSec = 8.2f;
	GravityDesc.fMaxFallSpeedPerSec = 840.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Gravity"),
		TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
		return E_FAIL;

	return S_OK;
}

CGrenadeBullet* CGrenadeBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGrenadeBullet* pInstance = new CGrenadeBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGrenadeBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrenadeBullet::Clone(void* pArg)
{
	CGrenadeBullet* pInstance = new CGrenadeBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGrenadeBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrenadeBullet::Free()
{
	__super::Free();
	Safe_Release(m_pGravityCom);
}
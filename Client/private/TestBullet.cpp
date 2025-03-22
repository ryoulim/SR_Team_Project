// 내 클래스 이름 : TestBullet
// 부모 클래스 이름 : Bullet

#include "TestBullet.h"

CTestBullet::CTestBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CTestBullet::CTestBullet(const CTestBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CTestBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTestBullet::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("TestBullet");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTestBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTestBullet::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= m_fTimeLimit)
		m_bDead = TRUE;

	if (m_bDead)
		return EVN_DEAD;
	m_pTransformCom->Go_Straight(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CTestBullet::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTestBullet::Render()
{
	return __super::Render();
}

HRESULT CTestBullet::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Line::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale.x = pDesc->fSpeedPerSec / 60.f; // 총 길이 / 프레임 수
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = COL_PBULLET;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Line"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CTestBullet* CTestBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTestBullet* pInstance = new CTestBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTestBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTestBullet::Clone(void* pArg)
{
	CTestBullet* pInstance = new CTestBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTestBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestBullet::Free()
{
	__super::Free();
}
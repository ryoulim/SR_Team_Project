// 내 클래스 이름 : InvisibleBlock
// 부모 클래스 이름 : Block

#include "InvisibleBlock.h"
#include "GameInstance.h"

CInvisibleBlock::CInvisibleBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CInvisibleBlock::CInvisibleBlock(const CInvisibleBlock& Prototype)
	: CMap(Prototype)
{
}

HRESULT CInvisibleBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInvisibleBlock::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CInvisibleBlock::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CInvisibleBlock::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CInvisibleBlock::Late_Update(_float fTimeDelta)
{
	//__super::Late_Update(fTimeDelta);
}

HRESULT CInvisibleBlock::Render()
{
	return __super::Render();
}

HRESULT CInvisibleBlock::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_BLOCK;
	ColliderDesc.iColliderID = CI_BLOCK_INVISIBLE;

	auto& vAngle = static_cast<DESC*>(pArg)->vAngle;
	_wstring ColliderTag = vAngle.x == 0 && vAngle.y == 0 && vAngle.z == 0 ?
		TEXT("Prototype_Component_Collider_AABB_Cube") :
		TEXT("Prototype_Component_Collider_OBB_Cube");

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ColliderTag,
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CInvisibleBlock* CInvisibleBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInvisibleBlock* pInstance = new CInvisibleBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInvisibleBlock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInvisibleBlock::Clone(void* pArg)
{
	CInvisibleBlock* pInstance = new CInvisibleBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CInvisibleBlock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInvisibleBlock::Free()
{
	__super::Free();
}
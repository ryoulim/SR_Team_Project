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

    DESC* pDesc = static_cast<DESC*>(pArg);

    if (nullptr != pArg)
    {
        /* For.Com_Collider */
        if (pDesc->bCollision)
        {
            CCollider::DESC ColliderDesc{};
            ColliderDesc.pTransform = m_pTransformCom;
            ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
            ColliderDesc.pOwner = this;
            ColliderDesc.iColliderGroupID = CG_BLOCK;
            ColliderDesc.iColliderID = CI_BLOCK_COMMON;

            auto& vAngle = static_cast<DESC*>(pArg)->vAngle;

            const _float rightAngles[] = {
                0.f,
                0.5f * PI,
                1.0f * PI,
                1.5f * PI
            };
            _bool isRightX = false, isRightY = false, isRightZ = false;
            _bool isZeroX = fabsf(vAngle.x) < FLT_EPSILON;
            _bool isZeroY = fabsf(vAngle.y) < FLT_EPSILON;
            _bool isZeroZ = fabsf(vAngle.z) < FLT_EPSILON;

            // 직각 각도인지 체크
            for (_uint i = 0; i < 4; ++i)
            {
                if (fabsf(vAngle.x - rightAngles[i]) < FLT_EPSILON) isRightX = true;
                if (fabsf(vAngle.y - rightAngles[i]) < FLT_EPSILON) isRightY = true;
                if (fabsf(vAngle.z - rightAngles[i]) < FLT_EPSILON) isRightZ = true;
            }

            _bool isAllZero = isZeroX && isZeroY && isZeroZ;
            _bool isAllRightAngle = isRightX && isRightY && isRightZ;

            if (isAllRightAngle)
            {
                if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
                    TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
                    return E_FAIL;

                if (!isAllZero)
                {
                    static_cast<CCollider_AABB_Cube*>(m_pColliderCom)->Update_Rotation(vAngle);
                }
            }
            else
            {
                if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
                    TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
                    return E_FAIL;
            }
        }
    }
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
// 내 클래스 이름 : Picture
// 부모 클래스 이름 : Interactive_Block

#include "Picture.h"

CPicture::CPicture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CPicture::CPicture(const CPicture& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CPicture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPicture::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPicture::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CPicture::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CPicture::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPicture::Render()
{
	return __super::Render();
}

void CPicture::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (CI_WEAPON(OtherColliderID))
	{
        if (!m_bBroken)
        {
            m_fTextureIdx++;
            m_bBroken = TRUE;
        }
	}
}

HRESULT CPicture::Ready_Components(void* pArg)
{
    CMap::Ready_Components(pArg);

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
            ColliderDesc.iColliderGroupID = CG_INTERACTIVE;
            ColliderDesc.iColliderID = CI_INTERACTIVE_PICTURE;

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

CPicture* CPicture::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPicture* pInstance = new CPicture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPicture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPicture::Clone(void* pArg)
{
	CPicture* pInstance = new CPicture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPicture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicture::Free()
{
	__super::Free();
}
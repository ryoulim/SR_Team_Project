// 내 클래스 이름 : PhotoFrame
// 부모 클래스 이름 : Map

#include "PhotoFrame.h"
#include "GameInstance.h"

CPhotoFrame::CPhotoFrame(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CPhotoFrame::CPhotoFrame(const CPhotoFrame& Prototype)
	: CMap(Prototype)
{
}

HRESULT CPhotoFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhotoFrame::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPhotoFrame::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CPhotoFrame::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CPhotoFrame::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPhotoFrame::Render()
{
	return __super::Render();
}

HRESULT CPhotoFrame::Ready_Components(void* pArg)
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
            ColliderDesc.iColliderID = CI_PHOTOFRAME;

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

void CPhotoFrame::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
    switch (OtherColliderID)
    {
    case CI_LOVERBOY:
        break;
    case CI_CHAINGUN:
        break;
    case CI_DISPENSOR_SHELL:
        break;
    default:
        return;
    }

	if (!m_bBroken)
	{
		++m_fTextureIdx;
		m_bBroken = true;
	}
}

CPhotoFrame* CPhotoFrame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPhotoFrame* pInstance = new CPhotoFrame(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPhotoFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPhotoFrame::Clone(void* pArg)
{
	CPhotoFrame* pInstance = new CPhotoFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPhotoFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPhotoFrame::Free()
{
	__super::Free();
}
// 내 클래스 이름 : AlphaRect
// 부모 클래스 이름 : Map

#include "AlphaRect.h"
#include "GameInstance.h"

CAlphaRect::CAlphaRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CAlphaRect::CAlphaRect(const CAlphaRect& Prototype)
	: CMap(Prototype)
{
}

HRESULT CAlphaRect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphaRect::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAlphaRect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CAlphaRect::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CAlphaRect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAlphaRect::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CAlphaRect::Ready_Components(void* pArg)
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
            ColliderDesc.iColliderGroupID = CG_ALPHARECT;
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

CAlphaRect* CAlphaRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAlphaRect* pInstance = new CAlphaRect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAlphaRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAlphaRect::Clone(void* pArg)
{
	CAlphaRect* pInstance = new CAlphaRect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CAlphaRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAlphaRect::Free()
{
	__super::Free();
}
// 내 클래스 이름 : FirePlug
// 부모 클래스 이름 : Interactive_Block

#include "FirePlug.h"
#include "GameInstance.h"

#include "HydroPump.h"

CFirePlug::CFirePlug(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CFirePlug::CFirePlug(const CFirePlug& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CFirePlug::Initialize_Prototype()
{
	/* 체력설정해줘라잇 */
    m_iMaxHp = 30;
    m_iHp = m_iMaxHp;
	return S_OK;
}

HRESULT CFirePlug::Initialize(void* pArg)
{
	m_szTextureID = TEXT("FirePlug");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fTextureIdx = 0.f;

	return S_OK;
}

void CFirePlug::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFirePlug::Update(_float fTimeDelta)
{
    if (10 >= m_iHp)
    {
        if (!m_bBroken)
        {
            Spawn_HydroPump();
            Im_Broken();
        }

        m_fTextureIdx = 5.f;
    }


	return __super::Update(fTimeDelta);
}

void CFirePlug::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFirePlug::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    BillboardShaderRender();

#ifdef _COLLIDERRENDER
	if (m_pColliderCom)
		m_pColliderCom->Render();
#endif

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CFirePlug::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
    if (CI_WEAPON(OtherColliderID))
    {
        m_iHp -= 10;
        /* 체력이 떨어지고, 일정 체력 이하가되면 텍스쳐 변경 */
        /* 0이하로 떨어지면 하이드로펌프 발사 */
    }
}

HRESULT CFirePlug::Ready_Components(void* pArg)
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
            ColliderDesc.iColliderID = CI_INTERACTIVE_FIREPLUG;

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

void CFirePlug::Spawn_HydroPump()
{
    CHydroPump::DESC tDesc = {};
    tDesc.fAniSpeed = 30.f;
    tDesc.fMaxFrame = 24.f;
    tDesc.fSpeedPerSec = 0.f;
    tDesc.fRotationPerSec = RADIAN(0.f);
    tDesc.vInitPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    tDesc.vScale = m_pTransformCom->Compute_Scaled();
    tDesc.eLevelID = m_eLevelID;
    tDesc.m_fDeadTime = 7.5f;

    tDesc.vScale.x *= 2.f;
    tDesc.vScale.y *= 2.f;
    tDesc.vInitPos.y += (tDesc.vScale.y * 0.25f);

    if (FAILED(m_pGameInstance->Add_GameObject(m_eLevelID, TEXT("Prototype_GameObject_HydroPump"),
        m_eLevelID, TEXT("Layer_Item"), &tDesc)))
        return;
}

void CFirePlug::Im_Broken()
{
    _float3 vScale = m_pTransformCom->Compute_Scaled();

    _float3 vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vPosition.y -= (vScale.y * 0.3f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

    _float fScaley = vScale.y / 2.5f;
    vScale.y = fScaley;
    m_pTransformCom->Scaling(vScale);

    m_bBroken = true;
}

CFirePlug* CFirePlug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFirePlug* pInstance = new CFirePlug(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFirePlug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFirePlug::Clone(void* pArg)
{
	CFirePlug* pInstance = new CFirePlug(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFirePlug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFirePlug::Free()
{
	__super::Free();
}
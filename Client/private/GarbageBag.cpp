// 내 클래스 이름 : GarbageBag
// 부모 클래스 이름 : Interactive_Block

#include "GarbageBag.h"
#include "GameInstance.h"
#include "Item.h"

CGarbageBag::CGarbageBag(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CGarbageBag::CGarbageBag(const CGarbageBag& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CGarbageBag::Initialize_Prototype()
{
	/* 체력설정해줘라잇 */
    m_iMaxHp = 30;
    m_iHp = m_iMaxHp;

	return S_OK;
}

HRESULT CGarbageBag::Initialize(void* pArg)
{
	m_szTextureID = TEXT("GarbageBag");
	m_szBufferType = TEXT("Rect");
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fTextureIdx = 0.f;

	return S_OK;
}

void CGarbageBag::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CGarbageBag::Update(_float fTimeDelta)
{
    if (m_bDead)
    {
        Create_Item();
        return EVN_DEAD;
    }

    if (0 >= m_iHp)
        m_bDead = true;
    else if (10 >= m_iHp)
        m_fTextureIdx = 2.f;
    else if (20 >= m_iHp)
        m_fTextureIdx = 1.f;

	return __super::Update(fTimeDelta);
}

void CGarbageBag::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CGarbageBag::Render()
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

void CGarbageBag::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
    if (CI_WEAPON(OtherColliderID))
    {
        m_iHp -= 10;

        /* 체력이 떨어지고, 일정 체력 이하가되면 텍스쳐 변경 */
        /* 0이하로 떨어지면 아이템 생성 후 사망 */
    }
}

HRESULT CGarbageBag::Ready_Components(void* pArg)
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
            ColliderDesc.iColliderID = CI_INTERACTIVE_GARBAGEBAG;

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

void CGarbageBag::Create_Item()
{
    CItem::DESC ItemDesc{};
    ItemDesc.vInitPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _float fScale = {};
    switch (m_eLevelID)
    {
    case LEVEL_GAMEPLAY:
        fScale = BOSSITEMSCALE;
        break;
    case LEVEL_INDOOR:
        fScale = INDOORITEMSCALE;
        break;
    case LEVEL_OUTDOOR:
        fScale = OUTDOORITEMSCALE;
        break;
    default:
        fScale = OUTDOORITEMSCALE;
        break;
    }
    ItemDesc.vScale = { fScale, fScale, fScale };
    ItemDesc.fRotationPerSec = RADIAN(0.f);
    ItemDesc.fSpeedPerSec = 300.f;
    ItemDesc.eLevelID = m_eLevelID;
    ItemDesc.szBufferType = TEXT("Rect");
    ItemDesc.szTextureID = TEXT("Item_Ammo");
    /* 밑으로 3개 랜드 돌려야겠는데 */
    _wstring strPrototypeName;
    switch (rand() % 4)
    {
    case 0:
        ItemDesc.fTextureNum = 0.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_CHAINGUN;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Ammo_Chaingun");
        break;
    case 1:
        ItemDesc.fTextureNum = 1.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_DISPENSER_SCATTER;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Ammo_Dispenser_Scatter");
        break;
    case 2:
        ItemDesc.fTextureNum = 2.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_DISPENSER_CANNON;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Ammo_Dispenser_Cannon");
        break;
    case 3:
        ItemDesc.fTextureNum = 3.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_LOVERBOY;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Ammo_LoverBoy");
        break;
    default:
        ItemDesc.fTextureNum = 3.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_LOVERBOY;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Ammo_LoverBoy");
        break;
    }

    if (FAILED(m_pGameInstance->Add_GameObject(m_eLevelID, strPrototypeName,
        m_eLevelID, TEXT("Layer_Item"), &ItemDesc)))
        return;
}

CGarbageBag* CGarbageBag::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGarbageBag* pInstance = new CGarbageBag(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGarbageBag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGarbageBag::Clone(void* pArg)
{
	CGarbageBag* pInstance = new CGarbageBag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGarbageBag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGarbageBag::Free()
{
	__super::Free();
}
// 내 클래스 이름 : TrashCan
// 부모 클래스 이름 : Interactive_Block

#include "TrashCan.h"
#include "GameInstance.h"
#include "Item.h"


CTrashCan::CTrashCan(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CTrashCan::CTrashCan(const CTrashCan& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CTrashCan::Initialize_Prototype()
{
    /* 체력 설정 해줘라잇 */
	m_iMaxHp = 30;
	m_iHp = m_iMaxHp;

	return S_OK;
}

HRESULT CTrashCan::Initialize(void* pArg)
{
	m_szTextureID = TEXT("TrashCan");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fTextureIdx = 0.f;

	return S_OK;
}

void CTrashCan::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTrashCan::Update(_float fTimeDelta)
{
    if (m_bDead)
    {
        Create_Item();
        return EVN_DEAD;
    }

    if (0 >= m_iHp)
        m_bDead = true;
    else if (20 > m_iHp)
        m_fTextureIdx = 1.f;

    return __super::Update(fTimeDelta);
}

void CTrashCan::Late_Update(_float fTimeDelta)
{
    _float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    CGameObject::Compute_ViewZ(&vTemp);

    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
        return;
}

HRESULT CTrashCan::Render()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    BillboardShaderRender();

#ifdef _COLLIDERRENDER
    if (m_pColliderCom)
        m_pColliderCom->Render();
#endif

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	return S_OK;
}

void CTrashCan::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (CI_WEAPON(OtherColliderID))
	{
        m_iHp -= 10;
	}
}

HRESULT CTrashCan::Ready_Components(void* pArg)
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
            ColliderDesc.iColliderID = CI_INTERACTIVE_TRASHCAN;

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

void CTrashCan::Create_Item()
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
    case LEVEL_UNDERGROUND:
        fScale = UNDERGROUNDITEMSCALE;
        break;
    default:
        fScale = OUTDOORITEMSCALE;
        break;
    }
    ItemDesc.fRotationPerSec = RADIAN(0.f);
    ItemDesc.fSpeedPerSec = 300.f;
    ItemDesc.eLevelID = m_eLevelID;
    /* 밑으로 3개 랜드 돌려야겠는데 */
    _wstring strPrototypeName;

    switch (rand() % 5)
    {
    case 0:
        ItemDesc.vScale = { fScale * 1.2f, fScale, fScale };
        ItemDesc.szTextureID = TEXT("Item_Healkit");
        ItemDesc.fTextureNum = 0.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_HEALKIT;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Healkit");
        break;
    case 1:
        ItemDesc.vScale = { fScale * 1.2f, fScale, fScale };
        ItemDesc.szTextureID = TEXT("Item_Healkit");
        ItemDesc.fTextureNum = 0.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_HEALKIT;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Healkit");
        break;
    case 2:
        ItemDesc.vScale = { fScale, fScale * 1.5f, fScale };
        ItemDesc.szTextureID = TEXT("Item_Armor");
        ItemDesc.fTextureNum = 1.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_ARMOR_FULL;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Armor_Full");
        break;
    case 3:
        ItemDesc.vScale = { fScale, fScale * 1.5f, fScale };
        ItemDesc.szTextureID = TEXT("Item_Armor");
        ItemDesc.fTextureNum = 1.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_ARMOR_FULL;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Armor_Full");
        break;
    case 4:
        ItemDesc.vScale = { fScale * 0.5f, fScale * 0.75f, fScale };
        ItemDesc.szTextureID = TEXT("Item_Armor");
        ItemDesc.fTextureNum = 0.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_ARMOR_PIECE;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Armor_Piece");
        break;
    default:
        ItemDesc.vScale = { fScale * 0.5f, fScale * 0.75f, fScale };
        ItemDesc.szTextureID = TEXT("Item_Armor");
        ItemDesc.fTextureNum = 0.f;
        ItemDesc.eColID = COLLIDER_ID::CI_ITEM_ARMOR_PIECE;
        strPrototypeName = TEXT("Prototype_GameObject_Item_Armor_Piece");
        break;
    }

    if (FAILED(m_pGameInstance->Add_GameObject(m_eLevelID, strPrototypeName,
        m_eLevelID, TEXT("Layer_Item"), &ItemDesc)))
        return;
}

CTrashCan* CTrashCan::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrashCan* pInstance = new CTrashCan(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrashCan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrashCan::Clone(void* pArg)
{
	CTrashCan* pInstance = new CTrashCan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTrashCan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrashCan::Free()
{
	__super::Free();
}
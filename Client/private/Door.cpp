// 내 클래스 이름 : Door
// 부모 클래스 이름 : Interactive_Block

#include "Door.h"
#include "GameInstance.h"

#include "UI_Manager.h"
#define UIMGR CUI_Manager::Get_Instance()
#define PRINT_DIALOG(Message) UIMGR->Insert_DialogQueue(Message)

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CDoor::CDoor(const CDoor& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CDoor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    if (1.f <= static_cast<DESC*>(pArg)->fRotationPerSec)
        m_bSecurity = true;

    if (m_eLevelID == LEVEL_INDOOR || m_eLevelID == LEVEL_UNDERGROUND)
    {
        m_pInteractPromptUI = m_pGameInstance->Find_Object(m_eLevelID, TEXT("Layer_UI"), 4);

        if (nullptr == m_pInteractPromptUI)
            return E_FAIL;

        Safe_AddRef(m_pInteractPromptUI);
    }


	return S_OK;
}

void CDoor::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CDoor::Update(_float fTimeDelta)
{
    if (m_bSecurity)
        return EVN_NONE;

    if (m_bPicked)
    {
        if (KEY_DOWN(DIK_E))
            m_bOpen = true;
    }

    if (m_bOpen)
    {
        m_fTimeAcc += fTimeDelta;
        Open_The_Door(fTimeDelta);
        if (m_fTimeAcc >= 1.7f)
        {
            m_bOpen = false;
        }
    }
	return __super::Update(fTimeDelta);
}

void CDoor::Late_Update(_float fTimeDelta)
{
    if (m_bPicked && nullptr != m_pInteractPromptUI)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, m_pInteractPromptUI);
    }

	__super::Late_Update(fTimeDelta);
}

HRESULT CDoor::Render()
{
	return __super::Render();
}

void CDoor::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
    switch (OtherColliderID)
    {
    case CI_PICKING_RAY:
        /* Press USE [E] to interact with the world. */
        m_bPicked = !m_bPicked;
        break;
    }
}

HRESULT CDoor::Ready_Components(void* pArg)
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
            ColliderDesc.iColliderGroupID = CG_INTERACTIVE;
            ColliderDesc.iColliderID = CI_INTERACTIVE_DOOR;

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

void CDoor::Open_The_Door(_float fTimeDelta)
{
    m_pTransformCom->Go_Up(fTimeDelta);
    m_pColliderCom->Update_Collider();
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDoor* pInstance = new CDoor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDoor::Clone(void* pArg)
{
	CDoor* pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();
    Safe_Release(m_pInteractPromptUI);
}
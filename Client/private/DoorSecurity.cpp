// 내 클래스 이름 : DoorSecurity
// 부모 클래스 이름 : Interactive_Block

#include "DoorSecurity.h"
#include "GameInstance.h"

#include "Player.h"
#include "Door.h"

#include "UI_Manager.h"
#include "InteractPromptUI.h"

CDoorSecurity::CDoorSecurity(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CDoorSecurity::CDoorSecurity(const CDoorSecurity& Prototype)
	: CInteractive_Block(Prototype),
    m_fMaxFrame (Prototype.m_fMaxFrame)
{
}

HRESULT CDoorSecurity::Initialize_Prototype()
{
    m_fMaxFrame = 4.f;

	return S_OK;
}

HRESULT CDoorSecurity::Initialize(void* pArg)
{
	m_szTextureID = TEXT("DoorSecurity");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    if (FAILED(Link_Door()))
        return E_FAIL;

    m_pInteractPromptUI = m_pGameInstance->Find_Object(m_eLevelID, TEXT("Layer_UI"), 4);
    if (nullptr == m_pInteractPromptUI)
        return E_FAIL;
    Safe_AddRef(m_pInteractPromptUI);

    m_pPlayer = static_cast<CPlayer*>(GET_PLAYER);
    Safe_AddRef(m_pPlayer);

    m_pSoundCom->SetVolume(0.2f);

	return S_OK;
}

void CDoorSecurity::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CDoorSecurity::Update(_float fTimeDelta)
{
    if (m_bPicked)
    {
        if (KEY_DOWN(DIK_E))
        {
            if (m_pPlayer->Get_HaveCardKey())
            {
                m_eState = OPEN;
                m_pPlayer->Start_Move_LeftHand();
                m_pSoundCom->Play("keycard_unlock");
            }           
            else
            {
                m_eState = LOCK;
                m_pSoundCom->Play("keycard_locked");
            }
        }
    }

    switch (m_eState)
    {
    case USUAL:
        Move_Frame(fTimeDelta);
        break;
    case LOCK:
        /* 몇 초 동안 인덱스 변경 */
        m_fTextureIdx = 4.f;

        m_fLockTimeAcc += fTimeDelta;
        if (m_fLockTimeAcc >= 1.5f)
        {
            m_eState = USUAL;
            m_fLockTimeAcc = 0.f;
        }
        break;
    case OPEN:
        m_fOpenTimeAcc += fTimeDelta;
        if (m_fOpenTimeAcc >= 1.5f)
        {
            m_fTextureIdx = 5.f;
            dynamic_cast<CDoor*>(m_pDoor)->Security_Off();
            m_fOpenTimeAcc = 0.f;
        }
        break;
    }

	return __super::Update(fTimeDelta);
}

void CDoorSecurity::Late_Update(_float fTimeDelta)
{
    static_cast<CInteractPromptUI*>(m_pInteractPromptUI)->Render_Start(fTimeDelta, m_bPicked);

    if (m_bPicked)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, m_pInteractPromptUI);
    }

    /* 이거 왜 되는지는 모르겠는데 일단 되긴 함,,*/
    m_bPicked = false;

	__super::Late_Update(fTimeDelta);
}

HRESULT CDoorSecurity::Render()
{
    return __super::Render();
}

void CDoorSecurity::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
    switch (OtherColliderID)
    {
    case CI_PICKING_RAY:
    {
        m_bPicked = true;
    }
        break;
    default:
        break;
    }
}

HRESULT CDoorSecurity::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

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
            ColliderDesc.iColliderID = CI_INTERACTIVE_SECURITY;

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

    /* For.Com_Sound */
    if (FAILED(__super::Add_Component(m_eLevelID, TEXT("Prototype_Component_Sound_DoorSecurity"),
        TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
        return E_FAIL;

    return S_OK;
}

void CDoorSecurity::Move_Frame(_float fTimeDelta)
{
    m_fTextureIdx += m_fMaxFrame * fTimeDelta;

    if (m_fTextureIdx >= m_fMaxFrame)
        m_fTextureIdx = 0.f;
}

HRESULT CDoorSecurity::Link_Door()
{
    list<CGameObject*>* pDoorList = m_pGameInstance->Find_Objects(m_eLevelID, TEXT("Layer_Door"));
    if (nullptr == pDoorList)
        return E_FAIL;

    _float3 vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _float fMinDistance = FLT_MAX;
    for (auto pDoor : *pDoorList)
    {
        _float3 vDoorPos = *static_cast<CTransform*>(pDoor->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
        _float3 vDistance = vDoorPos - vPosition;
        _float fDoorDistance = vDistance.Length();

        if (fDoorDistance < fMinDistance)
        {
            fMinDistance = fDoorDistance;

            if (nullptr == m_pDoor)
            {
                m_pDoor = pDoor;
                Safe_AddRef(m_pDoor);
            }
            else
            {
                Safe_Release(m_pDoor);
                m_pDoor = pDoor;
                Safe_AddRef(m_pDoor);
            }
        }
    }

    if (nullptr == m_pDoor)
        return E_FAIL;

    return S_OK;
}

CDoorSecurity* CDoorSecurity::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDoorSecurity* pInstance = new CDoorSecurity(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDoorSecurity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDoorSecurity::Clone(void* pArg)
{
	CDoorSecurity* pInstance = new CDoorSecurity(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDoorSecurity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoorSecurity::Free()
{
	__super::Free();

    Safe_Release(m_pDoor);
    Safe_Release(m_pPlayer);
    Safe_Release(m_pInteractPromptUI);
    Safe_Release(m_pSoundCom);
}
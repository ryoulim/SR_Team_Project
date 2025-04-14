// 내 클래스 이름 : Generator
// 부모 클래스 이름 : Interactive_Block

#include "Generator.h"
#include "GameInstance.h"

#include "FXMgr.h"
#include "CameraManager.h"
#include "Monster.h"

CGenerator::CGenerator(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CGenerator::CGenerator(const CGenerator& Prototype)
	: CInteractive_Block(Prototype),
    m_fMaxFrame {Prototype.m_fMaxFrame}
{
}

HRESULT CGenerator::Initialize_Prototype()
{
    /* 체력 설정 해줘라잇 */
    m_fMaxFrame = 15.f;
    m_iMaxHp = 30;
    m_iHp = m_iMaxHp;

	return S_OK;
}

HRESULT CGenerator::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Generator");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    m_fTextureIdx = 0.f;
    m_pSoundCom->SetVolume(0.3f);
    m_pSoundCom->Set3DState("engine_loop", 0.f, 2000.f);

    m_pSoundCom->Play("engine_loop");

	return S_OK;
}

void CGenerator::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CGenerator::Update(_float fTimeDelta)
{
    m_pSoundCom->Update3DPosition("engine_loop", *m_pTransformCom->Get_State(CTransform::STATE_POSITION));

    if (!m_bBroken)
    {
        Move_Frame(fTimeDelta);
        if (10 >= m_iHp 
            || KEY_DOWN(DIK_7))
        {
            m_bBroken = true;
            m_fTextureIdx = 16.f;
            m_pSoundCom->Stop("engine_loop");
            m_pSoundCom->Play("engine_off");
            Start_CutScene();
        }
    }
    else
    {
        Im_Broken(fTimeDelta);

        m_fBrokenTimeAcc += fTimeDelta;
        if (1.5f <= m_fBrokenTimeAcc)
            m_pSoundCom->Stop("engine_off");
    }

    if (m_bCutSceneEnd1)
    {
     /*   m_fCameraTimer += fTimeDelta;
        if (m_fCameraTimer > 1.f)
        {*/
            CAMERA_MANAGER->Switch(CCameraManager::FPS);
            m_bCutSceneEnd1 = FALSE;
        //}
    }

	return __super::Update(fTimeDelta);
}

void CGenerator::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CGenerator::Render()
{
	return __super::Render();
}

void CGenerator::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	/* 충돌하면 체력 깎이고, 체력이 일정 이하로 떨어지면 16고정 후 안개쉐이더 쫙 깔기 */
    if (CI_WEAPON(OtherColliderID))
    {
        m_iHp -= 5;

        _float3 vLook = CCollider::Get_Last_Collision_Depth().Normalize();
        FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 0);
    }
}

HRESULT CGenerator::Ready_Components(void* pArg)
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
            ColliderDesc.iColliderID = CI_INTERACTIVE_GENERATOR;

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

        if (FAILED(__super::Add_Component(m_eLevelID, TEXT("Prototype_Component_Sound_Generator"),
            TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
            return E_FAIL;
    }
    return S_OK;
}

void CGenerator::Move_Frame(_float fTimeDelta)
{
    if (m_bUpFrame)
    {
        m_fTextureIdx += m_fMaxFrame * fTimeDelta;

        if (m_fMaxFrame <= m_fTextureIdx)
            m_bUpFrame = false;
    }
    else
    {
        m_fTextureIdx -= m_fMaxFrame * fTimeDelta;

        if (0 >= m_fTextureIdx)
            m_bUpFrame = true;
    }
}

HRESULT CGenerator::Im_Broken(_float fTimeDelta)
{
    if(!m_bDoOnce)
    {
        //SPAWN_CULTIST(1711.f, 740.f, 479.f, LEVEL_OUTDOOR);
        //SPAWN_CULTIST(1709.f, 640.f, 235.f, LEVEL_OUTDOOR);
        //SPAWN_CULTIST(1313.f, 670.f, -7.f, LEVEL_OUTDOOR);
        //SPAWN_NUKEMUTANT(1709.f, 640.f, 235.f, LEVEL_OUTDOOR);
        //SPAWN_NUKEMUTANT(1709.f, 640.f, 235.f, LEVEL_OUTDOOR);
        //SPAWN_NUKEMUTANT(1709.f, 640.f, 235.f, LEVEL_OUTDOOR);

        //SPAWN_SHOTGUNNER(1638.f, 770.f, -244.f, LEVEL_OUTDOOR);
        //SPAWN_SHOTGUNNER(1472.f, 540.f, 309.f, LEVEL_OUTDOOR);
        //SPAWN_SHOTGUNNER(1045.f, 440.f, 344.f, LEVEL_OUTDOOR);

        //SPAWN_GREATER(1281.f, 730.f, -173.f, LEVEL_OUTDOOR);
        //SPAWN_GREATER(1517.f, 530.f, 692.f, LEVEL_OUTDOOR);
        //SPAWN_GREATER(1201.f, 430.f, 986.f, LEVEL_OUTDOOR);
        //SPAWN_NUKEMUTANT(1201.f, 440.f, 986.f, LEVEL_OUTDOOR);
        //SPAWN_NUKEMUTANT(1201.f, 440.f, 986.f, LEVEL_OUTDOOR);
        //SPAWN_NUKEMUTANT(1201.f, 440.f, 986.f, LEVEL_OUTDOOR);

        //SPAWN_CULTIST(804.f, 430.f, 1217.f, LEVEL_OUTDOOR);
        //SPAWN_SHOTGUNNER(743.f, 380.f, 1617.f, LEVEL_OUTDOOR);  
        //SPAWN_GREATER(619.f, 200.f, 137.f, LEVEL_OUTDOOR);
        //SPAWN_CULTIST(590.f, 380.f, 1677.f, LEVEL_OUTDOOR);
        //SPAWN_SHOTGUNNER(426.f, 330.f, 1340.f, LEVEL_OUTDOOR);
        //SPAWN_GREATER(190.f, 180.f, 352.f, LEVEL_OUTDOOR);
        //SPAWN_CULTIST(599.f, 230.f, 757.f, LEVEL_OUTDOOR);

        //SPAWN_NUKEMUTANT(804.f, 440.f, 1217.f, LEVEL_OUTDOOR);
        //SPAWN_NUKEMUTANT(804.f, 440.f, 1217.f, LEVEL_OUTDOOR);
        //SPAWN_NUKEMUTANT(804.f, 440.f, 1217.f, LEVEL_OUTDOOR);

        m_bDoOnce = true;
    }

    // 빠르게 0으로 감소
    _float fSpeed = 500.f;
    g_FogCustom -= fSpeed * fTimeDelta;

    // 0 이하로 내려가지 않도록
    if (g_FogCustom < 0.f)
        g_FogCustom = 0.f;


    return S_OK;
}

void CGenerator::Start_CutScene()
{
    auto Cameramanager = CAMERA_MANAGER;
    Cameramanager->Switch(CCameraManager::CUTSCENE);

    vector<_float3>* pMoveVector = new vector<_float3>;
    vector<_float3>* pLookVector = new vector<_float3>;

    pMoveVector->push_back({ 1391.90f, 896.50f, -312.51f });
    pLookVector->push_back(_float3{ -0.07f, -0.36f, 0.93f }.Normalize());

    pMoveVector->push_back({ 1615.84f, 894.59f, -267.09f });
    pLookVector->push_back(_float3{ -0.33f, -0.38f, 0.86f }.Normalize());

    pMoveVector->push_back({ 1615.84f, 894.59f, -267.09f });
    pLookVector->push_back(_float3{ -0.33f, -0.38f, 0.86f }.Normalize());

    Cameramanager->Start_CutScene(pMoveVector, pLookVector, 0.2f, &m_bCutSceneEnd1);
}

CGenerator* CGenerator::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGenerator* pInstance = new CGenerator(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGenerator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGenerator::Clone(void* pArg)
{
	CGenerator* pInstance = new CGenerator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGenerator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGenerator::Free()
{
	__super::Free();
    Safe_Release(m_pSoundCom);
}
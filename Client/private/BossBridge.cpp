// 내 클래스 이름 : BossBridge
// 부모 클래스 이름 : Map

#include "BossBridge.h"
#include "GameInstance.h"
#include "CameraManager.h"
#include "FXMgr.h"

CBossBridge::CBossBridge(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CBossBridge::CBossBridge(const CBossBridge& Prototype)
	: CMap(Prototype)
{
}

HRESULT CBossBridge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossBridge::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFallTime = 15.f;
	m_pCameraManager = CAMERA_MANAGER;

	return S_OK;
}

void CBossBridge::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBossBridge::Update(_float fTimeDelta)
{
	if (m_bTrigger)
	{
		m_fTimeaAcc += fTimeDelta;
		if (m_fFallTime <= m_fTimeaAcc)
			m_bTrigger = FALSE; // 어짜피 트리거는 켜지면서 죽으니까 다시 트루될 일 없음
		
		m_pTransformCom->Go_Down(fTimeDelta);
		m_pColliderCom->Update_Collider();

		if(!m_bDoOnce)
		{
			//소용돌이(중앙)
			FX_MGR->SpawnSpher(_float3{ 1430.f, 0.f, 1480.f }, LEVEL_GAMEPLAY);
			m_bDoOnce = true;
			BossMap_CutScene1();
		}
	}

	if (m_bCutSceneEnd[0])
	{
		BossMap_CutScene2();
		m_bCutSceneEnd[0] = FALSE;
	}
	else if (m_bCutSceneEnd[1])
	{
		m_pCameraManager->Switch(CCameraManager::FPS);
		return EVN_DEAD;
		m_bCutSceneEnd[1] = FALSE;
	}
	//else if (m_bCutSceneEnd[2])
	//{
	//	m_bCutSceneEnd[2] = FALSE;
	//	m_pCameraManager->Switch(CCameraManager::FPS);
	//	return EVN_DEAD;
	//}

	return __super::Update(fTimeDelta);
}

void CBossBridge::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBossBridge::Render()
{
	return __super::Render();
}

HRESULT CBossBridge::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_BLOCK;
	ColliderDesc.iColliderID = CI_BLOCK_COMMON;

	auto& vAngle = static_cast<DESC*>(pArg)->vAngle;
	_wstring ColliderTag = vAngle.x == 0 && vAngle.y == 0 && vAngle.z == 0 ?
		TEXT("Prototype_Component_Collider_AABB_Cube") :
		TEXT("Prototype_Component_Collider_OBB_Cube");

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ColliderTag,
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

inline void CBossBridge::On_Trigger() {
	m_bTrigger = TRUE;
}

void CBossBridge::BossMap_CutScene1()
{
	m_pCameraManager->Switch(CCameraManager::CUTSCENE);

	vector<_float3>* pMoveVector = new vector<_float3>;
	vector<_float3>* pLookVector = new vector<_float3>;

	pMoveVector->push_back({ 1932.32f, 387.41f, 822.90f });
	pLookVector->push_back(_float3{ -0.66f, -0.52f, -0.54f }.Normalize());

	pMoveVector->push_back({ 1815.00f, 378.90f, 920.00f });
	pLookVector->push_back(_float3{ -0.52f, -0.51f, -0.68f }.Normalize());

	pMoveVector->push_back({ 1685.00f, 369.50f, 1005.00f });
	pLookVector->push_back(_float3{ -0.34f, -0.50f, -0.79f }.Normalize());

	pMoveVector->push_back({ 1548.00f, 361.50f, 1065.00f });
	pLookVector->push_back(_float3{ -0.13f, -0.50f, -0.86f }.Normalize());

	pMoveVector->push_back({ 1415.00f, 356.00f, 1056.00f });
	pLookVector->push_back(_float3{ -0.0f, -0.51f, -0.86f }.Normalize());

	CAMERA_MANAGER->Start_CutScene(pMoveVector, pLookVector, 0.7f, &m_bCutSceneEnd[0]);
}

void CBossBridge::BossMap_CutScene2()
{
	vector<_float3>* pMoveVector = new vector<_float3>;
	vector<_float3>* pLookVector = new vector<_float3>;

	pMoveVector->push_back({ 1383.72f, 421.83f, 127.78f });
	pLookVector->push_back(_float3{ 0.03f, -0.57f, 0.82f }.Normalize());

	pMoveVector->push_back({ 1385.00f, 104.51f, 379.28f });
	pLookVector->push_back(_float3{ -0.00f, -0.25f, 0.97f }.Normalize());

	pMoveVector->push_back({ 1387.41f, 59.42f, 808.39f });
	pLookVector->push_back(_float3{ 0.02f, -0.01f, 1.00f }.Normalize());

	pMoveVector->push_back({ 1388.69f, 61.80f, 1020.258f });
	pLookVector->push_back(_float3{ 0.022f, 0.010f, 1.000f }.Normalize());

	pMoveVector->push_back({ 1390.61f, 65.08f, 1088.06f });
	pLookVector->push_back(_float3{ 0.025f, 0.035f, 1.000f }.Normalize());

	pMoveVector->push_back({ 1390.61f, 65.08f, 1098.06f });
	pLookVector->push_back(_float3{ 0.025f, 0.035f, 1.000f }.Normalize());

	CAMERA_MANAGER->Start_CutScene(pMoveVector, pLookVector, 0.7f, &m_bCutSceneEnd[1]);
}

CBossBridge* CBossBridge::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossBridge* pInstance = new CBossBridge(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBossBridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossBridge::Clone(void* pArg)
{
	CBossBridge* pInstance = new CBossBridge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBossBridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossBridge::Free()
{
	__super::Free();
}
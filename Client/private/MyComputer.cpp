#include "MyComputer.h"
#include "GameInstance.h"

#include "Monster.h"

CMyComputer::CMyComputer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CMyComputer::CMyComputer(const CMyComputer& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CMyComputer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMyComputer::Initialize(void* pArg)
{
	m_szTextureID = TEXT("MyComputer");
	m_szBufferType = TEXT("Computer");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (m_eLevelID == LEVEL_INDOOR)
	{
		m_pInteractPromptUI = m_pGameInstance->Find_Object(m_eLevelID, TEXT("Layer_UI"), 0);

		if (nullptr == m_pInteractPromptUI)
			return E_FAIL;

		Safe_AddRef(m_pInteractPromptUI);
	}

	return S_OK;
}

void CMyComputer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMyComputer::Update(_float fTimeDelta)
{
	if (m_bPicked || m_bSwitch)
	{
		if (KEY_DOWN(DIK_E))
		{
			if (m_CurCamera == CCameraManager::FPS)
			{
				if (!m_bDoOnce)
				{
					/* 두번째 광장 좀비 */
					SPAWN_NUKEMUTANT_EX(2114.f, 50.f, 545.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(1961.f, 50.f, 427.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(2180.f, 50.f, 600.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(2147.f, 50.f, 550.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(1585.f, 50.f, 766.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(1585.f, 50.f, 953.f, LEVEL_INDOOR);

					/* 첫번째 광장 좀비 */
					SPAWN_NUKEMUTANT_EX(717.f, 50.f, 511.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(600.f, 50.f, 591.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(700.f, 50.f, 671.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(500.f, 50.f, 511.f, LEVEL_INDOOR);
					SPAWN_NUKEMUTANT_EX(500.f, 50.f, 591.f, LEVEL_INDOOR);

					/* 기둥 뒤 사내 */
					SPAWN_SHOTGUNNER_EX(2012.f, 50.f, 464.f, LEVEL_INDOOR, true);
					SPAWN_SHOTGUNNER_EX(1874.f, 50.f, 464.f, LEVEL_INDOOR, true);

					/* 출구 쪽 대기 */
					SPAWN_SHOTGUNNER_EX(1765.f, 50.f, 950.f, LEVEL_INDOOR, true);
					SPAWN_SHOTGUNNER_EX(1776.f, 50.f, 900.f, LEVEL_INDOOR, true);
					SPAWN_SHOTGUNNER_EX(1718.f, 50.f, 950.f, LEVEL_INDOOR, true);

					/* 통로 왼쪽 */
					SPAWN_SHOTGUNNER_EX(1294.f, 50.f, 351.f, LEVEL_INDOOR, true);
					SPAWN_SHOTGUNNER_EX(1229.f, 50.f, 351.f, LEVEL_INDOOR, true);
					SPAWN_SHOTGUNNER_EX(1150.f, 50.f, 351.f, LEVEL_INDOOR, true);

					/* 시작점 쪽 대기 */
					SPAWN_SHOTGUNNER_EX(412.f, 50.f, 377.f, LEVEL_INDOOR, true);

					m_bDoOnce = true;
				}
				m_CurCamera = CCameraManager::CUTSCENE;
				CAMERA_MANAGER->Start_CutScene({ 450.05f, 150.56f, 396.35f }, { 0.57f, -0.64f, -0.52f });
				m_bSwitch = true;
			}
			else
			{
				m_CurCamera = CCameraManager::FPS;
				m_bSwitch = false;
			}

			CAMERA_MANAGER->Switch(m_CurCamera);
		}
	}

	return __super::Update(fTimeDelta);
}

void CMyComputer::Late_Update(_float fTimeDelta)
{
	if (m_bPicked && nullptr != m_pInteractPromptUI && !m_bSwitch)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, m_pInteractPromptUI);
	}

	/* 이거 왜 되는지는 모르겠는데 일단 되긴 함,,*/
	m_bPicked = false;

	__super::Late_Update(fTimeDelta);
}

HRESULT CMyComputer::Render()
{
	MultiTextureShaderRender(3);

	return S_OK;
}

HRESULT CMyComputer::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	DESC* pDesc = static_cast<DESC*>(pArg);

	if (nullptr != pArg)
	{
		/* For.Com_Collider */

		CCollider::DESC ColliderDesc{};
		ColliderDesc.pTransform = m_pTransformCom;
		ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
		ColliderDesc.pOwner = this;
		ColliderDesc.iColliderGroupID = CG_BLOCK;
		ColliderDesc.iColliderID = CI_INTERACTIVE_COMPUTER;

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

	return S_OK;
}

void CMyComputer::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (m_eLevelID != LEVEL_INDOOR)
		return;

	switch (OtherColliderID)
	{
	case CI_PICKING_RAY:
		/* Press USE [E] to interact with the world. */
		m_bPicked = true;
		break;
	}
}

CMyComputer* CMyComputer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMyComputer* pInstance = new CMyComputer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMyComputer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMyComputer::Clone(void* pArg)
{
	CMyComputer* pInstance = new CMyComputer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMyComputer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMyComputer::Free()
{
	__super::Free();

	Safe_Release(m_pInteractPromptUI);
}

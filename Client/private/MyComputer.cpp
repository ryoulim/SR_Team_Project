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
		m_pInteractPromptUI = m_pGameInstance->Find_Object(m_eLevelID, TEXT("Layer_UI"), 4);

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
	if (m_bPicked)
	{
		if (KEY_DOWN(DIK_E))
		{
			if (m_CurCamera == CCameraManager::FPS)
			{
				m_CurCamera = CCameraManager::CUTSCENE;
				CAMERA_MANAGER->Start_CutScene({ 450.05f, 150.56f, 396.35f }, { 0.57f, -0.64f, -0.52f });
				m_bSwitch = true;

				if (!m_bDoOnce)
				{
					CMonster::DESC Archangel_iDesc{};
					Archangel_iDesc.fSpeedPerSec = 60.f;
					Archangel_iDesc.fRotationPerSec = RADIAN(180.f);
					Archangel_iDesc.vActive = true;
					Archangel_iDesc.eLevel = LEVEL_INDOOR;
					Archangel_iDesc.fAttackDistance = 600.f;
					Archangel_iDesc.fDetectiveDistance = 800.f;
					_float3 Pos = { 541.f, 200.f, 659.f };
					Archangel_iDesc.vPosition = Pos;
					Archangel_iDesc.vReturnPos = Pos;
					if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Archangel"),
						LEVEL_INDOOR, L"Layer_Monster", &Archangel_iDesc)))
						return EVN_NONE;

					Pos = { 1200.f, 200.f, 570.f };
					Archangel_iDesc.vPosition = Pos;
					Archangel_iDesc.vReturnPos = Pos;
					if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Archangel"),
						LEVEL_INDOOR, L"Layer_Monster", &Archangel_iDesc)))
						return EVN_NONE;

					Pos = { 1700.f, 200.f, 510.f };
					Archangel_iDesc.vPosition = Pos;
					Archangel_iDesc.vReturnPos = Pos;
					if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Archangel"),
						LEVEL_INDOOR, L"Layer_Monster", &Archangel_iDesc)))
						return EVN_NONE;

					m_bDoOnce = true;
				}
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
	switch (OtherColliderID)
	{
	case CI_PICKING_RAY:
	{
		_float fDistance = 50.f;

		/* Press USE [E] to interact with the world. */
		m_bPicked = !m_bPicked;
	}

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

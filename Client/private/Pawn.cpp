#include "Pawn.h"
#include "FXMgr.h"
#include "UI_Manager.h"

#define INVINCIBILITY_FRAMES 1.5f

CPawn::CPawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPawn::CPawn(const CPawn& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CPawn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPawn::Initialize(void* pArg)
{
	m_tInfo.iHP = 100;
	m_tInfo.iArmor = 0;

	Set_Level(m_eLevelID = static_cast<DESC*>(pArg)->eLevelID);

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPawn::Priority_Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_P))
		Change_Level();
}

EVENT CPawn::Update(_float fTimeDelta)
{
	///* [ 포그 컨트롤 ] */
	//if (KEY_DOWN(DIK_I))
	//{
	//	g_FogTrigger = !g_FogTrigger;
	//	if (!g_FogTrigger)
	//	{
	//		m_bFogAnimation = false;
	//		g_FogCustom = 2000.f;
	//	}
	//}
	//if (KEY_DOWN(DIK_O))
	//	m_bFogAnimation = true;
	//if (m_bFogAnimation)
	//{
	//	// 빠르게 0으로 감소
	//	float fSpeed = 1000.f;
	//	g_FogCustom -= fSpeed * fTimeDelta;
	//	// 0 이하로 내려가지 않도록
	//	if (g_FogCustom < 0.f)
	//		g_FogCustom = 0.f;
	//}

	if (m_bOnHit)
	{
		m_fOnHitTimer += fTimeDelta;
		if (m_fOnHitTimer > INVINCIBILITY_FRAMES)
		{
			m_bOnHit = FALSE;
			m_fOnHitTimer = 0.f;
		}
	}

	return EVN_NONE;
}

void CPawn::Late_Update(_float fTimeDelta)
{
}

HRESULT CPawn::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

inline void CPawn::Set_Level(LEVEL ID) {
	m_eLevelID = ID;
	CUI_Manager::Get_Instance()->Init_UI_To_Player(&m_tInfo);

	if(m_pCollider)
		m_pGameInstance->Add_Collider(m_pCollider, CG_PAWN);
	if(m_pPrePosCollider)
		m_pGameInstance->Add_Collider(m_pPrePosCollider, CG_PAWN_PRE);
}

HRESULT CPawn::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_fInitSpeed = pDesc->fSpeedPerSec;
	}


	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = pDesc->vScale;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_PAWN;
	ColliderDesc.iColliderID = CI_PLAYER;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.iColliderGroupID = CG_PAWN_PRE;
	ColliderDesc.iColliderID = CI_PLAYER_PRE;
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_PrePosCollider"), reinterpret_cast<CComponent**>(&m_pPrePosCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


#include "Level_Loading.h"
#include "RaceBoss.h"
void CPawn::Change_Level()
{
	LEVEL eNextLevelID{};

	switch (m_eLevelID)
	{
	case LEVEL_INDOOR:
		eNextLevelID = LEVEL_OUTDOOR;
		break;
	case LEVEL_OUTDOOR:
		eNextLevelID = LEVEL_RACEFIRST;
		break;
	case LEVEL_RACEFIRST:
	{
		auto RaceBoss = m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_RaceBoss"));
	if (!RaceBoss || static_cast<CRaceBoss*>(RaceBoss)->IsDead())
		eNextLevelID = LEVEL_UNDERGROUND;
	else
		eNextLevelID = rand() % 2 == 0 ? LEVEL_RACESECOND : LEVEL_RACETHIRD;
	}
	break;
	case LEVEL_RACESECOND:
	{
		auto RaceBoss = m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_RaceBoss"));
		if (!RaceBoss || static_cast<CRaceBoss*>(RaceBoss)->IsDead())
			eNextLevelID = LEVEL_UNDERGROUND;
		else
			eNextLevelID = rand() % 2 == 0 ? LEVEL_RACEFIRST : LEVEL_RACETHIRD;
	}
	break;
	case LEVEL_RACETHIRD:
	{
		auto RaceBoss = m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_RaceBoss"));
		if (!RaceBoss || static_cast<CRaceBoss*>(RaceBoss)->IsDead())
			eNextLevelID = LEVEL_UNDERGROUND;
		else
			eNextLevelID = rand() % 2 == 0 ? LEVEL_RACEFIRST : LEVEL_RACESECOND;
	}
	break;
	case LEVEL_UNDERGROUND:
		eNextLevelID = LEVEL_GAMEPLAY;
		break;
	case LEVEL_GAMEPLAY:
		eNextLevelID = LEVEL_END;
		break;
	default:
		break;
	}
	/* 페이드아웃하면서 레벨 전환 되긴 하는데 강제체인지에도 일일이 나오는게 신경쓰여서 주석함 */
	/* 강제전환인지 아닌지 변수 추가하는건 귀찮고 쓸모 없는 것 같고..  */
	//CUI_Manager::Get_Instance()->Fade_Out(eNextLevelID); 
	m_pGameInstance->Change_Level(eNextLevelID);
}

#include "UI_Manager.h"
void CPawn::On_Hit(_int iDamage)
{
	if (m_bOnHit)
		return;

	m_bOnHit = TRUE;
	m_tInfo.iArmor -= iDamage;
	FX_MGR->SpawnHitEffect(m_eLevelID);

	CUI_Manager::Get_Instance()->Set_Face(CPortrait::PORTRAIT_ANGER);

	if (m_tInfo.iArmor <= 0)
	{
		// 음수니까 더해줘야겠지
		m_tInfo.iHP += m_tInfo.iArmor;
		m_tInfo.iArmor = 0;
		//플레이어는 안죽는다
	}
}

void CPawn::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pPrePosCollider);
}
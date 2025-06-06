﻿// 부모 클래스 이름 : Weapon

#include "Weapon_LoverBoy.h"
#include "UI_Manager.h"
#include "Monster.h"
#include "GameObject.h"
#include "FXMgr.h"

CWeapon_LoverBoy::CWeapon_LoverBoy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CWeapon{ pGraphic_Device }
{
}

CWeapon_LoverBoy::CWeapon_LoverBoy(const CWeapon_LoverBoy& Prototype)
	: CWeapon(Prototype)
{
}

HRESULT CWeapon_LoverBoy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_LoverBoy::Initialize(void* pArg)
{
	DESC Desc{};
	m_fTextureNum = 0.f;
	m_szTextureID = TEXT("Weapon_LoverBoy");
	m_vMovingPos = { 320.f,-115.f,0.1f };

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	///////
	m_tAmmoInfo.eType = LOVERBOY;
	m_tAmmoInfo.iCurAmmo = 50;
	m_tAmmoInfo.iMaxAmmo = 6;
	m_tAmmoInfo.iReloadedAmmo = 6;
	///////

	m_pSoundCom->SetVolume(0.6f);

	return S_OK;
}

void CWeapon_LoverBoy::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWeapon_LoverBoy::Update(_float fTimeDelta)
{	
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CWeapon_LoverBoy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon_LoverBoy::Render()
{
	__super::Render();

	if (m_eState == ST_S_ATK)
	{
		Left_Hand_Render();
	}

	return S_OK;
}

void CWeapon_LoverBoy::Set_State(STATE State)
{
	if (m_eState == State)
		return;

	m_eState = State;
	m_fMotionTimer = 0.f;

	switch (State)
	{
	case ST_IDLE:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_eState = ST_IDLE;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 0.f;
		break;
	case ST_OPENING:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 219.f,-321.f,0.1f });
		m_fTextureNum = 13.f;
		m_vCenter = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_vCenter.y = -FWINCY * 0.7f;
		m_pTransformCom->Quaternion_Revolution({ 0.f,0.f,1.f }, m_vCenter, RADIAN(90.f));
		break;
	case ST_WALK:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_eState = ST_WALK;
		m_fTextureNum = 0.f;
		break;
	case ST_W_ATK:
		m_pSoundCom->Play("Fire");
		FX_MGR->SpawnGunFire(_float3{ 750.f, 450.f, 0.1f }, LEVEL_STATIC);
		FX_MGR->SpawnBulletTracer(_float3{ 700.f, 400.f, 0.2f }, LEVEL_STATIC);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_fFrameSpeed = 50.f;
		m_eState = ST_W_ATK;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 6.f;
		break;
	case ST_S_ATK:
		if (m_tAmmoInfo.iCurAmmo < 3)
		{
			m_eState = ST_IDLE;
			break;
		}
		m_LeftHand.pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f,-600.f,0.1f });
		m_eState = ST_S_ATK;
		m_vCenter = { 250.f,-245.f,0.1f };
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 6.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_fFrameSpeed = 50.f;
		Search_Target();
		break;
	case ST_RELOAD:
		m_pSoundCom->Play("Reload");
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_fFrameSpeed = 25.f;
		m_eState = ST_RELOAD;
		m_fTextureNum = 6.f;
		m_fStartFrmae = 7.f;
		m_fEndFrame = 12.f;
		m_tAmmoInfo.iReloadedAmmo = m_tAmmoInfo.iMaxAmmo;							// 채영 총알 테스트로 추가함
		break;
	case ST_ENDING:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_eState = ST_ENDING;
		m_fTextureNum = 12.f;
		m_vCenter = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_vCenter.y = -FWINCY*1.4f;
		break;
	default:
		break;
	}

	_float3 vScale{};
	m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &vScale);
	m_pTransformCom->Scaling(vScale);
}

void CWeapon_LoverBoy::Weak_Attack(_float fTimeDelta)
{
	if (Update_Frame(fTimeDelta))
		Set_State(ST_IDLE);
}

void CWeapon_LoverBoy::Strong_Attack(_float fTimeDelta)
{
	if (m_fMotionTimer < 0.7)
	{
		m_LeftHand.pTransformCom->Chase(m_vCenter, fTimeDelta,5.f);
	}
	else if (m_fMotionTimer < 1.2f)
	{
		if (m_fStartFrmae == m_fTextureNum)
		{
			if (m_CurTarget == m_TargetMonsters.end())
			{
				m_fMotionTimer = 1.2f;
				return;
			}
			FX_MGR->SpawnGunFire(_float3{ 750.f, 450.f, 0.1f }, LEVEL_STATIC);
			CAMERA_MANAGER->StartRecoil();
			m_pSoundCom->Play("Fire");
			m_tAmmoInfo.iReloadedAmmo--;
			m_tAmmoInfo.iCurAmmo--;

			_float3 pPos = *m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
			_float3 pLook = m_CurTarget->second->Get_Pos() - pPos;
			_uint iColliderID{};

			auto pPickedObj = m_pGameInstance->Raycast(pPos, pLook.Normalize(), m_fRayLength, { CG_BLOCK,CG_MONSTER,CG_MBULLET,CG_MONSTER_HEAD }, iColliderID);
			if (pPickedObj)
			{
				pPickedObj->Get_Owner()->On_Collision(iColliderID, m_tAmmoInfo.eType);
			}
			m_CurTarget++;
		}
		Update_Frame(fTimeDelta);
	}
	else if (m_fMotionTimer < 1.25f)
	{
		m_fTextureNum = 0.f;
	}
	else if (m_fMotionTimer < 1.6f)
	{
		m_LeftHand.pTransformCom->Chase({ 0.f,-600.f,0.1f }, fTimeDelta);
	}
	else
	{
		for (auto Pair : m_TargetMonsters)
		{
			static_cast<CMonster*>(Pair.second->Get_Owner())->Render_Skull(FALSE);
			//Safe_Release(Pair.second);
			//Pair.second->Get_Owner()->Release();
		}
		m_TargetMonsters.clear();
		m_CurTarget = m_TargetMonsters.end();
		Set_State(ST_RELOAD);
	}
}

void CWeapon_LoverBoy::Reload(_float fTimeDelta)
{
	if (Update_Frame(fTimeDelta))
		Set_State(ST_ENDING);
}

void CWeapon_LoverBoy::Ending(_float fTimeDelta)
{
	m_pTransformCom->Quaternion_Revolution(_float3(0.f, 0.f, 1.f), m_vCenter, RADIAN(350.f) * fTimeDelta);
	m_vCenter.y -= 600.f * fTimeDelta;
	m_pTransformCom->Go_Down(fTimeDelta);

	if (m_fMotionTimer > 0.8f)
	{
		m_pTransformCom->Rotation_Reset();
		m_pSoundCom->Play("Reload2");
		Set_State(ST_OPENING);
	}
}

void CWeapon_LoverBoy::Opening(_float fTimeDelta)
{
	m_pTransformCom->Quaternion_Revolution(_float3(0.f, 0.f, 1.f), m_vCenter, -RADIAN(700.f) * fTimeDelta);


	if (m_fMotionTimer > 0.15f)
	{
		m_pTransformCom->Rotation_Reset();
		Set_State(ST_IDLE);
	}
}

void CWeapon_LoverBoy::Search_Target()
{
	auto MonsterList = m_pGameInstance->Get_Colliders(CG_MONSTER_HEAD);
	const auto& vCameraPosition = *m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
	const auto& vCameraLook = m_pCameraTransform->Get_State(CTransform::STATE_LOOK)->Normalize();

	const _float cosFovHalf = cosf(RADIAN(60.f * 0.8f));
	priority_queue<TargetInfo> topTargets;

	for (auto pCollider : *MonsterList)
	{
		_float3 toTarget = pCollider->Get_Pos() - vCameraPosition;
		_float distance = toTarget.Length();
		_float3 dir = toTarget / distance;

		if (dir.Dot(vCameraLook) < cosFovHalf || pCollider->Get_MaxLength() == 0)
			continue;

		// Raycast 벽 검사
		if (m_pGameInstance->RaycastBetweenPoints(vCameraPosition, pCollider->Get_Pos(), CG_BLOCK))
			continue;

		topTargets.push({ distance, pCollider });

		if (topTargets.size() > 3)
			topTargets.pop(); // 가장 먼 놈 제거
	}

	// 최종 대상 리스트 구성
	m_TargetMonsters.clear();

	while (!topTargets.empty())
	{
		auto& info = topTargets.top();
		m_TargetMonsters.emplace(info.fDistance, info.pCollider);

		static_cast<CMonster*>(info.pCollider->Get_Owner())->Render_Skull(TRUE);
		//Safe_AddRef(info.pCollider);
		CGameObject* pOwner = info.pCollider->Get_Owner();
		//Safe_AddRef(pOwner);


		topTargets.pop();
	}

	if (!m_TargetMonsters.empty())
		m_CurTarget = m_TargetMonsters.begin();
}

void CWeapon_LoverBoy::Left_Hand_Render()
{
	m_LeftHand.pTextureCom->Bind_Resource(0);

	m_LeftHand.pTransformCom->Bind_Resource();

	m_LeftHand.pVIBufferCom->Bind_Buffers();

	m_LeftHand.pVIBufferCom->Render();
}

HRESULT CWeapon_LoverBoy::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_LeftHand")),
		TEXT("Com_Left_Texture"), reinterpret_cast<CComponent**>(&m_LeftHand.pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_Rect")),
		TEXT("Com_Left_VIBuffer"), reinterpret_cast<CComponent**>(&m_LeftHand.pVIBufferCom))))
		return E_FAIL;

	DESC Desc{};
	Desc.fSpeedPerSec = 2000.f;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Left_Transform"), reinterpret_cast<CComponent**>(&m_LeftHand.pTransformCom),&Desc)))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_LoverBoy"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	_float3 VScale{};
	m_LeftHand.pTextureCom->Get_TextureSize(0, &VScale);
	m_LeftHand.pTransformCom->Scaling(VScale*1.5f);

	m_LeftHand.pTransformCom->Set_State(CTransform::STATE_POSITION, {-FWINCX,-FWINCY,0.1f});

	return __super::Ready_Components(pArg);
}

CWeapon_LoverBoy* CWeapon_LoverBoy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon_LoverBoy* pInstance = new CWeapon_LoverBoy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_LoverBoy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_LoverBoy::Clone(void* pArg)
{
	CWeapon_LoverBoy* pInstance = new CWeapon_LoverBoy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeapon_LoverBoy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_LoverBoy::Free()
{
	__super::Free();

	Safe_Release(m_LeftHand.pTextureCom);
	Safe_Release(m_LeftHand.pTransformCom);
	Safe_Release(m_LeftHand.pVIBufferCom);

	//for (auto& Pair : m_TargetMonsters)
	//{
	//	CGameObject* pOwner = Pair.second->Get_Owner();
	//	Safe_Release(pOwner);
	//	Safe_Release(Pair.second);
	//}

	m_TargetMonsters.clear();

}
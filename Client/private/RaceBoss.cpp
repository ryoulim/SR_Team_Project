#include "RaceBoss.h"
#include "PlayerOnBoat.h"
#include "Skull.h"
#include "RBState.h"
#include "FXMgr.h"
#include "CameraManager.h"
#include "PSystem.h"
#include "PSystem.h"

CRaceBoss::CRaceBoss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CRaceBoss::CRaceBoss(const CRaceBoss& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CRaceBoss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceBoss::Initialize(void* pArg)
{
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;

	Ready_Components(pArg);

	m_eState = ENTRANCE;

	ReadyForState();

	m_pPlayer = static_cast<CPlayerOnBoat*>(GET_PLAYER);
	Safe_AddRef(m_pPlayer);
	m_pPlayerpos = static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	Init_Skull();

	return S_OK;
}

void CRaceBoss::Priority_Update(_float fTimeDelta)
{
	/* 플레이어 위치를 항상 알아둬라 */
	m_pPlayerpos = static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceBoss::Update(_float fTimeDelta)
{
	// 몬스터는 업데이트 시점으로 와야함 
	if (m_bDead)
		return EVN_DEAD;

	if (m_eCurState != DEAD &&	m_pPlayerpos->z > 8300)
		Set_State(CRaceBoss::LEAVE);

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//이전 상태와 현재 상태가 다르다면 Enter 실행
	if (m_eCurState != m_ePreState)
	{
		m_pCurState->Enter(fTimeDelta);
		m_ePreState = m_eCurState;

#ifdef _CONSOL
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		//printf("보스 위치 : { %f, %f, %f }\n", vPos.x, vPos.y, vPos.z);
		printf("보스 상태 : %s\n", Debug_State(m_eCurState));
#endif

	}

	//Exectue는 무조건 실행
	m_pCurState->Execute(fTimeDelta);

	/*  --------------------[ 부위 파괴 시 지속 폭발 ]----------------------- */

	if (KEY_DOWN(DIK_0))
	{
		Set_State(DEAD);
		fill(std::begin(m_bPartDead), std::end(m_bPartDead), true);
	}
	/* [ 파괴 이펙트 ] */
	if (m_eCurState == DEAD)
	{
		m_fDieTime += fTimeDelta;
		if (m_fDieTime > 0.4f && m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y > 50)
		{
			FX_MGR->SpawnMultipleExplosionRaceBoss(vPos, m_eLevelID);
			m_fDieTime = 0.f;
		}
	}
	
	// 순서 왼쪽부터임
	if (m_bPartDead[0])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { -165.f, 25.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);
	if (m_bPartDead[1])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { -85.f, 15.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);
	if (m_bPartDead[2])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { 0.f, 25.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);
	if (m_bPartDead[3])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { 85.f, 15.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);
	if (m_bPartDead[4])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { 165.f, 25.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);

	/* -------------------------------------------------------------------- */

	Update_Skull(fTimeDelta); // 이 함수 반드시 보스의 움직임을 모두 업데이트 마친 다음에 호출해야 합니다.

	return EVN_NONE;
}

void CRaceBoss::Late_Update(_float fTimeDelta)
{
	for (auto& Collider : m_ColliderComs)
		Collider->Update_Collider();

	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (vPos.z > 9500.f)
		m_eState = LEAVE;

	m_vPrePos = vPos;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	if (m_bSkullActive)
		m_pSkull->Late_Update(fTimeDelta);
}

HRESULT CRaceBoss::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	//아래
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::BOTTOM)))
		return E_FAIL;

	//아래 줄무늬
	if (FAILED(m_pTextureCom->Bind_Resource(1)))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::BOTTOMLINE)))
		return E_FAIL;

	//정면
	if (FAILED(m_pTextureCom->Bind_Resource(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::FRONTFACE)))
		return E_FAIL;

	//위
	if (FAILED(m_pTextureCom->Bind_Resource(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::TOP)))
		return E_FAIL;

	//위(옆)
	if (FAILED(m_pTextureCom->Bind_Resource(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::TOPSIDE)))
		return E_FAIL;

	//아래(옆)
	if (FAILED(m_pTextureCom->Bind_Resource(5)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::BOTTOMSIDE)))
		return E_FAIL;

	// 총구
	for (_uint i = 0; i < 4; ++i)
	{
		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_iTextureID[i]))))
			return E_FAIL;

		m_pVIBufferCom->Render(CVIBuffer_RaceBoss::MUZZLE1 + i);
	}

	//가운데
	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_iTextureID[4]))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::MIDDLE)))
		return E_FAIL;

	//테두리
	if (FAILED(m_pTextureCom->Bind_Resource(10)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::OUTLINE)))
		return E_FAIL;

	//삼각형 무늬
	if (FAILED(m_pTextureCom->Bind_Resource(11)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::TRIANGLE)))
		return E_FAIL;

#ifdef _COLLIDERRENDER
	for(auto& Collider : m_ColliderComs)
		Collider->Render();
#endif

	return S_OK;
}

void CRaceBoss::Add_Collider()
{
	for (auto Collider : m_ColliderComs)
		m_pGameInstance->Add_Collider(Collider, CG_MONSTER);
}

_bool CRaceBoss::Judge_Skull(const _float3& vColliderPos, _float vColliderRadius,_float fTimedelta)
{
	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vVelocity = GetVelocityPerSecond(fTimedelta);

	const _float3 bulletVelocity = RACE_PBULLET_DIR * RACE_PBULLET_SPEED;

	// 상대 위치/속도
	const _float3 relativePos = vColliderPos - *m_pPlayerpos;
	const _float3 relativeVelocity = vVelocity - bulletVelocity;

	// bullet 반지름 추가
	const _float bulletRadius = 10.f; // 플레이어 총알 반지름 10임
	const _float totalRadius = vColliderRadius + bulletRadius;

	// 거리^2 가 (합쳐진 반지름)^2 이하가 되는 최소 t 계산
	const _float a = relativeVelocity.Dot(relativeVelocity);
	const _float b = 2.0f * relativePos.Dot(relativeVelocity);
	const _float c = relativePos.Dot(relativePos) - SQUARE(totalRadius);

	const _float discriminant = b * b - 4 * a * c;

	if (discriminant < 0.0f)
		return FALSE;

	const _float sqrtDisc = sqrtf(discriminant);
	const _float t1 = (-b - sqrtDisc) / (2 * a);
	const _float t2 = (-b + sqrtDisc) / (2 * a);

	// 미래에 충돌이 발생하는 경우만 TRUE
	return (t2 >= 0.0f);
}

void CRaceBoss::Render_Skull(MUZZLEPOS eMuzzle)
{
	if (m_eSkullMuzzlePos == eMuzzle)
		return;

	m_eSkullMuzzlePos = eMuzzle;
	m_bSkullActive = TRUE;
	m_pSkull->TimeReset();
	m_vSkullPos = Calc_Muzzle_Position(eMuzzle);
}

void CRaceBoss::Set_State(STATE eState)
{
	m_pCurState = m_pState[eState];
	m_eCurState = eState;
}

void CRaceBoss::Go_Straight(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);
}

void CRaceBoss::Go_Backward(_float fTimeDelta)
{
	m_pTransformCom->Go_Backward(fTimeDelta);
}

void CRaceBoss::Go_Up(_float fTimeDelta)
{
	m_pTransformCom->Go_Up(fTimeDelta);
}

void CRaceBoss::Go_Right(_float fTimeDelta)
{
	m_pTransformCom->Go_Right(fTimeDelta);
}

_float CRaceBoss::Compute_PosZ()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z;
}

_float CRaceBoss::Compute_PozY()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y;
}

void CRaceBoss::MoveCatMullRom(_float3& v0, _float3& vStartPos, _float3& vEndPos, _float3& v3, _float fTimeAcc)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, CatmulRomPos(v0, vStartPos, vEndPos, v3, fTimeAcc));
}

void CRaceBoss::Fire_HeadBullet(_float fTimeDelta)
{
	Fire_Bullet(CRaceBossBullet::HEAD, m_ePos, fTimeDelta);
	++m_iHeadBulletCount;
}

void CRaceBoss::Fire_TailBullet(_float fTimeDelta)
{
	Fire_Bullet(CRaceBossBullet::TAIL, m_ePos, fTimeDelta);
}

_uint CRaceBoss::Get_HeadBulletCount()
{
	return m_iHeadBulletCount;
}

void CRaceBoss::Set_HeadBulletCountZero()
{
	m_iHeadBulletCount = 0;
}

void CRaceBoss::ShuffleandPop()
{
	if (m_VecBulletPos.empty())
	{
		m_VecBulletPos = { LSIDE, LMIDDLE, MIDDLE, RMIDDLE, RSIDE };
		random_shuffle(m_VecBulletPos.begin(), m_VecBulletPos.end());
	}

	if (!m_VecBulletPos.empty())
	{
		m_ePos = m_VecBulletPos.back();
		m_VecBulletPos.pop_back();
	}
}

void CRaceBoss::Set_StartState(STATE eState)
{
	if (m_pCurState)
		m_pCurState->Exit();

	m_pCurState = m_pState[eState];
	m_eCurState = eState;
}

#include "FXMgr.h"
void CRaceBoss::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	On_Hit((MUZZLEPOS)MyColliderID, 10);
	
}

_float CRaceBoss::GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	//[0, 1] 범위의 임의 float 을 얻는다.
	float f = (rand() % 10000) * 0.0001f;

	//[lowBound, highBound] 범위의 float 을 리턴한다.
	return (f * (highBound - lowBound)) + lowBound;
}

const char* CRaceBoss::Debug_State(STATE eState)
{
	switch (eState)
	{
	case WAITFORPLAYER:
		return "WAITFORPLAYER";

	case ENTRANCE:
		return "ENTRANCE";

	case IDLE:
		return "IDLE";

	case SHOTREADY:
		return "SHOTREADY";

	case SHOTHEADBULLET:
		return "SHOTHEADBULLET";

	case SHOTTAILBULLET:
		return "SHOTTAILBULLET";

	case READYBOMB:
		return "READYBOMB";

	case BOMBATTACK:
		return "BOMBATTACK";

	case CROSSATTACK:
		return "CROSSATTACK";

	case COMEBACK:
		return "COMEBACK";

	case LEAVE:
		return "LEAVE";

	case NON:
		return "NON";

	default:
		return nullptr;
	}
}

HRESULT CRaceBoss::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RaceBoss"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RaceBoss"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	m_vScale = _float3(90.f, 90.f, 90.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 250.f, 0.f));
	//450,250,1000
	m_pTransformCom->Scaling(m_vScale);

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = {20.f,1.f,1.f};
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MONSTER; //임시로 몬스터
	ColliderDesc.iColliderID = LSIDE;

	m_ColliderComs.resize(5);
	_wstring Key = TEXT("Com_Collider");
	for (_uint i = 0; i < 5; ++i)
	{
		ColliderDesc.vOffSet = Calc_Muzzle_Position((MUZZLEPOS)(LSIDE + i));

		/* For.Com_Collider */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			Key + to_wstring(i), reinterpret_cast<CComponent**>(&m_ColliderComs[i]), &ColliderDesc)))
			return E_FAIL;

		ColliderDesc.iColliderID++;
	}

	return S_OK;
}

void CRaceBoss::ReadyForState()
{
	m_pState[WAITFORPLAYER] = new CRBState_WaitPlayer(this);
	m_pState[ENTRANCE] = new CRBState_Entrance(this);
	m_pState[IDLE] = new CRBState_IDLE(this);
	m_pState[READYBOMB] = new CRBState_ReadyBombing(this);
	m_pState[COMEBACK] = new CRBState_Comeback(this);
	m_pState[SHOTREADY] = new CRBState_ReadyShot(this);
	m_pState[SHOTHEADBULLET] = new CRBState_ShotHeadBullet(this);
	m_pState[SHOTTAILBULLET] = new CRBState_ShotTailBullet(this);
	m_pState[LEAVE] = new CRBState_Leave(this);
	m_pState[DEAD] = new CRBState_Dead(this);
	m_pState[CLOSE_TO_PLAYER] = new CRBState_CloseToPlayer(this);

	m_pState[BOMBATTACK] = new CRBState_BombAttack(this);
	m_pState[CROSSATTACK] = new CRBState_CrossAttack(this);
	m_pState[MOMBACK] = new CRBState_Momback(this);
	m_pState[MOMBACKREADY] = new CRBState_MombackReady(this);
	m_pState[MOMBACKREVERSE] = new CRBState_MombackReverse(this);


	//계속 터지길래 시작부터 값 채워놓음
	m_VecBulletPos = { LSIDE, LMIDDLE, MIDDLE, RMIDDLE, RSIDE };
	random_shuffle(m_VecBulletPos.begin(), m_VecBulletPos.end());

	m_pCurState = m_pState[ENTRANCE];
}

HRESULT CRaceBoss::Fire_Bullet(CRaceBossBullet::RBULLETTYPE eType, MUZZLEPOS ePos, _float fTimeDelta)
{
	CRaceBossBullet::DESC RaceBossBulletdesc{};
	RaceBossBulletdesc.bAnimation = false;
	RaceBossBulletdesc.iColliderID = CI_BOSS_GUIDBULLET;
	RaceBossBulletdesc.fSpeedPerSec = 1000.f;
	RaceBossBulletdesc.fRotationPerSec = RADIAN(180.f);
	RaceBossBulletdesc.vScale = { 20.f, 20.f, 20.f };
	RaceBossBulletdesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION) + Calc_Muzzle_Position(ePos);

	for (_uint i = 0; i < 4; i++)
	{
		if (m_iTextureID[i] == 8)
			m_iTextureID[i] = 6;
	}

	if (m_iTextureID[4] == 8)
		m_iTextureID[4] = 9;
	
	m_iTextureID[m_ePos - 62] = 8;

	if (eType == CRaceBossBullet::HEAD)
	{
		//HEAD 총알은 플레이어를 향함
		//auto pPlayer = GET_PLAYER;
		RaceBossBulletdesc.vLook = CalcBulletLook(RaceBossBulletdesc.vPosition, RaceBossBulletdesc.fSpeedPerSec, fTimeDelta);
		RaceBossBulletdesc.fMaxBulletSize = 20.f;
		m_vBulletDiretion = RaceBossBulletdesc.vLook;
	}

	if (eType == CRaceBossBullet::TAIL)
	{
		//TAIL 총알은 HEAD 총알을 향함
		RaceBossBulletdesc.vLook = m_vBulletDiretion;
		RaceBossBulletdesc.fMaxBulletSize = 10.f;
	}
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBossBullet"),
		m_eLevelID, L"Layer_RaceBossBullet", &RaceBossBulletdesc)))
		return E_FAIL;

	return S_OK;
}

void CRaceBoss::RestoreTextureID()
{
	m_iTextureID[m_ePos - 62] = 6;
	m_iTextureID[4] = 9;
}

HRESULT CRaceBoss::SpawnTargetAim(_float3 _vAimPosition)
{
	/* [ 에임을 생성한다. ] */
	CBombRadius::DESC Bombdesc = {};
	Bombdesc.vInitPos = _vAimPosition;
	Bombdesc.vScale = { 100.f, 100.f, 1.f };
	Bombdesc.eLevelID = m_eLevelID;
	Bombdesc.vAngle = { D3DXToRadian(90.f), 0.f, 0.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_BombRadius"),
		m_eLevelID, L"Layer_RaceBossBombRadius", &Bombdesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRaceBoss::SpawnTargetLine(_float3 _vLinePosition)
{
	/* [ 라인을 생성한다. ] */
	CMombackLine::DESC Linedesc = {};
	Linedesc.vInitPos = _vLinePosition;
	Linedesc.vScale = { 200.f, 4500.f, 1.f };
	Linedesc.eLevelID = m_eLevelID;
	Linedesc.vAngle = { D3DXToRadian(90.f), 0.f, 0.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MombackLine"),
		m_eLevelID, L"Layer_MombackLine", &Linedesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRaceBoss::SpawnTargetLineReverse(_float3 _vLinePosition)
{
	/* [ 라인을 생성한다. ] */
	CMombackLine::DESC Linedesc = {};
	Linedesc.vInitPos = _vLinePosition;
	Linedesc.vScale = { 200.f, 4500.f, 1.f };
	Linedesc.eLevelID = m_eLevelID;
	Linedesc.vAngle = { D3DXToRadian(90.f), D3DXToRadian(180.f), 0.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MombackLine"),
		m_eLevelID, L"Layer_MombackLine", &Linedesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRaceBoss::SpawnMultipleTargetAim(_float _fTimedelta)
{
	/* [ 보스를 기준으로 아래에 무차별 폭격을 가한다 ] */
	m_fBombTime += _fTimedelta;

	// 랜덤 범위(보스기준)
	_float3 CurruntPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float minX = CurruntPos.x - 200.f;
	_float maxX = CurruntPos.x + 200.f;
	_float minZ = CurruntPos.z - 600.f;
	_float maxZ = CurruntPos.z - 200.f;


	/* [ 스마트하게 배치 ] */
	if(m_fBombTime > 0.02f)
	{
		float x = GetRandomFloat(minX, maxX);
		float z = GetRandomFloat(minZ, maxZ);
		float y = 1.f;

		SpawnTargetAim({x, y, z});
		m_fBombTime = 0.f;
	}

	return S_OK;
}

_float3 CRaceBoss::Calc_Muzzle_Position(MUZZLEPOS eMuzzle)
{
	_float3 vAdjustPos = {};

	switch (eMuzzle)
	{
	case LSIDE:
		vAdjustPos = { -m_vScale.x * 1.625f, m_vScale.y * 0.25f, -m_vScale.z * 2.f };
		break;

	case LMIDDLE:
		vAdjustPos = { -m_vScale.x * 0.875f, m_vScale.y * 0.f, -m_vScale.z * 2.f };
		break;

	case MIDDLE:
		vAdjustPos = { 0.f, m_vScale.y * 0.25f, -m_vScale.z * 2.f };
		break;

	case RMIDDLE:
		vAdjustPos = { m_vScale.x * 0.875f, m_vScale.y * 0.f, -m_vScale.z * 2.f };
		break;

	case RSIDE:
		vAdjustPos = { m_vScale.x * 1.625f, m_vScale.y * 0.25f, -m_vScale.z * 2.f };
		break;

	default:
		break;
	}
	return vAdjustPos;
}

_float3 CRaceBoss::CatmulRomPos(_float3& v0, _float3& vStartPos, _float3& vEndPos, _float3& v3, _float fTimeAcc)
{
	_uint iTimeAcc = static_cast<_uint>(fTimeAcc);
	_float t = fTimeAcc - iTimeAcc;

	_float3 vResult = {};
	D3DXVec3CatmullRom(&vResult, &v0, &vStartPos, &vEndPos, &v3, t);

	// 캣멀롬 구하고 Z는 플레이어의 포지션 더해서 넣어줘야함!!!!
	vResult.z += m_pPlayerpos->z;

	return vResult;
}

_float3 CRaceBoss::CalcBulletLook(const _float3& vBulletStartPos, _float fBulletSpeed, _float fTimeDelta)
{
	// 플레이어 속도 (Z축만 사용)
	_float3 vTargetVelocity = m_pPlayer->GetVelocityPerSecond(fTimeDelta);
	vTargetVelocity.x = 0.0f;
	vTargetVelocity.y = 0.0f;

	// Z축 예측을 유도하기 위해 z+100 오프셋 추가
	_float3 vOffsetTargetPos = *m_pPlayerpos + _float3{ 0.f, 0.f, 100.f };
	_float3 toTarget = vOffsetTargetPos - vBulletStartPos;

	const _float fBulletSpeedSq = SQUARE(fBulletSpeed);
	const _float a = vTargetVelocity.Dot(vTargetVelocity) - fBulletSpeedSq;
	const _float b = 2.0f * toTarget.Dot(vTargetVelocity);
	const _float c = toTarget.Dot(toTarget);

	const _float discriminant = b * b - 4 * a * c;

	// 예외 처리: 해가 없거나 선형 방정식 형태에 가까울 경우
	if (discriminant < 0.0f || fabsf(a) < 1e-5f)
		return toTarget.Normalize();

	const _float sqrtDisc = sqrtf(discriminant);
	const _float inv2a = 0.5f / a; // (2a)^-1 계산을 한 번만
	const _float t1 = (-b + sqrtDisc) * inv2a;
	const _float t2 = (-b - sqrtDisc) * inv2a;

	_float t = (t1 > 0.0f && t2 > 0.0f) ? min(t1, t2) : max(t1, t2);
	if (t < 0.0f)
		return toTarget.Normalize();

	const _float3 predictedPos = *m_pPlayerpos + vTargetVelocity * t;
	return (predictedPos - vBulletStartPos).Normalize();
}

void CRaceBoss::Init_Skull()
{
	CSkull::DESC SkullDesc{};
	SkullDesc.eLevelID = LEVEL_STATIC;
	SkullDesc.vScale = { 1.f,1.f,1.f };

	m_pSkull = static_cast<CSkull*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC,
		TEXT("Prototype_GameObject_Skull"), &SkullDesc));
}

void CRaceBoss::Update_Skull(_float fTimeDelta)
{
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (!m_bPartDead[0] &&
		Judge_Skull(vPos + Calc_Muzzle_Position(LSIDE),20.f,fTimeDelta))
	{
		Render_Skull(LSIDE);
	}
	else if (!m_bPartDead[1] &&
		Judge_Skull(vPos + Calc_Muzzle_Position(LMIDDLE), 20.f, fTimeDelta))
	{
		Render_Skull(LMIDDLE);
	}
	else if (!m_bPartDead[2] &&
		Judge_Skull(vPos + Calc_Muzzle_Position(MIDDLE), 20.f, fTimeDelta))
	{
		Render_Skull(MIDDLE);
	}
	else if (!m_bPartDead[3] &&
		Judge_Skull(vPos + Calc_Muzzle_Position(RMIDDLE), 20.f, fTimeDelta))
	{
		Render_Skull(RMIDDLE);
	}
	else if (!m_bPartDead[4] &&
		Judge_Skull(vPos + Calc_Muzzle_Position(RSIDE), 20.f, fTimeDelta))
	{
		Render_Skull(RSIDE);
	}
	else
	{
		m_bSkullActive = FALSE;
		m_eSkullMuzzlePos = POSEND;
	}

	if (m_bSkullActive)
		m_pSkull->Update(*m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_vSkullPos, fTimeDelta);
}

void CRaceBoss::SpawnWaterParticle(_float fWaterSpeed, _float _fMin, _float _fMax)
{
	/* [ 물보라 파티클 01번 ] */
	CPSystem::DESC WaterBoatDesc{};
	WaterBoatDesc.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc.iParticleNums = 150;
	WaterBoatDesc.fMaxFrame = 1.f;
	WaterBoatDesc.fSize = 1.0f;
	WaterBoatDesc.fNum = fWaterSpeed;
	WaterBoatDesc.fLifeTime = 10.f;
	WaterBoatDesc.fMin = _fMin;
	WaterBoatDesc.fMax = _fMax;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut, &WaterBoatDesc)))
		return;

	m_pWaterBoatEffect_01 = *ppOut;

	/* [ 물보라 파티클 02번 ] */
	CPSystem::DESC WaterBoatDesc2{};
	WaterBoatDesc2.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc2.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc2.iParticleNums = 1500;
	WaterBoatDesc2.fMaxFrame = 1.f;
	WaterBoatDesc2.fSize = 0.3f;
	WaterBoatDesc2.fNum = fWaterSpeed;
	WaterBoatDesc2.fLifeTime = 10.f;
	WaterBoatDesc2.fMin = _fMin;
	WaterBoatDesc2.fMax = _fMax;

	CGameObject* pObject2 = nullptr;
	CGameObject** ppOut2 = &pObject2;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut2, &WaterBoatDesc2)))
		return;

	m_pWaterBoatEffect_02 = *ppOut2;

	/* [ 물보라 파티클 03번 ] */
	CPSystem::DESC WaterBoatDesc3{};
	WaterBoatDesc3.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc3.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc3.iParticleNums = 3000;
	WaterBoatDesc3.fMaxFrame = 1.f;
	WaterBoatDesc3.fSize = 0.2f;
	WaterBoatDesc3.fNum = fWaterSpeed;
	WaterBoatDesc3.fLifeTime = 10.f;
	WaterBoatDesc3.fMin = _fMin;
	WaterBoatDesc3.fMax = _fMax;

	CGameObject* pObject3 = nullptr;
	CGameObject** ppOut3 = &pObject2;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut3, &WaterBoatDesc3)))
		return;

	m_pWaterBoatEffect_03 = *ppOut3;

}
void CRaceBoss::SpawnDieParticle(_float fWaterSpeed)
{
	/* [ 물보라 파티클 01번 ] */
	CPSystem::DESC WaterBoatDesc{};
	WaterBoatDesc.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc.iParticleNums = 3500;
	WaterBoatDesc.fMaxFrame = 1.f;
	WaterBoatDesc.fSize = 1.f;
	WaterBoatDesc.fNum = fWaterSpeed;
	WaterBoatDesc.fLifeTime = 10.f;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_RaceBossDie"),
		m_eLevelID, L"Layer_Particle", ppOut, &WaterBoatDesc)))
		return;

	m_pWaterBoatEffect_01 = *ppOut;

	/* [ 물보라 파티클 02번 ] */
	CPSystem::DESC WaterBoatDesc2{};
	WaterBoatDesc2.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc2.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc2.iParticleNums = 3500;
	WaterBoatDesc2.fMaxFrame = 1.f;
	WaterBoatDesc2.fSize = 0.5f;
	WaterBoatDesc2.fNum = fWaterSpeed;
	WaterBoatDesc2.fLifeTime = 10.f;

	CGameObject* pObject2 = nullptr;
	CGameObject** ppOut2 = &pObject2;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_RaceBossDie"),
		m_eLevelID, L"Layer_Particle", ppOut2, &WaterBoatDesc2)))
		return;

	m_pWaterBoatEffect_02 = *ppOut2;

	/* [ 물보라 파티클 03번 ] */
	CPSystem::DESC WaterBoatDesc3{};
	WaterBoatDesc3.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc3.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc3.iParticleNums = 3000;
	WaterBoatDesc3.fMaxFrame = 1.f;
	WaterBoatDesc3.fSize = 0.3f;
	WaterBoatDesc3.fNum = fWaterSpeed;
	WaterBoatDesc3.fLifeTime = 10.f;

	CGameObject* pObject3 = nullptr;
	CGameObject** ppOut3 = &pObject2;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_RaceBossDie"),
		m_eLevelID, L"Layer_Particle", ppOut3, &WaterBoatDesc3)))
		return;

	m_pWaterBoatEffect_03 = *ppOut3;

}

void CRaceBoss::On_Hit(MUZZLEPOS HitPos, _int iDamage)
{
	_int iIndex = HitPos - LSIDE;
	if (m_iMuzzleHp[iIndex] == 0)
		return;

	m_iMuzzleHp[iIndex] -= iDamage;
	m_iHp -= iDamage;
	
	/* [ 포신 정중앙 ] */
	_float3 vCenterPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION) + Calc_Muzzle_Position(HitPos);
	_float3 vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	/* [ 가로 200 / 세로 37.5 / 깊이 400 ] */

	/* [ 포신 어디에 맞았나요? ] */ 
	switch (HitPos)
	{
	case LSIDE:
		// 왼쪽 2칸
		FX_MGR->SpawnRaceExplosion(vCenterPos, { -165.f, 25.f, -205.f }, m_eLevelID, { 30.f, 30.f, 1.f }, TEXT("RaceBossHit"), 13.f);
		break;
	case LMIDDLE:
		// 왼쪽 1칸
		FX_MGR->SpawnRaceExplosion(vCenterPos, { -85.f, 15.f, -205.f }, m_eLevelID, { 30.f, 30.f, 1.f }, TEXT("RaceBossHit"), 13.f);
		break;
	case MIDDLE:
		// 정 중 앙
		FX_MGR->SpawnRaceExplosion(vCenterPos, { 0.f, 25.f, -205.f }, m_eLevelID, { 30.f, 30.f, 1.f }, TEXT("RaceBossHit"), 13.f);
		break;
	case RMIDDLE:
		// 오른쪽 1칸
		FX_MGR->SpawnRaceExplosion(vCenterPos, { 85.f, 15.f, -205.f }, m_eLevelID, { 30.f, 30.f, 1.f }, TEXT("RaceBossHit"), 13.f);
		break;
	case RSIDE:
		// 오른쪽 2칸
		FX_MGR->SpawnRaceExplosion(vCenterPos, { 165.f, 25.f, -205.f }, m_eLevelID, { 30.f, 30.f, 1.f }, TEXT("RaceBossHit"), 13.f);
		break;
	default:
		break;
	}

	//부위파괴
	if (m_iMuzzleHp[iIndex] <= 0)
	{
		m_iMuzzleHp[iIndex] = 0;

		if (iIndex == 9) // 몸통이다
			;
		else
			m_iTextureID[iIndex] = 7; //부서진 이미지 번호

		/* [ 포신 어디가 파괴되었나요? ] */
		switch (HitPos)
		{
		case LSIDE:
			FX_MGR->SpawnRaceExplosion(vCenterPos, { -200.f, 50.f, -220.f }, m_eLevelID, { 130.f, 140.f, 1.f }, TEXT("Effect_Explor"), 32.f);
			CAMERA_MANAGER->Shake_Camera(0.5f, 0.5f);
			m_bPartDead[0] = true;
			break;
		case LMIDDLE:
			FX_MGR->SpawnRaceExplosion(vCenterPos, { -100.f, 50.f, -220.f }, m_eLevelID, { 130.f, 140.f, 1.f }, TEXT("Effect_Explor"), 32.f);
			CAMERA_MANAGER->Shake_Camera(0.5f, 0.5f);
			m_bPartDead[1] = true;
			break;
		case MIDDLE:
			FX_MGR->SpawnRaceExplosion(vCenterPos, { 0.f, 50.f, -220.f }, m_eLevelID, { 130.f, 140.f, 1.f }, TEXT("Effect_Explor"), 32.f);
			CAMERA_MANAGER->Shake_Camera(0.5f, 0.5f);
			m_bPartDead[2] = true;
			break;
		case RMIDDLE:
			FX_MGR->SpawnRaceExplosion(vCenterPos, { 100.f, 50.f, -220.f }, m_eLevelID, { 130.f, 140.f, 1.f }, TEXT("Effect_Explor"), 32.f);
			CAMERA_MANAGER->Shake_Camera(0.5f, 0.5f);
			m_bPartDead[3] = true;
			break;
		case RSIDE:
			FX_MGR->SpawnRaceExplosion(vCenterPos, { 200.f, 50.f, -220.f }, m_eLevelID, { 130.f, 140.f, 1.f }, TEXT("Effect_Explor"), 32.f);
			CAMERA_MANAGER->Shake_Camera(0.5f, 0.5f);
			m_bPartDead[4] = true;
			break;
		}
	}

	//아예죽음
	if (m_iHp <= 0)
	{
		m_iHp = 0;
		Set_State(DEAD);
	}

#ifdef _CONSOL
	printf("보스 힛! 현재 체력 : { %d / %d, %d, %d, %d, %d  }\n", m_iHp, m_iMuzzleHp[0], m_iMuzzleHp[1], m_iMuzzleHp[4], m_iMuzzleHp[2], m_iMuzzleHp[3]);
#endif#endif
}

CRaceBoss* CRaceBoss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceBoss* pInstance = new CRaceBoss(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceBoss::Clone(void* pArg)
{
	CRaceBoss* pInstance = new CRaceBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceBoss::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPlayer);
	Safe_Release(m_pSkull);

	for(auto& Collider : m_ColliderComs)
		Safe_Release(Collider);
	for (size_t i = WAITFORPLAYER; i < NON; ++i)
		Safe_Delete(m_pState[i]);
}

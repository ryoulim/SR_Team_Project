#include "RaceBoss.h"
#include "PlayerOnBoat.h"
#include "Skull.h"
#include "RBState.h"
#include "FXMgr.h"
#include "CameraManager.h"

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
	//이전 상태와 현재 상태가 다르다면 Enter 실행
	if (m_eCurState != m_ePreState)
	{
		m_pCurState->Enter(fTimeDelta);
		m_ePreState = m_eCurState;
	}

	//Exectue는 무조건 실행
	m_pCurState->Execute(fTimeDelta);

	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceBoss::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	/*  --------------------[ 부위 파괴 시 지속 폭발 ]----------------------- */
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (KEY_DOWN(DIK_0))
		fill(std::begin(m_bPartDead), std::end(m_bPartDead), true);
	if (KEY_DOWN(DIK_9))
		FX_MGR->SpawnMultipleExplosionRaceBoss(vPos, m_eLevelID);

	if (m_bPartDead[0])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { -85.f, 15.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);
	if (m_bPartDead[1])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { -165.f, 25.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);
	if (m_bPartDead[2])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { 0.f, 25.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);
	if (m_bPartDead[3])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { 85.f, 15.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);
	if (m_bPartDead[4])
		FX_MGR->SpawnMultipleExplosionRacePoint(fTimeDelta, vPos, { 165.f, 25.f, -205.f }, m_eLevelID, { 50.f, 50.f, 1.f }, TEXT("RaceBossHit"), 13.f);

	/* -------------------------------------------------------------------- */

#ifdef _CONSOL
	if (KEY_DOWN(DIK_X))
	{
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		printf("보스 위치 : { %f, %f, %f }\n", vPos.x, vPos.y, vPos.z);
		printf("보스 상태 : %d\n", m_eState);
	}
#endif

	//매 프레임마다 일정 수치만큼 밀림
	m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);

	Update_Skull(fTimeDelta); // 이 함수 반드시 보스의 움직임을 모두 업데이트 마친 다음에 호출해야 합니다.

	/// 아래는 테스트
	static _float fTimeAcc{};
	fTimeAcc += fTimeDelta;
	if (fTimeAcc > 1.f)
	{
		Fire_HeadBullet(fTimeDelta);
		fTimeAcc = 0.f;
	}

	if (KEY_DOWN(DIK_COMMA))
		m_bDead = TRUE;
	///


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
	m_fTextureNum = 0.f;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
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

#ifdef _COLLIDERRENDER
	for(auto& Collider : m_ColliderComs)
		Collider->Render();
#endif

	return S_OK;
}

_bool CRaceBoss::Judge_Skull(const _float3& vColliderPos, _float vColliderRadius,_float fTimedelta)
{
	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vVelocity = GetVelocityPerSecond(fTimedelta);

	const _float3 bulletVelocity = RACE_PBULLET_DIR * RACE_PBULLET_SPEED;

	// 상대 위치/속도
	const _float3 relativePos = vColliderPos - *m_pPlayerpos;
	const _float3 relativeVelocity = vVelocity - bulletVelocity;

	// 거리^2 가 반지름^2 이하가 되는 최소 t 계산
	const _float a = relativeVelocity.Dot(relativeVelocity);
	const _float b = 2.0f * relativePos.Dot(relativeVelocity);
	const _float c = relativePos.Dot(relativePos) - SQUARE(vColliderRadius);

	const _float discriminant = b * b - 4 * a * c;

	// 교차하지 않음 (충돌할 수 없음)
	if (discriminant < 0.0f)
		return FALSE;

	const _float sqrtDisc = sqrtf(discriminant);
	const _float t1 = (-b - sqrtDisc) / (2 * a);
	const _float t2 = (-b + sqrtDisc) / (2 * a);

	// t1, t2는 충돌 가능한 시간 범위
	// 과거(t < 0)는 무시하고 미래에 교차가 있는지 확인
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

void CRaceBoss::SelectAndDrawRadius()
{
	vector<_float> m_VecPosX = { 350.f, 450.f, 550.f };
	random_shuffle(m_VecPosX.begin(), m_VecPosX.end());
	
	m_fBombPosX[0] = m_VecPosX.back();
	m_VecPosX.pop_back();
	m_fBombPosZ = 0.f;
	Draw_BombRadius(m_fBombPosX[0], m_fBombPosZ);

	m_fBombPosX[1] = m_VecPosX.back();
	m_VecPosX.pop_back();
	m_fBombPosZ = 0.f;
	Draw_BombRadius(m_fBombPosX[1], m_fBombPosZ);

	m_VecPosX.clear();
	m_VecPosX.shrink_to_fit();
}

void CRaceBoss::Bombing()
{
	Fire_Bomb();
}

_bool CRaceBoss::Comeback(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta * 5.f);
	m_pTransformCom->Go_Down(fTimeDelta * 0.1f);

	_float3 fMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 fPlayerPos = *static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	if (fMyPos.z > fPlayerPos.z + 1000.f)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			_float3(450.f, 250.f, fPlayerPos.z + 1300.f)
		);
		return true;
	}
		

	return false;
}

HRESULT CRaceBoss::Draw_BombRadius(_float PosX, _float PosZ)
{
	CBombRadius::DESC Bombdesc = {};

	//Bombdesc.vInitPos = { static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)->x,
	//1.f, static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)->z };

	Bombdesc.vInitPos = { PosX, 1.f, 
		static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)->z };

	Bombdesc.vScale = { 100.f, 1000.f, 0.f };
	Bombdesc.eLevelID = m_eLevelID;
	Bombdesc.vAngle = { D3DXToRadian(90.f), 0.f, 0.f };

	if (FAILED(m_pGameInstance->Add_GameObject(m_eLevelID, TEXT("Prototype_GameObject_BombRadius"),
		m_eLevelID, L"Layer_RaceBossPattern", &Bombdesc)))
		return E_FAIL;

	return S_OK;
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
	//FX_MGR->SpawnExplosion2(CCollider::Get_Last_Collision_Pos(), m_eLevelID); 안나와서 주석처리
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

	m_vScale = _float3(100.f, 100.f, 100.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 250.f, 0.f));
	//450,250,1000
	m_pTransformCom->Scaling(m_vScale);

	CCollider_Capsule::DESC ColliderDesc{};
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
	m_pState[DRAWINGRADIUS] = new CRBState_DrawingRadius(this);
	m_pState[BOMBING] = new CRBState_Bombing(this);
	m_pState[COMEBACK] = new CRBState_Comeback(this);
	m_pState[SHOTREADY] = new CRBState_ReadyShot(this);
	m_pState[SHOTHEADBULLET] = new CRBState_ShotHeadBullet(this);
	m_pState[SHOTTAILBULLET] = new CRBState_ShotTailBullet(this);
	m_pState[LEAVE] = new CRBState_Leave(this);

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

HRESULT CRaceBoss::Fire_Bomb()
{
	CRaceBossBomb::DESC RaceBossBombdesc{};
	RaceBossBombdesc.bAnimation = false;
	RaceBossBombdesc.iColliderID = CI_BOSS_FIRE;
	RaceBossBombdesc.fSpeedPerSec = 1000.f;
	RaceBossBombdesc.fRotationPerSec = RADIAN(180.f);
	RaceBossBombdesc.vScale = { 25.f, 25.f, 25.f };
	RaceBossBombdesc.vLook = _float3(RaceBossBombdesc.vPosition.x, RaceBossBombdesc.vPosition.y, RaceBossBombdesc.vPosition.z + 1.f);
	
	RaceBossBombdesc.vPosition = _float3(
		m_fBombPosX[0] - 25.f,
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y,
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z
	);
		
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBossBomb"),
		m_eLevelID, L"Layer_RaceBossBullet", &RaceBossBombdesc)))
		return E_FAIL;

	RaceBossBombdesc.vPosition = _float3(
		m_fBombPosX[1] - 25.f,
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y,
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z
	);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBossBomb"),
		m_eLevelID, L"Layer_RaceBossBullet", &RaceBossBombdesc)))
		return E_FAIL;

	RaceBossBombdesc.vPosition = _float3(
		m_fBombPosX[0] + 25.f,
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y,
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z
	);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBossBomb"),
		m_eLevelID, L"Layer_RaceBossBullet", &RaceBossBombdesc)))
		return E_FAIL;

	RaceBossBombdesc.vPosition = _float3(
		m_fBombPosX[1] + 25.f,
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y,
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z
	);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBossBomb"),
		m_eLevelID, L"Layer_RaceBossBullet", &RaceBossBombdesc)))
		return E_FAIL;

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
	if (Judge_Skull(vPos + Calc_Muzzle_Position(LSIDE),20.f,fTimeDelta))
	{
		Render_Skull(LSIDE);
	}
	else if (Judge_Skull(vPos + Calc_Muzzle_Position(LMIDDLE), 20.f, fTimeDelta))
	{
		Render_Skull(LMIDDLE);
	}
	else if (Judge_Skull(vPos + Calc_Muzzle_Position(MIDDLE), 20.f, fTimeDelta))
	{
		Render_Skull(MIDDLE);
	}
	else if (Judge_Skull(vPos + Calc_Muzzle_Position(RMIDDLE), 20.f, fTimeDelta))
	{
		Render_Skull(RMIDDLE);
	}
	else if (Judge_Skull(vPos + Calc_Muzzle_Position(RSIDE), 20.f, fTimeDelta))
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
	case LMIDDLE:
		// 왼쪽 1칸
		FX_MGR->SpawnRaceExplosion(vCenterPos, { -85.f, 15.f, -205.f }, m_eLevelID, { 30.f, 30.f, 1.f }, TEXT("RaceBossHit"), 13.f);
		break;
	case LSIDE:
		// 왼쪽 2칸
		FX_MGR->SpawnRaceExplosion(vCenterPos, { -165.f, 25.f, -205.f }, m_eLevelID, { 30.f, 30.f, 1.f }, TEXT("RaceBossHit"), 13.f);
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

		if (iIndex == 4) // 몸통이다
			;
		else
			m_iTextureID[iIndex] = 3;

		/* [ 포신 어디가 파괴되었나요? ] */
		switch (HitPos)
		{
		case LMIDDLE:
			FX_MGR->SpawnRaceExplosion(vCenterPos, { -100.f, 50.f, -220.f }, m_eLevelID, { 130.f, 140.f, 1.f }, TEXT("Effect_Explor"), 32.f);
			CAMERA_MANAGER->Shake_Camera(0.5f, 0.5f);
			m_bPartDead[0] = true;
			break;
		case LSIDE:
			FX_MGR->SpawnRaceExplosion(vCenterPos, { -200.f, 50.f, -220.f }, m_eLevelID, { 130.f, 140.f, 1.f }, TEXT("Effect_Explor"), 32.f);
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
	if (m_iHp < 0)
	{
		m_iHp = 0;
		m_bDead = true;
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

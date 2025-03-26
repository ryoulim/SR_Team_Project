#include "Monster.h"
#include "DebugDraw.h"


CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject(Prototype)
	, m_eLevelID(Prototype.m_eLevelID)
	, m_szTextureID(Prototype.m_szTextureID)
	, m_szBufferType(Prototype.m_szBufferType)
	, m_iHP(Prototype.m_iHP)
	, m_iAttackPower(Prototype.m_iAttackPower)
	, m_iDefense(Prototype.m_iDefense)
	, m_fSpeed(Prototype.m_fSpeed)
	, m_vScale(Prototype.m_vScale)
	, m_eState(Prototype.m_eState)
	, m_strDialogue(Prototype.m_strDialogue)
	, m_strSound(Prototype.m_strSound)
	, m_vDropItems(Prototype.m_vDropItems)
	, m_vDirection(Prototype.m_vDirection)
	, m_vToPlayer(Prototype.m_vToPlayer)
	, m_fCurDistance(Prototype.m_fCurDistance)
	, m_fDetectiveDistance(Prototype.m_fDetectiveDistance)
{
	//크리에이트에서 초기화되는 모든 것을 복사해준다.
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	/* 플레이어를 알고 있어라 */
	m_pTargetPlayer = GET_PLAYER;
	Safe_AddRef(m_pTargetPlayer);

	/* 텍스처, 트랜스폼, 렉트버퍼, 콜라이더 컴포넌트 준비(위치초기화) */
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	//프레임 업데이트
	Animate_Monster(fTimeDelta);
}

EVENT CMonster::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	if (m_bActive)
	{
		m_pGravityCom->Update(fTimeDelta);
		MonsterTick(fTimeDelta);
	}

	return EVN_NONE;
}

void CMonster::Late_Update(_float fTimeDelta)
{
	//플레이어 감지 업데이트
	PlayerDistance();
	CalculateVectorToPlayer();
	IsPlayerDetected();
	
	//콜라이더 업데이트
	m_pCollider->Update_Collider();

	//몬스터 각도업데이트
	Compute_ViewAngle();

	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CMonster::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CMonster::Render()
{
	if (m_bDebug)
		Render_DebugFOV();


	Set_TextureType();
	
	if (m_isReadyMonster) // 몹 텍스쳐 전부 준비 안해서 임시로 분리
	{
		m_pTextureMap[m_iState][m_iDegree]->Get_TextureSize(static_cast<_uint>(m_fAnimationFrame), &m_vScale);
		m_pTransformCom->Scaling(m_vScale * 0.32f);
		if (FAILED(m_pTextureMap[m_iState][m_iDegree]->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
			return E_FAIL;
	}
	else 
	{
		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
			return E_FAIL;
	}
	

	if (!m_bCW || m_iDegree == 0 || m_iDegree == 180.f / m_fDivOffset) {
		if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard()))
			return E_FAIL; 
	}
	else {
		if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard_Inverse()))
			return E_FAIL;
	}
	// 0일 때 좌우반전이 안 되어야 하는데 자꾸 좌우반전 되는 버그가 있다..? 


	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	Release_RenderState();

	return S_OK;
}

HRESULT CMonster::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CMonster::Ready_Components(void* pArg)
{
	/* 텍스처 컴포넌트 */ // 나중에 지울 것
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* 렉트 버퍼 컴포넌트 */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* 트랜스폼 컴포넌트 */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	/* 위치, 스케일 초기화 */
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(m_vScale);
		m_bActive = pDesc->vActive;
		m_vReturnPos = pDesc->vReturnPos;
	}

	/* 콜라이드 컴포넌트 */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};	
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = {};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = COL_MONSTER;
	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	if (m_bActive)
	{
		CGravity::DESC GravityDesc{};
		GravityDesc.pTransformCom = m_pTransformCom;
		GravityDesc.fTimeIncreasePerSec = 8.2f;
		GravityDesc.fMaxFallSpeedPerSec = 840.f;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Gravity"),
			TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void CMonster::Compute_ViewAngle()
{
	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	_float3 vCurLook = *(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * -1.f;
	vCurLook.y = 0.f;
	vCurLook = *D3DXVec3Normalize(&vCurLook, &vCurLook);
	// 보는 방향이 거꾸로길래 수정

	_float3	vBillLook = {};
	_float3 temp = *(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) - vCameraPos;
	temp.y = 0.f;
	vBillLook = *D3DXVec3Normalize(&vBillLook, &temp);
	
	//vCurLook.y = vBillLook.y = 0.f;

	// 벡터의 내적 계산
	float dotProduct = D3DXVec3Dot(&vCurLook, &vBillLook);

	// 안정성을 위해 cosTheta를 클램핑 (-1 <= cosTheta <= 1)
	float cosTheta = max(-1.0f, min(1.0f, dotProduct));

	// 각도를 라디안으로 계산
	float angleInRadians = acos(cosTheta);

	// 각도를 도(degree)로 변환
	m_fPlayersViewAngle = D3DXToDegree(angleInRadians);

	float crossProduct = vCurLook.x * vBillLook.z - vCurLook.z * vBillLook.x;

	if (crossProduct > 0) 
		m_bCW = false;
	else
		m_bCW = true;
}


HRESULT CMonster::Set_TextureType()
{
	if (KEY_DOWN(DIK_1)) // 브레이크포인트 테스트용
		int a = 0;

	_float degree = m_fPlayersViewAngle / m_fDivOffset;
	_float div = 0.f;
	_float mod = modff(degree, &div);

	m_iDegree = (_uint)div + (_uint)(mod > 0.5f ? 1 : 0); // 기준 각도에서 +- 하는 중

	// state는 보스의 상태에 따라 결정
	// 임시로 walk만 지정
	//m_iState = (_uint)(STATE_WALK);
	// grenade같은 것은 state값을 방향 벡터의 각도로 두고 범용적으로 사용해도 좋을듯... 
	//m_iState = 0;

	/******************************************************************/
	//m_fAnimationMaxFrame = (_float)BOSS_STATUS_FRAME::WALK;
	////int maxframe = BOSS_STATUS_FRAME::WALK;
	//int curstatus = BOSS_STATUS::WALK;
	//_float startfrmnum = _float(m_iDegree * m_fAnimationMaxFrame + curstatus);
	//m_fAnimationFrame = startfrmnum + _fTimeDelta;
	//나중에쓸(수도있을)것
	/******************************************************************/



	return S_OK;
}


HRESULT CMonster::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 1.f)
		return S_OK;
	m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
	if (m_fAnimationFrame >= m_fAnimationMaxFrame)
		m_fAnimationFrame = 0.f;

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTargetPlayer);
	Safe_Release(m_pGravityCom);
	for (auto pair : m_pTextureMap)
	{
		for (auto otherpair : pair.second)
		{
			Safe_Release(otherpair.second);
		}

	}	
}

void CMonster::PlayerDistance()
{
	//카메라의 위치, //몬스터의 위치 // 그 사이의 거리
	_float4x4 matCamWorld;

	//카메라의 위치저장
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	//몬스터의 위치
	_float3 vMonsterPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	// 두 점 사이의 거리 계산
	_float distance = sqrtf(
		powf(vCameraPos.x - vMonsterPos.x, 2) +
		powf(vCameraPos.y - vMonsterPos.y, 2) +
		powf(vCameraPos.z - vMonsterPos.z, 2)
	);

	m_fCurDistance = distance;
}

void CMonster::CalculateVectorToPlayer()
{
	//목적 : 몬스터에서 플레이어로 향하는 방향벡터를 구한다.

	// 몬스터 위치
	_float3 vMonsterPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 플레이어 위치
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	// 방향 벡터 = 타겟 위치 - 내 위치
	m_vToPlayer = vPlayerPos - vMonsterPos;

	// 정규화해서 방향만 남긴다 (길이 1)
	D3DXVec3Normalize(&m_vToPlayer, &m_vToPlayer);
	
}

bool CMonster::IsPlayerDetected()
{
	// 거리 체크 (현재거리가 감지거리보다 작을 때)
	if (m_fCurDistance < m_fDetectiveDistance)
	{
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		float fDot = D3DXVec3Dot(&m_vToPlayer, &vLook);  // 3. 각도 비교

		if (fDot > cosf(D3DXToRadian(20)))          // 4. 일정 각도 이내
		{
			return true;
		}
	}
	return false;
}

void CMonster::Render_DebugFOV()
{
	//몬스터 포지션
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//몬스터의 방향(디렉션)
	_float3 vForward = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	vForward.y = 0.f;
	D3DXVec3Normalize(&vForward, &vForward);

	// 시야 각도 절반 (예: 60도 시야 → 30도씩 양쪽)
	float fHalfFOV = D3DXToRadian(20.f / 2.0f);
	float fLength = 100.0f;  // 시야 거리

	// 회전 행렬 생성 (Y축 기준 회전)
	_float4x4 matRotLeft, matRotRight;
	D3DXMatrixRotationY(&matRotLeft, -fHalfFOV);
	D3DXMatrixRotationY(&matRotRight, fHalfFOV);

	// 방향 벡터 회전
	_float3 vLeft, vRight;
	D3DXVec3TransformNormal(&vLeft, &vForward, &matRotLeft);
	D3DXVec3TransformNormal(&vRight, &vForward, &matRotRight);

	// 끝 점 계산
	_float3 vLeftEnd = vPos + vLeft * fLength;
	_float3 vRightEnd = vPos + vRight * fLength;

	Get_CamaraPos;
	_float3 vMonster = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToMonster = vMonster - vCameraPos;
	D3DXVec3Normalize(&vToMonster, &vToMonster);

	_float3 vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };
	float fDot = D3DXVec3Dot(&vToMonster, &vCameraLook);


	if (fDot > 0.5f) // 60도 이내 정면
	{
		// 디버그 선 그리기
		CDebugDraw::DrawLine(m_pGraphic_Device, vPos, vLeftEnd, D3DCOLOR_ARGB(255, 255, 255, 0));   // 노란 선
		CDebugDraw::DrawLine(m_pGraphic_Device, vPos, vRightEnd, D3DCOLOR_ARGB(255, 255, 255, 0));  // 노란 선
		CDebugDraw::DrawLine(m_pGraphic_Device, vLeftEnd, vRightEnd, D3DCOLOR_ARGB(100, 255, 255, 0)); // 상단 연결선
	}
}

const char* CMonster::GetMonsterStateName(MODE eState)
{
	switch (eState)
	{
	case MODE::MODE_IDLE:        return "IDLE";
	case MODE::MODE_DETECTIVE:   return "DETECTIVE";
	case MODE::MODE_BATTLE:      return "BATTLE";
	case MODE::MODE_ATTACK:      return "ATTACK";
	case MODE::MODE_RETURN:      return "RETURN";
	default:                     return "UNKNOWN";
	}
}

void CMonster::MonsterTick(_float fTimeDelta)
{
	//상태변화
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		if (IsPlayerDetected())
		{
			//플레이어 발견 시 행동
			cout << "따끈이 플레이어 발견!!" << endl;
			m_eState = MODE::MODE_BATTLE;
		}
		break;

	case MODE::MODE_BATTLE:
		//배틀 모드시에 행동
		break;
	case MODE::MODE_RETURN:
		//본래위치로 돌아가고 IDLE로 상태가 변한다.
		break;
	}

	// 상태행동(액션)
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		DoIdle(fTimeDelta); 
		break;
	case MODE::MODE_BATTLE:
		DoBattle(fTimeDelta); 
		break;
	case MODE::MODE_RETURN:
		DoReturn(fTimeDelta); 
		break;
	}
}

void CMonster::DoIdle(_float dt)
{
	switch (m_eIdlePhase)
	{
	case EIdlePhase::IDLE_MOVE:
	{
		m_fWanderElapsed += dt;

		m_pTransformCom->Go_Straight(dt);

		if (m_fWanderElapsed >= m_fWanderTime)
		{
			m_fWanderElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_WAIT;
		}
		break;
	}
	case EIdlePhase::IDLE_WAIT:
		m_fIdleWaitElapsed += dt;

		if (m_fIdleWaitElapsed >= m_fIdleWaitTime)
		{
			SetRandomDirection();                  // 회전할 방향 설정
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN; // 다음엔 회전하러 간다
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		bool bRotated = m_pTransformCom->RotateToDirection(vLook, m_vDirection, 5.f, dt);
		if (bRotated)  // 회전 완료 신호
		{
			m_eIdlePhase = EIdlePhase::IDLE_MOVE;
		}
		break;
	}
	}
}

void CMonster::DoBattle(_float dt)
{
	// 1. 플레이어와의 거리 계산
	_float fAttackRange = 250.f;
	_float fChaseRange = 700.f;

	// 2. 거리 기준 분기
	if (m_fCurDistance < fAttackRange)
	{
		// 가까우면 공격
		AttackPattern(dt);
		//cout << "몬스터의 공격 사거리 안입니다." << endl;
	}
	else if (m_fCurDistance < fChaseRange)
	{
		// 중간 거리면 추적
		ChasePlayer(dt);
		//cout << "몬스터 추격중입니다!" << endl;
	}
	else
	{
		// 너무 멀면 전투 종료
		m_eState = MODE_RETURN;
	}
}

void CMonster::DoReturn(_float dt)
{
	// 현재 위치
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 방향 계산
	_float3 vDir = m_vReturnPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	//원래방향으로 턴하기
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);
	if (bRotated)  // 회전 완료 신호
	{
		// 너무 가까우면 이동 종료
		if (fDistance < 1.0f) // 도착 판정 오차 허용
		{
			m_eState = MODE::MODE_IDLE;
			return;
		}

		// 이동 처리 (dt 고려)
		float fSpeed = m_fSpeed; // 예: 3.0f
		_float3 vMove = vDir * fSpeed * dt;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (vMyPos + vMove));
	}
}

void CMonster::SetRandomDirection()
{
	// 0 ~ 359도 사이의 랜덤 각도 생성
	float fAngle = D3DXToRadian(rand() % 360);  // rand() % 360 → 0~359도

	// 각도로부터 방향 벡터 생성
	_float3 vDir = {
		sinf(fAngle),   // x축
		0.f,
		cosf(fAngle)    // z축
	};

	m_vDirection = vDir;

	// 이동 시간 설정
	m_fWanderTime = (rand() % 2000) / 1000.f + 1.f;
	m_fWanderElapsed = 0.f;

}

void CMonster::AttackPattern(_float dt)
{
	//각각의 몬스터에 맞춰서 패턴을 오버라이딩 하시오.
}

void CMonster::ChasePlayer(_float dt)
{
	//타겟을 350거리까지 추격한다.
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	// 현재 위치
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 방향 계산
	_float3 vDir = TargetPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	//원래방향으로 턴하기
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);
	m_pTransformCom->Chase(TargetPos, dt, 200.f);
}


_float3 CMonster::CalculateEffectPos()
{
	// 충돌 위치 가져오기
	_float3 vImpactPos = m_pCollider->Get_Last_Collision_Pos();

	// 카메라의 Look 벡터 가져오기
	_float4x4 vCameraLook;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &vCameraLook);
	vCameraLook.MakeInverseMat(vCameraLook);
	_float3 vCameraPos = { vCameraLook._31, vCameraLook._32, vCameraLook._33 };

	// Look 벡터 정규화 (크기 1로 조정)
	vCameraPos.Normalize();

	// Look 방향으로 이동 (앞으로 1.0f 만큼 밀기)
	vImpactPos -= vCameraPos * 10.0f;

	return vImpactPos;
}


// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Archangel.h"
#include "FXMgr.h"
#include "MonsterNormalBullet.h"

CArchangel::CArchangel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CArchangel::CArchangel(const CArchangel& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CArchangel::Initialize_Prototype()
{
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Archangel_Walk");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 350;
	m_iMaxHP = 350;
	m_iAttackPower = 20;
	m_iDefense = 3;
	m_fSpeed = 13.f;
	m_vScale = { 56.7f, 130.f, 1.f };
	// 81 192
	m_eState = MODE::MODE_DETECTIVE;

	m_fDetectiveDistance = 300.f;

	//부속성
	m_strDialogue = "Archangel..Archangel...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("ArchangelDropItem");

	return S_OK;
}

HRESULT CArchangel::Initialize(void* pArg)
{
	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;
	m_fCooldownTime = 1.5f;     // 공격 쉬는 텀
	m_fBulletCooldown = 0.03f;	// 총알 발사 쿨
	m_fAttackTime = 0.2f;		// 공격 시간
	m_fTrailDuration = 0.5f;	// 잔상이 사라지는 시간	

	return S_OK;
}

void CArchangel::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CArchangel::Update(_float fTimeDelta)
{
	Update_TrailData(fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CArchangel::Late_Update(_float fTimeDelta)
{
	//플레이어 감지 업데이트
	PlayerDistance();
	CalculateVectorToPlayer();

	//콜라이더 업데이트
	m_pCollider->Update_Collider();

	//그래비티 업데이트
	if (m_bGravity)
		m_pGravityCom->Update(fTimeDelta);

	//몬스터 각도업데이트
	Compute_ViewAngle();
	Set_TextureType();

	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;


	if (m_bRotateAnimation == false)
		m_iDegree = 0;
	Resize_Texture(0.7f);
}

HRESULT CArchangel::Render()
{
	if (KEY_DOWN(DIK_LCONTROL))
	{
		int a = 0;
	}
	__super::Render();
	if (m_eState == MODE_READY || m_eState == MODE_BATTLE)
	{
		Render_TrailData(); 
	}						
	return S_OK;
	//특별히 더 렌더링 할게 있는 경우 ↓
}

void CArchangel::MonsterTick(_float dt)
{
	//상태변화
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		State_Change_IDLE(dt);
		break;

	case MODE::MODE_DETECTIVE:
		State_Change_DETECTIVE(dt);
		break;

	case MODE::MODE_READY:
		State_Change_READY(dt);
		break;

	case MODE::MODE_BATTLE:
		State_Change_BATTLE(dt);
		break;

	case MODE::MODE_DEAD:
		break;

	case MODE::MODE_RETURN:
		//본래위치로 돌아가고 IDLE로 상태가 변한다.
		m_bFoundPlayer = false;
		break;
	}

#ifdef _CONSOL
	Debug_Output();
#endif

	// 상태행동(액션)
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		DoIdle(dt);
		break;

	case MODE::MODE_DETECTIVE:
		DoDetect(dt);
		break;

	case MODE::MODE_READY:
		DoReady(dt);
		break;

	case MODE::MODE_BATTLE:
		DoBattle(dt);
		break;

	case MODE::MODE_DEAD:
		m_eCurMonsterState = STATE_DEAD;
		DoDead(dt);
		break;

	case MODE::MODE_RETURN:
		DoReturn(dt);
		break;
	}
}

void CArchangel::DoDetect(_float dt)
{
	// 감지 가능 거리 이내일 때 / 감지 상태 중 추격 가능 거리일 때
	ChasePlayer(dt, 50.f);

	m_eCurMonsterState = STATE_MOVE;
}

void CArchangel::DoReady(_float dt)
{
	m_fCooldownDuration += dt;
	m_eCurMonsterState = STATE_STAY;
	if (m_fCooldownDuration >= m_fCooldownTime)
	{
		m_isReadyToAttack = true;
		rand() % 10 > 7 ? m_eAttackPattern = ATTACK_FLY1 : m_eAttackPattern = ATTACK_FIRE1;
		if (m_eAttackPattern == ATTACK_FLY1) m_eCurMonsterState = STATE_FLY;
		if (m_eAttackPattern == ATTACK_FIRE1) m_eCurMonsterState = STATE_ATTACK;
		m_fBulletCooldownElapsed = 0.4f;
		m_fCooldownDuration = 0.f;
	}
	m_fAnimationFrame = 0.f;
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(TargetPos);
}

void CArchangel::DoIdle(_float dt)
{
	switch (m_eIdlePhase)
	{
	case EIdlePhase::IDLE_MOVE:
	{
		m_eCurMonsterState = STATE_MOVE;
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
		m_eCurMonsterState = STATE_STAY;

		if (m_fIdleWaitElapsed >= m_fIdleWaitTime)
		{
			SetRandomDirection();                  // 회전할 방향 설정
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN; // 다음엔 회전하러 간다
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		m_eCurMonsterState = STATE_STAY;
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

void CArchangel::DoBattle(_float dt)
{
	_float fChaseDistance = 500.f;
	if (m_fCurDistance > fChaseDistance && m_eState != MODE_BATTLE)
	{
		m_eState = MODE::MODE_BATTLE;
		m_eAttackPattern = ATTACK_FLY1;
		m_eCurMonsterState = STATE_FLY;
	}
	else
	{
		AttackPattern(dt);
	}
}

void CArchangel::AttackPattern(_float dt)
{	// 실제 공격 패턴 작성하는 곳

	switch (m_eAttackPattern)
	{
	case Client::CArchangel::ATTACK_FLY1:
	case Client::CArchangel::ATTACK_FLY2:
		FlyPattern(dt);
		break;

	case Client::CArchangel::ATTACK_FIRE1:
	case Client::CArchangel::ATTACK_FIRE2:
		FirePattern(dt);
		break;

	default:
		break;
	}

}

void CArchangel::FlyPattern(_float dt)
{
	_float3 vCurPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	switch (m_eAttackPattern)
	{
	case Client::CArchangel::ATTACK_FLY1:
		m_bGravity = false;
		m_pTransformCom->Go_Up(dt*1.3f);
		m_fFlyingUpTime += dt;
		if (m_fFlyingUpTime >= 0.4f)
		{
			m_eAttackPattern = ATTACK_FLY2;
			m_vTargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
			m_vTargetPos.y = vCurPos.y;
			m_fFlyingUpTime = 0.f;
		}
		m_fAnimationFrame += dt * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame - 1.f)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		break;

	case Client::CArchangel::ATTACK_FLY2:
		Make_TrailData(dt);
		m_pTransformCom->ChaseWithOutY(m_vTargetPos, dt, 3.f, 600.f);
		if ((vCurPos - m_vTargetPos).Length() < 10.f)
		{
			m_bCoolingDown = true;
			m_bGravity = true;
		}
		break;

	default:
		break;
	}
}

void CArchangel::FirePattern(_float dt)
{
	//m_bCoolingDown = true;
	switch (m_eAttackPattern)
	{
	case Client::CArchangel::ATTACK_FIRE1:
		m_fAnimationFrame += dt * m_fAnimationSpeed;
		if (m_fAnimationFrame >= 1.f)
			m_fAnimationFrame = 1.f;
		m_fFireReadyTime += dt;
		if (m_fFireReadyTime >= 0.5f)
		{
			m_eAttackPattern = ATTACK_FIRE2;
			m_fFireReadyTime = 0.f;
		}
		break;

	case Client::CArchangel::ATTACK_FIRE2:
		m_fAnimationFrame += dt * m_fAnimationSpeed;
		if (m_fAnimationFrame >= 3.f )
			m_fAnimationFrame = 3.f;
		m_fAttackTimer += dt;
		m_fBulletCooldownElapsed += dt;
		if (m_fAttackTimer >= m_fAttackTime)
		{
			m_fAttackTimer = 0.f;
			m_bCoolingDown = true;
		}
		if (m_fBulletCooldownElapsed >= m_fBulletCooldown)
		{
			_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->LookAt(TargetPos);

			CMonsterNormalBullet::DESC MonsterNormalBullet_iDesc{};
			MonsterNormalBullet_iDesc.iColliderID = CI_MONSTER_ARCHANGEL;
			MonsterNormalBullet_iDesc.fSpeedPerSec = 1000.f;
			MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
			MonsterNormalBullet_iDesc.vScale = { 30.f, 30.f, 1.f };
			MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			MonsterNormalBullet_iDesc.vPosition.y += 45.f;
			MonsterNormalBullet_iDesc.szTextureID = TEXT("BlueFire");
			MonsterNormalBullet_iDesc.bBlueFire = true;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
				m_eLevelID, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
				return;

			m_fBulletCooldownElapsed = 0.f;
		}
		break;

	default:
		break;
	}
}

void CArchangel::ChasePlayer(_float dt, _float fChaseDist)
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
	m_pTransformCom->ChaseWithOutY(TargetPos, dt, fChaseDist, 150.f);
}

HRESULT CArchangel::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	Ready_Textures();

	return S_OK;
}

HRESULT CArchangel::Ready_Textures()
{
	/* MOVE */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}
	/* ATTACK */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}
	/* FLY */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Fly_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Fly_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY][i])))))
			return E_FAIL;
	}
	/* STAY */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Stay_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Stay_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_STAY][i])))))
			return E_FAIL;
	}

	/* DEATH */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Archangel_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Archangel_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	return S_OK;
}


HRESULT CArchangel::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CArchangel::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_FLY:
			m_fAnimationMaxFrame = _float(MAX_FLY);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_STAY:
			m_fAnimationMaxFrame = _float(MAX_STAY);
			m_fAnimationSpeed = 0.f;
			break;
		case Client::CArchangel::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CArchangel::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CArchangel::STATE_STAY:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CArchangel::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_FLY: // 삭제?
		//m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		//if (m_fAnimationFrame >= m_fAnimationMaxFrame)
		//	m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_ATTACK: // 삭제?
		//m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		//if (m_fAnimationFrame >= m_fAnimationMaxFrame)
		//	m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		m_bRotateAnimation = false;
		break;
	}
	return S_OK;
}

void CArchangel::Update_TrailData(_float dt)
{
	// 잔상 데이터 업데이트
	std::queue<TrailData> updatedQueue;
	while (!m_TrailDataQueue.empty()) {
		TrailData trailData = m_TrailDataQueue.front();
		m_TrailDataQueue.pop();
		trailData.timeElapsed += dt;
		if (trailData.timeElapsed < m_fTrailDuration) {
			updatedQueue.push(trailData);
		}
	}
	m_TrailDataQueue = updatedQueue;
}

void CArchangel::Render_TrailData()
{
	// 잔상 렌더링
	queue<TRAILDATA> tempQueue = Sorted_TrailData();
	while (!tempQueue.empty())
	{
		TRAILDATA trailData = tempQueue.front();
		tempQueue.pop();

		// 잔상 위치로 변환 행렬 설정
		_float4x4 matWorld = trailData.matWorld;
		//D3DXMatrixTranslation(&matWorld, trailData.position.x, trailData.position.y, trailData.position.z);
		if (!m_bCW || m_iDegree == 0 || m_iDegree == 180.f / m_fDivOffset)
			Trail_Billboard(matWorld);
		else
			Trail_Billboard(matWorld, true); // Billboard Inverse

		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);

		// 잔상 렌더링 (예: 반투명한 텍스처 사용)
		m_pTextureMap[m_iState][m_iDegree]->Bind_Shader_To_Texture(m_pShaderCom, "Tex", static_cast<_uint>(m_fAnimationFrame));
		//m_pTextureMap[m_iState][m_iDegree]->Bind_Resource(static_cast<_uint>(m_fAnimationFrame));
		m_pShaderCom->SetFloat("opacity", 1.f - (trailData.timeElapsed / m_fTrailDuration));

		m_pVIBufferCom->Bind_Buffers();
		SetUp_RenderState();
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pShaderCom->Begin(CShader::SANDEVISTAN);
		m_pVIBufferCom->Render();
		m_pShaderCom->End();
		Release_RenderState();
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

void CArchangel::Trail_Billboard(_float4x4& matWorld, _bool isInverse) const
{
	_int iInverse = isInverse ? -1 : 1;
	_float4x4 CopyMatWorld = matWorld;
	//_float4x4 m_Return;
	_float3	vScaled = m_pTransformCom->Compute_Scaled();
	_float3	vPosition = *reinterpret_cast<_float3*>(CopyMatWorld.m[3]);

	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	_float3 vRight = reinterpret_cast<_float3*>(&matCamWorld.m[0][0])->Normalize() * vScaled.x * (_float)iInverse;
	_float3 vLook = reinterpret_cast<_float3*>(&matCamWorld.m[2][0])->Normalize() * vScaled.z;

	memcpy(&matWorld._11, &vRight, sizeof _float3);
	memcpy(&matWorld._21, *reinterpret_cast<_float3*>(CopyMatWorld.m[1]), sizeof _float3);
	memcpy(&matWorld._31, &vLook, sizeof _float3);
	memcpy(&matWorld._41, &vPosition, sizeof _float3);
}

void CArchangel::Make_TrailData(_float dt)
{
	m_fTrailTimer += dt;
	if (m_fTrailTimer >= 0.05f)
	{
		TRAILDATA newTrailData;
		newTrailData.position = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		newTrailData.matWorld = *m_pTransformCom->Get_WorldMatrix();
		newTrailData.size = m_pTransformCom->Compute_Scaled();
		newTrailData.timeElapsed = 0.0f;
		m_TrailDataQueue.push(newTrailData);
		m_fTrailTimer = 0.f;
	}
}

queue<CArchangel::TRAILDATA> CArchangel::Sorted_TrailData()
{
	queue<TRAILDATA> TempQueue = m_TrailDataQueue;
	list<TRAILDATA> SortedList;

	_float4x4	matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3		vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_float3));

	while (!TempQueue.empty())
	{
		SortedList.push_back(TempQueue.front());
		TempQueue.pop();
	}
	SortedList.sort([&vCamPos](const TRAILDATA& a, const TRAILDATA& b) {
		return (a.position - vCamPos).Length() > (b.position - vCamPos).Length();
		});
	for (auto& data : SortedList)
	{
		TempQueue.push(data);
	}
	return TempQueue;
}

void CArchangel::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	__super::On_Collision_NormalMonster(MyColliderID, OtherColliderID);
}

CArchangel* CArchangel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArchangel* pInstance = new CArchangel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArchangel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CArchangel::Clone(void* pArg)
{
	CArchangel* pInstance = new CArchangel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CArchangel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArchangel::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	while (!m_TrailDataQueue.empty())
		m_TrailDataQueue.pop();
}
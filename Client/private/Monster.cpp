#include "Monster.h"
#include "DebugDraw.h"
#include "Skull.h"

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
	//ũ������Ʈ���� �ʱ�ȭ�Ǵ� ��� ���� �������ش�.
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	/* �÷��̾ �˰� �־�� */
	m_pTargetPlayer = GET_PLAYER;
	Safe_AddRef(m_pTargetPlayer);

	/* �ؽ�ó, Ʈ������, ��Ʈ����, �ݶ��̴� ������Ʈ �غ�(��ġ�ʱ�ȭ) */
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	m_fBulletCooldown = 0.2f; // �ӽ÷� ����, �ڽ����� ���� ���� (��԰� ��� ������ ������ ��)

	CSkull::DESC SkullDesc{};
	SkullDesc.eLevelID = LEVEL_STATIC;
	SkullDesc.vScale = { 1.f,1.f,1.f };


	//// ���� �ذ����� �ڵ�
	m_pSkull = static_cast<CSkull*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC,
			TEXT("Prototype_GameObject_Skull"),&SkullDesc));

	m_vSkullOffset = m_pTransformCom->Compute_Scaled();
	m_vSkullOffset.x = 0.f;
	m_vSkullOffset.y *= 0.5f;
	m_vSkullOffset.z = 0.f;
	//////

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	//������ ������Ʈ
	Set_Animation();
	Animate_Monster(fTimeDelta);
}

EVENT CMonster::Update(_float fTimeDelta)
{
	if (m_bDead) // ��� üũ
	{
		m_fDeadBodyCounter += fTimeDelta;
		if (m_fDeadBodyCounter > 5000.f)
			return EVN_DEAD;
	}

	if (m_bActive)
	{
		MonsterTick(fTimeDelta);
	}

	if (m_bSkullActive)
	{
		const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pSkull->Update(vPos+m_vSkullOffset,fTimeDelta);
	}

	return EVN_NONE;
}

void CMonster::Late_Update(_float fTimeDelta)
{
	//�÷��̾� ���� ������Ʈ
	PlayerDistance();
	CalculateVectorToPlayer();
	
	if (m_bDead)
	{
		_float3 vOrigSize = {};
		m_pTextureMap[m_iState][m_iDegree]->Get_TextureSize(static_cast<_uint>(m_fAnimationFrame), &vOrigSize);
		_float fComputedSizeYFromOrig = -vOrigSize.y*0.5f + 20.f;
		// ���� ���� ���ݸ�ŭ ���� ���� ���α��̸�ŭ ���ϱ� 
		auto newY = m_vScale.y - 20.f;
		//m_pCollider->Update_OffSet({ 0.f, -10.f, 0.f });
		m_pCollider->Update_OffSet({ 0.f, -10.f, 0.f });
		//m_pCollider->Update_Scale({ vOrigSize.x, 20.f, 1.f });
		m_pCollider->Update_Scale({ vOrigSize.x * 0.5f, 3.f, 1.f });
		
		if (m_pHeadCollider != nullptr)
			m_pHeadCollider->Update_Scale({ 0.f,0.f,0.f });
	}

  	m_pCollider->Update_Collider();
	if (m_pHeadCollider != nullptr)
  		m_pHeadCollider->Update_Collider();


	//�׷���Ƽ ������Ʈ
	m_pGravityCom->Update(fTimeDelta);

	//���� ����������Ʈ
	Compute_ViewAngle();
	Set_TextureType();

	//�����׷� ������Ʈ
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	if (m_bSkullActive)
		m_pSkull->Late_Update(fTimeDelta);
}

void CMonster::Render_Skull(_bool bOn)
{
	m_bSkullActive = bOn;
	if(bOn)
		m_pSkull->TimeReset();
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

	//if (!m_bRotateAnimation)
	//	m_iDegree = 0;

	if (FAILED(m_pTextureMap[m_iState][m_iDegree]->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
		return E_FAIL;

	if (!m_bCW || m_iDegree == 0 || m_iDegree == 180.f / m_fDivOffset) {
		if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard()))
			return E_FAIL; 
	}
	else {
		if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard_Inverse()))
			return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	Release_RenderState();

#ifdef _COLLIDERRENDER
	if (m_pCollider != nullptr)
		m_pCollider->Render();
	if (m_pHeadCollider != nullptr)
		m_pHeadCollider->Render();
#endif

	return S_OK;
}

HRESULT CMonster::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	return S_OK;
}

void CMonster::FrameUpdate(_float timeDelta, _float _MaxFrame, _float fSpeed, _bool isLoop)
{
	if (isLoop)
	{
		//���� �������� �ƽ������Ӻ��� ũ�� ? 0 ���� �ٲ��.
		if (_MaxFrame <= m_fAnimationFrame)
		{
			m_fAnimationFrame -= _MaxFrame;
		}
	}
	else
	{
		//�ݺ��� �ƴϸ� �׳� ���� 
		if (_MaxFrame <= m_fAnimationFrame)
			return;
	}
	m_fAnimationFrame += fSpeed * timeDelta;

}

HRESULT CMonster::Ready_Components(void* pArg)
{

	/* ��Ʈ ���� ������Ʈ */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Ʈ������ ������Ʈ */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	/* ��ġ, ������ �ʱ�ȭ */
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(m_vScale);
		m_bActive = pDesc->vActive;
		m_vReturnPos = pDesc->vReturnPos;
		m_iNum = pDesc->iNums;
		m_eLevelID = pDesc->eLevel;
		m_fAttackDistance = pDesc->fAttackDistance;
	}

	/* �ݶ��̵� ������Ʈ */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};	
	ColliderDesc.pTransform = m_pTransformCom;
	//ColliderDesc.vOffSet = {0.f,10.f,0.f};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MONSTER;
	ColliderDesc.iColliderID = CI_MON_BODY;
	
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
	// ���� ������ �Ųٷα淡 ����

	_float3	vBillLook = {};
	_float3 temp = *(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) - vCameraPos;
	temp.y = 0.f;
	vBillLook = *D3DXVec3Normalize(&vBillLook, &temp);
	
	//vCurLook.y = vBillLook.y = 0.f;

	// ������ ���� ���
	float dotProduct = D3DXVec3Dot(&vCurLook, &vBillLook);

	// �������� ���� cosTheta�� Ŭ���� (-1 <= cosTheta <= 1)
	float cosTheta = max(-1.0f, min(1.0f, dotProduct));

	// ������ �������� ���
	float angleInRadians = acos(cosTheta);

	// ������ ��(degree)�� ��ȯ
	m_fPlayersViewAngle = D3DXToDegree(angleInRadians);

	float crossProduct = vCurLook.x * vBillLook.z - vCurLook.z * vBillLook.x;

	if (crossProduct > 0) 
		m_bCW = false;
	else
		m_bCW = true;
}

void CMonster::Resize_Texture(_float fSizePercent)
{
	_float3 vScale = { 0.f, 0.f, 0.f };
	m_pTextureMap[m_iState][m_iDegree]->Get_TextureSize(static_cast<_uint>(m_fAnimationFrame), &vScale);
	vScale.z = 1.f; // 0�� ���ͼ� �躤�Ͱ� �׾��������... 
	m_pTransformCom->Scaling(vScale * fSizePercent);
	m_vScale = m_vScale;
}

void CMonster::On_Collision_NormalMonster(_uint MyColliderID, _uint OtherColliderID)
{
	CMonster::On_Collision(MyColliderID, OtherColliderID);

	// �Ϲ� ���� On_Collision �Լ��� ���� ���ϵ� �� ���Ƽ� ������ ����.
	if (CI_BLOCK(OtherColliderID))
	{
		Collision_With_Block();
	}
	else if (CI_WEAPON(OtherColliderID))
	{
		Collision_With_Weapon();
	}
}

void CMonster::Collision_With_Weapon()
{
	if (!m_bFoundPlayer)
	{
		m_bFoundPlayer = true;
		m_eState = MODE::MODE_DETECTIVE;
	}

	//��ġŽ��
	_float3 vImpactPos = CalculateEffectPos();

	// ����Ʈ ����
	FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);

	//���� ���
	if (0 >= m_iHP)
	{
		//FX_MGR->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 130.f, 160.f, 1.f }, TEXT("PC_Explosion"), 14);
		m_bDead = true;
		m_eState = MODE_DEAD;
		_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->LookAt(TargetPos);

		// �ڷ� �˹�
		//if (m_fAnimationFrame >= m_fAnimationMaxFrame - 1.f)
			m_bKnockBack = true;

		return;
	}
}

void CMonster::Collision_With_Block()
{
	m_pCollider->Get_Last_Collision_Pos();

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 Depth = m_pCollider->Get_Last_Collision_Depth();
	if (Depth.y != 0)
		int a = 1;
	vPos += Depth;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

_bool CMonster::IsMonsterAbleToAttack()
{
	// ���� ����ĳ�������� �÷��̾�� ���� ���� ��ֹ� ���� üũ
	m_fRaycastTicker = 0.f;
	if (m_fCurDistance > m_fAttackDistance)
		return false;
	//return Raycast_Player();
	return true;
}

void CMonster::Debug_Output()
{
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();

	if (elapsed >= 1000)
	{
		switch (m_eState)
		{
		case Client::CMonster::MODE_IDLE:
			//cout << "IDLE";
			break;
		case Client::CMonster::MODE_READY:
			break;
		case Client::CMonster::MODE_BATTLE:
			//cout << "BATTLE";
			break;
		case Client::CMonster::MODE_DETECTIVE:
			//cout << "DETECTIVE";
			break;
		case Client::CMonster::MODE_RETURN:
			//cout << "RETURN";
			break;
		case Client::CMonster::MODE_END:
			//cout << "UNKNOWN";
			break;
		default:
			//cout << "UNKNOWN";
			break;
		}
		//cout << endl;
		g_LastLogTime = now;
	}
}

void CMonster::State_Change_IDLE(_float dt)
{
	if (IsPlayerDetected())	// ���� �Ÿ� ���� ���
	{
		m_bFoundPlayer = true;
		//�÷��̾� �߰� �� �ൿ
		if (IsMonsterAbleToAttack())
			m_eState = MODE::MODE_DETECTIVE;
	}
}

void CMonster::State_Change_DETECTIVE(_float dt)
{
	//�÷��̾� ���� ������ �� ���� Ž��
	m_fRaycastTicker += dt;
	if (m_fRaycastTicker > 0.5f)
	{
		if (IsMonsterAbleToAttack())	// RayPicking���� �÷��̾�� ���� ���� ��ֹ� üũ
		{
			m_bFoundPlayer = true;
			m_eState = MODE::MODE_READY;
		}
	}
}

void CMonster::State_Change_READY(_float dt)
{
	//���� �� �غ�  ( ���� ���� ������ �ʿ� )
	m_fRaycastTicker += dt;
	if (m_fRaycastTicker > 0.5f)
	{
		if (!IsMonsterAbleToAttack())
		{
			m_eState = MODE::MODE_DETECTIVE;
			return;
		}
	}
	// �غ� ������
	if (m_isReadyToAttack)
		m_eState = MODE::MODE_BATTLE;
}

void CMonster::State_Change_BATTLE(_float dt)
{
	if (m_bCoolingDown)
	{
		m_fCooldownDuration = 0.f;
		m_eState = MODE::MODE_READY;
		m_isReadyToAttack = false;
		m_bCoolingDown = false;
	}
	m_fRaycastTicker += dt;
	if (m_fRaycastTicker > 0.5f)
	{
		if (false == IsMonsterAbleToAttack())
		{
			m_fCooldownDuration = 0.f;
			m_eState = MODE::MODE_DETECTIVE;
			m_isReadyToAttack = false;
			m_bCoolingDown = false;
		}
	}
}


HRESULT CMonster::Set_TextureType()
{
	_float degree = m_fPlayersViewAngle / m_fDivOffset;
	_float div = 0.f;
	_float mod = modff(degree, &div);

	m_iDegree = (_uint)div + (_uint)(mod > 0.5f ? 1 : 0); // ���� �������� +- �ϴ� ��

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
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pHeadCollider);
	Safe_Release(m_pTargetPlayer);
	Safe_Release(m_pGravityCom);
	Safe_Release(m_pSkull);
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
	//ī�޶��� ��ġ, //������ ��ġ // �� ������ �Ÿ�
	_float4x4 matCamWorld;

	//ī�޶��� ��ġ����
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	//������ ��ġ
	_float3 vMonsterPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	// �� �� ������ �Ÿ� ���
	_float distance = sqrtf(
		powf(vCameraPos.x - vMonsterPos.x, 2) +
		powf(vCameraPos.y - vMonsterPos.y, 2) +
		powf(vCameraPos.z - vMonsterPos.z, 2)
	);

	m_fCurDistance = distance;
}

void CMonster::CalculateVectorToPlayer()
{
	//���� : ���Ϳ��� �÷��̾�� ���ϴ� ���⺤�͸� ���Ѵ�.

	// ���� ��ġ
	_float3 vMonsterPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// �÷��̾� ��ġ
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	// ���� ���� = Ÿ�� ��ġ - �� ��ġ
	m_vToPlayer = vPlayerPos - vMonsterPos;

	// ����ȭ�ؼ� ���⸸ ����� (���� 1)
	D3DXVec3Normalize(&m_vToPlayer, &m_vToPlayer);
	
}

bool CMonster::IsPlayerDetected() // �� �Լ� ���� ���� �ص� �ɱ�� 
{
	if (m_fCurDistance < 100.f && m_fCurDistance > 0.f)
		return true;
	// �Ÿ� üũ (����Ÿ��� �����Ÿ����� ���� ��)
	if (m_fCurDistance < m_fDetectiveDistance)
	{
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		m_vToPlayer.y = 0.f;
		vLook.y = 0.f;

		float fDot = D3DXVec3Dot(&m_vToPlayer, &vLook);  // 3. ���� ��

		if (fDot > cosf(D3DXToRadian(30)))          // 4. ���� ���� �̳�
		{
			return true;
		}
	}
	return false;
}

void CMonster::Render_DebugFOV()
{
	//���� ������
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//������ ����(�𷺼�)
	_float3 vForward = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	vForward.y = 0.f;
	D3DXVec3Normalize(&vForward, &vForward);

	// �þ� ���� ���� (��: 60�� �þ� �� 30���� ����)
	float fHalfFOV = D3DXToRadian(20.f / 2.0f);
	float fLength = 100.0f;  // �þ� �Ÿ�

	// ȸ�� ��� ���� (Y�� ���� ȸ��)
	_float4x4 matRotLeft, matRotRight;
	D3DXMatrixRotationY(&matRotLeft, -fHalfFOV);
	D3DXMatrixRotationY(&matRotRight, fHalfFOV);

	// ���� ���� ȸ��
	_float3 vLeft, vRight;
	D3DXVec3TransformNormal(&vLeft, &vForward, &matRotLeft);
	D3DXVec3TransformNormal(&vRight, &vForward, &matRotRight);

	// �� �� ���
	_float3 vLeftEnd = vPos + vLeft * fLength;
	_float3 vRightEnd = vPos + vRight * fLength;

	Get_CamaraPos;
	_float3 vMonster = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToMonster = vMonster - vCameraPos;
	D3DXVec3Normalize(&vToMonster, &vToMonster);

	_float3 vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };
	float fDot = D3DXVec3Dot(&vToMonster, &vCameraLook);


	if (fDot > 0.5f) // 60�� �̳� ����
	{
		// ����� �� �׸���
		CDebugDraw::DrawLine(m_pGraphic_Device, vPos, vLeftEnd, D3DCOLOR_ARGB(255, 255, 255, 0));   // ��� ��
		CDebugDraw::DrawLine(m_pGraphic_Device, vPos, vRightEnd, D3DCOLOR_ARGB(255, 255, 255, 0));  // ��� ��
		CDebugDraw::DrawLine(m_pGraphic_Device, vLeftEnd, vRightEnd, D3DCOLOR_ARGB(100, 255, 255, 0)); // ��� ���ἱ
	}
}

const char* CMonster::GetMonsterStateName(MODE eState)
{
	switch (eState)
	{
	case MODE::MODE_IDLE:        return "IDLE";
	case MODE::MODE_DETECTIVE:   return "DETECTIVE";
	case MODE::MODE_BATTLE:      return "BATTLE";
	case MODE::MODE_READY:       return "READY";
	case MODE::MODE_RETURN:      return "RETURN";
	default:                     return "UNKNOWN";
	}
}

_bool CMonster::Raycast_Player()
{
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = TargetPos - vMyPos;
	_uint colID = {};

	m_pGameInstance->Raycast(vMyPos, vDir, FLT_MAX, { CG_PAWN, CG_BLOCK }, colID);

	return CI_PLAYER(colID);
}


void CMonster::MonsterTick(_float fTimeDelta)
{
	//���º�ȭ
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		if (IsPlayerDetected())
		{
			m_eState = MODE::MODE_BATTLE;
		}
		break;

	case MODE::MODE_BATTLE:
		//��Ʋ ���ÿ� �ൿ
		break;
	case MODE::MODE_RETURN:
		//������ġ�� ���ư��� IDLE�� ���°� ���Ѵ�.
		break;
	}

	// �����ൿ(�׼�)
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
			SetRandomDirection();                  // ȸ���� ���� ����
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN; // ������ ȸ���Ϸ� ����
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		bool bRotated = m_pTransformCom->RotateToDirection(vLook, m_vDirection, 5.f, dt);
		if (bRotated)  // ȸ�� �Ϸ� ��ȣ
		{
			m_eIdlePhase = EIdlePhase::IDLE_MOVE;
		}
		break;
	}
	}
}

void CMonster::DoBattle(_float dt)
{
	// 1. �÷��̾���� �Ÿ� ���
	_float fAttackRange = 250.f;
	_float fChaseRange = 700.f;

	// 2. �Ÿ� ���� �б�
	if (m_fCurDistance < fAttackRange)
	{
		// ������ ����
		AttackPattern(dt);
		//cout << "������ ���� ��Ÿ� ���Դϴ�." << endl;
	}
	else if (m_fCurDistance < fChaseRange)
	{
		// �߰� �Ÿ��� ����
		ChasePlayer(dt);
		//cout << "���� �߰����Դϴ�!" << endl;
	}
	else
	{
		// �ʹ� �ָ� ���� ����
		m_eState = MODE_RETURN;
	}
}

void CMonster::DoReturn(_float dt)
{
	// ���� ��ġ
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// ���� ���
	_float3 vDir = m_vReturnPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	//������������ ���ϱ�
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);

	if (bRotated)  // ȸ�� �Ϸ� ��ȣ
	{
		// �ʹ� ������ �̵� ����
		if (fDistance < 1.0f) // ���� ���� ���� ���
		{
			m_eState = MODE::MODE_IDLE;
			return;
		}

		// �̵� ó�� (dt ����)
		float fSpeed = m_fSpeed;
		_float3 vMove = vDir * fSpeed * dt;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (vMyPos + vMove));
	}
}

void CMonster::DoReady(_float dt)
{
	m_fCooldownDuration += dt;
	if (m_fCooldownDuration >= m_fCooldownTime)
	{
		m_isReadyToAttack = true;
		m_fBulletCooldownElapsed = 0.4f;
		m_fCooldownDuration = 0.f;
	}
	m_fAnimationFrame = 0.f;
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(TargetPos);
}

void CMonster::DoDead(_float dt)
{
	if (m_bKnockBack)
	{
		m_pGravityCom->Jump(10.f);
		m_bKnockBack = false;
	}
	if (!m_bKnockBack && m_pGravityCom->isJump())
		m_pTransformCom->Go_Backward(dt * 3.f);
}

void CMonster::SetRandomDirection()
{
	// 0 ~ 359�� ������ ���� ���� ����
	float fAngle = D3DXToRadian(rand() % 360);  // rand() % 360 �� 0~359��

	// �����κ��� ���� ���� ����
	_float3 vDir = {
		sinf(fAngle),   // x��
		0.f,
		cosf(fAngle)    // z��
	};

	m_vDirection = vDir;

	// �̵� �ð� ����
	m_fWanderTime = (rand() % 2000) / 1000.f + 1.f;
	m_fWanderElapsed = 0.f;

}

void CMonster::AttackPattern(_float dt)
{
	//������ ���Ϳ� ���缭 ������ �������̵� �Ͻÿ�.
}

void CMonster::ChasePlayer(_float dt)
{
	//Ÿ���� 350�Ÿ����� �߰��Ѵ�.
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	// ���� ��ġ
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// ���� ���
	_float3 vDir = TargetPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	//������������ ���ϱ�
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);
	m_pTransformCom->ChaseWithOutY(TargetPos, dt, 200.f,100.f);
}


_float3 CMonster::CalculateEffectPos()
{
	// �浹 ��ġ ��������
	_float3 vImpactPos = m_pCollider->Get_Last_Collision_Pos();

	// ī�޶��� Look ���� ��������
	_float4x4 vCameraLook;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &vCameraLook);
	vCameraLook.MakeInverseMat(vCameraLook);
	_float3 vCameraPos = { vCameraLook._31, vCameraLook._32, vCameraLook._33 };

	// Look ���� ����ȭ (ũ�� 1�� ����)
	vCameraPos.Normalize();

	// Look �������� �̵� (������ 1.0f ��ŭ �б�)
	vImpactPos -= vCameraPos * 10.0f;
	//vImpactPos.y += m_vScale.y / 2.f;

	return vImpactPos;
}

void CMonster::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	/* [ �÷��̾��� ���⿡ ���� ������ HP �� ���̵��� �������� ] */

	/* �� �̰��� HP �� �����ϰ� ���� ����Ʈ�� �ؾ��� ó���� �������̵��ؼ� �Ͻÿ� ��  */

	/* �Ӹ��� �¾��� ��� */
	if (MyColliderID == CI_MON_HEAD)
		m_iHeadMultiplier = 2; // ������ 2��
	else
		m_iHeadMultiplier = 1;

	/* ���� */
	if (OtherColliderID == CI_LOVERBOY)
	{
		m_iHP -= 20 * m_iHeadMultiplier;
	}

	/* ����� */
	if (OtherColliderID == CI_CHAINGUN)
	{
		// 1�ʴ� 9�� == 27 ������
		m_iHP -= 10 * m_iHeadMultiplier;
	}

	/* ���� */
	if (OtherColliderID == CI_DISPENSOR_SHELL)
	{
		// 10���� ������ �Ÿ��� ��ź�� ����
		m_iHP -= 5 * m_iHeadMultiplier;
	}

	/* ���ʺ� */
	if (OtherColliderID == CI_DISPENSOR_GRENADE)
	{
		// 1���� �ؾ���
		m_iHP -= 50 * m_iHeadMultiplier;
	}


	//���� ��� ����
	if (OtherColliderID == CI_PICKING_RAY)
	{
		int a = 1;
	}
}


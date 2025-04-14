#include "Monster.h"
#include "DebugDraw.h"
#include "Skull.h"
#include "FXMgr.h"
#include "CameraManager.h"

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
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	m_pTargetPlayer = GET_PLAYER;
	Safe_AddRef(m_pTargetPlayer);

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	m_fBulletCooldown = 0.2f;

	CSkull::DESC SkullDesc{};
	SkullDesc.eLevelID = LEVEL_STATIC;
	SkullDesc.vScale = { 1.f,1.f,1.f };


	m_pSkull = static_cast<CSkull*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC,
			TEXT("Prototype_GameObject_Skull"),&SkullDesc));

	m_vSkullOffset = m_pTransformCom->Compute_Scaled();
	m_vSkullOffset.x = 0.f;
	m_vSkullOffset.y *= 0.5f;
	m_vSkullOffset.z = 0.f;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	Animate_Monster(fTimeDelta);
}

EVENT CMonster::Update(_float fTimeDelta)
{
	if (m_bDead) // ��� üũ
	{
		m_fDeadBodyCounter += fTimeDelta;
		if (m_fDeadBodyCounter > 10.f) // 여기부분 시체 남는 시간 조정용 
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
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	PlayerDistance();
	CalculateVectorToPlayer();
	
	if (m_bDead)
	{
		_float3 vOrigSize = {};
		m_pTextureMap[m_iState][m_iDegree]->Get_TextureSize(static_cast<_uint>(m_fAnimationFrame), &vOrigSize);
		_float fComputedSizeYFromOrig = -vOrigSize.y*0.5f + 20.f;
		
		auto newY = m_vScale.y - 20.f;
		//m_pCollider->Update_OffSet({ 0.f, -10.f, 0.f });
		m_pCollider->Update_OffSet({ 0.f, -10.f, 0.f });
		//m_pCollider->Update_Scale({ vOrigSize.x, 20.f, 1.f });
		m_pCollider->Update_Scale({ vOrigSize.x * 0.5f, 3.f, 1.f });
		
		if (m_pHeadCollider != nullptr)
		{
			m_pHeadCollider->Update_Scale({ 0.f,0.f,0.f });
			m_pHeadCollider->Update_OffSet({ -10000.f,-10000.f,-10000.f });
		}
	}

  	m_pCollider->Update_Collider();
	if (m_pHeadCollider != nullptr)
  		m_pHeadCollider->Update_Collider();

	m_pGravityCom->Update(fTimeDelta);

	Compute_ViewAngle();
	Set_TextureType();

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
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

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CMonster::BillboardShaderRender()
{
	if (!g_FogTrigger)
	{
		if (m_bDebug)
			Render_DebugFOV();

		//if (!m_bRotateAnimation)
		//	m_iDegree = 0;

		if (FAILED(m_pTextureMap[m_iState][m_iDegree]->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
			return E_FAIL;

		if (!m_bCW || m_iDegree == 0 || m_iDegree == 180.f / m_fDivOffset) 
		{
			if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard()))
				return E_FAIL;
		}
		else 
		{
			if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard_Inverse()))
				return E_FAIL;
		}

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		SetUp_RenderState();
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
		Release_RenderState();

		return S_OK;
	}
	else
	{
		SetUp_RenderState();

		/* [ 텍스처 셰이더로 넘기기 ] */
		if (FAILED(m_pTextureMap[m_iState][m_iDegree]->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", static_cast<_uint>(m_fAnimationFrame))))
			return E_FAIL;

		/* [ 메트릭스 셰이더로 넘기기 ] */
		_float4x4 maxView, maxProj;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &maxView);
		m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &maxProj);

		_float4x4 matBillboard;
		if (!m_bCW || m_iDegree == 0 || m_iDegree == 180.f / m_fDivOffset) 
		{
			matBillboard = m_pTransformCom->Billboard();
		}
		else 
		{
			matBillboard = m_pTransformCom->Billboard_Inverse();
		}

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &matBillboard)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &maxView)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &maxProj)))
			return E_FAIL;

		/* [ 카메라 위치 넘기기 ] */
		_float3 vCamPos;
		_float4x4 invView;
		D3DXMatrixInverse(&invView, nullptr, &maxView);
		vCamPos = *(_float3*)&invView._41;

		if (FAILED(m_pShaderCom->SetFloatArray("g_CameraPos", (_float*)&vCamPos, 3)))
			return E_FAIL;

		/* [ 안개 거리 설정 ] */
		_float fFogStart = 0.f;
		_float fFogEnd = 0.f;

		if (m_eLevelID == LEVEL_INDOOR)
		{
			fFogStart = INDOOR_START_FOG;
			fFogEnd = INDOOR_END_FOG;
		}
		else if (m_eLevelID == LEVEL_OUTDOOR)
		{
			fFogStart = OUTDOOR_START_FOG;
			fFogEnd = OUTDOOR_END_FOG;
			if (g_FogCustom <= 1995.f)
			{
				/* g_FogCustom 은 0까지 줄어든다 */
				fFogStart = g_FogCustom;
				fFogEnd = g_FogCustom + 700;
			}
		}
		else if (m_eLevelID == LEVEL_GAMEPLAY)
		{
			fFogStart = GAMEPLAY_START_FOG;
			fFogEnd = GAMEPLAY_END_FOG;
		}
		else if (m_eLevelID == LEVEL_UNDERGROUND)
		{
			fFogStart = UNDERGROUND_START_FOG;
			fFogEnd = UNDERGROUND_END_FOG;
		}
		else
		{
			fFogStart = 10000.f;
			fFogEnd = 15000.f;
		}

		if (FAILED(m_pShaderCom->SetFloat("g_FogStart", fFogStart)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetFloat("g_FogEnd", fFogEnd)))
			return E_FAIL;

		m_fShaderTime += m_pGameInstance->Get_Scaled_TimeDelta(TEXT("Timer_60"));
		if (FAILED(m_pShaderCom->SetFloat("g_fTime", m_fShaderTime)))
			return E_FAIL;

		/* [ 플래쉬 설정 ] */
		D3DXVECTOR4 ThunderPos = { FX_MGR->GetThunderPos(), 0.f };
		if (m_eLevelID == LEVEL_UNDERGROUND)
		{
			if (FX_MGR->IsFlashing())
			{
				ThunderPos = { *GET_PLAYER_TRANSFORM->Get_State(CTransform::STATE_POSITION), 0.f };
			}
			else
			{
				ThunderPos = { 0.f, 10000.f, 0.f, 0.f };
			}
		}


		if (m_eLevelID != LEVEL_OUTDOOR && m_eLevelID != LEVEL_UNDERGROUND)
			ThunderPos = { 0.f, 10000.f, 0.f, 0.f };


		if (FAILED(m_pShaderCom->SetVector("g_LightningPos", &ThunderPos)))
			return E_FAIL;
		
		/* [ 조명 세기 설정 ] */
		_float LigtIntensity = 0.5f;
		if (m_eLevelID == LEVEL_UNDERGROUND)
			LigtIntensity = 1.f;
		if (FAILED(m_pShaderCom->SetFloat("g_FlashIntensity", LigtIntensity)))
			return E_FAIL;


		/* [ 조명 거리 설정 ] */
		_float LigtRange = 600.f;
		if (m_eLevelID == LEVEL_UNDERGROUND)
			LigtRange = 350.f;
		if (FAILED(m_pShaderCom->SetFloat("g_LightRange", LigtRange)))
			return E_FAIL;

		/* [ 안개 색상 설정 ] */
		m_vFogColor = _float4(0.059f, 0.067f, 0.082f, 1.f);
		if (m_eLevelID == LEVEL_UNDERGROUND)
		{
			m_vFogColor = _float4(0.10f, 0.11f, 0.13f, 1.f);
			m_vFlashColor = _float4(1.0f, 0.75f, 0.3f, 1.f);
		}
		if (FAILED(m_pShaderCom->SetVector("g_FogColor", &m_vFogColor)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->SetVector("g_FlashColor", &m_vFlashColor)))
			return E_FAIL;

		//셰이더 시작
		m_pShaderCom->Begin(CShader::FOG);

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;


		/* ------------------------------------------------------------------------- */

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		/* ------------------------------------------------------------------------- */

		m_pShaderCom->End();
		Release_RenderState();
	}

	return S_OK;
}

HRESULT CMonster::Render()
{
	BillboardShaderRender();


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
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	return S_OK;
}

void CMonster::FrameUpdate(_float timeDelta, _float _MaxFrame, _float fSpeed, _bool isLoop)
{
	if (isLoop)
	{
		if (_MaxFrame <= m_fAnimationFrame)
		{
			m_fAnimationFrame -= _MaxFrame;
		}
	}
	else
	{
		if (_MaxFrame <= m_fAnimationFrame)
			return;
	}
	m_fAnimationFrame += fSpeed * timeDelta;

}

HRESULT CMonster::Ready_Components(void* pArg)
{

	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

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
		m_fDetectiveDistance = pDesc->fDetectiveDistance;
	}

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

	_float3	vBillLook = {};
	_float3 temp = *(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) - vCameraPos;
	temp.y = 0.f;
	vBillLook = *D3DXVec3Normalize(&vBillLook, &temp);
	
	//vCurLook.y = vBillLook.y = 0.f;

	float dotProduct = D3DXVec3Dot(&vCurLook, &vBillLook);

	float cosTheta = max(-1.0f, min(1.0f, dotProduct));

	float angleInRadians = acos(cosTheta);

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
	vScale.z = 1.f;
	m_pTransformCom->Scaling(vScale * fSizePercent);
	m_vScale = m_vScale;
}

void CMonster::On_Collision_NormalMonster(_uint MyColliderID, _uint OtherColliderID)
{
	CMonster::On_Collision(MyColliderID, OtherColliderID);

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

	_float3 vImpactPos = CalculateEffectPos();

	FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);

	if (0 >= m_iHP)
	{
		//FX_MGR->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 130.f, 160.f, 1.f }, TEXT("PC_Explosion"), 14);
		m_bDead = true;
		m_eState = MODE_DEAD;
		_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->LookAt(TargetPos);

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
	if (IsPlayerDetected())
	{
		m_bFoundPlayer = true;
		
		if (IsMonsterAbleToAttack() && m_eState != MODE::MODE_BATTLE)
			m_eState = MODE::MODE_DETECTIVE;
	}
}

void CMonster::State_Change_DETECTIVE(_float dt)
{
	m_fRaycastTicker += dt;
	if (m_fRaycastTicker > 0.5f)
	{
		if (IsMonsterAbleToAttack())
		{
			m_bFoundPlayer = true;
			m_eState = MODE::MODE_READY;
		}
	}
}

void CMonster::State_Change_READY(_float dt)
{
	m_fRaycastTicker += dt;
	if (m_fRaycastTicker > 0.5f)
	{
		if (!IsMonsterAbleToAttack())
		{
			m_eState = MODE::MODE_DETECTIVE;
			return;
		}
	}
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

	m_iDegree = (_uint)div + (_uint)(mod > 0.5f ? 1 : 0);

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
	Safe_Release(m_pShaderCom);
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
	_float4x4 matCamWorld;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	_float3 vMonsterPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float distance = sqrtf(
		powf(vCameraPos.x - vMonsterPos.x, 2) +
		powf(vCameraPos.y - vMonsterPos.y, 2) +
		powf(vCameraPos.z - vMonsterPos.z, 2)
	);

	m_fCurDistance = distance;
}

void CMonster::CalculateVectorToPlayer()
{
	_float3 vMonsterPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	m_vToPlayer = vPlayerPos - vMonsterPos;

	D3DXVec3Normalize(&m_vToPlayer, &m_vToPlayer);
	
}

bool CMonster::IsPlayerDetected()
{
	if (m_fCurDistance < 100.f && m_fCurDistance > 0.f)
		return true;

	if (m_fCurDistance < m_fDetectiveDistance)
	{
		if (FX_MGR->IsFlashing() && CAMERA_MANAGER->Get_CurCameraID() == CCameraManager::FPS)
			return true;

		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		m_vToPlayer.y = 0.f;
		vLook.y = 0.f;

		float fDot = D3DXVec3Dot(&m_vToPlayer, &vLook);

		if (fDot > cosf(D3DXToRadian(30)))
		{
			return true;
		}
	}
	return false;
}

void CMonster::Render_DebugFOV()
{
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vForward = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	vForward.y = 0.f;
	D3DXVec3Normalize(&vForward, &vForward);

	float fHalfFOV = D3DXToRadian(20.f / 2.0f);
	float fLength = 100.0f;  // �þ� �Ÿ�

	_float4x4 matRotLeft, matRotRight;
	D3DXMatrixRotationY(&matRotLeft, -fHalfFOV);
	D3DXMatrixRotationY(&matRotRight, fHalfFOV);

	_float3 vLeft, vRight;
	D3DXVec3TransformNormal(&vLeft, &vForward, &matRotLeft);
	D3DXVec3TransformNormal(&vRight, &vForward, &matRotRight);

	_float3 vLeftEnd = vPos + vLeft * fLength;
	_float3 vRightEnd = vPos + vRight * fLength;

	Get_CamaraPos;
	_float3 vMonster = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToMonster = vMonster - vCameraPos;
	D3DXVec3Normalize(&vToMonster, &vToMonster);

	_float3 vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };
	float fDot = D3DXVec3Dot(&vToMonster, &vCameraLook);


	if (fDot > 0.5f)
	{
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
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		if (IsPlayerDetected())
		{
			m_eState = MODE::MODE_BATTLE;
		}
		break;

	case MODE::MODE_BATTLE:
		break;
	case MODE::MODE_RETURN:
		break;
	}

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
			SetRandomDirection();
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN;
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		bool bRotated = m_pTransformCom->RotateToDirection(vLook, m_vDirection, 5.f, dt);
		if (bRotated)
		{
			m_eIdlePhase = EIdlePhase::IDLE_MOVE;
		}
		break;
	}
	}
}

void CMonster::DoBattle(_float dt)
{
	_float fAttackRange = 250.f;
	_float fChaseRange = 700.f;

	if (m_fCurDistance < fAttackRange)
	{
		AttackPattern(dt);
	}
	else if (m_fCurDistance < fChaseRange)
	{
		ChasePlayer(dt);
	}
	else
	{
		m_eState = MODE_RETURN;
	}
}

void CMonster::DoReturn(_float dt)
{
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = m_vReturnPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);

	if (bRotated)
	{
		if (fDistance < 1.0f)
		{
			m_eState = MODE::MODE_IDLE;
			return;
		}

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
	float fAngle = D3DXToRadian(rand() % 360); 

	_float3 vDir = {
		sinf(fAngle),
		0.f,
		cosf(fAngle)
	};

	m_vDirection = vDir;

	m_fWanderTime = (rand() % 2000) / 1000.f + 1.f;
	m_fWanderElapsed = 0.f;

}

void CMonster::AttackPattern(_float dt)
{
}

void CMonster::ChasePlayer(_float dt)
{
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = TargetPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);
	m_pTransformCom->ChaseWithOutY(TargetPos, dt, 200.f,100.f);
}


_float3 CMonster::CalculateEffectPos()
{
	_float3 vImpactPos = m_pCollider->Get_Last_Collision_Pos();

	_float4x4 vCameraLook;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &vCameraLook);
	vCameraLook.MakeInverseMat(vCameraLook);
	_float3 vCameraPos = { vCameraLook._31, vCameraLook._32, vCameraLook._33 };

	vCameraPos.Normalize();

	vImpactPos -= vCameraPos * 10.0f;
	//vImpactPos.y += m_vScale.y / 2.f;

	return vImpactPos;
}

void CMonster::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (MyColliderID == CI_MON_HEAD)
		m_iHeadMultiplier = 2;
	else
		m_iHeadMultiplier = 1;

	if (OtherColliderID == CI_LOVERBOY)
	{
		m_iHP -= 20 * m_iHeadMultiplier;
	}

	if (OtherColliderID == CI_CHAINGUN)
	{
		m_iHP -= 10 * m_iHeadMultiplier;
	}

	if (OtherColliderID == CI_DISPENSOR_SHELL)
	{
		m_iHP -= 5 * m_iHeadMultiplier;
	}

	if (OtherColliderID == CI_DISPENSOR_GRENADE)
	{
		m_iHP -= 25 * m_iHeadMultiplier;
	}

	if (OtherColliderID == CI_PICKING_RAY)
	{
		int a = 1;
	}
}


#include "Monster.h"

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
	, m_eBehavior(Prototype.m_eBehavior)
	, m_strDialogue(Prototype.m_strDialogue)
	, m_strSound(Prototype.m_strSound)
	, m_vDropItems(Prototype.m_vDropItems)
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
	FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, m_fAnimationSpeed, true);
}

EVENT CMonster::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CMonster::Late_Update(_float fTimeDelta)
{
	//플레이어 거리 업데이트
	PlayerDistance();
	
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
	Set_TextureType();

	if (m_bCW)
	{
		if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard_Inverse()))
			return E_FAIL;
	}
	else
	{
		if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard()))
			return E_FAIL;
	}

	if (m_isReadyMonster)
	{
		if (FAILED(m_pTextureMap[m_iState][m_iDegree]->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
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
	}

	/* 콜라이드 컴포넌트 */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};	
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = {};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

void CMonster::Compute_ViewAngle()
{
	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	_float3 vCurLook = {};
	vCurLook = *D3DXVec3Normalize(&vCurLook, (m_pTransformCom->Get_State(CTransform::STATE_LOOK)));

	_float3	vBillLook = {};
	_float3 temp = *(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) - vCameraPos;
	vBillLook = *D3DXVec3Normalize(&vBillLook, &temp);
	
	vCurLook.y = vBillLook.y = 0.f;

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
	if (KEY_DOWN(DIK_1)) // 테스트용
		int a = 0;

	_float degree = m_fPlayersViewAngle / m_fDivOffset;
	_float div = 0.f;
	_float mod = modff(degree, &div);

	m_iDegree = (_uint)div + (_uint)(mod > 0.5f ? 1 : 0); // 기준 각도에서 +- 하는 중

	// state는 보스의 상태에 따라 결정
	// 임시로 walk만 지정
	//m_iState = (_uint)(STATE_WALK);
	m_iState = 0;
	return S_OK;
}
HRESULT CMonster::Animate_Monster()
{



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

	m_fPlayerDistance = distance;
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


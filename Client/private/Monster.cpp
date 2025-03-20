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

	/* 텍스처, 트랜스폼, 렉트버퍼, 콜라이더 컴포넌트 준비 */
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	/* 위치, 스케일 초기화 */
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(m_vScale);
	}

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
}

EVENT CMonster::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CMonster::Late_Update(_float fTimeDelta)
{
	//플레이어 거리 업데이트
	PlayerDistance();

	//프레임 업데이트
	FrameUpdate(fTimeDelta, 11.f, 15.f, true);
	
	//콜라이더 업데이트
	m_pCollider->Update_Collider(m_pTransformCom);

	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CMonster::SetUp_RenderState()
{
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

	if (m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
		return E_FAIL;

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
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CMonster::Ready_Components(void* pArg)
{
	/* 텍스처 컴포넌트 */
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


	/* 콜라이드 컴포넌트 */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;


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

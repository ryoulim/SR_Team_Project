#include "RaceBossBomb.h"
#include "MonsterMissile.h"

CRaceBossBomb::CRaceBossBomb(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet { pGraphic_Device }
{
}

CRaceBossBomb::CRaceBossBomb(const CRaceBossBomb& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CRaceBossBomb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceBossBomb::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RaceBossBomb");
	m_szBufferType = TEXT("Rect");

	Ready_Components(pArg);

	//자신의 속도와 시작지점을 저장해 놓는다.
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_fSpeed = pDesc->fSpeedPerSec;
	m_vStartPos = pDesc->vPosition;

	//미사일 파티클
	CPSystem::DESC Missile_iDesc{};
	Missile_iDesc.vPosition = { 0.f, 0.f, 0.f };
	Missile_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	Missile_iDesc.iParticleNums = 100;
	Missile_iDesc.fSize = 12.f;
	Missile_iDesc.fMaxFrame = 20.f;
	Missile_iDesc.fNum = 40.f;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterMissile"),
		LEVEL_GAMEPLAY, L"Layer_Monster", ppOut, &Missile_iDesc)))
		return E_FAIL;

	m_pMissile = *ppOut;

	return S_OK;
}

void CRaceBossBomb::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceBossBomb::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	//따라오는 파티클
	if (m_pMissile)
		static_cast<CMonsterMissile*>(m_pMissile)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	/* [ 보스 미사일은 표적 위에서 소환되며 약간 사선으로 떨어진다 ] */
	m_pTransformCom->Go_Down(fTimeDelta);

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (vPos.y <= 0.f)
	{
		/* 미사일이 바닥에 박히면? 부모에게 메세지를 전달한다 */
		m_bDie = true;
		// 바닥에 터졌을때만 충돌처리 되도록 변경
		m_pCollider->Update_Collider();
		m_pCollider->Update_Scale({ 50.f,0.f,0.f });

		if (m_pMissile)
		{
			static_cast<CMonsterMissile*>(m_pMissile)->SetDead();
			m_pMissile = nullptr;
		}
	}
	
	return __super::Update(fTimeDelta);
}

void CRaceBossBomb::Late_Update(_float fTimeDelta)
{
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;

	//__super::Late_Update(fTimeDelta);
}

HRESULT CRaceBossBomb::SetUp_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}

HRESULT CRaceBossBomb::Render()
{
	SetUp_RenderState();
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard());

	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
		
	Release_RenderState();
	return S_OK;
}

HRESULT CRaceBossBomb::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	return S_OK;
}
 
void CRaceBossBomb::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	m_bDead = true;
}

HRESULT CRaceBossBomb::Ready_Components(void* pArg)
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
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(pDesc->vScale);
	}

	DESC* pDesc = static_cast<DESC*>(pArg);

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale.x = 1.f;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderID = CI_BOSS_BULLET;
	ColliderDesc.iColliderGroupID = CG_MBULLET;
	
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CRaceBossBomb* CRaceBossBomb::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceBossBomb* pInstance = new CRaceBossBomb(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceBossBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceBossBomb::Clone(void* pArg)
{
	CRaceBossBomb* pInstance = new CRaceBossBomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceBossBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceBossBomb::Free()
{
	__super::Free();
}

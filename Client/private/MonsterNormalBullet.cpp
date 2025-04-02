#include "MonsterNormalBullet.h"
#include "MonsterMissile.h"
#include "Particle_Define.h"
#include "CameraManager.h"

CMonsterNormalBullet::CMonsterNormalBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CMonsterNormalBullet::CMonsterNormalBullet(const CMonsterNormalBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CMonsterNormalBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonsterNormalBullet::Initialize(void* pArg)
{
	/* �÷��̾ �˰� �־�� */
	m_pTargetPlayer = GET_PLAYER;
	Safe_AddRef(m_pTargetPlayer);

	/* ī�޶� �˰� �־�� */
	m_pCamera = CAMERA_MANAGER;
	Safe_AddRef(m_pCamera);

	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("PC_Generic");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* [ Ÿ�� ���� ] */
	SetTargetDir();

	//�̻��� ��ƼŬ
	CPSystem::DESC Missile_iDesc{};
	Missile_iDesc.vPosition = { 0.f, 0.f, 0.f };
	Missile_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	Missile_iDesc.iParticleNums = 1000;
	Missile_iDesc.fSize = 0.07f;
	Missile_iDesc.fMaxFrame = 20.f;
	Missile_iDesc.fLifeTime = GetRandomFloat(1.f, 3.f);

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterMissile"),
		LEVEL_GAMEPLAY, L"Layer_Monster", ppOut, &Missile_iDesc)))
		return E_FAIL;

	m_pMissile = *ppOut;
	return S_OK;
}

HRESULT CMonsterNormalBullet::Ready_Components(void* pArg)
{
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
		//�� ������ �� ������ ���⼭ ��
		DESC* pDesc = static_cast<DESC*>(pArg);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_bGravity = pDesc->bGravity;
		m_szTextureID = pDesc->szTextureID;
		m_bAnimation = pDesc->bAnimation;
		m_fSpeed = pDesc->fSpeedPerSec;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* �ݶ��̵� ������Ʈ */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = {};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MBULLET;
	ColliderDesc.iColliderID = CI_MONSTER_BULLET;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	if (m_bGravity)
	{
		/* For.Com_Gravity */
		CGravity::DESC GravityDesc{};
		GravityDesc.pTransformCom = m_pTransformCom;
		GravityDesc.fTimeIncreasePerSec = 2.2f;
		GravityDesc.fMaxFallSpeedPerSec = 400.f;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Gravity"),
			TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void CMonsterNormalBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fLifetime += fTimeDelta;
	if (m_fLifetime >= 2.5f)
		m_bDead = true;
}

EVENT CMonsterNormalBullet::Update(_float fTimeDelta)
{
	if (m_bDead)
	{
		if (m_pMissile)
		{
			static_cast<CMonsterMissile*>(m_pMissile)->SetDead();
			m_pMissile = nullptr;
		}
		return EVN_DEAD;
	}
	if (m_pMissile)
		static_cast<CMonsterMissile*>(m_pMissile)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));


	//�÷��̾�� ���� ���󰡶�
	if (m_bGravity)
		m_pTransformCom->Go_Straight(fTimeDelta);
	else
		m_pTransformCom->ChaseCustom(m_vTargetPos, fTimeDelta, 1.f, m_fSpeed);

	return __super::Update(fTimeDelta);
}

void CMonsterNormalBullet::Late_Update(_float fTimeDelta)
{
	if (m_bGravity)
		m_pGravityCom->Update(fTimeDelta);

	//�����׷� ������Ʈ
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;


	if (m_bAnimation)
		FrameUpdate(fTimeDelta, 8.f, 15.f, true);
	__super::Late_Update(fTimeDelta);
}

#include "FXMgr.h"
void CMonsterNormalBullet::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (m_pMissile)
	{
		static_cast<CMonsterMissile*>(m_pMissile)->SetDead();
		m_pMissile = nullptr;
	}
	m_bDead = true; // ���� �Ƶ� �������°� �´� �� ����
	//if (CI_PLAYER(OtherColliderID))
	//{
	//	m_bDead = true;
	//}
}

HRESULT CMonsterNormalBullet::Render()
{
	if (m_bAnimation) // flesh
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		__super::Render();
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	else
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
			return E_FAIL;
		//m_pGraphic_Device->SetTexture(0, nullptr);

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard());
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	return S_OK;
}

void CMonsterNormalBullet::SetTargetDir()
{
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	//m_vTargetPos.y = 30.f;
	_float3 vDir = vMyPos - m_vTargetPos;
	vDir.Normalize();
	m_vTargetPos -= vDir * 400.f;

	// �ణ�� ���� ������ �߰�

	if (m_bGravity)
	{
		m_vTargetPos.y += 50.f;
		m_pTransformCom->LookAt(m_vTargetPos);
	}
	else
	{
		m_vTargetPos.x += GetRandomFloat(0.f, 25.f);
		m_vTargetPos.y += GetRandomFloat(0.f, 25.f);
		m_vTargetPos.z += GetRandomFloat(0.f, 25.f);
	}
}

CMonsterNormalBullet* CMonsterNormalBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonsterNormalBullet* pInstance = new CMonsterNormalBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonsterNormalBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonsterNormalBullet::Clone(void* pArg)
{
	CMonsterNormalBullet* pInstance = new CMonsterNormalBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMonsterNormalBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterNormalBullet::Free()
{
	__super::Free();

	Safe_Release(m_pTargetPlayer);
	Safe_Release(m_pCamera);
	Safe_Release(m_pGravityCom);
}
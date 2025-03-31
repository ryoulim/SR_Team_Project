#include "MonsterGuidBullet.h"
#include "MonsterMissile.h"
#include "Particle_Define.h"
#include "CameraManager.h"


CMonsterGuidBullet::CMonsterGuidBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CMonsterGuidBullet::CMonsterGuidBullet(const CMonsterGuidBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CMonsterGuidBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonsterGuidBullet::Initialize(void* pArg)
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

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	/* [ ���� ���� ] */
	m_vInitDir.Normalize();
	m_vCurrentDir = m_vInitDir;

	//�̻��� ��ƼŬ
	CPSystem::DESC Missile_iDesc{};
	Missile_iDesc.vPosition = { 0.f, 0.f, 0.f };
	Missile_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	Missile_iDesc.iParticleNums = 1000;
	Missile_iDesc.fSize = 2.f;
	Missile_iDesc.fMaxFrame = 20.f;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterMissile"),
		LEVEL_GAMEPLAY, L"Layer_Monster", ppOut, &Missile_iDesc)))
		return E_FAIL;

	m_pMissile = *ppOut;
	return S_OK;
}

HRESULT CMonsterGuidBullet::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
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
		//�� ������ �� ������ ���⼭ ��
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_vInitDir = pDesc->vDir;
	}

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


	return S_OK;
}

void CMonsterGuidBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMonsterGuidBullet::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	/* [ Ÿ�� ���� ] */
	SetTargetDir();

	//������� ��ƼŬ
	if (m_pMissile)
		static_cast<CMonsterMissile*>(m_pMissile)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	// �÷��̾���� ���ϴ� ���� ����
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToTarget = m_vTargetPos - vMyPos;
	vToTarget.Normalize();

	// �� ���� ���� ���͸� Ÿ�� �������� ���� �� ���� ���̴� ����
	
	// �ð� ����
	m_fPreChaseTime += fTimeDelta;
	// 0 ~ maxTurnSpeed ���̷� ���� Ŀ���� ȸ�� �ӵ�
	float fTurnSpeed = EaseInCubic(m_fPreChaseTime, 0.8f, 10.f);
	m_vCurrentDir = LerpVec(m_vCurrentDir, vToTarget, fTimeDelta * fTurnSpeed);
	m_vCurrentDir.Normalize();

	// �̵�
	vMyPos += m_vCurrentDir * 300.f * fTimeDelta;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vMyPos);

	return __super::Update(fTimeDelta);
}

void CMonsterGuidBullet::Late_Update(_float fTimeDelta)
{
	//�����׷� ������Ʈ
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

#include "FXMgr.h"
void CMonsterGuidBullet::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (m_pMissile)
	{
		static_cast<CMonsterMissile*>(m_pMissile)->SetDead();
		m_pMissile = nullptr;
	}

	m_bDead = TRUE;

	if (OtherColliderID == CI_BLOCK_COMMON)
	{
		FX_MGR->SpawnExplosion2(CCollider::Get_Last_Collision_Pos(), m_eLevelID);
		m_pCamera->Shake_Camera();
	}

	if (CI_WEAPON(OtherColliderID))
		FX_MGR->SpawnExplosion2(CCollider::Get_Last_Collision_Pos(), m_eLevelID);

}

HRESULT CMonsterGuidBullet::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;
	//m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard());
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

void CMonsterGuidBullet::SetTargetDir()
{
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	// �ణ�� ���� ������ �߰�
	m_vTargetPos.x += GetRandomFloat(-50.f, 50.f);
	m_vTargetPos.z += GetRandomFloat(-50.f, 50.f);
}

float CMonsterGuidBullet::EaseInCubic(float currentTime, float duration, float maxValue)
{
	float t = currentTime / duration;
	t = clamp(t, 0.f, 1.f); // ������ Ȯ��
	return t * t * t * maxValue;
}

CMonsterGuidBullet* CMonsterGuidBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonsterGuidBullet* pInstance = new CMonsterGuidBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonsterGuidBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonsterGuidBullet::Clone(void* pArg)
{
	CMonsterGuidBullet* pInstance = new CMonsterGuidBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMonsterGuidBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterGuidBullet::Free()
{
	__super::Free();

	Safe_Release(m_pTargetPlayer);
	Safe_Release(m_pCamera);
}
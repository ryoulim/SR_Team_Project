#include "RaceBoss.h"

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

	m_vScale = _float3(100.f, 100.f, 100.f);
	m_iHp = 100;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 250.f, 0.f));
	//450,250,1000
	m_pTransformCom->Scaling(m_vScale);
	m_eState = ENTRANCE;

	return S_OK;
}

void CRaceBoss::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceBoss::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	Action(fTimeDelta);

	return EVN_NONE;
}

void CRaceBoss::Late_Update(_float fTimeDelta)
{
	m_pCollider->Update_Collider();

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z > 9500.f)
		m_eState = LEAVE;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
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

	m_fTextureNum = 1.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::MUZZLE)))
		return E_FAIL;

	m_fTextureNum = 2.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceBoss::MIDDLE)))
		return E_FAIL;

	return S_OK;
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

	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = m_vScale;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MONSTER; //�ӽ÷� ����
	ColliderDesc.iColliderID = CI_MON_BODY;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CRaceBoss::Action(_float fTimeDelta)
{
	switch (m_eState)
	{
	case ENTRANCE:
		m_fTime += fTimeDelta * 0.98f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Lerp(m_vLerpStartPos, m_vLerpEndPos, m_fTime));

		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z > 1200.f)
		{
			m_eState = SHOTREADY;
			m_fTime = 0.f;
		}
		break;

	case SHOTREADY:
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_fTime += fTimeDelta;
		if (m_fTime > 1.f)
		{
			m_eState = SHOTHEADBULLET; //���� �ֱ⸶�� �� ���� �߻�
			m_fTime = 0.f;
		}
		break;

	case SHOTHEADBULLET:
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_fTime += fTimeDelta;
		if (m_fTime > 0.02f)
		{
			if (m_ePos == POSEND)
				m_ePos = static_cast<MUZZLEPOS>(rand() % 4);

			Fire_Bullet(CRaceBossBullet::HEAD, m_ePos);
			m_eState = SHOTTAILBULLET;
			m_fTime = 0.f;
		}
		break;

	case SHOTTAILBULLET:
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_fTime += fTimeDelta;
		if (m_fTime > 0.02f)
		{
			Fire_Bullet(CRaceBossBullet::TAIL, m_ePos);
			++m_iBulletCount;
			m_fTime = 0.f;
		}

		if (m_iBulletCount > 3)
		{
			m_eState = SHOTREADY;
			m_iBulletCount = 0;
			m_ePos = POSEND;
		}
		break;

	case LEAVE:
		m_pTransformCom->Go_Up(fTimeDelta);
		break;

	default:
		break;
	}
}

HRESULT CRaceBoss::Fire_Bullet(CRaceBossBullet::RBULLETTYPE eType, MUZZLEPOS ePos)
{
	_float3 vAdjustPos = {};

	switch (ePos)
	{
	case LSIDE:
		vAdjustPos = { -m_vScale.x * 1.625f, m_vScale.y * 0.25f, -m_vScale.z * 2.f };
		break;

	case LMIDDLE:
		vAdjustPos = { -m_vScale.x * 0.875f, m_vScale.y * 0.f, -m_vScale.z * 2.f };
		break;

	case RMIDDLE:
		vAdjustPos = { m_vScale.x * 0.875f, m_vScale.y * 0.f, -m_vScale.z * 2.f };
		break;

	case RSIDE:
		vAdjustPos = { m_vScale.x * 1.625f, m_vScale.y * 0.25f, -m_vScale.z * 2.f };
		break;

	default:
		return E_FAIL;
		break;
	}

	CRaceBossBullet::DESC RaceBossBulletdesc{};
	RaceBossBulletdesc.bAnimation = false;
	RaceBossBulletdesc.iColliderID = CI_BOSS_GUIDBULLET;
	RaceBossBulletdesc.fSpeedPerSec = 300.f;
	RaceBossBulletdesc.fRotationPerSec = RADIAN(180.f);
	RaceBossBulletdesc.vScale = { 20.f, 20.f, 20.f };
	RaceBossBulletdesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION) + vAdjustPos;

	if (eType == CRaceBossBullet::HEAD)
	{
		//HEAD �Ѿ��� �÷��̾ ����
		auto pPlayer = GET_PLAYER;
		RaceBossBulletdesc.vLook = *GET_PLAYER_TRANSFORM->Get_State(CTransform::STATE_POSITION)
			+ _float3(0.f, 0.f, 600.f);
		m_vBulletDiretion = RaceBossBulletdesc.vLook;
	}

	if (eType == CRaceBossBullet::TAIL)
	{
		//TAIL �Ѿ��� HEAD �Ѿ��� ����
		RaceBossBulletdesc.vLook = m_vBulletDiretion;
	}
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBossBullet"),
		m_eLevelID, L"Layer_RaceBossBullet", &RaceBossBulletdesc)))
		return E_FAIL;

	return S_OK;
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
	Safe_Release(m_pCollider);
}

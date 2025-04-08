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
	m_pPlayer = GET_PLAYER;

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

#ifdef _CONSOL
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	printf("보스 위치 : { %f, %f, %f }\n", vPos.x, vPos.y, vPos.z);
	printf("보스 상태 : %d\n", m_eState);
#endif

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
	ColliderDesc.iColliderGroupID = CG_MONSTER; //임시로 몬스터
	ColliderDesc.iColliderID = CI_MON_BODY;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CRaceBoss::Action(_float fTimeDelta)
{
	_float3 v0 = { 0.f, 1750.f, -1300.f };
	_float3 vStartPos = { 0.f, 1000.f, -500.f };
	_float3 vEndPos = { 450.f, 250.f, 1300.f };
	_float3 v3 = { 450.f, 2000.f, 2000.f };

	switch (m_eState)
	{
	case WAITFORPLAYER:
		m_vPlayerPos = *static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
		if (m_vPlayerPos.z > 0.f)
			m_eState = ENTRANCE;
		break;
		
	case ENTRANCE:
		m_fTime += (1.f - m_fTime) * fTimeDelta;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, CatmulRomPos(v0, vStartPos, vEndPos, v3, m_fTime));

		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z >= 1200.f)
		{
			m_eState = SHOTREADY;
			m_fTime = 0.f;
		}
		break;

	case SHOTREADY:
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_fTime += fTimeDelta;
		if (m_fTime > 3.f)
		{
			m_eState = SHOTHEADBULLET; //일정 주기마다 한 번씩 발사
			m_fTime = 0.f;
		}
		break;

	case SHOTHEADBULLET:
		m_pTransformCom->Go_Straight(fTimeDelta);
		ShuffleandPop();
		Fire_Bullet(CRaceBossBullet::HEAD, m_ePos);
		++m_iHeadBulletCount;
		m_eState = SHOTTAILBULLET;
		break;

	case SHOTTAILBULLET:
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_fTime += fTimeDelta;
		if (m_fTime > 0.02f)
		{
			Fire_Bullet(CRaceBossBullet::TAIL, m_ePos);
			++m_iTailBulletCount;
			m_fTime = 0.f;
		}

		if (m_iTailBulletCount > 3)
		{
			m_iTailBulletCount = 0;

			if (m_iHeadBulletCount > 4)
			{
				m_iHeadBulletCount = 0;
				m_eState = SHOTREADY;
			}
				
			else
				m_eState = SHOTHEADBULLET;
				
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

	case MIDDLE:
		vAdjustPos = { 0.f, m_vScale.y * 0.25f, -m_vScale.z * 2.f };
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
		//HEAD 총알은 플레이어를 향함
		RaceBossBulletdesc.vLook = *static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)
			+ _float3(0.f, 0.f, 650.f);
		m_vBulletDiretion = RaceBossBulletdesc.vLook;
	}

	if (eType == CRaceBossBullet::TAIL)
	{
		//TAIL 총알은 HEAD 총알을 향함
		RaceBossBulletdesc.vLook = m_vBulletDiretion;
	}
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBossBullet"),
		m_eLevelID, L"Layer_RaceBossBullet", &RaceBossBulletdesc)))
		return E_FAIL;

	return S_OK;
}

_float3 CRaceBoss::CatmulRomPos(_float3& v0, _float3& vStartPos, _float3& vEndPos, _float3& v3, _float fTimeAcc)
{
	_uint iTimeAcc = static_cast<_uint>(fTimeAcc);
	_float t = fTimeAcc - iTimeAcc;

	_float3 vResult = {};
	D3DXVec3CatmullRom(&vResult, &v0, &vStartPos, &vEndPos, &v3, t);

	return vResult;
}

void CRaceBoss::ShuffleandPop()
{
	if (m_VecBulletPos.empty())
	{
		m_VecBulletPos = { LSIDE, LMIDDLE, MIDDLE, RMIDDLE, RSIDE };
		random_shuffle(m_VecBulletPos.begin(), m_VecBulletPos.end());
	}

	if (!m_VecBulletPos.empty())
	{
		m_ePos = m_VecBulletPos.back();
		m_VecBulletPos.pop_back();
	}
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

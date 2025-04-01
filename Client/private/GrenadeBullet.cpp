// 내 클래스 이름 : GrenadeBullet
// 부모 클래스 이름 : Bullet

#include "GrenadeBullet.h"
#include "FXMgr.h"

CGrenadeBullet::CGrenadeBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CGrenadeBullet::CGrenadeBullet(const CGrenadeBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CGrenadeBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrenadeBullet::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = pDesc->szTextureTag;
	m_szBufferType = TEXT("Rect");
	m_bAnimation = pDesc->bAnimation;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//m_pGravityCom->Jump(30.f);

	m_fSpeedperSec = pDesc->fSpeedPerSec;
	m_fTimeLimit = pDesc->fTimeLimit;
	m_fJumpPower = pDesc->fInitJumpPower;

	return S_OK;
}

void CGrenadeBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CGrenadeBullet::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > m_fTimeLimit ||
		m_bDead)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(CCollider::Get_Last_Collision_Pos(), m_eLevelID, { 30.f,50.f,1.f }, TEXT("Effect_Explor"), 32);
		return EVN_DEAD;
	}

	m_pTransformCom->Go_Straight(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CGrenadeBullet::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
	m_pGravityCom->Update(fTimeDelta);
	if (!m_pGravityCom->isJump())
	{
		Friction();
		m_pGravityCom->Jump(m_fJumpPower);
	}

	if(m_bAnimation)
		FrameUpdate(fTimeDelta, 4.f, 15.f, true);

	__super::Late_Update(fTimeDelta);
}

HRESULT CGrenadeBullet::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	__super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	return S_OK;
}

void CGrenadeBullet::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (OtherColliderID == CI_MON_HEAD || 
		OtherColliderID == CI_MON_BODY ||
		CI_WEAPON(OtherColliderID))
	{
		m_bDead = TRUE;
	}
	else if (OtherColliderID == CI_BLOCK_COMMON && CCollider::Get_Last_Collision_Depth().y == 0)
	{
		Friction();

		m_pTransformCom->Move(CCollider::Get_Last_Collision_Depth());

		_float3 vScale = m_pTransformCom->Compute_Scaled();

		_float3 vIncident = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_float3 vNormal = CCollider::Get_Last_Collision_Depth();

		vIncident.Normalize();
		vNormal.Normalize();

		// 반사 벡터 계산
		_float vDot = vIncident.Dot(vNormal);
		_float3 vReflected = vIncident - 2.f * vDot * vNormal;

		_float3 vRight = _float3(0.f, 1.f, 0.f).Cross(vReflected).Normalize();

		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vReflected * vScale.z);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	}
}

HRESULT CGrenadeBullet::Ready_Components(void* pArg)
{
	//__super::Ready_Components(pArg);

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
		DESC* pDesc = static_cast<DESC*>(pArg);

		pDesc->vLook.y = max(pDesc->vLook.y, 0.f);
		_float3 vRight = _float3(0.f, 1.f, 0.f).Cross(pDesc->vLook); 

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransformCom->Set_State(CTransform::STATE_UP, pDesc->vLook.Cross(vRight));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, pDesc->vLook);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(pDesc->vScale);

		CCollider_Capsule::DESC ColliderDesc{};
		ColliderDesc.pTransform = m_pTransformCom;
		ColliderDesc.vScale = pDesc->vScale;
		ColliderDesc.pOwner = this;
		ColliderDesc.iColliderGroupID = pDesc->ColliderGroup;
		ColliderDesc.iColliderID = CI_DISPENSOR_GRENADE;

		/* For.Com_Collider */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
			return E_FAIL;

		/* For.Com_Gravity */
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

void CGrenadeBullet::Friction()
{
	m_fSpeedperSec *= 0.9f;
	m_fJumpPower *= 0.9f;
	m_pTransformCom->Set_SpeedPerSec(m_fSpeedperSec);
	if (m_fJumpPower < 1.f)
		m_bDead = true;
}

CGrenadeBullet* CGrenadeBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGrenadeBullet* pInstance = new CGrenadeBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGrenadeBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrenadeBullet::Clone(void* pArg)
{
	CGrenadeBullet* pInstance = new CGrenadeBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGrenadeBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrenadeBullet::Free()
{
	__super::Free();
	Safe_Release(m_pGravityCom);
}
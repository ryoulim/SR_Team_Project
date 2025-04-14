#include "Bullet.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBullet::CBullet(const CBullet& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBullet::FrameUpdate(_float timeDelta, _float _MaxFrame, _float fSpeed, _bool isLoop)
{
	if (isLoop)
	{
		//현재 프레임이 맥스프레임보다 크면 ? 0 으로 바꿔라.
		if (_MaxFrame <= m_fAnimationFrame)
		{
			m_fAnimationFrame -= _MaxFrame;
		}
	}
	else
	{
		//반복이 아니면 그냥 리턴 
		if (_MaxFrame <= m_fAnimationFrame)
			return;
	}
	m_fAnimationFrame += fSpeed * timeDelta;

}

HRESULT CBullet::Reset(void* pArg)
{
	_float4x4 CameraWorld{};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorld);
	m_pTransformCom->Set_WorldMatrix(&CameraWorld.MakeInverseMat(CameraWorld)); 

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_pCollider->Update_Scale({ pDesc->fSpeedPerSec / 60.f ,0.f,0.f });
	m_pCollider->Update_Collider();

	m_pGameInstance->Add_Collider(m_pCollider, CG_PBULLET);
	m_bDead = FALSE;
	m_fTimeAcc = 0.f;

	return S_OK;
}

void CBullet::Priority_Update(_float fTimeDelta)
{
}

EVENT CBullet::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CBullet::Late_Update(_float fTimeDelta)
{
	if (m_pCollider)
		m_pCollider->Update_Collider();
}

HRESULT CBullet::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard_Y());

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

#include "FXMgr.h"
void CBullet::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	m_bDead = TRUE;

	if(OtherColliderID == CI_MON_BODY)
		FX_MGR->SpawnExplosion(CCollider::Get_Last_Collision_Pos(), m_eLevelID);
}

HRESULT CBullet::Ready_Components(void* pArg)
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
		//_float4x4 mat{};
		//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &mat);
		//m_pTransformCom->Set_WorldMatrix(&mat.MakeInverseMat(mat));
		m_pTransformCom->Scaling(pDesc->vScale);
	}

	return S_OK;
}
void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
}
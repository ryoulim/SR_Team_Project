#include "Map.h"
#include "FXMgr.h"

CMap::CMap(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{pGraphic_Device}
{
}

CMap::CMap(const CMap& Prototype)
	:CGameObject(Prototype)
{
}

CMap::~CMap()
{
}

HRESULT CMap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMap::Initialize(void* pArg)
{
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMap::Priority_Update(_float fTimeDelta)
{
}

EVENT CMap::Update(_float fTimeDelta)
{
    return EVN_NONE;
}

void CMap::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CMap::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureIdx))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

#ifdef _COLLIDERRENDER
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void CMap::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{

	if (OtherColliderID == CI_LOVERBOY ||
		OtherColliderID == CI_CHAINGUN ||
		OtherColliderID == CI_DISPENSOR_SHELL)
	{
		/* [ 충돌한 면의 노말 벡터 ] */
		_float3 vLook = CCollider::Get_Last_Collision_Depth().Normalize();


		/* 콘크리트 총알 자국_0 */
		if (m_fTextureIdx == 98 || m_fTextureIdx == 62)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 0);
		}
		/* 유리 총알 자국 */
		else if (m_fTextureIdx == 116)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 1);
		}
		/* 콘크리트 총알 자국_1 */
		else if (m_fTextureIdx == 93 || m_fTextureIdx == 99 || m_fTextureIdx == 127)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 2);
		}
		/* 콘크리트 총알 자국_2 */
		else if (m_fTextureIdx == 108 || m_fTextureIdx == 109)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 3);
		}
		/* 나무 총알 자국 */
		else if (m_fTextureIdx == 32 || m_fTextureIdx == 128 || m_fTextureIdx == 130 || m_fTextureIdx == 132 || m_fTextureIdx == 133)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 4);
		}
		/* 콘크리트 총알 자국_3 */
		else if (m_fTextureIdx == 0 || m_fTextureIdx == 18 || m_fTextureIdx == 118)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 5);
		}
		/* 금속 총알 자국 */
		else if (m_fTextureIdx == 20 || m_fTextureIdx == 137 || m_fTextureIdx == 2 || m_fTextureIdx == 23 || m_fTextureIdx == 25 ||
			m_fTextureIdx == 110 || m_fTextureIdx == 114 || m_fTextureIdx == 115 || m_fTextureIdx == 120)
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 6);
		}
		else
		{
			FX_MGR->SpawnBulletMark(CCollider::Get_Last_Collision_Pos(), m_eLevelID, vLook, 0);
		}

	}







}

HRESULT CMap::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (TEXT("Terrain") == m_szBufferType || TEXT("RaceTerrain") == m_szBufferType)
	{
		if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;
	}

	else
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
		m_pTransformCom->QurternionRotation(pDesc->vAngle);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (pDesc->vInitPos));

		m_fTextureIdx = pDesc->fTextureIdx;
	}

	return S_OK;
}

void CMap::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
}

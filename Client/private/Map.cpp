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

	static bool bRenderMode{ FALSE };
	if (KEY_DOWN(DIK_F9))
		bRenderMode = !bRenderMode;
	if (m_pColliderCom && bRenderMode)
		m_pColliderCom->Render();
#endif

	return S_OK;
}

void CMap::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{

	if (OtherColliderID == CI_LOVERBOY ||
		OtherColliderID == CI_CHAINGUN ||
		OtherColliderID == CI_DISPENSOR_SHELL)
		FX_MGR->SpawnBlood(CCollider::Get_Last_Collision_Pos(), m_eLevelID);


	/*if (m_fTextureIdx == 1010)
		FX_MGR->SpawnBlood();
	if (m_fTextureIdx == 1001)
		FX_MGR->SpawnBulletTracer();
	if (m_fTextureIdx == 1001)
		FX_MGR->SpawnBlood();*/
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

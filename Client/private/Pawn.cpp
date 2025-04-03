#include "Pawn.h"

CPawn::CPawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPawn::CPawn(const CPawn& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CPawn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPawn::Initialize(void* pArg)
{
	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPawn::Priority_Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_P))
		Change_Level();
}

EVENT CPawn::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CPawn::Late_Update(_float fTimeDelta)
{
}

HRESULT CPawn::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPawn::Ready_Components(void* pArg)
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
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->Scaling(pDesc->vScale);
	}


	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = pDesc->vScale;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_PAWN;
	ColliderDesc.iColliderID = CI_PLAYER;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}
#include "Level_Loading.h"

void CPawn::Change_Level()
{
	LEVEL eNextLevelID{};

	switch (m_eLevelID)
	{
	case LEVEL_INDOOR:
		eNextLevelID = LEVEL_OUTDOOR;
		break;
	case LEVEL_OUTDOOR:
		eNextLevelID = LEVEL_RACEFIRST;
		break;
	case LEVEL_RACEFIRST:
		eNextLevelID = LEVEL_RACESECOND;
		break;
	case LEVEL_RACESECOND:
		eNextLevelID = LEVEL_RACETHIRD;
		break;
	case LEVEL_RACETHIRD:
		eNextLevelID = LEVEL_GAMEPLAY;
		break;
	default:
		break;
	}

	m_pGameInstance->Change_Level(eNextLevelID);
}

void CPawn::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
}
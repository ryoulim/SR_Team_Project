#include "Weapon.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CWeapon::CWeapon(const CWeapon& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWeapon::Priority_Update(_float fTimeDelta)
{
}

EVENT CWeapon::Update(_float fTimeDelta)
{
	Action(fTimeDelta);
	Update_Frame(fTimeDelta);
	_float3 vScale{};
	m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &vScale);
	m_pTransformCom->Scaling(vScale * 1.5f);

	return EVN_NONE;
}

void CWeapon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CWeapon::Render()
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

void CWeapon::Action(_float fTimeDelta)
{
	switch (m_eState)
	{
	case ST_IDLE:
		Idle();
		break;
	case ST_OPENING:
		Opening();
		break;
	case ST_W_ATK:
		Weak_Attack();
		break;
	case ST_S_ATK:
		Strong_Attack();
		break;
	case ST_RELOAD:
		Reload();
		break;
	default:
		break;
	}
}

void CWeapon::Idle()
{
}

HRESULT CWeapon::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_Rect")),
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
	}

	return S_OK;
}

void CWeapon::Update_Frame(_float fTimeDelta)
{
	m_fTextureNum += m_fFrameSpeed * fTimeDelta;
	if (m_fEndFrame < m_fTextureNum)
		Set_State(ST_IDLE);
}

void CWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
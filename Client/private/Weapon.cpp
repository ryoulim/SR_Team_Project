#include "Weapon.h"
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

	Set_State(ST_OPENING);

	return S_OK;
}

void CWeapon::Priority_Update(_float fTimeDelta)
{
}

EVENT CWeapon::Update(_float fTimeDelta)
{
 	Action(fTimeDelta);

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

void CWeapon::Walk(_float fTimeDelta)
{
	if (m_eState == ST_IDLE)
		Set_State(ST_WALK);
	else if (m_eState != ST_WALK)
		return;

	m_fWalkTimer += fTimeDelta;
#define WALKX 20.f
#define WALKY 10.f
#define FREQUENCY 8.f

	_float3 vOffset = _float3(
		sinf(m_fWalkTimer * FREQUENCY) * WALKX,
		cosf(m_fWalkTimer * FREQUENCY*2) * WALKY,
		0.f
	);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vMovingPos + vOffset);
}

void CWeapon::Action(_float fTimeDelta)
{
	m_fMotionTimer += fTimeDelta;

	switch (m_eState)
	{
	case ST_IDLE:
		Idle();
		break;
	case ST_OPENING:
		Opening(fTimeDelta);
		break;
	//case ST_WALK :
	//	Walk(fTimeDelta);
	//	break;
	case ST_W_ATK:
		Weak_Attack(fTimeDelta);
		break;
	case ST_S_ATK:
		Strong_Attack(fTimeDelta);
		break;
	case ST_RELOAD:
		Reload(fTimeDelta);
		break;
	case ST_ENDING:
		Ending(fTimeDelta);
		break;
	default:
		break;
	}
}

void CWeapon::Idle()
{
}

#include "TestBullet.h"
void CWeapon::Create_Bullet()
{
	_float4x4 matCamWorld; 
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld); matCamWorld.MakeInverseMat(matCamWorld);
	_float3 pPos = *reinterpret_cast<_float3*>(&matCamWorld.m[3][0]);
	_float3 pLook = *reinterpret_cast<_float3*>(&matCamWorld.m[2][0]);
	_uint iColliderID{};

	auto pPickedObj = m_pGameInstance->Raycast(pPos, pLook.Normalize(), m_fRayLength, {COL_BLOCK,COL_MONSTER}, iColliderID);
	if (pPickedObj)
	{
		pPickedObj->On_Collision(iColliderID, m_tAmmoInfo.eType);
	}
}
//CTestBullet::DESC BulletDesc{};
//BulletDesc.fSpeedPerSec = 3000.f;
//BulletDesc.vScale = { 3.f,3.f,3.f };

//if (FAILED(m_pGameInstance->Active_Object(TEXT("ObjectPool_TestBullet"),
//	LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), &BulletDesc)))
//	return;

//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TestBullet"),
//	LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), &BulletDesc)))
//	return;

void CWeapon::Key_Input()
{
	if (m_eState >= ST_OPENING)
		return;

	if (MOUSE_DOWN(DIMK_LBUTTON))
	{
		if (m_tAmmoInfo.iReloadedAmmo)
		{
			Set_State(CWeapon::ST_W_ATK);
			Create_Bullet();
			m_tAmmoInfo.iReloadedAmmo--;
			m_tAmmoInfo.iCurAmmo--;
		}
		else
			Set_State(CWeapon::ST_RELOAD);
	}
	if (MOUSE_DOWN(DIMK_RBUTTON))
	{
		Set_State(CWeapon::ST_S_ATK);
	}
	if (KEY_DOWN(DIK_R))
	{
		Set_State(CWeapon::ST_RELOAD);
	}
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

_bool CWeapon::Update_Frame(_float fTimeDelta)
{
	m_fTextureNum += m_fFrameSpeed * fTimeDelta;
	if (m_fEndFrame+1 < m_fTextureNum)
	{
		m_fTextureNum = m_fStartFrmae;
		return TRUE;
	}
	return FALSE;
}

void CWeapon::Setup_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
}

void CWeapon::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
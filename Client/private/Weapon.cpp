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

	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pPlayerTransform = pDesc->pPlayerTransform;
	Safe_AddRef(m_pPlayerTransform);

	m_pCameraManager = CAMERA_MANAGER;
	Safe_AddRef(m_pCameraManager);
	m_pCameraTransform = static_cast<CTransform*>(m_pCameraManager->Get_Camera(CCameraManager::FPS)->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pCameraTransform);

	m_fDepth = 6.f;
	Set_State(ST_OPENING);

	m_eLevelID = pDesc->eLevelID;

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
	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", static_cast<_uint>(m_fTextureNum));
	m_pShaderCom->SetFloat("opacity", m_fAlpha);
	m_pShaderCom->Begin(CShader::ALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
	//	return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

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
	//m_pCameraManager->StartRecoil(10.f, 0.5f);
	//m_pCameraManager->Shake_Camera(0.1f, 0.2f);


	// 여기 카메라의 흔들리메 영향받지 않도록 오프셋과 분리작업 필요
	_float3 pPos = *m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
	_float3 pLook = *m_pCameraTransform->Get_State(CTransform::STATE_LOOK);
	_uint iColliderID{};

	auto pPickedObj = m_pGameInstance->Raycast(pPos, pLook.Normalize(), m_fRayLength, {CG_BLOCK,CG_MONSTER,CG_MBULLET}, iColliderID);
	if (pPickedObj)
	{
		pPickedObj->On_Collision(iColliderID, m_tAmmoInfo.eType);
	}
}

void CWeapon::Key_Input()
{
	if (m_eState >= ST_OPENING)
		return;

	if (MOUSE_DOWN(DIMK_LBUTTON) && m_tAmmoInfo.iCurAmmo > 0)
	{
		if (m_tAmmoInfo.iReloadedAmmo)
		{
			//m_pTestSound->Set3DAttributes(*m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
			//m_pTestSound->SetVolume(0.7f);
			//m_pTestSound->Play();
			//m_pCoreSoundTest->Update3DPosition();
			//m_pCoreSoundTest->Play(0.7f);
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
	if (KEY_DOWN(DIK_R) && m_tAmmoInfo.iCurAmmo > 0)
	{
		Set_State(CWeapon::ST_RELOAD);
	}
}

HRESULT CWeapon::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
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

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
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
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTestSound);
	Safe_Release(m_pCoreSoundTest);

	Safe_Release(m_pPlayerTransform);
	Safe_Release(m_pCameraManager);
	Safe_Release(m_pCameraTransform);
}
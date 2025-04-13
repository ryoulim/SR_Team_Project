#include "BombRadius.h"
#include "GameInstance.h"
#include "RaceBossBomb.h"
#include "FXMgr.h"

CBombRadius::CBombRadius(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CBombRadius::CBombRadius(const CBombRadius& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CBombRadius::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBombRadius::Initialize(void* pArg)
{
	m_szTextureID = TEXT("BombRadius");
	m_szBufferType = TEXT("Rect");

	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;

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

	//셰이더 장착
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vInitPos);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_pTransformCom->QurternionRotation(pDesc->vAngle);
		m_eShadingLevel = pDesc->eLevelID;
	}

	m_pPlayer = GET_PLAYER;
	Safe_AddRef(m_pPlayer);

	m_fTextureNum = 0.f;

	return S_OK;
}

void CBombRadius::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBombRadius::Update(_float fTimeDelta)
{	
	if(m_bDead)
		return EVN_DEAD;

	/* [ 프레임 업데이트 ] */
	m_fTextureNum += 15.f * fTimeDelta;
	if (14.f <= m_fTextureNum)
	{
		m_fTextureNum = 14.f;
	}

	/* 미사일도 생성 */
	if (m_fTextureNum >= 10.f && !m_bDoOnce)
	{
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.y += 120.f;
		SpawnMissile(vPos);
		m_bDoOnce = true;
	}

	/* [ 만약 미사일이 바닥에 닿았으면 미사일을 제거한다 ] */
	if (m_pMissile)
	{
		if (static_cast<CRaceBossBomb*>(m_pMissile)->GetbDie())
		{
			_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPos.y += 30.f;
			FX_MGR->SpawnCustomExplosion(vPos, m_eLevelID, {150.f, 165.f, 1.f}, L"Effect_Explor", 32);
			static_cast<CRaceBossBomb*>(m_pMissile)->SetbDead(true);
			m_pMissile = nullptr;
			m_bDead = true;
		}
	}

	return __super::Update(fTimeDelta);
}

void CBombRadius::Late_Update(_float fTimeDelta)
{
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CBombRadius::SetUp_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}


HRESULT CBombRadius::Render()
{
	SetUp_RenderState();
	return __super::Render();
	Release_RenderState();
}

HRESULT CBombRadius::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	return S_OK;
}

void CBombRadius::SpawnMissile(_float3 _Position)
{
	CRaceBossBomb::DESC RaceBossBombdesc{};
	RaceBossBombdesc.bAnimation = false;
	RaceBossBombdesc.iColliderID = CI_BOSS_FIRE;
	RaceBossBombdesc.fSpeedPerSec = 300.f;
	RaceBossBombdesc.fRotationPerSec = RADIAN(180.f);
	RaceBossBombdesc.vScale = { 20.f, 45.f, 1.f };
	RaceBossBombdesc.vPosition = _Position;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBossBomb"),
		m_eLevelID, L"Layer_RaceBossBullet", ppOut, &RaceBossBombdesc)))
		return;

	m_pMissile = *ppOut;
}

CBombRadius* CBombRadius::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBombRadius* pInstance = new CBombRadius(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBombRadius");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBombRadius::Clone(void* pArg)
{
	CBombRadius* pInstance = new CBombRadius(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBombRadius");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBombRadius::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
}

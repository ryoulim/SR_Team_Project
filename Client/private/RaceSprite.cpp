#include "RaceSprite.h"
#include "GameInstance.h"
#include "Particle_Define.h"
#include "RaceBoss.h"

CRaceSprite::CRaceSprite(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CRaceSprite::CRaceSprite(const CRaceSprite& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CRaceSprite::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceSprite::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CRaceSprite::DESC* SpriteDESC = static_cast<DESC*>(pArg);

	m_eLevelID = LEVEL_STATIC;
	m_fAnimationMaxFrame = SpriteDESC->fMaxFrame;
	m_szTextureID = SpriteDESC->szTextureTag;
	m_bLoop = SpriteDESC->bLoop;
	m_szBufferType = TEXT("Rect");
	m_fAnimationFrame = SpriteDESC->fStartFrame;
	m_vPosOffset = SpriteDESC->vPosOffset;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRaceSprite::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceSprite::Update(_float fTimeDelta)
{
	FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, 15.f, m_bLoop);

	/* [ 이 스프라이트는 레이스 보스를 따라갑니다 ] */
	auto RacingBoss = CGameInstance::Get_Instance()->Find_Object(m_eLevelID, L"Layer_RaceBoss", 0);
	_float3 vBossPos = *static_cast<CTransform*>(RacingBoss->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	/* [ 오프셋을 이용하여 위치를 맞춰줍니다 ] */
	vBossPos.x += m_vPosOffset.x;
	vBossPos.y += m_vPosOffset.y;
	vBossPos.z += m_vPosOffset.z;

	/* [ 위치를 따라갑니다 ] */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vBossPos);

	return __super::Update(fTimeDelta);
}

void CRaceSprite::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceSprite::Render()
{
	if (m_bLoop)
	{
		return __super::Render();
	}
	else
	{
		if (m_fAnimationFrame > m_fAnimationMaxFrame)
		{
			return EVN_NONE;
		}
		return __super::Render();
	}

	return EVN_NONE;
}

CRaceSprite* CRaceSprite::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceSprite* pInstance = new CRaceSprite(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceSprite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceSprite::Clone(void* pArg)
{
	CRaceSprite* pInstance = new CRaceSprite(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceSprite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceSprite::Free()
{
	__super::Free();

}
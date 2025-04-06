// 내 클래스 이름 : HydroPump
// 부모 클래스 이름 : Effect

#include "HydroPump.h"
#include "GameInstance.h"

CHydroPump::CHydroPump(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CHydroPump::CHydroPump(const CHydroPump& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CHydroPump::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHydroPump::Initialize(void* pArg)
{
	m_szTextureID = TEXT("HydroPump");
	m_szBufferType = TEXT("Rect");

	DESC* pDesc = static_cast<DESC*>(pArg);

	m_fAnimationFrame = 0.f;
	m_fAnimationMaxFrame = pDesc->fMaxFrame;
	m_fAnimationSpeed = pDesc->fAniSpeed;
	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CHydroPump::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CHydroPump::Update(_float fTimeDelta)
{

	FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, m_fAnimationSpeed, true);
	return __super::Update(fTimeDelta);
}

void CHydroPump::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CHydroPump::Render()
{
	return __super::Render();
}

CHydroPump* CHydroPump::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHydroPump* pInstance = new CHydroPump(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHydroPump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHydroPump::Clone(void* pArg)
{
	CHydroPump* pInstance = new CHydroPump(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CHydroPump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHydroPump::Free()
{
	__super::Free();
}
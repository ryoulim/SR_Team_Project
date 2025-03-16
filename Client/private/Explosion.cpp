// 내 클래스 이름 : Explosion
// 부모 클래스 이름 : Effect

#include "Explosion.h"
#include "GameInstance.h"

CExplosion::CExplosion(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CExplosion::CExplosion(const CExplosion& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CExplosion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExplosion::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("PC_Explosion");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CExplosion::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CExplosion::Update(_float fTimeDelta)
{
	if (m_fTextureNum > 14)
	{
		return EVN_NONE;
	}

	
	m_fTextureNum += 15.f * fTimeDelta;


	return __super::Update(fTimeDelta);
}

void CExplosion::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CExplosion::Render()
{
	if (m_fTextureNum > 14)
	{
		return EVN_NONE;
	}

	return __super::Render();
}

CExplosion* CExplosion::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExplosion* pInstance = new CExplosion(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CExplosion::Clone(void* pArg)
{
	CExplosion* pInstance = new CExplosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExplosion::Free()
{
	__super::Free();

}
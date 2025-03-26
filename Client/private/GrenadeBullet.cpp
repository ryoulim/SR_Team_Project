// 내 클래스 이름 : GrenadeBullet
// 부모 클래스 이름 : Bullet

#include "GrenadeBullet.h"

CGrenadeBullet::CGrenadeBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CGrenadeBullet::CGrenadeBullet(const CGrenadeBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CGrenadeBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrenadeBullet::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("GrenadeBullet");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGrenadeBullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CGrenadeBullet::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CGrenadeBullet::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CGrenadeBullet::Render()
{
	return __super::Render();
}

HRESULT CGrenadeBullet::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);


	return S_OK;
}

CGrenadeBullet* CGrenadeBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGrenadeBullet* pInstance = new CGrenadeBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGrenadeBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrenadeBullet::Clone(void* pArg)
{
	CGrenadeBullet* pInstance = new CGrenadeBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGrenadeBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrenadeBullet::Free()
{
	__super::Free();
}
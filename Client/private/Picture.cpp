// 내 클래스 이름 : Picture
// 부모 클래스 이름 : Interactive_Block

#include "Picture.h"

CPicture::CPicture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CPicture::CPicture(const CPicture& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CPicture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPicture::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPicture::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CPicture::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CPicture::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPicture::Render()
{
	return __super::Render();
}

void CPicture::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (CI_WEAPON(OtherColliderID))
	{
		if (!m_bBroken)
		{
			m_fTextureIdx++;
			m_bBroken = TRUE;
		}
	}
}

CPicture* CPicture::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPicture* pInstance = new CPicture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPicture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPicture::Clone(void* pArg)
{
	CPicture* pInstance = new CPicture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPicture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicture::Free()
{
	__super::Free();
}
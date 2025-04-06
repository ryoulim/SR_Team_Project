// 내 클래스 이름 : FirePlug
// 부모 클래스 이름 : Interactive_Block

#include "FirePlug.h"
#include "GameInstance.h"

CFirePlug::CFirePlug(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CFirePlug::CFirePlug(const CFirePlug& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CFirePlug::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFirePlug::Initialize(void* pArg)
{
	m_szTextureID = TEXT("FirePlug");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFirePlug::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFirePlug::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFirePlug::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFirePlug::Render()
{
	return __super::Render();
}

CFirePlug* CFirePlug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFirePlug* pInstance = new CFirePlug(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFirePlug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFirePlug::Clone(void* pArg)
{
	CFirePlug* pInstance = new CFirePlug(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFirePlug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFirePlug::Free()
{
	__super::Free();
}
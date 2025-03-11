// 내 클래스 이름 : BackGround
// 부모 클래스 이름 : Statue

#include "BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue{ pGraphic_Device }
{
}

CBackGround::CBackGround(const CBackGround& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_LOGO;
	m_szTextureID = TEXT("BackGround");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBackGround::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CBackGround::Render()
{
	return __super::Render();
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround* pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
}
// 내 클래스 이름 : TextTerrain
// 부모 클래스 이름 : Terrain

#include "TestTerrain.h"
#include "GameInstance.h"

CTestTerrain::CTestTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CTestTerrain::CTestTerrain(const CTestTerrain& Prototype)
	: CMap(Prototype)
{
}

HRESULT CTestTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTestTerrain::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_TEST;
	m_szTextureID = TEXT("Terrain");
	m_szBufferType = TEXT("Terrain");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTestTerrain::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTestTerrain::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CTestTerrain::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTestTerrain::Render()
{
	return __super::Render();
}

CTestTerrain* CTestTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTestTerrain* pInstance = new CTestTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTestTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTestTerrain::Clone(void* pArg)
{
	CTestTerrain* pInstance = new CTestTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTestTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestTerrain::Free()
{
	__super::Free();
}
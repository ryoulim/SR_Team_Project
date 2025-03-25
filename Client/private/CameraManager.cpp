// 내 클래스 이름 : CameraManager
// 부모 클래스 이름 : GameObject

#include "CameraManager.h"
#include "Camera.h"

CCameraManager::CCameraManager(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CCameraManager::CCameraManager(const CCameraManager& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CCameraManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCameraManager::Initialize(void* pArg)
{
	m_Cameras.push_back(static_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_FPS_Camera"), pArg)));
	m_Cameras.push_back(static_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Dynamic_Camera"), pArg)));
	
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_FPS_Camera"),
	//LEVEL_GAMEPLAY, TEXT("Layer_Camera"), pArg)))
	//return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Dynamic_Camera"),
	//LEVEL_GAMEPLAY, TEXT("Layer_Camera"), pArg)))
	//return E_FAIL;

	return S_OK;
}

void CCameraManager::Priority_Update(_float fTimeDelta)
{
	m_Cameras[m_eID]->Priority_Update(fTimeDelta);
}

EVENT CCameraManager::Update(_float fTimeDelta)
{
	return	m_Cameras[m_eID]->Update(fTimeDelta);
}

void CCameraManager::Late_Update(_float fTimeDelta)
{
	m_Cameras[m_eID]->Late_Update(fTimeDelta);
}

HRESULT CCameraManager::Render()
{
	return S_OK;
}

void CCameraManager::Switch(_bool isFPSMode)
{
	if (isFPSMode)
		m_eID = FPS;
	else
		m_eID = DYNAMIC;
}

CCameraManager* CCameraManager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCameraManager* pInstance = new CCameraManager(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCameraManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCameraManager::Clone(void* pArg)
{
	CCameraManager* pInstance = new CCameraManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCameraManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCameraManager::Free()
{
	__super::Free();

	for (auto& Camera : m_Cameras)
		Safe_Release(Camera);

	m_Cameras.clear();
}
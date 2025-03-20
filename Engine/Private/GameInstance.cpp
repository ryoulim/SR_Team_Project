#include "GameInstance.h"

#include "Renderer.h"
#include "Level_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Key_Manager.h"
#include "Input_Device.h"
#include "Timer_Manager.h"
#include "csvReader.h"

IMPLEMENT_SINGLETON(CGameInstance);

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInputDevice = CInput_Device::Create(EngineDesc.hInst,EngineDesc.hWnd);
	if (nullptr == m_pInputDevice)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pKeyManager = CKey_Manager::Create();
	if (nullptr == m_pKeyManager)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pCsv_Reader = CcsvReader::Create();
	if (nullptr == m_pCsv_Reader)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInputDevice->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pObject_Manager->Update(fTimeDelta);

	

	m_pObject_Manager->Late_Update(fTimeDelta);


	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Render_Begin();

	m_pRenderer->Draw();

	m_pLevel_Manager->Render();

	m_pGraphic_Device->Render_End();

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* 특정 레벨의 자원을 삭제한다. */
	
	/* 특정 레벨의 객체을 삭제한다. */
	m_pObject_Manager->Clear(iLevelIndex);
	/* 특정 레벨의 원형객을 삭제한다. */
	m_pPrototype_Manager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Draw_Font(const _wstring& str, LPRECT pRect)
{
	return m_pGraphic_Device->Draw_Font(str,pRect);
}

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}
#pragma endregion

#pragma region PROTOTYPE_MANAGERㄱ

HRESULT CGameInstance::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeType, iPrototypeLevelIndex, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region OBJECT_MANAGER

HRESULT CGameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

HRESULT CGameInstance::Add_GameObjectReturn(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, CGameObject** ppOut, void* pArg)
{
	return m_pObject_Manager->Add_GameObjectReturn(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, ppOut, pArg);
}

CGameObject* CGameInstance::Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex)
{
	return m_pObject_Manager->Find_Object(iLevelIndex,strLayerTag,iVectorIndex);
}

vector<CGameObject*>& CGameInstance::Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Find_Objects(iLevelIndex, strLayerTag);
}

HRESULT CGameInstance::Create_Object_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strObjectTag, _uint iPoolSize)
{
	return m_pObject_Manager->Create_Object_Pool(iPrototypeLevelIndex, strPrototypeTag, strObjectTag, iPoolSize);
}

HRESULT CGameInstance::Release_Object_Pool(const _wstring& strObjectTag)
{
	return m_pObject_Manager->Release_Object_Pool(strObjectTag);
}

_uint CGameInstance::Active_Object(const _wstring& strObjectTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Active_Object(strObjectTag, iLevelIndex, strLayerTag, pArg);
}

_uint CGameInstance::Deactive_Object(const _wstring& strObjectTag, CGameObject* pObject)
{
	return m_pObject_Manager->Deactive_Object(strObjectTag, pObject);
}

void CGameInstance::Intersect(_uint iLevelIndex, const _wstring& strLayerTag1, const _wstring& strLayerTag2)
{
	m_pObject_Manager->Intersect(iLevelIndex, strLayerTag1, strLayerTag2);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma endregion

#pragma region INPUT_DEVICE

_byte CGameInstance::Get_DIKState(_ubyte eKeyID)
{
	return m_pInputDevice->Get_DIKState(eKeyID);
}

_byte CGameInstance::Get_DIMKeyState(DIMK eMouseKeyID)
{
	return m_pInputDevice->Get_DIMKeyState(eMouseKeyID);
}

_long CGameInstance::Get_DIMMoveState(DIMM eMouseMoveID)
{
	return m_pInputDevice->Get_DIMMoveState(eMouseMoveID);
}
#pragma endregion


#pragma region KEY_MANAGER

_bool CGameInstance::Key_Pressing(_int _Key)
{
	return m_pKeyManager->Key_Pressing(_Key);
}

_bool CGameInstance::Key_Up(_int _Key)
{
	return m_pKeyManager->Key_Up(_Key);
}

_bool CGameInstance::Key_Down(_int _Key)
{
	return m_pKeyManager->Key_Down(_Key);
}

#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Update_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Update(strTimerTag);
}

HRESULT CGameInstance::Readcsv(const _wstring& strcsvPath, class CLevelData* pDatas)
{
	return m_pCsv_Reader->Readcsv(strcsvPath, pDatas);
}

#pragma endregion

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pCsv_Reader);

	Safe_Release(m_pTimer_Manager);

	Safe_Release(m_pKeyManager);

	Safe_Release(m_pRenderer);

	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pPrototype_Manager);

	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pInputDevice);

	Safe_Release(m_pGraphic_Device);

	Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();
}

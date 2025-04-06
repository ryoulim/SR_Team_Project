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
#include "Collider_Manager.h"
#include "Sound_Device.h"
#include "Management.h"

IMPLEMENT_SINGLETON(CGameInstance);

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut)
{
	srand(static_cast<unsigned int>(time(0)));

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInputDevice = CInput_Device::Create(EngineDesc.hInst,EngineDesc.hWnd);
	if (nullptr == m_pInputDevice)
		return E_FAIL;

	m_pSound_Device = CSound_Device::Create(EngineDesc.strBankFilePath, EngineDesc.strSoundFolderPath);
	if (nullptr == m_pSound_Device)
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

	m_pCollider_Manager = CCollider_Manager::Create(EngineDesc.INumColliderGroups);
	if(nullptr == m_pCsv_Reader)
		return E_FAIL;

	m_pManagement = CManagement::Create();
	if (nullptr == m_pManagement)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInputDevice->Update();
	m_pSound_Device->Update();

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

FORCEINLINE
HRESULT CGameInstance::Draw_Font(const _wstring& str, LPRECT pRect)
{
	return m_pGraphic_Device->Draw_Font(str,pRect);
}

#pragma region LEVEL_MANAGER

FORCEINLINE
HRESULT CGameInstance::Change_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}
void CGameInstance::Change_Level(_uint iLevelIndex)
{
	m_pLevel_Manager->Change_Level(iLevelIndex);
}
#pragma endregion

#pragma region PROTOTYPE_MANAGER

FORCEINLINE
HRESULT CGameInstance::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}

FORCEINLINE
CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeType, iPrototypeLevelIndex, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region OBJECT_MANAGER

FORCEINLINE
HRESULT CGameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

FORCEINLINE
HRESULT CGameInstance::Add_GameObjectReturn(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, CGameObject** ppOut, void* pArg)
{
	return m_pObject_Manager->Add_GameObjectReturn(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, ppOut, pArg);
}

FORCEINLINE
HRESULT CGameInstance::Release_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Release_Layer(iLevelIndex,strLayerTag);
}

FORCEINLINE
HRESULT CGameInstance::Push_GameObject(CGameObject* pObject, _uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Push_GameObject(pObject, iLevelIndex, strLayerTag);
}

FORCEINLINE
CGameObject* CGameInstance::Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex)
{
	return m_pObject_Manager->Find_Object(iLevelIndex,strLayerTag,iVectorIndex);
}

FORCEINLINE
list<CGameObject*>* CGameInstance::Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Find_Objects(iLevelIndex, strLayerTag);
}

FORCEINLINE
void CGameInstance::Update_Frustum(const _float4x4& viewProj)
{
	m_pObject_Manager->Update_Frustum(viewProj);
}

FORCEINLINE
_bool CGameInstance::IsPointInFrustum(const _float3& Point)
{
	return m_pObject_Manager->IsPointInFrustum(Point);
}
#pragma endregion

#pragma region COLLIDER_MANAGER

FORCEINLINE
HRESULT CGameInstance::Add_Collider(CCollider* pCollider, _uint iColliderGroupID)
{
	return m_pCollider_Manager->Add_Collider(pCollider, iColliderGroupID);
}

FORCEINLINE
void CGameInstance::Delete_Collider(const CGameObject* pOwner)
{
	m_pCollider_Manager->Delete_Collider(pOwner);
}

void CGameInstance::Clear_Collider()
{
	m_pCollider_Manager->Clear();
}

FORCEINLINE
void CGameInstance::Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2)
{
	m_pCollider_Manager->Intersect(iColliderGroupID1, iColliderGroupID2);
}

FORCEINLINE
_bool CGameInstance::RaycastBetweenPoints(const _float3& Point1, const _float3& Point2, _uint ColliderGroupID)
{
	return m_pCollider_Manager->RaycastBetweenPoints(Point1, Point2, ColliderGroupID);
}

FORCEINLINE
class CCollider* CGameInstance::Raycast(const _float3& rayOrigin, const _float3& rayDir, _float rayLength, const initializer_list<_uint>& ColliderGroupIDs, _uint& _Out_ ColliderID)
{
	return m_pCollider_Manager->Raycast(rayOrigin, rayDir, rayLength, ColliderGroupIDs, ColliderID);
}

FORCEINLINE
_float CGameInstance::Raycast_Downward(const _float3& rayOrigin, _uint iColliderGroupID)
{
	return m_pCollider_Manager->Raycast_Downward(rayOrigin, iColliderGroupID);
}

FORCEINLINE
const list<class CCollider*>* CGameInstance::Get_Colliders(_uint iIndex)
{
	return m_pCollider_Manager->Get_Colliders(iIndex);
}

#pragma endregion

#pragma region RENDERER

FORCEINLINE
HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma endregion

#pragma region INPUT_DEVICE

FORCEINLINE
_byte CGameInstance::Get_DIKState(_ubyte eKeyID)
{
	return m_pInputDevice->Get_DIKState(eKeyID);
}

FORCEINLINE
_byte CGameInstance::Get_DIMKeyState(DIMK eMouseKeyID)
{
	return m_pInputDevice->Get_DIMKeyState(eMouseKeyID);
}

FORCEINLINE
_long CGameInstance::Get_DIMMoveState(DIMM eMouseMoveID)
{
	return m_pInputDevice->Get_DIMMoveState(eMouseMoveID);
}

FORCEINLINE
_bool CGameInstance::Mouse_Down(_ubyte eKeyID)
{
	return m_pInputDevice->Mouse_Down(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Mouse_Pressing(_ubyte eKeyID)
{
	return m_pInputDevice->Mouse_Pressing(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Mouse_Up(_ubyte eKeyID)
{
	return m_pInputDevice->Mouse_Up(eKeyID);
}
#pragma endregion

FORCEINLINE
_bool CGameInstance::Key_Pressing(_ubyte eKeyID)
{
	return m_pInputDevice->Key_Pressing(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Key_Up(_ubyte eKeyID)
{
	return m_pInputDevice->Key_Up(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Key_Down(_ubyte eKeyID)
{
	return m_pInputDevice->Key_Down(eKeyID);
}

#pragma region TIMER_MANAGER

FORCEINLINE
_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

FORCEINLINE
_float CGameInstance::Get_Scaled_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_Scaled_TimeDelta(strTimerTag);
}

FORCEINLINE
void CGameInstance::Set_TimeScale(const _wstring& strTimerTag, _float fDeltaScale)
{
	m_pTimer_Manager->Set_TimeScale(strTimerTag, fDeltaScale);
}

FORCEINLINE
HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

FORCEINLINE
void CGameInstance::Update_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Update(strTimerTag);
}
#pragma endregion

#pragma region CSV_READER
FORCEINLINE
HRESULT CGameInstance::Readcsv(const _wstring& strcsvPath, class CLevelData* pDatas)
{
	return m_pCsv_Reader->Readcsv(strcsvPath, pDatas);
}
#pragma endregion


#pragma region SOUND_DEVICE
FORCEINLINE
HRESULT CGameInstance::LoadBank(const string& name)
{
	return m_pSound_Device->LoadBank(name);
}

FORCEINLINE
void CGameInstance::UnloadBank(const string& name)
{
	m_pSound_Device->UnloadBank(name);
}

FORCEINLINE
CSound_Event* CGameInstance::Create_Sound_Event(const string& eventPath)
{
	return m_pSound_Device->Create_Event_Instance(eventPath);
}

FORCEINLINE
HRESULT CGameInstance::LoadSound(const string& Path, _bool is3D, _bool loop, _bool stream, unordered_map<string, class CSound_Core*>* _Out_ pOut)
{
	return m_pSound_Device->LoadSound(Path, is3D, loop, stream, pOut);
}

//FORCEINLINE
//CSound_Core* CGameInstance::Create_Core_Sound(const string& strSoundKey)
//{
//	return m_pSound_Device->Create_Core_Instance(strSoundKey);
//}

FORCEINLINE
void CGameInstance::Set_Listener_Position(const CTransform* pTransform, const _float3& vel)
{
	m_pSound_Device->Set_Listener_Position(pTransform, vel);
}

FORCEINLINE
void CGameInstance::Set_Master_Volume(_float volume)
{
	m_pSound_Device->Set_Master_Volume(volume);
}
#pragma endregion

#pragma region 

FORCEINLINE
HRESULT CGameInstance::Add_Manager(const _wstring& strManagertag, CBase* pManager)
{
	return m_pManagement->Add_Manager(strManagertag, pManager);
}

FORCEINLINE
CBase* CGameInstance::Find_Manager(const _wstring& strManagertag) const
{
	return m_pManagement->Find_Manager(strManagertag);
}

#pragma endregion

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pManagement);

	Safe_Release(m_pCollider_Manager);

	Safe_Release(m_pCsv_Reader);

	Safe_Release(m_pTimer_Manager);

	Safe_Release(m_pKeyManager);

	Safe_Release(m_pRenderer);

	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pPrototype_Manager);

	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pSound_Device);

	Safe_Release(m_pInputDevice);

	Safe_Release(m_pGraphic_Device);

	Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();
}

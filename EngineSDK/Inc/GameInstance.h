#pragma once

/* 엔진과 클라이언트 간의 링크의 역활을 수행한다. */
/* 엔진 내에 존재하는 유일한 싱글톤 클래스이다. */
/* 엔진 개발자가 클라개밫자에게 보여주고싶은 함수를 ... */
#include "Renderer.h"
#include "Prototype_Manager.h"

BEGIN(Engine)


class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ LPDIRECT3DDEVICE9* ppOut);
	void Update_Engine(_float fTimeDelta);
	HRESULT Draw();	
	void Clear(_uint iLevelIndex);

#pragma region GRAPHIC_DEVICE 
	HRESULT		Draw_Font(const _wstring& str, LPRECT pRect);
#pragma endregion

#pragma region LEVEL_MANAGER
	HRESULT Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	void Change_Level(_uint iLevelIndex);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	HRESULT Add_GameObjectReturn(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, CGameObject** ppOut, void* pArg);
	HRESULT	Release_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	HRESULT Push_GameObject(class CGameObject* pObject, _uint iLevelIndex, const _wstring& strLayerTag);

	// 매개변수에 맞는 오브젝트를 찾아서 반환해준다.
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex = 0);
	// 매개변수에 맞는 레이어의 오브젝트 리스트를 찾아서 반환해준다.
	list<CGameObject*>* Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag);
	//절두체 업데이트
	void Update_Frustum(const _float4x4& viewProj);
	// 스크린상에 있는지 검사
	_bool IsPointInFrustum(const _float3& Point);
#pragma endregion

#pragma region COLLISION_MANAGER
	HRESULT Add_Collider(class CCollider* pCollider, _uint iColliderGroupID);
	// 오브젝트 키를 받아서 그거에 해당하는 콜라이더를 싹다 지워버림
	void Delete_Collider(const class CGameObject* pOwner);
	void Clear_Collider();
	// A그룹과 B그룹의 충돌을 검사한다.
	void Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2);
	/// <summary>
	/// 개빡치는 레이캐스트
	/// </summary>
	/// <param name="rayOrigin"> 레이를 쏘는 시작지점 </param>
	/// <param name="rayDir"> 레이를 쏘는 방향 </param>
	/// <param name="rayLength"> 레이의 최대 길이 </param>
	/// <param name="ColliderGroupIDs"> 체크할 그룹을 중괄호로 넣으세요 </param>
	/// <param name="ColliderID"> 여기다가 부딪힌 콜라이더 아이디 뱉어줄게 온콜리전은 니가불러 </param>
	/// <returns></returns>
	class CGameObject* Raycast(const _float3& rayOrigin, const _float3& rayDir, _float rayLength, const initializer_list<_uint>& ColliderGroupIDs, _uint& _Out_ ColliderID);
	// 아래로만 쏘는 레이
	_float Raycast_Downward(const _float3& rayOrigin, _uint iColliderGroupID);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region INPUT_DEVICE
	_byte Get_DIKState(_ubyte eKeyID);
	_byte Get_DIMKeyState(DIMK eMouseKeyID);
	_long Get_DIMMoveState(DIMM eMouseMoveID);

	_bool Mouse_Down(_ubyte eKeyID);
	_bool Mouse_Pressing(_ubyte eKeyID);
	_bool Mouse_Up(_ubyte eKeyID);

	_bool		Key_Pressing(_ubyte eKeyID);
	_bool		Key_Up(_ubyte eKeyID);
	_bool		Key_Down(_ubyte eKeyID);
#pragma endregion

#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region CSV_READER
	HRESULT Readcsv(const _wstring& strcsvPath, class CLevelData* pDatas);
#pragma endregion

#pragma region SOUND_DEVICE
	HRESULT LoadBank(const string& name);
	void UnloadBank(const string& name);

	class CSound_Event* Create_Sound_Event(const string& eventPath);
	class CSound_Core* Create_Core_Sound(const string& path, _bool is3D = TRUE, _bool loop = FALSE, _bool stream = FALSE);

	void Set_Listener_Position(const class CTransform* pTransform, const _float3& vel);
	void Set_Master_Volume(_float volume);
#pragma endregion

#pragma region MANAGEMENT
	HRESULT Add_Manager(const _wstring& strManagertag, CBase* pManager);
	CBase* Find_Manager(const _wstring& strManagertag) const;
#pragma endregion

#pragma region UTILITY
	_float RandomFloat(_float min, _float max)
	{
		if (min > max)
			swap(min, max);
		uniform_real_distribution<float> dist(min, max);
		return dist(m_Gen);
	}
#pragma endregion

private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CInput_Device*		m_pInputDevice = { nullptr };
	class CKey_Manager*			m_pKeyManager = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CcsvReader*			m_pCsv_Reader = { nullptr };
	class CCollider_Manager*	m_pCollider_Manager = { nullptr };
	class CSound_Device*		m_pSound_Device = { nullptr };
	class CManagement*			m_pManagement = { nullptr };

	mt19937 m_Gen{ std::random_device{}() };

public:
	void Release_Engine();
	virtual void Free() override;
};

END
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
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	HRESULT Add_GameObjectReturn(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, CGameObject** ppOut, void* pArg);
	// 매개변수에 맞는 오브젝트를 찾아서 반환해준다.
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex = 0);
	// 매개변수에 맞는 레이어의 오브젝트 리스트를 찾아서 반환해준다.
	list<CGameObject*>& Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag);

	// 오브젝트 풀 생성
	HRESULT Create_Object_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strObjectTag, _uint iPoolSize);
	// 오브젝트 풀 삭제
	HRESULT Release_Object_Pool(const _wstring& strObjectTag);
	// 오브젝트 풀에서 객체 하나를 오브젝트 리스트에 넣는다.(Reset 함수 호출)
	_uint Active_Object(const _wstring& strObjectTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	// 오브젝트 리스트에서 해당객체를 빼서 오브젝트 풀에 반환한다.
	_uint Deactive_Object(const _wstring& strObjectTag, class CGameObject* pObject);
	//절두체 업데이트
	void Update_Frustum(const _float4x4& viewProj);
	// 스크린상에 있는지 검사
	_bool IsPointInFrustum(const _float3& Point);
#pragma endregion

#pragma region COLLISION_MANAGER
	HRESULT Add_Collider(class CCollider* pCollider, _uint iColliderGroupID);
	// 오브젝트 키를 받아서 그거에 해당하는 콜라이더를 싹다 지워버림
	void Delete_Collider(const class CGameObject* pOwner);
	// A그룹과 B그룹의 충돌을 검사한다.
	void Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2);
	// 개같은 레이캐스트
	_bool Raycast(const _float3& rayOrigin, const _float3& rayDir, _uint iColliderGroupID);
	// 아래로만 쏘는 레이
	_bool Raycast_Downward(const _float3& rayOrigin, _uint iColliderGroupID);
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

#pragma region KEY_MANAGER
#pragma endregion

#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region CSV_READER
	HRESULT Readcsv(const _wstring& strcsvPath, class CLevelData* pDatas);
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

public:
	void Release_Engine();
	virtual void Free() override;
};

END
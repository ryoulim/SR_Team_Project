#pragma once

/* ������ Ŭ���̾�Ʈ ���� ��ũ�� ��Ȱ�� �����Ѵ�. */
/* ���� ���� �����ϴ� ������ �̱��� Ŭ�����̴�. */
/* ���� �����ڰ� Ŭ�󰳓T�ڿ��� �����ְ���� �Լ��� ... */
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
	// �Ű������� �´� ������Ʈ�� ã�Ƽ� ��ȯ���ش�.
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex = 0);
	// �Ű������� �´� ���̾��� ������Ʈ ���͸� ã�Ƽ� ��ȯ���ش�.
	vector<CGameObject*>& Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag);

	// ������Ʈ Ǯ ����
	HRESULT Create_Object_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strObjectTag, _uint iPoolSize);
	// ������Ʈ Ǯ ����
	HRESULT Release_Object_Pool(const _wstring& strObjectTag);
	// ������Ʈ Ǯ���� ��ü �ϳ��� ������Ʈ ����Ʈ�� �ִ´�.(Reset �Լ� ȣ��)
	_uint Active_Object(const _wstring& strObjectTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	// ������Ʈ ����Ʈ���� �ش簴ü�� ���� ������Ʈ Ǯ�� ��ȯ�Ѵ�.
	_uint Deactive_Object(const _wstring& strObjectTag, class CGameObject* pObject);

#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region INPUT_DEVICE
	_byte Get_DIKState(_ubyte eKeyID);
	_byte Get_DIMKeyState(DIMK eMouseKeyID);
	_long Get_DIMMoveState(DIMM eMouseMoveID);

#pragma endregion

#pragma region KEY_MANAGER
	_bool		Key_Pressing(_int _Key);
	_bool		Key_Up(_int _Key);
	_bool		Key_Down(_int _Key);
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

public:
	void Release_Engine();
	virtual void Free() override;
};

END
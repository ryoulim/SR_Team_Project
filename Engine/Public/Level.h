#pragma once
#include "Base.h"
#include "LevelData.h"

/* Ŭ�� �����ڰ� ���� ����Ŭ�������� �θ� �Ǵ� Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	typedef map<const _wstring, void*> LEVELDATA;
protected:
	CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize(class CLevelData* pLevelData);
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual _wstring Compute_PrototypeName(const _wstring& strPrototypeTag);
	virtual HRESULT Load_Map(_uint eLevelIdx, const _wstring& FileName);
	HRESULT Load_VertexBuffer(class CGameObject* _pGameObject, HANDLE phFile, _ulong* pByte);

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };
	class CLevelData* m_pData = { nullptr };

public:	
	virtual void Free() override;

};

END
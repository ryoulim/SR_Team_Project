// �� Ŭ���� �̸� : LoadingCutscene
// �θ� Ŭ���� �̸� : UI

#pragma once
#include "UI.h"

BEGIN(Client)
/* �θ� �Ǵ� �ƾ� �������̽��� */
class CLoadingCutscene : public CUI 
{
public:
	typedef struct tagLoadingCutsceneDesc : public CUI::DESC
	{

	}DESC;

protected:
	CLoadingCutscene(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoadingCutscene(const CLoadingCutscene& Prototype);
	virtual ~CLoadingCutscene() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLoadingCutscene* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
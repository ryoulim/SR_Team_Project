#pragma once
#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCutScene_Camera final : public CCamera
{
private:
	CCutScene_Camera(LPDIRECT3DDEVICE9 pGraphicDevice);
	CCutScene_Camera(const CCutScene_Camera& Prototype);
	virtual ~CCutScene_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void		Start_CutScene(vector<_float3>* pMovePoints, vector<_float3>* pLookPoints, _float fCameraSpeed, _bool* _Out_ pEndFlag);
	void		CutSceneEnd();

private:
	vector<_float3>*	m_pMovePoints{nullptr};
	vector<_float3>*	m_pLookPoints{ nullptr };

	_float				m_fTimeAcc{};
	_float				m_fCameraSpeed{};

	_bool*				m_bEndFlag{ nullptr };

private:
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;

private:
	_float3				CatmullRom(_uint iIndex, _float t, _bool isPos) const;

	//const _float3&		GetSafePoint(_int index) const;
public:
	static CCutScene_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CCutScene_Camera* Clone(void* pArg) override;
	virtual void Free() override;
};

END
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

	void		Start_CutScene(const _float3& vPos, const _float3& vLook);
	void		Start_CutScene(vector<_float3>* pMovePoints, vector<_float3>* pLookPoints, _float fCameraSpeed, _bool* _Out_ pEndFlag);
	void		StartShake(_float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot);
	void		CutSceneEnd();

private:
	vector<_float3>*	m_pMovePoints{nullptr};
	vector<_float3>*	m_pLookPoints{ nullptr };

	_float				m_fTimeAcc{};
	_float				m_fCameraSpeed{};

	_bool*				m_pEndFlag{ nullptr };

	// 카메라 쉐이크 관련
	_bool  m_bShake = FALSE;
	_float m_fShakeTime = 0.f;
	_float m_fShakeDuration = 0.f;
	_float m_fShakeIntensity = 0.f;
	_float m_fShakeFreqPos = 100.f; // 위치 진동 속도
	_float m_fShakeFreqRot = 40.f;  // 회전 진동 속도
	_float3  m_vCurrentShakePos = { 0.f, 0.f, 0.f };
	_float3  m_vCurrentShakeRot = { 0.f, 0.f, 0.f };

private:
	virtual void		Update_View_Matrix() override;
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;

private:
	_float3				CatmullRom(_uint iIndex, _float t, _bool isPos) const;
	void				Update_Camera_Shake(_float fTimedelta);

public:
	static CCutScene_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CCutScene_Camera* Clone(void* pArg) override;
	virtual void Free() override;
};

END
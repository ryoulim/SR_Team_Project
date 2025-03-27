// 내 클래스 이름 : CameraManager
// 부모 클래스 이름 : GameObject

#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Client)

class CCameraManager final : public CGameObject
{
public:
	enum CAMERAID { FPS, DYNAMIC };
private:
	CCameraManager(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCameraManager(const CCameraManager& Prototype);
	virtual ~CCameraManager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	/// <summary>
	/// 카메라 흔들어 제껴
	/// </summary>
	/// <param name="fIntensity"> 카메라 흔드는 세기 </param>
	/// <param name="fDuration"> 카메라 흔드는 시간 </param>
	/// <param name="fShakeFreqPos"> 이동 진동 주파수(클수록 빨라짐) </param>
	/// <param name="fShakeFreqRot"> 회전 진동 주파수(클수록 빨라짐)</param>
	virtual void Shake_Camera(_float fIntensity = 1.f, _float fDuration = 1.f, _float fShakeFreqPos = 100.f, _float fShakeFreqRot = 40.f);

	void Switch(_bool isFPSMode);

	CGameObject* Get_Camera(CAMERAID m_ID) {
		return m_Cameras[m_ID];
	}

private:
	CAMERAID m_eID{ FPS };
	vector<CGameObject*> m_Cameras;
public:
	static CCameraManager* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
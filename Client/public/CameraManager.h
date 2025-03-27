// �� Ŭ���� �̸� : CameraManager
// �θ� Ŭ���� �̸� : GameObject

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
	/// ī�޶� ���� ����
	/// </summary>
	/// <param name="fIntensity"> ī�޶� ���� ���� </param>
	/// <param name="fDuration"> ī�޶� ���� �ð� </param>
	/// <param name="fShakeFreqPos"> �̵� ���� ���ļ�(Ŭ���� ������) </param>
	/// <param name="fShakeFreqRot"> ȸ�� ���� ���ļ�(Ŭ���� ������)</param>
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
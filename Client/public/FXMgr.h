#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include <random>

#define FX_MGR static_cast<CFXMgr*>(m_pGameInstance->Find_Manager(TEXT("FX_Manager")))
			/* ����Ʈ �̱��� */

/* [ ������𼭵� ����Ʈ�� �����ų �� �ִ�. ] */

BEGIN(Engine)
class CObjectPool;
END

BEGIN(Client)
extern bool g_FogTrigger;
extern float g_FogCustom;

class CCameraSprite;
class CFXMgr : public CBase
{
private:
	CFXMgr();
	virtual ~CFXMgr() = default;

public:
	HRESULT		Initialize();
	HRESULT		Update(_float timeDelta);

	bool IsFlashing() const;
	bool IsThunder() const;
	void TriggerFlash();
	void TriggerThunder();

public: //����
	void SpawnCustomExplosion(_float3 _vPosition, LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe);
	void SpawnExplosion(_float3 _vPosition, LEVEL eLevel);
	void SpawnExplosion2(_float3 _vPosition, LEVEL eLevel);
	void SpawnExplosion3(_float3 _vPosition, LEVEL eLevel);
	void SpawnMultipleExplosions(_float fTimeDelta, LEVEL eLevel);
	void SpawnMultipleExplosions2(_float fTimeDelta, LEVEL eLevel);
	void SpawnMultipleExplosions3(_float fTimeDelta, LEVEL eLevel);

	void SpawnRaceExplosion(_float3 _vPosition, _float3 _vPosOffset, LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe);
	void SpawnMultipleExplosionRacePoint(_float fTimeDelta, _float3 _vPosition, _float3 _vPosOffset, LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe);
	void SpawnMultipleExplosionRaceBoss(_float3 _Position, LEVEL eLevel);

public: //��
	void SpawnRain(LEVEL eLevel);
	void SpawnThunder(_float3 _vPosition, LEVEL eLevel);
	void SpawnMultipleThunder(_float fTimeDelta, LEVEL eLevel);

public: //���
	void SpawnRava(_float3 _vPosition, LEVEL eLevel);
	
public: //�Ѿ�
	void SpawnGunFire(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnShotGunFire(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnBulletTracer(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnBulletTracerMachineGun(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnShotGunTracer(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnGunFireMachineGun(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnFireMachineGun(_float3 _ScreenPos, LEVEL eLevel);

public: //����̵�
	void SpawnTornado(_float3 _vPosition, LEVEL eLevel);
	void SpawnSpher(_float3 _vPosition, LEVEL eLevel);
	void SpawnSpherSmall(_float3 _vPosition, LEVEL eLevel);
	void SpawnField(_float3 _vPosition, LEVEL eLevel);

public: //ź��
	void SpawnEmptyBullet(_float3 _vPosition, LEVEL eLevel);

public: //���긮��
	void SpawnBlood(_float3 _vPosition, LEVEL eLevel);

public: //�Ѿ��ڱ�
	void SpawnBulletMark(_float3 _vPosition, LEVEL eLevel, _float3 _vLook, _int _iNum);
	void SpawnBulletDust(_float3 _vPosition, LEVEL eLevel);

public: //�ǰ� ����Ʈ
	void SpawnHitEffect(LEVEL eLevel);
	void SpawnThanderEffect(LEVEL eLevel);
	void SpawnHealEffect(LEVEL eLevel);

public: //������ų
	void FireAttack(_float3 _vPosition, LEVEL eLevel, _int _iNum);
	void JumpAttack(_float3 _vPosition, LEVEL eLevel);
	void CutSceneSmoke(_float3 _vPosition, LEVEL eLevel);

public: //�÷��̾�뽬
	void PlayerDash(LEVEL eLevel);
	void Damage_Indicator(LEVEL eLevel, _float RotZRadian);

public: //������ġ
	_float3 GetThunderPos() { return m_vThunderPos; }
	void SetThunderPos(_float3 _vPosition) { m_vThunderPos = _vPosition; }


private:
	class CGameInstance* m_pGameInstance;
	unordered_map<_wstring, class CObjectPool*> m_ObjectPools;
	CSoundController* m_pSoundCom = { nullptr };

public:
	static CFXMgr* Create();
	virtual void Free() override;

private: /* ������ ��ġ�� ��� ���� */
	_float3 m_vThunderPos = { 0.f, 10000.f, 0.f };

private: /* �� �� �i��? */
	_float m_fFlashTimer = 0.f;
	_float m_fThunderTimer = 0.f;
	_float m_fMaxFlashTime = 0.1f;
	_float m_fMaxThunderTime = 0.3f;
};

END
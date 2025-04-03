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

class CCameraSprite;
class CFXMgr : public CBase
{
private:
	CFXMgr();
	virtual ~CFXMgr() = default;

public:
	HRESULT		Initialize();

public: //����
	void SpawnCustomExplosion(_float3 _vPosition, LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe);
	void SpawnExplosion(_float3 _vPosition, LEVEL eLevel);
	void SpawnExplosion2(_float3 _vPosition, LEVEL eLevel);
	void SpawnExplosion3(_float3 _vPosition, LEVEL eLevel);
	void SpawnMultipleExplosions(_float fTimeDelta, LEVEL eLevel);
	void SpawnMultipleExplosions2(_float fTimeDelta, LEVEL eLevel);
	void SpawnMultipleExplosions3(_float fTimeDelta, LEVEL eLevel);

public: //��
	void SpawnRain(LEVEL eLevel);

public: //��
	void SpawnFire(_float3 _vPosition, LEVEL eLevel);
	
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

public: //�ǰ� ����Ʈ
	void SpawnHitEffect(LEVEL eLevel);
	void SpawnHealEffect(LEVEL eLevel);

public: //������ų
	void FireAttack(_float3 _vPosition, LEVEL eLevel, _int _iNum);
	void JumpAttack(_float3 _vPosition, LEVEL eLevel);

public: //�÷��̾�뽬
	void PlayerDash(LEVEL eLevel);


public: //�ߺ������� ���� �Լ���
	void SetbHit(_bool Hit) { m_IsHit = Hit; }
	bool GetbHit() { return m_IsHit; }

private:
	class CGameInstance* m_pGameInstance;
	unordered_map<_wstring, class CObjectPool*> m_ObjectPools;
public:
	static CFXMgr* Create();
	virtual void Free() override;

private:
	_bool m_IsHit = false;
};

END
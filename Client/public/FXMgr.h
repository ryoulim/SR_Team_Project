#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include <random>

#define FX_MGR static_cast<CFXMgr*>(m_pGameInstance->Find_Manager(TEXT("FX_Manager")))
			/* 이펙트 싱글톤 */

/* [ 언제어디서든 이펙트를 실행시킬 수 있다. ] */

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

public: //폭발
	void SpawnCustomExplosion(_float3 _vPosition, LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe);
	void SpawnExplosion(_float3 _vPosition, LEVEL eLevel);
	void SpawnExplosion2(_float3 _vPosition, LEVEL eLevel);
	void SpawnExplosion3(_float3 _vPosition, LEVEL eLevel);
	void SpawnMultipleExplosions(_float fTimeDelta, LEVEL eLevel);
	void SpawnMultipleExplosions2(_float fTimeDelta, LEVEL eLevel);
	void SpawnMultipleExplosions3(_float fTimeDelta, LEVEL eLevel);

public: //비
	void SpawnRain(LEVEL eLevel);

public: //불
	void SpawnFire(_float3 _vPosition, LEVEL eLevel);
	
public: //총알
	void SpawnGunFire(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnShotGunFire(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnBulletTracer(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnBulletTracerMachineGun(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnShotGunTracer(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnGunFireMachineGun(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnFireMachineGun(_float3 _ScreenPos, LEVEL eLevel);

public: //토네이도
	void SpawnTornado(_float3 _vPosition, LEVEL eLevel);
	void SpawnSpher(_float3 _vPosition, LEVEL eLevel);
	void SpawnSpherSmall(_float3 _vPosition, LEVEL eLevel);
	void SpawnField(_float3 _vPosition, LEVEL eLevel);

public: //탄피
	void SpawnEmptyBullet(_float3 _vPosition, LEVEL eLevel);

public: //피흘리기
	void SpawnBlood(_float3 _vPosition, LEVEL eLevel);


public: //총알자국
	void SpawnBulletMark(_float3 _vPosition, LEVEL eLevel, _float3 _vLook, _int _iNum);

public: //피격 이펙트
	void SpawnHitEffect(LEVEL eLevel);
	void SpawnHealEffect(LEVEL eLevel);

public: //보스스킬
	void FireAttack(_float3 _vPosition, LEVEL eLevel, _int _iNum);
	void JumpAttack(_float3 _vPosition, LEVEL eLevel);

public: //플레이어대쉬
	void PlayerDash(LEVEL eLevel);


public: //중복방지를 위한 함수들
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
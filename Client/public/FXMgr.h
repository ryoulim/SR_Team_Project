#pragma once

#include "Base.h"
#include "GameInstance.h"

#include "Client_Defines.h"
#include <random>


			/* 이펙트 싱글톤 */

/* [ 언제어디서든 이펙트를 실행시킬 수 있다. ] */

BEGIN(Client)

class CCameraSprite;
class CFXMgr : public CBase
{
	DECLARE_SINGLETON(CFXMgr)
	
private:
	CFXMgr();
	virtual ~CFXMgr() = default;

public:
	void		Initialize();
	void		Update(_float fTimeDelta);
	void		LateUpdate();

	void		SpawnCustomExplosion(_float3 _vPosition, LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe);


public: //폭발
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
	void SpawnBulletTracer(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnBulletTracerMachineGun(_float3 _ScreenPos, LEVEL eLevel);

public: //토네이도
	void SpawnTornado(_float3 _vPosition, LEVEL eLevel);
	void SpawnSpher(_float3 _vPosition, LEVEL eLevel);

public: //탄피
	void SpawnEmptyBullet(_float3 _vPosition, LEVEL eLevel);

public: //피흘리기
	void SpawnBlood(_float3 _vPosition, LEVEL eLevel);


public:
	float GetRandomValue(float min, float max)
	{
		if (min > max) std::swap(min, max);
		uniform_real_distribution<float> dist(min, max);
		return dist(m_Gen);
	}


public:
	virtual void Free() override;

private: //랜덤난수 생성 변수
	random_device m_Rd;
	mt19937 m_Gen;
	uniform_real_distribution<float> m_Dist;

public: //스크린 이펙트 활성화 배열
	vector<CCameraSprite*> m_vecSceenEffect;


private:
	CGameInstance*	m_pGameInstance;
};

END
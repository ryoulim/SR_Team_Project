#pragma once

#include "Base.h"
#include "GameInstance.h"

#include "Client_Defines.h"
#include <random>


			/* ����Ʈ �̱��� */

/* [ ������𼭵� ����Ʈ�� �����ų �� �ִ�. ] */

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


public: //����
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
	void SpawnBulletTracer(_float3 _ScreenPos, LEVEL eLevel);
	void SpawnBulletTracerMachineGun(_float3 _ScreenPos, LEVEL eLevel);

public: //����̵�
	void SpawnTornado(_float3 _vPosition, LEVEL eLevel);
	void SpawnSpher(_float3 _vPosition, LEVEL eLevel);

public: //ź��
	void SpawnEmptyBullet(_float3 _vPosition, LEVEL eLevel);

public: //���긮��
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

private: //�������� ���� ����
	random_device m_Rd;
	mt19937 m_Gen;
	uniform_real_distribution<float> m_Dist;

public: //��ũ�� ����Ʈ Ȱ��ȭ �迭
	vector<CCameraSprite*> m_vecSceenEffect;


private:
	CGameInstance*	m_pGameInstance;
};

END
#include "Particle_Define.h"
#include <random>

const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	//[0, 1] ������ ���� float �� ��´�.
	float f = (rand() % 10000) * 0.0001f;

	//[lowBound, highBound] ������ float �� �����Ѵ�.
	return (f * (highBound - lowBound)) + lowBound;
}

int GetRandomInt(int lowBound, int highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	int iResult = (rand() % (highBound - lowBound + 1)) + lowBound;

	return iResult;
}



DWORD FtoDW(float f)
{
	return *((DWORD*)&f);
}

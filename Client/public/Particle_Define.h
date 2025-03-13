#pragma once

#include <d3d9.h>
#include <d3dx9.h>

struct Particle
{
	D3DXVECTOR3	_Position;
	D3DCOLOR	_Color;
	float		_Size;
	static const DWORD FVF;
};


struct Attribute 
{
	D3DXVECTOR3 _Position = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 _Velocity = { 0.0f, -5.0f, 0.0f };
	D3DXVECTOR3 _Accelerator = { 0.0f, 0.0f, 0.0f };

	float		_LifeTime = 1.0f;   // 수명
	float		_Age = 0.0f;        // 나이
	float		_Size = 5.0f;		// 사이즈
	bool		_isAlive = false;   // 기본적으로 죽은 상태

	D3DXCOLOR _Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	D3DXCOLOR _ColorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

};


struct BoundingBox
{
	const float INFINTY = FLT_MAX;
	const float EPSILON = 0.001f;

	BoundingBox()
	{
		_max.x = 10.0f;  // 최대한 큰 값
		_max.y = 10.0f;
		_max.z = 10.0f;

		_min.x = 0.0f;  // 최대한 작은 값
		_min.y = 0.0f;
		_min.z = 0.0f;
	}

	bool isPointInside(D3DXVECTOR3& p)
	{
		if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
			p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
		{
			return true;
		}
		else
			return false;
	}

	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;

	
	BoundingBox& operator=(const BoundingBox& other) 
	{
		if (this == &other)  // 자기 자신 대입 방지
			return *this;

		_min = other._min;
		_max = other._max;
		return *this;
	}
};

DWORD FtoDW(float f);

int GetRandomInt(int lowBound, int highBound);
float GetRandomFloat(float lowBound, float highBound);
void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);

#pragma region Color

const D3DXCOLOR RANDOM(D3DCOLOR_XRGB(GetRandomInt(1,255), GetRandomInt(1, 255), GetRandomInt(1, 255)));

const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));

const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

#pragma endregion

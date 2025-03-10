#pragma once

BEGIN(Engine)

typedef struct ENGINE_DLL tagVector : public D3DXVECTOR3
{
	// 기본 생성자 호출 시 모든 값을 0으로 초기화 합니다.
	tagVector();
	tagVector(CONST FLOAT* _p) : D3DXVECTOR3(_p) {}
	tagVector(CONST D3DVECTOR& _rhs) : D3DXVECTOR3(_rhs) {}
	tagVector(CONST D3DXFLOAT16* _p) : D3DXVECTOR3(_p) {}
	tagVector(FLOAT x, FLOAT y, FLOAT z) : D3DXVECTOR3(x, y, z) {}

	// 정규벡터를 만들어준다.
	void Normalize();
	// 길이를 구해준다
	FLOAT Length();
	// 벡터간 거리를 구해준다
	FLOAT Distance(const D3DXVECTOR3& _vector);
	// 매개변수와의 내적값을 구해준다.
	FLOAT Dot(const D3DXVECTOR3& _vector);
	// 매개변수와의 외적값을 구해준다.
	tagVector* Cross(const D3DXVECTOR3& _vector);
}VECTOR, *LPVECTOR;

END
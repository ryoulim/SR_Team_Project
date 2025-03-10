#pragma once

BEGIN(Engine)

typedef struct ENGINE_DLL tagVector : public D3DXVECTOR3
{
	// �⺻ ������ ȣ�� �� ��� ���� 0���� �ʱ�ȭ �մϴ�.
	tagVector();
	tagVector(CONST FLOAT* _p) : D3DXVECTOR3(_p) {}
	tagVector(CONST D3DVECTOR& _rhs) : D3DXVECTOR3(_rhs) {}
	tagVector(CONST D3DXFLOAT16* _p) : D3DXVECTOR3(_p) {}
	tagVector(FLOAT x, FLOAT y, FLOAT z) : D3DXVECTOR3(x, y, z) {}

	// ���Ժ��͸� ������ش�.
	const tagVector& Normalize();

	// ���̸� �����ش�
	FLOAT Length() const;

	// ���Ͱ� �Ÿ��� �����ش�
	FLOAT Distance(const D3DXVECTOR3& _vector) const;

	// �Ű��������� �������� �����ش�.
	FLOAT Dot(const D3DXVECTOR3& _vector) const;

	// �Ű��������� �������� �����ش�.
	tagVector Cross(const D3DXVECTOR3& _vector) const;

	// ���⺤�Ϳ� ��İ��� ����
	const tagVector& TransformNormal(const D3DXMATRIX& _matrix);

	// ��ġ���Ϳ� ��İ��� ����
	const tagVector& TransformCoord(const D3DXMATRIX& _matrix);

}VECTOR, *LPVECTOR;

END
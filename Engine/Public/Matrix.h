#pragma once

BEGIN(Engine)

typedef struct ENGINE_DLL tagMatrix : public D3DXMATRIX
{
    // 기본 생성자 호출 시 내부적으로 Identity 함수가 동작합니다.
    tagMatrix();
    tagMatrix(CONST FLOAT* _p) : D3DXMATRIX(_p) {}
    tagMatrix(CONST D3DMATRIX& _rhs) : D3DXMATRIX(_rhs) {}
    tagMatrix(CONST D3DXFLOAT16* _p) : D3DXMATRIX(_p) {}
    tagMatrix(FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
        FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
        FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
        FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44)
        :D3DXMATRIX(_11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44) {
    }

    //뷰 행렬용 생성자
    tagMatrix(const D3DXVECTOR3& _vCamera, const D3DXVECTOR3& _vLook, const D3DXVECTOR3& _vUp = { 0.f,1.f,0.f });

    //원근투영 행렬용 생성자
    tagMatrix(FLOAT _fFOV, FLOAT _fWinSizeX, FLOAT _fWinSizeY, FLOAT _fNear, FLOAT _fFar);

    //직교투영 행렬용 생성자
    tagMatrix(FLOAT _fWinSizeX, FLOAT _fWinSizeY, FLOAT _fNear, FLOAT _fFar);

    //회전행렬용 생성자
    tagMatrix(const D3DXVECTOR3& vector, FLOAT Angle);
    
    //뷰 행렬을 새로 계산해준다.
    const tagMatrix& MakeViewMat(const D3DXVECTOR3& _vCamera, const D3DXVECTOR3& _vLook, const D3DXVECTOR3& _vUp = { 0.f,1.f,0.f });

    //원근 투영행렬을 새로 계산해준다.
    const tagMatrix& MakePerspectiveProjMat(FLOAT _fFOV, FLOAT _fAspect, FLOAT _fNear, FLOAT _fFar);

    //직교투영 행렬용 새로 계산해준다.
    const tagMatrix& MakeOrthoProjMat(FLOAT _fWinSizeX, FLOAT _fWinSizeY, FLOAT _fNear, FLOAT _fFar);

    // 이 행렬을 매개변수의 역행렬로 만든다.
    const tagMatrix& MakeInverseMat(const D3DXMATRIX& Matrix);

    // 회전행렬을 새로 계산해준다.
    const tagMatrix& MakeRotationMat(const D3DXVECTOR3& vector, FLOAT Angle);

} MATRIX, WORLDMAT, VIEWMAT, PROJMAT, *LPMATRIX;

END
/* 1. �˽ѳ�(��). */
/* 1. �����ǽİ�.(��) */
/* 1. ��õ����.(��) */
/* 1. ���̻�, ���̻� */
/* 1. 5�� ȣ�λ���.(��) */
/* 1. ���¹����ڻ��� 1, 2 .(��) */
/* 1. �߰׸�����2��.(��) */
/* 1. ���ѻڶ���.(��) */
/* 1. ���չ��� ���̵�(����) */
/* 1. ����.(��) */
/* 1. �ڽ�����.(��, ��) */
/* 1. ���Ʈ��. */
/* 1. ������Ӿ�ī���� */
/* 1. ����� ���� */
/* 1. ����� Ż��. (���, �ְ� ������, �ְ� ī��, ) */ 

    // ���� �Լ�
    inline D3DXVECTOR3 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, float t)
    {
        return start + (end - start) * t;
    }


    inline D3DXMATRIX InterpolateMatrix(const D3DXMATRIX& mat1, const D3DXMATRIX& mat2, float t)
    {
        D3DXMATRIX result;
        D3DXMatrixIdentity(&result);

        // ��ġ ����
        D3DXVECTOR3 pos1(mat1._41, mat1._42, mat1._43);
        D3DXVECTOR3 pos2(mat2._41, mat2._42, mat2._43);
        D3DXVECTOR3 interpolatedPos = Lerp(pos1, pos2, t);

        // ��� ���� ����
        for (int i = 0; i < 3; ++i)  // X, Y, Z ��
        {
            result.m[i][0] = mat1.m[i][0] * (1.0f - t) + mat2.m[i][0] * t;
            result.m[i][1] = mat1.m[i][1] * (1.0f - t) + mat2.m[i][1] * t;
            result.m[i][2] = mat1.m[i][2] * (1.0f - t) + mat2.m[i][2] * t;
        }

        // Orthonormalization (����ȭ)
        D3DXVECTOR3 xAxis(result.m[0][0], result.m[0][1], result.m[0][2]);
        D3DXVECTOR3 yAxis(result.m[1][0], result.m[1][1], result.m[1][2]);
        D3DXVECTOR3 zAxis(result.m[2][0], result.m[2][1], result.m[2][2]);

        D3DXVec3Normalize(&xAxis, &xAxis);
        D3DXVec3Cross(&yAxis, &zAxis, &xAxis);
        D3DXVec3Normalize(&yAxis, &yAxis);
        D3DXVec3Cross(&zAxis, &xAxis, &yAxis);
        D3DXVec3Normalize(&zAxis, &zAxis);

        result.m[0][0] = xAxis.x; result.m[0][1] = xAxis.y; result.m[0][2] = xAxis.z;
        result.m[1][0] = yAxis.x; result.m[1][1] = yAxis.y; result.m[1][2] = yAxis.z;
        result.m[2][0] = zAxis.x; result.m[2][1] = zAxis.y; result.m[2][2] = zAxis.z;

        // ������ ��ġ ����
        result._41 = interpolatedPos.x;
        result._42 = interpolatedPos.y;
        result._43 = interpolatedPos.z;

        return result;
    }
}

HRESULT CItem_Model::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;


    // ���� ������ ����� ���Ϳ� ����
    Matrix mat = {};
    mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(70));
    mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(45));
    mat.Set_State(mat.STATE_POSITION, _float3(1.f, -0.5f, 1.2f));

    Matrix matrix = {};
    matrix.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(100));
    matrix.Set_State(matrix.STATE_POSITION, _float3(-1.5f, 0.2f, -0.3f));



    m_swing.push_back({ 0.f, mat });
    m_swing.push_back({ 0.3f, matrix * mat });

    return S_OK;

}

void CItem_Model::Update(_float fTimeDelta)
{
    Matrix        ViewMatrix = {};
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
   

    // ��ư Ŭ�� �ִϸ��̼�
    if (GetKeyState(VK_LBUTTON) & 0x8000)
    {
        fElapsedTime += fTimeDelta;
    }
    else
    {
        // �������� ������ ó�� ��ġ�� ����ٴ�
        m_pTransformCom->Set_Matrix(m_swing[0].matTransform * ViewMatrix);
    }  

    if (fElapsedTime >= m_swing.back().fTime)
    {
        fElapsedTime = 0.0f;  // ó������ ���
    }

    // Ű������ ã��
    KEYFREAME key1, key2;
    for (size_t i = 0; i < m_swing.size() - 1; ++i)
    {
        if (fElapsedTime >= m_swing[i].fTime && fElapsedTime <= m_swing[i + 1].fTime)
        {
            key1 = m_swing[i];
            key2 = m_swing[i + 1];
            break;
        }
    }

     // ���� ���� ��� (0~1 ���� ��)
    float t = (fElapsedTime - key1.fTime) / (key2.fTime - key1.fTime);

    // ��� ����
    D3DXMATRIX interpolatedMatrix = InterpolateMatrix(key1.matTransform, key2.matTransform, t);

    // ���� ��� ����
    m_pTransformCom->Set_Matrix(interpolatedMatrix * ViewMatrix);
}
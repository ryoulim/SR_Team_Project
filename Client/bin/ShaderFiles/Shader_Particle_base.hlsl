//float, float2, float3, float4 == vector
//float1x3, float2x3, float3x3, float4x4 = Matrix


matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture g_Texture;
texture g_CubeTexture;


sampler CubSampler = sampler_state
{
    //�ؽ�ó ť����ε�
    texture = g_CubeTexture;

    //�ε巯�� ������� -> Point �� ���� �ȼ��� �� ����
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

sampler DefaultSampler = sampler_state
{
    //�ؽ�ó ���ε�
    texture = g_Texture;

    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

/* �� ����������Ʈ�� �������� ������ ����������Ѵ� �� */

struct VS_IN
{
    //������ ��ǲ ����
    float3 vPosition : POSITIONT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    //������ �ƿ�ǲ ����
    float4 vPosition : POSITIONT;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

/* ����ü�ȿ� �ø�ƽ������ FVF���� �����ߴ��Ͱ� ������ �ִ� */
/* ����  vWorldPos �� ���Ƿ� �ؽ����� ��Ҵµ� � Ư�� ���Ͱ��� �޾ƿ��� ���� �� �ؽ����� �޾ƿ� �� �ִ�.*/

VS_OUT VS_MAIN(VS_IN IN)
{
    VS_OUT Out;
    
    /* ������ ��ȯ(����, ��, ����) */
    
    /* ���� ������ VS_OUT���� �������� */
    return Out;
}

/* ������� ������ �ڵ����� �Ǵ� �� */

/* 1. ���� ��������� ���� 3���� ���´� */
/* 2. w ������ ������ �Ϸ�ȴ� */
/* 3. ����Ʈ �����̽��� ��ȯ�ȴ� */
/* 4. �����Ͷ����� �Ϸ��Ѵ�(�ȼ� ����) */

/* -------------------------------------------------------- */


/* ���� �ȼ� ���̵��� �غ��� */

struct PS_IN
{
    //�ȼ��� ��ǲ ����
    float4 vPosition : POSITIONT;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    //�ȼ��� �ƿ�ǲ ����
    vector vColor : COLOR0;
};


//�ȼ� ���� ����
PS_OUT PS_MAIN(PS_IN IN)
{
    PS_OUT Out;
    
    //�ؽ�ó ���� �� ���� �������ϰ�
    Out.vColor = tex2D(DefaultSampler, IN.vTexcoord);
    Out.vColor.a = 0.5f;
    
    return Out;
}

PS_OUT PS_MAIN_RED(PS_IN IN)
{
    PS_OUT Out;
    
    //���������� ����
    Out.vColor = vector(1.f, 0.f, 0.f, 1.f);
    
    return Out;
}

//���÷��� �ؽ�ó�� ��� ������, ��� ���Ƿ��� ������ �ȼ��� ���� Ÿ�ٿ� ��� �׷�����
technique DefaultTechnique
{
    pass DefaultPass
    {
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        BlendOp = Add;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

    pass RedPass
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_RED();
    }
}
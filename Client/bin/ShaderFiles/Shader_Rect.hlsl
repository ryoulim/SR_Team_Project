
//float, float2, float3, float4 == vector
//float1x3, float2x3, float3x3, float4x4 = matrix
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
// 3�� 25�� ���� ���̴� 
texture g_Texture;
textureCUBE g_CubeTexture;

sampler CubeSampler = sampler_state
{
    texture = g_CubeTexture;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

sampler DefaultSampler = sampler_state
{
    texture = g_Texture;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

//VertexBuffer = ����0 ����1 ����2 ����3
//IndexBuffer = 0 1 2 0 2 3 



struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{    
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};


/* ������ ��ȯ(����, ��, ����) */
/* ������ ������ �ٲ۴�.  */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    /* ���� ������ ��ġ * ���� * �� * ���� */    
    vector      vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vWorldPos = vPosition;
    
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);        
    
    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;   
    
    return Out;   
}

/* ����3���� ���´� */
/* w�����⿬��. */
/* ����Ʈ��ȯ */
/* �����Ͷ����� -> �ȼ��������. */

struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

/* �ȼ��� ���� �����Ѵ�. */
// vector PS_MAIN(PS_IN In) : COLOR0


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;    
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
    //texCUBE(CubeSampler, In.vTexcoord);
    
    
    
    Out.vColor.gb = Out.vColor.r;
    
    if (Out.vColor.a < 0.1f)
        discard;        
    
    Out.vColor.a = 0.5f;
        
    return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = vector(1.f, 0.f, 0.f, 1.f);
        
    return Out;
}



technique DefaultTechnique
{
    pass DefaultPass
    { 

        aLPHAbLENDeNABLE = True;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        BlendOp = Add;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

    pass asdouble
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_RED();
    }
}
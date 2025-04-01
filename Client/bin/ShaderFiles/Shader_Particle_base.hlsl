//float, float2, float3, float4 == vector
//float1x3, float2x3, float3x3, float4x4 = Matrix


matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture g_Texture;
texture g_CubeTexture;


sampler CubSampler = sampler_state
{
    //텍스처 큐브바인딩
    texture = g_CubeTexture;

    //부드러운 보간방식 -> Point 를 쓰면 픽셀이 잘 보임
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

sampler DefaultSampler = sampler_state
{
    //텍스처 바인딩
    texture = g_Texture;

    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

/* ↑ 렌더스테이트의 설정들을 위에서 설정해줘야한다 ↑ */

struct VS_IN
{
    //정점의 인풋 정보
    float3 vPosition : POSITIONT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    //정점의 아웃풋 정보
    float4 vPosition : POSITIONT;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

/* 구조체안에 시멘틱순서는 FVF에서 선언했던것과 연관이 있다 */
/* 또한  vWorldPos 를 임의로 텍스쿠드로 잡았는데 어떤 특정 벡터값을 받아오고 싶을 때 텍스쿠드로 받아올 수 있다.*/

VS_OUT VS_MAIN(VS_IN IN)
{
    VS_OUT Out;
    
    /* 정점의 변환(월드, 뷰, 투영) */
    
    /* 계산된 정점을 VS_OUT으로 내보낸다 */
    return Out;
}

/* 여기까지 됐으면 자동으로 되는 일 */

/* 1. 면을 만들기위한 정점 3개가 나온다 */
/* 2. w 나누기 연산이 완료된다 */
/* 3. 뷰포트 스페이스로 변환된다 */
/* 4. 레스터라이즈 완료한다(픽셀 생성) */

/* -------------------------------------------------------- */


/* 이제 픽셀 셰이딩을 해보자 */

struct PS_IN
{
    //픽셀의 인풋 정보
    float4 vPosition : POSITIONT;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    //픽셀의 아웃풋 색상
    vector vColor : COLOR0;
};


//픽셀 연산 시작
PS_OUT PS_MAIN(PS_IN IN)
{
    PS_OUT Out;
    
    //텍스처 매핑 후 알파 반투명하게
    Out.vColor = tex2D(DefaultSampler, IN.vTexcoord);
    Out.vColor.a = 0.5f;
    
    return Out;
}

PS_OUT PS_MAIN_RED(PS_IN IN)
{
    PS_OUT Out;
    
    //빨간색으로 변경
    Out.vColor = vector(1.f, 0.f, 0.f, 1.f);
    
    return Out;
}

//샘플러는 텍스처를 어떻게 읽을지, 블록 안의렌더 설정은 픽셀이 렌더 타겟에 어떻게 그려질지
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
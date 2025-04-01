
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_Billboard;

texture g_Texture;

float2 g_vTexelSize;

bool g_bUseTexture;

float g_fBlurAmount, g_fLifeRatio, g_fEmissivePower, g_fTime;



sampler BlurSampler = sampler_state
{
    texture = g_Texture;

    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

sampler EmissiveSampler = sampler_state
{
    texture = g_Texture;

    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

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

/* [ 정점셰이더 ] */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    vector vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vWorldPos = vPosition;
    
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);
    
    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_IN_T
{
    float4 vPosition : POSITIONT;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : COLOR0;
};


/* [ 픽셀셰이더 ] 연기 */
PS_OUT PS_MAIN_SMOKE(PS_IN_T IN)
{
    PS_OUT Out;

    float2 offset = g_vTexelSize * g_fBlurAmount;
    float4 sum = float4(0, 0, 0, 0);

    // 3x3 커널 샘플링
    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x)
        {
            sum += tex2D(BlurSampler, IN.vTexcoord + float2(x, y) * offset);
        }
    }
    sum /= 9.0f;
    
    Out.vColor = sum;
    Out.vColor.a *= g_fLifeRatio;
    Out.vColor.a *= 0.3f;

    return Out;
}

/* [ 픽셀셰이더 ] 화염공격 */
PS_OUT PS_MAIN_FIREATTACK(PS_IN_T IN)
{
    PS_OUT Out;
    
    float4 texColor = tex2D(EmissiveSampler, IN.vTexcoord);

    // 중심 glow 계산
    float2 center = float2(0.5f, 0.5f);
    float dist = distance(IN.vTexcoord, center);
    float glow = pow(saturate(1.0 - dist * 2.2), 2.0); // 중심에 집중

    // 시간 기반 깜빡임 (펄스)
    float pulse = sin(g_fTime * 20.0f) * 0.5 + 0.5;

    // 오렌지빛 발광 추가
    float3 fireColor = float3(1.2, 0.7, 0.3); // 주황~노랑 계열 강화
    float3 finalColor = texColor.rgb + fireColor * glow * pulse * g_fEmissivePower;

    // 검정 배경 제거 (자동 알파)
    if (texColor.r + texColor.g + texColor.b < 0.1f)
        texColor.a = 0.0f;

    // 최종 출력
    Out.vColor = float4(finalColor, texColor.a * glow);
    
    return Out;
}

/* [ 픽셀셰이더 ] 미사일공격 */
PS_OUT PS_MAIN_BOSSMISSILE(PS_IN IN)
{
    PS_OUT Out;
    
    float4 texColor = tex2D(EmissiveSampler, IN.vTexcoord);

    float2 center = float2(0.5, 0.5);
    float dist = distance(IN.vTexcoord, center);

// 중심 강조를 pow()로 강화
    float glow = pow(saturate(1.0 - dist * 2.5), 2.0);

// 시간 기반 펄스
    float Speed = 200.f;
    float pulse = sin(g_fTime * Speed) * 0.5 + 0.5;
    float enhancedPulse = pow(pulse, 0.3);

// 발광 색 (선택적으로 강조)
    float3 glowColor = float3(1.2, 1.0, 0.6);

// 색 조합
    float3 finalColor = texColor.rgb + glowColor * glow * enhancedPulse * 1.5;

    Out.vColor = float4(finalColor, texColor.a * glow);
    
    return Out;
}

/* [ 픽셀셰이더 ] 디버깅용 */
PS_OUT PS_MAIN_DEBUG(PS_IN IN)
{
    PS_OUT Out;
    
    float pulse = sin(g_fTime * 10.0f) * 0.5f + 0.5f;

    // 깜빡임 시각화: 빨간색 강도 = pulse
    Out.vColor = float4(pulse, 0.0, 0.0, 1.0);
    
    return Out;
}


technique DefaultTechnique
{
    pass SMOKEPass
    {
        PixelShader = compile ps_3_0 PS_MAIN_SMOKE();
    }

    pass BOSSMISSILEPass
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_BOSSMISSILE();
    }

    pass DebugPass
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_DEBUG();
    }

    pass FIREATTACKPass
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_FIREATTACK();
    }
}
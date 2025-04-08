/* [ 정점셰이더를 위한 행렬 계산변수 ] */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_Billboard;

/* [ 픽셀셰이더를 위한 변수 ] */
float g_fBlurAmount, g_fLifeRatio, g_fEmissivePower, g_fTime, g_fOpacity;

/* [ 텍스처 바인딩 변수 ] */
texture g_Texture, g_Paricle;
float2 g_vTexelSize;

bool g_bUseTexture;

/* [ 안개 셰이더를 위한 변수 ] */
float3 g_CameraPos;
float g_FogStart;
float g_FogEnd;
float4 g_FogColor;

/* [ 번개를 위한 변수 ] */
float3 g_LightningPos;
float g_LightRange;
float g_FlashIntensity;

sampler LinearSampler = sampler_state
{
    texture = g_Texture;

    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

sampler ParticleSampler = sampler_state
{
    texture = g_Paricle;

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

struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_IN_PARTICLE
{
    float2 vTexcoord : TEXCOORD0;
    float4 vColor : COLOR0;
};

struct PS_OUT
{
    vector vColor : COLOR0;
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

/* [ 파티클 정점 ] */
struct VS_PARTICLE_IN
{
    float4 Pos : POSITION;
    float4 Color : COLOR0;
    float2 UV : TEXCOORD0;
};

struct VS_PARTICLE_OUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR0;
    float2 UV : TEXCOORD0;
};

/* [ 파티클 정점셰이더 ] */
VS_PARTICLE_OUT VS_MAIN_PARTICLE(VS_PARTICLE_IN input)
{
    VS_PARTICLE_OUT output;
    output.Pos = mul(input.Pos, g_ViewMatrix); // ViewProj만 곱해도 됨
    output.Color = input.Color;
    output.UV = input.UV; // 그대로 넘김!
    return output;
    
}

/* [ 픽셀셰이더 ] 연기 */
PS_OUT PS_MAIN_SMOKE(PS_IN_PARTICLE IN)
{
    PS_OUT Out;
    
    float2 offset = g_vTexelSize * g_fBlurAmount;
    float4 sum = float4(0, 0, 0, 0);
    
    // 3x3 커널 샘플링
    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x)
        {
            sum += tex2D(ParticleSampler, IN.vTexcoord + float2(x, y) * offset);
        }
    }
    sum /= 9.0f;
    
    Out.vColor = sum;
    //Out.vColor.a *= g_fLifeRatio;
    Out.vColor.a *= 0.1f;

    return Out;
}

/* [ 픽셀셰이더 ] 화염공격 */
PS_OUT PS_MAIN_FIREATTACK(PS_IN_PARTICLE IN)
{
    PS_OUT Out;
    
    float4 texColor = tex2D(LinearSampler, IN.vTexcoord);

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
    
    float4 texColor = tex2D(LinearSampler, IN.vTexcoord);

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
    
    Out.vColor = float4(1, 0, 0, 1); // 빨간색으로 출력
    //Out.vColor = tex2D(ParticleSampler, IN.vTexcoord);
    //Out.vColor = float4(IN.vTexcoord.xy, 0.0f, 1.0f); 
    return Out;
}

/* [ 픽셀셰이더 ] 피격용 */
PS_OUT PS_MAIN_HIT(PS_IN IN)
{
    PS_OUT Out;
    
    float4 texColor = tex2D(LinearSampler, IN.vTexcoord);

// 알파 보정
    texColor.a = saturate(texColor.a * 1.5f);

// 색상 강조 (약간만)
    texColor.rgb *= 1.2f;

// 외부에서 제어하는 오파시티와 곱합
    texColor.a *= g_fOpacity;

    Out.vColor = texColor;
    
    return Out;
}

/* [ 픽셀셰이더 ] 회복용 */
PS_OUT PS_MAIN_HEAL(PS_IN IN)
{
    PS_OUT Out;
    
    float4 texColor = tex2D(LinearSampler, IN.vTexcoord);
    
    texColor.a *= g_fOpacity;

    Out.vColor = texColor;
    
    return Out;
}

/* [ 픽셀셰이더 ] 워터용 */
PS_OUT PS_MAIN_WATER(PS_IN_PARTICLE IN)
{
    PS_OUT Out;

    // 기본 물 색상 (딥블루)
    float4 waterColor = float4(0.2f, 0.5f, 0.9f, 1.0f);

    // 중심 기준 거리 계산
    float2 center = float2(0.5f, 0.5f);
    float dist = distance(IN.vTexcoord, center);

    // Glow 값 계산 (0~1)
    float glow = saturate(1.0f - dist * 2.0f); // 중앙 밝고, 외곽 어둡게

    // Glow를 색과 알파에 곱하기
    waterColor.rgb *= 1.2f * glow; // 중심 더 진하게
    waterColor.a *= glow; // 중심에서만 더 보이게

    // 수명 비례 알파
    waterColor.a *= saturate(g_fLifeRatio);

    Out.vColor = waterColor;
    return Out;
}

/* [ 안개 셰이딩을 위한 텍스처 저장슬룻 ] */
sampler2D g_DiffuseTex : register(s0);

float4 PS_MAIN_FOG(float2 Texcoord : TEXCOORD0, float3 WorldPos : TEXCOORD1) : COLOR
{
    float4 texColor = tex2D(LinearSampler, Texcoord);

    float dist = distance(g_CameraPos, WorldPos);
    float fogFactor = saturate((dist - g_FogStart) / (g_FogEnd - g_FogStart));

    float4 finalColor = lerp(texColor, g_FogColor, fogFactor);
    float4 MainColor = float4(finalColor.r, finalColor.g, finalColor.b, texColor.a);
    
    // 추가적인 라이트 연산
    float distToLightning = distance(WorldPos, g_LightningPos);
    float lightningFactor = saturate(1 - distToLightning / g_LightRange);
    float flash = abs(sin(g_fTime * 20)) * g_FlashIntensity;

    float3 flashColor = float3(0.486, 0.584, 0.918);
    MainColor.rgb += flashColor * lightningFactor * flash;
    
    return MainColor;

}


technique DefaultTechnique
{
    pass SMOKEPass
    {
        //VertexShader = compile vs_3_0 VS_MAIN_PARTICLE();
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

    pass HITPass
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_HIT();
    }

    pass HEALPass
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_HEAL();
    }

    pass WATERPass
    {
        PixelShader = compile ps_3_0 PS_MAIN_WATER();
    }

    pass FOGPass
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_FOG();
    }
}
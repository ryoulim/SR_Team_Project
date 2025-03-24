// shader.fx
texture Tex; // 텍스처 선언

sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};

float Time; // 누적 시간 값
float Threshold = 0.6; // 블룸 임계값
float GlitterIntensity = 0.5; // 글리터 강도
float Speed = 10.0; // 반짝임 속도 조정

float4 PS_BloomGlitterDynamic(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 color = tex2D(SamplerTex, texCoord);

    // 1. 블룸 효과: 밝은 영역만 추출
    float brightness = max(color.r, max(color.g, color.b)); // 픽셀 밝기 계산
    float4 bloom = (brightness > Threshold) ? color : float4(0.0, 0.0, 0.0, 0.0);

    // 2. 역동적인 글리터 효과: 빠르게 변하는 랜덤값 생성
    float glitter = frac(sin(dot(texCoord.xy, float2(12.9898, 78.233)) * Speed + Time) * 43758.5453);
    if (glitter > 0.7) // 특정 값 이상일 때 글리터 적용
    {
        bloom.rgb += float3(1.0, 0.8, 0.6) * glitter * GlitterIntensity; // 따뜻한 색상 추가
    }

    // 3. 블룸과 글리터 결합
    color.rgb += bloom.rgb;

    return color;
}

technique BloomGlitterDynamicEffect
{
    pass P0
    {
        PixelShader = compile ps_2_0 PS_BloomGlitterDynamic();
    }
}
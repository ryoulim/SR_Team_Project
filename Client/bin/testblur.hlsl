
texture Tex; // 텍스처 선언
sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};
float4 PS_Blur(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 color = float4(0, 0, 0, 0);

    // 블러 커널 크기 (필요에 따라 조정 가능)
    float offset = 0.05;

    // 주변 9개의 픽셀 샘플링
    color += tex2D(SamplerTex, texCoord + float2(-offset, -offset)); // 왼쪽 위
    color += tex2D(SamplerTex, texCoord + float2(0.0, -offset));    // 중앙 위
    color += tex2D(SamplerTex, texCoord + float2(offset, -offset)); // 오른쪽 위

    color += tex2D(SamplerTex, texCoord + float2(-offset, 0.0));    // 왼쪽 중앙
    color += tex2D(SamplerTex, texCoord);                          // 현재 픽셀
    color += tex2D(SamplerTex, texCoord + float2(offset, 0.0));    // 오른쪽 중앙

    color += tex2D(SamplerTex, texCoord + float2(-offset, offset)); // 왼쪽 아래
    color += tex2D(SamplerTex, texCoord + float2(0.0, offset));     // 중앙 아래
    color += tex2D(SamplerTex, texCoord + float2(offset, offset)); // 오른쪽 아래

    // 평균화 (9개의 샘플)
    color /= 9.0;

    return color;
}

technique BlurTechnique
{
    pass P0
    {
        PixelShader = compile ps_2_0 PS_Blur();
    }
}
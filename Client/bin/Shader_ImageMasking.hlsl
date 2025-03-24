// shader.fx
texture Tex; // 텍스처 선언

sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};

float maskingDistance = 0.1;

float4 PS(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 color = tex2D(SamplerTex, texCoord);

    // 텍스처 중앙 좌표
    float2 center = float2(0.5, 0.5);

    // 중심으로부터의 거리 계산
    float distance = length(texCoord - center);

    // 특정 반경 밖의 픽셀 제거
	if (distance > maskingDistance && color.r > 0.9 && color.g > 0.9 && color.b > 0.9)
    {
        // discard; // 흰색 픽셀 제거
		color.a = 0.5;
    }

    return color;
}


technique Technique1 {
    pass P0 {
        PixelShader = compile ps_2_0 PS();
    }
}

// shader.fx
texture Tex; // 텍스처 선언

sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};

float darknessFactor = 0.5; // 어둡게 만들기 위한 명암 감소 비율 (0.0 ~ 1.0)

float4 PS(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        // 배경 부분은 알파 값을 변경하지 않음
        return color;
    }
    // RGB를 darknessFactor로 곱하여 어둡게 함
    color.rgb *= darknessFactor;

    return color;
}

technique Technique1 {
    pass P0 {
        PixelShader = compile ps_2_0 PS();
    }
}

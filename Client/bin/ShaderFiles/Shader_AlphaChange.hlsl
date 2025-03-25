// shader.fx
texture Tex; // 텍스처 선언

sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};

float opacity; // 지정하고 싶은 alpha opacity 값 전달

float4 PS(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        // 배경 부분은 알파 값을 변경하지 않음
        return color;
    }

	color.a *= opacity; // alpha channel에 지정한 opacity 곱연산

    return color;
}
technique Technique1 {
    pass P0 {
        PixelShader = compile ps_2_0 PS();
    }
}

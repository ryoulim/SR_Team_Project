// shader.fx
texture Tex; // �ؽ�ó ����

sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};

float darknessFactor = 0.5; // ��Ӱ� ����� ���� ��� ���� ���� (0.0 ~ 1.0)

float4 PS(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        // ��� �κ��� ���� ���� �������� ����
        return color;
    }
    // RGB�� darknessFactor�� ���Ͽ� ��Ӱ� ��
    color.rgb *= darknessFactor;

    return color;
}

technique Technique1 {
    pass P0 {
        PixelShader = compile ps_2_0 PS();
    }
}

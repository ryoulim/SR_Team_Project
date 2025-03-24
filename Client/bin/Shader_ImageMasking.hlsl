// shader.fx
texture Tex; // �ؽ�ó ����

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

    // �ؽ�ó �߾� ��ǥ
    float2 center = float2(0.5, 0.5);

    // �߽����κ����� �Ÿ� ���
    float distance = length(texCoord - center);

    // Ư�� �ݰ� ���� �ȼ� ����
	if (distance > maskingDistance && color.r > 0.9 && color.g > 0.9 && color.b > 0.9)
    {
        // discard; // ��� �ȼ� ����
		color.a = 0.5;
    }

    return color;
}


technique Technique1 {
    pass P0 {
        PixelShader = compile ps_2_0 PS();
    }
}

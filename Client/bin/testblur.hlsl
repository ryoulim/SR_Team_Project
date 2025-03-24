
texture Tex; // �ؽ�ó ����
sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};
float4 PS_Blur(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 color = float4(0, 0, 0, 0);

    // �� Ŀ�� ũ�� (�ʿ信 ���� ���� ����)
    float offset = 0.05;

    // �ֺ� 9���� �ȼ� ���ø�
    color += tex2D(SamplerTex, texCoord + float2(-offset, -offset)); // ���� ��
    color += tex2D(SamplerTex, texCoord + float2(0.0, -offset));    // �߾� ��
    color += tex2D(SamplerTex, texCoord + float2(offset, -offset)); // ������ ��

    color += tex2D(SamplerTex, texCoord + float2(-offset, 0.0));    // ���� �߾�
    color += tex2D(SamplerTex, texCoord);                          // ���� �ȼ�
    color += tex2D(SamplerTex, texCoord + float2(offset, 0.0));    // ������ �߾�

    color += tex2D(SamplerTex, texCoord + float2(-offset, offset)); // ���� �Ʒ�
    color += tex2D(SamplerTex, texCoord + float2(0.0, offset));     // �߾� �Ʒ�
    color += tex2D(SamplerTex, texCoord + float2(offset, offset)); // ������ �Ʒ�

    // ���ȭ (9���� ����)
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
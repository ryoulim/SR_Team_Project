// shader.fx
texture Tex; // �ؽ�ó ����

sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};

float Time; // ���� �ð� ��
float Threshold = 0.6; // ��� �Ӱ谪
float GlitterIntensity = 0.5; // �۸��� ����
float Speed = 10.0; // ��¦�� �ӵ� ����

float4 PS_BloomGlitterDynamic(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 color = tex2D(SamplerTex, texCoord);

    // 1. ��� ȿ��: ���� ������ ����
    float brightness = max(color.r, max(color.g, color.b)); // �ȼ� ��� ���
    float4 bloom = (brightness > Threshold) ? color : float4(0.0, 0.0, 0.0, 0.0);

    // 2. �������� �۸��� ȿ��: ������ ���ϴ� ������ ����
    float glitter = frac(sin(dot(texCoord.xy, float2(12.9898, 78.233)) * Speed + Time) * 43758.5453);
    if (glitter > 0.7) // Ư�� �� �̻��� �� �۸��� ����
    {
        bloom.rgb += float3(1.0, 0.8, 0.6) * glitter * GlitterIntensity; // ������ ���� �߰�
    }

    // 3. ���� �۸��� ����
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
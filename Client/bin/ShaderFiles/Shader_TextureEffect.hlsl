texture Tex; // �ؽ�ó ����

sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};


// ColorPickingChange
float3 newColor = float3(1.f, 1.f, 1.f);
float3 origColor =  float3(60.0 / 255.0, 70.0 / 255.0, 160.0 / 255.0);

// ColorChange
float hueShift = 0.5; // ���ϴ� ���� �� (0.0 ~ 1.0 ����)

// Masking
float maskingDistance = 0.1f;

// AlphaChange
float opacity = 0.8f; // �����ϰ� ���� alpha opacity �� ����

// ShadeChange
float darknessFactor = 0.5f; // ��Ӱ� ����� ���� ��� ���� ���� (0.0 ~ 1.0)


float4 PS_AlphaChange(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        // ��� �κ��� ���� ���� �������� ����
        return color;
    }

	color.a *= opacity; // alpha channel�� ������ opacity ������

    return color;
}

float4 PS_ColorPickingChange(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) { 
        return color;
    }

    float3 originalColor = color.rgb;

    float diff = distance(originalColor, origColor);

    float threshold = 0.7;
    float blendFactor = saturate(1.0 - diff / threshold);

    float luminance = dot(originalColor, float3(0.299, 0.587, 0.114)); 
    float minRGB = min(originalColor.r, min(originalColor.g, originalColor.b));
    float maxRGB = max(originalColor.r, max(originalColor.g, originalColor.b));
    float saturation = (maxRGB - minRGB); 

    if (saturation < 0.05) { 
        return color; 
    }

    color.rgb = lerp(originalColor, newColor, blendFactor);

    return color;
}

float4 PS_ImageMasking(float2 texCoord : TEXCOORD0) : COLOR
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
		color.a = 0.3;
    }

    return color;
}

float4 PS_ShadeChange(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        // ��� �κ��� ���� ���� �������� ����
        return color;
    }
    // RGB�� darknessFactor�� ���Ͽ� ��Ӱ� ��
    color.rgb *= darknessFactor;

    return color;
}

float3 AdjustHue(float3 rgb, float hueShift)
{
    // RGB���� ���� �ִ�/�ּҰ� ���
    float maxRGB = max(rgb.r, max(rgb.g, rgb.b));
    float minRGB = min(rgb.r, min(rgb.g, rgb.b));
    float delta = maxRGB - minRGB;

    // ���� Hue ���
    float hue = 0.0;
    if (delta > 0.0) {
        if (maxRGB == rgb.r) {
            hue = (rgb.g - rgb.b) / delta;
        } else if (maxRGB == rgb.g) {
            hue = 2.0 + (rgb.b - rgb.r) / delta;
        } else {
            hue = 4.0 + (rgb.r - rgb.g) / delta;
        }
    }
    hue = frac(hue / 6.0 + hueShift); // ���� ���� ����

    // RGB ����
    float c = delta; // ä�� �� ��ü
    float x = c * (1.0 - abs(fmod(hue * 6.0, 2.0) - 1.0));
    float3 result;
    if (hue < 1.0 / 6.0) {
        result = float3(c, x, 0.0);
    } else if (hue < 2.0 / 6.0) {
        result = float3(x, c, 0.0);
    } else if (hue < 3.0 / 6.0) {
        result = float3(0.0, c, x);
    } else if (hue < 4.0 / 6.0) {
        result = float3(0.0, x, c);
    } else if (hue < 5.0 / 6.0) {
        result = float3(x, 0.0, c);
    } else {
        result = float3(c, 0.0, x);
    }

    // ��� ����
    float m = minRGB; // �ּ� ��� ����
    return result + m;
}

float4 PS_ColorChange(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        return color; // ���� �ȼ��� �״�� ����
    }

    color.rgb = AdjustHue(color.rgb, hueShift);

    return color;
}


technique Technique1 {
    pass P0 {
        PixelShader = compile ps_2_0 PS_ColorChange();
    }
	pass P1 {
        PixelShader = compile ps_2_0 PS_AlphaChange();
	}
	pass P2 {
        PixelShader = compile ps_2_0 PS_ImageMasking();
	}
	pass P3 {
        PixelShader = compile ps_2_0 PS_ShadeChange();
	}
}

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture Tex; // 텍스처 선언
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
float hueShift = 0.5; // 원하는 색조 값 (0.0 ~ 1.0 범위)

// AlphaChange & Sandevistan(alpha + color)
float opacity = 0.8f; // 지정하고 싶은 alpha opacity 값 전달


float3 AdjustHue(float3 rgb, float hueShift)
{
    // RGB에서 색상 최대/최소값 계산
    float maxRGB = max(rgb.r, max(rgb.g, rgb.b));
    float minRGB = min(rgb.r, min(rgb.g, rgb.b));
    float delta = maxRGB - minRGB;

    // 원본 Hue 계산
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
    hue = frac(hue / 6.0 + hueShift); // 색조 변경 적용

    // RGB 재계산
    float c = delta; // 채도 값 대체
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

    // 밝기 조정
    float m = minRGB; // 최소 밝기 유지
    return result + m;
}


technique Technique1 {
    pass P0 {
        PixelShader = compile ps_2_0 PS_ColorChange();
    }
	pass P1 {
        PixelShader = compile ps_2_0 PS_AlphaChange();
	}

}

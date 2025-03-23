texture Tex; 

sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};

// 외부에서 설정할 색상 값
float3 newColor;
float3 origColor =  float3(60.0 / 255.0, 70.0 / 255.0, 160.0 / 255.0);

float4 PS(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) { 
        return color;
    }

    // 원본 색상 (RGB)
    float3 originalColor = color.rgb;

    // 파란색 기준값
    // 색상 차이 계산
    float diff = distance(originalColor, origColor);

    // 색상 차이가 작을수록 파란색으로 인식
    float threshold = 0.7;
    float blendFactor = saturate(1.0 - diff / threshold);

    // 회색(흰색) 보호 로직 추가
    float luminance = dot(originalColor, float3(0.299, 0.587, 0.114)); // 밝기 계산
    float minRGB = min(originalColor.r, min(originalColor.g, originalColor.b));
    float maxRGB = max(originalColor.r, max(originalColor.g, originalColor.b));
    float saturation = (maxRGB - minRGB); // 채도 값 계산

    if (saturation < 0.05) { 
        return color; // 채도가 낮아 회색~흰색이면 원본 유지
    }

    // 색상 변경 적용 (회색 제외)
    color.rgb = lerp(originalColor, newColor, blendFactor);

    return color;
}

technique Technique1 {
    pass P0 {
        PixelShader = compile ps_2_0 PS();
    }
}
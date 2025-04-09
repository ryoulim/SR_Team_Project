matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


texture Tex; // 텍스처 선언
sampler2D SamplerTex = sampler_state {
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
};

#ifdef MAKE_VS
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{    
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    /* 로컬 정점의 위치 * 월드 * 뷰 * 투영 */    
    vector      vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vWorldPos = vPosition;
    
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);        
    
    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;   
    
    return Out;   
}

struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

#endif

// ColorPickingChange
float3 newColor = float3(1.f, 1.f, 1.f);
float3 origColor =  float3(60.0 / 255.0, 70.0 / 255.0, 160.0 / 255.0);

// ColorChange
float hueShift = 0.5; // 원하는 색조 값 (0.0 ~ 1.0 범위)

// Masking_Center(AIM)
float maskingDistance = 0.1f;
// Masking_HPBar
float HPPercent = 0.5f;

// AlphaChange & Sandevistan(alpha + color)
float opacity = 0.8f; // 지정하고 싶은 alpha opacity 값 전달

// ShadeChange
float darknessFactor = 0.5f; // 어둡게 만들기 위한 명암 감소 비율 (0.0 ~ 1.0)

#ifdef MAKE_VS

PS_OUT PS_AlphaChangeWithVS(PS_IN In) : COLOR
{
    PS_OUT Out;  
    Out.vColor = tex2D(SamplerTex, In.vTexcoord);

    if (Out.vColor.a < 0.1) {
        // 배경 부분은 알파 값을 변경하지 않음
        return Out.vColor;
    }

	Out.vColor.a *= opacity; // alpha channel에 지정한 opacity 곱연산

    return Out.vColor;
}
#endif

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

float4 PS_AlphaChange(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        // 배경 부분은 알파 값을 변경하지 않음
        return color;
    }

	color.a *= opacity; // alpha channel에 지정한 opacity 곱연산

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

float4 PS_ImageMasking_Center(float2 texCoord : TEXCOORD0) : COLOR
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
        color.a = 0.3;
    }

    return color;
}

float4 PS_ImageMasking_HPBar(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 color = tex2D(SamplerTex, texCoord);

    // 중심으로부터의 거리 계산
    float distance = length(texCoord.x - 0);
    float hue = 0;
    // 특정 반경 밖의 픽셀 제거
    if (distance >= HPPercent)
    {
        discard; 
    }
    if (HPPercent <= 0.2f && HPPercent > 0.f)
        hue = 0.7;
    else if (HPPercent <= 0.4f && HPPercent > 0.2f)
        hue = 0.8;

    color.rgb = AdjustHue(color.rgb, hue);
    color.a *= 0.9f;

    return color;
}

float4 PS_ShadeChange(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        // 배경 부분은 알파 값을 변경하지 않음
        return color;
    }
    // RGB를 darknessFactor로 곱하여 어둡게 함
    color.rgb *= darknessFactor;

    return color;
}

float4 PS_ColorChange(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        return color; // 투명 픽셀은 그대로 유지
    }

    color.rgb = AdjustHue(color.rgb, hueShift);

    return color;
}

float4 PS_Sandevistan(float2 texCoord : TEXCOORD0) : COLOR {
    float4 color = tex2D(SamplerTex, texCoord);

    if (color.a < 0.1) {
        // 배경 부분은 알파 값을 변경하지 않음
        return color;
    }

    color.rgb = AdjustHue(color.rgb, opacity);

	color.a *= opacity; // alpha channel에 지정한 opacity 곱연산

    return color;
}


technique Technique1 {
    pass P0 {
        #ifdef MAKE_VS
        VertexShader = compile vs_3_0 VS_MAIN();
        #endif
        PixelShader = compile ps_2_0 PS_ColorChange();
    }
	pass P1 {
        #ifdef MAKE_VS
        VertexShader = compile vs_3_0 VS_MAIN();
        #endif
        PixelShader = compile ps_2_0 PS_AlphaChange();
	}
	pass P2 {
        #ifdef MAKE_VS
        VertexShader = compile vs_3_0 VS_MAIN();
        #endif
        PixelShader = compile ps_2_0 PS_ImageMasking_Center();
	}
	pass P3 {
        #ifdef MAKE_VS
        VertexShader = compile vs_3_0 VS_MAIN();
        #endif
        PixelShader = compile ps_2_0 PS_ShadeChange();
	}
	pass P4 {
        #ifdef MAKE_VS
        VertexShader = compile vs_3_0 VS_MAIN();
        #endif
        PixelShader = compile ps_2_0 PS_Sandevistan();
	}
    pass P5 {
        #ifdef MAKE_VS
        VertexShader = compile vs_3_0 VS_MAIN();
        #endif
        PixelShader = compile ps_2_0 PS_ImageMasking_HPBar();
    }
}

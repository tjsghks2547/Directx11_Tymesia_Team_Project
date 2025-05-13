#include "Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4 g_LightViewMatrix, g_LightProjMatrix;
float4x4 g_ViewMatrixInv, g_ProjMatrixInv;

float4 g_vLightDir, g_vLightPos;
float g_fRange, g_fSpot;

float4 g_vLightDiffuse, g_vLightAmbient, g_vLightSpecular;

float4 g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
float4 g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

Texture2D g_Texture;
Texture2D g_DiffuseTexture;
Texture2D g_SpecularTexture;
Texture2D g_NormalTexture;
Texture2D g_ShadeTexture;
Texture2D g_DepthTexture;
Texture2D g_ShadowTexture;
texture2D g_FogTexture;
Texture2D g_OccusionTexture;
Texture2D g_LightShaftXTexture;
Texture2D g_LightShaftYTexture;

Texture2D g_WaterTexture;
Texture2D g_WaterDepthTexture;

Texture2D g_MtrlSpecular;
Texture2D g_RoughnessTexture;
Texture2D g_EmissiveTexture;

Texture2D g_FinalTexture;

Texture2D g_HighLightTexture;
Texture2D g_HighLightXTexture;
Texture2D g_HighLightYTexture;

Texture2D g_DistortionTexture;
Texture2D g_MotionBlurTexture;
Texture2D g_GlowBeginTexture;
Texture2D g_GlowXTexture;
Texture2D g_GlowYTexture;

Texture2D g_WeightBlendTexture;

Texture2D g_VelocityTexture;

Texture2D g_BloomXTexture;
Texture2D g_BloomYTexture;
Texture2D g_BloomBeginTexture;



float4 g_vCamPosition;

float g_fViewPortWidth, g_fViewPortHeight;

float4 g_LightShaftValue;

float4 g_ScreenLightDir;
float4 g_ScreenLightPos;
float4 g_ScreenCameraDir;
float4 g_LightViewProjDir;

float4x4 g_PreFrameViewInvMatrix; // 이전 프레임의 뷰 매트릭스 
float4x4 g_CurFrameViewMatrix, g_CurFrameProjMatrix; // 현재 프레임의 뷰 매트릭스와 투영 매트릭스 

Texture2D g_PreFrameDepthTexture;

bool g_bMotionBlurOnOff;

//g_LightShaftValue . x == Density
//y == Decay
//z == Weight
//w == Exposure

/* FogFactors */

float g_FogRange;
float3 fFogFactor; //x : FogDensity , y : FogStartHeight, z : FogEndHeight
float2 fNoiseFactor; //x : NoiseScale, y : NoiseSpeed
float2 fHeightNoiseFactor;

Texture3D g_NoiseTexture;

float g_fTime;

//float4 psLightShaft(float2 texcoord)
//{
//    int NUM_SAMPLES = 128;
    
//    float2 DeltaTexCoord = g_ScreenLightDir;
    
//    DeltaTexCoord *= 1.f / (float) NUM_SAMPLES * g_LightShaftValue.x;
    
//    float4 vColor = g_OccusionTexture.Sample(LinearSampler, texcoord);
    
//    if (vColor.r <= 0.f)
//        vColor = float4(1.f, 1.f, 1.f, 1.f);
//    else
//        vColor = float4(0.f, 0.f, 0.f, 0.f);
    
//    float IlluminationDecay = 1.f;
    
//    for (int i = 0; i < NUM_SAMPLES; ++i)
//    {
//        texcoord -= DeltaTexCoord;
        
//        float4 Sample2 = g_OccusionTexture.Sample(LinearSampler, texcoord);
        
//        if (Sample2.r <= 0.f)
//            Sample2 = float4(1.f, 1.f, 1.f, 1.f);
//        else
//            Sample2 = float4(0.f, 0.f, 0.f, 0.f);
        
//        Sample2 *= IlluminationDecay * g_LightShaftValue.z;
        
//        vColor += Sample2;
        
//        IlluminationDecay *= g_LightShaftValue.y;
//    }
    
//    return saturate(vColor * g_LightShaftValue.w);
//}

/* 줌 블러 관련  */
Texture2D g_Final_Last_Texture;
bool g_bZoomBlurOnOff;  
float2 g_ZoomBlurCenter;
float g_bZoomBlurStrength;

bool  g_bReverseScreenEffect;
float g_ScreenReverseRadius;
bool  g_bReverseEnd;
float g_fReverseEndStrength;


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    return Out;
}


struct PS_OUT_LIGHT
{
    float4 vShade : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y;
	
    vector vShade = saturate(max(dot(normalize(g_vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
    Out.vShade = g_vLightDiffuse * vShade;
    
    vector vMtrlSpecular = g_MtrlSpecular.Sample(LinearSampler, In.vTexcoord);
    vector vRoughness = g_RoughnessTexture.Sample(LinearSampler, In.vTexcoord);
	
    vector vWorldPos;
	
   /* 투영공간상의 위치 */
   /* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
	
	
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
    vWorldPos = vWorldPos.xyzw * fViewZ;
	
	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	
	/* 월드위치 */
	/* 로컬위치 * 월드행렬  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    vector vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
    vector vLook = vWorldPos - g_vCamPosition; // 캠에서 해당 물체를 바라보는 look 방향을 구한거 .
	
    Out.vSpecular = ((g_vLightSpecular * vMtrlSpecular.r) * pow(max(dot(normalize(vReflect * -1.f), normalize(vLook)), 0.f), lerp(128.f, 4.f, vRoughness.r)));
    
    return Out;

}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    vector vMtrlSpecular = g_MtrlSpecular.Sample(LinearSampler, In.vTexcoord);
    vector vRoughness = g_RoughnessTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y;
    
    vector vShade;
    
    vShade = saturate(max(dot(normalize(g_vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
    
    vector vWorldPos;

	/* 투영공간상의 위치 */
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
    vWorldPos = vWorldPos.xyzw * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드위치 */
	/* 로컬위치 * 월드행렬  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLightDir = vWorldPos - g_vLightPos;
    float fLength = length(vLightDir);

    float fAtt = saturate((g_fRange - fLength) / g_fRange);

    Out.vShade = g_vLightDiffuse * vShade * fAtt;

    vector vReflect = reflect(normalize(vLightDir), normalize(vNormal));
    vector vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular = (g_vLightSpecular * vMtrlSpecular.r) * pow(max(dot(normalize(vReflect * -1.f), normalize(vLook)), 0.f), lerp(128.f, 4.f, vRoughness.r)) * fAtt;
    
    return Out;
}


PS_OUT_LIGHT PS_MAIN_SPOT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    vector vMtrlSpecular = g_MtrlSpecular.Sample(LinearSampler, In.vTexcoord);
    
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y;
    
    vector vShade;
    
    vShade = saturate(max(dot(normalize(g_vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
    
    vector vWorldPos;

	/* 투영공간상의 위치 */
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
    vWorldPos = vWorldPos.xyzw * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vLightDir = vWorldPos - g_vLightPos;
    
    float fLength = length(vLightDir);

    float fAtt = saturate((g_fRange - fLength) / g_fRange);
    
    float fAngle = pow(max(dot(normalize(-vLightDir), normalize(-g_vLightDir)), 0.f), g_fSpot);
    
    float Att = fAngle / dot(fAtt, float3(1.f, 0.f, g_fRange * g_fRange));
    
    Out.vShade = g_vLightDiffuse * vShade * fAngle * Att;
   
    vector vReflect = reflect(normalize(vLightDir), normalize(vNormal));
    vector vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular = (g_vLightSpecular * vMtrlSpecular) * pow(max(dot(normalize(vReflect * -1.f), normalize(vLook)), 0.f), 50.f) * fAtt;
    
    return Out;
}

PS_OUT PS_MAIN_FINAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexcoord);
    
    float fWeight = vDistortion.r * 1.2f;

    vector vFinal = g_FinalTexture.Sample(LinearSampler, In.vTexcoord + fWeight);
    
    vector vHighLight = g_HighLightYTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vGlow = g_GlowYTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vBloom = g_BloomYTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vGodRay = g_LightShaftYTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vWeightBlend = g_WeightBlendTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vVelocity = g_VelocityTexture.Sample(LinearSampler, In.vTexcoord);
    
    float fViewZ = vDepth.y;
	
    vector vWorldPos;
    
    /* 이 과정을 On Off 할 수 있도록 설정해야할 듯 함.  */
    if (g_bMotionBlurOnOff)
    {
    
	/* 모션 블러 과정 */ 
        int NumBlurSample = 200;
   
        vVelocity.xy /= (float) NumBlurSample;
    
        int iCnt = 1;
    
        float4 BColor;
    
        for (int i = iCnt; i < NumBlurSample; ++i)
        {
            BColor = g_FinalTexture.Sample(LinearSampler, In.vTexcoord + vVelocity.xy * (float) i);
        
            if (vVelocity.a < BColor.a + 0.04f)
            {
                iCnt++;
                vFinal.xyzw += BColor;
            }
        }
    
        vFinal.xyzw /= (float) iCnt;
    }
    
    Out.vColor = vFinal + vHighLight + vGlow + vBloom + vGodRay + vWeightBlend;
    
    return Out;
}

float g_fWeights_HighLight[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1.f, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

//float g_fWeights_HighLight[13] =
//{
//    0.00903f, 0.02178f, 0.04475f, 0.07842f, 0.11698f, 0.14866f, 0.16105f,
//    0.14866f, 0.11698f, 0.07842f, 0.04475f, 0.02178f, 0.00903f
//};

PS_OUT PS_MAIN_HIGHLIGHT_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -6; i < 7; i++)
    {
        
        //vTexcoord = float2(In.vTexcoord.x + (1.f / 1600.f) * i, In.vTexcoord.y);	
        vTexcoord = float2(In.vTexcoord.x + (1.f / g_fViewPortWidth) * i, In.vTexcoord.y);

        Out.vColor += g_fWeights_HighLight[i + 6] * g_HighLightTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }

    Out.vColor /= 6.f;

    return Out;
}

PS_OUT PS_MAIN_HIGHLIGHT_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -6; i < 7; i++)
    {
        //vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / 900.f) * i);
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / g_fViewPortHeight) * i);

        Out.vColor += g_fWeights_HighLight[i + 6] * g_HighLightXTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }

    Out.vColor /= 6.f;
    
    vector vHighLight = g_HighLightTexture.Sample(LinearSampler, In.vTexcoord); //원래 선환이가 기본 텍스쳐 + 비빈거 합치는거 여기에 다 넣어버리기->g_HighLight Y Texture 에 다들어감

    Out.vColor += vHighLight;
    
    return Out;
}

PS_OUT PS_MAIN_MOTIONBLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vMotionBlur = g_MotionBlurTexture.Sample(LinearSampler, In.vTexcoord); //속도 샘플링?
    
    float2 vTexcoord = 0.f;
    
    float Amount_X = (vMotionBlur.r / g_fViewPortWidth);
    float Amount_Y = (vMotionBlur.g / g_fViewPortHeight);
    
    vector vFinal_Color_MotionBlur;
    
    if (vMotionBlur.r > 0.5f || vMotionBlur.g > 0.5f)
    {  
        [unroll]
        for (int i = -6; i < 7; i++)
        {
            vTexcoord = float2(In.vTexcoord.x + (Amount_X * 8.f) * i, In.vTexcoord.y + (Amount_Y * 8.f) * i);
            Out.vColor += g_fWeights_HighLight[i + 6] * g_FinalTexture.Sample(LinearSampler, vTexcoord);
        }
    }
    
    Out.vColor /= 13.f;
    
    return Out;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (0.f == vDiffuse.a)
        discard;

    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    float4 vLight = float4(vShade.xyz, 1.f); // 빛 연산한후 a값 유지
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vDiffuse * vLight + vSpecular;
	
    return Out;
}

float g_fWeights_Glow[11] = { 0.019, 0.081, 0.136, 0.370, 0.604, 1.f, 0.604, 0.370, 0.136, 0.081, 0.019 };

PS_OUT PS_MAIN_GLOW_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -5; i < 6; i++)
    {
        
        //vTexcoord = float2(In.vTexcoord.x + (1.f / 1600.f) * i, In.vTexcoord.y);	
        vTexcoord = float2(In.vTexcoord.x + (1.f / g_fViewPortWidth) * i, In.vTexcoord.y);

        Out.vColor += g_fWeights_Glow[i + 5] * g_GlowBeginTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }

    Out.vColor /= 1.5f;

    return Out;
}

PS_OUT PS_MAIN_GLOW_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -5; i < 6; i++)
    {
        
        //vTexcoord = float2(In.vTexcoord.x + (1.f / 1600.f) * i, In.vTexcoord.y);	
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / g_fViewPortHeight) * i);

        Out.vColor += g_fWeights_Glow[i + 5] * g_GlowXTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }

    Out.vColor /= 1.5f;

    return Out;
}

float g_fWeights_GodRay[31] =
{
    0.0119, 0.0210, 0.0350, 0.0568, 0.0880, 0.1315, 0.1882, 0.2598, 0.3437, 0.4369, 0.5328, 0.6262, 0.7060, 0.7648, 0.7960, 1.f,
   0.7960, 0.7648, 0.7060, 0.6262, 0.5328, 0.4369, 0.3437, 0.2598, 0.1882, 0.1315, 0.0880, 0.0568, 0.0350, 0.0210, 0.0119
};

PS_OUT PS_MAIN_GODRAY_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 vTexcoord = 0.f;
    
    for (int i = -15; i < 16; i++)
    {
        vTexcoord = float2(In.vTexcoord.x + (1.f / g_fViewPortWidth) * i, In.vTexcoord.y);

        Out.vColor += g_fWeights_GodRay[i + 15] * g_OccusionTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }
    
    Out.vColor /= 15.f;

    return Out;
}

PS_OUT PS_MAIN_GODRAY_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -15; i < 16; i++)
    {
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / g_fViewPortHeight) * i);

        Out.vColor += g_fWeights_GodRay[i + 15] * g_LightShaftXTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }
    
    Out.vColor /= 15.f;
    
    return Out;
}

PS_OUT PS_OCCULSION(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    if (vDepth.x > 0.f)
    {
        Out.vColor = float4(0.f, 0.f, 0.f, 1.f);
        return Out;
    }
    float result = pow(((In.vTexcoord.x - g_ScreenLightPos.x) / (9.f / 16.f)), 2.f) + pow((In.vTexcoord.y - g_ScreenLightPos.y), 2.f);
    float radiusPow = pow(0.022f, 2.f);
    
    float4 vColor = float4(2.f, 2.f, 2.f, 1.f);
    
    float3 CameraDir = g_ScreenCameraDir.xyz;
    
    if (result < radiusPow)
    {
        Out.vColor = vColor;
    }
    else
    {
        Out.vColor = float4(0.f, 0.f, 0.f, 1.f);
    }
    
    return Out;
}

float Epsilon = 1e-5;
 
float3 RGBtoHCV(in float3 RGB)
{
    // Based on work by Sam Hocevar and Emil Persson
    float4 P = (RGB.g < RGB.b) ? float4(RGB.bg, -1.0, 2.0 / 3.0) : float4(RGB.gb, 0.0, -1.0 / 3.0);
    float4 Q = (RGB.r < P.x) ? float4(P.xyw, RGB.r) : float4(RGB.r, P.yzx);
    float C = Q.x - min(Q.w, Q.y);
    float H = abs((Q.w - Q.y) / (6 * C + Epsilon) + Q.z);
    return float3(H, C, Q.x);
}

float3 HUEtoRGB(in float H)
{
    H = frac(H);
    float R = abs(H * 6 - 3) - 1;
    float G = 2 - abs(H * 6 - 2);
    float B = 2 - abs(H * 6 - 4);
    return saturate(float3(R, G, B));
}

float3 RGBtoHSV(in float3 RGB)
{
    float3 HCV = RGBtoHCV(RGB);
    float S = (HCV.z > 0.0) ? (HCV.y / HCV.z) : 0.0;
    
    return float3(HCV.x, S, HCV.z);
}

float3 HSVtoRGB(in float3 HSV)
{
    float3 RGB = HUEtoRGB(HSV.x);
    return ((RGB - 1) * HSV.y + 1) * HSV.z;
}

float g_fWeights_Bloom[11] = { 0.045, 0.085, 0.2, 0.45, 0.75, 1.0, 0.75, 0.45, 0.2, 0.085, 0.045 };

PS_OUT PS_BLOOM_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float2 vTexcoord = 0.f;

    for (int i = -5; i < 6; i++)  
    {
        vTexcoord = float2(In.vTexcoord.x + (1.f / g_fViewPortWidth) * i, In.vTexcoord.y);
        float4 sampleColor = g_BloomBeginTexture.Sample(LinearSampler_Clamp, vTexcoord);

        float3 hsv = RGBtoHSV(sampleColor.rgb);

        if (hsv.z > 0.8f)  
        {
            hsv.z = min(hsv.z * 1.5f, 1.2f); 
            hsv.y = min(hsv.y * 1.2f, 1.0f); 
        }

        Out.vColor.rgb += g_fWeights_Bloom[i + 5] * HSVtoRGB(hsv);
    }
    
    Out.vColor /= 1.2f;
    return Out;
}

PS_OUT PS_BLOOM_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float2 vTexcoord = 0.f;

    for (int i = -5; i < 6; i++)
    {
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / g_fViewPortHeight) * i);
        float4 sampleColor = g_BloomXTexture.Sample(LinearSampler_Clamp, vTexcoord);
        
        float3 hsv = RGBtoHSV(sampleColor.rgb);
        
        float brightnessBoost = smoothstep(0.7, 1.0, hsv.z);
        hsv.z = min(hsv.z * (1.0 + brightnessBoost * 0.8), 1.5f);
        hsv.y *= (1.0 + brightnessBoost * 0.3);

        Out.vColor.rgb += g_fWeights_Bloom[i + 5] * HSVtoRGB(hsv);
    }

    Out.vColor /= 1.2f;
    return Out;
}



//sampler2D gTexSampler : register(s0);
//float2 gCenter; // 중심점 (보통 float2(0.5, 0.5))
//float gStrength; // 블러 강도 (샘플 간 거리)
//int gSampleCount; // 샘플 개수

PS_OUT PS_MAIN_ZOOM_BLUR(PS_IN In)
{
    
    PS_OUT Out = (PS_OUT) 0;

    
    int gSampleCount = 23;

    float2 Center = g_ZoomBlurCenter; // 명시( 반드시 float 쓸 것 )    
    
    float2 dir = Center - In.vTexcoord; /*float2(In.vTexcoord.x - Center.x, abs(Center.y - In.vTexcoord.y));*/ // 중심점에서 현재 픽셀까지의 방향       
    //dir.y += 0.25f;
    
    
    
    float dist = length(dir);
    
    
    //float strength = saturate(1.0 - dist); // 거리 멀수록 약하게
    
    
    if (g_bZoomBlurOnOff)
    {
    
        for (int i = 1; i < gSampleCount; ++i)
        {
            
            float strength = dist + 1.f; //saturate(1.0 - dist);
            float scale = i / (float) gSampleCount;
            float2 sampleUV = In.vTexcoord + dir * scale * strength * g_bZoomBlurStrength; //0.1f; // 0.1f 이 강도를 점점 쌔게 주면 될거같음  fTimeDelta로 줄까.
       

            Out.vColor += g_Final_Last_Texture.Sample(LinearSampler_Clamp, sampleUV);
        }
        
        
        Out.vColor /= gSampleCount;
        
        //if (dist >= 0.15f)
        //{
        //    for (int i = 1; i < gSampleCount; ++i)
        //    {
        //        float scale = i / (float) gSampleCount;
        //        float2 sampleUV = In.vTexcoord + dir * scale * g_bZoomBlurStrength; //0.1f; // 0.1f 이 강도를 점점 쌔게 주면 될거같음  fTimeDelta로 줄까.
       

        //        Out.vColor += g_Final_Last_Texture.Sample(LinearSampler_Clamp, sampleUV);
        //    }

        //    Out.vColor /= gSampleCount;
        //}
        //else
        //{
        //    Out.vColor = g_Final_Last_Texture.Sample(LinearSampler, In.vTexcoord);
        //}
    }
    else
    {
        Out.vColor = g_Final_Last_Texture.Sample(LinearSampler, In.vTexcoord);
    }
    
    
    
    if (g_bReverseScreenEffect)
    {
        float2 center = g_ZoomBlurCenter;
        float dist = distance(In.vTexcoord, center);
        
        if (g_ScreenReverseRadius >= dist)  
            Out.vColor.rgb = 1.0f - Out.vColor.rgb;
        
    }
    
    
    if (g_bReverseEnd)
    {
        
        
        float strength = saturate(g_fReverseEndStrength); // 0 → 정상 / 1 → 완전 반전 
        
        float3 inverted = g_Final_Last_Texture.Sample(LinearSampler, In.vTexcoord).rgb;
        
        Out.vColor.rgb = lerp(inverted, Out.vColor.rgb, strength);

    }
       
    return Out;
}





technique11 DefaultTechnique
{
    pass Debug //0 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
    }

    pass Directional //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend_Second, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
    }

    pass Point //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend_Second, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }


    pass Final //3 원래 Deferred
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FINAL();
    }

    pass HighLightX //4 원래 Blur X
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HIGHLIGHT_X();
    }

    pass HighLightY //5 원래 Blur Y 여기에 그냥 예전 Blur_Final을 해버림
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HIGHLIGHT_Y();
    }

    pass Deferred //6 원래 Final
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
    }

    pass GlowX //7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW_X();
    }

    pass GlowY //8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW_Y();
    }

    pass MotionBlur //8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MOTIONBLUR();
    }

    pass Spot //10
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SPOT();
    }

    pass GodRayX //11
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GODRAY_X();
    }


    pass GodRayY //12
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GODRAY_Y();
    }


    pass Occulsion //13
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_OCCULSION();
    }

    pass BloomX // 14
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BLOOM_X();
    }

    pass BloomY // 15
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BLOOM_Y();
    }


    pass Zoom_Blur // 16
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ZOOM_BLUR();
    }
}

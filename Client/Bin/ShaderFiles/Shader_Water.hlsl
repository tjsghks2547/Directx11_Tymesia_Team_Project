#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4x4 g_ReflectionView;

float g_fTime;
float g_WaveSpeed;
float g_WaveFrequency;
float g_WaveAmplitude;

float g_WaveHeight;

float4 g_vCamPosition;

float2 g_WindDirection;

Texture2D g_RefractionTexture;
Texture2D g_NormalTexture;
Texture2D g_ReflectionTexture;
Texture2D g_RippleTexture;

int g_FresnelMode;

float xDrawMode;

float g_dullBlendFactor;

float specPerturb;
float specPower;

struct Ripple
{
    float2 g_RippleCenter;
    float2 g_Ripplefactor;
};

StructuredBuffer<Ripple> g_Ripples;
int g_RippleCount;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : TEXCOORD0;
    float4 ReflectionMapSamplingPos : TEXCOORD1;
    float2 BumpMapSamplingPos : TEXCOORD2;
    float4 RefractionMapSamplingPos : TEXCOORD3;
    float4 wPosition : TEXCOORD4;
    float4 vProjPos : TEXCOORD5;
    float2 vNoiseSamplingPos : TEXCOORD6;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    float4 displacedPos = float4(In.vPosition, 1.f);
    
    //float dhdx = cos(displacedPos.x * g_WaveFrequency + g_fTime * g_WaveSpeed) * g_WaveFrequency * g_WaveAmplitude;
    //float dhdz = -sin(displacedPos.z * g_WaveFrequency + g_fTime * g_WaveFrequency) * g_WaveFrequency * g_WaveAmplitude;
    
    //float3 tangentX = normalize(float3(1, dhdx, 0));
    //float3 tangentZ = normalize(float3(0, dhdz, 1));
    //Out.vNormal = float4(normalize(cross(tangentZ, tangentX)), 1.f);
    
    float4x4 VP = mul(g_ViewMatrix, g_ProjMatrix);
    float4x4 WVP = mul(g_WorldMatrix, VP);
    float4x4 RVP = mul(g_ReflectionView, g_ProjMatrix);
    float4x4 WRVP = mul(g_WorldMatrix, RVP);
    
    Out.vPosition = mul(displacedPos, WVP);
    Out.wPosition = mul(displacedPos, g_WorldMatrix);
    Out.ReflectionMapSamplingPos = mul(displacedPos, WRVP);
    Out.RefractionMapSamplingPos = mul(displacedPos, WVP);
    
    float2 absoluteTexCoords = In.vTexcoord;
    float4 rotatedTexCoords = float4(absoluteTexCoords, 0.f, 1.f);
    float2 moveVector = float2(0.f, 1.f);
    
    Out.BumpMapSamplingPos = rotatedTexCoords.xy / g_WaveFrequency + (g_fTime * g_WaveSpeed) * g_WaveAmplitude * moveVector.xy;
    Out.vProjPos = Out.vPosition;
    Out.vNoiseSamplingPos = In.vTexcoord;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : TEXCOORD0;
    float4 ReflectionMapSamplingPos : TEXCOORD1;
    float2 BumpMapSamplingPos : TEXCOORD2;
    float4 RefractionMapSamplingPos : TEXCOORD3;
    float4 wPosition : TEXCOORD4;
    float4 vProjPos : TEXCOORD5;
    float2 vNoiseSamplingPos : TEXCOORD6;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 projectedTexCoords;
    projectedTexCoords.x = In.ReflectionMapSamplingPos.x / In.ReflectionMapSamplingPos.w / 2.0f + 0.5f;
    projectedTexCoords.y = -In.ReflectionMapSamplingPos.y / In.ReflectionMapSamplingPos.w / 2.0f + 0.5f;
 
    float2 bumpUV1 = In.BumpMapSamplingPos;
    
    float4 bumpColor1 = g_NormalTexture.Sample(LinearSampler, bumpUV1);
    
    float4 bumpColor = bumpColor1;

    float2 totalPerturb = float2(1.f, 1.f);
    //  Perturbating the color
    float2 baseperturbation = g_WaveHeight * (bumpColor.rg - 0.5f);
    
    for (int i = 0; i < g_RippleCount; ++i)
    {
        float2 diff = In.vNoiseSamplingPos - g_Ripples[i].g_RippleCenter.xy;
        float d = length(diff);
        
        if (d <= g_Ripples[i].g_Ripplefactor.y)
        {
            float2 TextureUV = (In.vNoiseSamplingPos.xy - float2(g_Ripples[i].g_RippleCenter.x - g_Ripples[i].g_Ripplefactor.y, g_Ripples[i].g_RippleCenter.y - g_Ripples[i].g_Ripplefactor.y)) / (g_Ripples[i].g_Ripplefactor.y * 2.f);
            
            float4 rippleColor = g_RippleTexture.Sample(LinearSampler, TextureUV);
            
            totalPerturb -= (rippleColor.r * (1.5f - g_Ripples[i].g_Ripplefactor.x)) * 5.f;
        }
    }
    
    float2 totalperturbation = baseperturbation * totalPerturb;
    
    //  Final texture coordinates
    float2 perturbatedTexCoords = projectedTexCoords + totalperturbation;
    
    float4 reflectiveColor = g_ReflectionTexture.Sample(LinearSampler, perturbatedTexCoords);

    float2 projectedRefrTexCoords;
    projectedRefrTexCoords.x = In.RefractionMapSamplingPos.x / In.RefractionMapSamplingPos.w / 2.0f + 0.5f;
    projectedRefrTexCoords.y = -In.RefractionMapSamplingPos.y / In.RefractionMapSamplingPos.w / 2.0f + 0.5f;
    float2 perturbatedRefrTexCoords = projectedRefrTexCoords + totalperturbation;
    
    float3 eyeVector = normalize(g_vCamPosition - In.wPosition).xyz;
    
    float3 normalVector = float3(0.f, 1.f, 0.f);
    float4 refractiveColor = g_RefractionTexture.Sample(LinearSampler, perturbatedRefrTexCoords);
   
    
    float fresnelTerm = (float) 0;

    if (g_FresnelMode == 1)
    {
        fresnelTerm = 0.02 + 0.97f * pow((1 - dot(eyeVector, normalVector)), 5);
    }
    else if (g_FresnelMode == 0)
    {
        fresnelTerm = 1 - dot(eyeVector, normalVector) * 1.3f;
    }
    else if (g_FresnelMode == 2)
    {
        float fangle = 1.0f + dot(eyeVector, normalVector);
        fangle = pow(fangle, 5);
        fresnelTerm = 1 / fangle;
    }
    else
    {
        fresnelTerm = 0.02 + 0.97f * pow((1 - dot(eyeVector, normalVector)), 5);
    }
    
    fresnelTerm = fresnelTerm * xDrawMode;
    
   // Out.vColor = float4(fresnelTerm, 0.f, 0.f, 1.f);
    fresnelTerm = saturate(fresnelTerm);

    //  Create the combined color
    float4 combinedColor = refractiveColor * (1 - fresnelTerm) + reflectiveColor * (fresnelTerm);
    
    float waveFactor = saturate(length(totalperturbation) * 5.0f); // tweak scale
    float brightness = lerp(0.5f, 1.0f, 1.0f - waveFactor);
    float4 dullColor = float4(0.5f, 0.f, 0.f, 1.0f);
	    
    float dullBlendFactor = g_dullBlendFactor;
    
    float dullBrightBlendFactor = g_dullBlendFactor * brightness;
    
    Out.vColor = (dullBrightBlendFactor * dullColor) + (1 - dullBlendFactor) * combinedColor;
    
    //float4 speccolor;

    //float3 lightSourceDir = normalize(float3(0.5f, 0.5f, 0.5f));

    //float3 halfvec = normalize(eyeVector + lightSourceDir + float3(totalperturbation.x * specPerturb, totalperturbation.y * specPerturb, 0));
    
    //float3 temp = 0.1f;

    //temp.x = pow(dot(halfvec, normalVector), specPower);
	
    //speccolor = float4(0.98, 0.97, 0.7, 0.6);
	
    //speccolor = speccolor * abs(temp.x);

    //speccolor = float4(speccolor.x * speccolor.w, speccolor.y * speccolor.w, speccolor.z * speccolor.w, 0.f);
    
    //Out.vColor += speccolor;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    return Out;
}


technique11 DefaultTechnique
{
    pass Water
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
   
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

}
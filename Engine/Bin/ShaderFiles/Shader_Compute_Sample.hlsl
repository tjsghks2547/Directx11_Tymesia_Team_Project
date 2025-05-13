#include "Engine_Shader_Defines.hlsli"

Texture2D<float4> g_OcculsionTexture : register(t0);
RWTexture2D<float4> g_OutputTexture : register(u0);

cbuffer LightSahftParams : register(b0)
{
    float2 g_ScreenLightPos;
    float4 g_LightShaftValue;
};

[numthreads(16, 16, 1)]
void CSMain_Sample(int3 dispatchThreadID : SV_DispatchThreadID)
{   
    int iNumSamples = 128;
    
    int2 Texcoord = dispatchThreadID.xy;
    float2 uv = Texcoord / float2(1600, 900);
    
    float4 vColor = g_OcculsionTexture.Load(int3(Texcoord, 0));
    
    float2 DeltaTexCoord = uv - g_ScreenLightPos.xy;
    DeltaTexCoord *= 1.f / (float) iNumSamples * g_LightShaftValue.xy;
    
    float IlluminationDecay = 1.f;
    
    for (int i = 0; i < iNumSamples; ++i)
    {
        uv -= DeltaTexCoord;
        
        int2 NewTexcoord = int2(uv * float2(1600, 900));
        
        float4 Sample2 = g_OcculsionTexture.Load(int3(NewTexcoord, 0));
        Sample2 *= IlluminationDecay * g_LightShaftValue.z;
        
        vColor += Sample2;
        IlluminationDecay *= g_LightShaftValue.y;
    }
    
    g_OutputTexture[Texcoord] = saturate(vColor * g_LightShaftValue.w);
}

technique11 DefaultTechnique
{
    pass SampleCompute
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Sample()));
    }
}
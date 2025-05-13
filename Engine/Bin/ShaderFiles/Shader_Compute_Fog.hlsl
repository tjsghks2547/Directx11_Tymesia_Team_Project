#include "Engine_Shader_Defines.hlsli"

Texture2D<float4> g_DepthTexture : register(t0);
Texture3D<float4> g_NoiseTexture : register(t1);
Texture2D<float4> g_GodRayTexture : register(t2);
Texture2D<float4> g_FinalTexture : register(t3);
Texture2D<float> g_RangeFogTexture : register(t4);
RWTexture2D<float4> g_OutputTexture : register(u0);
SamplerState g_LinearSampler : register(s0);

cbuffer FogParams : register(b0)
{
    float4x4 g_ProjMatrixInv;
    float4x4 g_ViewMatrixInv;
    
    float2 fHeightNoiseFactor;
    float2 fFogStartDistance; // x 거리 안개 거리 , y 높이 안개 거리
    
    float4 g_vCamPosition;
    float4 g_FogColor;
    float4 fFogFactor;
    float g_fTime;
};

[numthreads(16, 16, 1)]
void CSMain_Fog(int3 dispatchThreadID : SV_DispatchThreadID)
{
    int2 Texcoord = dispatchThreadID.xy;
    float2 uv = Texcoord / float2(1600, 900);
    
    float4 vDepth = g_DepthTexture.Load(int3(Texcoord, 0));
    float3 vGodRay = g_GodRayTexture.Load(int3(Texcoord, 0)).rgb;
    
    float fViewZ = vDepth.y;
	
    vector vWorldPos;

    vWorldPos.x = uv.x * 2.f - 1.f;
    vWorldPos.y = uv.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.f;
    
    vWorldPos = vWorldPos.xyzw * fViewZ; // w값으로 변환
    // 투영 행렬
    
    float fDistanceFogFactor = 1.f - saturate((1.f / 2.71828f) * pow((vWorldPos.z * fFogStartDistance.x), 2.f)); // 거리 안개 요소

    vWorldPos = mul(vWorldPos, g_ProjMatrixInv); // 뷰 행렬

    vWorldPos = mul(vWorldPos, g_ViewMatrixInv); // 월드 행렬
    
    vector FogColor = lerp(g_FogColor, vector(1.f, 1.f, 1.f, 1.f), length(vGodRay)); // godray에 따라 색 보간
    
    float fogFade = smoothstep(fFogStartDistance.y * 0.3f, fFogStartDistance.y * 1.5f, length(vWorldPos.xyz - g_vCamPosition.xyz)); // 높이 안개 사라지는 거리
    
    float fHeightFogFactor = 1.f - saturate((1.f / 2.71828f) * exp(-fFogFactor.x * (vWorldPos.y - fFogFactor.z))); // 높이 안개 요소
    
    /* 안개 노이즈 텍스쳐 잠시 삭제 */
    
    float fHeightNoiseFogFactor = smoothstep(fHeightNoiseFactor.x - 10.f, fHeightNoiseFactor.y + 10.f, vWorldPos.y); // 높이 안개 y값이 높아지면 강해지게 보간
    
    float fHeightNoise = (1.f - fHeightNoiseFogFactor) * 0.2f; // noise에 효과 적용
    
    float2 noiseFlow = float2(g_fTime, g_fTime); // 시간 흐름에 따라 흐르게 변경
    
    float scaleFactor = 0.02f; // noise 텍스쳐 크기 조절 (작을수록 noise 크게 그려짐)
    float3 scaledPos = (vWorldPos * scaleFactor).xyz; // 월드에 scalefactor조절
    
    float3 noiseUV = frac(abs(scaledPos) + float3(g_fTime * 0.1f, 0.0f, g_fTime * 0.1f)); // noise UV frac으로 조절
    
    //noiseUV = clamp(noiseUV, 0.f, 1.f);
    
    //float3 newTexCoord = float3((noiseUV) * 127.f + 0.5f);
    
   // float noise = g_NoiseTexture.Load(int4(newTexCoord, 0.f)).r; // noise에서 샘플링
    float noise = g_NoiseTexture.SampleLevel(g_LinearSampler, noiseUV, 0.f).r; // noise에서 샘플링
    noise = smoothstep(0.3, 0.7, noise); // 노이즈 선형 보간
    
    fHeightFogFactor += noise; // 노이즈 높이안개에만 적용
    
    float fFinalFogFactor = min(fDistanceFogFactor, fHeightFogFactor); // 거리가 멀땐 거리 안개, 거리가 가까울땐 높이 안개 적용(더 작은수 적용)
    fFinalFogFactor /= fogFade; // 높이안개 거리에 따라 사라지게 적용
    
    fFinalFogFactor = 1.f - smoothstep(0.f, 1.f, fFinalFogFactor); // 안개 선형 보간(필요할지 모르겠음?)
    
    float4 vColor = g_FinalTexture.Load(int3(Texcoord, 0.f));
    float fRangeFogfactor = g_RangeFogTexture.Load(int3(Texcoord, 0));
    
    fFinalFogFactor = max(fRangeFogfactor, fFinalFogFactor);
    
    if (vDepth.x <= 0.f)
        vColor = FogColor; // 깊이가 없으면 안개 최대로 적용 (물체가 그려지지않은곳들 메꾸기)
    else
        vColor = float4(lerp(vColor, FogColor, fFinalFogFactor)); // 현재 색깔과 안개 색 선형보간
 
    g_OutputTexture[Texcoord] = vColor;
}

technique11 DefaultTechnique
{
    pass SampleCompute
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Fog()));
    }
}
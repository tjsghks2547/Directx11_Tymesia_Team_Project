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
    float2 fFogStartDistance; // x �Ÿ� �Ȱ� �Ÿ� , y ���� �Ȱ� �Ÿ�
    
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
    
    vWorldPos = vWorldPos.xyzw * fViewZ; // w������ ��ȯ
    // ���� ���
    
    float fDistanceFogFactor = 1.f - saturate((1.f / 2.71828f) * pow((vWorldPos.z * fFogStartDistance.x), 2.f)); // �Ÿ� �Ȱ� ���

    vWorldPos = mul(vWorldPos, g_ProjMatrixInv); // �� ���

    vWorldPos = mul(vWorldPos, g_ViewMatrixInv); // ���� ���
    
    vector FogColor = lerp(g_FogColor, vector(1.f, 1.f, 1.f, 1.f), length(vGodRay)); // godray�� ���� �� ����
    
    float fogFade = smoothstep(fFogStartDistance.y * 0.3f, fFogStartDistance.y * 1.5f, length(vWorldPos.xyz - g_vCamPosition.xyz)); // ���� �Ȱ� ������� �Ÿ�
    
    float fHeightFogFactor = 1.f - saturate((1.f / 2.71828f) * exp(-fFogFactor.x * (vWorldPos.y - fFogFactor.z))); // ���� �Ȱ� ���
    
    /* �Ȱ� ������ �ؽ��� ��� ���� */
    
    float fHeightNoiseFogFactor = smoothstep(fHeightNoiseFactor.x - 10.f, fHeightNoiseFactor.y + 10.f, vWorldPos.y); // ���� �Ȱ� y���� �������� �������� ����
    
    float fHeightNoise = (1.f - fHeightNoiseFogFactor) * 0.2f; // noise�� ȿ�� ����
    
    float2 noiseFlow = float2(g_fTime, g_fTime); // �ð� �帧�� ���� �帣�� ����
    
    float scaleFactor = 0.02f; // noise �ؽ��� ũ�� ���� (�������� noise ũ�� �׷���)
    float3 scaledPos = (vWorldPos * scaleFactor).xyz; // ���忡 scalefactor����
    
    float3 noiseUV = frac(abs(scaledPos) + float3(g_fTime * 0.1f, 0.0f, g_fTime * 0.1f)); // noise UV frac���� ����
    
    //noiseUV = clamp(noiseUV, 0.f, 1.f);
    
    //float3 newTexCoord = float3((noiseUV) * 127.f + 0.5f);
    
   // float noise = g_NoiseTexture.Load(int4(newTexCoord, 0.f)).r; // noise���� ���ø�
    float noise = g_NoiseTexture.SampleLevel(g_LinearSampler, noiseUV, 0.f).r; // noise���� ���ø�
    noise = smoothstep(0.3, 0.7, noise); // ������ ���� ����
    
    fHeightFogFactor += noise; // ������ ���̾Ȱ����� ����
    
    float fFinalFogFactor = min(fDistanceFogFactor, fHeightFogFactor); // �Ÿ��� �ֶ� �Ÿ� �Ȱ�, �Ÿ��� ����ﶩ ���� �Ȱ� ����(�� ������ ����)
    fFinalFogFactor /= fogFade; // ���̾Ȱ� �Ÿ��� ���� ������� ����
    
    fFinalFogFactor = 1.f - smoothstep(0.f, 1.f, fFinalFogFactor); // �Ȱ� ���� ����(�ʿ����� �𸣰���?)
    
    float4 vColor = g_FinalTexture.Load(int3(Texcoord, 0.f));
    float fRangeFogfactor = g_RangeFogTexture.Load(int3(Texcoord, 0));
    
    fFinalFogFactor = max(fRangeFogfactor, fFinalFogFactor);
    
    if (vDepth.x <= 0.f)
        vColor = FogColor; // ���̰� ������ �Ȱ� �ִ�� ���� (��ü�� �׷������������� �޲ٱ�)
    else
        vColor = float4(lerp(vColor, FogColor, fFinalFogFactor)); // ���� ����� �Ȱ� �� ��������
 
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
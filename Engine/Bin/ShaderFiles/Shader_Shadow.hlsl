#include "Engine_Shader_Defines.hlsli"


float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4 g_ViewMatrixInv, g_ProjMatrixInv;
Texture2DArray g_CascadeShadowMapTexture;
Texture2D g_FinalTexture;
Texture2D g_DepthTexture;

matrix g_lightTransform[3];

matrix g_lightviewmatrix[3];
matrix g_lightprojmatrix[3];

float g_LightEndClipSpaceZ1;
float g_LightEndClipSpaceZ2;
float g_LightEndClipSpaceZ3;

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
float CalcCascadeShadowFactor(int iCasecaseIndex, float4 lightspacepos)
{
    float3 projCoords = lightspacepos.xyz / lightspacepos.w;
    projCoords.x = projCoords.x * 0.5f + 0.5f;
    projCoords.y = -projCoords.y * 0.5f + 0.5f;
    if (projCoords.z > 1.0)
        return 0.0f;
    
    float currentDepth = projCoords.z;
    float bias = 0.01f;
    float shadow = 0.0;

    float3 samplePos = projCoords;
    samplePos.z = iCasecaseIndex;
    
	[unroll]
    for (int x = -1; x <= 1; ++x)
    {
        [unroll]
        for (int y = -1; y <= 1; ++y)
        {
            shadow += g_CascadeShadowMapTexture.SampleCmpLevelZero(ShadowSampler, samplePos, currentDepth - bias, int2(x, y));
        }
    }
    shadow /= 9.0f;
    return shadow;  
}



cbuffer ConstantBuffer : register(b0)
{
    matrix lightviewmatrix[3];
    matrix lightprojmatrix[3];
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vColor = g_FinalTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vWorldPos;
    
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    float LightEndClipSpace[3];
    LightEndClipSpace[0] = g_LightEndClipSpaceZ1;
    LightEndClipSpace[1] = g_LightEndClipSpaceZ2;
    LightEndClipSpace[2] = g_LightEndClipSpaceZ3;
  
	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드위치 */
	/* 로컬위치 * 월드행렬  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    float4 LightPos[3];
    
    for (int i = 0; i < 3; ++i)
    {
        LightPos[i] = mul(vWorldPos, g_lightprojmatrix[i]);
    }
    
    //float fShadowFactor[3];
    float fShadow = { 0.f }; //,   0.f, 0.f};
    
    [unroll]
    for (int d = 0; d < 3; ++d)
    {
        if (vDepthDesc.x <= LightEndClipSpace[d])
        {
            float s = CalcCascadeShadowFactor(d, LightPos[d]);
            fShadow = max(fShadow, s); // 3가지의 그림자 값 중 가장 큰수 적용
        }
    }
    
    fShadow /= 3.f;
    
    vector vShadow = vector(0.f, 0.f, 0.f, 0.f);
    
    Out.vColor = lerp(vColor, vShadow, fShadow);
    
    return Out;
}


technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(Rs_Depth_Bias);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = null;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}

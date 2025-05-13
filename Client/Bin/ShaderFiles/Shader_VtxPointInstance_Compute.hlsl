
#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
Texture2D g_NoiseTexture;
float3 g_vRGB;
float4 g_vCamPosition;
float g_fAlpha_Amount;

struct VS_IN
{
    float3 vPosition : POSITION;
    float fPSize : PSIZE;

    row_major float4x4 TransformMatrix : WORLD;
    float2 vLifeTime : TEXCOORD0;
    float fSpeed : TEXCOORD1;
};


struct VS_OUT
{
    float4 vPosition : POSITION;
    float fPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    
    float3 vRight : TEXCOORD1;
    float3 vUp : TEXCOORD2;
    float3 vLook : TEXCOORD3S;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
	
	/* 로컬스페이스내에서 움직임을 가져간다. */
    vector vPosition = mul(vector(In.vPosition, 1.f), In.TransformMatrix);

    Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.fPSize = length(In.TransformMatrix._11_12_13);
    Out.vLifeTime = In.vLifeTime;
 
    Out.vRight = mul(In.TransformMatrix._11_12_13_14, g_WorldMatrix).xyz;
    Out.vUp = mul(In.TransformMatrix._21_22_23_24, g_WorldMatrix).xyz;
    Out.vLook = mul(In.TransformMatrix._31_32_33_34, g_WorldMatrix).xyz;
    
    return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
    float fPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    
    float3 vRight : TEXCOORD1;
    float3 vUp : TEXCOORD2;
    float3 vLook : TEXCOORD3S;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

struct GS_OUT_WEIGHT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};


/* Geometry Shader == 기하셰이더 */
/* */
[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
    GS_OUT Out[4];

    //float3 vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    //float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].fPSize * 0.5f);
    //float3 vUp = normalize(cross(vLook, vRight)) * (In[0].fPSize * 0.5f);

    float3 vRight = (In[0].vRight * (In[0].fPSize * 0.5f));
    float3 vUp = (In[0].vUp * (In[0].fPSize * 0.5f));
    float3 vLook = (In[0].vLook * (In[0].fPSize * 0.5f));
    
    float4x4 matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;

    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    //Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;

    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    //Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;

    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;

    DataStream.Append(Out[0]);
    DataStream.Append(Out[1]);
    DataStream.Append(Out[2]);
    DataStream.RestartStrip();

    DataStream.Append(Out[0]);
    DataStream.Append(Out[2]);
    DataStream.Append(Out[3]);
    DataStream.RestartStrip();
}

[maxvertexcount(6)]
void GS_MAIN_WEIGHT(point GS_IN In[1], inout TriangleStream<GS_OUT_WEIGHT> DataStream)
{
    GS_OUT_WEIGHT Out[4];

    //float3 vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    //float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].fPSize * 0.5f);
    //float3 vUp = normalize(cross(vLook, vRight)) * (In[0].fPSize * 0.5f);
    
    float3 vRight = (In[0].vRight * (In[0].fPSize * 0.5f));
    float3 vUp = (In[0].vUp * (In[0].fPSize * 0.5f));
    float3 vLook = (In[0].vLook * (In[0].fPSize * 0.5f));
    
    float4x4 matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    Out[0].vProjPos = Out[0].vPosition;

    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    Out[1].vProjPos = Out[1].vPosition;

    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    Out[2].vProjPos = Out[2].vPosition;

    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    Out[3].vProjPos = Out[3].vPosition;
    
    DataStream.Append(Out[0]);
    DataStream.Append(Out[1]);
    DataStream.Append(Out[2]);
    DataStream.RestartStrip();

    DataStream.Append(Out[0]);
    DataStream.Append(Out[2]);
    DataStream.Append(Out[3]);
    DataStream.RestartStrip();
}

[maxvertexcount(6)]
void GS_MAIN_BLOOD(point GS_IN In[1], inout TriangleStream<GS_OUT_WEIGHT> DataStream)
{
    GS_OUT_WEIGHT Out[4];

    float3 vLook = In[0].vLook * (In[0].fPSize * 0.5f);
    float fLength_Look = length(vLook);
    float3 vRight = In[0].vRight * (In[0].fPSize * 0.5f);
    float fLength_Right = length(vRight);
    float3 vUp = In[0].vUp * (In[0].fPSize * 0.5f);
    float fLength_Up = length(vUp);
    
    vLook = normalize(g_vCamPosition.xyz - In[0].vPosition.xyz) * fLength_Look;
    vUp = normalize(cross(vLook, vRight)) * fLength_Up;
    vRight = normalize(cross(vUp, vLook)) * fLength_Right;
    
    float4x4 matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    Out[0].vProjPos = Out[0].vPosition;
    
    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    //Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    Out[1].vProjPos = Out[1].vPosition;
    
    
    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    //Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    Out[2].vProjPos = Out[2].vPosition;
    
    
    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    Out[3].vProjPos = Out[3].vPosition;
    
    
    DataStream.Append(Out[0]);
    DataStream.Append(Out[1]);
    DataStream.Append(Out[2]);
    DataStream.RestartStrip();

    DataStream.Append(Out[0]);
    DataStream.Append(Out[2]);
    DataStream.Append(Out[3]);
    DataStream.RestartStrip();
}

[maxvertexcount(6)]
void GS_MAIN_DUST(point GS_IN In[1], inout TriangleStream<GS_OUT_WEIGHT> DataStream)
{
    GS_OUT_WEIGHT Out[4];

    float3 vLook = In[0].vLook * (In[0].fPSize * 0.5f);
    float fLength_Look = length(vLook);
    float3 vRight = In[0].vRight * (In[0].fPSize * 0.5f);
    float fLength_Right = length(vRight);
    float3 vUp = In[0].vUp * (In[0].fPSize * 0.5f);
    float fLength_Up = length(vUp);
    
    vLook = normalize(g_vCamPosition.xyz - In[0].vPosition.xyz) * fLength_Look;
    vRight = normalize(cross(vUp, vLook)) * fLength_Right;
    vUp = normalize(cross(vLook, vRight)) * fLength_Up;
    
    float4x4 matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    Out[0].vProjPos = Out[0].vPosition;

    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    Out[1].vProjPos = Out[1].vPosition;

    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    Out[2].vProjPos = Out[2].vPosition;

    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    Out[3].vProjPos = Out[3].vPosition;
    
    DataStream.Append(Out[0]);
    DataStream.Append(Out[1]);
    DataStream.Append(Out[2]);
    DataStream.RestartStrip();

    DataStream.Append(Out[0]);
    DataStream.Append(Out[2]);
    DataStream.Append(Out[3]);
    DataStream.RestartStrip();
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

struct PS_IN_WEIGHT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

struct PS_OUT_BLOOD
{
    float4 vColor :  SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth :  SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
   
  
    if (0.2f >= Out.vColor.a)
        discard;


    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;

    float3 vRGB = g_vRGB;
    Out.vColor *= vector(vRGB, 1.f);
    
    return Out;
}

PS_OUT PS_MAIN_WEIGHTBLEND(PS_IN_WEIGHT In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vResult = vDiffuse;
    if (vResult.a < 0.01f)
        discard;
    
    float3 vRGB = g_vRGB;
    vResult *= vector(vRGB, 1.f);
    
    float x = (In.vProjPos.z / In.vProjPos.w);
    float fWeight = saturate(max(1e-2, In.vProjPos.w / 70.f));
    
    //vResult.xyz /= clamp(vResult.a, 0.1f, 800.f);
    vResult.xyz *= fWeight;
    float fLifeTime = 1.f - (In.vLifeTime.y / In.vLifeTime.x);
    vResult.a *= fLifeTime;
    Out.vColor = vResult;
    
    return Out;
}

PS_OUT PS_MAIN_GLOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;
    
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float3 vRGB = g_vRGB;
    vDiffuse *= vector(vRGB, 1.f);
    vector vResult = vDiffuse;
    if (vResult.a < 0.01f)
        discard;
    
    Out.vColor = vResult;
    
    return Out;
}

PS_OUT_BLOOD PS_MAIN_BLOOD(PS_IN_WEIGHT In)
{
    PS_OUT_BLOOD Out = (PS_OUT_BLOOD) 0;

    if (In.vLifeTime.y >= In.vLifeTime.x || In.vLifeTime.y <= In.vTexcoord.x)
        discard;
    
    float4 vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (saturate(In.vLifeTime.y / In.vLifeTime.x) >= pow(vDiffuse.a - 0.1f, 3.f))
        discard;
    
    float2 vTexcoord = saturate(In.vTexcoord * (1.f - In.vLifeTime.y));
    
    float3 vRGB = g_vRGB;
    
    vDiffuse *= vector(vRGB, 1.f);
    
    if (0.2f >= vDiffuse.a)
        discard;
    
    Out.vColor = vDiffuse;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.vNormal = 1.f;

    return Out;
}

PS_OUT PS_MAIN_DUST(PS_IN_WEIGHT In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;
    
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vResult = vDiffuse;
    if (vResult.a < 0.01f)
        discard;
    
    float3 vRGB = g_vRGB;
    vResult.xyz *= vRGB;
    float fWeight = saturate(max(1e-2, In.vProjPos.w / (70.f * g_fAlpha_Amount)));
    
    //vResult.xyz /= clamp(vResult.a, 0.1f, 800.f);
    float fLifeTime = 1.f - pow(In.vLifeTime.y / In.vLifeTime.x, 2.f);
    vResult.xyz *= fWeight * fLifeTime;
    Out.vColor = vResult;
    
    return Out;
}

technique11 DefaultTechnique
{
    
    pass DefaultPass // 0 번 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass WeightBlend // 1 번 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_WeightBlend, 0);
        SetBlendState(BS_WeightBlend_Client, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_WEIGHT();
        PixelShader = compile ps_5_0 PS_MAIN_WEIGHTBLEND();
    }

    pass Glow // 2 번 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_WeightBlend_Client, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }

    pass Blood // 3 번 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_BLOOD();
        PixelShader = compile ps_5_0 PS_MAIN_BLOOD();
    }

    pass Bloom // 4 번 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_WeightBlend, 0);
        SetBlendState(BS_WeightBlend_Client, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_WEIGHT();
        PixelShader = compile ps_5_0 PS_MAIN_WEIGHTBLEND();
    }

    pass Dust // 5 번 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_WeightBlend, 0);
        SetBlendState(BS_WeightBlend_Client, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_DUST();
        PixelShader = compile ps_5_0 PS_MAIN_DUST();
    }

    pass World // 6 번 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_WeightBlend, 0);
        SetBlendState(BS_WeightBlend_Client, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_DUST();
        PixelShader = compile ps_5_0 PS_MAIN_DUST();
    }

    pass Blood_World // 7 번 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_BLOOD();
        PixelShader = compile ps_5_0 PS_MAIN_BLOOD();
    }
}

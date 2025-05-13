#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D		g_Texture;
Texture2D		g_NoiseTexture;
Texture2D       g_NoiseTexture2;


float			g_Time;
float4			g_vCamPosition;

int             g_ItemNumber;

struct VS_IN
{
	float3			vPosition : POSITION;	
	float2			vTexcoord : TEXCOORD0;	
};

struct VS_OUT
{
	float4			vPosition : SV_POSITION;
	float2			vTexcoord : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vWorldPos = Out.vPosition;

	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjMatrix);	
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4			vPosition : SV_POSITION;
	float2			vTexcoord : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float fSpeedOffsetX = { 0.0f }; // 노이즈 텍스쳐 x(좌우로) 얼마나 빠르게 움직이게할꺼셈? 
    float fSpeedOffsetY = { 0.5f }; // 노이즈 텍스쳐 y(위로) 얼마나 빠르게 움직이게할꺼셈? 
    float fRatioNoiseTexture = { 1.0f }; // 노이즈 텍스쳐 쓰는 비율너낌?
    
    float2 vTexCoord = In.vTexcoord;

    float noise = g_NoiseTexture.Sample(LinearSampler, vTexCoord * fRatioNoiseTexture + float2(fSpeedOffsetX, g_Time * fSpeedOffsetY)).r;
    float2 vDistortion = (noise - 0.5f) * float2(0.1f, 0.2f);
    vTexCoord += vDistortion;

    
    float3 vRedColor = lerp(float3(1.0, 0.3, 0.0), float3(1.0, 0.8, 0.0), float3(1.0, 0.0, 0.0));
    float3 vGreenColor = lerp(float3(0.8, 1.0, 0.7), float3(0.5, 1.0, 0.6), float3(0.3, 1.0, 0.4));
    float3 vBlueColor = lerp(float3(0.5, 0.4, 1.0), float3(0.2, 0.6, 1.0), float3(0.4, 0.3, 1.0));
    float3 vYellowColor = lerp(float3(1.0, 1.0, 0.4), float3(1.0, 0.9, 0.2), float3(1.0, 0.8, 0.0));
    float3 vWhiteColor = lerp(float3(0.8f, 0.8f, 0.8f), float3(0.9, 0.9, 0.9), float3(1.0, 0.8, 0.9));
    float fDistortionStrength = 1.2f;

    float4 FlareColor = g_Texture.Sample(LinearSampler_Clamp, vTexCoord);
    
    if (g_ItemNumber == 1) // 파랑 (KEY 1) 
        Out.vColor.rgb = FlareColor.rgb * vBlueColor * fDistortionStrength;
    else if (g_ItemNumber == 2)  // 초록 (SKILL PIECE) 
        Out.vColor.rgb = FlareColor.rgb * vGreenColor * fDistortionStrength;
    else if (g_ItemNumber == 3) // 빨강 (KEY 2) 
        Out.vColor.rgb = FlareColor.rgb * vRedColor * fDistortionStrength;
    else if (g_ItemNumber == 4) // 노랑 (ITEM_FORGIVEN)
        Out.vColor.rgb = FlareColor.rgb * vYellowColor * fDistortionStrength;
    else if (g_ItemNumber == 5) // 하양 (ITEM_FIELD)
        Out.vColor.rgb = FlareColor.rgb * vWhiteColor * fDistortionStrength;
    
    Out.vColor.a = FlareColor.a * 0.5f;

   
    return Out;
}

technique11 DefaultTechnique
{
    // 0번
	pass DefaultPass
	{
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Skip_ITEMZ, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}

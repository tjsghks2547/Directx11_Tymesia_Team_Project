#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_DiffuseTexture[2];
Texture2D g_NormalTexture[2];
Texture2D g_ORMTexture[2];
Texture2D g_MouseTexture;

Texture2D g_MaskTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), mul(g_WorldMatrix, g_ViewMatrix)));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
	
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float fSpecular : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);

	//vector		vShade = max(dot(normalize(g_vLightDir) * -1.f, 
	//	normalize(In.vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient);
	//
	//vector		vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
	//vector		vLook = In.vWorldPos - g_vCamPosition;
	//
	///* 0 ~ 1 */
	//float		fSpecular = pow(max(dot(normalize(vReflect) * -1.f,
	//	normalize(vLook)), 0.f), 50.f);	

	//Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(vShade) 
	//	+ fSpecular * (g_vLightSpecular * g_vMtrlSpecular);
	
    Out.vDiffuse = vMtrlDiffuse;

	/* 0 ~ 1 */   /* -1 ~ 1 */
	/* -1 -> 0, 1 -> 1 */
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.fSpecular = 0.1f;

    return Out;
}

PS_OUT PS_MASKING_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 15.f);
    vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexcoord * 15.f);
    
    vector vSourNormal = g_NormalTexture[0].Sample(LinearSampler, In.vTexcoord * 15.f);
    vector vDestNormal = g_NormalTexture[1].Sample(LinearSampler, In.vTexcoord * 15.f);
    
    vector vSourORM = g_ORMTexture[0].Sample(LinearSampler, In.vTexcoord * 15.f);
    vector vDestORM = g_ORMTexture[1].Sample(LinearSampler, In.vTexcoord * 15.f);
    
    vector vMaskTexture = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    
    /*
    

    vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexcoord * 30.f);
    
    vector vSourNormal = g_NormalTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vDestNormal = g_NormalTexture[1].Sample(LinearSampler, In.vTexcoord * 30.f);
    
    vector vSourORM = g_ORMTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vDestORM = g_ORMTexture[1].Sample(LinearSampler, In.vTexcoord * 30.f);
    
    */
	
    vector vMtrlDiffuse = vDestDiffuse * vMaskTexture + vSourDiffuse * (1.f - vMaskTexture);
    
    vector vMtrlNormal = vector(vDestNormal.xyz * 0.5f + 0.5f, 0.f) * vMaskTexture + vector(vSourNormal.xyz * 0.5f + 0.5f, 0.f) * (1.f - vMaskTexture); // vMaskTexture;
     
    
    float3 vNormal = vMtrlNormal.xyz * 2.f - 1.f;
    
    vMtrlNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vMtrlNormal;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    //Out.fSpecular = ((1 - vDestORM.b) * 0.5f + (vDestORM.b * 1.f)) * (1 - vDestORM.g) * vMaskTexture // Dest의 specular 계산
    //+ ((1 - vSourORM.b) * 0.5f + (vSourORM.b * 1.f)) * (1 - vSourORM.g) * (1.f - vMaskTexture); // Sour의 specular 계산
	
    Out.fSpecular = 0.f;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass TerrainMasking
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MASKING_MAIN();
    }
}

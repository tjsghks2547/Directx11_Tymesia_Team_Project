#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

/* hlsl 용 벡터타입 : float2, float3, float4, vector */
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
float3 g_vRGB;
float g_fTimer;

//sampler LinearSampler = sampler_state
//{ 
//	Filter = MIN_MAG_MIP_LINEAR;
//};

/* 1. 정점과 인덱스 버퍼 + 셰이더 바인딩해서 그린다. */
/* 2. 인덱스버퍼에 저장되어있는 인덱스에 해당하는 정점을 하나 꺼내서, */
/* 3. 정점쉐이더함수를 호출하며 인자로 정점(로컬스페이스기준)을 전달해준다. */
/* 4. 정점 쉐이더에서 정점에 필요한 연산을 수행한다.(ex> 정점의 로컬위치에다가 월드, 뷰, 투영행려을 곱한다. ) */
/* 5. 정점의 변환을 수행한다(4) + 정점의 구성 정보를 추가 또는 삭제한다. */

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	//Out.vWorldPos = Out.vPosition;

    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    Out.vTexcoord = In.vTexcoord;

    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    //float x = (In.vProjPos.z / In.vProjPos.w);
    vector vResult = vDiffuse;
    //float fWeight = saturate(max(15e-2, In.vProjPos.w / 70.f));
    
    //vResult *= fWeight;
    vResult.xyz = (1.f - (vResult.xyz * g_vRGB));
    vResult.a *= saturate(1.f - g_fTimer);
    Out.vColor = vResult;
	
    if (0.1f >= Out.vColor.a)	
        discard;

    return Out;
}

PS_OUT PS_MAIN_DISTORTION(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float x = (In.vProjPos.z / In.vProjPos.w);
    vector vResult = vDiffuse;
    float fWeight = saturate(max(15e-2, In.vProjPos.w / 70.f));
    
    //vResult *= fWeight;
    vResult.xyz *= fWeight * g_vRGB;
    
    Out.vColor = vResult;
	

    return Out;
}

PS_OUT PS_MAIN_WEIGHT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float x = (In.vProjPos.z / In.vProjPos.w);
    vector vResult = vDiffuse;
    float fWeight = saturate(max(15e-2, In.vProjPos.w / 70.f));
    
    //vResult *= fWeight;
    vResult.xyz *= fWeight * g_vRGB;
    
    Out.vColor = vResult;
	
	//if (0.01f >= Out.vColor.a)	
	//	discard;	

    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend_Inv, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        //SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass Distortion
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
    }

    pass Weight
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_WeightBlend, 0);
        SetBlendState(BS_WeightBlend_Client, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WEIGHT();
    }
}

#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

/* hlsl �� ����Ÿ�� : float2, float3, float4, vector */
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
float3 g_vRGB;
float g_fTimer;

//sampler LinearSampler = sampler_state
//{ 
//	Filter = MIN_MAG_MIP_LINEAR;
//};

/* 1. ������ �ε��� ���� + ���̴� ���ε��ؼ� �׸���. */
/* 2. �ε������ۿ� ����Ǿ��ִ� �ε����� �ش��ϴ� ������ �ϳ� ������, */
/* 3. �������̴��Լ��� ȣ���ϸ� ���ڷ� ����(���ý����̽�����)�� �������ش�. */
/* 4. ���� ���̴����� ������ �ʿ��� ������ �����Ѵ�.(ex> ������ ������ġ���ٰ� ����, ��, ��������� ���Ѵ�. ) */
/* 5. ������ ��ȯ�� �����Ѵ�(4) + ������ ���� ������ �߰� �Ǵ� �����Ѵ�. */

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

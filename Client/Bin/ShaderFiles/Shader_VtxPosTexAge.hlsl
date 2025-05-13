#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

/* hlsl �� ����Ÿ�� : float2, float3, float4, vector */
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D		g_Texture;


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
	float3			vPosition : POSITION;	
	float2			vTexcoord : TEXCOORD0;	
    float			vAge      : TEXCOORD1;
};

struct VS_OUT
{
	float4			vPosition : SV_POSITION;
	float2			vTexcoord : TEXCOORD0;
    float			vAge      : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	//Out.vWorldPos = Out.vPosition;

	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjMatrix);	
	Out.vTexcoord = In.vTexcoord;
    Out.vAge = In.vAge;

	return Out;
}

/* 5. �������̴��� ������ �Ǹ� �׸��� ���¿����� �߰����� ������ ����� ��ٸ���. */
/* 5-1. D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST ���ٸ� �߰��� �ΰ��� ������ �������̴��� ����Ҷ� ���� ��ٸ���. */
/* 6. ���� �׸������ϴ� ������ ���¿� �°Բ� ������ ��� ���ϵȴٸ� �� �������� w�����⿬��(��������)�� �����Ѵ�. ���������̽��� ����. */
/* 7. ����Ʈ�� ������ ��ġ�� ��ȯ�ϰԵȴ�. ������ ��ǥ�� �������� ��� �� �Űܳ���. */
/* 8. �����Ͷ���� �����ϰԵȴ�. (�����Ͷ����� : �������� ������ ������� �Ͽ� �ȼ��� ������ �����Ѵ�) => �ȼ��� �������. */
/* 9. ���� �ȼ��� PS_MAIN�Լ��� ȣ���ϸ鼭 ���ڷ� ������ �ش�. */
/* 10. �ȼ� ���̴����� ���޹��� �ȼ��� ������ ��������Ͽ� �������� �ȼ��� ���� �����Ͽ� �����ϰԵǰ� */
/* 11. ���ϵ� �������� ������ ����۸� ä��� �ȴ�. */

struct PS_IN
{
	float4			vPosition : SV_POSITION;
	float2			vTexcoord : TEXCOORD0;
	float			Age : TEXCOORD1;
};

struct PS_OUT
{
	float4			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);	 

	
    //float4 texColor = gDiffuseMap.Sample(gSampler, input.TexCoord);

    //// Age ������� ���� ���̵�
    //// Age=0 -> Alpha=1, Age=gMaxAge -> Alpha=0
    //float alpha = saturate(1.0f - (input.Age / gMaxAge));

    //texColor.a *= alpha;

    //return texColor;
	//if(In.vTexcoord.x > g_State)		
	//	Out.vColor.rgb = 0.f;		
	////
	if (0.3f >= Out.vColor.a)	
		discard;	

	return Out;
}



technique11 DefaultTechnique
{
	pass DefaultPass
	{
        SetRasterizerState(Rs_Cull_NONE);	
        SetDepthStencilState(DSS_Default, 0);	
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);	
	

		VertexShader = compile vs_5_0 VS_MAIN();	
        GeometryShader = NULL;	
		PixelShader = compile ps_5_0 PS_MAIN();	
	}

}

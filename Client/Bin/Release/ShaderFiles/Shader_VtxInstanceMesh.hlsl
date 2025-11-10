#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D       g_DiffuseTexture;
Texture2D		g_NormalTexture;
Texture2D       g_NoiseTexture; 

float           g_Time;
float           g_DissolveAmount;   

float4x4 g_PreWorldMatrix, g_PreViewMatrix;

struct VS_IN
{
	float3			vPosition : POSITION;	
	float3			vNormal   : NORMAL;
	float2			vTexcoord : TEXCOORD0;	
	float3			vTangent  : TANGENT;
	
    // row_major float4x4 Transformatrix; 
    float4          InstanceMatrix[4] : INSTANCE_MATRIX;
};

struct VS_OUT
{
	float4			vPosition : SV_POSITION;
	float4			vNormal   : NORMAL;
	float2			vTexcoord : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
    float4			vProjPos  : TEXCOORD2; 
	
	   
    float4			vTangent  : TANGENT;
    float4			vBinormal : BINORMAL;
};

struct VS_OUT_MotionBlur
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float4 vDir      : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;
    //matrix			WorldMatrix = In.InstanceMatrix;
    matrix WorldMatrix = matrix
    (
        In.InstanceMatrix[0],
        In.InstanceMatrix[1],
        In.InstanceMatrix[2],
        In.InstanceMatrix[3]
    );
	
	//matWV  = mul(g_WorldMatrix, g_ViewMatrix);
    matWV = mul(WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), matWV));   
	Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), WorldMatrix);
    Out.vProjPos = Out.vPosition; 
	
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix));
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f); // 외적 순서 중요하다 왜냐하면 순서바뀌면 binormal이 - 축으로 설정되니깐 
	
	return Out;
}


VS_OUT_MotionBlur VS_MAIN_MOTIONBLUR(VS_IN In)
{
    VS_OUT_MotionBlur Out = (VS_OUT_MotionBlur) 0;

    matrix matWV, matWVP;

    
    matrix WorldMatrix = matrix
    (
        In.InstanceMatrix[0],
        In.InstanceMatrix[1],
        In.InstanceMatrix[2],
        In.InstanceMatrix[3]
    );
    
    matWV = mul(WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);


    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), matWV));
    Out.vTexcoord = In.vTexcoord;
    
    
    //motion blur를 위한 추가작업시작    
    float4 vNewPos = Out.vPosition;
    float4 vOldPos = mul(vector(In.vPosition, 1.f), WorldMatrix);
    vOldPos = mul(vOldPos, g_PreViewMatrix);
    vOldPos = mul(vOldPos, g_ProjMatrix);
	
    float3 vDir = vNewPos.xyz - vOldPos.xyz;
    
    float a = dot(normalize(vDir), normalize(Out.vNormal.xyz));
    if (a < 0.f)
        Out.vPosition = vOldPos;
    else
        Out.vPosition = vNewPos;
   
    
    float2 velocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);
   
    Out.vDir.xy = velocity * 0.5f;
    if (abs(velocity.x) <= 0.1f || abs(velocity.y) <= 0.1f)      
        Out.vDir.xy = 0.0025f;
    
  
    Out.vDir.y *= -1.f;
    
    Out.vDir.z = Out.vPosition.z;
    Out.vDir.w = Out.vPosition.w;
	
    return Out;
}

struct PS_IN
{
	float4			vPosition : SV_POSITION;
	float4			vNormal   : NORMAL;
	float2			vTexcoord : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
    float4			vProjPos  : TEXCOORD2;
	
		
    float4          vTangent  : TANGENT;
    float4          vBinormal : BINORMAL;
};

struct PS_IN_MOTIONBLUR
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float4 vDir      : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal  : SV_TARGET1;
    float4 vDepth   : SV_TARGET2;
    float fSpecular : SV_TARGET3;
};

struct PS_OUT_MOTIONBLUR
{
    float4 vColor : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    float4 vNormalDesc  = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);	
	

	/* 탄젠트 스페이스에 존재하는 노멀이다. */	
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;	
	
	
	/* 월드 스페이스상의 노말로 변환하자. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = normalize(mul(vNormal, WorldMatrix));
	

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);	
    //Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth   = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);	
    Out.fSpecular = 0.1f;
	
	return Out;
}


PS_OUT_MOTIONBLUR PS_MAIN_MOTIONBLUR(PS_IN_MOTIONBLUR In)
{
    PS_OUT_MOTIONBLUR Out = (PS_OUT_MOTIONBLUR) 0;
    
    Out.vColor.xy = In.vDir.xy * 11.f;
    
    //방향(속도)
    Out.vColor.z = 1.0f;
    Out.vColor.w = In.vDir.z / In.vDir.w;
    

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


    pass MotionBlur //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_MOTIONBLUR();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MOTIONBLUR();
    }
    //pass InstancingPass
    //{
    //    SetRasterizerState(RS_Default);
    //    SetDepthStencilState(DSS_Default);
    //    SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        
    //    VertexShader = compile vs_5_0 VS_MAIN();
    //    GeometryShader = NULL;
    //    PixelShader = compile ps_5_0 PS_MAIN();


    //}

}

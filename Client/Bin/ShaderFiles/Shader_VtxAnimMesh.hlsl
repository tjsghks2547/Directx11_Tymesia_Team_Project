#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


/* �� �޽�(�����鿡)�� ������ �ִ� �� ��ĵ� (�� �� ���� ������� �����ϸ� ��) ���� ���⼭ �׳� 256�����Ѵ� */
/* �ʹ� ���� ���� ���� �迭�� �����ϸ� ���̴��� �޸𸮸� ū ������ �ƴϱ� ������ �ȵȴ�.*/
float4x4		g_BoneMatrices[256];    

//StructuredBuffer<float4x4> g_BoneMatrices;	
Texture2D g_DiffuseTexture;	
Texture2D g_NormalTexture;	
Texture2D g_AOTexture;

/*  Dissolve ���� ��� ���۵� */
Texture2D g_NoiseTexture;
float     g_DissolveAmount; 
float     g_EdgeWidth = 1.f; 
float4    g_EdgeColor = { 0.f, 0.f, 1.f, 1.f };
float     g_Time;
float     g_TimeStart;

matrix g_LightViewMatrix[3];
matrix g_LightProjMatrix[3];

/* ����» */
Texture2D       g_GhostNoiseTexture;
float4          g_GhostColor;
float           g_DissolveValue;

/* ���� ���� ���� */
Texture2D g_DissolveNoiseTexture;
bool g_Appear;
bool g_Dead;
float g_ReverseDissolveTime;


/* ORM �ؽ�ó ���� */ 
Texture2D g_ORM_Texture;


struct VS_IN
{
	float3			vPosition :   POSITION;	
	float3			vNormal   :   NORMAL;
	float2			vTexcoord :   TEXCOORD0;	
	float3			vTangent  :   TANGENT;

    uint4			vBlendIndex  : BLENDINDEX;
    float4			vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
    float4			vPosition : SV_POSITION; // �ش� SV_ ��� ���̻�� ���� ��� �� Z�����Ⱑ �� �Ϸ�� ������ ��ǥ�� ��Ÿ�� �� 
	float4			vNormal   : NORMAL;
	float2			vTexcoord : TEXCOORD0;
    float4			vWorldPos : TEXCOORD1;
    float4			vProjPos  : TEXCOORD2;  // SV_ ��� ���̻簡 �����Ƿ� Z������� ��������� �Ǽ� ���°� �ƴ�. 
   
    float4          vTangent   : TANGENT;   
    float4          vBinormal  : BINORMAL;  
};

VS_OUT VS_MAIN(VS_IN In)
{	
	
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
	
	/* �������� ���° ���� ������ �޴����� ���� ������ In.vBlendIndex�� ������ �ְ� */
	/* ���� �ش� �� ����� ��������� �� ���� ����޴� ������ ����ġ�� ����Ǿ������� �װ� ���ؼ� �� �����ش�. */
	/* ��������� ���� �����̽� ���� */
	
    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
	
    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
	g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
	g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
	g_BoneMatrices[In.vBlendIndex.w] * fWeightW;
	
	
	
    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal   = normalize(mul(vNormal, g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vPosition, g_WorldMatrix);
    Out.vProjPos  = Out.vPosition;	
    
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)); 
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f);   
	
    return Out;
}

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

//VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
//{
//    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

//    matrix matWV, matWVP;

//    matWV = mul(g_WorldMatrix, g_ViewMatrix);
//    matWVP = mul(matWV, g_ProjMatrix);

//    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

//    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
//		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
//		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
//		g_BoneMatrices[In.vBlendIndex.w] * fWeightW;

//	/* ���ý����̽� �󿡼� ��ȯ�� �Ѵ�. */
//    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

//    Out.vPosition = mul(vPosition, matWVP);
//    Out.vProjPos = Out.vPosition;

//    return Out;
//}

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fWeightW;

	/* ���ý����̽� �󿡼� ��ȯ�� �Ѵ�. */
    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

    Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.vTexcoord = In.vTexcoord;

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

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;	// ����Ÿ�� 0���� ���� 
    float4 vNormal  : SV_TARGET1;   // ����Ÿ�� 1���� ���� 
    float4 vDepth   : SV_TARGET2;   // ����Ÿ�� 2���� ���� 
    float fSpecular : SV_TARGET3;
    float fRoughness : SV_TARGET4;
};

struct PS_OUT_GLOW
{
    float4 vGlow : SV_TARGET0; // ����Ÿ�� 0���� ���� 
};

struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

//struct PS_IN_SHADOW
//{
//    float4 vPosition : SV_POSITION;
//    float4 vProjPos : TEXCOORD0;
//};

struct PS_OUT_SHADOW
{
    float4 vShadow : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);	
    
    if (vMtrlDiffuse.a < 0.1f)  
        discard;    
   
    
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    /* ź��Ʈ �����̽��� �����ϴ� ����̴�. ���� (0~1 ) UnNormal�� ���� �Ǿ� ���� */  
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;     

   /* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix)); 
    
    Out.vDiffuse = vMtrlDiffuse;	    
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);       
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth  = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.fSpecular = 1.f;
    
    Out.fRoughness = 0.1f;

	return Out;
}


PS_OUT PS_ORM_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a < 0.1f)  
        discard;
   
    
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    /* ź��Ʈ �����̽��� �����ϴ� ����̴�. ���� (0~1 ) UnNormal�� ���� �Ǿ� ���� */  
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

   /* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.fSpecular = g_ORM_Texture.Sample(LinearSampler, In.vTexcoord).b;
    
    Out.fRoughness = g_ORM_Texture.Sample(LinearSampler, In.vTexcoord).g;

    return Out;
}


PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a < 0.1f)  
        discard;
   
    /* Dissolve ���� */ 
    
    // ������ �ؽ�ó���� ���ø� (UV ������, Ÿ�� ���� �̿��� ���� ����)
    float2 noiseUV    = In.vTexcoord + float2(0.0f, g_Time * 0.1);  
    float  noiseValue = g_NoiseTexture.Sample(LinearSampler, noiseUV).r;    
    
    // Dissolve ���� 
    // noiseValue < gDissolveAmount �̸� �ȼ��� ������ ( ���� ó�� ) 
    // clip(x)�� ȣ���ϸ� x < 0 �� �� �ش� �ȼ��� ������. 
    
    if (noiseValue < g_DissolveAmount)
    {
        clip(-1); // �ȼ� ��� 
    }
    
    // (����) Dissolve ��踦 ������ Edge Color�� ���� 
    // ���� (noiseValue)�� gDissolveAmount ��ó�� ���� �� 
    // ��) edgeWidth ���� �������� ȿ�� 
    //float edgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);    
    
    // edgeFactor�� �̿��� ��� ���� ���� 
    // edgeFactor�� 1�̸� ��� ��(����), 0�̸� ��� �� (���� Ŭ��) 
    // ��� = edgeFactor�� 0~1 ���̸� ������ �κ�
    // (�� �� �����ϰ� �Ϸ��� saturate, lerp�� �̿�)
    
    //float  edgeStrength = 1.0 - edgeFactor; // ��� �αٿ��� 1
    //float4 edgeBlend = lerp(vMtrlDiffuse, g_EdgeColor, edgeStrength);    
    //float4 finalColor = lerp(vMtrlDiffuse, edgeBlend, edgeStrength);
    
    /* ////////////////////////////////// */
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    /* ź��Ʈ �����̽��� �����ϴ� ����̴�. ���� (0~1 ) UnNormal�� ���� �Ǿ� ���� */  
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

   /* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    Out.vDiffuse = vMtrlDiffuse;       
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.fSpecular = 1.f;


    return Out;
}

//PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)   
//{
//    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;

//    Out.vShadow = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);

//    return Out;
//}

PS_OUT PS_NOT_Discard_Alpha(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    /* ź��Ʈ �����̽��� �����ϴ� ����̴�. ���� (0~1 ) UnNormal�� ���� �Ǿ� ���� */  
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

   /* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    Out.vDiffuse = vector(vMtrlDiffuse.xyz, 0.1f);
   // Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.1f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);

    return Out;
}

PS_OUT PS_EYE_Mesh(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    float fAO = g_AOTexture.Sample(LinearSampler, In.vTexcoord).r;
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    /* ź��Ʈ �����̽��� �����ϴ� ����̴�. ���� (0~1 ) UnNormal�� ���� �Ǿ� ���� */  
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

   /* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    Out.vDiffuse = vMtrlDiffuse * fAO;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);

    return Out;
}


struct GS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};
struct GS_OUT_SHADOW
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(9)]
void GS_MAIN(triangle GS_IN_SHADOW In[3], inout TriangleStream<GS_OUT_SHADOW> DataStream)
{
    for (int face = 0; face < 3; ++face)
    {
   
        for (int i = 0; i < 3; ++i)
        {
            GS_OUT_SHADOW element;
        
            element.RTIndex = face;
            
            element.vPosition = mul(In[i].vPosition, g_LightProjMatrix[face]);
            element.vTexcoord = In[i].vTexcoord;
            DataStream.Append(element);
        }
        
        DataStream.RestartStrip();
    }
}
struct PSIn
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
};


struct PSOut
{
    float vColor : SV_TARGET0;
};

PSOut PSMainShadow(PSIn In)
{
    PSOut Out = (PSOut) 0;
    
    Out.vColor = In.vPosition.z;
    
    return Out;
}



PS_OUT PS_MONSTER_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    float4 g_LineColor = float4(0.8f, 0.6f, 0.4f, 1.0f);
    if (vMtrlDiffuse.a < 0.1f)  
        discard;
    
    float2 noiseUV = In.vTexcoord + float2(0.0f, g_Time * 0.1);
    float noiseValue = g_NoiseTexture.Sample(LinearSampler, noiseUV).r;

    float EdgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);
    float EdgeStrength = 1.0 - EdgeFactor;

    float4 GlowColor = g_LineColor * 3.f;
    float4 EdgeBlend = lerp(vMtrlDiffuse, GlowColor, EdgeStrength * 2.5f);
    
    if (noiseValue < g_DissolveAmount - g_EdgeWidth * 0.2)
    {
        clip(-1);
    }

    float4 finalColor = EdgeBlend;
    finalColor += GlowColor * EdgeStrength * 0.5f;

    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.0f - 1.0f;
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    Out.vDiffuse = finalColor;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);

    return Out;
}



PS_OUT PS_PLAYER_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    float4 g_LineColor = float4(0.25f, 0.8f, 0.25f, 1.0f);
    
    //float threshold = 0.5f; // 0.0 ~ 1.0  
    float border = 0.02f; // �׵θ� �β�  
    
    if (vMtrlDiffuse.a < 0.1f)  
        discard;
    
    float2 noiseUV = In.vTexcoord;
    
    float noiseValue = g_NoiseTexture.Sample(LinearSampler_Clamp, noiseUV).r;
  
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.0f - 1.0f;
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    
   
    if (noiseValue < g_DissolveAmount)
    {
        clip(-1);
    }
    

    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    
    if (g_TimeStart > 1.f)
    {
        if (noiseValue <= g_DissolveAmount + border)
        {
            float4 finalColor = g_LineColor;
            Out.vDiffuse = finalColor;
            Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
            Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
        
            return Out;
        }
    }
    
    return Out;
}

PS_OUT_GLOW PS_GHOSEMY(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;

    float noise = g_GhostNoiseTexture.Sample(LinearSampler, In.vTexcoord).r;
    float dissolve = saturate(g_DissolveValue - noise);

    //if (dissolve > 0.3f)
    //    clip(-1);
    
    if (dissolve <= 0.01f)
        clip(-1);

    float4 vDiffuseColor = g_GhostColor;
    //float4 vGreenColor = float4(0.6f, 0.9f, 0.5f, 1.0f);
    //float4 vGreenColor = vector(lerp(float3(0.8, 1.0, 0.7), float3(0.5, 1.0, 0.6), float3(0.3, 1.0, 0.4)), 1.0f);
    //float4 vResultColor = lerp(vDiffuseColor, vGreenColor, dissolve);
    //vResultColor.a = saturate(1.0f - dissolve) * 0.01f;
    
    float4 vResultColor = vDiffuseColor;
    vResultColor.rgb *= 0.4f;
    vResultColor.a = 0.05f;

    Out.vGlow = vResultColor;

    //float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    //float3 vNormal = vNormalDesc.xyz * 2.0f - 1.0f;
    //float3x3 matWorld = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    //vNormal = normalize(mul(vNormal, matWorld));
    //Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);

    //Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    //Out.fSpecular = 0.1f;
    return Out;
}


PS_OUT PS_PLAYER_CLAW_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;


    

      // �⺻ ������ 
 
    if (g_Appear)
    {
        //In.vTexcoord.x += g_Time * 5.f;
        //In.vTexcoord.y += g_Time * 10.f;      
    
        Out.vDiffuse = float4(0.3f, 1.f, 0.7f, 0.8f);

        float2 noiseUV = In.vTexcoord;
    
        float noiseValue = g_DissolveNoiseTexture.Sample(LinearSampler_Clamp, noiseUV).r;
  
        if ((1.0 - noiseValue) > g_ReverseDissolveTime)
        {
            clip(-1);
        }

    }
    
   
    if (g_Dead)
    {
        float2 noiseUV = In.vTexcoord;
    
        float noiseValue = g_DissolveNoiseTexture.Sample(LinearSampler_Clamp, noiseUV).r;
  
        if (noiseValue < g_DissolveAmount)
        {
            clip(-1);
        }
  
    }
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.0f - 1.0f;
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = normalize(mul(vNormal, WorldMatrix));
        
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    
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


    pass DessolvePass
    {

        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
    }


    pass Shadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = NULL;
    }

    pass NOT_Discard_Alpha
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_NOT_Discard_Alpha();
    }

    pass Eye_Mesh
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_EYE_Mesh();
    }
    pass MONSTER_DISSOLVE // 5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MONSTER_DISSOLVE();
    }

    pass PLAYER_DISSOLVE // 6
    {
        SetRasterizerState(RS_Default); 
        SetDepthStencilState(DSS_Default, 0);   
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);  

        VertexShader = compile vs_5_0 VS_MAIN();    
        GeometryShader = NULL;  
        PixelShader = compile ps_5_0 PS_PLAYER_DISSOLVE();  
    }

    pass GHOSEMY // 7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_GHOSEMY();
    }


    

    pass PLAYER_CLAW_DISSOLVE // 8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PLAYER_CLAW_DISSOLVE();
    }

    pass NON_CULL_DefaultPass // 9
    {

        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass ORM_TEXTURE_DefaultPass // 10 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ORM_MAIN();
    }

    pass NON_CULL_ORM_TEXTURE_DefaultPass // 11 
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ORM_MAIN();
    }
}
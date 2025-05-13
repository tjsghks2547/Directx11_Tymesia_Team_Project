
#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
Texture2D g_NoiseTexture; 
Texture2D g_AlphaMaskTexture; 
float  g_DissolveAmount;
float  g_Time;
float4 g_vCamPosition;
float2 g_vTexSize; 






struct VS_IN
{
    float3 vPosition : POSITION;
    float fPSize : PSIZE;

    row_major float4x4 TransformMatrix : WORLD;
    float2 vLifeTime : TEXCOORD0;
    float2 vBornTime : TEXCOORD1;
};

struct VS_SPRITE_IN
{
    float3 vPosition : POSITION;
    float fPSize : PSIZE;

    row_major float4x4 TransformMatrix : WORLD;
    float2 vLifeTime : TEXCOORD0;
    float2 vBornTime : TEXCOORD1;
    float2 vTexcoord : TEXCOORD2;   
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float  fPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    float2 vBornTime : TEXCOORD1;   

};


struct VS_SPRTIE_OUT    
{
    float4 vPosition : POSITION;
    float fPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    float2 vBornTime : TEXCOORD1;
    float2 vTexcoord : TEXCOORD2;   
    
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
	
	/* ���ý����̽������� �������� ��������. */
    vector vPosition = mul(vector(In.vPosition, 1.f), In.TransformMatrix);

    Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.fPSize = length(In.TransformMatrix._11_12_13);
    Out.vLifeTime = In.vLifeTime;
    Out.vBornTime = In.vBornTime;
 

    return Out;
}

VS_SPRTIE_OUT VS_MAIN_SPRITE(VS_SPRITE_IN In)   
{
    VS_SPRTIE_OUT Out = (VS_SPRTIE_OUT) 0;  
	
	/* ���ý����̽������� �������� ��������. */
    vector vPosition = mul(vector(In.vPosition, 1.f), In.TransformMatrix);

    Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.fPSize = length(In.TransformMatrix._11_12_13); // ���⼭ ũ�Ⱑ ������. 
    Out.vLifeTime = In.vLifeTime;
    Out.vBornTime = In.vBornTime;
    Out.vTexcoord = In.vTexcoord;


    return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
    float fPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    float2 vBornTime : TEXCOORD1;

};

struct GS_SPRITE_IN
{
    float4 vPosition : POSITION;
    float fPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    float2 vBornTime : TEXCOORD1;
    float2 vTexcoord : TEXCOORD2;

};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;   
    float2 vLifeTime : TEXCOORD1;   
    float2 vBornTime : TEXCOORD2;   
};


/* Geometry Shader == ���ϼ��̴� */
/* */
[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
    GS_OUT Out[4];

    float3 vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].fPSize * 0.5f);
    float3 vUp = normalize(cross(vLook, vRight)) * (In[0].fPSize * 0.5f);

    float4x4 matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    Out[0].vBornTime = In[0].vBornTime; 

    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    //Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    Out[1].vBornTime = In[0].vBornTime;

    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    //Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    Out[2].vBornTime = In[0].vBornTime;

    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    Out[3].vBornTime = In[0].vBornTime;

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
void GS_MAIN_SPRTIE(point GS_SPRITE_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
    GS_OUT Out[4];

    float3 vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].fPSize * 0.5f); // ����ٰ� vLook�� �װ� �־���߰ٳ� ������ �� 0.5 
    float3 vUp = normalize(cross(vLook, vRight)) * (In[0].fPSize * 0.5f);
   

    float4x4 matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f) * g_vTexSize + In[0].vTexcoord;
    Out[0].vLifeTime = In[0].vLifeTime;
    Out[0].vBornTime = In[0].vBornTime;

    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f) * g_vTexSize + In[0].vTexcoord;
    //Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    Out[1].vBornTime = In[0].vBornTime;

    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f) * g_vTexSize + In[0].vTexcoord;
    //Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    Out[2].vBornTime = In[0].vBornTime;

    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f) * g_vTexSize + In[0].vTexcoord;
    Out[3].vLifeTime = In[0].vLifeTime;
    Out[3].vBornTime = In[0].vBornTime;

    DataStream.Append(Out[0]);
    DataStream.Append(Out[1]);
    DataStream.Append(Out[2]);
    DataStream.RestartStrip();

    DataStream.Append(Out[0]);
    DataStream.Append(Out[2]);
    DataStream.Append(Out[3]);
    DataStream.RestartStrip();
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
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
    //float2 vBornTime : TEXCOORD2;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);    
   
  
    if (0.2f >= Out.vColor.a)
        discard;

    //Out.vColor = float4((In.vLifeTime.x - In.vLifeTime.y) * 3.f, 0.f, 0.f, 1.f);

    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;

    return Out;
}


PS_OUT PS_GAIN_EFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
 
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    float3 color = { 1.f, 1.f, 0.3f };
    Out.vColor.rgb = color;
    
    if (0.2f >= Out.vColor.a)
        discard;

    //Out.vColor = float4((In.vLifeTime.x - In.vLifeTime.y) * 3.f, 0.f, 0.f, 1.f);

    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;

    return Out;
}


PS_OUT PS_FIRE_EFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //In.vTexcoord.x = 1.f / 7.f; 
    //In.vTexcoord.y = 1.f / 7.f; 

    
    
    vector vMtrlDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);   
    
    //if (vMtrlDiffuse.a < 0.1f)   
    //    discard;
    
    if (0.2f >= vMtrlDiffuse.r) 
        discard;    
    
    //vector Color =  g_AlphaMaskTexture.Sample(LinearSampler, In.vTexcoord);   
    
   

    //float threshold = 0.f;
    //if (length(Out.vColor.rgb) <= threshold)
    //{
    //    clip(-1);
    //}

    
    
    if (In.vLifeTime.y >= In.vLifeTime.x)       
        discard;
   
    
    Out.vColor = vMtrlDiffuse; //+Color;

    return Out;
}




PS_OUT PS_BULLET_GAIN_EFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

   
  
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.r < 0.1f)
        discard;
    
    if (In.vLifeTime.y >= In.vLifeTime.x)       
        discard;
   
   

    return Out;
}




technique11 DefaultTechnique
{
    // 0 �� 
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    // 1 �� 
    pass GAIN_EFFECT
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_GAIN_EFFECT();
    }

    // 2 �� 
    pass FIRE_EFFECT
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SPRITE();
        GeometryShader = compile gs_5_0 GS_MAIN_SPRTIE();
        PixelShader = compile ps_5_0 PS_FIRE_EFFECT();
    }

    // 3 �� 
    pass BULLET_EFFECT
    {
        SetRasterizerState(RS_Default); 
        SetDepthStencilState(DSS_Default, 0);   
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);  

        VertexShader = compile vs_5_0 VS_MAIN(); 
        GeometryShader = compile gs_5_0 GS_MAIN();   
        PixelShader = compile ps_5_0 PS_BULLET_GAIN_EFFECT();   
    }
}

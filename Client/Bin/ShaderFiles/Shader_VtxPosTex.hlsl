#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

/* hlsl �� ����Ÿ�� : float2, float3, float4, vector */
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D		g_Texture;
float			g_HpState; 
float			g_MpState;

float           g_CurHP;
float           g_PreHP;
float           g_TimeDelta;
float3          g_ButtonColor;

/* ����Ʈ ���� ��� ���۵� */ 
Texture2D g_HitEffect0;
Texture2D g_HitEffect1;     
Texture2D g_HitEffect2;     

float g_fAlpha; 
float g_RotationAngle;   // ���� ���� ȸ���� 


float g_Alpha; 
float2 g_vTexcoord;
float2 g_vOffSetTexcoord;


//sampler LinearSampler = sampler_state
//{ 
//	Filter = MIN_MAG_MIP_LINEAR;
//};

/* 1. ������ �ε��� ���� + ���̴� ���ε��ؼ� �׸���. */
/* 2. �ε������ۿ� ����Ǿ��ִ� �ε����� �ش��ϴ� ������ �ϳ� ������, */
/* 3. �������̴��Լ��� ȣ���ϸ� ���ڷ� ����(���ý����̽�����)�� �������ش�. */
/* 4. ���� ���̴����� ������ �ʿ��� ������ �����Ѵ�.(ex> ������ ������ġ���ٰ� ����, ��, ��������� ���Ѵ�. ) */
/* 5. ������ ��ȯ�� �����Ѵ�(4) + ������ ���� ������ �߰� �Ǵ� �����Ѵ�. */

float4 AlphaOver(float4 base, float4 over)
{
    float alphaA = over.a;
    float alphaB = base.a;
    
    // ���� ����
    float outAlpha = alphaA + alphaB * (1 - alphaA);

    float3 outColor = float3(0, 0, 0);
    if (outAlpha > 1e-6) // 0�� �ƴϾ�� ������ ����
    {
        outColor = (over.rgb * alphaA + base.rgb * alphaB * (1 - alphaA)) / outAlpha;
    }

    return float4(outColor, outAlpha);
}

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



VS_OUT VS_MAIN_SPRITE(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);  
    Out.vWorldPos = Out.vPosition;  

    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    Out.vTexcoord = In.vTexcoord * g_vTexcoord + g_vOffSetTexcoord; 

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
	float4			vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord); 

    
    //if (0.1f >= Out.vColor.a)
    //    discard;


    return Out;
}

PS_OUT PS_MAIN_HIT_EFFECT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    
    //float fAlphaHit_Effect0 = g_fAlpha; 
    //float fAlphaHit_Effect1 = g_HitEffect1.Sample(LinearSampler, In.vTexcoord).a;          
    //float fAlphaHit_Effect2 = g_HitEffect2.Sample(LinearSampler, In.vTexcoord).a;   
    
    
    float4 Color_Hit_Effect0 = g_HitEffect0.Sample(LinearSampler, In.vTexcoord);          
    //float4 Color_Hit_Effect1 = g_HitEffect1.Sample(LinearSampler, In.vTexcoord);       
    //float4 Color_Hit_Effect2 = g_HitEffect2.Sample(LinearSampler, In.vTexcoord);           
    //
    
    Color_Hit_Effect0.a = g_fAlpha;              
    //Color_Hit_Effect1.a = 0.5f;     
    //Color_Hit_Effect2.a = 0.2f;         
  
   
    Out.vColor = Color_Hit_Effect0;//+Color_Hit_Effect1; // + Color_Hit_Effect2;
        
    //Out.vColor = g_HitEffect0.Sample(LinearSampler, In.vTexcoord)
    //+ g_HitEffect1.Sample(LinearSampler, In.vTexcoord)
    //+ g_HitEffect2.Sample(LinearSampler, In.vTexcoord);

    vector YellowTint = vector(1.0f, 1.0f, 0.0f, 1.0f); 
    vector orange = vector(1.0f, 0.76f, 0.4f, 1.0f);
    
    Out.vColor = Out.vColor * orange;   


    // float3 Color = { 0.0f, 0.0f, 0.0f };
	////if(In.vTexcoord.x > g_State)		
	////	Out.vColor.rgb = 0.f;		
	////
    //   if (Out.vColor.rgb == Color)
    //       discard;
    
    float threshold = 0.99f;

    // color.rgb�� ���̰� threshold �̸��̸� clip
    if (length(Out.vColor.rgb) < threshold)
    {
        clip(-1);
    }
    

	return Out;
}



PS_OUT PS_MAIN_BULLET_EFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if(Out.vColor.r < 0.1f)
        discard;
    
    //float threshold = 0.99f;

    //// color.rgb�� ���̰� threshold �̸��̸� clip
    //if (length(Out.vColor.rgb) < threshold)
    //{
    //    clip(-1);
    //}
    

    return Out;
}

PS_OUT PS_MAIN_HP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	

    if (In.vTexcoord.x > g_HpState)		
        Out.vColor.rgb = 0.f;
	//
    if (0.1f >= Out.vColor.a)
        discard;

    return Out;
}


PS_OUT PS_MAIN_MP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	

    if (In.vTexcoord.x > g_MpState)		
        Out.vColor.rgb = 0.f;
	//
    if (0.1f >= Out.vColor.a)
        discard;

    return Out;
}


PS_OUT PS_MAIN_UI(PS_IN In) 
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/   
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)  
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord); 

    float threshold = 0.99f;        

    // color.rgb�� ���̰� threshold �̸��̸� clip
    if (length(Out.vColor.rgb) > threshold) 
    {
        clip(-1);       
    }
    
    return Out; 
}



PS_OUT PS_MAIN_UI_ALPHA(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/   
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)  
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if(Out.vColor.a < 0.2f)
        discard;    

    //float threshold = 0.99f;

    //// color.rgb�� ���̰� threshold �̸��̸� clip
    //if (length(Out.vColor.rgb) > threshold)
    //{
    //    clip(-1);
    //}
    
    return Out;
}

PS_OUT PS_MAIN_UI_ALPHA_BLEND(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;    

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/   
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)  
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.a = 0.85f;
    //if (Out.vColor.a < 0.2f)
    //    discard;

    //float threshold = 0.01f;
    //
    //// color.rgb�� ���̰� threshold �̸��̸� clip
    //if (length(Out.vColor.rgb) < threshold)
    //{
    //    clip(-1);
    //}
    
    return Out;
}


PS_OUT PS_MAIN_PORTAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/   
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)  
    //Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    //float threshold = 0.1f;

    //// color.rgb�� ���̰� threshold �̸��̸� clip
    //if (length(Out.vColor.rgb) < threshold)
    //{
    //    clip(-1);
    //}
    
    //1. �ؽ�ó �߽� (0.5,0.5)
    float2 center = float2(0.5f, 0.5f);
    
    //2. UV ��ǥ�� �ؽ�ó �߽��� �������� �̵� (�߽ɰ� ����) 
    float2 uv = In.vTexcoord - center; 
    
    //3. ȸ�� ��� ���� : ȸ������ ���� sin, cos ��� 
    float s = sin(g_RotationAngle);
    float c = cos(g_RotationAngle);
    float2 rotatedUV = float2(uv.x * c - uv.y * s,
                              uv.x * s + uv.y * c);
    
    
    //4. ȸ�� �� ���� ��ǥ��� ���� (�߽ɰ� ���ϱ�) 
    rotatedUV += center; 
    
    //5. ȸ���� UV��ǥ�� �ؽ�ó ���ø� 
    Out.vColor = g_Texture.Sample(LinearSampler_Clamp, rotatedUV);
    
    
    
    //float threshold = 0.1f;
        
    //// color.rgb�� ���̰� threshold �̸��̸� clip
    //if (length(Out.vColor.rgb) < threshold) 
    //{
    //    clip(-1);
    //}   
    
    if (Out.vColor.g <= 0.1f)   
        discard;    
    
    
    return Out;
}


PS_OUT PS_FADE_IN_OUI_UI(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/   
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)  
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor.a -= g_Alpha; 
    //float threshold = 0.99f;

    //// color.rgb�� ���̰� threshold �̸��̸� clip
    //if (length(Out.vColor.rgb) > threshold)
    //{
    //    clip(-1);
    //}
    
    return Out;
}

PS_OUT PS_MAIN_CRACK_EFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;    

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/   
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)  
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord); 
    
    //float3 gray = { 1.f, 0.7f, 0.f };
    //Out.vColor.rgb = gray;
    
    
    if(Out.vColor.r < 0.1f)
        discard;
    
    return Out; 
}


PS_OUT PS_MAIN_CLAW_EFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/   
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)  
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    //float3 gray = { 1.f, 0.7f, 0.f };
    //Out.vColor.rgb = gray;
    
    
    if (Out.vColor.b > 0.1f)
        discard;
    
    return Out;
}

PS_OUT PS_Thymesia_Ui(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    

    return Out;
}
PS_OUT PS_Boss_HP_Gage_Effect(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    // g_Texture���� ���� ���ø�
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (In.vTexcoord.x < g_CurHP || In.vTexcoord.x > g_PreHP)
    {
        Out.vColor.a = 0.f;
        discard;
        return Out;
    }
    
    // g_HpState���� ū �������� alpha ���� ���������� �����ϵ��� ó��
    if (In.vTexcoord.x > g_CurHP && In.vTexcoord.x < g_PreHP)
    {
        // smoothstep�� ����Ͽ� �ε巴�� alpha �� ����
        // g_TimeDelta�� ������ �� �ð� ����
     
        float fAlpha = smoothstep(0.f, 1.f, g_TimeDelta);
        Out.vColor.a = 1.0f - fAlpha; // alpha ���� 1���� ���������� 0���� ����
    }

    return Out;
}

PS_OUT PS_Boss_HP_Gage(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (In.vTexcoord.x > g_CurHP)		
        Out.vColor.a = 0.f;
    
    if (0.1f >= Out.vColor.a)
        discard;

    return Out;
}

PS_OUT PS_Boss_HP_Flash(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float fAlpha = smoothstep(0.f, 1.f, g_TimeDelta);
    Out.vColor.a = 1.0f - fAlpha; // alpha ���� 1���� ���������� 0���� ����
                 
    if (length(Out.vColor.rgb) <= 0.2f) 
        discard;
    
    //if (0.1f >= Out.vColor.a)
    //    discard;
    
    return Out;
}

PS_OUT PS_INTERACTION_BUTTON(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor.a *= g_fAlpha;
    

    return Out;
}

PS_OUT PS_Thymesia_UI_Change_To_White(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    // �ؽ�ó ������ ���ø�
    float4 fDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);

    // ������(���� 0,0,0)�� ������ ���(1,1,1)���� ����
    float threshold = 0.05; // ���������� �Ǻ��� �Ӱ谪
    if (fDiffuse.x < threshold && fDiffuse.y < threshold && fDiffuse.z < threshold)
    {
        fDiffuse = float4(1, 1, 1, fDiffuse.w);
    }

    Out.vColor = fDiffuse;

    return Out;
}

PS_OUT PS_STUN_LOCKED_ON(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
   
 
    if (Out.vColor.a < 0.2f)
    {
        discard;
    }
    
    float2 center = float2(0.5f, 0.5f);
    float dist = length(In.vTexcoord - center);
    
    float redIntensity = saturate(1.0 - dist * 2.f);
    Out.vColor.rgb = lerp(Out.vColor.rgb, float3(1.f, 0.f, 0.f), redIntensity);
    
    
    return Out;
}

technique11 DefaultTechnique
{
    // 0��
	pass DefaultPass
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    // 1��
    pass HP_Bar
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();	
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HP();	
    }

    // 2��
    pass MP_Bar
    {
        SetRasterizerState(RS_Default);	
        SetDepthStencilState(DSS_Default, 0);	
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);	

        VertexShader = compile vs_5_0 VS_MAIN();	
        GeometryShader = NULL;	
        PixelShader = compile ps_5_0 PS_MAIN_MP();	
    }

    // 3��
    pass Hit_Effect
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HIT_EFFECT();  
    }

    // 4��
    pass UI
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UI();
    }

    // 5��
    pass Portal
    {

        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PORTAL();
    }

    // 6��
    pass UI_Alpha
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UI_ALPHA();
    }

    // 7��
    pass UI_Alpha_Blend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UI_ALPHA_BLEND();
    }

    // 8��

    pass Bullet_Effect
    {
        SetRasterizerState(RS_Default); 
        SetDepthStencilState(DSS_Default, 0);   
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);   
    
        VertexShader = compile vs_5_0 VS_MAIN();    
        GeometryShader = NULL;  
        PixelShader = compile ps_5_0 PS_MAIN_BULLET_EFFECT();
    }

    // 9��
    pass UI_FadeInOut
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);   

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_FADE_IN_OUI_UI();   
    }

    // 10�� 

    pass Crack_Effect
    {
        SetRasterizerState(RS_Default); 
        SetDepthStencilState(DSS_Default, 0);   
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);  

        VertexShader = compile vs_5_0 VS_MAIN(); 
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CRACK_EFFECT();        
    }

    // 11��
    pass Claw_Effect
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CLAW_EFFECT();
    }
  // 12��
    pass Thymesia_Ui
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_Ui();
    }
//13��
    pass Boss_HP_Gage
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Boss_HP_Gage();
    }

//14��
    pass Boss_HP_Gage_Effect
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Boss_HP_Gage_Effect();
    }

//15��
    pass Boss_HP_Gage_Flash
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Boss_HP_Flash();
    }

//16�� 
    pass InterAction_Button
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_INTERACTION_BUTTON();
    }
//17��
    pass Thymesia_UI_Change_To_White
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Change_To_White();
    }
//18��
    pass UI_Blend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UI_ALPHA_BLEND();
    }
//19��
    pass STUN_LOCKED_ON
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_STUN_LOCKED_ON();
    }

}

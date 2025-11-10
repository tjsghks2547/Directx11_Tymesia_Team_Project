#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_OldViewMatrix, g_OldWorldMatrix;
Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_NoiseTexture;
Texture2D g_MaskTexture;

float g_fWeightX;
float g_fWeightY;
float g_TimeX;
float g_TimeY;
float g_StartTexcoordX;
float g_StartTexcoordY;
float g_DissolveAmount;
bool g_bUsing_Noise; //0 Noise True, Texcoord

float g_MaskCountX;
float g_MaskCountY;

//float g_fRGB_R;
//float g_fRGB_G;
//float g_fRGB_B;
float3 g_vRGB;


vector g_vCamPosition;

bool g_bTexcoordX;
bool g_bTexcoordY;
float g_fTexcoordLerpX;
float g_fTexcoordLerpY;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

struct VS_OUT_MotionBlur
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    
    float4 vDir : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
	
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f); // 외적 순서 중요하다 왜냐하면 순서바뀌면 binormal이 - 축으로 설정되니깐 
	
    return Out;
}

VS_OUT_MotionBlur VS_MAIN_MOTIONBLUR(VS_IN In)
{
    VS_OUT_MotionBlur Out = (VS_OUT_MotionBlur) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    
    //motion blur를 위한 추가작업시작
    float4 vNewPos = Out.vPosition;
    float4 vOldPos = mul(vector(In.vPosition, 1.f), g_OldWorldMatrix);
    vOldPos = mul(vOldPos, g_OldViewMatrix);
    vOldPos = mul(vOldPos, g_ProjMatrix);
	
    float3 vDir = vNewPos.xyz - vOldPos.xyz;
    
    float a = dot(normalize(vDir), normalize(Out.vNormal.xyz));
    if (a < 0.f)
        Out.vPosition = vOldPos;
    else
        Out.vPosition = vNewPos;
    
    float2 vVelocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);
    Out.vDir.x = vVelocity.x * 0.5f;
    Out.vDir.y = vVelocity.y * (-0.5f);
    Out.vDir.z = Out.vPosition.z;
    Out.vDir.w = Out.vPosition.w;
    //motion blur를 위한 추가작업끝
    
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f);
	
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
		
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

struct PS_IN_MOTIONBLUR
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    
    float4 vDir : TEXCOORD3;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

struct PS_OUT_DISTORTION
{
    float4 vDistortion : SV_TARGET0;
};

struct PS_OUT_GLOW
{
    float4 vGlow : SV_TARGET0;
};

struct PS_OUT_MOTIONBLUR
{
    float4 vMotionBlur : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    
    
    //vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    //vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    
    float fMask = vMask.r;
    
    //if (vMtrlDiffuse.a < 0.1f)
    //    discard;
	
    //float4 vNormalDesc  = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);	
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	

	/* 탄젠트 스페이스에 존재하는 노멀이다. */	
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
	
	
	/* 월드 스페이스상의 노말로 변환하자. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = normalize(mul(vNormal, WorldMatrix));
	

    Out.vDiffuse = vMtrlDiffuse * fMask;
    Out.vDiffuse.a = 1.f;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    //Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
	
    return Out;
}


PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vMaskTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y);
    vector vMask = g_MaskTexture.Sample(LinearSampler, vMaskTexcoord);
    float fMask = vMask.r;
    
    float2 vMtrlTexcoord = float2(In.vTexcoord.x * g_MaskCountX, In.vTexcoord.y * g_MaskCountY);
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vMtrlTexcoord) * fMask;
    
    //if (vMtrlDiffuse.a < 0.1f)  
    //    discard;
   
    /* Dissolve 관련 */ 
    
    // 노이즈 텍스처에서 샘플링 (UV 스케일, 타임 등을 이용해 변형 가능)
    float2 noiseUV = In.vTexcoord + float2(g_TimeX * 0.1f, g_TimeY * 0.1f);
    float noiseValue = g_NoiseTexture.Sample(LinearSampler, noiseUV).r;
    
    
    //(선택) Dissolve 경계를 검출해 Edge Color를 보정 
    // 경계는 (noiseValue)가 gDissolveAmount 근처에 있을 때 
    // 예) edgeWidth 범위 내에서만 효과 
    float g_EdgeWidth = 0.3f;
    float edgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);
    
    //edgeFactor를 이용해 경계 색상 블랜딩 
    //edgeFactor가 1이면 경계 밖(정상), 0이면 경계 안 (완전 클립)    
    //경계 = edgeFactor가 0~1 사이를 지나는 부분
    //(좀 더 심플하게 하려면 saturate, lerp를 이용) 
    
    float4 g_EdgeColor = { 1.f, 1.f, 0.f, 1.f };
    float edgeStrength = 1.0 - edgeFactor; // 경계 부근에서 1
    float4 edgeBlend = lerp(vMtrlDiffuse, g_EdgeColor, edgeStrength);
    float4 finalColor = lerp(vMtrlDiffuse, edgeBlend, edgeStrength);
    
    // Dissolve 조건 
    // noiseValue < gDissolveAmount 이면 픽셀을 버린다 ( 투명 처리 ) 
    // clip(x)를 호출하면 x < 0 일 때 해당 픽셀이 버려짐. 
    if (noiseValue < g_DissolveAmount)
    {
           // 예를 들어, noiseValue가 g_DissolveAmount 근처일 때는 edge 효과를 유지하고,
            // 그보다 많이 낮으면 투명하게 만듦.
        float alphaFactor = saturate((noiseValue - (g_DissolveAmount - g_EdgeWidth)) / g_EdgeWidth);
        finalColor.a *= alphaFactor;
        
        

         // 필요에 따라, alpha가 너무 낮으면 완전히 폐기할 수도 있음
        if (finalColor.a < 0.01f) 
            clip(-1);
       
    }
    
    // (선택) Dissolve 경계를 검출해 Edge Color를 보정 
    // 경계는 (noiseValue)가 gDissolveAmount 근처에 있을 때 
    // 예) edgeWidth 범위 내에서만 효과 
    //float edgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);    
    
    // edgeFactor를 이용해 경계 색상 블랜딩 
    // edgeFactor가 1이면 경계 밖(정상), 0이면 경계 안 (완전 클립) 
    // 경계 = edgeFactor가 0~1 사이를 지나는 부분
    // (좀 더 심플하게 하려면 saturate, lerp를 이용)
    
    //float  edgeStrength = 1.0 - edgeFactor; // 경계 부근에서 1
    //float4 edgeBlend = lerp(vMtrlDiffuse, g_EdgeColor, edgeStrength);    
    //float4 finalColor = lerp(vMtrlDiffuse, edgeBlend, edgeStrength);
    
    /* ////////////////////////////////// */
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    /* 탄젠트 스페이스에 존재하는 노멀이다. 지금 (0~1 ) UnNormal로 저장 되어 있음 */  
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

   /* 월드 스페이스상의 노말로 변환하자. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    //Out.vDiffuse = vMtrlDiffuse;
    /* 디졸브 관련 */
    float blendFactor = 0.5; // 0 ~ 1 사이의 값, 효과의 강도 조절
    
    //float3 vRGB = float3(g_fRGB_R, g_fRGB_G, g_fRGB_B);
    
    Out.vDiffuse = lerp(vMtrlDiffuse, finalColor, blendFactor) * vector(g_vRGB, 1.f);
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);

    //Out.vDiffuse = 1.f;

    return Out;
}

PS_OUT_DISTORTION PS_MAIN_DISTORTION(PS_IN In)
{
    PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION) 0;

    //아마 이런 작업이 필요할것임
    
    //vector vNoise = g_DistortionTexture.Sample(LinearSampler, In.vTexCoord * g_fTimer);
    //
    //float fWeight = vNoise.r * 0.9f;
    //
    //Out.vColor = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord) * vNoise;
    vector vNoise = 0.f;
    
    vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    float fMask = vMask.r;
    
    float2 vTexcoord = float2(In.vTexcoord.x * g_TimeX + g_StartTexcoordX, In.vTexcoord.y * g_TimeY + g_StartTexcoordY);
    
    if (true == g_bUsing_Noise)
        vNoise = g_NoiseTexture.Sample(LinearSampler, vTexcoord);
    else
        vNoise = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    
    float fWeightX = vNoise.r * g_fWeightX;
    float fWeightY = vNoise.r * g_fWeightY;
    
    vTexcoord = float2(In.vTexcoord.x * fWeightX, In.vTexcoord.y * fWeightY);
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord) * fMask;
    
    //여기까지 지워야할지도
    
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    Out.vDistortion = vMtrlDiffuse;
	
    return Out;
}

PS_OUT_GLOW PS_MAIN_GLOW(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;
    float2 vMaskTexcoord = float2(In.vTexcoord.x * g_MaskCountX, In.vTexcoord.y * g_MaskCountY);
    

    
    vector vMask = g_MaskTexture.Sample(LinearSampler, vMaskTexcoord);
    float fMask = vMask.r;
    
    vector vNoise = 0.f;
    
    float2 vTexcoord = 0;
    
    //vTexcoord = float2(In.vTexcoord.x * g_TimeX + g_StartTexcoordX, In.vTexcoord.y * g_TimeY + g_StartTexcoordY);
    vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y);
    
    if (true == g_bUsing_Noise)
        vNoise = g_NoiseTexture.Sample(LinearSampler_Clamp, vTexcoord);
    else
        vNoise = g_DiffuseTexture.Sample(LinearSampler_Clamp, vTexcoord);
    
    float fWeightX = vNoise.r * g_fWeightX;
    float fWeightY = vNoise.r * g_fWeightY;
    
    //vTexcoord = float2(In.vTexcoord.x * fWeightX, In.vTexcoord.y * fWeightY);
    //vTexcoord = float2((In.vTexcoord.x * g_TimeX + g_StartTexcoordX) * fWeightX, (In.vTexcoord.y * g_TimeY + g_StartTexcoordY) * fWeightY);
    vTexcoord = float2((g_TimeX + g_StartTexcoordX) * fWeightX, (g_TimeY + g_StartTexcoordY) * fWeightY);
    
    if (true == g_bTexcoordX)
        vTexcoord.x = lerp(0.5f, vTexcoord.x, (1.f - vTexcoord.y) * g_fTexcoordLerpX);
    if (true == g_bTexcoordY)
        vTexcoord.y = lerp(0.5f, vTexcoord.y, (1.f - vTexcoord.x) * g_fTexcoordLerpY);
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler_Clamp, vTexcoord) * fMask;
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
    
    //float3 vRGB = float3(g_fRGB_R, g_fRGB_G, g_fRGB_B);
	
    Out.vGlow = vMtrlDiffuse * vector(g_vRGB, 1.f);
    
    //vector vLook = normalize(In.vWorldPos - g_vCamPosition);
    //
    //
    //
    //float fDegree_Look_Normal = dot(vLook, In.vNormal);
    //    
    //if (-0.4f < fDegree_Look_Normal && 0.4f > fDegree_Look_Normal)
    //    Out.vGlow.rgba = 1.f;
	
    return Out;
}

PS_OUT PS_MAIN_BLOOD(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vMaskTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y);
    vector vMask = g_MaskTexture.Sample(LinearSampler, vMaskTexcoord);
    float fMask = vMask.r;
    
    float2 vMtrlTexcoord = float2(In.vTexcoord.x * g_MaskCountX, In.vTexcoord.y * g_MaskCountY);
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vMtrlTexcoord) * fMask;
    
    float2 noiseUV = In.vTexcoord + float2(g_TimeX * 0.1f, g_TimeY * 0.1f);
    float noiseValue = g_NoiseTexture.Sample(LinearSampler, noiseUV).r;
    
    float g_EdgeWidth = 0.3f;
    float edgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);
    
    float4 g_EdgeColor = { 0.f, 0.f, 0.f, 1.f };
    float edgeStrength = 1.0 - edgeFactor; // 경계 부근에서 1
    float4 edgeBlend = lerp(vMtrlDiffuse, g_EdgeColor, edgeStrength);
    float4 finalColor = lerp(vMtrlDiffuse, edgeBlend, edgeStrength);
    
    if (noiseValue < g_DissolveAmount)
    {
        float alphaFactor = saturate((noiseValue - (g_DissolveAmount - g_EdgeWidth)) / g_EdgeWidth);
        finalColor.a *= alphaFactor;
        
        if (finalColor.a < 0.01f) 
            clip(-1);
    }
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    float blendFactor = 0.2f; // 0 ~ 1 사이의 값, 효과의 강도 조절
    
    Out.vDiffuse = lerp(vMtrlDiffuse, finalColor, blendFactor) * vector(g_vRGB, 1.f);
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.vDiffuse.a = 1.f;
    
    
    return Out;
}

PS_OUT_DISTORTION PS_MAIN_ROUND(PS_IN In)
{
    PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION) 0;

    float2 vMaskTexcoord = float2(In.vTexcoord.x * g_MaskCountX, In.vTexcoord.y * g_MaskCountY);
    vector vMask = g_MaskTexture.Sample(LinearSampler, vMaskTexcoord);
    float fMask = vMask.r;
    
    float2 vRound = In.vTexcoord - float2(0.5f, 0.5f);
    float fDist = length(vRound);
    
    float fWave = sin((fDist - g_TimeX * g_TimeY) * 30) * exp(-fDist * 8) * g_DissolveAmount;
    
    float3 vNoise = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord).rgb * 2 - 1.f;
    
    float2 vOffset = vNoise.xy * fWave;
    
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord + vOffset) * vMask;
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    Out.vDistortion = vMtrlDiffuse;
	
    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass //0
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    
    pass DissolvePass //1
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
    }

    pass Distortion //2
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
    }

    pass Glow //3
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }

    pass Blood //4
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOD();
    }

    pass Round //5
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ROUND();
    }
}

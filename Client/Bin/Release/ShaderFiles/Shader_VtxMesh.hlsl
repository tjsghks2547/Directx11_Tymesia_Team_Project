#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_OldViewMatrix, g_OldWorldMatrix;
Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
vector g_vCamPosition;

matrix g_LightViewMatrix[3];
matrix g_LightProjMatrix[3];

float4 g_fAlphaValue;
float g_RimPower = 0.5f;
float4 g_RimColor = { 0.1f, 0.1f, 0.1f, 1.0f };
float fRimStrength = 0.5f;
float g_fObjectAlpha;


/*  Dissolve 관련 상수 버퍼들 */
Texture2D g_NoiseTexture;
Texture2D g_DissolveNoiseTexture;

float g_DissolveAmount;
float g_ReverseDissolveTime;    
float g_EdgeWidth = 1.f;
float4 g_EdgeColor = { 0.f, 0.f, 1.f, 1.f };
float g_Time;
bool g_Dead;
bool g_Appear;

float4x4 g_PreWorldMatrix, g_PreViewMatrix; 

Texture2D g_DissolveTexture;
float g_DissolveValue;

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

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_MotionBlur
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vDir : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

struct VS_OUT_OCCULUSION
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), matWV));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
	
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f); // 외적 순서 중요하다 왜냐하면 순서바뀌면 binormal이 - 축으로 설정되니깐 
	
    return Out;
}

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vTexcoord = In.vTexcoord;
    //Out.vProjPos = Out.vPosition;

    return Out;
}

VS_OUT_MotionBlur VS_MAIN_MOTIONBLUR(VS_IN In)
{
    VS_OUT_MotionBlur Out = (VS_OUT_MotionBlur) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), matWV));
    Out.vTexcoord = In.vTexcoord;
    
    
    //motion blur를 위한 추가작업시작    
    float4 vNewPos = Out.vPosition;
    float4 vOldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
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

VS_OUT VS_MAIN_OCCULUSION(VS_IN In)
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

struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_IN_MOTIONBLUR
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vDir : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

struct PS_IN_OCCULUSION
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float fSpecular : SV_TARGET3;
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
    float4 vColor : SV_TARGET0;
};

struct PS_OUT_OCCULSION
{
    float4 vDiffuse : SV_TARGET0;
};

struct PS_OUT_WEIGHTBLEND
{
    float4 vBlendDiffuse : SV_TARGET0;
};

struct PS_OUT_ITEM_GLOW
{
    float4 vItemGlow : SV_TARGET0;
};

struct PS_OUT_RIMLIGHT
{
    float4 vColor : SV_TARGET0;
};

struct PS_OUT_BRANCH_GLOW
{
    float4 vColor : SV_TARGET0;
};

struct PS_OUT_LAMP
{
    float4 vLampGlow : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	

	/* 탄젠트 스페이스에 존재하는 노멀이다. */	
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
	
	
	/* 월드 스페이스상의 노말로 변환하자. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = normalize(mul(vNormal, WorldMatrix));
	

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    //Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.fSpecular = 0.1f;
    
    return Out;
}


PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a < 0.1f)  
        discard;
   
    /* Dissolve 관련 */ 
    
    // 노이즈 텍스처에서 샘플링 (UV 스케일, 타임 등을 이용해 변형 가능)
    float2 noiseUV = In.vTexcoord + float2(0.0f, g_Time * 0.1);
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
    Out.vDiffuse = lerp(vMtrlDiffuse, finalColor, blendFactor);
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.fSpecular = 1.f;


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
    
    //구색만 맞춰본것이므로 이 아래는 쭉 지워야할지도
    vector vNoise = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord * g_Time);
    
    float fWeight = vNoise.r * 0.9f;
    
    float2 vTexcoord = float2(In.vTexcoord.x * fWeight, In.vTexcoord.y);
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord) * vNoise;
    
    //여기까지 지워야할지도
    
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    Out.vDistortion = vMtrlDiffuse;
	
    return Out;
}

PS_OUT_GLOW PS_MAIN_GLOW(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    Out.vGlow = vMtrlDiffuse;
    
    vector vLook = normalize(In.vWorldPos - g_vCamPosition);
    
    
    
    float fDegree_Look_Normal = dot(vLook, In.vNormal);
        
    if (-0.4f < fDegree_Look_Normal && 0.4f > fDegree_Look_Normal)
        Out.vGlow.rgba = 1.f;
	
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



PS_OUT_OCCULSION PS_MAIN_OCCULUSION(PS_IN In)
{
    PS_OUT_OCCULSION Out = (PS_OUT_OCCULSION) 0;
   
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a >= 0.9f)
    {
        Out.vDiffuse = float4(0.f, 0.f, 0.f, 1.f);
    }
    else
    {
        Out.vDiffuse = float4(0.1f, 0.1f, 0.1f, 1.f);
    }
    return Out;
}


PS_OUT_WEIGHTBLEND PS_MAIN_WEIGHTBLEND(PS_IN In)
{
    PS_OUT_WEIGHTBLEND Out = (PS_OUT_WEIGHTBLEND) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    float x = (In.vProjPos.z / In.vProjPos.w);
    vector vResult = vDiffuse;
    float fWeight = saturate(max(15e-2, In.vProjPos.w / 70.f));
    
    //vResult *= fWeight;
    vResult.xyz *= fWeight;
    
    Out.vBlendDiffuse = vResult;
	
    return Out;
}

PS_OUT_ITEM_GLOW PS_MAIN_ITEM_GLOW(PS_IN In)
{
    PS_OUT_ITEM_GLOW Out = (PS_OUT_ITEM_GLOW) 0;
    
    vector vLook = normalize(In.vWorldPos - g_vCamPosition);
    
    float fDegree_Look_Normal = dot(vLook, In.vNormal);
        
    Out.vItemGlow = g_fAlphaValue;
	
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

PS_OUT PS_MONSTER_WEAPON_DISSOLVE(PS_IN In)
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

PS_OUT_RIMLIGHT PS_MAIN_OBJECT_RIMLIGHT(PS_IN In)
{
    PS_OUT_RIMLIGHT Out = (PS_OUT_RIMLIGHT) 0;

    float3 vDirection = normalize(g_vCamPosition.xyz - In.vWorldPos.xyz);
    
    float fRim = (1 - dot(vDirection, In.vNormal.xyz));
    fRim = pow(abs(fRim), g_RimPower);
    
    float4 vRimLight = fRim * fRimStrength * g_RimColor * g_fObjectAlpha;
    Out.vColor += vRimLight;

    return Out;
}

PS_OUT_BRANCH_GLOW PS_DEAD_BRANCH(PS_IN In)
{
    PS_OUT_BRANCH_GLOW Out = (PS_OUT_BRANCH_GLOW) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    if (vDiffuse.a < 0.1f)
        discard;
    
    float fDissolveValue = g_DissolveTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (fDissolveValue < g_DissolveValue)
    {
        float fResult = smoothstep(0.0f, 1.0f, g_DissolveValue * 0.5f);
        vDiffuse.rgb = lerp(float3(0.3f, 0.7f, 0.6f), vDiffuse.rgb, fResult);
    }
    
    if (fDissolveValue < g_DissolveValue - 0.01f)
    {
        clip(-1);
    }
    
    float fGlowFactor = g_fObjectAlpha;

    float3 vTargetColor = float3(0.3f, 0.7f, 0.6f);
    //float3 vTargetColor = float3(0.2f, 1.0f, 0.9f); 
    float fMixValue = 0.7f;

    float3 glowColor = lerp(vDiffuse.rgb, vTargetColor, fMixValue) * fGlowFactor * 0.5f;

    Out.vColor.rgb = glowColor;
    Out.vColor.a = 1.0f;

    return Out;
}

PS_OUT_LAMP PS_LAMP(PS_IN In)
{
    PS_OUT_LAMP Out = (PS_OUT_LAMP) 0;

    float2 vTexCoord = In.vTexcoord;
    
    float2 vLampCenter = float2(0.25f, 1.0f);
    float fRadius = 0.24f;
    float fDist = distance(vTexCoord, vLampCenter);
    float fGlowValue = smoothstep(fRadius, fRadius - 0.02f, fDist);

    float3 vGlowColor = float3(1.0f, 0.9f, 0.6f);
    float fGlowPower = fGlowValue * g_fObjectAlpha * 3.0f;

    Out.vLampGlow.rgb = vGlowColor * fGlowPower;
    Out.vLampGlow.a = fGlowValue;

    return Out;
}


PS_OUT_GLOW PS_WEAPON_GLOW(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;
    
    
    if (g_Appear)
    {
        In.vTexcoord.x += g_Time * 5.f;
        //In.vTexcoord.y += g_Time * 10.f;      
    
        float3 Color = float3(0.3f, 1.f, 0.7f);
        Out.vGlow.rgb = Color;
        Out.vGlow.a = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord).r / 3.f;

        float2 noiseUV = In.vTexcoord;
    
        float noiseValue = g_DissolveNoiseTexture.Sample(LinearSampler_Clamp, noiseUV).r;
  
        if ((1.0 - noiseValue) > g_ReverseDissolveTime)
        {
            clip(-1);
        }
    }
    
    
    //if (!g_Dead && !g_Appear)
    //{
    
    //    In.vTexcoord.x += g_Time * 5.f;
    ////In.vTexcoord.y += g_Time * 10.f;      
    
    //    float3 Color = float3(0.3f, 1.f, 0.7f);
    //    Out.vGlow.rgb = Color;
    //    Out.vGlow.a = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord).r / 3.f;
    //}
    
    if (g_Dead)
    {

        float2 noiseUV = In.vTexcoord;
    
        float noiseValue = g_DissolveNoiseTexture.Sample(LinearSampler_Clamp, noiseUV).r;
  
        if (noiseValue < g_DissolveAmount)
        {
            clip(-1);
        }
    }
   
    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass //0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    
    pass DissolvePass //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
    }

    pass Shadow //2 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = NULL;
    }

    pass Distortion //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
    }

    pass Glow //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }

    pass MotionBlur //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_MOTIONBLUR();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MOTIONBLUR();
    }

    pass Occulsion //6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_OCCULUSION();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_OCCULUSION();
    }

    pass WeightBlend //7 매쉬용 WeightBlend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_WeightBlend, 0);
        SetBlendState(BS_WeightBlend_Client, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WEIGHTBLEND();
    }

    pass ItemGlow //8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ITEM_GLOW();
    }

    pass MONSTER_WEAPON_DISSOLVE //9
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MONSTER_WEAPON_DISSOLVE();
    }

    pass CullNonePass // 10
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass ObjectRimLight // 11
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_OBJECT_RIMLIGHT();
    }

    pass DeadBranchPass // 12
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DEAD_BRANCH();
    }

    pass LampPass // 13
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_LAMP();
    }



    pass WeaponGlowPass // 14  Weapon 무기용 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_WEAPON_GLOW();
    }

}

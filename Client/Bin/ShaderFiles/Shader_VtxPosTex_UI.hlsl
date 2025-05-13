#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
Texture2D g_TexIcon;
Texture2D g_TexEdge;
Texture2D g_TexEffect;
Texture2D g_TexNoise;
Texture2D g_TexGlow;

bool g_bIconOn; // 아이콘 이미지  onoff
bool g_bSkillOn; // 아이템 슬롯 이미지 열고 닫기

float g_fTimeDelta; // 인게임 업데이트 시간값
float g_fTImeAlpha; // 시간에 따른 알파 값

bool g_bImageOn; // True 값일 때 이미지 모습이 보임
bool g_bImageLoopOn; // true 값일 때 이미지 모습이 깜박깜박 보임 

bool g_bTexIconOff; // true 값일 때 이미지 모습이 깜박깜박 보임 
bool g_bTexEdgeOff; // true 값일 때 이미지 모습이 깜박깜박 보임 
bool g_bTexEffectOff; // true 값일 때 이미지 모습이 깜박깜박 보임 

uint g_SlotNum; // 플레이어 화면 스킬 슬롯 두개의 사이즈가 달라 스킬 아이콘과의 비율을 맞추기 위해 받아옴  

float g_fGageBar_Max;
float g_fGageBar_Current;
float g_fGageBar_Delay;

bool g_bIconChange;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vWorldPos = Out.vPosition;

    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    return Out;
}

PS_OUT PS_Thymesia_UI(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    return Out;
}

PS_OUT PS_Thymesia_UI_ImageOpen(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor.a *= abs(g_fTImeAlpha);
    
    if (!g_bImageOn)
    {
        Out.vColor.a = 0.0f;
        
    }
    
    return Out;
}
PS_OUT PS_Thymesia_UI_ImageOnOff(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (true == g_bImageOn)
    {
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    }
    else
    {
        Out.vColor.a = 0.0f;

    }
    

    return Out;
}

PS_OUT PS_Thymesia_UI_ImageOnOffLoop(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (true == g_bImageLoopOn) // 루프 값이 켜졌을 때
    {
        Out.vColor.a *= abs(g_fTImeAlpha);

    }
    else
    {
        Out.vColor.a = 0.0f;
    }
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_ItemSlot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float4 vBackColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vFrontColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    
    if (!g_bIconOn)
        vFrontColor.a = 0.0f;
    
    Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
    
    return Out;
}
PS_OUT PS_Thymesia_UI_Image_Pause(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.a -= Out.vColor.a / 2.0f;
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_TalentSlot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 ChangeUV = ((In.vTexcoord - 0.5) * 2.0 + 0.5f);
    ChangeUV = saturate(ChangeUV); // UV 범위를 [0, 1]로 제한

    float4 vTexColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vTexIconColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    float4 vTexEdgeColor = g_TexEdge.Sample(LinearSampler, In.vTexcoord);
    float4 vTexEffectColor = g_TexEffect.Sample(LinearSampler, In.vTexcoord);

    
    
    if (g_bTexIconOff) // 비활성화 특성 아이콘 어둡게 처리
        vTexIconColor.a -= max(0.0f, vTexIconColor.a - 0.6f); // 음수 안나오게
    
    if (g_bTexEdgeOff)// 호버 이미지 반짝 반짝
        vTexEdgeColor.a = 0.0f;
    else
        vTexEdgeColor.a *= abs(g_fTImeAlpha);
        
    
    if (g_bTexEffectOff)// 이펙트 이미지 끄기
        vTexEffectColor.a = 0.0f;
    
    
    vTexEffectColor = lerp(vTexEffectColor, vTexColor, saturate(vTexColor.a));
    vTexEffectColor = lerp(vTexEffectColor, vTexEdgeColor, saturate(vTexEdgeColor.a));
    Out.vColor = lerp(vTexEffectColor, vTexIconColor, saturate(vTexIconColor.a));

    if (g_bTexIconOff && g_bTexEdgeOff && g_bTexEffectOff)
    {
        Out.vColor = vTexColor;
        Out.vColor = lerp(Out.vColor, vTexIconColor, saturate(vTexIconColor.a));

    }
    
    
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_PlunderSlot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    //saturate(g_fTimeDelta);
    float fEdgeWidth = 0.05; // 경계선 두께 0.03 ~ 0.1 값이 일반적 
    float4 vEdgeColor = float4(1.f, 0.f, 0.f, 1.f); 
    float4 vBackColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vIconColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    float fNoise = g_TexNoise.Sample(LinearSampler, In.vTexcoord + g_fTimeDelta).r;
   
    if (g_bSkillOn) // 현재 스킬이 켜져 있으면 
        vBackColor.rgb = ((vBackColor.rgb - 0.5) * 0.5) + 0.5; // 각각 딤드처리
    if (!g_bIconOn && !g_bSkillOn) // 스킬 이미지가 없고 , 스킬 쿨이 돌고 있지 않다면  
        vBackColor.rgb = ((vBackColor.rgb - 0.5) * 0.5) + 0.5; //
    
    vIconColor.a *= saturate(fNoise + g_fTimeDelta);
    if (vIconColor.a <= 0.01f)
        vIconColor.a = 0.0f;
    if (!g_bIconOn)
        vIconColor.a = 0.0f;
    
    Out.vColor = lerp(vBackColor, vIconColor, vIconColor.a);
    
    
        return Out;
    }

PS_OUT PS_Thymesia_UI_Image_Skill_CoolTime(PS_IN In) // 스킬 사용 시 쿨타임 연출용
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    
    if (!g_bImageOn)
        Out.vColor.a = 0.0f;
    
    
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_HPBar(PS_IN In) // HP Bar 감소
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vBarColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vGlowColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //if (In.vTexcoord.x >= 0.9 && In.vTexcoord.x <= 1.0)
    //{
    //    vBarColor.rgb += vGlowColor.rgb * 0.1f;
    //}
    
    Out.vColor = vBarColor;
    
    
    float Percent = clamp(g_fGageBar_Current / g_fGageBar_Max, 0.0f, 1.0f);
    
    if (In.vTexcoord.x > Percent)
        discard;
    

    return Out;
}

PS_OUT PS_Thymesia_UI_Image_MPBar(PS_IN In) // MP Bar 감소
{
    PS_OUT Out = (PS_OUT) 0;
    float4 vFrontColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vBackColor = g_TexEffect.Sample(LinearSampler, In.vTexcoord);
    
    
    float Percent = clamp(g_fGageBar_Current / g_fGageBar_Max, 0.0f, 1.0f);
    float DelayPercent = clamp(g_fGageBar_Delay / g_fGageBar_Max, 0.0f, 1.0f);
    
    if (In.vTexcoord.x > Percent)
        vFrontColor.a = 0.0f;
   
    
    if (In.vTexcoord.x > DelayPercent)
    {
        //float Alpha = saturate((In.vTexcoord.x - Percent) / (1.0f - Percent)); // 0~1 사이 값으로
        //Alpha = pow(Alpha, 0.5f);
        vBackColor.a = 0.0f;// -  Alpha; // 좌 -> 우
        
    }
    else
        vBackColor.a = 1.0f;
    
      
    Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
  
    // Out.vColor = ((testValue < Percent) ? float4(1.0f, 0.0f, 0.0f, 1.0f) : float4(0.0f, 0.0f, 1.0f, 1.0f)); 조건 테스트용
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_Skill_Equip_Slot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float4 vTexColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vTexIconColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    float4 vTexEdgeColor = g_TexEdge.Sample(LinearSampler, In.vTexcoord);
    
    
    if (g_bTexIconOff) // 비활성화 특성 아이콘 어둡게 처리
        vTexColor.rgb = ((vTexColor.rgb - 0.5) * 0.5) + 0.5; 
        vTexIconColor.a -= max(0.0f, vTexIconColor.a - 0.6f); // 음수 안나오게

        //vTexIconColor.a -= max(0.0f, vTexIconColor.a - 0.6f); // 음수 안나오게
    if (g_bIconOn)
        vTexIconColor.a = 0;
    
    if (g_bTexEdgeOff)// 호버 이미지 반짝 반짝
        vTexEdgeColor.a = 0.0f;
    else
        vTexEdgeColor.a *= abs(g_fTImeAlpha);
        
        Out.vColor = vTexColor;
        Out.vColor = lerp(Out.vColor, vTexIconColor, saturate(vTexIconColor.a));

    return Out;
}

PS_OUT PS_Thymesia_UI_Image_SKill1st(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float2 ChangeUV = ((In.vTexcoord - 0.5) * 0.5 + 0.5f);
    ChangeUV = saturate(ChangeUV); // UV 범위를 [0, 1]로 제한
    
    // vEffectColor.rgb = ((vEffectColor.rgb - 0.1) * 0.5) + 0.5; // 대조하는거? // 딤드처리
    
    float4 vBackColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vEffectColor = g_TexEffect.Sample(LinearSampler, In.vTexcoord);
    float4 vFrontColor = g_TexIcon.Sample(LinearSampler, ChangeUV);
  
    if (!g_bIconOn) // 스킬 이미지가 없으면 
    {
        vFrontColor.a = 0.0f; // 아이콘 투명하게
        vBackColor.rgb = ((vBackColor.rgb - 0.5) * 0.5) + 0.5; //딤드처리
    }
    if(g_bSkillOn)
    {
        vFrontColor.rgb = ((vFrontColor.rgb - 0.5) * 0.5) + 0.5; // 각각 딤드처리
        vBackColor.rgb = ((vBackColor.rgb - 0.5) * 0.5) + 0.5;
    }
    
    
    
     /*끝나면 알파가 0.0f가 되어 안보임 */
    float fBase = smoothstep(g_fTImeAlpha / 5.0f, 0.0f, 1.0 - In.vTexcoord.y); // 1.0 - In.vTexcoord.y 아래에서 부터 위로
    float fBase2 = pow(fBase, 0.1f); // 자연스럽게 곡선으로 올라오겠끔  설정
    vEffectColor.a *= fBase2; 
   
    if (g_bIconChange)
    {
        float fIconBase = smoothstep(0.0f, 1.f, (g_fTimeDelta / 2) - (In.vTexcoord.y)); // 1.0 - In.vTexcoord.y 아래에서 부터 위로
        float fIconBase2 = pow(fIconBase, 0.1f); // 자연스럽게 곡선으로 올라오겠끔  설정
        vFrontColor.a *= fIconBase2; // * 0.9f;
   
        vBackColor = lerp(vBackColor, vEffectColor, vEffectColor.a);
        Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
  
    }
    else
    {
        float fIconBase = smoothstep(0.0f, 1.f, g_fTimeDelta - (In.vTexcoord.x + In.vTexcoord.y)); // 1.0 - In.vTexcoord.y 아래에서 부터 위로
        float fIconBase2 = pow(fIconBase, 0.1f); // 자연스럽게 곡선으로 올라오겠끔  설정
        vFrontColor.a *= fIconBase2; // * 0.9f;
   
        vBackColor = lerp(vBackColor, vEffectColor, vEffectColor.a);
        Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
  
    }
    
        return Out;
    }


PS_OUT PS_Thymesia_UI_Image_PoisonBar(PS_IN In) // HP Bar 감소
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vBarColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
   // float2 fNoiseUV = In.vTexcoord * 2.0 + g_fTimeDelta;
    
   // float fNoise = g_TexNoise.Sample(LinearSampler, fNoiseUV).r;
    
   // float fShineing = 0.5f + 0.5f * sin(g_fTimeDelta * 1.2f);
    
    
    
    float fRate = clamp(g_fGageBar_Current / g_fGageBar_Max, 0.0f, 1.0f);
    
    if (In.vTexcoord.x > fRate)
        vBarColor.rgb *= 0.1f;
    
    //vBarColor.a *= saturate(fNoise + g_fTimeDelta);
    
    
    //float4 vGlowColor = (1.0f, 1.0f, 1.0f, 1.0f);
    
    ////if (In.vTexcoord.x >= 0.9 && In.vTexcoord.x <= 1.0)
    ////{
    ////    vBarColor.rgb += vGlowColor.rgb * 0.1f;
    ////}
    
    
    
    //float Percent = clamp(g_fGageBar_Current / g_fGageBar_Max, 0.0f, 1.0f);
    
    //if (In.vTexcoord.x > Percent)
    //    discard;
    
    Out.vColor = vBarColor;

    return Out;
}

technique11 DefaultTechnique
{
  // 0번
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Thymesia_UI // 1번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI();
    }

    pass Thymesia_UI_ImageOnOff // 2번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOnOff();
    }

    pass Thymesia_UI_ImageOnOffLoop // 3번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOnOffLoop();
    }

    pass Thymesia_UI_Image_ItemSlot // 4번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_ItemSlot();
    }
    
    pass Thymesia_UI_Image_Pause // 5번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_Pause();
    }
    

    pass Thymesia_UI_Image_TalentSlot // 6번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_TalentSlot();
    }
    
    pass Thymesia_UI_Image_PlunderSlot // 7번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_PlunderSlot();
    }
    
    pass Thymesia_UI_Image_SkillIcon // 8번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_Skill_CoolTime();
    }

    pass Thymesia_UI_Image_HPBar // 9번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_HPBar();
    }
     pass Thymesia_UI_Image_Skill // 10번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_Skill_Equip_Slot();
    }
     pass Thymesia_UI_ImageOpen // 11번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOpen();
    }

    pass Thymesia_UI_Image_SKill1st // 12번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_SKill1st();
    }
    
    pass Thymesia_UI_Image_MPBar // 13번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_MPBar();
    }
    
    pass Thymesia_UI_Image_PoisonBar // 14번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_PoisonBar();
    }
    
}

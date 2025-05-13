#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_OldViewMatrix, g_OldWorldMatrix;
Texture2D g_DiffuseTexture;

float g_fHeight;
float g_fRotation_Weight;
float g_TimeX;
float g_TimeY;
float g_fHorizonSpeed;

float g_fMaxTimer;

float4 g_vModelPosition;

float4 g_Explosion_Horizon1;
float4 g_Explosion_Vertical1;
float4 g_Explosion_Rotation1;
float4 g_Explosion_Horizon2;
float4 g_Explosion_Vertical2;
float4 g_Explosion_Rotation2;
float4 g_Explosion_Horizon3;
float4 g_Explosion_Vertical3;
float4 g_Explosion_Rotation3;



struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT_POMPEII
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
    float4 vTangent : TEXCOORD3;
    float4 vBinormal : TEXCOORD4;
};

VS_OUT_POMPEII VS_MAIN_POMPEII(VS_IN In)
{
    VS_OUT_POMPEII Out = (VS_OUT_POMPEII) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), matWV));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
	
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f);
	
    return Out;
}

struct GS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
    float4 vTangent : TEXCOORD3;
    float4 vBinormal : TEXCOORD4;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

float g_fGravity = -9.8f;



#define PARTS_COUNT 12 // 13 ���� �ȵǴ�����..?������;;
[maxvertexcount(3 * PARTS_COUNT)]
void GS_MAIN(triangle GS_IN input[3], inout TriangleStream<GS_OUT> triStream)
{
    float g_fHorizon[12] =
    {
        g_Explosion_Horizon1.x, g_Explosion_Horizon1.y, g_Explosion_Horizon1.z, g_Explosion_Horizon1.w
        , g_Explosion_Horizon2.x, g_Explosion_Horizon2.y, g_Explosion_Horizon2.z, g_Explosion_Horizon2.w
        , g_Explosion_Horizon3.x, g_Explosion_Horizon3.y, g_Explosion_Horizon3.z, g_Explosion_Horizon3.w
    };
    float g_fVertical[12] =
    {
        g_Explosion_Vertical1.x, g_Explosion_Vertical1.y, g_Explosion_Vertical1.z, g_Explosion_Vertical1.w
        , g_Explosion_Vertical2.x, g_Explosion_Vertical2.y, g_Explosion_Vertical2.z, g_Explosion_Vertical2.w
        , g_Explosion_Vertical3.x, g_Explosion_Vertical3.y, g_Explosion_Vertical3.z, g_Explosion_Vertical3.w
    };
    float g_fRotation[12] =
    {
        g_Explosion_Rotation1.x * 10.f, g_Explosion_Rotation1.y * 10.f, g_Explosion_Rotation1.z * 10.f, g_Explosion_Rotation1.w * 10.f
        , g_Explosion_Rotation2.x * 10.f, g_Explosion_Rotation2.y * 10.f, g_Explosion_Rotation2.z * 10.f, g_Explosion_Rotation2.w * 10.f
        , g_Explosion_Rotation3.x * 10.f, g_Explosion_Rotation3.y * 10.f, g_Explosion_Rotation3.z * 10.f, g_Explosion_Rotation3.w * 10.f
    };
    for (int iPartIndex = 0; iPartIndex < PARTS_COUNT; ++iPartIndex)
    {
        float fAngle = (2.0f * 3.141592f) * (iPartIndex / (float) PARTS_COUNT);
        
        float3 vMoveDir = normalize(float3(cos(fAngle), 1.0f, sin(fAngle)));
        
        float3 vMoveValue = vMoveDir * (g_fHorizonSpeed * g_fHorizon[iPartIndex]); //�̰� ���������� ��������� �󸶳� �־�����, ����1.5f

        float fFallingTime = g_TimeX;
        
        float fInitialPopVelocity = g_fHeight * g_fVertical[iPartIndex]; //�̰ŵ� ���������� ��������� �󸶳� �ö󰬴ٰ� ��������, ����1.0f
        
        float fFallingValue = (fInitialPopVelocity * fFallingTime) + (0.4f * g_fGravity * fFallingTime * fFallingTime);
        vMoveValue.y += fFallingValue;

        float3 vRotationAxis = normalize(float3(cos(fAngle), 0.5f, sin(fAngle)));

        float fRotationSpeed = g_fRotation[iPartIndex] * g_fRotation_Weight; // �̰͵� ���������� �����ش� �󸶳� ��۹�� ȥ�� ȸ���ϴ°�, ����10.0f
        
        float fRotationAngle = g_TimeX * fRotationSpeed;
        float fCosAngle = cos(fRotationAngle);
        float fSinAngle = sin(fRotationAngle);

        float x = vRotationAxis.x;
        float y = vRotationAxis.y;
        float z = vRotationAxis.z;
        float3x3 matRotation =
        {
            fCosAngle + ((x * x) * (1 - fCosAngle)), (1 - fCosAngle) * x * y - fSinAngle * z, (1 - fCosAngle) * x * z + fSinAngle * y,
            (1 - fCosAngle) * x * y + fSinAngle * z, fCosAngle + ((y * y) * (1 - fCosAngle)), (1 - fCosAngle) * y * z - fSinAngle * x,
            (1 - fCosAngle) * x * z - fSinAngle * y, (1 - fCosAngle) * y * z + fSinAngle * x, fCosAngle + ((z * z) * (1 - fCosAngle))
        };
        
        
        // ���� �� �� ���� [0] [1] [2] 3���� �ȼ� ���̴��� ���� �غ��ؿ�.
        for (int i = 0; i < 3; ++i)
        {
            GS_OUT Out = (GS_OUT) 0;

            Out.vTexcoord = input[i].vTexcoord;
            Out.vNormal = input[i].vNormal;
            Out.vTangent = input[i].vTangent;
            Out.vBinormal = input[i].vBinormal;

            // ������ ���� �߽��� �������� ���������ؼ� ȸ�� �������� �� �߽����� ��ƿ�
            // �� �߽� �������� ������ ������ �󸶳� ������ �ִ����� ���� ����, 
            // ex). �� �̹� ������ ���� ��ġ�� ���� �߽ɺ��� 2 ���� ���� �־�.
            
            float3 vLocalOffset = input[i].vWorldPos.xyz - g_vModelPosition.xyz;
            
            // ���� �� 2���ٰ� ȸ���� �ϴ� �õ帮�Խ� ȸ����� ������ �ƿ�������
            float3 vRotated = mul(vLocalOffset, matRotation);
            
            // �׸��� �� ���� ���� ���� ��ġ�� �� ���� ȸ�� ���ΰŸ� �ξ� �ƿ��ָ� ���ư��� ���� �߽����� 
            // ����������, ���� ���� ��ġ�� + ȸ�� �Ȱ� + �� �̵����� ���� ������ ä�����.
            float3 vFinalPos = g_vModelPosition.xyz + vRotated + vMoveValue;

            // ���� Out.vWorldPos �� ��ġ ������ ��Ƽ� �Ƚ��� ������!! 
            Out.vWorldPos = float4(vFinalPos, 1);
            Out.vPosition = mul(Out.vWorldPos, mul(g_ViewMatrix, g_ProjMatrix));
            triStream.Append(Out);
        }
        
        // ���� For�� �� ���� ��. ��� ���� �� ���� �غ�����. ������ PARTS_COUNT - 1�� ���Ѵٱ�.
        triStream.RestartStrip();
    }
}


struct PS_IN_POMPEII
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

struct PS_OUT_POMPEII
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT_POMPEII PS_POMPEII(PS_IN_POMPEII In)
{
    PS_OUT_POMPEII Out = (PS_OUT_POMPEII) 0;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    //float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    //float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
	
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    //vNormal = normalize(mul(vNormal, WorldMatrix));
	
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = 0.f; //vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    
    return Out;
}


technique11 DefaultTechnique
{
    pass Pompeii //10 GeometryShader
    {
        SetRasterizerState(Rs_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_POMPEII();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_POMPEII();
    }
}

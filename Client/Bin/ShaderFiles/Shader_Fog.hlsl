#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_FrontTexture;
Texture2D g_BackTexture;
Texture2D g_FinalTexture;
Texture2D g_DepthTexture;
Texture2D g_PreviousFogTexture;

float4x4 g_ViewMatrixInv;
float4x4 g_ProjMatrixInv;

float4 g_vCubePos;
float3 g_vCubeExtents;
float4 g_vCamPos;

Texture3D g_VolumeFogTexture;

float g_fLifeTime;

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
};

VS_OUT VS_MAIN(uint id : SV_VertexID)
{
    VS_OUT Out = (VS_OUT) 0;
    
    Out.vPosition = mul(vector(0.f, 0.f, 0.f, 1.f), g_WorldMatrix);

    return Out;
}


struct GS_IN_FOG
{
    float4 vPosition : SV_POSITION;
};
struct GS_OUT_FOG
{
    float4 vPosition : SV_POSITION;
    float4 vWorldPos : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

float2 ComputeBackFaceUV(float3 pos, float minX, float minY, float maxX, float maxY)
{
    float u = (pos.x - minX) / (maxX - minX);
    float v = (pos.y - minY) / (maxY - minY);
    return float2(u, v);
}

float2 ComputeSideFaceUV(float3 pos, float minX, float minY, float minZ, float maxZ, float maxY)
{
    float u = (pos.z - minZ) / (maxZ - minZ);
    float v = (pos.y - minY) / (maxY - minY);
    return float2(u, v);
}

float2 ComputeTopBottomFaceUV(float3 pos, float minX, float minY, float maxX, float maxY, float maxZ, float minZ)
{
    float u = (pos.x - minX) / (maxX - minX);
    float v = (pos.z - minZ) / (maxZ - minZ);
    return float2(u, v);
}
float3 ComputeNormal(float3 a, float3 b, float3 c)
{
    float3 ab = b - a;
    float3 ac = c - a;
    return normalize(cross(ab, ac)).xyz; // 오른손 법칙
}
[maxvertexcount(36)]
void GS_MAIN_FOG(point GS_IN_FOG In[1], inout TriangleStream<GS_OUT_FOG> triStream)
{
    float3 center = In[0].vPosition.xyz; // 월드 정점 
    float3 halfSize = g_vCubeExtents.x * 0.5; // 5.f 가 cubesize 임의 지정 나중에 수정
    float3 halfhalfSize = g_vCubeExtents.y * 0.5; // 5.f 가 cubesize 임의 지정 나중에 수정
        
    float3 v0 = center + float3(-halfSize.x, -halfhalfSize.y, -halfSize.z);
    float3 v1 = center + float3(halfSize.x, -halfhalfSize.y, -halfSize.z);
    float3 v2 = center + float3(halfSize.x, halfhalfSize.y, -halfSize.z);
    float3 v3 = center + float3(-halfSize.x, halfhalfSize.y, -halfSize.z);
    float3 v4 = center + float3(-halfSize.x, -halfhalfSize.y, halfSize.z);
    float3 v5 = center + float3(halfSize.x, -halfhalfSize.y, halfSize.z);
    float3 v6 = center + float3(halfSize.x, halfhalfSize.y, halfSize.z);
    float3 v7 = center + float3(-halfSize.x, halfhalfSize.y, halfSize.z);
        
    float minX = center.x - halfSize.x;
    float maxX = center.x + halfSize.x;
    float minY = center.y - halfSize.y;
    float maxY = center.y + halfSize.y;
    float minZ = center.z - halfSize.z;
    float maxZ = center.z + halfSize.z;
        
    
    matrix matWVP;
    
    matWVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    GS_OUT_FOG element;
        
    float3 normalFront = ComputeNormal(v4, v5, v6);
    
    element.vWorldPos = float4(v4, 1.f);
    element.vPosition = mul(float4(v4, 1.f), matWVP); // 앞면
    element.vTexcoord = ComputeBackFaceUV(v4, minX, minY, maxX, maxY);
    triStream.Append(element);
    
    element.vWorldPos = float4(v5, 1.f);
    element.vPosition = mul(float4(v5, 1.f), matWVP);
    element.vTexcoord = ComputeBackFaceUV(v5, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v6, 1.f);
    element.vPosition = mul(float4(v6, 1.f), matWVP);
    element.vTexcoord = ComputeBackFaceUV(v6, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    triStream.RestartStrip();
             
    normalFront = ComputeNormal(v4, v6, v7);
    
    element.vWorldPos = float4(v4, 1.f);
    element.vPosition = mul(float4(v4, 1.f), matWVP); // 앞면
    element.vTexcoord = ComputeBackFaceUV(v4, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v6, 1.f);
    element.vPosition = mul(float4(v6, 1.f), matWVP);
    element.vTexcoord = ComputeBackFaceUV(v6, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v7, 1.f);
    element.vPosition = mul(float4(v7, 1.f), matWVP);
    element.vTexcoord = ComputeBackFaceUV(v7, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    triStream.RestartStrip();
        
        
    normalFront = ComputeNormal(v0, v2, v1);
    
    element.vWorldPos = float4(v0, 1.f);
    element.vPosition = mul(float4(v0, 1.f), matWVP); // 뒷면
    element.vTexcoord = ComputeBackFaceUV(v0, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v2, 1.f);
    element.vPosition = mul(float4(v2, 1.f), matWVP);
    element.vTexcoord = ComputeBackFaceUV(v2, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v1, 1.f);
    element.vPosition = mul(float4(v1, 1.f), matWVP);
    element.vTexcoord = ComputeBackFaceUV(v1, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    triStream.RestartStrip();
        
    normalFront = ComputeNormal(v0, v3, v2);
        
    element.vWorldPos = float4(v0, 1.f);
    element.vPosition = mul(float4(v0, 1.f), matWVP); // 뒷면
    element.vTexcoord = ComputeBackFaceUV(v0, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v3, 1.f);
    element.vPosition = mul(float4(v3, 1.f), matWVP);
    element.vTexcoord = ComputeBackFaceUV(v3, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v2, 1.f);
    element.vPosition = mul(float4(v2, 1.f), matWVP);
    element.vTexcoord = ComputeBackFaceUV(v2, minX, minY, maxX, maxY);
    triStream.Append(element);
        
    triStream.RestartStrip();
        
        
    normalFront = ComputeNormal(v0, v4, v7);
    
    element.vWorldPos = float4(v0, 1.f);
    element.vPosition = mul(float4(v0, 1.f), matWVP); // 좌측면
    element.vTexcoord = ComputeSideFaceUV(v0, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v4, 1.f);
    element.vPosition = mul(float4(v4, 1.f), matWVP);
    element.vTexcoord = ComputeSideFaceUV(v4, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v7, 1.f);
    element.vPosition = mul(float4(v7, 1.f), matWVP);
    element.vTexcoord = ComputeSideFaceUV(v7, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    triStream.RestartStrip();
        
        
    normalFront = ComputeNormal(v0, v7, v3);
    
    element.vWorldPos = float4(v0, 1.f);
    element.vPosition = mul(float4(v0, 1.f), matWVP); // 좌측면
    element.vTexcoord = ComputeSideFaceUV(v0, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v7, 1.f);
    element.vPosition = mul(float4(v7, 1.f), matWVP);
    element.vTexcoord = ComputeSideFaceUV(v7, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v3, 1.f);
    element.vPosition = mul(float4(v3, 1.f), matWVP);
    element.vTexcoord = ComputeSideFaceUV(v3, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    triStream.RestartStrip();
        
        
    normalFront = ComputeNormal(v1, v6, v5);
    
    element.vWorldPos = float4(v1, 1.f);
    element.vPosition = mul(float4(v1, 1.f), matWVP); // 우측면
    element.vTexcoord = ComputeSideFaceUV(v1, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v6, 1.f);
    element.vPosition = mul(float4(v6, 1.f), matWVP);
    element.vTexcoord = ComputeSideFaceUV(v6, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v5, 1.f);
    element.vPosition = mul(float4(v5, 1.f), matWVP);
    element.vTexcoord = ComputeSideFaceUV(v5, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    triStream.RestartStrip();
                
    normalFront = ComputeNormal(v1, v2, v6);
        
    element.vWorldPos = float4(v1, 1.f);
    element.vPosition = mul(float4(v1, 1.f), matWVP); // 우측면
    element.vTexcoord = ComputeSideFaceUV(v1, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v2, 1.f);
    element.vPosition = mul(float4(v2, 1.f), matWVP);
    element.vTexcoord = ComputeSideFaceUV(v2, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    element.vWorldPos = float4(v6, 1.f);
    element.vPosition = mul(float4(v6, 1.f), matWVP);
    element.vTexcoord = ComputeSideFaceUV(v6, minX, minY, minZ, maxZ, maxY);
    triStream.Append(element);
        
    triStream.RestartStrip();
        
        
    normalFront = ComputeNormal(v4, v1, v5);
        
    element.vWorldPos = float4(v4, 1.f);
    element.vPosition = mul(float4(v4, 1.f), matWVP); // 상단면
    element.vTexcoord = ComputeTopBottomFaceUV(v4, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    element.vWorldPos = float4(v1, 1.f);
    element.vPosition = mul(float4(v1, 1.f), matWVP);
    element.vTexcoord = ComputeTopBottomFaceUV(v1, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    element.vWorldPos = float4(v5, 1.f);
    element.vPosition = mul(float4(v5, 1.f), matWVP);
    element.vTexcoord = ComputeTopBottomFaceUV(v5, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    triStream.RestartStrip();
                
        
    normalFront = ComputeNormal(v4, v0, v1);
    
    element.vWorldPos = float4(v4, 1.f);
    element.vPosition = mul(float4(v4, 1.f), matWVP); // 상단면
    element.vTexcoord = ComputeTopBottomFaceUV(v4, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    element.vWorldPos = float4(v0, 1.f);
    element.vPosition = mul(float4(v0, 1.f), matWVP);
    element.vTexcoord = ComputeTopBottomFaceUV(v0, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    element.vWorldPos = float4(v1, 1.f);
    element.vPosition = mul(float4(v1, 1.f), matWVP);
    element.vTexcoord = ComputeTopBottomFaceUV(v1, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    triStream.RestartStrip();
        
        
    normalFront = ComputeNormal(v7, v6, v2);
    
        
    element.vWorldPos = float4(v7, 1.f);
    element.vPosition = mul(float4(v7, 1.f), matWVP); // 하단면
    element.vTexcoord = ComputeTopBottomFaceUV(v7, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    element.vWorldPos = float4(v6, 1.f);
    element.vPosition = mul(float4(v6, 1.f), matWVP);
    element.vTexcoord = ComputeTopBottomFaceUV(v6, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    element.vWorldPos = float4(v2, 1.f);
    element.vPosition = mul(float4(v2, 1.f), matWVP);
    element.vTexcoord = ComputeTopBottomFaceUV(v2, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    triStream.RestartStrip();
                
    normalFront = ComputeNormal(v3, v7, v2);
        
    element.vWorldPos = float4(v3, 1.f);
    element.vPosition = mul(float4(v3, 1.f), matWVP); // 하단면
    element.vTexcoord = ComputeTopBottomFaceUV(v3, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    element.vWorldPos = float4(v7, 1.f);
    element.vPosition = mul(float4(v7, 1.f), matWVP);
    element.vTexcoord = ComputeTopBottomFaceUV(v7, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    element.vWorldPos = float4(v2, 1.f);
    element.vPosition = mul(float4(v2, 1.f), matWVP);
    element.vTexcoord = ComputeTopBottomFaceUV(v2, minX, minY, maxX, maxY, maxZ, minZ);
    triStream.Append(element);
        
    triStream.RestartStrip();
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vWorldPos : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


PS_OUT PS_MAIN_FOG(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = float4(In.vWorldPos.xyz, 1);
    
    return Out;
}

struct VS_OUT_FINAL
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT_FINAL VS_MAIN_FINAL(uint id : SV_VertexID)
{
    VS_OUT_FINAL Out = (VS_OUT_FINAL) 0;
    
    float2 ndcPos[4] =
    {
        float2(-1.0f, 1.0f),
        float2(1.0f, 1.0f),
        float2(-1.0f, -1.0f),
        float2(1.0f, -1.0f)
    };

    float2 uv[4] =
    {
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f),
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f)
    };

    Out.vPosition = float4(ndcPos[id], 0.0f, 1.0f);
    Out.vTexcoord = uv[id];

    return Out;
}


struct PS_IN_FINAL
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT_FINAL
{
    float vColor : SV_TARGET0;
};


PS_OUT_FINAL PS_MAIN(PS_IN_FINAL In)
{
    PS_OUT_FINAL Out = (PS_OUT_FINAL) 0;
    
    float3 frontPos = g_FrontTexture.Sample(LinearSampler, In.vTexcoord).rgb;
    float3 backPos = g_BackTexture.Sample(LinearSampler, In.vTexcoord).rgb;
    
    float3 vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord).rgb;
    float fViewZ = vDepthDesc.y;
	
    vector vWorldPos;
	
	/* 투영공간상의 위치 */
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
   
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
    vWorldPos = vWorldPos.xyzw * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	/* 월드위치 */

	/* 로컬위치 * 월드행렬  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    float thickness = length(backPos - frontPos);
    
    if (thickness > 0.001f)
    {
        int numSteps = 32;
        
        float3 camPos = g_vCamPos.xyz;
        
        
        float3 rayDir;
        
        float rayLength, fogFactor;
        
        if (frontPos.x < 0.001f && frontPos.y < 0.001f && frontPos.z < 0.001f)
        {
            rayDir = normalize(backPos - g_vCamPos.xyz);
            rayLength = length(backPos - g_vCamPos.xyz);
            
            float stepSize = rayLength / float(numSteps);
        
            float totalDensity = 0.f;
        
            float radius = g_vCubeExtents.x / 2.f;
            float Height = g_vCubeExtents.y / 2.f;
        
            float totalFade = 0.0f;
            float totalWeight = 0.0f;
            float3 samplepos = g_vCamPos.xyz;
        
            [unroll]
            for (int i = 0; i < numSteps; ++i)
            {
                samplepos += rayDir * stepSize;

                //if (length(samplepos - camPos) > length(vWorldPos.xyz - camPos))
                //    continue;
                
                float3 offset = samplepos - g_vCubePos.xyz;
                float dist = length(offset);
                
                float fade = 1.0 - smoothstep(radius * 0.3, radius, dist);
                float Heightfade = 1.f - smoothstep(Height * 0.5f, Height, length(samplepos.y - g_vCubePos.y));
                
                float density = g_VolumeFogTexture.Sample(LinearSampler, float3(samplepos.x * 0.05f, samplepos.y * 0.05f, samplepos.z * 0.05f)).r * 0.8f;
                
                totalDensity += density * stepSize * fade * Heightfade;
            }
            
            fogFactor = 1.0 - exp(-totalDensity * 0.6f);
        }
        else
        {
            rayDir = normalize(backPos - frontPos);
            rayLength = length(backPos - frontPos);
            
            float stepSize = rayLength / float(numSteps);
        
            float3 centerPos = (backPos + frontPos) / 2.f;
        
            float totalDensity = 0.f;
        
            float radius = g_vCubeExtents.x / 2.f;
            float Height = g_vCubeExtents.y / 2.f;
        
            float totalFade = 0.0f;
            float totalWeight = 0.0f;
            float3 samplepos = frontPos;
        
            if (length(frontPos - camPos) > length(vWorldPos.xyz - camPos)) // front 텍스쳐의 깊이 체크
            {
                Out.vColor = 0.f;
        
                return Out;
            }
        
            [unroll]
            for (int i = 0; i < numSteps; ++i)
            {
                //if (length(samplepos - camPos) > length(vWorldPos.xyz - camPos))
                //    continue;
        
                
                samplepos += rayDir * stepSize;
                
                float3 offset = samplepos - g_vCubePos.xyz;
                float dist = length(offset);

                float fade = 1.0 - smoothstep(radius * 0.3, radius, dist);
                float Heightfade = 1.f - smoothstep(Height * 0.5f, Height, length(samplepos.y - g_vCubePos.y));
                
                float density = g_VolumeFogTexture.Sample(LinearSampler, float3(samplepos.x * 0.05f, samplepos.y * 0.05f, samplepos.z * 0.05f)).r * 0.8f;

                totalDensity += density * stepSize * fade * Heightfade;
            }
            
            fogFactor = 1.0 - exp(-totalDensity * 0.6f);
        }
        
        fogFactor *= 1.f - (g_fLifeTime / 2.f);
        
        Out.vColor = fogFactor;
    }
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Render_Back
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Skip_Always, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
   
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_FOG();
        PixelShader = compile ps_5_0 PS_MAIN_FOG();
    }

    pass Render_Front
    {
        SetRasterizerState(Rs_Cull_CW);
        SetDepthStencilState(DSS_Skip_Always, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
   
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_FOG();
        PixelShader = compile ps_5_0 PS_MAIN_FOG();
    }

    pass Render_Fog_Final
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
   
        VertexShader = compile vs_5_0 VS_MAIN_FINAL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}

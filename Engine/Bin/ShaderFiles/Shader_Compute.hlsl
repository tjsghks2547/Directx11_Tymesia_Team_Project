#include "Engine_Shader_Defines.hlsli"

struct Point_Particle
{
    float4 vRight;
    float4 vUp;
    float4 vLook;
    float4 vTranslation;
    float2 vLifeTime;
    float3 vSpeed;
    float3 vPivot;
    float3 vScale;
    float fDelayTime;
};

cbuffer GlobalPosition : register(b0)
{
    float4 g_vWorld;
}

cbuffer GlobalCamera : register(b1)
{
    float4 g_vCamPosition;
}

float g_fTime = 0.0167f; //1.f / 60.f //이거 안됨 상수버퍼로 던져야지 가능할듯 아직 안해봄

StructuredBuffer<Point_Particle> g_tInput_Compute : register(t0);
RWStructuredBuffer<Point_Particle> g_tOutput_Compute : register(u0);

groupshared Point_Particle sharedParticles[256];

[numthreads(256, 1, 1)]
void CSMain_Particle_Reset(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed = tInput.vSpeed * 1.f;
    sharedParticles[groupIndex].vRight = tInput.vRight * 1.f;
    sharedParticles[groupIndex].vUp = tInput.vUp * 1.f;
    sharedParticles[groupIndex].vLook = tInput.vLook * 1.f;
    sharedParticles[groupIndex].vTranslation.x = tInput.vTranslation.x * 1.f;
    sharedParticles[groupIndex].vTranslation.z = tInput.vTranslation.z * 1.f;
    sharedParticles[groupIndex].vTranslation.y = tInput.vTranslation.y * 1.f;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    sharedParticles[groupIndex].vLifeTime.y = 0.f;
    sharedParticles[groupIndex].fDelayTime = 0.f;
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Drop(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed = tInput.vSpeed * 1.f;
    sharedParticles[groupIndex].vRight = tInput.vRight * 1.f;
    sharedParticles[groupIndex].vUp = tInput.vUp * 1.f;
    sharedParticles[groupIndex].vLook = tInput.vLook * 1.f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vTranslation.xyz -= tInput.vSpeed.xyz * 0.0167f;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    
    if (tInput.vLifeTime.x < sharedParticles[groupIndex].vLifeTime.y)
    {
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz * 1.f + tInput.vTranslation.xyz * 1.f;
    }
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Explosion(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    sharedParticles[groupIndex].vSpeed = tInput.vSpeed * 1.f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.f;
    sharedParticles[groupIndex].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    vDir = vDir * tInput.vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Spark(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.01f;
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    sharedParticles[groupIndex].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * fScale;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * fScale;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * fScale;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Holding(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;
    float fSpeed = length(sharedParticles[groupIndex].vSpeed);
    
    if (fSpeed > length(0.0167f * tInput.vSpeed))
    {
        sharedParticles[groupIndex].vSpeed -= 0.0167f * tInput.vSpeed;
        vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Blood(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    float fSpeed = length(sharedParticles[groupIndex].vSpeed);
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f * 2.f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;
    
    if (fSpeed > length(0.0167f * tInput.vSpeed * 2.f))
    {
        sharedParticles[groupIndex].vSpeed -= 0.0167f * 2.f * tInput.vSpeed;
        vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Dust(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    float fScale = tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    sharedParticles[groupIndex].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz));
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * fScale;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * fScale;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * fScale;
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Dust_Delay(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    
    float3 vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz));
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vScale = 0.f;
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
        sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f * max(1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x), 0.f);
        vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    float fScale = tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    sharedParticles[groupIndex].vScale = tInput.vScale * fScale;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * sharedParticles[groupIndex].vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * sharedParticles[groupIndex].vScale.z;
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Hurricane(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed = tInput.vSpeed * 1.f;
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    
    float3 vDir = 0.f;
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        float fAngleX = (tInput.vTranslation.x + tInput.vTranslation.y + tInput.vTranslation.z) + tInput.vSpeed.x * sharedParticles[groupIndex].vLifeTime.y;
        float fAngleZ = (tInput.vTranslation.x + tInput.vTranslation.y + tInput.vTranslation.z) + tInput.vSpeed.z * sharedParticles[groupIndex].vLifeTime.y;
    
        sharedParticles[groupIndex].vTranslation.x = tInput.vTranslation.x + (tInput.vTranslation.x - tInput.vPivot.x) * cos(fAngleX);
        sharedParticles[groupIndex].vTranslation.z = tInput.vTranslation.z + (tInput.vTranslation.z - tInput.vPivot.z) * cos(fAngleZ);
        vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
        vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.y -= vDir.y;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    
    float fScale = tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    sharedParticles[groupIndex].vScale = tInput.vScale * fScale;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * sharedParticles[groupIndex].vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * sharedParticles[groupIndex].vScale.z;
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Scythe(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vScale = 0.f;
    }
    else
    {
        sharedParticles[groupIndex].vScale = tInput.vScale;
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        sharedParticles[groupIndex].vSpeed.x = tInput.vSpeed.x * max(1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x), 0.f);
        sharedParticles[groupIndex].vSpeed.yz -= tInput.vSpeed.yz * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
        vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    float3 vScale = tInput.vScale * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * vScale.z;
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Holding_World(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vScale = tInput.vScale * 1.f;
    float3 vDir = 0.f;
    
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        //sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz * 1.f + tInput.vTranslation.xyz * 1.f;
        
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        sharedParticles[groupIndex].vSpeed.x = tInput.vSpeed.x * max(1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x), 0.f);
        sharedParticles[groupIndex].vSpeed.yz -= tInput.vSpeed.yz * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
        vDir = sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.f;
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    sharedParticles[groupIndex].vScale = tInput.vScale * fScale;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * sharedParticles[groupIndex].vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * sharedParticles[groupIndex].vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Burst(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vScale = tInput.vScale * 1.f;
    sharedParticles[groupIndex].vSpeed.x = tInput.vSpeed.x * max(1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x), 0.f);
    sharedParticles[groupIndex].vSpeed.yz -= tInput.vSpeed.yz * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
    
    float3 vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz));
    
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vScale = 0.f;
        
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        
        vDir *= sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    
    sharedParticles[groupIndex].vLook = vector(normalize(g_vCamPosition.xyz - sharedParticles[groupIndex].vTranslation.xyz) * tInput.vScale.z, 0.f);
    sharedParticles[groupIndex].vRight = normalize(vector(cross(float3(0.f, 1.f, 0.f), sharedParticles[groupIndex].vLook.xyz), 0.f)) * sharedParticles[groupIndex].vScale.x;
    sharedParticles[groupIndex].vUp = normalize(vector(cross(sharedParticles[groupIndex].vLook.xyz, sharedParticles[groupIndex].vRight.xyz), 0.f)) * tInput.vScale.y;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}


[numthreads(256, 1, 1)]
void CSMain_Particle_World_Continue(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    if (tInput.vLifeTime.x <= sharedParticles[groupIndex].vLifeTime.y)
    {
        sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz + tInput.vTranslation.xyz;
        sharedParticles[groupIndex].vSpeed = tInput.vSpeed * 1.f;
        sharedParticles[groupIndex].fDelayTime = 0.f;

    }
    float3 vDir = 0.f;
    
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz * 1.f + tInput.vTranslation.xyz;
        
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        sharedParticles[groupIndex].vSpeed.xyz = tInput.vSpeed.xyz * max(0.4f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x), 0.01f);
        sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
        vDir = sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.f;
    
    if (fScale < 0.2f)
        fScale = 0.2f;
    
    sharedParticles[groupIndex].vScale = tInput.vScale * fScale;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * sharedParticles[groupIndex].vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * sharedParticles[groupIndex].vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Round(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vScale = tInput.vScale * 1.f;
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz; // * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    sharedParticles[groupIndex].vSpeed.y = tInput.vSpeed.y * (1.f - (sharedParticles[groupIndex].vLifeTime.y * 4.f / sharedParticles[groupIndex].vLifeTime.x));
    
    float3 vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz));
    //vDir = cross(float3(0.f, 1.f, 0.f), vDir);
    vDir *= sharedParticles[groupIndex].vSpeed * 0.0167f;
    
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vScale = 0.f;
        
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    
    float fScale = 1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    
    float3 vScale = tInput.vScale * fScale;
    
    sharedParticles[groupIndex].vLook = vector(normalize(g_vCamPosition.xyz - sharedParticles[groupIndex].vTranslation.xyz) * vScale.z, 0.f);
    sharedParticles[groupIndex].vRight = normalize(vector(cross(float3(0.f, 1.f, 0.f), sharedParticles[groupIndex].vLook.xyz), 0.f)) * vScale.x;
    sharedParticles[groupIndex].vUp = normalize(vector(cross(sharedParticles[groupIndex].vLook.xyz, sharedParticles[groupIndex].vRight.xyz), 0.f)) * vScale.y;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Falling_World(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    float3 vDir = 0.f;
    
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        //sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation = g_vWorld * 1.f;
        
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        sharedParticles[groupIndex].vSpeed.xyz = tInput.vSpeed.xyz * max(0.4f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x), 0.01f);
        sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
        vDir = sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.f;
    
    if (fScale < 0.2f)
        fScale = 0.2f;
    
    sharedParticles[groupIndex].vScale = tInput.vScale * fScale;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * sharedParticles[groupIndex].vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * sharedParticles[groupIndex].vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Spark_Update(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.01f;
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    sharedParticles[groupIndex].vScale = tInput.vScale * fScale;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * sharedParticles[groupIndex].vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * sharedParticles[groupIndex].vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_World_Continue_Gas(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    if (tInput.vLifeTime.x <= sharedParticles[groupIndex].vLifeTime.y)
    {
        sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz + tInput.vTranslation.xyz;
        sharedParticles[groupIndex].fDelayTime = 0.f;

    }
    float3 vDir = 0.f;
    float fScale = 0.f;
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.01f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz * 1.f + tInput.vTranslation.xyz;
        
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * max(0.4f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x), 0.01f);
        float fY = max((0.f - pow(sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x, 2.f)) * 2.f, -0.01f);
        sharedParticles[groupIndex].vSpeed.y = abs(tInput.vSpeed.y) * fY * -1.f;
        vDir = sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
        
        fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
        fScale = (abs(fScale - 0.5f) * -2.f) + 1.f;
    
        if (fScale < 0.2f)
            fScale = 0.2f;
    }
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * fScale;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * fScale;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * fScale;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_World_Blood(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    
    float fSpeed = length(tInput.vSpeed);
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        //sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz * 1.f + tInput.vTranslation.xyz;
        
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        
        if (fSpeed > length(sharedParticles[groupIndex].vLifeTime.y * tInput.vSpeed * 2.f))
            sharedParticles[groupIndex].vSpeed = (1.f - sharedParticles[groupIndex].vLifeTime.y * 2.f) * tInput.vSpeed;
    }
    
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;
    
    

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Round_Hurricane(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    float3 vDir_Right = 0.f;
    if (tInput.fDelayTime >= sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.01f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz * 1.f + tInput.vTranslation.xyz;
        
    }
    else
    {
        
        vDir = float3(normalize(g_vWorld.xyz * 1.f - sharedParticles[groupIndex].vTranslation.xyz)); // Pivot 으로 보내기
        vDir_Right = float3(normalize(cross(vDir, float3(0.f, 1.f, 0.f))));
        sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x;
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        
        sharedParticles[groupIndex].vSpeed = (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x)) * tInput.vSpeed;
        vDir_Right = vDir_Right * sharedParticles[groupIndex].vSpeed.x * 0.0167f;
    }
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= (vDir + vDir_Right);
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    float fScale = 1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.01f;
    
    if (fScale < 0.2f)
        fScale = 0.2f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * fScale * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * fScale * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * fScale * tInput.vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
    
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Falling_World_Axis(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    float3 vDir = 0.f;
    
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        //sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation = g_vWorld * 1.f;
        
    }
    else
    {
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        sharedParticles[groupIndex].vSpeed.xyz = tInput.vSpeed.xyz * max(0.4f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x), 0.01f);
        sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
        vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz) * sharedParticles[groupIndex].vSpeed * 0.0167f);
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.f;
    
    if (fScale < 0.2f)
        fScale = 0.2f;
    
    sharedParticles[groupIndex].vScale = tInput.vScale * fScale;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * sharedParticles[groupIndex].vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * sharedParticles[groupIndex].vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Dust_Continue(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.01f;
    
    if (fScale < 0.2f)
        fScale = 0.2f;
    
    sharedParticles[groupIndex].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz));
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * fScale;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * fScale;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * fScale;
    
    if (tInput.vLifeTime.x <= sharedParticles[groupIndex].vLifeTime.y)
    {
        sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = tInput.vTranslation.xyz;
    }
    
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Round_Hurricane_Version2(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    float3 vDir_Right = 0.f;
    if (tInput.fDelayTime >= sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.01f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = g_vWorld.xyz * 1.f + tInput.vTranslation.xyz;
        
    }
    else
    {
        
        vDir = float3(normalize(g_vWorld.xyz * 1.f - sharedParticles[groupIndex].vTranslation.xyz)); // Pivot 으로 보내기
        vDir_Right = float3(normalize(cross(vDir, float3(0.f, -1.f, 0.f))));
        sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x;
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        
        sharedParticles[groupIndex].vSpeed = (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x)) * tInput.vSpeed;
        vDir_Right = vDir_Right * sharedParticles[groupIndex].vSpeed.x * 0.0167f;
    }
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f * tInput.vPivot;
    sharedParticles[groupIndex].vTranslation.xyz -= (vDir + vDir_Right);
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    float fScale = 1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.01f;
    
    if (fScale < 0.01f)
        fScale = 0.0f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * fScale * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * fScale * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * fScale * tInput.vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
    
}


[numthreads(32, 1, 1)]
void CSMain_Particle_Holding_Version2(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;
    
    if (sharedParticles[groupIndex].vLifeTime.y < 0.4f)
        sharedParticles[groupIndex].vSpeed.xz -= 0.0167f * tInput.vSpeed.xz;
    else
    {
        sharedParticles[groupIndex].vSpeed = 0.f;
        sharedParticles[groupIndex].vSpeed.y -= 0.03f * tInput.vSpeed.y;
    }
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Drop_Version2(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed = tInput.vSpeed * 1.f;
    sharedParticles[groupIndex].vRight = tInput.vRight * 1.f;
    sharedParticles[groupIndex].vUp = tInput.vUp * 1.f;
    sharedParticles[groupIndex].vLook = tInput.vLook * 1.f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vTranslation.xyz -= tInput.vSpeed.xyz * 0.0167f;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    
    if (tInput.vLifeTime.x < sharedParticles[groupIndex].vLifeTime.y)
    {
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vTranslation.xyz = tInput.vTranslation.xyz * 1.f;
    }
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}


technique11 DefaultTechnique
{
    pass ParticleReset //0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Reset()));
    }

    pass ParticleDrop //1
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Drop()));
    }

    pass ParticleExplosion //2
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Explosion()));
    }

    pass ParticleSpark //3
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Spark()));
    }

    pass ParticleHolding //4
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Holding()));
    }

    pass ParticleBlood //5
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Blood()));
    }

    pass ParticleDust //6
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Dust()));
    }

    pass ParticleDust_Delay //7
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Dust_Delay()));
    }

    pass ParticleHurricane //8
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Hurricane()));
    }

    pass ParticleScythe //9
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Scythe()));
    }

    pass ParticleWorld //10
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Holding_World()));
    }

    pass ParticleBurst //11
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Burst()));
    }

    pass ParticleWorld_Continue //12
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_World_Continue()));
    }

    pass ParticleRound //13
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Round()));
    }

    pass ParticleFalling //14
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Falling_World()));
    }

    pass ParticleSparkUpdate //15
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Spark_Update()));
    }

    pass ParticleWorldGas //16
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_World_Continue_Gas()));
    }

    pass ParticleWorldBlood //17
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_World_Blood()));
    }

    pass Particle_Round_Hurricane //18
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Round_Hurricane()));
    }

    pass ParticleFalling_Axis //19
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Falling_World_Axis()));
    }

    pass ParticleDust_Continue //20
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Dust_Continue()));
    }

    pass Particle_Round_Hurricane_Version2 //21
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Round_Hurricane_Version2()));
    }

    pass Particle_Holding_Version2 //22
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Holding_Version2()));
    }

    pass Particle_Drop_Version2 //23
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Drop_Version2()));
    }

}
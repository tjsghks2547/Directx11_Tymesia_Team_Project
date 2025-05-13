#include "VIBuffer_Point_Compute.h"
#include "Shader_Compute.h"
#include "GameInstance.h"

CVIBuffer_Point_Compute::CVIBuffer_Point_Compute(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CVIBuffer(_pDevice, _pContext)
{
}

CVIBuffer_Point_Compute::CVIBuffer_Point_Compute(const CVIBuffer_Point_Compute& _Prototype)
	:CVIBuffer(_Prototype)
    , m_pUAV(_Prototype.m_pUAV)
    , m_pSRV(_Prototype.m_pSRV)
    , m_pBuffer_UAV(_Prototype.m_pBuffer_UAV)
    , m_pBuffer_SRV(_Prototype.m_pBuffer_SRV)
    , m_pBuffer_Copy(_Prototype.m_pBuffer_Copy)
    , m_pBuffer_StartPosition(_Prototype.m_pBuffer_StartPosition)
    , m_pBuffer_CameraPosition(_Prototype.m_pBuffer_CameraPosition)
    , m_InstanceBufferDesc(_Prototype.m_InstanceBufferDesc)
    , m_InstanceInitialData(_Prototype.m_InstanceInitialData)
    , m_iInstanceVertexStride(_Prototype.m_iInstanceVertexStride)
    , m_iNumIndexPerInstance(_Prototype.m_iNumIndexPerInstance)
    , m_iNumInstance(_Prototype.m_iNumInstance)
    , m_pVBInstance(_Prototype.m_pVBInstance)
    , m_pInstanceVertices(_Prototype.m_pInstanceVertices)
    , m_isLoop(_Prototype.m_isLoop)
{
    Safe_AddRef(m_pUAV);
    Safe_AddRef(m_pSRV);
    Safe_AddRef(m_pBuffer_UAV);
    Safe_AddRef(m_pBuffer_SRV);
    Safe_AddRef(m_pBuffer_Copy);
    Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Point_Compute::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer_Point_Compute::Initialize(void* _pArg)
{
    PARTICLE_COMPUTE_DESC* pDesc = static_cast<PARTICLE_COMPUTE_DESC*>(_pArg);

    m_iNumInstance = pDesc->iNumInstance;
    m_iNumVertices = 1;
    m_iVertexStride = sizeof(VTXPOSTEX);
    m_iInstanceVertexStride = sizeof(COMPUTE_PARTICLE_INSTANCE);
    m_iNumIndexPerInstance = 6;   /* 하나의 Rect 만드는데 필요한 인덱스 */
    m_iNumIndices = m_iNumIndexPerInstance * m_iNumInstance;  /* 총 인덱스 수 */
    m_iIndexStride = 2;
    m_iNumVertexBuffers = 2;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;



    /* 사본을 만들 때 실제 활용하기 위한 인스턴스 정점 버퍼를 할당하기 위해 필요한 초기 데이터를 미리 다 셋팅해놓는다.*/
    ZeroMemory(&m_InstanceBufferDesc, sizeof(m_InstanceBufferDesc));
    m_InstanceBufferDesc.ByteWidth = m_iNumInstance * m_iInstanceVertexStride;
    m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_InstanceBufferDesc.StructureByteStride = m_iInstanceVertexStride;
    m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    m_InstanceBufferDesc.MiscFlags = 0;

    m_pInstanceVertices = new COMPUTE_PARTICLE_INSTANCE[m_iNumInstance];
    ZeroMemory(m_pInstanceVertices, sizeof(COMPUTE_PARTICLE_INSTANCE) * m_iNumInstance);

    switch (pDesc->iParticle_Initialize_Type)
    {
#pragma region Base
    case 0: //Base
        for (_uint i = 0; i < m_iNumInstance; i++)
        {
            m_pInstanceVertices[i].vLifeTime.x = m_pGameInstance->Compute_Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y);
            m_pInstanceVertices[i].vSpeed.x = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.x;
            m_pInstanceVertices[i].vSpeed.y = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.y;
            m_pInstanceVertices[i].vSpeed.z = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.z;
            m_pInstanceVertices[i].vPivot = pDesc->vPivot;

            _float4         vTranslation = {};

            _float fRandom = {};
            if (true == pDesc->bReverse_XYZ[0]) //x반전이냐
            {
                fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
                if (fRandom < 0.f)
                    m_pInstanceVertices[i].vPivot.x *= -1.f;

                vTranslation.x = m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
            }
            else
            {
                vTranslation.x = m_pGameInstance->Compute_Random(pDesc->vCenter.x, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
            }
            if (true == pDesc->bReverse_XYZ[1]) //y반전이냐
            {
                fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
                if (fRandom < 0.f)
                    m_pInstanceVertices[i].vPivot.y *= -1.f;

                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            }
            else
            {
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            }
            if (true == pDesc->bReverse_XYZ[2]) //z반전이냐
            {
                fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
                if (fRandom < 0.f)
                    m_pInstanceVertices[i].vPivot.z *= -1.f;

                vTranslation.z = m_pGameInstance->Compute_Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f);
            }
            else
            {
                vTranslation.z = m_pGameInstance->Compute_Random(pDesc->vCenter.z, pDesc->vCenter.z + pDesc->vRange.z * 0.5f);
            }

            vTranslation.w = 1;

            m_pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
            m_pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
            m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
            m_pInstanceVertices[i].vTranslation = vTranslation;

            _float3 vScale = { m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.x,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.y,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.z };

            XMStoreFloat4(&m_pInstanceVertices[i].vRight, XMLoadFloat4(&m_pInstanceVertices[i].vRight) * vScale.x);
            XMStoreFloat4(&m_pInstanceVertices[i].vUp, XMLoadFloat4(&m_pInstanceVertices[i].vUp) * vScale.y);
            XMStoreFloat4(&m_pInstanceVertices[i].vLook, XMLoadFloat4(&m_pInstanceVertices[i].vLook) * vScale.z);

            m_pInstanceVertices[i].vScale = vScale;
            m_pInstanceVertices[i].fDelayTime = m_pGameInstance->Compute_Random(pDesc->vDelayTime.x, pDesc->vDelayTime.y);
        }
        break;
#pragma endregion

#pragma region Circle

    case 1: //Circle 오로지 평면 상태의 눕혀져있는
        for (_uint i = 0; i < m_iNumInstance; i++)
        {
            m_pInstanceVertices[i].vLifeTime.x = m_pGameInstance->Compute_Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y);
            m_pInstanceVertices[i].vSpeed.x = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.x;
            m_pInstanceVertices[i].vSpeed.y = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.y;
            m_pInstanceVertices[i].vSpeed.z = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.z;

            m_pInstanceVertices[i].vPivot = pDesc->vPivot;
            _float4         vTranslation = {};

            _float fRandom = {};
            if (true == pDesc->bReverse_XYZ[0]) //x반전이냐
                vTranslation.x = m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
            else
                vTranslation.x = m_pGameInstance->Compute_Random(pDesc->vCenter.x, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
            if (true == pDesc->bReverse_XYZ[1]) //y반전이냐
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            else
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            if (true == pDesc->bReverse_XYZ[2]) //z반전이냐
            {
                _float z = { pow(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, 2.f) - pow(vTranslation.x, 2.f) };
                if (0 < z)
                    vTranslation.z = sqrtf(z);
                else
                    vTranslation.z = sqrtf(z * -1.f);

                fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
                if (fRandom < 0.f)
                    vTranslation.z *= -1.f;

            }
            else
            {
                _float z = { pow(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, 2.f) - pow(vTranslation.x, 2.f) };
                if (0 < z)
                    vTranslation.z = sqrtf(z);
                else
                    vTranslation.z = sqrtf(z * -1.f);
            }

            vTranslation.w = 1;

            m_pInstanceVertices[i].vPivot = pDesc->vCenter;

            m_pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
            m_pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
            m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
            m_pInstanceVertices[i].vTranslation = vTranslation;

            _float3 vScale = { m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.x,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.y,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.z };

            XMStoreFloat4(&m_pInstanceVertices[i].vRight, XMLoadFloat4(&m_pInstanceVertices[i].vRight) * vScale.x);
            XMStoreFloat4(&m_pInstanceVertices[i].vUp, XMLoadFloat4(&m_pInstanceVertices[i].vUp) * vScale.y);
            XMStoreFloat4(&m_pInstanceVertices[i].vLook, XMLoadFloat4(&m_pInstanceVertices[i].vLook) * vScale.z);

            m_pInstanceVertices[i].vScale = vScale;
            m_pInstanceVertices[i].fDelayTime = m_pGameInstance->Compute_Random(pDesc->vDelayTime.x, pDesc->vDelayTime.y);
        }
        break;
#pragma endregion

#pragma region Circle_Z

    case 2: //Circle_Z
        for (_uint i = 0; i < m_iNumInstance; i++)
        {
            m_pInstanceVertices[i].vLifeTime.x = m_pGameInstance->Compute_Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y);
            m_pInstanceVertices[i].vSpeed.x = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.x;
            m_pInstanceVertices[i].vSpeed.y = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.y;
            m_pInstanceVertices[i].vSpeed.z = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.z;

            m_pInstanceVertices[i].vPivot = pDesc->vPivot;
            _float4         vTranslation = {};

            _float fRandom = {};
            if (true == pDesc->bReverse_XYZ[2]) //z반전이냐
                vTranslation.z = m_pGameInstance->Compute_Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f);
            else
                vTranslation.z = m_pGameInstance->Compute_Random(pDesc->vCenter.z, pDesc->vCenter.z + pDesc->vRange.z * 0.5f);
            if (true == pDesc->bReverse_XYZ[1]) //y반전이냐
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            else
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            if (true == pDesc->bReverse_XYZ[0]) //x반전이냐
            {
                _float x = { pow(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, 2.f) - pow(vTranslation.z, 2.f) };
                if (0 < x)
                    vTranslation.x = sqrtf(x);
                else
                    vTranslation.x = sqrtf(x * -1.f);

                fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
                if (fRandom < 0.f)
                    vTranslation.x *= -1.f;

            }
            else
            {
                _float x = { pow(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, 2.f) - pow(vTranslation.z, 2.f) };
                if (0 < x)
                    vTranslation.x = sqrtf(x);
                else
                    vTranslation.x = sqrtf(x * -1.f);
            }

            vTranslation.w = 1;

            m_pInstanceVertices[i].vPivot = pDesc->vCenter;

            m_pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
            m_pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
            m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
            m_pInstanceVertices[i].vTranslation = vTranslation;

            _float3 vScale = { m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.x,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.y,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.z };

            XMStoreFloat4(&m_pInstanceVertices[i].vRight, XMLoadFloat4(&m_pInstanceVertices[i].vRight) * vScale.x);
            XMStoreFloat4(&m_pInstanceVertices[i].vUp, XMLoadFloat4(&m_pInstanceVertices[i].vUp) * vScale.y);
            XMStoreFloat4(&m_pInstanceVertices[i].vLook, XMLoadFloat4(&m_pInstanceVertices[i].vLook) * vScale.z);

            m_pInstanceVertices[i].vScale = vScale;
            m_pInstanceVertices[i].fDelayTime = m_pGameInstance->Compute_Random(pDesc->vDelayTime.x, pDesc->vDelayTime.y);
        }
        break;
#pragma endregion

#pragma region FloorSpark

    case 3: //FloorSpark
        for (_uint i = 0; i < m_iNumInstance; i++)
        {
            m_pInstanceVertices[i].vLifeTime.x = m_pGameInstance->Compute_Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y);
            m_pInstanceVertices[i].vSpeed.x = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.x;
            m_pInstanceVertices[i].vSpeed.y = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.y;
            m_pInstanceVertices[i].vSpeed.z = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.z;
            m_pInstanceVertices[i].vPivot = pDesc->vPivot;

            _float4         vTranslation = {};

            _float fRandom = {};
            if (true == pDesc->bReverse_XYZ[0]) //x반전이냐
                vTranslation.x = m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
            else
                vTranslation.x = m_pGameInstance->Compute_Random(pDesc->vCenter.x, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
            if (true == pDesc->bReverse_XYZ[1]) //y반전이냐
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            else
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            if (true == pDesc->bReverse_XYZ[2]) //z반전이냐 
            {
                _float z = { pow(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, 2.f) - pow(vTranslation.x, 2.f) };
                if (0 < z)
                    vTranslation.z = sqrtf(z);
                else
                    vTranslation.z = sqrtf(z * -1.f);

                fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
                if (fRandom < 0.f)
                    vTranslation.z *= -1.f;

            }
            else
            {
                _float z = { pow(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, 2.f) - pow(vTranslation.x, 2.f) };
                if (0 < z)
                    vTranslation.z = sqrtf(z);
                else
                    vTranslation.z = sqrtf(z * -1.f);
            }

            vTranslation.w = 1;
            m_pInstanceVertices[i].vPivot = _float3(vTranslation.x + m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f),
                vTranslation.y + m_pGameInstance->Compute_Random(0.f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f),
                vTranslation.z + m_pGameInstance->Compute_Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f));

            m_pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
            m_pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
            m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
            m_pInstanceVertices[i].vTranslation = vTranslation;

            _float3 vScale = { m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.x,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.y,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.z };

            XMStoreFloat4(&m_pInstanceVertices[i].vRight, XMLoadFloat4(&m_pInstanceVertices[i].vRight) * vScale.x);
            XMStoreFloat4(&m_pInstanceVertices[i].vUp, XMLoadFloat4(&m_pInstanceVertices[i].vUp) * vScale.y);
            XMStoreFloat4(&m_pInstanceVertices[i].vLook, XMLoadFloat4(&m_pInstanceVertices[i].vLook) * vScale.z);

            m_pInstanceVertices[i].vScale = vScale;
            m_pInstanceVertices[i].fDelayTime = m_pGameInstance->Compute_Random(pDesc->vDelayTime.x, pDesc->vDelayTime.y);
        }
        break;

#pragma endregion

#pragma region Sphere

    case 4: //Sphere
        for (_uint i = 0; i < m_iNumInstance; i++)
        {
            m_pInstanceVertices[i].vLifeTime.x = m_pGameInstance->Compute_Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y);
            m_pInstanceVertices[i].vSpeed.x = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.x;
            m_pInstanceVertices[i].vSpeed.y = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.y;
            m_pInstanceVertices[i].vSpeed.z = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y) * pDesc->vSpeed_Weight.z;

            m_pInstanceVertices[i].vPivot = pDesc->vPivot;
            _float4         vTranslation = {};

            _float fRandom = {};
            if (true == pDesc->bReverse_XYZ[0]) //x반전이냐
                vTranslation.x = m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
            else
                vTranslation.x = m_pGameInstance->Compute_Random(pDesc->vCenter.x, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
            if (true == pDesc->bReverse_XYZ[1]) //y반전이냐
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            else
                vTranslation.y = m_pGameInstance->Compute_Random(pDesc->vCenter.y, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
            if (true == pDesc->bReverse_XYZ[2]) //z반전이냐
            {
                _float z = { pow(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, 2.f) - pow(vTranslation.x, 2.f) - pow(vTranslation.y, 2.f) };
                if (0 < z)
                    vTranslation.z = sqrtf(z);
                else
                    vTranslation.z = sqrtf(z * -1.f);

                fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
                if (fRandom < 0.f)
                    vTranslation.z *= -1.f;

            }
            else
            {
                _float z = { pow(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, 2.f) - pow(vTranslation.x, 2.f) - pow(vTranslation.y, 2.f) };
                if (0 < z)
                    vTranslation.z = sqrtf(z);
                else
                    vTranslation.z = sqrtf(z * -1.f);
            }

            vTranslation.w = 1;

            m_pInstanceVertices[i].vPivot = _float3(vTranslation.x + m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f),
                vTranslation.y + m_pGameInstance->Compute_Random(0.f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f),
                vTranslation.z + m_pGameInstance->Compute_Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f));

            m_pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
            m_pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
            m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
            m_pInstanceVertices[i].vTranslation = vTranslation;

            _float3 vScale = { m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.x,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.y,
                               m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y) * pDesc->vScale_Weight.z };

            XMStoreFloat4(&m_pInstanceVertices[i].vRight, XMLoadFloat4(&m_pInstanceVertices[i].vRight) * vScale.x);
            XMStoreFloat4(&m_pInstanceVertices[i].vUp, XMLoadFloat4(&m_pInstanceVertices[i].vUp) * vScale.y);
            XMStoreFloat4(&m_pInstanceVertices[i].vLook, XMLoadFloat4(&m_pInstanceVertices[i].vLook) * vScale.z);

            m_pInstanceVertices[i].vScale = vScale;
            m_pInstanceVertices[i].fDelayTime = m_pGameInstance->Compute_Random(pDesc->vDelayTime.x, pDesc->vDelayTime.y);
        }
        break;
#pragma endregion
    }

#pragma region 과거의잔재
    //for (_uint i = 0; i < m_iNumInstance; i++)
    //{
    //    _float4         vTranslation = {};
    //
    //    vTranslation.x = m_pGameInstance->Compute_Random(pDesc.vCenter.x - pDesc.vRange.x * 0.5f, pDesc.vCenter.x + pDesc.vRange.x * 0.5f);
    //    vTranslation.y = m_pGameInstance->Compute_Random(pDesc.vCenter.y - pDesc.vRange.y * 0.5f, pDesc.vCenter.y + pDesc.vRange.y * 0.5f);
    //    vTranslation.z = m_pGameInstance->Compute_Random(pDesc.vCenter.z - pDesc.vRange.z * 0.5f, pDesc.vCenter.z + pDesc.vRange.z * 0.5f);
    //    vTranslation.w = 1;
    //
    //    m_pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
    //    m_pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
    //    m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
    //    m_pInstanceVertices[i].vTranslation = vTranslation;
    //    m_pInstanceVertices[i].vLifeTime.x = m_pGameInstance->Compute_Random(pDesc.vLifeTime.x, pDesc.vLifeTime.y);
    //    m_pInstanceVertices[i].vLifeTime.y = 0.f;
    //    m_pInstanceVertices[i].vSpeed.x = m_pGameInstance->Compute_Random(pDesc.vSpeed.x, pDesc.vSpeed.y) * pDesc.vSpeed_Weight.x;
    //    m_pInstanceVertices[i].vSpeed.y = m_pGameInstance->Compute_Random(pDesc.vSpeed.x, pDesc.vSpeed.y) * pDesc.vSpeed_Weight.y;
    //    m_pInstanceVertices[i].vSpeed.z = m_pGameInstance->Compute_Random(pDesc.vSpeed.x, pDesc.vSpeed.y) * pDesc.vSpeed_Weight.z;
    //    m_pInstanceVertices[i].vPivot = pDesc.vPivot;
    //
    //    _float fRandom = {};
    //    if (true == pDesc.bReverse_XYZ[0])
    //    {
    //        fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
    //        if (fRandom < 0.f)
    //            m_pInstanceVertices[i].vPivot.x *= -1.f;
    //    }
    //    if (true == pDesc.bReverse_XYZ[1])
    //    {
    //        fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
    //        if (fRandom < 0.f)
    //            m_pInstanceVertices[i].vPivot.y *= -1.f;
    //    }
    //    if (true == pDesc.bReverse_XYZ[2])
    //    {
    //        fRandom = m_pGameInstance->Compute_Random(-1.f, 1.f);
    //        if (fRandom < 0.f)
    //            m_pInstanceVertices[i].vPivot.z *= -1.f;
    //    }
    //
    //    _float3 vScale = { m_pGameInstance->Compute_Random(pDesc.vSize.x, pDesc.vSize.y) * pDesc.vScale_Weight.x,
    //                       m_pGameInstance->Compute_Random(pDesc.vSize.x, pDesc.vSize.y) * pDesc.vScale_Weight.y,
    //                       m_pGameInstance->Compute_Random(pDesc.vSize.x, pDesc.vSize.y) * pDesc.vScale_Weight.z };
    //
    //    XMStoreFloat4(&m_pInstanceVertices[i].vRight, XMLoadFloat4(&m_pInstanceVertices[i].vRight) * vScale.x);
    //    XMStoreFloat4(&m_pInstanceVertices[i].vUp, XMLoadFloat4(&m_pInstanceVertices[i].vUp) * vScale.y);
    //    XMStoreFloat4(&m_pInstanceVertices[i].vLook, XMLoadFloat4(&m_pInstanceVertices[i].vLook) * vScale.z);
    //
    //
    //    m_pInstanceVertices[i].vScale = vScale;
    //}
#pragma endregion

    m_InstanceInitialData.pSysMem = m_pInstanceVertices;

#pragma endregion 


#pragma region VERTEX_BUFFER
    ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
    m_BufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.StructureByteStride = m_iVertexStride;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;

    VTXPOINT* pVertices = new VTXPOINT[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);

    pVertices[0].vPosition = _float3(0.f, 0.f, 0.f);
    pVertices[0].fPSize = 1.f;

    ZeroMemory(&m_InitialData, sizeof(m_InitialData));
    m_InitialData.pSysMem = pVertices;

    if (FAILED(__super::Create_Buffer(&m_pVB)))
        return E_FAIL;

    Safe_Delete_Array(pVertices);

#pragma endregion 


#pragma region INDEX_BUFFER
    ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
    m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.StructureByteStride = m_iIndexStride;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;

    _ushort* pIndices = new _ushort[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

    //for(int i =0; i< m_iNumIndices; i++)
    //{
    //    pIndices[i] = 0; 
    //}

    ZeroMemory(&m_InitialData, sizeof(m_InitialData));
    m_InitialData.pSysMem = pIndices;

    if (FAILED(__super::Create_Buffer(&m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);

#pragma endregion 

    if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InstanceInitialData, &m_pVBInstance)))
        return E_FAIL;

    if (FAILED(CreateStructureBuffer()))
        return E_FAIL;
    
    if (FAILED(CreateBuffer_SRV_UAV()))
        return E_FAIL;

    if (FAILED(CreateBuffer_Constant()))
        return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Point_Compute::Bind_InputAssembler()
{
    if (nullptr == m_pContext)
        return E_FAIL;

    ID3D11Buffer* pVertexBuffer[] =
    {
        m_pVB,
        m_pVBInstance,
    };

    _uint         iVertexStrides[] =
    {
        m_iVertexStride,
        m_iInstanceVertexStride,
    };

    _uint         iOffsets[] =
    {
        0,
        0,
    };


    m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffer, iVertexStrides, iOffsets);
    m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
    m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);


    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::Render()
{
    if (nullptr == m_pContext)
        return E_FAIL;

    m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0, 0);

    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::Compute_Shader(CShader_Compute* _pComputeShader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ)
{
    m_pContext->CSSetShader(_pComputeShader->Get_ComputeShader(), NULL, 0);
    
    m_pContext->CSSetShaderResources(0, 1, &m_pSRV);
    
    m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pUAV, (_uint*)&m_pUAV);

    if (FAILED(CameraCopyBuffer()))
        return E_FAIL;

    m_pContext->CSSetConstantBuffers(1, 1, &m_pBuffer_CameraPosition);

    m_pContext->Dispatch(_iThreadCountX, _iThreadCountY, _iThreadCountZ);

    

    ID3D11UnorderedAccessView* ppUAVnull[1] = {nullptr};
    ID3D11ShaderResourceView* ppSRVnull[3] = {nullptr, nullptr};
    ID3D11Buffer* ppCBnull[1] = {nullptr};

    m_pContext->CSSetShader(nullptr, nullptr, 0);
    m_pContext->CSSetUnorderedAccessViews(0, 1, ppUAVnull, (_uint*)(&ppUAVnull));
    m_pContext->CSSetShaderResources(0, 2, ppSRVnull);
    //m_pContext->CSSetConstantBuffers(0, 1, ppCBnull);

    if (FAILED(CreateAndCopyBuffer()))
        return E_FAIL;
    

    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::Compute_Shader(CShader_Compute* _pComputeShader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, _float4 _vStartPos)
{
    m_pContext->CSSetShader(_pComputeShader->Get_ComputeShader(), NULL, 0);

    m_pContext->CSSetShaderResources(0, 1, &m_pSRV);

    m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pUAV, (_uint*)&m_pUAV);

    if (FAILED(StartPositionCopyBuffer(_vStartPos)))
        return E_FAIL;

    m_pContext->CSSetConstantBuffers(0, 1, &m_pBuffer_StartPosition);

    if (FAILED(CameraCopyBuffer()))
        return E_FAIL;

    m_pContext->CSSetConstantBuffers(1, 1, &m_pBuffer_CameraPosition);

    m_pContext->Dispatch(_iThreadCountX, _iThreadCountY, _iThreadCountZ);



    ID3D11UnorderedAccessView* ppUAVnull[1] = { nullptr };
    ID3D11ShaderResourceView* ppSRVnull[3] = { nullptr, nullptr };
    ID3D11Buffer* ppCBnull[1] = { nullptr };

    m_pContext->CSSetShader(nullptr, nullptr, 0);
    m_pContext->CSSetUnorderedAccessViews(0, 1, ppUAVnull, (_uint*)(&ppUAVnull));
    m_pContext->CSSetShaderResources(0, 2, ppSRVnull);
    m_pContext->CSSetConstantBuffers(0, 1, ppCBnull);

    if (FAILED(CreateAndCopyBuffer()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::Compute_Shader_Reset(CShader_Compute* _pComputeShader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ)
{
    m_pContext->CSSetShader(_pComputeShader->Get_ComputeShader_Reset(), NULL, 0);

    m_pContext->CSSetShaderResources(0, 1, &m_pSRV);

    m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pUAV, (_uint*)&m_pUAV);

    //m_pContext->CSSetConstantBuffers(0, 1, &m_pBuffer_StartPosition);

    m_pContext->Dispatch(_iThreadCountX, _iThreadCountY, _iThreadCountZ);



    ID3D11UnorderedAccessView* ppUAVnull[1] = { nullptr };
    ID3D11ShaderResourceView* ppSRVnull[3] = { nullptr, nullptr };
    ID3D11Buffer* ppCBnull[1] = { nullptr };

    m_pContext->CSSetShader(nullptr, nullptr, 0);
    m_pContext->CSSetUnorderedAccessViews(0, 1, ppUAVnull, (_uint*)(&ppUAVnull));
    m_pContext->CSSetShaderResources(0, 2, ppSRVnull);
    //m_pContext->CSSetConstantBuffers(0, 1, ppCBnull);

    if (FAILED(CreateAndCopyBuffer()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::CreateStructureBuffer()
{
    D3D11_BUFFER_DESC inputDesc = {};
    inputDesc.Usage = D3D11_USAGE_DEFAULT;
    inputDesc.ByteWidth = sizeof(COMPUTE_PARTICLE_INSTANCE) * m_iNumInstance;
    inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    inputDesc.CPUAccessFlags = 0;
    inputDesc.StructureByteStride = sizeof(COMPUTE_PARTICLE_INSTANCE);
    inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

    m_pDevice->CreateBuffer(&inputDesc, &m_InstanceInitialData, &m_pBuffer_SRV);

    D3D11_BUFFER_DESC outputDesc = {};
    outputDesc.Usage = D3D11_USAGE_DEFAULT;
    outputDesc.ByteWidth = sizeof(COMPUTE_PARTICLE_INSTANCE) * m_iNumInstance;
    outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    outputDesc.CPUAccessFlags = 0;
    outputDesc.StructureByteStride = sizeof(COMPUTE_PARTICLE_INSTANCE);
    outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

    m_pDevice->CreateBuffer(&outputDesc, NULL, &m_pBuffer_UAV);


    D3D11_BUFFER_DESC desc = {};
    ZeroMemory(&desc, sizeof(desc));

    m_pBuffer_UAV->GetDesc(&desc);

    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;

    if (FAILED(m_pDevice->CreateBuffer(&desc, NULL, &m_pBuffer_Copy)))
        return E_FAIL;
    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::CreateBuffer_SRV_UAV()
{
    D3D11_BUFFER_DESC descBuf = {};
    ZeroMemory(&descBuf, sizeof(descBuf));
    m_pBuffer_SRV->GetDesc(&descBuf);


    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.BufferEx.FirstElement = 0;
    srvDesc.BufferEx.Flags = 0;
    srvDesc.BufferEx.NumElements = m_iNumInstance;
    
    m_pDevice->CreateShaderResourceView(m_pBuffer_SRV, &srvDesc, &m_pSRV);

    ZeroMemory(&descBuf, sizeof(descBuf));
    m_pBuffer_UAV->GetDesc(&descBuf);

    D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};

    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    desc.Buffer.FirstElement = 0;
    desc.Buffer.Flags = 0;
    desc.Buffer.NumElements = m_iNumInstance;

    return m_pDevice->CreateUnorderedAccessView(m_pBuffer_UAV, &desc, &m_pUAV);
}

HRESULT CVIBuffer_Point_Compute::CreateBuffer_Constant()
{
    COMPUTE_POSITION pCameraPos{};
    ZeroMemory(&pCameraPos, sizeof(COMPUTE_POSITION));

    D3D11_BUFFER_DESC constantDesc = {};
    constantDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantDesc.ByteWidth = sizeof(COMPUTE_POSITION);
    constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantDesc.StructureByteStride = 0;
    constantDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA constantData = {};
    constantData.pSysMem = &pCameraPos;

    if (FAILED(m_pDevice->CreateBuffer(&constantDesc, &constantData, &m_pBuffer_StartPosition)))
        return E_FAIL;

    if (FAILED(m_pDevice->CreateBuffer(&constantDesc, &constantData, &m_pBuffer_CameraPosition)))
        return E_FAIL;

    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::CreateAndCopyBuffer()
{
    //D3D11_MAPPED_SUBRESOURCE subCamera;
    //
    //m_pContext->Map(m_pBuffer_StartPosition, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subCamera);
    //XMFLOAT4* pCamera = {};
    //pCamera = static_cast<XMFLOAT4*>(subCamera.pData);
    //pCamera[0] = m_pGameInstance->Get_CamPosition();
    //m_pContext->Unmap(m_pBuffer_StartPosition, 0);

    m_pContext->CopyResource(m_pBuffer_Copy, m_pBuffer_UAV);

    D3D11_MAPPED_SUBRESOURCE subResource;
    D3D11_MAPPED_SUBRESOURCE msr;

    m_pContext->Map(m_pBuffer_Copy, 0, D3D11_MAP_READ, 0, &subResource);
    COMPUTE_PARTICLE_INSTANCE* pData = {};
    pData = (COMPUTE_PARTICLE_INSTANCE*)subResource.pData;

    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &msr);
    COMPUTE_PARTICLE_INSTANCE* pDest = {};
    pDest = (COMPUTE_PARTICLE_INSTANCE*)msr.pData;
    //for (_uint i = 0; i < m_iNumInstance; i++) //과거의 잔재 남겨두긴 해놈
    //    pDest[i] = pData[i];

    memcpy(pDest, pData, sizeof(COMPUTE_PARTICLE_INSTANCE) * m_iNumInstance);


    m_pContext->Unmap(m_pVBInstance, 0);
    

    m_pContext->Unmap(m_pBuffer_Copy, 0);

    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::StartPositionCopyBuffer(_float4 _vStartPos)
{
    D3D11_MAPPED_SUBRESOURCE subStartPosition;

    m_pContext->Map(m_pBuffer_StartPosition, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subStartPosition);
    XMFLOAT4* pStartPos = {};
    pStartPos = static_cast<XMFLOAT4*>(subStartPosition.pData);
    pStartPos[0] = _vStartPos;
    m_pContext->Unmap(m_pBuffer_StartPosition, 0);

    return S_OK;
}

HRESULT CVIBuffer_Point_Compute::CameraCopyBuffer()
{
    D3D11_MAPPED_SUBRESOURCE subCameraPosition;

    m_pContext->Map(m_pBuffer_CameraPosition, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subCameraPosition);
    XMFLOAT4* pCamPos = {};
    pCamPos = static_cast<XMFLOAT4*>(subCameraPosition.pData);
    pCamPos[0] = m_pGameInstance->Get_CamPosition();
    m_pContext->Unmap(m_pBuffer_CameraPosition, 0);

    return S_OK;
}

CVIBuffer_Point_Compute* CVIBuffer_Point_Compute::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CVIBuffer_Point_Compute* pInstance = new CVIBuffer_Point_Compute(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Point_Compute");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Point_Compute::Clone(void* _pArg)
{
	CVIBuffer_Point_Compute* pInstance = new CVIBuffer_Point_Compute(*this);

	if (FAILED(pInstance->Initialize(_pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Point_Compute");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point_Compute::Free()
{
	__super::Free();

    //if (false == m_isCloned)
    //{
    //    Safe_Delete_Array(m_pSpeeds);
    //    Safe_Delete_Array(m_pCombinedMatrix);
    //}

    Safe_Delete_Array(m_pInstanceVertices);
    Safe_Release(m_pUAV);
    Safe_Release(m_pSRV);
    Safe_Release(m_pBuffer_UAV);
    Safe_Release(m_pBuffer_SRV);
    Safe_Release(m_pBuffer_Copy);
    Safe_Release(m_pBuffer_StartPosition);
    Safe_Release(m_pBuffer_CameraPosition);
    Safe_Release(m_pVBInstance);
}

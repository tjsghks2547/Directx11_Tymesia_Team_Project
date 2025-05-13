#include "RippleManager.h"

#include "Shader.h"

#define MAXRIPPLE 360

CRippleManager::CRippleManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : m_pDevice{ _pDevice }
    , m_pContext{ _pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CRippleManager::Initialize()
{
    m_RippleInfos.reserve(MAXRIPPLE);

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(RIPPLEINFO) * MAXRIPPLE;
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.StructureByteStride = sizeof(RIPPLEINFO);
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

    m_pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pRippleBuffer);

    if (m_pRippleBuffer == nullptr)
        return E_FAIL;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.NumElements = MAXRIPPLE;
    m_pDevice->CreateShaderResourceView(m_pRippleBuffer, &srvDesc, &m_pRippleSRV);

    if (m_pRippleSRV == nullptr)
        return E_FAIL;

    return S_OK;
}

void CRippleManager::Update(_float _fTimeDelta)
{
    for (vector<RIPPLEINFO>::iterator pRipple = m_RippleInfos.begin(); pRipple != m_RippleInfos.end(); )
    {
        (*pRipple).g_Ripplefactor.x += _fTimeDelta;// * 0.01f
        (*pRipple).g_Ripplefactor.y += _fTimeDelta * 0.006f;
        if ((*pRipple).g_Ripplefactor.x > 1.5f)
        {
            pRipple = m_RippleInfos.erase(pRipple);
        }
        else
            pRipple++;
    }

    if (m_RippleInfos.size() > 0)
    {
        D3D11_MAPPED_SUBRESOURCE mapped;
        m_pContext->Map(m_pRippleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        memcpy(mapped.pData, m_RippleInfos.data(), sizeof(RippleInfo) * m_RippleInfos.size());
        m_pContext->Unmap(m_pRippleBuffer, 0);
    }
}

void CRippleManager::Add_RippleInfo(_float2 vPos, _float fRippleRange)
{
    if (m_RippleInfos.size() > MAXRIPPLE)
        return;

    RippleInfo Info = {};

    Info.g_RippleCenter = { (vPos.x - m_vWaterPos.x) / 199.f,  (vPos.y - m_vWaterPos.y) / 199.f };
    Info.g_Ripplefactor.x = 0;
    Info.g_Ripplefactor.y = fRippleRange * 0.001f;

    m_RippleInfos.push_back(Info);
}

HRESULT CRippleManager::Bind_RippleSRV(CShader* pShader)
{
    _int iSize = (_int)m_RippleInfos.size();

    if (FAILED(pShader->Bind_RawValue("g_RippleCount", &iSize, sizeof(_int))))
        return E_FAIL;

    if (FAILED(pShader->Bind_SRV("g_Ripples", m_pRippleSRV)))
        return E_FAIL;

    return S_OK;
}

CRippleManager* CRippleManager::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CRippleManager* pInstance = new CRippleManager(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CRippleManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRippleManager::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pRippleBuffer);
    Safe_Release(m_pRippleSRV);
}

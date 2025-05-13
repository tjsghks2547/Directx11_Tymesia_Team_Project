#include "RenderTarget.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    , m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);

}

HRESULT CRenderTarget::Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    D3D11_TEXTURE2D_DESC TextureDesc;
    ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

    TextureDesc.Width = iWidth;
    TextureDesc.Height = iHeight;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = ePixelFormat;

    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.SampleDesc.Count = 1;

    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.CPUAccessFlags = 0;
    TextureDesc.MiscFlags = 0;

    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
        return E_FAIL;

    if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
        return E_FAIL;

    if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
        return E_FAIL;


    m_vClearColor = vClearColor;

    return S_OK;
}


HRESULT CRenderTarget::Initialize_ShadowMap(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor, _uint iArraySize)
{
    D3D11_TEXTURE2D_DESC TextureDesc;
    ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

    TextureDesc.Width = iWidth;
    TextureDesc.Height = iHeight;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = iArraySize;
    TextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;

    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.SampleDesc.Count = 1;

    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.CPUAccessFlags = 0;
    TextureDesc.MiscFlags = 0;

    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
        return E_FAIL;

    D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
    descSRV.Format = ePixelFormat;
    descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    descSRV.Texture2DArray.ArraySize = iArraySize;
    descSRV.Texture2DArray.FirstArraySlice = 0;
    descSRV.Texture2DArray.MipLevels = 1;
    descSRV.Texture2DArray.MostDetailedMip = 0;

    if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, &descSRV, &m_pSRV)))
        return E_FAIL;

    m_vClearColor = vClearColor;

    return S_OK;
}


HRESULT CRenderTarget::Initialize_UAV(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    D3D11_TEXTURE2D_DESC TextureDesc;
    ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

    TextureDesc.Width = iWidth;
    TextureDesc.Height = iHeight;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = ePixelFormat;

    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.SampleDesc.Count = 1;

    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    TextureDesc.CPUAccessFlags = 0;
    TextureDesc.MiscFlags = 0;

    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
        return E_FAIL;

    if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
        return E_FAIL;

    if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
        return E_FAIL;

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = TextureDesc.Format;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    uavDesc.Texture2D.MipSlice = 0;

    if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pTexture2D, &uavDesc, &m_pUAV)))
        return E_FAIL;


    m_vClearColor = vClearColor;

    return S_OK;
}



void CRenderTarget::Clear()
{
    m_pContext->ClearRenderTargetView(m_pRTV, reinterpret_cast<_float*>(&m_vClearColor));
}

HRESULT CRenderTarget::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
    return pShader->Bind_SRV(pConstantName, m_pSRV);
}

HRESULT CRenderTarget::Copy_Resource(ID3D11Texture2D* pTexture2D)
{
    m_pContext->CopyResource(m_pTexture2D, pTexture2D);

    return S_OK;
}


#ifdef _DEBUG
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
    _uint           iNumViewports = { 1 };
    D3D11_VIEWPORT  ViewportDesc{};

    m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);  // 뷰포트 사이즈 얻어오기 위해 가져온거 

    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(fSizeX, fSizeY, 1.f));

    XMStoreFloat4((_float4*)&m_WorldMatrix.m[3],
        XMVectorSet(fX - (ViewportDesc.Width * 0.5f), -fY + (ViewportDesc.Height * 0.5f), 0.f, 1.f));

    return S_OK;
}

HRESULT CRenderTarget::Render_Debug(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{

    pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);

    pShader->Bind_SRV("g_Texture", m_pSRV);

    pShader->Begin(0);

    pVIBuffer->Bind_InputAssembler();

    pVIBuffer->Render();

    return S_OK;
}
#endif

CRenderTarget* CRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

    if (FAILED(pInstance->Initialize(iWidth, iHeight, ePixelFormat, vClearColor)))
    {
        MSG_BOX("Faield to Created : CRenderTarget");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CRenderTarget* CRenderTarget::Create_ShadowMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor, _uint iArraySize)
{
    CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

    if (FAILED(pInstance->Initialize_ShadowMap(iWidth, iHeight, ePixelFormat, vClearColor, iArraySize)))
    {
        MSG_BOX("Faield to Created : CRenderTarget");
        Safe_Release(pInstance);
    }

    return pInstance;
}


CRenderTarget* CRenderTarget::Create_UAV(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

    if (FAILED(pInstance->Initialize_UAV(iWidth, iHeight, ePixelFormat, vClearColor)))
    {
        MSG_BOX("Faield to Created : CRenderTarget");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CRenderTarget::Free()
{
    __super::Free();


    //SaveDDSTextureToFile(m_pContext, m_pTexture2D, TEXT("../Bin/Diffuse.dds")); 

    Safe_Release(m_pSRV);
    Safe_Release(m_pRTV);
    Safe_Release(m_pTexture2D);
    Safe_Release(m_pUAV);


    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

}

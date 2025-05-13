#include "CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    , m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CCustomFont::Initialize(const _tchar* pFontFilePath)
{
    m_pFont = new SpriteFont(m_pDevice, pFontFilePath);
    m_pBatch = new SpriteBatch(m_pContext);

    /*텍스트 알파 블렌딩*/
    m_BlendDesc.RenderTarget->BlendEnable = true;
    m_BlendDesc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_ALPHA;
    m_BlendDesc.RenderTarget->DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    m_BlendDesc.RenderTarget->BlendOp = D3D11_BLEND_OP_ADD;
    m_BlendDesc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ONE;
    m_BlendDesc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ZERO;
    m_BlendDesc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
    m_BlendDesc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


    m_pDevice->CreateBlendState(&m_BlendDesc, &m_pBlendState);

    ///*폰트 깊이 버퍼 활성화 용도였는데 필요 없어서 사용X*/
    //m_Desc.DepthEnable = TRUE;
    //m_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    //m_Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    //m_Desc.StencilEnable = FALSE;
    //m_pDevice->CreateDepthStencilState(&m_Desc, &m_pDepthStencil);

    return S_OK;
}

HRESULT CCustomFont::Render(const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
    if (nullptr == m_pFont ||
        nullptr == m_pBatch)
        return E_FAIL;


    m_pBatch->Begin(SpriteSortMode_Deferred);

     m_pFont->DrawString(m_pBatch, pText, vPosition, XMLoadFloat4(&vColor), fRotation, vOrigin, fScale, effects, 0.0f);

    m_pBatch->End();

    return S_OK;
}
HRESULT CCustomFont::Render_World(const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
    if (nullptr == m_pFont ||
        nullptr == m_pBatch)
        return E_FAIL;

    m_pBatch->Begin(SpriteSortMode_Deferred);

    m_pFont->DrawString(m_pBatch, pText, vPosition, XMLoadFloat4(&vColor), fRotation, vOrigin, fScale, effects, layerDepth);

    m_pBatch->End();

    return S_OK;
}
HRESULT CCustomFont::Render_Shadow(const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
    if (nullptr == m_pFont ||
        nullptr == m_pBatch)
        return E_FAIL;

    m_pBatch->Begin(SpriteSortMode_FrontToBack);

    m_pFont->DrawString(m_pBatch, pText, { vPosition.x + 1.f,vPosition.y + 1.f }, Colors::Black, fRotation, vOrigin, fScale, effects, layerDepth);
    m_pFont->DrawString(m_pBatch, pText, { vPosition.x - 1.f,vPosition.y + 1.f }, Colors::Black, fRotation, vOrigin, fScale, effects, layerDepth);
    m_pFont->DrawString(m_pBatch, pText, vPosition, XMLoadFloat4(&vColor), fRotation, vOrigin, fScale, effects, layerDepth);

    m_pBatch->End();

    return S_OK;
}
HRESULT CCustomFont::Render_Outline(const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
    if (nullptr == m_pFont ||
        nullptr == m_pBatch)
        return E_FAIL;

    m_pBatch->Begin(SpriteSortMode_FrontToBack);

    m_pFont->DrawString(m_pBatch, pText, { vPosition.x + 1.5f,vPosition.y + 1.5f }, Colors::Black, fRotation, vOrigin, fScale, effects, layerDepth);
    m_pFont->DrawString(m_pBatch, pText, { vPosition.x - 1.5f,vPosition.y + 1.5f }, Colors::Black, fRotation, vOrigin, fScale, effects, layerDepth);
    m_pFont->DrawString(m_pBatch, pText, { vPosition.x - 1.5f,vPosition.y - 1.5f }, Colors::Black, fRotation, vOrigin, fScale, effects, layerDepth);
    m_pFont->DrawString(m_pBatch, pText, { vPosition.x + 1.5f,vPosition.y - 1.5f }, Colors::Black, fRotation, vOrigin, fScale, effects, layerDepth);

    m_pFont->DrawString(m_pBatch, pText, vPosition, XMLoadFloat4(&vColor), fRotation, vOrigin, fScale, effects, layerDepth);
    //m_pFont->DrawString(m_pBatch, pText, vPosition, Colors::Gray, fRotation, vOrigin, fScale, effects, layerDepth);

    m_pBatch->End();

    return S_OK;
}

HRESULT CCustomFont::Render_Color(const _tchar* pText1, const _tchar* pText2, const _float2& vPosition, _float4 vColor1, _float4 vColor2, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{

    if (nullptr == m_pFont ||
        nullptr == m_pBatch)
        return E_FAIL;
    m_pBatch->Begin(SpriteSortMode_FrontToBack);


    m_pFont->DrawString(m_pBatch, pText1, vPosition, XMLoadFloat4(&vColor1), fRotation, vOrigin, fScale, effects, 0.0f);

    _float2 fSize = { XMVectorGetX(m_pFont->MeasureString(pText1)),vPosition.y };

    fSize.x += vPosition.x;

    m_pFont->DrawString(m_pBatch, pText2, fSize, XMLoadFloat4(&vColor2), fRotation, vOrigin, fScale, effects, 0.0f);


    m_pBatch->End();

    return S_OK;
}

HRESULT CCustomFont::Render_Alpha(const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
    if (nullptr == m_pFont ||
        nullptr == m_pBatch)
        return E_FAIL;


    m_pBatch->Begin(SpriteSortMode_Deferred, m_pBlendState);


    m_pFont->DrawString(m_pBatch, pText, { vPosition.x + 0.5f,vPosition.y + 0.5f }, { 256.f,256.f,256.f,vColor.w}, fRotation, vOrigin, fScale, effects, layerDepth);
    //m_pFont->DrawString(m_pBatch, pText, { vPosition.x - 0.5f,vPosition.y + 0.5f }, { 256.f,256.f,256.f,vColor.w}, fRotation, vOrigin, fScale, effects, layerDepth);
    //m_pFont->DrawString(m_pBatch, pText, { vPosition.x - 0.5f,vPosition.y - 0.5f }, { 256.f,256.f,256.f,vColor.w}, fRotation, vOrigin, fScale, effects, layerDepth);
    m_pFont->DrawString(m_pBatch, pText, { vPosition.x + 0.5f,vPosition.y - 0.5f }, { 256.f,256.f,256.f,vColor.w}, fRotation, vOrigin, fScale, effects, layerDepth);

    m_pFont->DrawString(m_pBatch, pText, vPosition, XMLoadFloat4(&vColor), fRotation, vOrigin, fScale, effects, 0.0f);

    m_pBatch->End();

    return S_OK;
}

_float2 CCustomFont::Get_TextSize(const _tchar* pText)
{
    _float2 fSize = { XMVectorGetX(m_pFont->MeasureString(pText)),XMVectorGetY(m_pFont->MeasureString(pText)) };

    return fSize;
}

CCustomFont* CCustomFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
    CCustomFont* pInstance = new CCustomFont(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pFontFilePath)))
    {
        MSG_BOX("Failed to Created : CustomFont");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCustomFont::Free()
{
    __super::Free();


    Safe_Delete(m_pBatch);
    Safe_Delete(m_pFont);

    Safe_Release(m_pBlendState);
    Safe_Release(m_pContext);
    Safe_Release(m_pDevice); 
    //Safe_Release(m_pDepthStencil);


}

#include "Target_Manager.h"
#include "RenderTarget.h"   

#include "Shader_Compute_Deferred.h"

CTarget_Manager::CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    ,m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CTarget_Manager::Initialize()        
{
    return S_OK;    
}

HRESULT CTarget_Manager::Bind_RT_ShaderResource(const _wstring& strRenderTargetTag, CShader* pShader, const _char* pConstantName)
{
    CRenderTarget* pRenderTarget = Find_RenderTarget(strRenderTargetTag);       
    if (pRenderTarget == nullptr)       
        return E_FAIL;  
    
    return pRenderTarget->Bind_ShaderResource(pShader,pConstantName);   
}

HRESULT CTarget_Manager::Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    if (nullptr != Find_RenderTarget(strRenderTargetTag))
        return E_FAIL;  
   
    CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, iWidth, iHeight, ePixelFormat, vClearColor);
    if (nullptr == pRenderTarget)
        return E_FAIL; 

    m_RenderTargets.emplace(strRenderTargetTag, pRenderTarget);

    return S_OK;    
}

HRESULT CTarget_Manager::Add_Shadow_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor, _uint iArraySize)
{
    if (nullptr != Find_RenderTarget(strRenderTargetTag))
        return E_FAIL;

    CRenderTarget* pRenderTarget = CRenderTarget::Create_ShadowMap(m_pDevice, m_pContext, iWidth, iHeight, ePixelFormat, vClearColor, iArraySize);
    if (nullptr == pRenderTarget)
        return E_FAIL;

    m_RenderTargets.emplace(strRenderTargetTag, pRenderTarget);

    return S_OK;
}

HRESULT CTarget_Manager::Add_UAV_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    if (nullptr != Find_RenderTarget(strRenderTargetTag))
        return E_FAIL;

    CRenderTarget* pRenderTarget = CRenderTarget::Create_UAV(m_pDevice, m_pContext, iWidth, iHeight, ePixelFormat, vClearColor);
    if (nullptr == pRenderTarget)
        return E_FAIL;

    m_RenderTargets.emplace(strRenderTargetTag, pRenderTarget);

    return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag)
{
    CRenderTarget* pRenderTarget = Find_RenderTarget(strRenderTargetTag);   
    if (pRenderTarget == nullptr)
        return E_FAIL; 

    vector<CRenderTarget*>* pMRT = Find_MRT(strMRTTag);
    
    if (nullptr == pMRT)
    {
        vector<CRenderTarget*> MRT;

        MRT.reserve(8); // 렌더타겟은 최대 8개를 만들 수 있음. 
        MRT.push_back(pRenderTarget);

        m_MRTs.emplace(strMRTTag, MRT);
    }

    else
        pMRT->push_back(pRenderTarget); 

    
    Safe_AddRef(pRenderTarget);

    return S_OK;    
}

HRESULT CTarget_Manager::Begin_MRT(const _wstring& strMRTTag, _bool isClear, ID3D11DepthStencilView* pDSV)  
{
    ID3D11ShaderResourceView* pSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {    
    nullptr 
    };  

    m_pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSRV);    

    m_pContext->OMGetRenderTargets(1, &m_pBackRTV, &m_pOriginalDSV);    // 여기서 의도치않게 OMGetRenderTargets을 하면 m_pBackRTV와 m_pOrginalDsv 가 Add_Referecne가 되어서 END_MRT에서 Safe_Release를 한다.
                                                                        // 깊이버퍼 가져올려고 쓴것임
    vector<CRenderTarget*>* pMRTs = Find_MRT(strMRTTag);
    if (nullptr == pMRTs)
        return E_FAIL; 

    ID3D11RenderTargetView* pRTVs[8] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };

    _uint iNumRenderTarget = { 0 };

    for (auto& pRenderTarget : *pMRTs)
    {
        if(true == isClear)
             pRenderTarget->Clear(); 
        pRTVs[iNumRenderTarget++] = pRenderTarget->Get_RTV();
    }

    if (nullptr != pDSV)
    {
        // m_pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
        m_pContext->OMSetRenderTargets(0, nullptr, pDSV);
    }
    else
        m_pContext->OMSetRenderTargets(iNumRenderTarget, pRTVs, m_pOriginalDSV);    

    
    return S_OK;    
}

HRESULT CTarget_Manager::End_MRT(ID3D11DepthStencilView* _pDSV)
{
   
    m_pContext->OMSetRenderTargets(1, &m_pBackRTV, m_pOriginalDSV);
    
    Safe_Release(m_pOriginalDSV);
    Safe_Release(m_pBackRTV);

   /* if (nullptr != _pDSV)
        m_pContext->ClearDepthStencilView(_pDSV, D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.f, 0); */

    return S_OK;
}

HRESULT CTarget_Manager::RTV_Compute_LightShaft(const _wstring& strRenderTargetTagToRead, const _wstring& strRenderTargetTagToWrite, CShader_Compute_Deferred* pCompute_Shader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, void* pArg)
{
    CRenderTarget* pRenderTargetToRead = Find_RenderTarget(strRenderTargetTagToRead);

    if (pRenderTargetToRead == nullptr)
        return E_FAIL;

    CRenderTarget* pRenderTargetToWrite = Find_RenderTarget(strRenderTargetTagToWrite);

    if (pRenderTargetToRead == nullptr)
        return E_FAIL;

    return pCompute_Shader->Compute_Shader_LightShaft(_iThreadCountX, _iThreadCountY, _iThreadCountZ, pRenderTargetToRead->Get_SRV(), pRenderTargetToWrite->Get_UAV(), pArg);
}

HRESULT CTarget_Manager::RTV_Compute_Fog(const _wstring& strRenderTargetTagDepth, ID3D11ShaderResourceView* pNoiseSRV, const _wstring& strRenderTargetTagGodRay, const _wstring& strRenderTargetTagFinal, const _wstring& strRenderTargetTagRangeFogFinal, const _wstring& strRenderTargetTagFog, CShader_Compute_Deferred* pCompute_Shader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, void* pArg)
{

    CRenderTarget* pRenderTargetDepth = Find_RenderTarget(strRenderTargetTagDepth);

    if (pRenderTargetDepth == nullptr)
        return E_FAIL;

    CRenderTarget* pRenderTargetGodRay = Find_RenderTarget(strRenderTargetTagGodRay);

    if (pRenderTargetGodRay == nullptr)
        return E_FAIL;

    CRenderTarget* pRenderTargetFinal = Find_RenderTarget(strRenderTargetTagFinal);

    if (pRenderTargetFinal == nullptr)
        return E_FAIL;
    CRenderTarget* pRenderTargetRangeFog = Find_RenderTarget(strRenderTargetTagRangeFogFinal);

    if (pRenderTargetRangeFog == nullptr)
        return E_FAIL;

    CRenderTarget* pRenderTargetFog = Find_RenderTarget(strRenderTargetTagFog);

    if (pRenderTargetFog == nullptr)
        return E_FAIL;

    return pCompute_Shader->Compute_Shader_Fog(_iThreadCountX, _iThreadCountY, _iThreadCountZ, pRenderTargetDepth->Get_SRV(), pNoiseSRV, pRenderTargetGodRay->Get_SRV(), pRenderTargetFinal->Get_SRV(), pRenderTargetRangeFog->Get_SRV(), pRenderTargetFog->Get_UAV(), pArg);
}


HRESULT CTarget_Manager::Clear_RTV(const _wstring& strRenderTargetTag)
{
    CRenderTarget* pRenderTarget = Find_RenderTarget(strRenderTargetTag);

    if (pRenderTarget == nullptr)
        return E_FAIL;

    pRenderTarget->Clear();

    return S_OK;
}

HRESULT CTarget_Manager::Copy_RTV(const _wstring& strRenderTargetTag, const _wstring& strRenderTargetTagToCopy)
{
    CRenderTarget* pSrvRenderTarget = Find_RenderTarget(strRenderTargetTag);

    if (pSrvRenderTarget == nullptr)
        return E_FAIL;

    CRenderTarget* pDstRenderTarget = Find_RenderTarget(strRenderTargetTagToCopy);

    if (pDstRenderTarget == nullptr)
        return E_FAIL;

    if (FAILED(pDstRenderTarget->Copy_Resource(pSrvRenderTarget->Get_Texture2D())))
        return E_FAIL;

    return S_OK;
}

#ifdef _DEBUG
HRESULT CTarget_Manager::Ready_RT_Debug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
    CRenderTarget* pRenderTarget = Find_RenderTarget(strRenderTargetTag);

    if (pRenderTarget == nullptr)
        return E_FAIL;  

    return pRenderTarget->Ready_Debug(fX,fY,fSizeX,fSizeY);    
}

HRESULT CTarget_Manager::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    vector<CRenderTarget*>* pMRTs = Find_MRT(strMRTTag);  
    if (nullptr == pMRTs)
        return E_FAIL; 

    for(auto& pRenderTarget : *pMRTs)
    {
        if (nullptr != pRenderTarget)
            pRenderTarget->Render_Debug(pShader, pVIBuffer);
    }


    return S_OK; 
}
#endif

CRenderTarget* CTarget_Manager::Find_RenderTarget(const _wstring& strRenderTargetTag)
{
    auto& iter = m_RenderTargets.find(strRenderTargetTag); 
    
    if (iter == m_RenderTargets.end())
        return nullptr; 

    return iter->second;        
}

vector<CRenderTarget*>* CTarget_Manager::Find_MRT(const _wstring& strMRTTag)
{
    auto iter = m_MRTs.find(strMRTTag);

    if (iter == m_MRTs.end())
        return nullptr; 

    return &iter->second; 

}

CTarget_Manager* CTarget_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTarget_Manager* pInstance = new CTarget_Manager(pDevice, pContext);
    
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CTarget_Mangaer");
        Safe_Release(pInstance);
    }

    return pInstance;   
}

void CTarget_Manager::Free()
{
    __super::Free();    
        
    for (auto& Pair : m_MRTs)
    {
        for (auto& pRenderTargets : Pair.second)
            Safe_Release(pRenderTargets);

        Pair.second.clear();    
    }

    m_MRTs.clear();

    for (auto& Pair : m_RenderTargets) 
        Safe_Release(Pair.second); 

    m_RenderTargets.clear();    

    //for (auto& Pair :m_MRTs )
    
    Safe_Release(m_pContext);   
    Safe_Release(m_pDevice);    

}

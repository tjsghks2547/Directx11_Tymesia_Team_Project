#include "Edge_Cell.h"
#include "VIBuffer_Edge_Cell.h"

CEdge_Cell::CEdge_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    , m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}



HRESULT CEdge_Cell::Initialize(const _float3* pPoints)
{
    memcpy(m_vPoints, pPoints, sizeof(_float3) * 2);


#ifdef _DEBUG
    m_pVIBuffer = CVIBuffer_Edge_Cell::Create(m_pDevice, m_pContext, m_vPoints);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;
#endif 

    return S_OK;
}



#ifdef _DEBUG
HRESULT CEdge_Cell::Render()
{
    m_pVIBuffer->Bind_InputAssembler();

    m_pVIBuffer->Render();

    return S_OK;
}

#endif

CEdge_Cell* CEdge_Cell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints)
{
    CEdge_Cell* pInstance = new CEdge_Cell(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pPoints)))
    {
        MSG_BOX("Failed to Created : CEdge_Cell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEdge_Cell::Free()
{
    __super::Free();

#ifdef _DEBUG

    Safe_Release(m_pVIBuffer);

#endif // _DEBUG

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);

}

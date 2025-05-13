#include "Cell.h"
#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    , m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}



HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
    memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

    m_iIndex = iIndex;

    XMStoreFloat4(&m_vPlane,
        XMPlaneFromPoints(XMLoadFloat3(&pPoints[POINT_A]), XMLoadFloat3(&pPoints[POINT_B]), XMLoadFloat3(&pPoints[POINT_C])));


#ifdef _DEBUG
    m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;
#endif 
    _vector vPos = { };

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            vPos.m128_f32[i] += XMLoadFloat3(&pPoints[j]).m128_f32[i];

    vPos /= 3.f;

    XMStoreFloat4(&m_vPos, vPos);

    return S_OK;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
    for (size_t i = 0; i < LINE_END; i++)
    {
        _vector  vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
        _vector  vLine = XMLoadFloat3(&m_vPoints[(i + 1) % 3]) - XMLoadFloat3(&m_vPoints[i]); // 시계방향 ( 0이 맨 위 삼각형 꼭지점 기준) 
        _vector  vNoraml = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

        if (XMVectorGetX(XMVector3Dot(vDir, vNoraml)) > 0) // 내적의 곱이 0보다 작으면 내부  0보다 크면 외부
        {
            *pNeighborIndex = m_iNeighborIndices[i]; // i =0 일 때 직선AB가 해당 플레이어의 방향벡터 와 내적해서 0보다 작으면  이웃 
            return false;
        }
    }

    return true;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
    //XMVectorEqual() 은 하나하나 백터의 각 요소 x,y,z,w를 비교해서 같으면 1 안같으면 0을 반환

    if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[POINT_A])))
    {
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_B])))
            return true;
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_C])))
            return true;
    }

    if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[POINT_B])))
    {
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_C])))
            return true;
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_A])))
            return true;
    }

    if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[POINT_C])))
    {
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_A])))
            return true;
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_B])))
            return true;
    }

    return false;

}

_float CCell::Compute_Height(_fvector vPosition)
{
    // ax+ by + cz + d =0;
    // y = (-ax - cz -d ) / b 

    return (-m_vPlane.x * vPosition.m128_f32[0] - m_vPlane.z * vPosition.m128_f32[2] - m_vPlane.w) / m_vPlane.y;
}

void CCell::Set_CellInfo(vector<CNavigation::CELL*>& pCells)
{
    CNavigation::CELL* pCellInfo = new CNavigation::CELL;

    pCellInfo->iIndex = m_iIndex;
    pCellInfo->byOption = 0;
    pCellInfo->iParentIndex = 0;
    XMStoreFloat3(&pCellInfo->vPos, XMLoadFloat4(&m_vPos));
    _float3 vCenter = {};
    XMStoreFloat3(&vCenter, (XMLoadFloat3(&m_vPoints[POINT_A]) + XMLoadFloat3(&m_vPoints[POINT_B]) + XMLoadFloat3(&m_vPoints[POINT_C])) / 3);

    _vector vACenter = XMLoadFloat3(&vCenter) - (XMLoadFloat3(&m_vPoints[POINT_A]));
    _vector vBCenter = XMLoadFloat3(&vCenter) - (XMLoadFloat3(&m_vPoints[POINT_B]));
    _vector vCCenter = XMLoadFloat3(&vCenter) - (XMLoadFloat3(&m_vPoints[POINT_C]));

    _float3 NewPoint_A{}, NewPoint_B{}, NewPoint_C{};
    XMStoreFloat3(&NewPoint_A, (XMLoadFloat3(&m_vPoints[POINT_A])) + (vACenter * 0.3f));
    XMStoreFloat3(&NewPoint_B, (XMLoadFloat3(&m_vPoints[POINT_B])) + (vBCenter * 0.3f));
    XMStoreFloat3(&NewPoint_C, (XMLoadFloat3(&m_vPoints[POINT_C])) + (vCCenter * 0.3f));

    pCellInfo->ePoint[CNavigation::N_POINT_A] = NewPoint_A;
    pCellInfo->ePoint[CNavigation::N_POINT_B] = NewPoint_B;
    pCellInfo->ePoint[CNavigation::N_POINT_C] = NewPoint_C;

    pCellInfo->eLine[CNavigation::N_LINE_AB][0] = NewPoint_A;
    pCellInfo->eLine[CNavigation::N_LINE_AB][1] = NewPoint_B;

    pCellInfo->eLine[CNavigation::N_LINE_BC][0] = NewPoint_B;
    pCellInfo->eLine[CNavigation::N_LINE_BC][1] = NewPoint_C;

    pCellInfo->eLine[CNavigation::N_LINE_CA][0] = NewPoint_C;
    pCellInfo->eLine[CNavigation::N_LINE_CA][1] = NewPoint_A;

    pCellInfo->eLineInfo[CNavigation::N_LINE_AB] = m_iNeighborIndices[LINE_AB];
    pCellInfo->eLineInfo[CNavigation::N_LINE_BC] = m_iNeighborIndices[LINE_BC];
    pCellInfo->eLineInfo[CNavigation::N_LINE_CA] = m_iNeighborIndices[LINE_CA];

    pCells.push_back(pCellInfo);
}

void CCell::Set_CellAdj(vector<CNavigation::CELL*>& pCells, vector<list<CNavigation::CELL*>>& pCellAdj)
{
    pCellAdj.resize(pCells.size());

    for (auto iter : m_iNeighborIndices)
    {
        if (iter != -1)
            pCellAdj[m_iIndex].push_back(pCells[iter]);
    }
}

_float3 CCell::Get_Center()
{
    _float3 vCellCenterPoint =
    {
        (m_vPoints[POINT_A].x + m_vPoints[POINT_B].x + m_vPoints[POINT_C].x) / 3.0f,
        (m_vPoints[POINT_A].y + m_vPoints[POINT_B].y + m_vPoints[POINT_C].y) / 3.0f,
        (m_vPoints[POINT_A].z + m_vPoints[POINT_B].z + m_vPoints[POINT_C].z) / 3.0f
    };

    return vCellCenterPoint;
}


#ifdef _DEBUG
HRESULT CCell::Render()
{
    m_pVIBuffer->Bind_InputAssembler();

    m_pVIBuffer->Render();

    return S_OK;
}

#endif

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex)
{
    CCell* pInstance = new CCell(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pPoints, iIndex)))
    {
        MSG_BOX("Failed to Created : CCell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCell::Free()
{
    __super::Free();

#ifdef _DEBUG

    Safe_Release(m_pVIBuffer);

#endif // _DEBUG

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);

}

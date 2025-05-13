#include "Navigation.h"
#include "Cell.h"   
#include "Edge_Cell.h"


#ifdef _DEBUG
#include "Shader.h"
#include "GameInstance.h"   
#endif

_float4x4 CNavigation::m_WorldMatrix = {}; // static변수라 바로 초기화 못하고 이렇게 cpp와서 초기화 해줘야함.
_float4x4 CNavigation::m_WorldMatrixInverse = {}; // 그리고 또한 월드 매트릭스가 현재 여러개 생성되면 안되니깐 이렇게
// Staic 변수로 만들어줘서 하나의 메모리 주소만 생성되게 해서 공유할 수 있도록 함. 

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& Prototype)
    :CComponent(Prototype)
    , m_Cells(Prototype.m_Cells)
    , m_CellInfo{ Prototype.m_CellInfo }
    , m_CellAdj{ Prototype.m_CellAdj }
    , m_EdgeCells(Prototype.m_EdgeCells)
    , m_RenderEdgeLine(Prototype.m_RenderEdgeLine)
#ifdef _DEBUG
    , m_pShader(Prototype.m_pShader)
#endif // _DEBUG
{
#ifdef _DEBUG
    Safe_AddRef(m_pShader);
#endif // _DEBUG
    for (auto& pCell : m_Cells)
        Safe_AddRef(pCell);

    for (auto& pEdge_Render_Cell : m_RenderEdgeLine)
        Safe_AddRef(pEdge_Render_Cell);

}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFile)
{
    _ulong      dwByte = {};
    HANDLE hFile = CreateFile(pNavigationDataFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    _uint iTotalFloorNum = {};
    ReadFile(hFile, &iTotalFloorNum, sizeof(_uint), &dwByte, nullptr);


    _uint iCellCountNum = {};
    ReadFile(hFile, &iCellCountNum, sizeof(_uint), &dwByte, nullptr);

    //while (true)
    //map<_uint>
    for (_uint t = 0; t < iTotalFloorNum; ++t)
    {
        for (_uint i = 0; i < iCellCountNum; ++i)
        {
            //_float3         vPoints[CCell::POINT_END] = {};
            _float3         vPoints[CCell::POINT_END] = {};

            ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

            for (_uint i = 0; i < CCell::POINT_END; i++)
                vPoints[i].y -= 0.05f;

            // 루프 탈출문 조건
            if (0 == dwByte)
                break;

            // size 로 줘서 처음에 하나 들어가면 용량 1개, 하나 또 들어가면 2개.~~ 
            CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_uint)(m_Cells.size()));
            //CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, iCellCountNum);
            if (nullptr == pCell)
                return E_FAIL;

            m_Cells.push_back(pCell);
        }
    }

    CloseHandle(hFile);





    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_WorldMatrixInverse, XMMatrixIdentity());






#ifdef _DEBUG
    m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
    if (nullptr == m_pShader)
        return E_FAIL;
#endif

    // 이제 이걸 호출해줘야 최종적으로 현재 셀 4개 를 뛰놀수 있게 되었음. 
    if (FAILED(SetUp_Neighbors()))
        return E_FAIL;

    for (auto pCell : m_Cells)
        pCell->Set_CellInfo(m_CellInfo);

    for (auto pCell : m_Cells)
        pCell->Set_CellAdj(m_CellInfo, m_CellAdj);



    /* 엣지 라인 네비 셀 만들기 */
    for (auto& iter : m_Cells)
    {
        _uint index = {};
        for (int i = 0; i < 3; i++)
        {
            if (iter->Get_NeighborIndex((CCell::LINE)(i)) == -1)  //    LINE_AB, LINE_BC, LINE_CA, LINE_END 
            {
                if (i == 0) // AB
                {
                    _float4 Line_AB = {};
                    _vector AB_Line_Dir = XMVectorSetW(iter->Get_Point(CCell::POINT_B) - iter->Get_Point(CCell::POINT_A), 0.f);
                    XMStoreFloat4(&Line_AB, AB_Line_Dir);
                    m_Edge_Cell_Line.LINE_EDGE_1 = Line_AB;


                    /* 디버그 렌더링 용 */
                    _float3 A = { iter->Get_Point(CCell::POINT_A).m128_f32[0],iter->Get_Point(CCell::POINT_A).m128_f32[1],iter->Get_Point(CCell::POINT_A).m128_f32[2] };
                    _float3 B = { iter->Get_Point(CCell::POINT_B).m128_f32[0],iter->Get_Point(CCell::POINT_B).m128_f32[1],iter->Get_Point(CCell::POINT_B).m128_f32[2] };

                    _float3 test[2] = { A,B };

                    CEdge_Cell* pEdge_Cell = CEdge_Cell::Create(m_pDevice, m_pContext, test);

                    if (nullptr == pEdge_Cell)
                        return E_FAIL;

                    m_RenderEdgeLine.push_back(pEdge_Cell);


                }

                else if (i == 1)
                {
                    _float4 Line_BC = {};
                    _vector BC_Line_Dir = XMVectorSetW(iter->Get_Point(CCell::POINT_C) - iter->Get_Point(CCell::POINT_B), 0.f);
                    XMStoreFloat4(&Line_BC, BC_Line_Dir);
                    m_Edge_Cell_Line.LINE_EDGE_2 = Line_BC;



                    /* 디버그 렌더링 용 */
                    _float3 B = { iter->Get_Point(CCell::POINT_B).m128_f32[0],iter->Get_Point(CCell::POINT_B).m128_f32[1],iter->Get_Point(CCell::POINT_B).m128_f32[2] };
                    _float3 C = { iter->Get_Point(CCell::POINT_C).m128_f32[0],iter->Get_Point(CCell::POINT_C).m128_f32[1],iter->Get_Point(CCell::POINT_C).m128_f32[2] };

                    _float3 test[2] = { B,C };

                    CEdge_Cell* pEdge_Cell = CEdge_Cell::Create(m_pDevice, m_pContext, test);

                    if (nullptr == pEdge_Cell)
                        return E_FAIL;

                    m_RenderEdgeLine.push_back(pEdge_Cell);



                }

                else if (i == 2)
                {
                    _float4 Line_CA = {};
                    _vector CA_Line_Dir = XMVectorSetW(iter->Get_Point(CCell::POINT_A) - iter->Get_Point(CCell::POINT_C), 0.f);
                    XMStoreFloat4(&Line_CA, CA_Line_Dir);
                    m_Edge_Cell_Line.LINE_EDGE_3 = Line_CA;


                    /* 디버그 렌더링 용 */
                    _float3 C = { iter->Get_Point(CCell::POINT_C).m128_f32[0],iter->Get_Point(CCell::POINT_C).m128_f32[1],iter->Get_Point(CCell::POINT_C).m128_f32[2] };
                    _float3 A = { iter->Get_Point(CCell::POINT_A).m128_f32[0],iter->Get_Point(CCell::POINT_A).m128_f32[1],iter->Get_Point(CCell::POINT_A).m128_f32[2] };

                    _float3 test[2] = { C,A };

                    CEdge_Cell* pEdge_Cell = CEdge_Cell::Create(m_pDevice, m_pContext, test);

                    if (nullptr == pEdge_Cell)
                        return E_FAIL;

                    m_RenderEdgeLine.push_back(pEdge_Cell);
                }

            }

        }
        m_Edge_Cell_Line.NeighborIndex[0] = { iter->Get_NeighborIndex(CCell::LINE_AB) };
        m_Edge_Cell_Line.NeighborIndex[1] = { iter->Get_NeighborIndex(CCell::LINE_BC) };
        m_Edge_Cell_Line.NeighborIndex[2] = { iter->Get_NeighborIndex(CCell::LINE_CA) };



        m_EdgeCells.push_back(m_Edge_Cell_Line);

        ZeroMemory(&m_Edge_Cell_Line, sizeof(EDGE_CELL_LINE));

        index++;
    }


    return S_OK;
}

HRESULT CNavigation::Initialize_Prototype_CreateOnly()
{
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_WorldMatrixInverse, XMMatrixIdentity());

#ifdef _DEBUG
    m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
    if (nullptr == m_pShader)
        return E_FAIL;
#endif

    if (FAILED(SetUp_Neighbors()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
    if (nullptr != pArg)
    {
        NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

        m_iCurrentCellIndex = pDesc->iCurrentCellIndex;
    }

    return S_OK;
}

void CNavigation::Set_CurrentNaviIndex(_vector _vWorldPos)
{
    _int                iNeighborIndex = { -1 };

    for (_uint i = 0; i < m_Cells.size(); ++i)
    {
        if (true == m_Cells[i]->isIn(_vWorldPos, &iNeighborIndex))
        {
            m_iCurrentCellIndex = i;
            return;
        }
    }
}

_uint CNavigation::Find_Closest_Cell(/*_uint _iFloorNum, */_vector _vWorldPos)
{
    _float fMinsDistance = FLT_MAX;
    _uint iClosestCellIndex = 0;
    _float fPosX = XMVectorGetX(_vWorldPos);
    _float fPosY = XMVectorGetY(_vWorldPos);
    _float fPosZ = XMVectorGetZ(_vWorldPos);


    for (_uint i = 0; i < m_Cells.size(); ++i)
    {
        XMFLOAT3 vCellCenter = m_Cells[i]->Get_Center();
        _float fDistance = sqrt(
            powf(fPosX - vCellCenter.x, 2) +
            powf(fPosY - vCellCenter.y, 2) +
            powf(fPosZ - vCellCenter.z, 2)
        );

        if (fDistance < fMinsDistance)
        {
            fMinsDistance = fDistance;
            iClosestCellIndex = i;
        }
    }

    m_iCurrentCellIndex = iClosestCellIndex;    

    return iClosestCellIndex;
}

void CNavigation::Set_NaviFloor(_uint _iFloorNumber)
{
    m_iFloorNumber = _iFloorNumber;

    m_iCurrentCellIndex = 0;
}

_bool CNavigation::isMove(_fvector vWorldPos)
{
    _vector     vPosition = XMVector3TransformCoord(vWorldPos, XMLoadFloat4x4(&m_WorldMatrixInverse));

    _int        iNeighborIndex = { -1 }; // -1로 설정되면 이웃이 없다는 거를 의미.
    m_iNeighborIndex = iNeighborIndex;

    if (false == m_Cells[m_iCurrentCellIndex]->isIn(vPosition, &iNeighborIndex))
    {
        /* 나간 방향에 이웃이 있다면? */
        if (-1 != iNeighborIndex)
        {
            while (true)
            {
                if (-1 == iNeighborIndex)
                    return false;
                if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex))
                    break;
            }

            m_iCurrentCellIndex = iNeighborIndex;
            m_iNeighborIndex = iNeighborIndex;
            return true;
        }
        else
        {
            m_iNeighborIndex = -1;
            return false;
        }
    }
    else
    {
        m_iNeighborIndex = iNeighborIndex;
        return true;
    }

}

_float CNavigation::Compute_Height(_fvector vWorldPos)
{
    if (-1 == m_iCurrentCellIndex)
        return 0.f;

    _vector vPosition = XMVector3TransformCoord(vWorldPos, XMLoadFloat4x4(&m_WorldMatrixInverse));

    vPosition = XMVectorSetY(vPosition, m_Cells[m_iCurrentCellIndex]->Compute_Height(vPosition));

    vPosition = XMVector3TransformCoord(vPosition, XMLoadFloat4x4(&m_WorldMatrix));

    return XMVectorGetY(vPosition);
}

_vector CNavigation::Setting_SlidingMove(_fvector _vWorldPos, _fvector _vLook)
{
    if (m_iNeighborIndex == -1)
    {
        _vector vPosition = XMVector3TransformCoord(_vWorldPos, XMLoadFloat4x4(&m_WorldMatrixInverse));

        for (_uint i = 0; i < CCell::LINE_END; ++i)
        {

            if (i == 0)
            {
                if (XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_1))) == 0.f)   // 없으면 다 0이니깐 이렇게 부여함 
                {
                    _float  Distance = 1000.f;    // 제일 뒤로 보내기 위해 높은 숫자로 설정     
                    _vector  vNormal = { 0.f,0.f,0.f,0.f }; // 임시 노말 값 그냥 의미 x  


                    _float4 vNormalVector = {};
                    XMStoreFloat4(&vNormalVector, vNormal);

                    m_mapSildeMap.emplace(Distance, vNormalVector);
                }

                else
                {
                    // 방향 벡터 구하기 .  
                    _vector vDir = XMVectorSetW(XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_1) - vPosition, 0.f);
                    // 해당 라인    
                    _vector vLine = (XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_1));
                    // 법선 벡터 구하기 
                    _vector vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

                    /* 해당 물체와 직선 위의 한점 간의 거리 구하기  */

                    _vector PA = _vWorldPos - m_Cells[m_iCurrentCellIndex]->Get_Point((CCell::POINT)i);
                    _vector Cross = XMVector3Cross(vLine, PA);

                    _float  Distance = XMVectorGetX(XMVector3Length(Cross) / XMVector3Length(vDir));

                    _float4 vNormalVector = {};
                    XMStoreFloat4(&vNormalVector, vNormal);

                    m_mapSildeMap.emplace(Distance, vNormalVector);
                }
            }

            else if (i == 1)
            {
                if (XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_2))) == 0.f)   // 없으면 다 0이니깐 이렇게 부여함   
                {
                    _float  Distance = 1000.f;    // 제일 뒤로 보내기 위해 높은 숫자로 설정      
                    _vector  vNormal = { 0.f,0.f,0.f,0.f }; // 임시 노말 값 그냥 의미 x     


                    _float4 vNormalVector = {};
                    XMStoreFloat4(&vNormalVector, vNormal);

                    m_mapSildeMap.emplace(Distance, vNormalVector);
                }

                else
                {
                    // 방향 벡터 구하기 .  
                    _vector vDir = XMVectorSetW(XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_2) - vPosition, 0.f);
                    // 해당 라인    
                    _vector vLine = (XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_2));
                    // 법선 벡터 구하기 
                    _vector vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

                    /* 해당 물체와 직선 위의 한점 간의 거리 구하기  */

                    _vector PA = _vWorldPos - m_Cells[m_iCurrentCellIndex]->Get_Point((CCell::POINT)i);
                    _vector Cross = XMVector3Cross(vLine, PA);

                    _float  Distance = XMVectorGetX(XMVector3Length(Cross) / XMVector3Length(vDir));

                    _float4 vNormalVector = {};
                    XMStoreFloat4(&vNormalVector, vNormal);

                    m_mapSildeMap.emplace(Distance, vNormalVector);
                }
            }

            else if (i == 2)
            {
                if (XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_3))) == 0.f)   // 없으면 다 0이니깐 이렇게 부여함   
                {
                    _float  Distance = 1000.f;    // 제일 뒤로 보내기 위해 높은 숫자로 설정      
                    _vector  vNormal = { 0.f,0.f,0.f,0.f }; // 임시 노말 값 그냥 의미 x      


                    _float4 vNormalVector = {};
                    XMStoreFloat4(&vNormalVector, vNormal);

                    m_mapSildeMap.emplace(Distance, vNormalVector);
                }

                else
                {
                    // 방향 벡터 구하기 .  
                    _vector vDir = XMVectorSetW(XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_3) - vPosition, 0.f);
                    // 해당 라인    
                    _vector vLine = (XMLoadFloat4(&m_EdgeCells[m_iCurrentCellIndex].LINE_EDGE_3));
                    // 법선 벡터 구하기 
                    _vector vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

                    /* 해당 물체와 직선 위의 한점 간의 거리 구하기  */

                    _vector PA = _vWorldPos - m_Cells[m_iCurrentCellIndex]->Get_Point((CCell::POINT)i);
                    _vector Cross = XMVector3Cross(vLine, PA);

                    _float  Distance = XMVectorGetX(XMVector3Length(Cross) / XMVector3Length(vDir));

                    _float4 vNormalVector = {};
                    XMStoreFloat4(&vNormalVector, vNormal);

                    m_mapSildeMap.emplace(Distance, vNormalVector);
                }
            }
        }

        // 여기까지가 이제 제일 가까운 직선 구하고 해당 직선의 법선 벡터 구한거고 
        auto& iter = m_mapSildeMap.begin();

        /* 이제 입사각 벡터만 구하면 된다리.*/
        /* 아 플레이어의 look 이잔항;;*/

        /* 물체의 직선 입사각 방향 벡터*/
        _vector PlayerDir = XMVectorSetW(XMVector3Normalize(_vLook), 0.f);

        /* 직선의 법선 벡터*/
        _vector LineNormal = XMVector3Normalize(XMLoadFloat4(&iter->second));

        // 입사 벡터에서 법선 방향 성분 제거 (슬라이딩 벡터 계산)
        _vector ReflectAmount = XMVector3Dot(PlayerDir, LineNormal);
        _vector SlideVector = PlayerDir - ReflectAmount * LineNormal;

        // W값 명시적으로 0 설정
        SlideVector = XMVectorSetW(SlideVector, 0.f);

        m_mapSildeMap.clear();

        return  SlideVector;
    }

    _vector Example = {};

    return Example;
}

_bool CNavigation::bIsOn_Line(_fvector _vWorldPos)
{
    _vector vPosition = XMVector3TransformCoord(_vWorldPos, XMLoadFloat4x4(&m_WorldMatrixInverse));

    for (_uint i = 0; i < CCell::LINE_END; ++i)
    {
        _vector vDir = (vPosition - m_Cells[m_iCurrentCellIndex]->Get_Point((CCell::POINT)i));
        _vector vLine = (m_Cells[m_iCurrentCellIndex]->Get_Point((CCell::POINT)((i + 1) % 3)) - m_Cells[m_iCurrentCellIndex]->Get_Point((CCell::POINT)i));
        _vector vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.0f, 0.0f, XMVectorGetX(vLine), 0.0f));

        // 음수이면 안에있다.양수이면 바깥에 있다.
        if (0.0f < XMVectorGetX(XMVector3Dot(vDir, vNormal)))
        {
            return true;
        }
    }

    return false;
}

_bool CNavigation::MakeRoute(_int iStartIndex, _int iGoalIndex)
{
    if (!m_OpenList.empty())
        m_OpenList.pop_front();

    m_CloseList.push_back(iStartIndex);

    for (auto& pCell : m_CellAdj[iStartIndex])
    {
        if (iGoalIndex == pCell->iIndex)
        {
            pCell->iParentIndex = iStartIndex;
            return true;
        }
        if (!CheckOpen(pCell->iIndex) && !CheckClose(pCell->iIndex))
        {
            pCell->iParentIndex = iStartIndex;
            m_OpenList.push_back(pCell->iIndex);

        }
    }

    if (m_OpenList.empty())
        return false;

    int iStartIdx = m_iStartIndex;

    vector<CELL*>& vecCell = m_CellInfo;

    m_OpenList.sort([&vecCell, &iGoalIndex, &iStartIdx](int Dst, int Src)->bool
        {
            _vector vPCost1 = XMLoadFloat3(&vecCell[iStartIdx]->vPos) - XMLoadFloat3(&vecCell[Dst]->vPos);
            _vector vPCost2 = XMLoadFloat3(&vecCell[iStartIdx]->vPos) - XMLoadFloat3(&vecCell[Src]->vPos);

            _vector vGCost1 = XMLoadFloat3(&vecCell[iGoalIndex]->vPos) - XMLoadFloat3(&vecCell[Dst]->vPos);
            _vector vGCost2 = XMLoadFloat3(&vecCell[iGoalIndex]->vPos) - XMLoadFloat3(&vecCell[Src]->vPos);

            _float	fCost1 = XMVectorGetX((XMVector3Length(vPCost1) + XMVector3Length(vGCost1)));
            _float	fCost2 = XMVectorGetX((XMVector3Length(vPCost2) + XMVector3Length(vGCost2)));

            return fCost1 < fCost2;
        });


    return MakeRoute(m_OpenList.front(), iGoalIndex);
}

void CNavigation::MakeBestList(_int iStartIndex, _int iGoalIndex)
{
    m_BestList.push_front(m_CellInfo[iGoalIndex]);

    _int iRouteIndex = m_CellInfo[iGoalIndex]->iParentIndex;

    while (true)
    {
        if (iStartIndex == iRouteIndex)
            break;
        m_BestList.push_front(m_CellInfo[iRouteIndex]);

        iRouteIndex = m_CellInfo[iRouteIndex]->iParentIndex;
    }
}

void CNavigation::Start_Astar(_uint iGoalIndex)
{
    Clear_Astar();

    m_iStartIndex = m_iCurrentCellIndex;

    //각종 예외처리
    if (m_iStartIndex < 0 || iGoalIndex < 0 || m_Cells.size() <= m_iStartIndex || m_Cells.size() <= iGoalIndex)
        return;

    //시작지점과 도착지점이 같으면 return;
    if (m_iStartIndex == iGoalIndex)
        return;

    if (MakeRoute(m_iStartIndex, iGoalIndex))
        MakeBestList(m_iStartIndex, iGoalIndex);

    m_Portal.clear();
    for (auto iter = m_BestList.begin(); iter != m_BestList.end();)
    {
        CELL* pCell = *iter;


        _int j = 0;

        for (; j < N_LINE_END; j++)
        {
            if (pCell->eLineInfo[j] == pCell->iParentIndex)
                break;
        }

        list<_vector> pPortals;

        pPortals.push_front(XMLoadFloat3(&pCell->eLine[j][0]));
        pPortals.push_front(XMLoadFloat3(&pCell->eLine[j][1]));

        for (auto pPotal : pPortals)
        {
            _float4 fPotal;
            XMStoreFloat4(&fPotal, pPotal);
            m_Portal.push_back(fPotal);
        }
        iter++;
    }
}

void CNavigation::Clear_Astar()
{
    m_OpenList.clear();
    m_CloseList.clear();
    m_BestList.clear();
}

_vector CNavigation::MoveAstar(_vector pCurPos, _bool& isEmpty)
{
    if (!m_BestList.empty())
    {
        vector<_vector> vPoints;
        _float3 fCurPos = {};

        XMStoreFloat3(&fCurPos, pCurPos);

        Navigate_Portals(m_Portal, fCurPos, m_BestList.back()->vPos, vPoints);

        _vector vDir = vPoints.front() - pCurPos;

        _float fDistance = XMVectorGetX(XMVector3Length(vDir));

        if (fDistance <= 3.0f)
            m_BestList.pop_front();

        _vector vCurPos = XMVectorSet(XMVectorGetX(vDir), 0.f, XMVectorGetZ(vDir), 0.f);
        pCurPos = XMVector3Normalize(vCurPos);
        isEmpty = true;
    }
    else
        isEmpty = false;

    return pCurPos;
}

_bool CNavigation::CheckOpen(_int iIndex)
{
    for (auto& iOpenIndex : m_OpenList)
    {
        if (iIndex == iOpenIndex)
            return true;
    }

    return false;
}

_bool CNavigation::CheckClose(_int iIndex)
{
    for (auto& iCloseIndex : m_CloseList)
    {
        if (iIndex == iCloseIndex)
            return true;
    }

    return false;
}
//주어진 두 점 a와 b가 실질적으로 동일한지 비교하는 함수
_bool CNavigation::IsPointsClose(_vector fPointA, const _vector fPointB)
{
    static const _float Close_Enough = 0.001f * 0.001f;
    return Calculate_Squared_Distance(fPointA, fPointB) < Close_Enough;
}
//주어진 세 점으로 이루어진 삼각형의 면적의 두 배를 반환하는 함수.
_float CNavigation::TriangleArea2x(const _vector fPointA, const _vector fPointB, const _vector fPointC)
{
    const _float DxAB = XMVectorGetX(fPointB) - XMVectorGetX(fPointA);
    const _float DyAB = XMVectorGetZ(fPointB) - XMVectorGetZ(fPointA);;
    const _float DxAC = XMVectorGetX(fPointC) - XMVectorGetX(fPointA);
    const _float DyAC = XMVectorGetZ(fPointC) - XMVectorGetZ(fPointA);

    return DxAC * DyAB - DxAB * DyAC;
}
//두 벡터 간의 거리에 제곱을 계산 하는 함수
_float CNavigation::Calculate_Squared_Distance(const _vector fPointA, const _vector fPointB)
{
    return XMVectorGetX(XMVector3Length(fPointB - fPointA));
}

_int CNavigation::Navigate_Portals(vector<_float4> pPortals, _float3 fStartPos, _float3 fEndPos, vector<_vector>& vPoints)
{
    _int iNumPoints = {};
    _vector vCurrentPotal, vLeftPortal, vRightPortal;
    _int iCurrentPotalIndex{}, iLeftPortalIndex{}, iRightPortalIndex{};

    vCurrentPotal = XMLoadFloat3(&fStartPos);
    vLeftPortal = XMLoadFloat4(&pPortals[0]);
    vRightPortal = XMLoadFloat4(&pPortals[1]);

    iNumPoints++;

    for (_int i = 1; (i * 2) < pPortals.size(); i++)
    {
        _vector vLeft = XMLoadFloat4(&pPortals[(i * 2) - 2]); // 왼쪽 포탈
        _vector vRight = XMLoadFloat4(&pPortals[(i * 2) - 1]); // 오른쪽 포탈

        if (TriangleArea2x(vCurrentPotal, vRightPortal, vRight) <= 0.f)
        {
            if (IsPointsClose(vCurrentPotal, vRightPortal) || TriangleArea2x(vCurrentPotal, vLeftPortal, vRight) > 0.f)
            {
                vRightPortal = vRight;
                iRightPortalIndex = i;
            }
            else
            {
                vPoints.push_back(vLeftPortal);
                iNumPoints++;
                vCurrentPotal = vLeftPortal;
                iCurrentPotalIndex = iLeftPortalIndex;

                vLeftPortal = vCurrentPotal;
                vRightPortal = vCurrentPotal;
                iLeftPortalIndex = iCurrentPotalIndex;
                iRightPortalIndex = iCurrentPotalIndex;

                i = iCurrentPotalIndex;
                continue;
            }

        }

        if (TriangleArea2x(vCurrentPotal, vLeftPortal, vLeft) >= 0.f)
        {
            if (IsPointsClose(vCurrentPotal, vLeftPortal) || TriangleArea2x(vCurrentPotal, vRightPortal, vLeft) < 0.f)
            {
                vLeftPortal = vLeft;
                iLeftPortalIndex = i;
            }
            else
            {
                vPoints.push_back(vRightPortal);
                iNumPoints++;
                vCurrentPotal = vRightPortal;
                iCurrentPotalIndex = iRightPortalIndex;

                vLeftPortal = vCurrentPotal;
                vRightPortal = vCurrentPotal;
                iLeftPortalIndex = iCurrentPotalIndex;
                iRightPortalIndex = iCurrentPotalIndex;
                i = iCurrentPotalIndex;
                continue;
            }
        }
    }

    vPoints.push_back(XMLoadFloat3(&fEndPos));
    iNumPoints++;

    return iNumPoints;
}



#ifdef _DEBUG
HRESULT CNavigation::Render()
{
    if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    _float4  vColor = -1 == m_iCurrentCellIndex ? _float4(0.f, 1.f, 0.f, 1.f) : _float4(1.f, 0.f, 0.f, 1.f);

    if (FAILED(m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4))))
        return E_FAIL;

    /* 5월 8일 */
    if (m_pGameInstance->isKeyEnter(DIK_9)) 
        m_bNaviRenderOnOff = !m_bNaviRenderOnOff;   

    if (m_pGameInstance->isKeyEnter(DIK_0)) 
        m_bEdgeNaviRenderOnOff = !m_bEdgeNaviRenderOnOff;   

    if (-1 == m_iCurrentCellIndex)
    {
        m_pShader->Begin(0);

        if (m_bNaviRenderOnOff)
        {
             
            for (auto& pCell : m_Cells) 
                pCell->Render();    
        }

        if (m_bEdgeNaviRenderOnOff) 
        {
              
            for (auto& pDebug_Edge_Cell : m_RenderEdgeLine)  //이거 키면 랜더링 슬라이드용 네비       
                pDebug_Edge_Cell->Render();     
        }
    }

    else
    {
        _float4x4 WorldMatrix = m_WorldMatrix;
        
        WorldMatrix.m[3][1] += 0.1f;  // 이거는 월드매트릭스에 직접 접근한거 3번째줄이 POSITION줄을 의미하고 거기에 1번은 Y값
        
        if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
            return E_FAIL;
        
        m_pShader->Begin(0);
        
        m_Cells[m_iCurrentCellIndex]->Render();
    }

    return S_OK;
}
#endif

HRESULT CNavigation::SetUp_Neighbors()
{
    for (auto& pSourCell : m_Cells)
    {
        for (auto& pDestCell : m_Cells)
        {
            if (pSourCell == pDestCell)
                continue;

            if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
                pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
            if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
                pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
            if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
                pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
        }
    }

    return S_OK;
}

HRESULT CNavigation::SetUp_EDGE_Neighbors()
{
    for (auto& iter : m_EdgeCells)
    {

        for (int i = 0; i < 3; i++)
        {
            if (i == 0)
            {
                if (iter.NeighborIndex[i] != -1)
                {

                    if (XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_EdgeCells[iter.NeighborIndex[i]].LINE_EDGE_1))) > 0.f)
                    {
                        iter.LINE_EDGE_NEIGHBOR[i] = m_EdgeCells[iter.NeighborIndex[i]].LINE_EDGE_1;
                    }

                    if (XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_EdgeCells[iter.NeighborIndex[i]].LINE_EDGE_2))) > 0.f)
                    {
                        iter.LINE_EDGE_NEIGHBOR[i] = m_EdgeCells[iter.NeighborIndex[i]].LINE_EDGE_2;
                    }

                    if (XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_EdgeCells[iter.NeighborIndex[i]].LINE_EDGE_3))) > 0.f)
                    {
                        iter.LINE_EDGE_NEIGHBOR[i] = m_EdgeCells[iter.NeighborIndex[i]].LINE_EDGE_3;
                    }

                }
            }

        }

    }


    return S_OK;
}



CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile)
{
    CNavigation* pInstance = new CNavigation(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFile)))
    {
        MSG_BOX("Failed To Created : CNavigation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CNavigation* CNavigation::CreateOnly(ID3D11Device* pdevice, ID3D11DeviceContext* pContext)
{
    CNavigation* pInstance = new CNavigation(pdevice, pContext);
    if (FAILED(pInstance->Initialize_Prototype_CreateOnly()))
    {
        MSG_BOX("Failed To Created : CNavigation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

HRESULT CNavigation::Create_Cell(const _float3 _vPoints[3])
{
    CCell* pCell = CCell::Create(m_pDevice, m_pContext, _vPoints, (_uint)(m_Cells.size()));
    if (nullptr == pCell)
        return E_FAIL;

    m_Cells.push_back(pCell);

    cout << "셀 하나 생성! 후 push_back 완료!" << endl;

    return S_OK;
}

HRESULT CNavigation::Delete_Cell(const _float3 _vPoints[3])
{
    for (auto iter = m_Cells.begin(); iter != m_Cells.end(); ++iter)
    {
        if ((*iter)->Compare_Points(XMLoadFloat3(&_vPoints[0]), XMLoadFloat3(&_vPoints[1])) &&
            (*iter)->Compare_Points(XMLoadFloat3(&_vPoints[1]), XMLoadFloat3(&_vPoints[2])) &&
            (*iter)->Compare_Points(XMLoadFloat3(&_vPoints[2]), XMLoadFloat3(&_vPoints[0])))
        {
            Safe_Release(*iter);
            m_Cells.erase(iter);

            return S_OK;
        }
    }

    return E_FAIL;
}

CComponent* CNavigation::Clone(void* pArg)
{
    CNavigation* pInstance = new CNavigation(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CNavigation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNavigation::Free()
{
    __super::Free();

#ifdef _DEBUG
    Safe_Release(m_pShader);
#endif

    for (auto& pCell : m_Cells)
        Safe_Release(pCell);

    for (auto& pEdge_Render_Cell : m_RenderEdgeLine)
        Safe_Release(pEdge_Render_Cell);


    m_CellAdj.clear();

    if (!m_isCloned)
        for (auto& pCellInfo : m_CellInfo)
            Safe_Delete(pCellInfo);
    m_CellInfo.clear();

    m_Cells.clear();

    m_EdgeCells.clear();
}

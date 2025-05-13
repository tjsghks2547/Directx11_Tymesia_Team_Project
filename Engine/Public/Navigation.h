#pragma once

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CNavigation final : public CComponent
{
public:
	enum NAVI_LINE { N_LINE_AB, N_LINE_BC, N_LINE_CA, N_LINE_END };
	enum NAVI_POINT { N_POINT_A, N_POINT_B, N_POINT_C, N_POINT_END };
public:
	struct NAVIGATION_DESC
	{
		_int   iCurrentCellIndex = { -1 };
	};

	typedef struct CELL_INFO
	{
		_float3 vPos;

		_int byOption;
		_int iIndex;
		_int iParentIndex;

		_int eLineInfo[N_LINE_END];

		_float3 ePoint[N_POINT_END];
		_float3 eLine[N_LINE_END][2];

	}CELL;


	struct EDGE_CELL_LINE
	{
		//직선 3개 넣기 
		_float4  LINE_EDGE_1;
		_float4  LINE_EDGE_2;
		_float4  LINE_EDGE_3;

		_float4  LINE_EDGE_NEIGHBOR[6]; // 최대 이웃이 6개일 수 밖에 없음. 		


		_int     NeighborIndex[3];
	};

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;


public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFile);
	HRESULT	        Initialize_Prototype_CreateOnly();
	virtual HRESULT Initialize(void* pArg) override;

public:
	void SetUp_WorldMatrix(_fmatrix WorldMatrix)
	{
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
		XMStoreFloat4x4(&m_WorldMatrixInverse, XMMatrixInverse(nullptr, WorldMatrix));
	}

	void                  Set_CurrentNaviIndex(_vector _vWorldPos);
	_uint                 Find_Closest_Cell(/*_uint _iFloorNum, */_vector _vWorldPos);

public:
	void                  Set_NaviFloor(_uint _iFloorNumber);
	_uint                 Get_NaviFloorNumber() { return m_iFloorNumber; }

public:
	_bool                 isMove(_fvector vWorldPos);
	_float                Compute_Height(_fvector vWorldPos);
	vector<class CCell*>* Get_VecCells() { return &m_Cells; }
	void                  Set_CurCellIndex(_uint _CellIndex) { m_iCurrentCellIndex = _CellIndex; }
	_uint                 Get_CurCellIndex() { return m_iCurrentCellIndex; }
	_uint                 Get_BestListSize() { return static_cast<_uint>(m_BestList.size()); }
	_vector               Setting_SlidingMove(_fvector _vWorldPos, _fvector _vLook);
	_bool                 bIsOn_Line(_fvector _vWorldPos);

public:
	_bool                 MakeRoute(_int iStartIndex, _int iGoalIndex);
	void                  MakeBestList(_int iStartIndex, _int iGoalIndex);
	void                  Start_Astar(_uint iGoalIndex);
	void                  Clear_Astar();
	_vector               MoveAstar(_vector pCurPos, _bool& isEmpty);

public:
	_bool                 CheckOpen(_int iIndex);
	_bool                 CheckClose(_int iIndex);
	_bool                 IsPointsClose(_vector fPointA, const _vector fPointB);
public:
	_float                TriangleArea2x(const _vector fPointA, const _vector fPointB, const _vector fPointC);
	_float                Calculate_Squared_Distance(const _vector fPointA, const _vector fPointB);
	_int                  Navigate_Portals(vector<_float4> pPortals, _float3 fStartPos, _float3 fEndPos, vector<_vector>& vPoints);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	/* 이 네비를 이용하는 객체가 현재 어떤 셀 안에 있는지? */
	_int					m_iCurrentCellIndex = { -1 };
	vector<class CCell*>	m_Cells;
	vector<class CEdge_Cell*> m_RenderEdgeLine;
	vector<EDGE_CELL_LINE>  m_EdgeCells;
	EDGE_CELL_LINE			m_Edge_Cell_Line;

	static _float4x4        m_WorldMatrix;
	static _float4x4        m_WorldMatrixInverse;
	_int                    m_iNeighborIndex = {};

	_uint					m_iBeslist_Count = {};


private:
	class CShader*			m_pShader = { nullptr };

	
private:
	_uint					m_iFloorNumber = {};


private:
	_int                   m_iStartIndex = {};
	list<_int>             m_CloseList;
	list<_int>             m_OpenList;
	list<CELL*>            m_BestList;

	vector<CELL*>          m_CellInfo;
	vector<list<CELL*>>    m_CellAdj;

	vector<_float4>        m_Portal;

	/* 슬라이딩 벡터 관련 함수와 변수 들 */
private:
	map<_float, _float4>  m_mapSildeMap;
	_float4				  m_PreDir = {};
	_float4				  m_PreSliderDir = {};
	_bool				  m_bFirst = { true };


	_bool				  m_bNaviRenderOnOff = { false };
	_bool				  m_bEdgeNaviRenderOnOff = { false };


public:
	void Set_PreDir(_float4 _PreDir) { m_PreDir = _PreDir; }
	void Set_First(_bool _First) { m_bFirst = _First; }
	/* ========================= */
private:
	HRESULT SetUp_Neighbors();
	HRESULT SetUp_EDGE_Neighbors();


public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile);
	static CNavigation* CreateOnly(ID3D11Device* pdevice, ID3D11DeviceContext* pContext);


	HRESULT	Create_Cell(const _float3 _vPoints[3]);
	HRESULT Delete_Cell(const _float3 _vPoints[3]);


	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END

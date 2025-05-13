#pragma once

#include "Base.h"
#include "Navigation.h"

BEGIN(Engine)

class CEdge_Cell final : public CBase
{
private:
	CEdge_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEdge_Cell() = default;

public:
	HRESULT Initialize(const _float3* pPoints);


#ifdef  _DEBUG
public:
	HRESULT Render();

#endif


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	_float3			m_vPoints[2] = {};

#ifdef _DEBUG
private:
	class CVIBuffer_Edge_Cell* m_pVIBuffer = { nullptr };
#endif


public:
	static CEdge_Cell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints);
	virtual void  Free() override;

};

END
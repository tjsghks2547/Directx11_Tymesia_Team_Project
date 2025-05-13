#pragma once

#include "VIBuffer.h"


BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Edge_Cell final : public CVIBuffer
{
private:
	CVIBuffer_Edge_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Edge_Cell(const CVIBuffer_Edge_Cell& Prototype);
	virtual ~CVIBuffer_Edge_Cell() = default;

public:
	virtual HRESULT Initialize_Prototype(const _float3* pPoints);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Edge_Cell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END

#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSwordTrail_Buffer final : public CVIBuffer
{
private:
	CSwordTrail_Buffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSwordTrail_Buffer(const CSwordTrail_Buffer& Prototype);
	virtual ~CSwordTrail_Buffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Set_Trail_Local(deque<_float3>& _dequeCenterPos, _uint _idequeCount, const _float3& _vRight, const _float3& _vUp);
	HRESULT Set_Trail_Reset();

public:
	static CSwordTrail_Buffer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END
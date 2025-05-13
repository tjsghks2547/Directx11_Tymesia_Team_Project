#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Terrain : public CVIBuffer
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _uint dwCntX, const  _uint dwCntZ, _uint dwVertexItv, const _tchar* path, const _tchar* _HeightPath = nullptr);
	virtual HRESULT Initialize(void* pArg) override;

public:
	_float					Get_Height(const XMFLOAT3& _vPos);
	_float3					Get_TerrainPos() { return m_fTerrainPos; }
	void 				    Set_TerrainPos(_float3 _fPos) { m_fTerrainPos = _fPos; }	

private:
	_float4                 m_vPlane = {};
	//XMVECTOR*				m_VertexPos = { nullptr };
	_float4*				m_VertexPos = { nullptr };
	VTXNORTEX*				m_pVertices = { nullptr };
	_int                   m_iNumverticesX, m_iNumverticesZ;
	_float3					m_fTerrainPos = {};


public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _uint dwCntX, const  _uint dwCntZ, _uint dwVertexItv, const _tchar* pPath, const _tchar* _HeightPath = nullptr);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once

#include "VIBuffer_Particle_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Mesh_Instancing final : public CVIBuffer_Particle_Instancing
{
public:
	struct PARTICLE_MESH_INSTANCING_DESC : public CVIBuffer_Particle_Instancing::PARTICLE_INSTANCING_DESC
	{
		/* ���� ������ �� �ν��Ͻ� ���� �� �־��ֱ� ���� �����Ͽ���.*/
	};

private:
	CVIBuffer_Mesh_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Mesh_Instancing(const CVIBuffer_Mesh_Instancing& Prototype);
	virtual ~CVIBuffer_Mesh_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype(const PARTICLE_INSTANCING_DESC* pArg) override;
	virtual HRESULT Initialize(void* pArg) override;

private:

public:
	static  CVIBuffer_Mesh_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_INSTANCING_DESC* pArg);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;


};

END
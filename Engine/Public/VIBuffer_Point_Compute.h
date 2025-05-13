#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Compute final : public CVIBuffer
{
public:
	struct PARTICLE_COMPUTE_DESC
	{
		_uint		iNumInstance = {};
		_float3     vRange = {};
		_float3     vCenter = {};
		_float2     vSize = {};
		_float2     vSpeed = {};
		_float2     vLifeTime = {};
		_float3		vPivot = {};
		_bool		bReverse_XYZ[3] = { false, false, false };
		_float3		vSpeed_Weight; //속도 가중치
		_float3		vScale_Weight; //Scale 가중치

		_uint		iParticle_Initialize_Type = { 0 }; // Particle 시작위치가 원점기준이냐(0), 중심기준 원모양이냐(1), 중심기준 구모양이냐(2)
		_float2		vDelayTime = _float2(0.f, 0.f); // Particle 시작위치가 원점기준이냐(0), 중심기준 원모양이냐(1), 중심기준 구모양이냐(2)
	};

	struct COMPUTE_POSITION
	{
		_float4     vPos = {};
	};

private:
	CVIBuffer_Point_Compute(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Point_Compute(const CVIBuffer_Point_Compute& _Prototype);
	virtual ~CVIBuffer_Point_Compute() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_InputAssembler() override;
	virtual HRESULT Render() override;
	HRESULT Compute_Shader(class CShader_Compute* _pComputeShader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ);
	HRESULT Compute_Shader(class CShader_Compute* _pComputeShader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, _float4 _vStartPos);
	HRESULT Compute_Shader_Reset(class CShader_Compute* _pComputeShader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ);

private:

	D3D11_BUFFER_DESC				m_InstanceBufferDesc = {};
	D3D11_SUBRESOURCE_DATA			m_InstanceInitialData = {};
	_uint							m_iInstanceVertexStride = {};
	_uint							m_iNumIndexPerInstance = {};
	_uint							m_iNumInstance = {};

	COMPUTE_PARTICLE_INSTANCE*		m_pInstanceVertices = { nullptr };
	ID3D11Buffer*					m_pVBInstance = { nullptr };

	_bool							m_isLoop = { false };

	ID3D11UnorderedAccessView*		m_pUAV = { nullptr };
	ID3D11ShaderResourceView*		m_pSRV = { nullptr };

	ID3D11Buffer*					m_pBuffer_UAV = { nullptr };
	ID3D11Buffer*					m_pBuffer_SRV = { nullptr };
	ID3D11Buffer*					m_pBuffer_Copy = { nullptr };
	ID3D11Buffer*					m_pBuffer_StartPosition = { nullptr };
	ID3D11Buffer*					m_pBuffer_CameraPosition = { nullptr };

	HRESULT CreateStructureBuffer();
	HRESULT CreateBuffer_SRV_UAV();
	HRESULT CreateBuffer_Constant();
	HRESULT CreateAndCopyBuffer();
	HRESULT StartPositionCopyBuffer(_float4 _vStartPos);
	HRESULT CameraCopyBuffer();

public:
	static CVIBuffer_Point_Compute* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CComponent* Clone(void* _pArg) override;
	virtual void Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CShader_Compute;
class CVIBuffer_Point_Compute;
class CTexture;
END

BEGIN(Client)

class CEffect_Particle final : public CEffect
{
public:
	struct EFFECT_PARTICLE_DESC : public CEffect::EFFECT_DESC
	{
		wstring szShaderName;
		_uint iParticle_Count = {};
		_uint iShaderPass = {};
		_float3 vRGB = { 1.f, 1.f, 1.f };
		_float3 vScale = {};
		_float3 vRot = {};
		_float3 vTranslation = {};
		_float fAlpha_Amount = { 0.1f };

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
		_float2		vDelayTime = _float2(0.f, 0.f);
	};

private:
	CEffect_Particle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_Particle(const CEffect_Particle& _Prototype);
	virtual ~CEffect_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* _pArg) override;
	virtual void Priority_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Late_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_WeightBlend() override;
	virtual HRESULT Render_Bloom() override;


	virtual void Set_IsPlaying(_bool _bIsPlaying) override;

private:
	CShader_Compute* m_pShaderCom = { nullptr };
	CVIBuffer_Point_Compute* m_pBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_uint m_iParticle_Count = {};
	_uint m_iDiffuse = {}; //Texture 색깔 뭔지

	_uint m_iShaderPass = {};

	_float3 m_vRGB = {};

	_float m_fAlpha_Amount = { 0.1f };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CEffect_Particle* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END
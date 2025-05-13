#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CWater final : public CGameObject
{
public:
	typedef struct WaterInfo : public CGameObject::GAMEOBJECT_DESC
	{
		_float4 vWaterInfo = { 30.0f, 10.0f, -150.0f, 1.0f };
		_float fDullBlendFactor = { 1.f };
		_float fWaveFrequency = { 0.05f };
	}WATERINFO;

private:
	CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWater(const CWater& Prototype);
	virtual ~CWater() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Reflection() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pRippleTextureCom = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };

private:
	_float							m_fAspect = { 0.f };

	_float							m_fTime = { 0.f };
	_float							m_fWaveSpeed = { 0.05f };
	_float							m_fWaveFrequency = { 0.05f };
	_float							m_fWaveAmplitude = { 0.3f };

	_float							m_fWaveHeight = { 0.5f };

	_float4x4						m_matReflectionView = { };

	_float2							m_fWindDirection = { 0.5f, 0.5f };
	_float							m_fWindDirDebug[2] = { 0.5f, 0.5f };

	_int							m_iFresnelMode = { 0 };
	_float							m_iDrawMode = { 0.f };

	_float							m_fDullBlendFactor = { 1.f };

	_float							m_fSpecPerturb = { 4.f };
	_float							m_fSpecPower = { 150.f };

public:
	HRESULT Ready_Components(void* _pArg);
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Reflection();
public:
	static CWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
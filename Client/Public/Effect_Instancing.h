#pragma once
#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CEffect_Instancing final : public CEffect
{
public:
	struct EFFECT_INSTANCING_DESC : public CEffect::EFFECT_DESC
	{
		wstring szModelName;
		_float fTimer_SpeedX = {};
		_float fTimer_SpeedY = {};
		_float fDissolve_Speed = {};
		_float fHeightX = {};
		_float fHeightY = {};
		_bool bLoop = { false };	

		_float3 vScale = {};
		_float3 vRot = {};
		_float3 vTranslation = {};
	};
private:
	CEffect_Instancing(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_Instancing(const CEffect_Instancing& _Prototype);
	virtual ~CEffect_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* _pArg) override;
	virtual void Priority_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Late_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Set_IsPlaying(_bool _bIsPlaying) override;

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	_uint m_iDiffuse = {};

	_float m_fHeightX = {};
	_float m_fHeightY = {};

	_bool m_bLoop = { false };

	_float4 m_vExplosionPower[9] = {};

private:
	void Random_Explosion(_float _fMin, _float _fMax);

public:
	static  CEffect_Instancing* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END
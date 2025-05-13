#pragma once
#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CEffect_Mesh final : public CEffect
{
public:
	struct EFFECT_MESH_DESC : public CEffect::EFFECT_DESC
	{
		wstring szModelName;
		_uint iShaderPass = {};
		//_uint iDiffuse = {};
		_uint iNoise = {};
		_uint iMask = {};
		//_float fMaxTimer = {};
		_float fTimer_SpeedX = {};
		_float fTimer_SpeedY = {};
		_float fDissolve_Speed = {};
		_float fWeightX = {};
		_float fWeightY = {};
		_float fStartTexcoordX = {};
		_float fStartTexcoordY = {};
		_float fMaskCountX = {};
		_float fMaskCountY = {};
		_float3 vRGB = {};
		_bool bTexcoordX = { false };
		_bool bTexcoordY = { false };
		_float fTexcoord_LerpX = {};
		_float fTexcoord_LerpY = {};
		_bool bUsing_Noise = { false };
		_bool bLoop = { false };	
		_bool bMinus_X = { false };	
		_bool bMinus_Y = { false };	
		_bool bGray = { false };	
		_uint iNormal = { 0 };

		_float3 vScale = {};
		_float3 vRot = {};
		_float3 vTranslation = {};
	};
private:
	CEffect_Mesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_Mesh(const CEffect_Mesh& _Prototype);
	virtual ~CEffect_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* _pArg) override;
	virtual void Priority_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Late_Update(_float _fTimeDelta) override;
	virtual HRESULT	Render() override;
	virtual HRESULT Render_Distortion() override;
	virtual HRESULT Render_Glow() override;
	virtual HRESULT Render_WeightBlend() override;

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	_uint m_iShaderPass = {};

	_uint m_iDiffuse = {};
	_uint m_iNoise = {};
	_uint m_iMask = {};
	_uint m_iNormal = {};

	_float m_fWeightX = {};
	_float m_fWeightY = {};

	_float m_fStartTexcoordX = {};
	_float m_fStartTexcoordY = {};

	_float m_fMaskCountX = {};
	_float m_fMaskCountY = {};

	_float3 m_vRGB = {};

	_bool m_bTexcoordX = { false };
	_bool m_bTexcoordY = { false };

	_float m_fTexcoord_LerpX = {};
	_float m_fTexcoord_LerpY = {};

	_bool m_bUsing_Noise = { false };
	_bool m_bLoop = { false };
	_bool m_bMinus_X = { false };
	_bool m_bMinus_Y = { false };
	_bool m_bGray = { false };

private:
	//void Timer_Check(_float _fTimeDelta);
	//지금 Model Component 랑 Shader Pass 유동적으로 설정하려고 했음

public:
	static  CEffect_Mesh* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END
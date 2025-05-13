#pragma once
#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CSwordTrail_Buffer;
END

BEGIN(Client)

class CEffect_Sword final : public CEffect
{
public:
	struct EFFECT_SWORD_DESC : public CEffect::EFFECT_DESC
	{
		_uint iShaderPass = {};
		_float fLength_Up = {};
		_float fLength_Right = {};
		_float fParent_Look = {};
		_float3 vRGB = {};

		_float3 vScale = {};
		_float3 vRot = {};
		_float3 vTranslation = {};

		_uint iSword_XYZ = { 2 };
	};

private:
	CEffect_Sword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	CEffect_Sword(const CEffect_Sword& _pPrototype);
	virtual ~CEffect_Sword() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* _Arg) override;

	virtual void Priority_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Late_Update(_float _fTimeDelta) override;

	virtual HRESULT Render() override;
	virtual HRESULT Render_Distortion() override;
	virtual HRESULT Render_WeightBlend() override;

	virtual void Set_IsPlaying(_bool _bIsPlaying) override;

private:
	CShader* m_pShaderCom = { nullptr };
	CSwordTrail_Buffer* m_pBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_uint m_iShaderPass = {};
	_uint m_iDiffuse = {};
	_float m_fLength_Up = {};
	_float m_fLength_Right = {};
	_float m_fParent_Look = {};

	_float3 m_vRGB = {};
	_uint m_iSword_XYZ = { 2 };

	deque<_float3> m_dequeCenterPos;

	_bool m_bisCalculate = { false };

private:
	void Calculate_Trail(_float _fTimeDelta);
	void Delete_Trail();

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

public:
	static CEffect_Sword* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual	CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;

};

END
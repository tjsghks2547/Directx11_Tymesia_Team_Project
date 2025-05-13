#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CBody_GhostSemy final : public CPartObject
{
public:
	struct BODY_Ghost_Aisemy_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState = { nullptr };
	};

private:
	CBody_GhostSemy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CBody_GhostSemy(const CBody_GhostSemy& _Prototype);
	virtual ~CBody_GhostSemy() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* _pArg) override;
	virtual void				Priority_Update(_float _fTimeDelta) override;
	virtual void				Update(_float _fTimeDelta) override;
	virtual void				Late_Update(_float _fTimeDelta) override;
	virtual HRESULT				Render() override;
	virtual HRESULT				Render_Glow() override;

	_bool						Get_AnimationStop() { return m_bStopAnimation; }
	void						Set_AnimationStop(_bool _bFinish) { m_bStopAnimation = _bFinish; }
	void						Activate_SemyBody(_bool _bActivate, _uint _iType = 0);
	_bool						IsActivate() { return m_bActivate; }

	_bool						IsAnimationFinish();
	void						IsAppear(_bool _bReverse) { m_bReverse = _bReverse; };

	void						BindOff();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pNoiseTextureCom = { nullptr };

private:
	const _uint* m_pParentState = { nullptr };
	_uint                       m_iPreAnimationState = {};

private:
	_uint						m_iPassNum = {};
	_bool						m_bStopAnimation = { false };
	_bool						m_bReverse = { false };
	_bool						m_bActivate = { false };
	_float4						m_vGhostColor = _float4(0.2f, 1.0f, 0.7f, 1.0f);
	_float						m_fDissolveAmount = {};

public:
	HRESULT						Ready_Components();
	HRESULT						Bind_ShaderResources();

public:
	static CBody_GhostSemy* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void				Free() override;
};
END

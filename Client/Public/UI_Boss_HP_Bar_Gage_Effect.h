
#pragma once

#include "Client_Defines.h"
#include "UI_Image.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Boss_HP_Bar_Gage_Effect final : public CUI_Image
{
public:
	struct UI_Boss_HP_Bar_Gage_Effect_Desc : CUIObject::UIOBJECT_DESC
	{
		_bool* bBossActive = {};
		_bool* bBossDead = {};
		_float* fMaxHP = {};
		_float* fCurHP = {};
		_float* fShieldHP = {};
	};
private:
	CUI_Boss_HP_Bar_Gage_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Boss_HP_Bar_Gage_Effect(const CUI_Boss_HP_Bar_Gage_Effect& Prototype);
	virtual ~CUI_Boss_HP_Bar_Gage_Effect() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	_float* m_fBoss_Max_HP = {};
	_float* m_fBoss_Cur_HP = {};
	_float* m_fBoss_Shield_HP = {};
	_float m_fBoss_Pre_Shield_HP = {};
	_float m_fBoss_Pre_HP = {};

	_bool* m_bBossActive = {};
	_bool* m_bBossDead = {};
	_bool m_bActive = {};

	_float m_fSizeX = {};
	_float m_fSizeY = {};
	_uint  m_iTest = {};
	_float m_fTimeDelta = {};
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
public:
	HRESULT Ready_Components();

public:
	static CUI_Boss_HP_Bar_Gage_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
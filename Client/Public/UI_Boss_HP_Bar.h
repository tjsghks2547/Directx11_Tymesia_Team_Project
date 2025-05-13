#pragma once

#include "Client_Defines.h"
#include "UI_Image.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Boss_HP_Bar final : public CUI_Image
{
public:
	struct UI_BOSS_HP_BAR_DESC : CUIObject::UIOBJECT_DESC
	{
		_bool* bBoss_HP_Bar_Active = {};
		_bool* bBossDead = {};
		_float* fMaxHP = {};
		_float* fCurHP = {};
		_float* fShieldHP = {};
		_uint* iPhase = {};
		wstring sBossName = {};
	};
private:
	CUI_Boss_HP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Boss_HP_Bar(const CUI_Boss_HP_Bar& Prototype);
	virtual ~CUI_Boss_HP_Bar() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	_float* m_fBoss_Cur_HP = {};
	_float* m_fBoss_Max_HP = {};
	_bool* m_bBoss_HP_Bar_Active = {};
	_bool* m_bBossDead = {};
	wstring m_sBossName = {};

	_float m_fSizeX = {};
	_float m_fSizeY = {};
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
public:
	HRESULT Ready_Components();

public:
	static CUI_Boss_HP_Bar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
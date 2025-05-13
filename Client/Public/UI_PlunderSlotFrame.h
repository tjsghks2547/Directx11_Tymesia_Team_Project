#pragma once

#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_PlunderSlotFrame final : public CUI_Image
{
private:
	CUI_PlunderSlotFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PlunderSlotFrame(const CUI_PlunderSlotFrame& Prototype);
	virtual ~CUI_PlunderSlotFrame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_TexIcon(_uint iNum) { m_iTexicon = iNum; }
	_uint Get_TexIcon() { return  m_iTexicon; }

	void Set_SkillOn(_bool bUse) { m_bSkillOn = bUse; }
	_bool Get_SkillOn() {return  m_bSkillOn; }

	void Set_EffectOn(_bool bUse) { m_bEffectOn = bUse; }
	_bool Get_EffectOn() {return  m_bEffectOn; }

private:
	CShader*				m_pShaderCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTexIconCom = { nullptr };
	CTexture* m_pTexEffectCom = { nullptr };
	CTexture* m_pTexNoiseCom = { nullptr };


private:
	_uint m_iTexicon = {0};
	_uint m_iBeforeIcon = {};
	_uint m_iAfterIcon = {};
	_uint m_iTexEffect = { 1 };

	_bool m_bTexIconON = { false }; // 아이콘 onoff
	_bool m_bSkillOn = { false }; // 스킬 사용
	_bool m_bEffectOn = { false }; // 이펙트를 켜라
	
	_bool m_bIconEffectOpen = { false };
	_bool m_bIconEffectClose = { false };


	_float m_fSkill_CoolTime = {};
	_float m_fEffect_Alpha = {};
	_float m_fIcon_CreativeTime = {};

public:
	HRESULT Ready_Components();

public:
	static CUI_PlunderSlotFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
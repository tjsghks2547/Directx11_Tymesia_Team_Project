#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Skill_Slot final : public CUI_Button
{
public:
	enum TexKind { TEX_SLOT, TEX_ICON, TEX_EDGE, TEX_EFFECT, TEX_END };
private:
	CUI_Skill_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Skill_Slot(const CUI_Skill_Slot& Prototype);
	virtual ~CUI_Skill_Slot() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Slot_State(SkILL_SLOTSTATE eState) { m_eSlotState = eState; }
	SkILL_SLOTSTATE Get_Slot_State() { return m_eSlotState; }

private:
	SkILL_SLOTSTATE m_eSlotState = {}; // ���� ���� �� => UI�׷쿡�� �����ϴ� ��

public:
	void Set_TexSlot(_uint iTexNum) { m_iTexSlot = iTexNum; }
	_uint Get_TexSlot() { return m_iTexSlot; }

	void Set_TexEdge(_uint iTexNum) { m_iTexEdge = iTexNum; }
	_uint Get_TexEdge() { return m_iTexEdge; }

	void Set_TexEffect(_uint iTexNum) { m_iTexEffect = iTexNum; }
	_uint Get_TexEffect() { return m_iTexEffect; }

	void Set_IconChange(_uint iTexNum) { m_iIconChange = iTexNum; }

	void Set_NeedItem(ITEM_TYPE eItemType) { m_eNeedItem = eItemType; }
	ITEM_TYPE Get_NeedItem() { return m_eNeedItem; }
	
	void Set_MySkill(PLAYER_SKILL eSkill) { m_eMySkill = eSkill; }
	PLAYER_SKILL Get_MySkill() { return m_eMySkill; }
private:
	_uint m_iTexSlot = { 0 };
	_uint m_iTexEdge = { 1 };
	_uint m_iTexEffect = { 1 };

	_uint m_iIconChange = { 0 };
	ITEM_TYPE m_eNeedItem = {ITEM_TYPE::ITEM_SKILLPIECE};
	PLAYER_SKILL m_eMySkill = {};
public:
	void Set_TexIconOff(_bool bTexOff) { m_bTexIconOff = bTexOff; }
	_uint Get_TexIconOff() { return m_bTexIconOff; }

	void Set_TexEdgeOff(_bool bTexOff) { m_bTexEdgeOff = bTexOff; }
	_uint Get_TexEdgeOff() { return m_bTexEdgeOff; }

	void Set_TexEffectOff(_bool bTexOff) { m_bTexEffectOff = bTexOff; }
	_uint Get_TexEffectOff() { return m_bTexEffectOff; }

private:
	_bool m_bTexIconOff = { false }; // Ư���� Ȱ��ȭ �Ǿ� ���� �ʴٸ� �̹����� ����ó��
	_bool m_bTexEdgeOff = { false }; // ȣ�� ��¦��¦
	_bool m_bTexEffectOff = { false }; // ����Ʈ �̹��� �׸��� ����
	_bool m_bSoundOnOff = { true };

public:
	void Set_OpenContion(_bool bOpen) { m_bOpenContion = bOpen; }
	void Set_TexIconOpen(_bool bOpen) { m_bTexIconOpen = bOpen; }

private:
	_float m_fCurrentTime = { 0 }; // ȣ�� ��¦ ��¦ ȿ�� �ֱ� ���� �ð� ��
	_bool m_bOpenContion = { false };
	_bool m_bTexIconOpen = { true }; // 

	_bool m_bCurrentSelect = { false };

private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom[TEX_END] = { nullptr };

public:
	HRESULT Ready_Components();

public:
	static CUI_Skill_Slot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
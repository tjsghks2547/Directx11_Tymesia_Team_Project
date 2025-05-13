#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Frame final : public CUI_Button
{
public:
	enum TexKind { TEX_SLOT, TEX_ICON, TEX_EDGE, TEX_EFFECT, TEX_END };
private:
	CUI_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Frame(const CUI_Frame& Prototype);
	virtual ~CUI_Frame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Slot_State(SLOTSTATE eState) { m_eSlotState = eState; }
	SLOTSTATE Get_Slot_State() { return m_eSlotState; }

private:
	SLOTSTATE m_eSlotState = {}; // 슬롯 상태 값 => UI그룹에서 세팅하는 값
	 
public:
	void Set_TexSlot(_uint iTexNum) { m_iTexSlot = iTexNum; }
	_uint Get_TexSlot() { return m_iTexSlot; }

	void Set_TexEdge(_uint iTexNum) { m_iTexEdge = iTexNum; }
	_uint Get_TexEdge() { return m_iTexEdge; }

	void Set_TexEffect(_uint iTexNum) { m_iTexEffect = iTexNum; }
	_uint Get_TexEffect() { return m_iTexEffect; }

private:
	_uint m_iTexSlot = { 1 };
	_uint m_iTexEdge = { 1 };
	_uint m_iTexEffect = { 1 };

public:
	void Set_TexIconOff(_bool bTexOff) { m_bTexIconOff = bTexOff; }
	_uint Get_TexIconOff() { return m_bTexIconOff; }

	void Set_TexEdgeOff(_bool bTexOff) { m_bTexEdgeOff = bTexOff; }
	_uint Get_TexEdgeOff() { return m_bTexEdgeOff; }

	void Set_TexEffectOff(_bool bTexOff) { m_bTexEffectOff = bTexOff; }
	_uint Get_TexEffectOff() { return m_bTexEffectOff; }

private:
	_bool m_bTexIconOff = { false }; // 특성이 활성화 되어 있지 않다면 이미지를 어둡게 처리하기 위한 용도
	_bool m_bTexEdgeOff = { false }; // 호버 반짝반짝
	_bool m_bTexEffectOff = { false }; // 이펙트 이미지 그리기 여부


public:
	void Set_TalentPoint(_int biPoint) {

		if (0 > biPoint - 1)
		{
			m_iNeedTalentPoint = 0;
		}
		else
		{
			m_iNeedTalentPoint = biPoint - 1;
		}
	}
	_int Get_TalentPoint() { return m_iNeedTalentPoint; }

	void Set_Talent_Active(_bool bOpen) { m_bActive = bOpen; }
	_bool Get_Talent_Active() { return m_bActive; }

	_int Get_ConditionID() { return m_iContionID; }
private:
	_float m_fCurrentTime = { 0 }; // 호버 반짝 반짝 효과 주기 위한 시간 값
	_int m_iNeedTalentPoint = {1}; // 이 특성을 찍기 위해서 필요한 특성 포인트
	_bool m_bActive = { false };



	_int m_iContionID = {}; // 내번호의 - 100 인 특성이 활성화 되어야 
public:
	HRESULT Ready_Components();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[TEX_END] = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CUI_Frame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
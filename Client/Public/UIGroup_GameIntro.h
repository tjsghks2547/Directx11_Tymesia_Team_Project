#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CUI_Scene;
//class CUI_Text;
END

BEGIN(Client)

class CUIGroup_GameIntro final : public CUIObject
{
private:
	CUIGroup_GameIntro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_GameIntro(const CUIGroup_GameIntro& Prototype);
	virtual ~CUIGroup_GameIntro() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Button_Check();


public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
private:
	vector<UI_TextInfo> m_TextInfo = {};
	CUI_Scene* m_pMyScene = {};
	_bool bUIChange = { true };

private:
	CUIObject* m_pLogoImage = { nullptr }; // ªË¡¶?
	CUIObject* m_pAnyKeyText = { nullptr };

	CUIObject* m_pButton1 = { nullptr };
	CUIObject* m_pButton2 = { nullptr };
	CUIObject* m_pButton3 = { nullptr };
	CUIObject* m_pButton4 = { nullptr };
	CUIObject* m_pButton5 = { nullptr };
	CUIObject* m_pButton6 = { nullptr };
	
	CUIObject* m_pBefore = { nullptr };
	CUIObject* m_pCurrent = { nullptr };


	_float m_fButtonOpenTime = {};
	_bool m_ButtonOpen = { false };
	_bool m_TextOpen = { true };
	_bool m_OpenComplete = { false };

public:
	static CUIGroup_GameIntro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

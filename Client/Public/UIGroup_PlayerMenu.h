#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CUI_Scene;
END

BEGIN(Client)

class CUIGroup_PlayerMenu final : public CUIObject
{
private:
	CUIGroup_PlayerMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerMenu(const CUIGroup_PlayerMenu& Prototype);
	virtual ~CUIGroup_PlayerMenu() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void MenuButton_Check();
	void ChangePopButton_Check();


public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);

private:
	vector<UI_TextInfo> m_TextInfo = {};
	CUI_Scene* m_pMyScene = {};
	CUI_Scene* m_pSceneChangePop = {};

	_bool		m_bChangePopOpen = { false };
	LEVELID		m_eMyLevelID = {};

	CUIObject* m_pMapImage = {};
	CUIObject* m_pMapName = {};

public:
	static CUIGroup_PlayerMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
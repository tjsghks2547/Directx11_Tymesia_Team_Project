#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CUI_Scene;
END

BEGIN(Client)

class CUIGroup_Landing final : public CUIObject
{
private:
	CUIGroup_Landing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_Landing(const CUIGroup_Landing& Prototype);
	virtual ~CUIGroup_Landing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Map_Name();

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadData_UIText_Info(const _tchar* szSceneName);
private:
	vector<UI_TextInfo> m_TextInfo = {};

	CUI_Scene* m_pMessage_Dead = {};
	CUI_Scene* m_pMessage_Beacon = {};
	CUI_Scene* m_pMessage_Recall = {};
	CUI_Scene* m_pMessage_Memories = {};
	CUI_Scene* m_pMessage_MapName = {};

	_float m_fRandingTime = {};
	_float m_fDeadTime = {};
	LEVELID m_eMyLevel = {};

public:
	static CUIGroup_Landing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CUI_Scene;
END

BEGIN(Client)

class CUIGroup_Dialogue final : public CUIObject
{
private:
	CUIGroup_Dialogue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_Dialogue(const CUIGroup_Dialogue& Prototype);
	virtual ~CUIGroup_Dialogue() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void AIsemy_Pop_Button();
	void AIsemy_Pop_Boss_Button();

private:
	vector<UI_TextInfo> m_BossTextInfo = {};

	CUI_Scene* m_pTalkScene = {};
	CUI_Scene* m_pPopScene = {};

	CUI_Scene* m_pTalkScene_Boss = {}; // 철학자 세미
	CUI_Scene* m_pPopScene_Boss = {}; // 고스트 세미

	_float m_fDelayTime = {};
	_float m_fTextTime = {};
	CUIObject* m_pLastText[3] = { nullptr };

public:
	//void Boss_Talk_Pop(UIBOSSTALK eBoss);

private:
	CUI_Scene* m_pBossTalk = {};
	CUIObject* m_pBossTextBox = {};

	_float m_fBossTalkTime = {};

private:
	LEVELID m_eMyLevel = {};
private:
	CUI_Scene* m_pEndingTalk = {};
	CUI_Scene* m_pEndingImage = {};


public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadData_UIText_Info(const _tchar* szSceneName);

public:
	static CUIGroup_Dialogue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

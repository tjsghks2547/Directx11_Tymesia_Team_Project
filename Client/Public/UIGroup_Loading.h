#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CUI_Scene;
END

BEGIN(Client)

class CUIGroup_Loading final : public CUIObject
{
private:
	CUIGroup_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_Loading(const CUIGroup_Loading& Prototype);
	virtual ~CUIGroup_Loading() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Texture_Num_Check();


public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadData_UIText_Info(const _tchar* szSceneName);
private:
	vector<UI_TextInfo> m_TextInfo = {};
	CUI_Scene* m_pMyScene = {};
	LEVELID m_eLevelID = {};

public:
	static CUIGroup_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

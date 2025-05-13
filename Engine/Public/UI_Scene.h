#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CUIObject;

class CUI_Scene final : public CBase
{
private:
	CUI_Scene();
	virtual ~CUI_Scene() = default;

public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

public: // �׷����ٴ� �� Update�� ���� ������ üũ �ϴ� ��
	HRESULT Add_UIObject_Type(_int iUIType, CGameObject* pUIObj); // �߰��ϴ� UIObj Ÿ�Կ� ���� �����ϴ� �����̳� ����
	HRESULT UIScene_UIObject_Render_OnOff(_bool bOpen); // ���� ��� UIObj �׸��⸦ ����
	_bool Get_Scene_Render_State() { return m_SceneRender; } // ���� �׷����� �ִ��� Ȯ�� 
	HRESULT Set_All_UIObject_Condition_Open(_bool bOpen); // ���� ��� UIObj �׷����� ������ ȭ�鿡 ����Ű�� ����

	void Clear_Last(_uint iUIType);; // �������� �߰��� ��ü ����
	void Clear_Choice(_uint iUIType, CUIObject* pUIObj); // �������� �߰��� ��ü ����


	void Find_TextBox_PlayerScreen(_uint iGroupID, _uint iCount); // ���� ��� �� ��ġ �������� �κ��� ����...�ϴ� �߰� 

	vector<class CUI_Button*> Find_UI_Button() { return m_Button; }
	vector<class CUI_Image*> Find_UI_Image() { return m_Image; }

	vector<class CUI_Text*> Find_UI_TextBox() { return m_TextBox; }

	void Set_Condition(_int iCondition1, _int iCondition2) { m_iCondition[0] = iCondition1, m_iCondition[1] = iCondition2; }
	_int Get_Condition(_uint iConditionNum ) { return m_iCondition[iConditionNum]; }

private:
	vector<class CUI_Button*>			m_Button;
	vector<class CUI_Image*>			m_Image;
	vector<class CUI_Text*>				m_TextBox;
private:
	_bool								m_SceneRender = { false }; // �⺻ �� : ���� ���� �׷����� �ʰ� �ִ�
	_int                                m_iCondition[2] = {}; // �ν��Ͻ��� ���� �� �ְ� ���� ��� �뵵�� ���


public:
	static CUI_Scene* Create();
	virtual void Free() override;
};

END
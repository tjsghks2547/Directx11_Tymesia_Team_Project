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

public: // 그려진다는 건 Update를 할지 말지를 체크 하는 것
	HRESULT Add_UIObject_Type(_int iUIType, CGameObject* pUIObj); // 추가하는 UIObj 타입에 따라 저장하는 컨테이너 설정
	HRESULT UIScene_UIObject_Render_OnOff(_bool bOpen); // 씬에 모든 UIObj 그리기를 끄기
	_bool Get_Scene_Render_State() { return m_SceneRender; } // 씬이 그려지고 있는지 확인 
	HRESULT Set_All_UIObject_Condition_Open(_bool bOpen); // 씬에 모든 UIObj 그려지고 있지만 화면에 끄고키는 설정

	void Clear_Last(_uint iUIType);; // 마지막에 추가한 객체 삭제
	void Clear_Choice(_uint iUIType, CUIObject* pUIObj); // 마지막에 추가한 객체 삭제


	void Find_TextBox_PlayerScreen(_uint iGroupID, _uint iCount); // 몬스터 사망 시 수치 가져오는 부분을 위해...일단 추가 

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
	_bool								m_SceneRender = { false }; // 기본 값 : 현재 씬은 그려지지 않고 있다
	_int                                m_iCondition[2] = {}; // 인스턴스를 통해 값 주고 싶은 경우 용도로 사용


public:
	static CUI_Scene* Create();
	virtual void Free() override;
};

END
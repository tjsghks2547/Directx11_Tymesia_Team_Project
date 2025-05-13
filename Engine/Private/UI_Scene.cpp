#include "UI_Scene.h"

#include "GameObject.h"

#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_Text.h"


CUI_Scene::CUI_Scene()
{
}

void CUI_Scene::Priority_Update(_float fTimeDelta)
{
	if (m_SceneRender)
	{
		for (auto& Image : m_Image)
			Image->Priority_Update(fTimeDelta);

		for (auto& Button : m_Button)
			Button->Priority_Update(fTimeDelta);

		for (auto& Text : m_TextBox)
			Text->Priority_Update(fTimeDelta);
	}
}

void CUI_Scene::Update(_float fTimeDelta)
{

	if (m_SceneRender)
	{
		for (auto& Image : m_Image)
			Image->Update(fTimeDelta);

		for (auto& Button : m_Button)
			Button->Update(fTimeDelta);

		for (auto& Text : m_TextBox)
			Text->Update(fTimeDelta);
	}
}

void CUI_Scene::Late_Update(_float fTimeDelta)
{
	if (m_SceneRender)
	{
		for (auto& Image : m_Image)
			Image->Late_Update(fTimeDelta);

		for (auto& Button : m_Button)
			Button->Late_Update(fTimeDelta);

		for (auto& Text : m_TextBox)
			Text->Late_Update(fTimeDelta);
	}
}

HRESULT CUI_Scene::Add_UIObject_Type(_int iUIType, CGameObject* pUIObj)
{
	if (nullptr == pUIObj)
		return E_FAIL;

	switch (iUIType)
	{
	case UI_BUTTON:
		m_Button.push_back(dynamic_cast<CUI_Button*>(pUIObj));
		break;
	case UI_IMAGE:
		m_Image.push_back(dynamic_cast<CUI_Image*>(pUIObj));
		break;
	case UI_TEXT:
		m_TextBox.push_back(dynamic_cast<CUI_Text*>(pUIObj));
		break;
	}

	return S_OK;
}

HRESULT CUI_Scene::UIScene_UIObject_Render_OnOff(_bool bOpen)
{
	for (auto& Image : m_Image)
		Image->Set_Render_OnOff(bOpen);

	for (auto& Button : m_Button)
		Button->Set_Render_OnOff(bOpen);

	for (auto& Text : m_TextBox)
		Text->Set_Render_OnOff(bOpen);

	if (bOpen) // ���� ��µǴ� ��� UI�� ���� ��� ������Ʈ�� ���� �Ѵ�
		m_SceneRender = true;
	else
		m_SceneRender = false;

	return S_OK;
}

HRESULT CUI_Scene::Set_All_UIObject_Condition_Open(_bool bOpen)
{
	// �� ���� ��� ������Ʈ�� => A ���� ���ǿ� B�� ������ ����� ui => Open ���� �߰�
// ���ʿ� ��� Ų ���·� �����ص� ���¿��� => Ŭ�󿡼� ���ǿ� ���� ���ʿ��� ���� �ֵ��� ����

	for (auto& Image : m_Image)
		Image->Set_OnOff(bOpen);

	for (auto& Button : m_Button)
		Button->Set_OnOff(bOpen);

	for (auto& Text : m_TextBox)
		Text->Set_OnOff(bOpen);

	return S_OK;
}

void CUI_Scene::Clear_Last(_uint iUIType)
{
	switch (iUIType)
	{
	case UI_BUTTON:
		if (!m_Button.empty())
		{
			Safe_Release(m_Button.back());
			m_Button.pop_back();
		}
		break;
	case UI_IMAGE:
		if (!m_Image.empty())
		{
			Safe_Release(m_Image.back());
			m_Image.pop_back();
		}
		break;
	case UI_TEXT:
		if (!m_TextBox.empty())
		{
			Safe_Release(m_TextBox.back());
			m_TextBox.pop_back();
		}
		break;
	}
}

void CUI_Scene::Clear_Choice(_uint iUIType, CUIObject* pUIObj)
{
	switch (iUIType)
	{
	case UI_BUTTON:
		if (!m_Button.empty())
			m_Button.erase(remove(m_Button.begin(), m_Button.end(), pUIObj), m_Button.end());
		break;
	case UI_IMAGE:
		if (!m_Image.empty())
			m_Image.erase(remove(m_Image.begin(), m_Image.end(), pUIObj), m_Image.end());
		break;
	case UI_TEXT:
		if (!m_TextBox.empty())
			m_TextBox.erase(remove(m_TextBox.begin(), m_TextBox.end(), pUIObj), m_TextBox.end());
		break;
	}
}

void CUI_Scene::Find_TextBox_PlayerScreen(_uint iGroupID,_uint iCount)
{
	// ����, �÷��̾�Լ� ���ϴ� Ÿ�ֿ̹� ���ϴ� ��ġ���� �������� ���ؼ�...�̷� �Լ��� ����...
	_tchar pDust[MAX_PATH] = {};
	_tchar* pSor = { L"+ %d" };

	wsprintf(pDust, pSor, iCount);
	for (auto& TextBox : m_TextBox)
	{
		if (iGroupID == TextBox->Get_UI_GroupID())
		{
			TextBox->Set_OnOff(true);
			TextBox->Set_TextDrawType(TEXT_ALPHA_ANIM);
			TextBox->Set_Change_TextColor(FONT_GREEN);
			TextBox->Set_Content(pDust);
		}
	}

}

CUI_Scene* CUI_Scene::Create()
{
	return new CUI_Scene();
}

void CUI_Scene::Free()
{
	__super::Free();

	for (auto& pUIObj : m_Button)
		Safe_Release(pUIObj);
	for (auto& pUIObj : m_Image)
		Safe_Release(pUIObj);
	for (auto& pUIObj : m_TextBox)
		Safe_Release(pUIObj);
	
	m_Button.clear();
	m_Image.clear();
	m_TextBox.clear();
}

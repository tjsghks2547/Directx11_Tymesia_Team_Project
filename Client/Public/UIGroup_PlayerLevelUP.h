#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CUI_Scene;
END

BEGIN(Client)

class CUIGroup_PlayerLevelUP final : public CUIObject
{
private:
	CUIGroup_PlayerLevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerLevelUP(const CUIGroup_PlayerLevelUP& Prototype);
	virtual ~CUIGroup_PlayerLevelUP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Button_Input_Check(); // ��ư ���� �� �߻� �̺�Ʈ
	HRESULT Button_Render_Check(); // ��ư�� �׷����� ����
	HRESULT Button_Image_On_Check(); // ���� - � ������ �޶����� �� �����ִ� �κ� 
	HRESULT Button_Image_OnOff_Check(_bool bPower, _bool bVitality, _bool bPlaque); // ���� - � ������ �޶����� �� �����ִ� �κ� 
	HRESULT LevelUP_Memory_Check();
	HRESULT LevelUP_Status_Check();
	HRESULT LevelUP_Status_Info_Check();
	HRESULT LevelUP_TalentPoint_Check();

public:

	HRESULT LevelUP_Apply_Button(); // ���� �˾����� ��ư
	HRESULT LevelUP_Reset_Button(); // ���� �˾����� ��ư

	HRESULT LevelUP_Apply(); // ����
	HRESULT LevelUP_Reset(); // ����

private:
	_bool m_bApplyOn = false;
	_bool m_bResetOn = false;
	_bool m_bEscapeCheck = false;


private:
	// ����
	_uint m_iCurrentLevel = {};
	_uint m_iNextLevel = {};

	// ��� ����
	_int m_iMemoryCurrentCount = {}; // ������ ���� �ִ� ����� ���� ����
	_int m_iMemoryNextCount = {}; // �����ؼ� ������ => ������ ���� �� ���� ����� ���� ����

	_int m_iCurrentMemoryNeed = {}; // ���� ���� ���� �ʿ��� ����� ���� ���� �޾� ��
	_int m_iNextMemoryNeed = {}; // ���� ���� ���� �ʿ��� ����� ���� ���� �޾� ��

	_int m_iMemoryTotalUse = { 0 }; //���ݱ��� �÷��̾ �������� ����� ��� ���� ������ ������


	// ĳ���� �ɷ�ġ ����
	_uint m_iCurrentPower = {}; // �� ��ġ
	_uint m_iNextPower = {}; // �� ��ġ

	_uint m_iCurrentVitality = {}; // Ȱ�� ��ġ
	_uint m_iNextVitality = {}; // Ȱ�� ��ġ

	_uint m_iCurrentPlague = {}; // ���� ��ġ
	_uint m_iNextPlague = {}; // ���� ��ġ

	// ĳ���� ���� ����
	_uint		m_iCurrentAttackPower = {}; // ���� ����� 
	_uint		m_iNextAttackPower = {}; // ���� �����

	_uint		m_iCurrentAmountOfWounds = {}; // ��ó ��
	_uint		m_iNextAmountOfWounds = {}; // ��ó ��

	_uint		m_iCurrentClawAttackPower = {}; // ��������
	_uint		m_iNextClawAttackPower = {}; // ��������

	_uint		m_iCurrentFullHp = {}; // ü��
	_uint		m_iNextFullHp = {}; // ü��

	_uint		m_iCurrentFullMp = {}; // ������
	_uint		m_iNextFullMp = {}; // ������

	// �÷��̾� ��ų�� ���ݷ� ��� ������ ���� ��
	_float     m_fCurrentBonus_SkillDamage = {};
	_float     m_fNextBonus_SkillDamage = {};

	// Ư�� ����
	_uint       m_iCurrentTalentPoint = {};
	_uint       m_iNextTalentPoint = {};


public:
	void Set_TalentPoint_ALlUnspent(_int iPoint) { m_iCurrentUnspent = iPoint, m_iNextUnspent = iPoint; }
	_int Get_TalentPoint_CurrentUnspent() {return  m_iCurrentUnspent; }

private:
	_int       m_iCurrentUnspent = {};
	_int       m_iNextUnspent = {};

	LEVELID		m_eMyLevelID = {};

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	void Setting_Default_Status_Value(); // ����, ���߿� �÷��̾ �ʱ�ȭ �ϴ� ��� �����Ǵ� ��

private:
	CUI_Scene* m_pMyScene = {}; //���� �� ui �׷�
	class CGameObject* m_pPlayer = { nullptr };


	CUI_Scene* m_pApplyPopUp = {}; // 1 �� �˾� ����
	CUI_Scene* m_pResetPopUp = {}; // 2 �� �˾� �ʱ�ȭ(���� �ʱ�ȭ�� �ƴ� ������ ���Ƿ� �ø� ��ġ ������ ����)

private:
	vector<UI_TextInfo> m_TextInfo = {};

public:
	static CUIGroup_PlayerLevelUP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
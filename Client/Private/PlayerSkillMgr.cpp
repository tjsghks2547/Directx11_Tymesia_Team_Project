#include "pch.h"    
#include "PlayerSkillMgr.h"

#include "SkillAxe.h"
#include "SkillCane.h"
#include "SkillGreadSword.h"
#include "SkillHalberd.h"
#include "SkillJavelinSword.h"
#include "SkillScythe.h"

CPlayerSkillMgr::CPlayerSkillMgr()
{
}

HRESULT CPlayerSkillMgr::Initialize()
{
    /* ��ų �߰� �� ��*/
    m_vecPlayerSkillState.reserve(20); // ���� �ּҰ� ������ �ʰ� �ϱ� ���� ���� 



    /* 0�� ���� ��ų ���� */
    CSkillAxe* pSkillAxeState = CSkillAxe::Create();
    m_vecPlayerSkillState.push_back(pSkillAxeState);

    /* 1�� ������ ������ ��ų ���� */
    CSkillCane* pSkillCaneState = CSkillCane::Create();
    m_vecPlayerSkillState.push_back(pSkillCaneState);

    /* 2�� �˺� ��� ��ų ���� */
    CSkillGreadSword* pSkillGreadSword = CSkillGreadSword::Create();
    m_vecPlayerSkillState.push_back(pSkillGreadSword);

    /* 3�� �ҹ��� ��ų ���� */
    CSkillHalberd* pSkillHalberd = CSkillHalberd::Create();
    m_vecPlayerSkillState.push_back(pSkillHalberd);

    /* 4�� �ں��� ��ų ���� */
    CSkillJavelinSword* pSkillJavelinSword = CSkillJavelinSword::Create();
    m_vecPlayerSkillState.push_back(pSkillJavelinSword);

    /* 5�� �� ��ų ���� */
    CSkillScythe* pSkillScythe = CSkillScythe::Create();
    m_vecPlayerSkillState.push_back(pSkillScythe);


    return S_OK;
}

_bool CPlayerSkillMgr::Priority_Update(CGameObject* pGameObject, _float fTimeDelta)
{
    return false;
}

HRESULT CPlayerSkillMgr::Update_Skill_CoolTime(CGameObject* pGameObject, _float fTimeDelta)
{
    for (auto& iter : m_vecPlayerSkillState)
        iter->Update(pGameObject, fTimeDelta);

    return S_OK;
}

void CPlayerSkillMgr::Late_Update(CGameObject* pGameObject, _float fTimeDelta)
{
}

HRESULT CPlayerSkillMgr::UnLockSkill(PLAYER_SKILL _eSkill)
{
    for (auto& iter : m_vecPlayerSkillState)
    {
        if (iter->Get_Skill_Name() == _eSkill)
        {
            iter->Set_Skill_UnLock(true);
            return S_OK;
        }
    }

    MSG_BOX("�ش� ��ų�� ã�� �� ��� �ر��� ���ؿ�.");

    return S_OK;
}

Skill_Information* CPlayerSkillMgr::Bring_Skill_information(PLAYER_SKILL _eSkill)
{
    for (auto& iter : m_vecPlayerSkillState)
    {
        if (iter->Get_Skill_Name() == _eSkill)
        {
            return iter->Get_Skill_Information();
        }
    }

    return nullptr;
}


CPlayerSkillMgr* CPlayerSkillMgr::Create()
{
    CPlayerSkillMgr* pInstance = new CPlayerSkillMgr();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : PlayerSkillMgr");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayerSkillMgr::Free()
{
    __super::Free();

    for (auto& iter : m_vecPlayerSkillState)
    {
        Safe_Release(iter);
    }

    m_vecPlayerSkillState.clear();
}

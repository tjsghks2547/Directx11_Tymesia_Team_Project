#include "pch.h"    
#include "SkillScythe.h"

CSkillScythe::CSkillScythe()
{
}

HRESULT CSkillScythe::Initialize()
{
    m_pSkill_information = new Skill_Information();


    /* ��ų ���� ���� ���� �ϱ� */

    //m_fSkillCoolTime = { 3.f };   
    //m_fSkillCurrentCoolTime;  
    //m_bSkill_Unlocked;    
    //m_bSkill_CanUse;  
    m_eSkill_Name = PLAYER_SKILL::PLAYER_SKILL_SCYTHE;
    /* ============================ */

    m_pSkill_information->eSkill_Name = &m_eSkill_Name;
    m_pSkill_information->fSkillCoolTime = &m_fSkill_CoolTime;
    m_pSkill_information->fSkillCurrentCoolTime = &m_fSkill_CurrentCoolTime;
    m_pSkill_information->bSkill_Unlocked = &m_bSkill_Unlocked;
    m_pSkill_information->bSkill_CanUse = &m_bSkill_CanUse;


    return S_OK;
}

_bool CSkillScythe::Priority_Update(CGameObject* pGameObject, _float fTimeDelta)
{
    if (m_bSkill_Unlocked && !m_bSkill_CanUse) //  ��ų�� �رݵǾ��� ����� �Ұ����� �� ��Ÿ�� �����̹Ƿ� ��Ÿ�� ������� �ڵ�   
    {
        m_fSkill_CurrentCoolTime += fTimeDelta;

        if (m_fSkill_CurrentCoolTime > m_fSkill_CoolTime)
        {
            m_fSkill_CurrentCoolTime = 0.f;
            m_bSkill_CanUse = true;

            return true;
        }
    }

    else if (m_bSkill_Unlocked && m_bSkill_CanUse)
    {
        m_bSkill_CanUse = false;
        return true;
    }
    return false;
}

void CSkillScythe::Update(CGameObject* pGameObject, _float fTimeDelta)
{
    if (m_bSkill_Unlocked && !m_bSkill_CanUse)
    {
        m_fSkill_CurrentCoolTime += fTimeDelta;

        m_bSkill_CanUse = false;

        if (m_fSkill_CurrentCoolTime > m_fSkill_CoolTime)
        {
            m_fSkill_CurrentCoolTime = 0.f;
            m_bSkill_CanUse = true;
        }
    }
}

void CSkillScythe::Late_Update(CGameObject* pGameObject, _float fTimeDelta)
{
}

CSkillScythe* CSkillScythe::Create()
{
    CSkillScythe* pInstance = new CSkillScythe();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : SkillAxe");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkillScythe::Free()
{
    __super::Free();




}

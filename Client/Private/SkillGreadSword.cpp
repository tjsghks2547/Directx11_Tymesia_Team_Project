#include "pch.h"    
#include "SkillGreadSword.h"

CSkillGreadSword::CSkillGreadSword()
{
}

HRESULT CSkillGreadSword::Initialize()
{
    m_pSkill_information = new Skill_Information();


    /* 스킬 관련 세부 설정 하기 */

    //m_fSkillCoolTime = { 3.f };
    //m_fSkillCurrentCoolTime;
    //m_bSkill_Unlocked;
    //m_bSkill_CanUse;
    m_eSkill_Name = PLAYER_SKILL::PLAYER_SKILL_GREADSWORD;
    /* ============================ */

    m_pSkill_information->eSkill_Name = &m_eSkill_Name;
    m_pSkill_information->fSkillCoolTime = &m_fSkill_CoolTime;
    m_pSkill_information->fSkillCurrentCoolTime = &m_fSkill_CurrentCoolTime;
    m_pSkill_information->bSkill_Unlocked = &m_bSkill_Unlocked;
    m_pSkill_information->bSkill_CanUse = &m_bSkill_CanUse;


    return S_OK;
}

_bool CSkillGreadSword::Priority_Update(CGameObject* pGameObject, _float fTimeDelta)
{
    if (m_bSkill_Unlocked && !m_bSkill_CanUse) //  스킬이 해금되었고 사용이 불가능할 때 쿨타임 상태이므로 쿨타임 돌리기는 코드   
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

void CSkillGreadSword::Update(CGameObject* pGameObject, _float fTimeDelta)
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

void CSkillGreadSword::Late_Update(CGameObject* pGameObject, _float fTimeDelta)
{
}

CSkillGreadSword* CSkillGreadSword::Create()
{
    CSkillGreadSword* pInstance = new CSkillGreadSword();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : SkillAxe");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkillGreadSword::Free()
{
    __super::Free();

}

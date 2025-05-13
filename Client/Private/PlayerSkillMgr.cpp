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
    /* 스킬 추가 할 곳*/
    m_vecPlayerSkillState.reserve(20); // 벡터 주소가 변하지 않게 하기 위해 설정 



    /* 0번 도끼 스킬 상태 */
    CSkillAxe* pSkillAxeState = CSkillAxe::Create();
    m_vecPlayerSkillState.push_back(pSkillAxeState);

    /* 1번 매지션 지팡이 스킬 상태 */
    CSkillCane* pSkillCaneState = CSkillCane::Create();
    m_vecPlayerSkillState.push_back(pSkillCaneState);

    /* 2번 검병 대검 스킬 상태 */
    CSkillGreadSword* pSkillGreadSword = CSkillGreadSword::Create();
    m_vecPlayerSkillState.push_back(pSkillGreadSword);

    /* 3번 할버드 스킬 상태 */
    CSkillHalberd* pSkillHalberd = CSkillHalberd::Create();
    m_vecPlayerSkillState.push_back(pSkillHalberd);

    /* 4번 자벨린 스킬 상태 */
    CSkillJavelinSword* pSkillJavelinSword = CSkillJavelinSword::Create();
    m_vecPlayerSkillState.push_back(pSkillJavelinSword);

    /* 5번 낫 스킬 상태 */
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

    MSG_BOX("해당 스킬을 찾을 수 없어서 해금을 못해요.");

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

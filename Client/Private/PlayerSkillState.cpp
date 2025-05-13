#include "pch.h"
#include "PlayerSkillState.h"
#include "GameObject.h"


CPlayerSkillState::CPlayerSkillState()
{

}

HRESULT CPlayerSkillState::Initialize()
{
    return S_OK;
}

_bool CPlayerSkillState::Priority_Update(CGameObject* pGameObject, _float fTimeDelta)
{

    return false;

}

void CPlayerSkillState::Update(CGameObject* pGameObject, _float fTimeDelta)
{


}

void CPlayerSkillState::Late_Update(CGameObject* pGameObject, _float fTimeDelta)
{


}

void CPlayerSkillState::Free()
{
    __super::Free();

    delete m_pSkill_information;
    m_pSkill_information = nullptr; //  dangling pointer ¹æÁö     

}

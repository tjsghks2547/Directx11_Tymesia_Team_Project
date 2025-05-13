#include "pch.h"
#include "State.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CState::CState()
    :m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CState::Initialize()
{
    return S_OK;
}

void CState::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CState::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CState::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CState::Free()
{
    __super::Free();
    Safe_Release(m_pGameInstance);
}

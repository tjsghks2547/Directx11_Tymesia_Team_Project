#include "pch.h"
#include "ArchiveGetUp.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CArchiveGetUp::CArchiveGetUp()
{
}

HRESULT CArchiveGetUp::Initialize()
{
	return S_OK;
}

void CArchiveGetUp::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CArchiveGetUp::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CArchiveGetUp::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CArchiveGetUp* CArchiveGetUp::Create()
{
	CArchiveGetUp* pInstance = new CArchiveGetUp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ArchiveGetUp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArchiveGetUp::Free()
{
	__super::Free();
}

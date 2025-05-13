#include "pch.h"
#include "Varg_Catched.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CVarg_Catched::CVarg_Catched()
{
}

HRESULT CVarg_Catched::Initialize()
{
	return S_OK;
}

void CVarg_Catched::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir)) * -1.f;
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);


	float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, MonsterDir));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, MonsterDir);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f)
	{
		Radian = -Radian;
	}

	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
	pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, MonsterPos);	


}

void CVarg_Catched::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CVarg_Catched::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CVarg_Catched* CVarg_Catched::Create()
{
	CVarg_Catched* pInstance = new CVarg_Catched();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CVarg_Catched");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVarg_Catched::Free()
{
	__super::Free();
}

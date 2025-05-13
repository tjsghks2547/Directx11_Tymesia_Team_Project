#include "pch.h"
#include "PlayerRun.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CPlayerRun::CPlayerRun()
{
}

HRESULT CPlayerRun::Initialize()
{
	return S_OK;
}

void CPlayerRun::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	_vector vLookFront = XMVector3Normalize(XMVector3Cross(CamRight, vUp));
	_vector vLookBack = vLookFront * -1.f;
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));


	/* 키가 두개가 동시에 눌려져 있을 때 */
	if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A)
		|| m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D)
		|| m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A)
		|| m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D)
		|| m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyEnter(DIK_D)
		|| m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyEnter(DIK_A)
		|| m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyEnter(DIK_D)
		|| m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyEnter(DIK_A))
	{

		if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
		{
			_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(-45.f));
			_vector vLeftLook45Degree = XMVector3Transform(vLookFront, rotationMartix);


			float dotResult = XMVectorGetX(XMVector3Dot(vLeftLook45Degree, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLeftLook45Degree);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);
		}


		else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
		{
			_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(45.f));
			_vector vRightLook45Degree = XMVector3Transform(vLookFront, rotationMartix);


			float dotResult = XMVectorGetX(XMVector3Dot(vRightLook45Degree, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vRightLook45Degree);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);
		}


		else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
		{
			_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(45.f));
			_vector vLeftLook45Degree = XMVector3Transform(vLookBack, rotationMartix);


			float dotResult = XMVectorGetX(XMVector3Dot(vLeftLook45Degree, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLeftLook45Degree);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);
		}


		else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
		{
			_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(-45.f));
			_vector vRightLook45Degree = XMVector3Transform(vLookBack, rotationMartix);


			float dotResult = XMVectorGetX(XMVector3Dot(vRightLook45Degree, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vRightLook45Degree);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);
		}


	}

	else
	{
		if (m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
		{
			float dotResult = XMVectorGetX(XMVector3Dot(vLookFront, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLookFront);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);


			if (abs(Radian) <= 0.8f)
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);

		}


		else if (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
		{
			float dotResult = XMVectorGetX(XMVector3Dot(vLookBack, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLookBack);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);

			if (abs(Radian) <= 0.8f)
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);


		}


		else if (m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A))
		{
			_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(-90.f));
			_vector vLeftLook90Degree = XMVector3Transform(vLookFront, rotationMartix);

			float dotResult = XMVectorGetX(XMVector3Dot(vLeftLook90Degree, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLeftLook90Degree);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);

			if (abs(Radian) <= 0.8f)
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);


		}


		else if (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D))
		{

			_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(90.f));
			_vector vRightLook90Degree = XMVector3Transform(vLookFront, rotationMartix);

			float dotResult = XMVectorGetX(XMVector3Dot(vRightLook90Degree, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vRightLook90Degree);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);

			if (abs(Radian) <= 0.8f)
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);
		}
	}



}

void CPlayerRun::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CPlayerRun::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CPlayerRun* CPlayerRun::Create()
{
	CPlayerRun* pInstance = new CPlayerRun();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : PlayerRun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerRun::Free()
{
	__super::Free();
}

#include "pch.h"
#include "Attack_LButton_1.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Player.h"


CAttack_LButton_1::CAttack_LButton_1()
{
}

HRESULT CAttack_LButton_1::Initialize()
{
	return S_OK;
}

void CAttack_LButton_1::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	_vector vLookFront = XMVector3Normalize(XMVector3Cross(CamRight, vUp));
	_vector vLookBack = XMVector3Normalize(XMVector3Cross(vUp, CamRight));

	// vLook과 캐릭터의 look 의 내적구하기 
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));



	/* 키가 두개가 동시에 눌려져 있을 때 */
	if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A)
		|| m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D)
		|| m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A)
		|| m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
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


			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

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

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);
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

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);
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

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);
		}
	}

	/* 키가 한 개가 눌려져 있을 때 */
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


			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

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


			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

		}


		else if (m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A))
		{

			_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(-90.f));
			_vector vLeftLook90Degree = XMVector3Transform(vLookFront, rotationMartix);


			_vector PlayerLeft = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT)) * -1.f;

			float dotResult = XMVectorGetX(XMVector3Dot(vLeftLook90Degree, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLeftLook90Degree);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

		}


		else if (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D))
		{

			_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(90.f));
			_vector vRightLook90Degree = XMVector3Transform(vLookFront, rotationMartix);


			_vector PlayerLeft = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT)) * -1.f;

			float dotResult = XMVectorGetX(XMVector3Dot(vRightLook90Degree, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vRightLook90Degree);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

		}
	}

}

void CAttack_LButton_1::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CAttack_LButton_1::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CAttack_LButton_1* CAttack_LButton_1::Create()
{
	CAttack_LButton_1* pInstance = new CAttack_LButton_1();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Attack_LButton_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttack_LButton_1::Free()
{
	__super::Free();
}

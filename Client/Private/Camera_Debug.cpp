#include "pch.h" 
#include "Camera_Debug.h"	
#include "GameInstance.h"
#include "Player.h"
#include "Layer.h"

CCamera_Debug::CCamera_Debug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Debug::CCamera_Debug(const CCamera_Debug& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCamera_Debug::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Debug::Initialize(void* pArg)
{
	FREE_CAMERA_DESC* pDesc = static_cast<FREE_CAMERA_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	/* 플레이어가 먼저 레이어에 들어가니깐*/
	/* 플레이어 위치값 가져오면 될듯*/

	_float4 vTestPosition = { 111.80f, 15.51f, -68.2f, 1.f }; // 보스 정문	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vTestPosition));




	return S_OK;
}


void CCamera_Debug::Priority_Update(_float fTimeDelta)
{

	if (m_pGameInstance->isKeyEnter(DIK_F4))
	{
		m_bDebugCameraOnOff = !m_bDebugCameraOnOff;	 // 이러면 프리는 처음 false에서  애는 true로 시작하니 엇갈리게 설정 가능. 
	}


	if (!m_bDebugCameraOnOff)
	{
		if (GetKeyState('W') & 0x8000)
		{
			m_pTransformCom->Go_Straight(fTimeDelta);
		}

		if (GetKeyState('S') & 0x8000)
		{
			m_pTransformCom->Go_Backward(fTimeDelta);
		}

		if (GetKeyState('A') & 0x8000)
		{
			m_pTransformCom->Go_Left(fTimeDelta);
		}

		if (GetKeyState('D') & 0x8000)
		{
			m_pTransformCom->Go_Right(fTimeDelta);
		}

		_long	MouseMove = {};

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fMouseSensor);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * m_fMouseSensor);
		}

		__super::Priority_Update(fTimeDelta);
	}


}

void CCamera_Debug::Update(_float fTimeDelta)
{

}

void CCamera_Debug::Late_Update(_float fTimeDelta)
{


}

HRESULT CCamera_Debug::Render()
{
	return S_OK;
}

CCamera_Debug* CCamera_Debug::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Debug* pInstance = new CCamera_Debug(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CharacterCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Debug::Clone(void* pArg)
{
	CCamera_Debug* pInstance = new CCamera_Debug(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CharacterCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Debug::Free()
{
	__super::Free();

	//Safe_Release(m_pPlayerTransformCom);
}

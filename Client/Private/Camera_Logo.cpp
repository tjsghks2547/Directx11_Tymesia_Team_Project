#include "pch.h" 
#include "Camera_Logo.h"	
#include "GameInstance.h"
#include "Player.h"
#include "Layer.h"

CCamera_Logo::CCamera_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Logo::CCamera_Logo(const CCamera_Logo& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCamera_Logo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Logo::Initialize(void* pArg)
{
	LOGO_CAMERA_DESC* pDesc = static_cast<LOGO_CAMERA_DESC*>(pArg);


	strcpy_s(m_szName, "Camera_Logo");

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}


void CCamera_Logo::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CCamera_Logo::Update(_float fTimeDelta)
{
}

void CCamera_Logo::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Logo::Render()
{
	return S_OK;
}

CCamera_Logo* CCamera_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Logo* pInstance = new CCamera_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CharacterCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Logo::Clone(void* pArg)
{
	CCamera_Logo* pInstance = new CCamera_Logo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CharacterCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Logo::Free()
{
	m_pGameInstance->Delete_Shadow(m_pTransformCom);

	__super::Free();
}
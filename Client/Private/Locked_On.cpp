#include "pch.h"
#include "Locked_On.h"
#include "GameInstance.h"

CLocked_On::CLocked_On(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CLocked_On::CLocked_On(const CLocked_On& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CLocked_On::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CLocked_On::Initialize(void* pArg)
{
	LOCKED_ON_DESC* pDesc = static_cast<LOCKED_ON_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;
	m_bLocked_On = pDesc->bLocked_On_Active;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(_float3(100.f, 100.f, 1.f));

	return S_OK;
}

void CLocked_On::Priority_Update(_float fTimeDelta)
{
	if (!*m_bLocked_On)
		return;

	if (*m_pParentState == MONSTER_STATE::STATE_STUN)
	{
		m_iTextureNum = 1;
		m_iPassNum = 19;
		m_pTransformCom->Scaling(_float3(20.f, 20.f, 1.f));
	}
	else
	{
		m_iPassNum = 18;
		m_iTextureNum = 0;
		m_pTransformCom->Scaling(_float3(100.f, 100.f, 1.f));
	}

}

void CLocked_On::Update(_float fTimeDelta)
{
	if (!*m_bLocked_On)
		return;
	_matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *
		SocketMatrix *
		XMLoadFloat4x4(m_pParentWorldMatrix)
	);
}

void CLocked_On::Late_Update(_float fTimeDelta)
{
	if (!*m_bLocked_On)
		return;

	Bill_Board();

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CLocked_On::Render()
{
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureNum)))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPassNum);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CLocked_On::Bill_Board()
{
	_vector vParentTranslation, vParentRotation, vParentScale;
	XMMatrixDecompose(&vParentScale, &vParentRotation, &vParentTranslation, XMLoadFloat4x4(&m_CombinedWorldMatrix));
	_matrix matWithoutRotation = XMMatrixAffineTransformation(vParentScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), vParentTranslation);

	_vector vRight{}, vUp{}, vLook{};
	_vector vPos = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());
	vLook = matWithoutRotation.r[3] - vPos;

	vLook = XMVector3Normalize(vLook);
	vUp = matWithoutRotation.r[1];

	vRight = XMVector3Cross(vUp, vLook);
	vLook = XMVector3Cross(vRight, vUp);

	_float3 vScale = _float3(XMVectorGetX(XMVector3Length(matWithoutRotation.r[0])), XMVectorGetY(XMVector3Length(matWithoutRotation.r[1])), XMVectorGetZ(XMVector3Length(matWithoutRotation.r[2])));

	matWithoutRotation.r[0] = XMVector3Normalize(vRight) * vScale.x;
	matWithoutRotation.r[1] = XMVector3Normalize(vUp) * vScale.y;
	matWithoutRotation.r[2] = XMVector3Normalize(vLook) * vScale.z;

	XMStoreFloat4x4(&m_CombinedWorldMatrix, matWithoutRotation);
}

HRESULT CLocked_On::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Locked_On"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLocked_On::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CLocked_On* CLocked_On::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLocked_On* pInstance = new CLocked_On(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CLocked_On");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLocked_On::Clone(void* pArg)
{
	CLocked_On* pInstance = new CLocked_On(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CLocked_On");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLocked_On::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

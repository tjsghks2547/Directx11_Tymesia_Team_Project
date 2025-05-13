#include "pch.h"
#include "Effect_Mesh.h"
#include "GameInstance.h"

CEffect_Mesh::CEffect_Mesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CEffect{ _pDevice, _pContext }
{
}

CEffect_Mesh::CEffect_Mesh(const CEffect_Mesh& _Prototype)
    : CEffect(_Prototype)
{
}

HRESULT CEffect_Mesh::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Mesh::Initialize(void* _pArg)
{
	EFFECT_MESH_DESC* pDesc = static_cast<EFFECT_MESH_DESC*>(_pArg);

	m_iShaderPass = pDesc->iShaderPass;
	m_iDiffuse = pDesc->iDiffuse;
	m_iNoise = pDesc->iNoise;
	m_iMask = pDesc->iMask;
	m_iNormal = pDesc->iNormal;
	m_fMaxTimer = pDesc->fMaxTimer;
	m_fDissolve_Speed = pDesc->fDissolve_Speed;
	m_fTimer_SpeedX = pDesc->fTimer_SpeedX;
	m_fTimer_SpeedY = pDesc->fTimer_SpeedY;
	m_fWeightX = pDesc->fWeightX;
	m_fWeightY = pDesc->fWeightY;
	m_fStartTexcoordX = pDesc->fStartTexcoordX;
	m_fStartTexcoordY = pDesc->fStartTexcoordY;
	m_fMaskCountX = pDesc->fMaskCountX;
	m_fMaskCountY = pDesc->fMaskCountY;
	m_vRGB = pDesc->vRGB;

	m_bTexcoordX = pDesc->bTexcoordX;
	m_bTexcoordY = pDesc->bTexcoordY;
	m_fTexcoord_LerpX = pDesc->fTexcoord_LerpX;
	m_fTexcoord_LerpY = pDesc->fTexcoord_LerpY;

	m_bUsing_Noise = pDesc->bUsing_Noise;
	m_bLoop = pDesc->bLoop;
	m_bMinus_X = pDesc->bMinus_X;
	m_bMinus_Y = pDesc->bMinus_Y;
	m_bGray = pDesc->bGray;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, pDesc->szModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	m_pTransformCom->Scaling(pDesc->vScale);
	m_pTransformCom->Rotation(XMConvertToRadians(pDesc->vRot.x), XMConvertToRadians(pDesc->vRot.y), XMConvertToRadians(pDesc->vRot.z * -1.f)); //이부분은 Tool이랑 뭔가 이상함
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vTranslation), 1.f));


    return S_OK;
}

void CEffect_Mesh::Priority_Update(_float _fTimeDelta)
{
	
}

void CEffect_Mesh::Update(_float _fTimeDelta)
{
	if (nullptr != m_pSocketMatrix && nullptr != m_pSettingMatrix) //소켓에 붙을 파티클
	{
		_matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		XMStoreFloat4x4(&m_matCombined, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * SocketMatrix * XMLoadFloat4x4(m_pSettingMatrix));

		if (true == m_bIsPlaying)
			Timer_Check(_fTimeDelta);
	}
	else
	{

		__super::Update(_fTimeDelta);
	}

}

void CEffect_Mesh::Late_Update(_float _fTimeDelta)
{
	switch (m_iShaderPass)
	{
	case 0:
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
		break;
	case 1: //nonblend
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
		break;
	case 2: //Distortion
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_DISTORTION, this);
		break;
	case 3: //Glow
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
		break;
	case 4: //Blood
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
		break;
	case 5: //Round
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_DISTORTION, this);
		break;
	case 6: //Roar
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
		break;
	case 7: //Disappear
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
		break;
	case 8: //Reverse
		//	m_pGameInstance->Add_RenderGroup(CRenderer::RG_REVERSE, this);
		break;
	case 9: //WeaponDisappear
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
		break;
	case 10: //TrailDisappear
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
		break;
	case 11: //BloodDisappear
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
		break;
	case 12: //BLOOD_SUCKER
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
		break;

	case 13: //GAS_BOOMBOOM
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
		break;
	case 14: //WEIGHT_BLEND
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_WEIGHTBLEND, this);
		break;
	}
}

HRESULT CEffect_Mesh::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fMaxTimer", &m_fMaxTimer, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURE_NORMAL]->Bind_ShaderResource(m_pShaderCom, "g_NormalTexture", m_iNormal)))
		return E_FAIL;

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pShaderCom->Begin(m_iShaderPass);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CEffect_Mesh::Render_Distortion()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pShaderCom->Begin(m_iShaderPass);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CEffect_Mesh::Render_Glow()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fMaxTimer", &m_fMaxTimer, sizeof(_float))))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pShaderCom->Begin(m_iShaderPass);
		m_pModelCom->Render(i);
	}

    return S_OK;
}

HRESULT CEffect_Mesh::Render_WeightBlend()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fMaxTimer", &m_fMaxTimer, sizeof(_float))))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pShaderCom->Begin(m_iShaderPass);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CEffect_Mesh::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_DiffuseTexture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Diffuse"),
		TEXT("Com_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_DIFFUSE]))))
		return E_FAIL;

	/* Com_NoiseTexture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Noise"),
		TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NOISE]))))
		return E_FAIL;

	/* Com_MaskTexture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Mask"),
		TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_MASK]))))
		return E_FAIL;

	/* Com_NormalTexture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Noise"),
		TEXT("Com_Normal"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NORMAL]))))
		return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Mesh::Bind_ShaderResources()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matCombined)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	//Particle 전용
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_TimeX", &m_fTimerX, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_TimeY", &m_fTimerY, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_StartTexcoordX", &m_fStartTexcoordX, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_StartTexcoordY", &m_fStartTexcoordY, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaskCountX", &m_fMaskCountX, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaskCountY", &m_fMaskCountY, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRGB", &m_vRGB, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bTexcoordX", &m_bTexcoordX, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bTexcoordY", &m_bTexcoordY, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTexcoordLerpX", &m_fTexcoord_LerpX, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTexcoordLerpY", &m_fTexcoord_LerpY, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bMinus_X", &m_bMinus_X, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bMinus_Y", &m_bMinus_Y, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bGray", &m_bGray, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURE_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iDiffuse)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURE_NOISE]->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", m_iNoise)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_iMask)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolve, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bUsing_Noise", &m_bUsing_Noise, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fWeightX", &m_fWeightX, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fWeightY", &m_fWeightY, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}


CEffect_Mesh* CEffect_Mesh::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CEffect_Mesh* pInstance = new CEffect_Mesh(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed Create : Effect_Mesh_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Mesh::Clone(void* _pArg)
{
	CEffect_Mesh* pInstance = new CEffect_Mesh(*this);

	if (FAILED(pInstance->Initialize(_pArg)))
	{
		MSG_BOX("Failed Clone : Effect_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Mesh::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);
	Safe_Release(m_pModelCom);
}

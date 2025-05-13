#include "pch.h"
#include "Effect_Instancing.h"
#include "GameInstance.h"

CEffect_Instancing::CEffect_Instancing(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CEffect{ _pDevice, _pContext }
{
}

CEffect_Instancing::CEffect_Instancing(const CEffect_Instancing& _Prototype)
    : CEffect(_Prototype)
{
}

HRESULT CEffect_Instancing::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Instancing::Initialize(void* _pArg)
{
	EFFECT_INSTANCING_DESC* pDesc = static_cast<EFFECT_INSTANCING_DESC*>(_pArg);

	m_iDiffuse = pDesc->iDiffuse;
	m_fMaxTimer = pDesc->fMaxTimer;
	m_fDissolve_Speed = pDesc->fDissolve_Speed;
	m_fTimer_SpeedX = pDesc->fTimer_SpeedX;
	m_fTimer_SpeedY = pDesc->fTimer_SpeedY;
	m_fHeightX = pDesc->fHeightX;
	m_fHeightY = pDesc->fHeightY;
	m_bLoop = pDesc->bLoop;

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

	Random_Explosion(0.6f, 1.5f);

    return S_OK;
}

void CEffect_Instancing::Priority_Update(_float _fTimeDelta)
{
	
}

void CEffect_Instancing::Update(_float _fTimeDelta)
{
	//socket 이면 위치조정 및 시간 체크 및 이제 false 가 되는지 체크
	__super::Update(_fTimeDelta);

}

void CEffect_Instancing::Late_Update(_float _fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CEffect_Instancing::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CEffect_Instancing::Set_IsPlaying(_bool _bIsPlaying)
{
	m_bIsPlaying = _bIsPlaying;
	if (true == _bIsPlaying) //시작해라 느낌
	{
		m_fTimerX = 0.f;
		m_fTimerY = 0.f;
		m_fDissolve = 0.f;
		m_fTimer_Timelag = 0.f;
		Clear_Setting();
		Random_Explosion(0.6f, 1.5f);
	}
}

HRESULT CEffect_Instancing::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_Instancing"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_DiffuseTexture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Instancing_Image"),
		TEXT("Com_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Instancing::Bind_ShaderResources()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matCombined)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_TimeX", &m_fTimerX, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_TimeY", &m_fTimerY, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iDiffuse)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fHorizonSpeed", &m_fDissolve, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fHeight", &m_fHeightX, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRotation_Weight", &m_fHeightY, sizeof(_float))))
		return E_FAIL;

	_float4 vPos = {};
	XMStoreFloat4(&vPos, _vector{ m_matCombined._41, m_matCombined._42, m_matCombined._43, m_matCombined._44 });

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vModelPosition", &vPos, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Horizon1", &m_vExplosionPower[0], sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Vertical1", &m_vExplosionPower[1], sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Rotation1", &m_vExplosionPower[2], sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Horizon2", &m_vExplosionPower[3], sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Vertical2", &m_vExplosionPower[4], sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Rotation2", &m_vExplosionPower[5], sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Horizon3", &m_vExplosionPower[6], sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Vertical3", &m_vExplosionPower[7], sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Explosion_Rotation3", &m_vExplosionPower[8], sizeof(_float4))))
		return E_FAIL;
	return S_OK;
}


void CEffect_Instancing::Random_Explosion(_float _fMin, _float _fMax)
{
	for (_uint i = 0; i < 9; i++)
	{
		m_vExplosionPower[i] = _float4(m_pGameInstance->Compute_Random(_fMin, _fMax),
									   m_pGameInstance->Compute_Random(_fMin, _fMax),
									   m_pGameInstance->Compute_Random(_fMin, _fMax),
									   m_pGameInstance->Compute_Random(_fMin, _fMax));
	}
}

CEffect_Instancing* CEffect_Instancing::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CEffect_Instancing* pInstance = new CEffect_Instancing(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed Create : Effect_Instancing_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Instancing::Clone(void* _pArg)
{
	CEffect_Instancing* pInstance = new CEffect_Instancing(*this);

	if (FAILED(pInstance->Initialize(_pArg)))
	{
		MSG_BOX("Failed Clone : Effect_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pModelCom);
}

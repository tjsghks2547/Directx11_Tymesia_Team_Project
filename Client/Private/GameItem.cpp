#include "pch.h"
#include "GameItem.h"
#include "GameInstance.h"
#include "UIGroup_Inventory.h"

CGameItem::CGameItem(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CItem{ _pDevice, _pContext }
{
}

CGameItem::CGameItem(const CGameItem& _Prototype)
    :CItem(_Prototype)
{
}

HRESULT CGameItem::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CGameItem::Initialize(void* _pArg)
{
    strcpy_s(m_szName, "GAMEITEM");

    GAMEITEM_DESC* pDesc = static_cast<GAMEITEM_DESC*>(_pArg);

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    strcpy_s(m_GameItemName, pDesc->GameItemName.c_str());

    m_eItemType = pDesc->eItemType;
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    m_pButtonGameObject = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Button"), "BUTTON");
    m_pButton = static_cast<CButton*>(m_pButtonGameObject);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    _vector vItemPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION); ;
    XMStoreFloat4(&m_vInitialPos, vItemPos);

    m_pGameInstance->Add_Item(pDesc->eItemType, pDesc->iItemCount, this, pDesc->bTaken);
    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.5f, 0.5f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    _uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::ITEM, iSettingColliderGroup);
    //m_pGameInstance->Add_Actor_Scene(m_pActor);

    m_pGroupInven = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_PlayerInventory"), "Inventory");


    switch (m_eItemType)
    {
    case Engine::ITEM_TYPE::ITEM_KEY1:
        m_iItemTypeNumber = 4;
        break;
    case Engine::ITEM_TYPE::ITEM_KEY2:
        m_iItemTypeNumber = 4;
        break;
    case Engine::ITEM_TYPE::ITEM_MEMORY:
        m_iItemTypeNumber = 5;
        break;
    case Engine::ITEM_TYPE::ITEM_FORGIVEN:
        m_iItemTypeNumber = 5;
        break;
    case Engine::ITEM_TYPE::ITEM_SKILLPIECE:
        m_iItemTypeNumber = 1;
        break;
    case Engine::ITEM_TYPE::ITEM_FIELDITEM:
        m_iItemTypeNumber = 4;
        break;

    case Engine::ITEM_TYPE::ITEM_BADZOKER:
        m_iItemTypeNumber = 2;
        break;
    case Engine::ITEM_TYPE::ITEM_BEOMSEUNG:
        m_iItemTypeNumber = 2;
        break;
    case Engine::ITEM_TYPE::ITEM_FAKER:
        m_iItemTypeNumber = 2;
        break;
    case Engine::ITEM_TYPE::ITEM_UIN:
        m_iItemTypeNumber = 2;
        break;
    case Engine::ITEM_TYPE::ITEM_PLAYERMASTER:
        m_iItemTypeNumber = 2;
        break;
    case Engine::ITEM_TYPE::ITEM_YUBHIN:
        m_iItemTypeNumber = 2;
        break;
        
    case Engine::ITEM_TYPE::ITEM_HERB_1:
        m_iItemTypeNumber = 1;
        break;
    case Engine::ITEM_TYPE::ITEM_HERB_2:
        m_iItemTypeNumber = 1;
        break;
    case Engine::ITEM_TYPE::ITEM_HERB_3:
        m_iItemTypeNumber = 1;
        break;
    case Engine::ITEM_TYPE::ITEM_HERB_4:
        m_iItemTypeNumber = 1;
        break;
    case Engine::ITEM_TYPE::ITEM_HERB_5:
        m_iItemTypeNumber = 1;
        break;
    case Engine::ITEM_TYPE::ITEM_HERB_6:
        m_iItemTypeNumber = 1;
        break;
    case Engine::ITEM_TYPE::ITEM_HERB_7:
        m_iItemTypeNumber = 1;
        break;
        // 1 : ÆÄ¶û
        // 2 : ÃÊ·Ï
        // 3 : »¡°­ 
        // 4 : ³ë¶û 
    }


    return S_OK;
}

void CGameItem::Priority_Update(_float _fTimeDelta)
{

}

void CGameItem::Update(_float _fTimeDelta)
{
    m_fTime += _fTimeDelta;
    if (m_fTime >= 20.0f)
    {
        m_fTime = 0.0f;
    }

    if (!m_bAcquired)
    {
        if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
            m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 0.f,0.f,1.f });
    }

    if (m_fElapsedTime >= 1.07f)
    {
        m_bDropping = false;
    }

    if (m_bDropping)
    {
        m_fElapsedTime += _fTimeDelta * 0.5f;
        _vector vItemPos = XMLoadFloat4(&Bezier(m_vInitialPos, m_vCurvePos, m_vEndPos, m_fElapsedTime));
        _vector vDir = XMVector3Normalize(vItemPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        m_pTransformCom->LookAt(vItemPos + vDir);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vItemPos);
    }

    if (!m_bStartAcquireEffect)
    {
        m_vCurrentScale = _float3(1.3f, 1.3f, 1.3f);
    }
    else if (m_bStartAcquireEffect && !m_bFinishAcquireEffect)
    {
        if (m_bEnLarging)
        {
            m_fEnLargingTime += _fTimeDelta;

            _float fEnlargeDuration = 0.1f;
            _float fRatio = min(m_fEnLargingTime / fEnlargeDuration, 1.0f);
            _float fScale = Compute_LerpItemScale(1.3f, 2.5f, fRatio);
            m_vCurrentScale = _float3(fScale, fScale, fScale);
            m_fAlphaValue.w = 5.0f;

            if (fRatio >= 1.0f)
            {
                m_bEnLarging = false;
                m_bEnLargingDone = true;
                m_fAcquireEffectTime = 0.f;
            }
        }
        else if (m_bEnLargingDone)
        {
            m_fAcquireEffectTime += _fTimeDelta;

            _float fShrinkDuration = 0.35f;
            _float fRatio = min(m_fAcquireEffectTime / fShrinkDuration, 1.0f);
            _float fScale = Compute_LerpItemScale(2.5f, 0.0f, fRatio);
            m_vCurrentScale = _float3(fScale, fScale, fScale);

            m_fAlphaValue.w = Compute_LerpItemScale(1.0f, 0.0f, fRatio);

            if (fRatio >= 1.0f)
            {
                m_bFinishAcquireEffect = true;
                m_bAcquired = true;
                m_pGameInstance->Acquire_Item(m_eItemType);
            }
        }
    }
}

void CGameItem::Late_Update(_float fTimeDelta)
{
    if (!m_bAcquired || m_bStartAcquireEffect)
    {
        Setting_BillBoard();
        m_pTransformCom->Scaling(m_vCurrentScale);
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
    }
}

HRESULT CGameItem::Render()
{
    return S_OK;
}

HRESULT CGameItem::Render_Glow()
{
    if (!m_bActivate)
        return S_OK;

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4   ViewMatrix, ProjMatrix;
    ViewMatrix = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW);
    ProjMatrix = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fTime, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pNoiseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_ItemNumber", &m_iItemTypeNumber, sizeof(_uint))))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CGameItem::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ItemSoul"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ItemSoulNoise"), TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pNoiseTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxItem"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CGameItem::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

void CGameItem::Set_ItemPos(_fvector _vPosition)
{
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vPosition);
}

void CGameItem::Setting_BillBoard()
{
    _vector vLook = XMVector3Normalize(XMVectorSetW(XMLoadFloat4(&m_pGameInstance->Get_CamPosition()) - m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.0f));
    _vector vRight = XMVector3Normalize(XMVector3Cross(_vector{ 0.0f, 1.0f, 0.0f, 0.0f }, vLook)) * 0.5f;
    _vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * 0.5f;

    vLook = XMVector3Normalize(vLook) * 0.5f;

    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
    m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
}

// ´ê±â ½ÃÀÛÇÒ ¶§ ¼ø°£
void CGameItem::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!m_bAcquired && !m_bDropping)
    {
        _vector vItemPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        vItemPos = XMVectorSetY(vItemPos, XMVectorGetY(vItemPos) + 1.0f);

        _float4 vItemPosition;
        XMStoreFloat4(&vItemPosition, vItemPos);

        m_pButton->Set_WorldPosition(vItemPosition);
        m_pButton->Set_ButtonText(TEXT("E"), TEXT("ÁÝ±â"));
        m_pButton->Activate_Button(true);
    }
}

// ´ê´Â Áß.
void CGameItem::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    if (!m_bAcquired && false == m_bStartAcquireEffect)
    {
        switch (m_eItemType)
        {
        case Engine::ITEM_TYPE::ITEM_KEY1:
        case Engine::ITEM_TYPE::ITEM_KEY2:
        case Engine::ITEM_TYPE::ITEM_MEMORY:
        case Engine::ITEM_TYPE::ITEM_FORGIVEN:
        case Engine::ITEM_TYPE::ITEM_SKILLPIECE:
        case Engine::ITEM_TYPE::ITEM_FIELDITEM:
            if (m_pGameInstance->isKeyEnter(DIK_E))
            {
                m_bStartAcquireEffect = true;
                m_bEnLarging = true;
                m_fEnLargingTime = 0.f;
                m_pButton->Activate_Button(false);
                // 1 : ÆÄ¶û
                // 2 : ÃÊ·Ï
                // 3 : »¡°­ 
                // 4 : ³ë¶û 
                switch (m_iItemTypeNumber)
                {
                case 1:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_BLUE, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                case 2:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_GREEN, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                case 3:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_RED, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                case 4:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_YELLOW, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                case 5:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_WHITE, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                }

                m_pGameInstance->Play_Sound(TEXT("Item_PickUp.ogg"), CHANNELID::SOUND_STRUCT, 0.5f);
            }
        default:
            if (m_pGameInstance->isKeyEnter(DIK_E))
            {
                m_bStartAcquireEffect = true;
                m_bEnLarging = true;
                m_fEnLargingTime = 0.f;
                m_pButton->Activate_Button(false);
                // 1 : ÆÄ¶û
                // 2 : ÃÊ·Ï
                // 3 : »¡°­ 
                // 4 : ³ë¶û 
                switch (m_iItemTypeNumber)
                {
                case 1:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_BLUE, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                case 2:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_GREEN, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                case 3:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_RED, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                case 4:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_YELLOW, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                case 5:
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_WHITE, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                    break;
                }

                m_pGameInstance->Play_Sound(TEXT("Item_PickUp.ogg"), CHANNELID::SOUND_STRUCT, 0.5f);
            }
            break;
        }
        

    }
}

// ¶³¾îÁú ¶§ ( ³ª°¡±â Á÷Àü. )
void CGameItem::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    //if (!m_bAcquired)
    {
        m_pButton->Activate_Button(false);
    }
}

CGameItem* CGameItem::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CGameItem* pInstance = new CGameItem(_pDevice, _pContext);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CGameItem");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CGameItem::Clone(void* _pArg)
{
    CGameItem* pInstance = new CGameItem(*this);
    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CGameItem");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGameItem::Free()
{
    __super::Free();

    m_pGameInstance->Sub_Actor_Scene(m_pActor);

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pNoiseTextureCom);
    Safe_Release(m_pVIBufferCom);
}

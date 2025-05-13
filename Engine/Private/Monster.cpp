#include "Monster.h"
#include "GameInstance.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster& Prototype)
    :CContainerObject(Prototype)
{
}

void CMonster::Reset_Info()
{
    //나중에 뭐가 필요한지 추가해봐야할듯
    m_fDelayTime = 0.f;
    m_bNeed_Rotation = false;
}
void CMonster::Cheat()
{
    m_fMonsterCurHP -= m_fMonsterMaxHP * 0.5f;
    m_fShieldHP -= m_fMonsterMaxHP * 0.5f;
}
HRESULT CMonster::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER");
    m_fMonsterMaxHP = 100.f;
    m_fMonsterCurHP = m_fMonsterMaxHP;
    m_fShieldHP = m_fMonsterMaxHP;
    m_fRotateSpeed = 10.f;
    m_fCoolTime = 1.f;
    CGameObject::GAMEOBJECT_DESC* Desc = static_cast<GAMEOBJECT_DESC*>(pArg);

    Desc->fSpeedPerSec = 1.f;
    Desc->fScaling = _float3{ 0.0025f,0.0025f,0.0025f };
    Desc->fRotationPerSec = XMConvertToRadians(90.f);
    XMStoreFloat4(&m_vSpawnPoint, XMLoadFloat4(&Desc->fPosition));

    if (FAILED(__super::Initialize(Desc)))
        return E_FAIL;

    return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
    if (m_bDead)
    {
        m_bRender = false;
        m_pGameInstance->Add_Delete_Monster(this);
        return;
    }
#ifdef _DEBUG


    if (m_pGameInstance->isKeyEnter(DIK_K))
    {
        Cheat();
    }

#endif // _DEBUG

    m_fTimeDelta = fTimeDelta;
    CalCulate_Distance();

    if (m_fDistance <= m_fActive_Distance && !m_bActive)
    {
        Active();
    }
    //스폰포인트로부터 거리가 멀고 패턴이 진행중이지않으며 돌아갈수있을떄 돌아가라
    if (m_fSpawn_Distance >= m_fSpawn_Distance_Max && !m_bPatternProgress && !Is_Player_Near() && !m_Is_Boss)
    {
        Return_To_Spawn();
    }

    if (m_fMonsterCurHP <= 0.f && !m_IsStun)
    {
        Stun();
    }
    if (m_bHP_Bar_Active && !m_Is_Boss)
    {
        m_fHP_Bar_Active_Timer += fTimeDelta;
        if (m_fHP_Bar_Active_Timer >= 10.f)
        {
            m_fHP_Bar_Active_Timer = 0.f;
            m_bHP_Bar_Active = false;
        }
    }

    __super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
    if (!m_Is_Player_Dead)
        PatternCreate();

    RootAnimation();

    State_Update(fTimeDelta);
    if (m_bNeed_Rotation)
        Rotation_To_Player();

    _vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition) + m_fNaviOffset)); // Navi OffSet 추가

    __super::Update(fTimeDelta);
}

void CMonster::Late_Update(_float fTimeDelta)
{
    Recovery_HP();
    //파츠 돌리기
    __super::Late_Update(fTimeDelta);
}

HRESULT CMonster::Render()
{
    return S_OK;
}

void CMonster::RootAnimation()
{
    _vector      vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector test = { 0.f,0.f,0.f,1.f };
    /* 루트 모션 애니메션 코드 */
    m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("root");

    if ((!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModelCom->Get_LerpFinished() && m_bActive && !m_bNot_Need_Root))
    {
        if ((m_pNavigationCom->isMove(vCurPosition) && m_fDistance > m_fRootDistance) || m_bCan_Move_Anim)
            m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);

        /* 2월 19일 추가 코드 */
        if (!m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
        {
            _vector PrePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

            _float4x4 test = {};
            XMStoreFloat4x4(&test, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pRootMatrix)));
            const _float4x4* test2 = const_cast<_float4x4*>(&test);
            m_pTransformCom->Set_MulWorldMatrix(test2);

            // Pretest 와 test2 의 변위량 구해서 넣어놓기
            _vector Curtest = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

            _float MoveSpeed = XMVectorGetX(XMVector3Length(PrePos - Curtest));

            /* 루트 애니메이션의 이동량 만큼만 슬라이딩 할 수 있게 설정하기*/
            m_pTransformCom->Sliding_Monster_Root_Ani(m_fTimeDelta, m_pNavigationCom, MoveSpeed);
        }
    }
}

void CMonster::CalCulate_Distance()
{
    XMStoreFloat4(&m_vPlayerPos, m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));
    _vector pPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_vPlayerPos) - pPosition));
    m_fSpawn_Distance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_vSpawnPoint) - pPosition));
}

void CMonster::PatternCreate()
{
}

void CMonster::State_Update(_float fTimeDelta)
{
}

void CMonster::Active()
{
}

void CMonster::Return_To_Spawn()
{
}

void CMonster::Stun()
{
}

void CMonster::RotateDegree_To_Player()
{
    m_bNeed_Rotation = false;
    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    _vector vLook2 = XMLoadFloat4(&m_vPlayerPos) - vPos;

    vLook2 = XMVectorSet(XMVectorGetX(vLook2), 0.f, XMVectorGetZ(vLook2), 0.f);


    vLook = XMVector3Normalize(vLook);
    vLook2 = XMVector3Normalize(vLook2);

    //회전해야 하는 각도
    _float fAngle = acos(XMVectorGetX(XMVector3Dot(vLook, vLook2)));
    fAngle = XMConvertToDegrees(fAngle);
    _vector fCrossResult = XMVector3Cross(vLook, vLook2);

    if (XMVectorGetY(fCrossResult) < 0)
    {
        fAngle *= -1;
    }
    m_fRotateDegree = fAngle;

    if (fabs(m_fRotateDegree) > 1.f)
        m_bNeed_Rotation = true;
}

void CMonster::Rotation_To_Player()
{
    _vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector vMyLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    _vector vTargetDir = XMLoadFloat4(&m_vPlayerPos) - vMyPos;

    vTargetDir = XMVectorSet(XMVectorGetX(vTargetDir), 0.f, XMVectorGetZ(vTargetDir), 0.f);
    vTargetDir = XMVector3Normalize(vTargetDir);
    vMyLook = XMVector3Normalize(vMyLook);

    _vector vNewLook = XMVectorLerp(vMyLook, vTargetDir, m_fRotateSpeed * m_fTimeDelta);
    vNewLook = XMVector3Normalize(vNewLook);

    m_pTransformCom->Look(vNewLook);


    _float fDot = XMVectorGetX(XMVector3Dot(vNewLook, vTargetDir));
    if (fDot >= 0.9995f)
        m_bNeed_Rotation = false;
    else
        m_bNeed_Rotation = true;
}


_bool CMonster::Is_Player_Near()
{
    return m_fDistance < 3.f;
}

void CMonster::Recovery_HP()
{
    if (m_fMonsterCurHP != m_fShieldHP)
    {
        m_fRecoveryTime += m_fTimeDelta;
        if (m_fRecoveryTime >= 5.f)
            m_bCanRecovery = true;
    }
    if (m_bCanRecovery)
    {
        m_fShieldHP += 0.1f;
        if (m_fShieldHP >= m_fMonsterCurHP)
        {
            m_fShieldHP = m_fMonsterCurHP;
            m_bCanRecovery = false;
            m_fRecoveryTime = 0.f;
        }
    }
}

void CMonster::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CMonster::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CMonster::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

void CMonster::Free()
{
    __super::Free();
    m_pGameInstance->Sub_Actor_Scene(m_pActor);
    m_pGameInstance->Sub_Actor_Scene(m_pStunActor);
    Safe_Release(m_pNavigationCom);
}
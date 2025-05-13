#include "Effect.h"
#include "GameInstance.h"   

CEffect::CEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject{ _pDevice, _pContext }
{
}

CEffect::CEffect(const CEffect& _Prototype)
	: CGameObject(_Prototype)
	, m_bIsPlaying{ _Prototype.m_bIsPlaying }
	, m_pAnimation_Speed{ _Prototype.m_pAnimation_Speed }
	, m_pSettingMatrix{ _Prototype.m_pSettingMatrix }
	, m_pSocketMatrix{ _Prototype.m_pSocketMatrix }
	//, m_eEffectType{ _Prototype.m_eEffectType }
{
}

HRESULT CEffect::Initialize_Prototype()
{
	

    return S_OK;
}

HRESULT CEffect::Initialize(void* _pArg)
{
	EFFECT_DESC* pDesc = static_cast<EFFECT_DESC*>(_pArg);

	//m_eEffectType = pDesc->eType;

	if (FAILED(__super::Initialize(_pArg)))
		return E_FAIL;

	XMStoreFloat4x4(&m_matParentWorld, XMMatrixIdentity());

    return S_OK;
}

void CEffect::Priority_Update(_float _fTimeDelta)
{
}

void CEffect::Update(_float _fTimeDelta)
{
	if (nullptr != m_pSettingMatrix)
	{
		//m_pTransformCom->Set_MulWorldMatrix(m_pSettingMatrix);
		XMStoreFloat4x4(&m_matCombined, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pSettingMatrix));
	}
	else// socket이 아니다
	{
		XMStoreFloat4x4(&m_matCombined, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(&m_matParentWorld));
	}
	if (true == m_bIsPlaying)
		Timer_Check(_fTimeDelta);
}

void CEffect::Late_Update(_float _fTimeDelta)
{
}

HRESULT CEffect::Render()
{
    return S_OK;
}

void CEffect::Set_IsPlaying(_bool _bIsPlaying)
{
	m_bIsPlaying = _bIsPlaying;
	if (true == _bIsPlaying) //시작해라 느낌
	{
		m_fTimerX = 0.f;
		m_fTimerY = 0.f;
		m_fDissolve = 0.f;
		m_fTimer_Timelag = 0.f;
		Clear_Setting();
	}
}

void CEffect::Clear_Setting()
{
	if (nullptr != m_pSettingMatrix)
		m_pSettingMatrix = nullptr;
	if (nullptr != m_pAnimation_Speed)
		m_pAnimation_Speed = nullptr;
	if (nullptr != m_pSocketMatrix)
		m_pSocketMatrix = nullptr;

	XMStoreFloat4x4(&m_matParentWorld, XMMatrixIdentity());
}

void CEffect::Set_Direction(_fvector _vDir)
{
	m_pTransformCom->Look(_vDir);
}

void CEffect::Set_Pos_With_Matrix(_float4x4 _matWorld)
{
	XMStoreFloat4x4(&m_matParentWorld, XMLoadFloat4x4(&_matWorld));
}

void CEffect::Set_Animation_Speed(const _float* _pAnimation_Speed)
{
	m_pAnimation_Speed = _pAnimation_Speed;
}

void CEffect::Timer_Check(_float _fTimeDelta)
{
	if (m_fMaxTimer < m_fTimerX || m_fMaxTimer < m_fTimerY)
	{
		m_bIsPlaying = false;
		m_fTimerX = 0.f;
		m_fTimerY = 0.f;
		m_fDissolve = 0.f;
	}
	else
	{
		if (nullptr != m_pAnimation_Speed)
		{
			m_fTimerX += _fTimeDelta * m_fTimer_SpeedX * (*m_pAnimation_Speed);
			m_fTimerY += _fTimeDelta * m_fTimer_SpeedY * (*m_pAnimation_Speed);
			m_fDissolve += _fTimeDelta * m_fDissolve_Speed * (*m_pAnimation_Speed);
			if (0.1f > (*m_pAnimation_Speed))
			{
				m_fTimer_Timelag += _fTimeDelta;
				if (0.02f < m_fTimer_Timelag)
					m_pAnimation_Speed = nullptr;
			}
		}
		else
		{
			m_fTimerX += _fTimeDelta * m_fTimer_SpeedX;
			m_fTimerY += _fTimeDelta * m_fTimer_SpeedY;
			m_fDissolve += _fTimeDelta * m_fDissolve_Speed; //이놈은 TimerX 한테 영향받음 나중에 따로 뺼지 아니면 X와 Y를 같이 넣어줘야할지 고민됨
		}
		
	}
}

void CEffect::Free()
{
	__super::Free();

}

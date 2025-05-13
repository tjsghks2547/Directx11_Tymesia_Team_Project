#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "States.h"

BEGIN(Client)
template <typename T>
class CState_Machine : public CBase
{
public:
	CState_Machine() = default;
	virtual ~CState_Machine() = default;

public:
	HRESULT     Initialize()
	{
		return S_OK;
	}

	void State_Update(_float fTimeDelta, T* pObject)
	{
		if (m_pState == nullptr)
			return;
		m_pState->State_Update(fTimeDelta, pObject);
	}

	void        ChangeState(CStates<T>* pState, T* pObject)
	{
		if (m_pState != nullptr)
		{
			m_pState->State_Exit(pObject);
			Safe_Release(m_pState);
		}

		m_pState = pState;
		if (m_pState == nullptr)
			return;

		m_pState->State_Enter(pObject);
	}

private:
	CStates<T>* m_pState = { nullptr };
public:
	static CState_Machine* Create()
	{
		CState_Machine* pInstance = new CState_Machine();

		if (FAILED(pInstance->Initialize()))
		{
			MSG_BOX("Failed To Created : CState_Machine");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual void Free()
	{
		__super::Free();
		Safe_Release(m_pState);
	}
};

END
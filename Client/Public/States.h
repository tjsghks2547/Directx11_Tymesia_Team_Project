#pragma once
#include "Client_Defines.h"	
#include "Base.h"


BEGIN(Client)

class CCamera_Free;

template <typename T>
class CStates abstract : public CBase
{
protected:
	CStates() = default;
	virtual ~CStates() = default;

public:
	virtual void State_Enter(T* pObject) = 0;	 // ������ ������Ʈ���� �˱� ���� ���� ������Ʈ �־���ҵ�
	virtual void State_Update(_float fTimeDelta, T* pObject) = 0;
	virtual void State_Exit(T* pObject) = 0;
protected:
	_uint m_iIndex = {};
	CCamera_Free* m_pCamera = { nullptr };	
public:
	virtual void Free()
	{
		__super::Free();
	}
};

END

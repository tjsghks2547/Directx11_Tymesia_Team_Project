#pragma once


#include "State.h"

class CStateMgr final : public CState
{
private:
	CStateMgr();
	virtual ~CStateMgr() = default;


public:
	HRESULT Initialize();


private:
	vector<CState*>  m_vecState;

public:
	vector<CState*>& Get_VecState() { return m_vecState; }


public:
	static CStateMgr* Create();
	virtual void Free() override;

};


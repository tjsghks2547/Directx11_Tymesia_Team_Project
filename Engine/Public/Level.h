#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();
	_uint  Get_Current_Level_Index() { return m_iCurrentLevel; }

public:
	void Set_NextLevel_Open(_bool bOpen, _uint _iNumberLevel) { 
		m_bNextLevelOpen = bOpen; 
		m_iNextLevel = _iNumberLevel;
	}

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	class CGameInstance*		m_pGameInstance = { nullptr };
protected:
	_bool m_bNextLevelOpen = { false };
	_uint m_iCurrentLevel = {};
	_uint m_iNextLevel = {}; //임시로 추가

public:
	virtual void Free() override;
};

END
#pragma once


#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

/* ��������(Window API) ������ ������ �����ϱ� ����� ������ */
/* �� �ڵ��� �帧�� CMainAppŬ������ �ݺ����� ������Ʈ�� �������� ��ܿ´�. */
BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private: /* ������ ����� �̿뤷�ϱ����� �����ν�ư�� Ŭ������ ������üȭ �س��Ҵ�. */
	CGameInstance*			m_pGameInstance      = { nullptr };
	ID3D11Device*			m_pDevice            = { nullptr };
	ID3D11DeviceContext*	m_pContext           = { nullptr };

	_bool*				    m_pSlowWord          = { false };	
	_float				    m_fSlowWorldEndTimer = {};	

#ifdef _DEBUG
private:
	_float				m_fTimerAcc = {};
	_uint				m_iDrawCount = {};
	_tchar				m_szFPS[MAX_PATH] = {};
#endif

public:
	HRESULT Start_Level(LEVELID eStartLevelID);

	// Ƽ�޽þ� ��Ʈ
	HRESULT	LoadFont_Thymasia();
public:
	static CMainApp* Create();
	virtual void Free() override;
};

END
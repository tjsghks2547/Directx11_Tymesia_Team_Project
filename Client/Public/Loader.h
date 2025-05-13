#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader : public CBase
{
protected:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;


public:
	virtual HRESULT Initialize(LEVELID eNextLevelID);
	virtual HRESULT Start_Loading();
	_bool isFinished() const {
		return m_isFinished;
	}


#ifdef _DEBUG
public:
	void SetUp_WindowText();
#endif

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };
	LEVELID					m_eNextLevelID = { LEVEL_END };

	HANDLE					m_hThread = {};
	CRITICAL_SECTION		m_CriticalSection = {};

	_bool					m_isFinished = { false };
	_tchar					m_szLoadingText[MAX_PATH] = {};

protected:
	virtual HRESULT Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix) PURE;
	virtual vector<string> GetFBXFileNames(const string& folderPath);

public:
	//static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END
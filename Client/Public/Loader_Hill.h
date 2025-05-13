#pragma once

#include "Client_Defines.h"
#include "Loader.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader_Hill final : public CLoader
{
private:
	CLoader_Hill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader_Hill() = default;


public:
	virtual HRESULT Initialize(LEVELID eNextLevelID) override;
	virtual HRESULT Start_Loading() override;

private:
	HRESULT Loading_For_Level_Hill();
	
private:
	HRESULT Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix) override;

public:
	static CLoader_Hill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END
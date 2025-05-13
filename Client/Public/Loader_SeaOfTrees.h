#pragma once

#include "Client_Defines.h"
#include "Loader.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader_SeaOfTrees final : public CLoader
{
private:
	CLoader_SeaOfTrees(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader_SeaOfTrees() = default;


public:
	virtual HRESULT Initialize(LEVELID eNextLevelID) override;
	virtual HRESULT Start_Loading() override;

private:
	HRESULT Loading_For_Level_SeaOfTrees();
	HRESULT Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix) override;

public:
	static CLoader_SeaOfTrees* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END
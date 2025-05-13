#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	typedef struct TerrainInfo : public CGameObject::GAMEOBJECT_DESC
	{
		_float4 vTerrainPos = { 30.0f, 0.0f, -150.0f, 1.0f };
	}TERRAININFO;
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureNormalCom = { nullptr };
	CTexture* m_pTextureORMCom = { nullptr };
	CTexture* m_pTextureMaskCom = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

public:
	HRESULT Ready_Components(void* _pArg);
	HRESULT Bind_ShaderResources();

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
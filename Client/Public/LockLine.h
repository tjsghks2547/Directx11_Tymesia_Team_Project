#pragma once
#include "Client_Defines.h"
#include "Locked_On.h"
#include "Player.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)
class CLockLine final : public CGameObject
{
public:
	struct LOCKLINE_DESC : public CPartObject::PARTOBJECT_DESC
	{
	};

private:
	CLockLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLockLine(const CLockLine& Prototype);
	virtual ~CLockLine() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
	virtual HRESULT				Render_Glow() override;
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
private:
	_uint						m_iCurrentLevel = {};
	CContainerObject* m_pTargetMonsterPtr = { nullptr };
	CPlayer* m_pPlayer = { nullptr };
	CGameObject* m_pTargetPtr = { nullptr };
	_float4						m_vStartPos = {};
private:
	_float						m_fLineLength = { 1.0f };
	_float						m_fTime = {};

public:
	HRESULT						Ready_Components();

public:
	static CLockLine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};

END
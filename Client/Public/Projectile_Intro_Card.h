#pragma once

#include "PartObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CProjectile_Intro_Card final : public CPartObject
{
public:
	struct PROJECTILE_INTRO_CARD_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_bool* bActive = { nullptr };
		_bool* bRender = { nullptr };
		_bool* bChangeModel = { nullptr };
	};
private:
	CProjectile_Intro_Card(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProjectile_Intro_Card(const CProjectile_Intro_Card& Prototype);
	virtual ~CProjectile_Intro_Card() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Glow() override;
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pExodia_Card_ModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
private:
	const _uint* m_pParentState = { nullptr };
	const _bool* m_pParentActive = { nullptr };
	const _bool* m_pRender = { nullptr };
	const _bool* m_pChangeModel = { nullptr };
public:
	static CProjectile_Intro_Card* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END


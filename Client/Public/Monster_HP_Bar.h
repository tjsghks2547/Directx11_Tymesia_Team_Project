#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END


BEGIN(Client)
class CMonster_HP_Bar final : public CContainerObject
{
public:
	struct Monster_HP_Bar_DESC : public CContainerObject::CONTAINEROBJECT_DESC
	{
		const _float4x4* pMonsterMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		_float* fHeight = { nullptr };
		_float* fMaxHP = { nullptr };
		_float* fCurHP = { nullptr };
		_float* fShieldHP = { nullptr };
		_bool* bHP_Bar_Active = { nullptr };
		_bool* bDead = { nullptr };
	};
private:
	CMonster_HP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_HP_Bar(const CMonster_HP_Bar& Prototype);
	virtual ~CMonster_HP_Bar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Bill_Board();
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	const _float4x4* m_pMonsterMatrix = { nullptr };
	_float4x4 m_ResultMatrix = {};

private:
	_bool* m_bHP_Bar_Active = { nullptr };
	_bool* m_bMonsterDead = { nullptr };
	_float* m_fHeight = { nullptr };
	_float			   m_fTimeDelta = { 0.f };
private:
	const _uint* m_pParentState = { nullptr };


public:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects(Monster_HP_Bar_DESC* pDesc);
	HRESULT Bind_ShaderResources();
public:
	static CMonster_HP_Bar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CBody_Magician final : public CPartObject
{
	enum eDissolveState
	{
		DISSOLVE_NONE,           // �ƹ��͵� �� �ϴ� ����
		DISSOLVE_ON,    // ������� ��
		DISSOLVE_OFF    // ��Ÿ���� ��
	};

public:
	struct BODY_MAGICIAN_DESC : public CPartObject::PARTOBJECT_DESC
	{
		_bool* IsDissolveOn = { nullptr };
		_bool* IsDissolveOff = { nullptr };
		_bool* bRender = { nullptr };
	};

private:
	CBody_Magician(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Magician(const CBody_Magician& Prototype);
	virtual ~CBody_Magician() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
private:
	_bool* m_IsDissolveOn = {};
	_bool* m_IsDissolveOff = {};
	_bool* m_bRender = {};
	_bool* m_bDead = {};
private:
	eDissolveState m_eDissolveState = DISSOLVE_NONE;
	_float m_fDissolveTimer = 0.f; // �������� ���
	_uint m_iPassNum = {};

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBody_Magician* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
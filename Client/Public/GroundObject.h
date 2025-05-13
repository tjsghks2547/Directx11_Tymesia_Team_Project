#pragma once
#include "EnvironmentObject.h"

BEGIN(Client)
class CGroundObject final : public CEnvironmentObject
{
public:
	struct GROUND_OBJECCT_DESC : public CEnvironmentObject::ENVIRONMENT_OBJECT_DESC
	{

	};

private:
	CGroundObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CGroundObject(const CGroundObject& _Prototype);
	virtual ~CGroundObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* _pArg) override;
	virtual void Priority_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Late_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Motion_Blur() override;	

	void Update_InstanceBuffer(_uint _iInstanceIndex, const XMFLOAT3& _vPosition, const XMFLOAT3& _vScale, const XMFLOAT4& _vRotation);
	void Update_InstanceBuffer_ForCulling();
private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Bind_Motion_Blur_ShaderResources();	
	vector<_bool>			m_vecVisible;

public:
	static CGroundObject* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};
END
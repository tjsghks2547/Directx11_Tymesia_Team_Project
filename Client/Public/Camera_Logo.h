#pragma once

#include "Client_Defines.h"
#include "Camera.h"


BEGIN(Client)

class CCamera_Logo final : public CCamera
{
public:
	struct LOGO_CAMERA_DESC : public CCamera::CAMERA_DESC
	{
	};

private:
	CCamera_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Logo(const CCamera_Logo& Prototype);
	virtual ~CCamera_Logo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static  CCamera_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
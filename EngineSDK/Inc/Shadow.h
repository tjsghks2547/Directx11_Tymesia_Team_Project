#pragma once

/* �׸��ڸ� ǥ���ϱ� ���� ������ ������ ��� �ִ´�. */

#include "Base.h"

BEGIN(Engine)

class CTransform;

class CShadow final : public CBase
{
private:
	CShadow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CShadow() = default;

public:
	HRESULT Bind_LightPos(class CShader* pShader, const _char* pConstantName);

	HRESULT Bind_LightDir(class CShader* pShader, const _char* pConstantName);

	HRESULT Bind_LightProjDir(CShader* pShader, const _char* pConstantName);

	_float2 Get_LightPos();

public:
	HRESULT Initialize();
	HRESULT SetUp_ShadowLight(_fvector vEye, _fvector vAt, _float fLightAngle, _float fAspect, _float fNear, _float fFar, _matrix matInvCam, _fvector vCamInfo, CTransform* pTransform); // ���⼭ �÷��̾� ��ġ �����ͷ� �Ѱ�����
	HRESULT Bind_ViewMatrix(class CShader* pShader, const _char* pConstantName);
	HRESULT Bind_ProjMatrix(class CShader* pShader, const _char* pConstantName);
	HRESULT Delete_Shadow(class CTransform* pTransform);

	HRESULT Bind_LightZ(CShader* pShader);
	void    Update();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	_float4x4			 m_LightViewMatrix = {};
	_float4x4			 m_LightProjMatrix = {};

	_float4				 m_LightPos = {};

	_float	    		 m_cascadeEnd[4] = {};

	_float4	    		 m_cascadeFrustum[3][8] = {};

	_float4x4		     m_shadowOrthoView[3] = {};
	_float4x4			 m_shadowOrthoProj[3] = {};

	_float4x4		     m_shadowCopyOrthoView[3] = {};
	_float4x4			 m_shadowCopyOrthoProj[3] = {};


	class CGameInstance* m_pGameInstance = { nullptr };

	_float				 m_CascadeEndCliSpaceZ[3] = {};
	_float4				 m_LightDir = { };

	CTransform* m_pPlayerTransform = { nullptr };

public:
	static CShadow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Dynamic final : public CBase
{
private:
	CLight_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight_Dynamic() = default;

public:
	const LIGHT_DESC* Get_LightDesc() const {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc, class CTransform* pTransform);
	void Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	_bool Delete_Light(class CTransform* pTransform);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	LIGHT_DESC				m_LightDesc{};
	class CTransform* m_pTransform = { nullptr };

public:
	static CLight_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHT_DESC& LightDesc, class CTransform* pTransform);
	virtual void Free() override;
};

END
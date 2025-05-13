#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight() = default;

public:
	LIGHT_DESC* Get_LightDesc() {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc);
	void Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

public:
	_bool	Delete_Light(_uint _iCurrentLevel) {
		if (m_LightDesc.iCurrentLevel == _iCurrentLevel)
			return true;

		return false;
	}

	_bool  Delete_Light_Type(LIGHT_DESC::TYPE _eType) {
		if (m_LightDesc.eType == _eType)
			return true;

		return false;
	}

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	LIGHT_DESC				m_LightDesc{};

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHT_DESC& LightDesc);
	virtual void Free() override;
};

END
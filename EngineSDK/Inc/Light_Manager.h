#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
private:
	CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iIndex) const;

public:
	HRESULT Initialize();
	void Update(_float _fTimeDelta);
	HRESULT Add_Light(const LIGHT_DESC& LightDesc, class CTransform* pTransform);
	HRESULT Add_Light_Range(const LIGHT_DESC& LightDesc, _float fMaxRange, _float fRangeSpeed);
	HRESULT Add_Light_Range_MINUS(const LIGHT_DESC& LightDesc, _float fMinRange, _float fRangeSpeed);
	void Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

	HRESULT Delete_Light_Type(LIGHT_DESC::TYPE eType);

	HRESULT Delete_Dynamic_Light(class CTransform* pTransform);

	HRESULT Delete_Static_Light(_uint iCurrentLevel);

private:
	ID3D11Device* m_pDevice = {};
	ID3D11DeviceContext* m_pContext = {};
	list<class CLight*>				m_Lights;
	list<class CLight_Dynamic*>		m_LightDynamics;

	vector<LIGHT_RANGE_DESC>		m_LightRangeDescs;
	vector<LIGHT_RANGE_DESC_MINUS>		m_LightRangeMinDescs;

public:
	static CLight_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
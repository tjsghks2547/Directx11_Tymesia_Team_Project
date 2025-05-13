#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 1. ���� ������ �ʿ���ϴ� �ڿ��� �غ��Ѵ�. */
/* 2. �ε�ȭ���� ����(�ε�ȭ�鿡 �ʿ��� ��ü�� ����� ���� ������ �����Ѵ�)�Ѵ�. */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;


public:
	virtual HRESULT Initialize(LEVELID eNextLevelID, _uint iLoadingNum, _bool bCheck);
	virtual void Update(_float fTimeDelta) override;
#ifdef _DEBUG
	virtual HRESULT Render() override;
#endif

public:
	HRESULT Ready_Layer_UIGroup_Loading(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Mouse(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Fade(const _tchar* pLayerTag);

private:
	LEVELID							m_eNextLevelID = { LEVEL_END };
	class CLoader*					m_pLoader = { nullptr };
	_uint							m_iLoadingeNum = {}; // �׽�Ʈ �뵵�� �߰�

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID, _uint iLoadingNum,_bool bCheck);
	virtual void Free() override;
};

END
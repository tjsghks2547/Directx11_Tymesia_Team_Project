#pragma once
#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_QuestBackground final : public CUI_Image
{
private:
	CUI_QuestBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_QuestBackground(const CUI_QuestBackground& Prototype);
	virtual ~CUI_QuestBackground() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_MonsterCount(_int iCount) { m_iMonsterCount = iCount; }
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float3 m_fTextPosition = {};

	_int m_iMonsterCount = {};
	_wstring m_strContentText2 = {};
public:
	HRESULT Ready_Components();

public:
	static CUI_QuestBackground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_SquareFrame final : public CUI_Image
{
private:
	CUI_SquareFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SquareFrame(const CUI_SquareFrame& Prototype);
	virtual ~CUI_SquareFrame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
	_bool Get_Mouse_Side_Select_OnOff() { return m_bMouseSideSelectOn; }


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


	_bool m_bMouseSideSelectOn = { false };

public:
	HRESULT Ready_Components();

public:
	static CUI_SquareFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

#include "Client_Defines.h"
#include "UI_Image.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_LoadingIcon final : public CUI_Image
{
private:
	CUI_LoadingIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_LoadingIcon(const CUI_LoadingIcon& Prototype);
	virtual ~CUI_LoadingIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_CurrentTime(_float fTime) { m_fCurrentTime = fTime; }

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


private:
	_bool m_bImageLoopOn = { false };
	_float m_fCurrentTime = { 0 };

public:
	HRESULT Ready_Components();

public:
	static CUI_LoadingIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
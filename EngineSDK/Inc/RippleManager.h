#pragma once
#include "Base.h"
//#include "GameObject.h"
#include "UIObject.h"

BEGIN(Engine)
class CRippleManager final : public CBase
{

private:
    CRippleManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual ~CRippleManager() = default;

public:
    HRESULT                                 Initialize();
    void                                    Update(_float _fTimeDelta);

    void			                        Add_RippleInfo(_float2 vPos, _float fRippleRange);
    void				                    Set_WaterPos(_float2 vWaterPos) { m_vWaterPos = vWaterPos; }
    void				                    Initialize_IsInWater(_bool _bInWater) { m_bIsInWater = _bInWater; }
    void				                    Set_bInWater() { m_bIsInWater = !m_bIsInWater; }

    HRESULT                                 Bind_RippleSRV(class CShader* pShader);

    _bool                                   IsInWater() { return m_bIsInWater; }
private:
    vector<RippleInfo>                      m_RippleInfos;
    _float2                                 m_vWaterPos = { -1000.f, -1000.f };
    _bool                                   m_bIsInWater = { false };


    ID3D11Buffer*                           m_pRippleBuffer = { nullptr };
    ID3D11ShaderResourceView*               m_pRippleSRV = { nullptr };
private:
    ID3D11Device*                           m_pDevice = { nullptr };
    ID3D11DeviceContext*                    m_pContext = { nullptr };

public:
    static CRippleManager*                   Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual void                            Free() override;
};
END

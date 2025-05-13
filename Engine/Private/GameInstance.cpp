#include "..\Public\GameInstance.h"


#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Graphic_Device.h"
#include "Prototype_Manager.h"
#include "Target_Manager.h"
#include "CollisionMgr.h"	
#include "EventMgr.h"
#include "EffectMgr.h"
#include "SoundMgr.h"	
#include "Font_Manager.h"
#include "Frustum.h"
#include "ItemMgr.h"
#include "Shadow.h"
#include "Monster_Manager.h"
#include "Projectile_Manager.h"
#include "GameObject.h"
#include "PhysX_Manager.h"
#include "TriggerManager.h"
#include "RippleManager.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC & EngineDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	m_iViewportWidth = EngineDesc.iViewportWidth;
	m_iViewportHeight = EngineDesc.iViewportHeight;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iViewportWidth, EngineDesc.iViewportHeight, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);	
	if (nullptr == m_pTarget_Manager)	
		return E_FAIL;	

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;


	m_pEvent_Manager = CEventMgr::Create(*ppDevice, *ppContext);
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	m_pEffect_Manager = CEffectMgr::Create();		
	if (nullptr == m_pEffect_Manager)	
		return E_FAIL;	

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pSound_Manager = CSoundMgr::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL; 
	

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	m_pItemMgr = CItemMgr::Create();	
	if (nullptr == m_pItemMgr)	
		return E_FAIL;	

	m_pShadow = CShadow::Create(*ppDevice, *ppContext);	
	if (nullptr == m_pShadow)	
		return E_FAIL;	

	m_pTrigger_Manager = CTriggerManager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTrigger_Manager)
		return E_FAIL;

	m_pMonster_Manager = CMonster_Manager::Create();
	if (nullptr == m_pMonster_Manager)
		return E_FAIL;

	m_pProjectile_Manager = CProjectile_Manager::Create();
	if (nullptr == m_pProjectile_Manager)
		return E_FAIL;

	m_pUI_Manager = CUI_Manager::Create(EngineDesc.iNumUIScenes);
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pPhysX_Manager = CPhysX_Manager::Create(*ppDevice, *ppContext);	
	if (nullptr == m_pPhysX_Manager)	
		return E_FAIL;	

	m_pRipple_Manager = CRippleManager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRipple_Manager)
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager)
		return;
	/* 내 게임내에 필요한  다수 객체의 갱신작업을 모두 모아서 수행을 할거다. */

	m_pInput_Device->Update_InputDev();

	m_pLevel_Manager->Update(fTimeDelta);

	m_pPipeLine->Priority_Update();	

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pMonster_Manager->Priority_Update(fTimeDelta);
	m_pProjectile_Manager->Priority_Update(fTimeDelta);
	m_pEffect_Manager->Priority_Update(fTimeDelta);
	m_pUI_Manager->Priority_Update(fTimeDelta);

	m_pPipeLine->Update();

	m_pFrustum->Update();
	m_pTrigger_Manager->Update(fTimeDelta);

	m_pObject_Manager->Update(fTimeDelta);
	m_pMonster_Manager->Update(fTimeDelta);
	m_pProjectile_Manager->Update(fTimeDelta);
	m_pEffect_Manager->Update(fTimeDelta);
	m_pUI_Manager->Update(fTimeDelta);
	m_pLight_Manager->Update(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);
	m_pMonster_Manager->Late_Update(fTimeDelta);
	m_pProjectile_Manager->Late_Update(fTimeDelta);
	m_pEffect_Manager->Late_Update(fTimeDelta);	
	m_pUI_Manager->Late_Update(fTimeDelta);

	m_pPhysX_Manager->Update(fTimeDelta);	// PhysX 충돌 매니저 

	m_pEvent_Manager->Update();   //  객체의 삭제와 생성에 대한 매니저. 
	m_pShadow->Update();	      //  그림자를 비출 광원의 위치 업데이트를 위한 것.  ( 해당 기능은 거의 필요 없을거로 보임 ) 
	m_pRipple_Manager->Update(fTimeDelta);

	m_pSound_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Render_Begin(const _float4& vClearColor)
{
	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
 	m_pRenderer->Render();

//#ifdef _DEBUG
	if (this->isKeyEnter(DIK_O))
		m_bPhysicsOnOff = !m_bPhysicsOnOff;	

	if(m_bPhysicsOnOff)
		m_pPhysX_Manager->Render_PhysXDebugger();		
//#endif // _DEBUG

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	m_pGraphic_Device->Present();

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* 삭제된 레벨용 원형객체를 클리어한다. */
	m_pPrototype_Manager->Clear(iLevelIndex);

	/* 삭제된 레벨용 사봔객체를 들고있는 레이어들을 클리어한다. */
	m_pObject_Manager->Clear(iLevelIndex);
}

#pragma region TIMER_MANAGER
HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();	
}
_float CGameInstance::Get_TimeDelta(const _wstring & strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring & strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring & strTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Open_Level(_uint iNewLevelIndex, CLevel * pNewLevel)
{
	return m_pLevel_Manager->Open_Level(iNewLevelIndex, pNewLevel);	
}

void CGameInstance::Set_NextLevel_Open(_bool bOpen, _uint _iNumberLevel) //0323 Level전환 임시로 추가함
{
	return m_pLevel_Manager->Set_NextLevel_Open(bOpen, _iNumberLevel);
}

_uint CGameInstance::Get_Current_Level_Index()
{
	return m_pLevel_Manager->Get_Current_Level_Index();
}

#pragma endregion

#pragma region PROTOTYPE_MANAGER
HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CBase * pPrototype)
{

	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}
CBase * CGameInstance::Clone_Prototype(PROTOTYPE eType, _uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(eType, iLevelIndex, strPrototypeTag, pArg);
}

#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, _uint iLevelIndex, const _wstring & strLayerTag, void * pArg, _char* pName)
{
	return m_pObject_Manager->Add_GameObject_To_Layer(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg, pName);
}

CGameObject* CGameInstance::Add_GameObject_To_Layer_Take(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg, _char* pName)
{
	return m_pObject_Manager->Add_GameObject_To_Layer_Take(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg, pName);
}

HRESULT CGameInstance::Sub_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* _pGameObject)
{
	return m_pObject_Manager->Sub_GameObject_To_Layer(iLevelIndex, strLayerTag, _pGameObject);
}

CGameObject* CGameInstance::Get_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _char* ObjectName)
{
	return m_pObject_Manager->Get_GameObject_To_Layer(iLevelIndex, strLayerTag, ObjectName);	
}

list<class CGameObject*>* CGameInstance::Get_LayerGameObject(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Get_LayerGameObject(iLevelIndex, strLayerTag);
}

CComponent* CGameInstance::Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName)
{
	return m_pObject_Manager->Find_Component(iLevelIndex, strLayerTag, strComponentName);
}

map<const _wstring, class CLayer*>* CGameInstance::Get_Layers()
{
	return m_pObject_Manager->Get_Layers();
}

_uint CGameInstance::Get_NumLevel()
{
	return m_pObject_Manager->Get_NumLevel();	
}

HRESULT CGameInstance::UIGroup_Render_OnOff(_uint iLevelIndex, const _wstring& strLayerTag, _bool bCheck)
{
	return m_pObject_Manager->UIGroup_Render_OnOff(iLevelIndex, strLayerTag, bCheck);
}



_bool CGameInstance::UIGroup_Render_State(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->UIGroup_Render_State(iLevelIndex, strLayerTag);
}

HRESULT CGameInstance::Delete_GameObject_From_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Delete_GameObject_From_Layer(iLevelIndex, strLayerTag);
}
#pragma endregion


#pragma region LEVEL_MANAGER
HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroupID, CGameObject * pGameObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroupID, pGameObject);
}

void CGameInstance::Set_MotionBlur(_bool _bOnOff)
{
	return m_pRenderer->Set_MotionBlur(_bOnOff);
}

void CGameInstance::Set_Dithering(_bool _OnOff)
{
	return m_pRenderer->Set_Dithering(_OnOff);
}

_bool* CGameInstance::Get_Dithering_Ptr()
{
	return m_pRenderer->Get_Dithering_Ptr();
}

void CGameInstance::Set_ZoomBlur_Option(_bool _bOnOff, _float _fStrength)
{
	return m_pRenderer->Set_ZoomBlur_Option(_bOnOff, _fStrength);	
}

void CGameInstance::Set_LightShaftValue(_float4 _vLightShaftValue)
{
	return m_pRenderer->Set_LightShaftValue(_vLightShaftValue);
}

void CGameInstance::Set_FogFactors(FOGPARAMS _ParamDesc)
{
	return m_pRenderer->Set_FogFactors(_ParamDesc);
}

#ifdef _DEBUG
#endif // _DEBUG
void CGameInstance::Set_FogOnOff()
{
	m_pRenderer->Set_FogOnOff();
}

void CGameInstance::Set_GodRayOnOff()
{
	m_pRenderer->Set_GodRayOnOff();
}

void CGameInstance::Set_ShadowOnOff()
{
	m_pRenderer->Set_ShadowOnOff();
}

void CGameInstance::Set_ReverseScreenEffect(_bool _OnOff)
{
	m_pRenderer->Set_ReverseScreenEffect(_OnOff);
}

void CGameInstance::Set_ReverseScreenRadius(_float _Radius)
{
	m_pRenderer->Set_ReverseScreenRadius(_Radius);
}

void CGameInstance::Set_ReverseEnd(_bool _OnOff, _float _ReverseEndStrength)
{
	m_pRenderer->Set_ReverseEnd(_OnOff, _ReverseEndStrength);	
}

_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

_float4x4 CGameInstance::Get_Transform_Float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4_Inverse(eState);
}

_matrix CGameInstance::Get_Transform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Matrix_Inverse(eState);
}

_float4 CGameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	return m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

void CGameInstance::Set_Reflection_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	return m_pPipeLine->Set_Reflection_Transform(eState, TransformMatrix);
}

_float4x4 CGameInstance::Get_PreTransform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_PreTransform_Float4x4(eState);
}

_matrix CGameInstance::Get_PreTransform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_PreTransform_Matrix(eState);
}

_float4x4 CGameInstance::Get_PreTransform_Float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_PreTransform_Float4x4_Inverse(eState);
}

_matrix CGameInstance::Get_PreTransform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_PreTransform_Matrix_Inverse(eState);
}

void CGameInstance::Set_Zoom_Blur_Center(_float2 _fPos)
{
	return m_pPipeLine->Set_Zoom_Blur_Center(_fPos);	
}

_float2 CGameInstance::Get_Zoom_Blur_Center()
{
	return  m_pPipeLine->Get_Zoom_Blur_Center();	
}

_float4x4 CGameInstance::Get_Reflection_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return  m_pPipeLine->Get_Reflection_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Reflection_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Reflection_Transform_Matrix(eState);
}

_float4x4 CGameInstance::Get_Reflection_Transform_Float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Reflection_Transform_Float4x4_Inverse(eState);
}

_matrix CGameInstance::Get_Reflection_Transform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Reflection_Transform_Matrix_Inverse(eState);
}

_float4 CGameInstance::Get_Reflection_CamPosition() const
{
	return m_pPipeLine->Get_Reflection_CamPosition();
}

#pragma endregion

#pragma region INPUT_DEVICE

_byte	CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}
_byte	CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}
_long	CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

_bool CGameInstance::isKeyEnter(_int _iKey)
{
	return m_pInput_Device->isKeyEnter(_iKey);
}

_bool CGameInstance::isKeyPressed(_int _iKey)
{
	return m_pInput_Device->isKeyPressed(_iKey);	
}

_bool CGameInstance::isAnyEnter()
{
	return m_pInput_Device->isAnyEnter();
}

_bool CGameInstance::isKeyReleased(_int _iKey)
{
	return m_pInput_Device->isKeyReleased(_iKey);
}


_bool CGameInstance::isMouseEnter(_uint _iKey)
{
	return m_pInput_Device->isMouseEnter(_iKey);	
}	

void CGameInstance::Push_MouseState(MOUSEKEYSTATE eMouseKeyState)
{
	m_pInput_Device->Push_MouseState(eMouseKeyState);	
}
_bool CGameInstance::isMousePressed(_uint _iKey)
{
	return m_pInput_Device->isMousePressed(_iKey);	
}
_bool CGameInstance::isMouseRelease(_uint _iKey)
{
	return m_pInput_Device->isMouseRelease(_iKey);	
}

#pragma endregion

#pragma region LIGHT_MANAGER

const LIGHT_DESC* CGameInstance::Get_LightDesc(_uint iIndex) const
{
	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc, class CTransform* pTransform)
{
	return m_pLight_Manager->Add_Light(LightDesc, pTransform);
}
HRESULT CGameInstance::Add_Light_Range(const LIGHT_DESC& LightDesc, _float fMAXRange, _float fRangeSpeed)
{
	return m_pLight_Manager->Add_Light_Range(LightDesc, fMAXRange, fRangeSpeed);
}
HRESULT CGameInstance::Add_Light_MINRange(const LIGHT_DESC& LightDesc, _float fMINRange, _float fRangeSpeed)
{
	return m_pLight_Manager->Add_Light_Range_MINUS(LightDesc, fMINRange, fRangeSpeed);
}
void CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render_Lights(pShader, pVIBuffer);
}
HRESULT CGameInstance::Delete_Dynamic_Light(CTransform* pTransform)
{
	return m_pLight_Manager->Delete_Dynamic_Light(pTransform);
}
HRESULT CGameInstance::Delete_Static_Light(_uint iCurrentLevel)
{
	return m_pLight_Manager->Delete_Static_Light(iCurrentLevel);
}
HRESULT CGameInstance::Delete_Light_Type(LIGHT_DESC::TYPE _eType)
{
	return m_pLight_Manager->Delete_Light_Type(_eType);
}
#pragma endregion


#pragma region TARGET_MANAGER
HRESULT CGameInstance::Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strRenderTargetTag,iWidth,iHeight,ePixelFormat,vClearColor);
}

HRESULT CGameInstance::Bind_RT_ShaderResource(const _wstring& strRenderTargetTag, CShader* pShader, const _char* pConstantName)
{
	return m_pTarget_Manager->Bind_RT_ShaderResource(strRenderTargetTag,pShader,pConstantName);	
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strRenderTargetTag);	
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag, _bool isClear, ID3D11DepthStencilView* pDSV)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag,isClear,pDSV);	
}

HRESULT CGameInstance::End_MRT(ID3D11DepthStencilView* _pDSV)
{
	return m_pTarget_Manager->End_MRT(_pDSV);
}

HRESULT CGameInstance::Add_Shadow_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor, _uint iArraySize)
{
	return m_pTarget_Manager->Add_Shadow_RenderTarget(strRenderTargetTag, iWidth, iHeight, ePixelFormat , vClearColor, iArraySize);
}

HRESULT CGameInstance::Add_UAV_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return  m_pTarget_Manager->Add_UAV_RenderTarget(strRenderTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::RTV_Compute_LightShaft(const _wstring& strRenderTargetTagToRead, const _wstring& strRenderTargetTagToWrite, CShader_Compute_Deferred* pCompute_Shader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, void* pArg)
{
	return m_pTarget_Manager->RTV_Compute_LightShaft(strRenderTargetTagToRead, strRenderTargetTagToWrite, pCompute_Shader, _iThreadCountX, _iThreadCountY, _iThreadCountZ, pArg);
}

HRESULT CGameInstance::RTV_Compute_Fog(const _wstring& strRenderTargetTagDepth, ID3D11ShaderResourceView* pNoiseSRV, const _wstring& strRenderTargetTagGodRay, const _wstring& strRenderTargetTagFinal, const _wstring& strRenderTargetRangeFogFinal, const _wstring& strRenderTargetTagFog,
	CShader_Compute_Deferred* pCompute_Shader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, void* pArg)
{
	return m_pTarget_Manager->RTV_Compute_Fog(strRenderTargetTagDepth, pNoiseSRV, strRenderTargetTagGodRay, strRenderTargetTagFinal, strRenderTargetRangeFogFinal, strRenderTargetTagFog, pCompute_Shader, _iThreadCountX, _iThreadCountY, _iThreadCountZ, pArg);
}

HRESULT CGameInstance::Clear_RTV(const _wstring& strRenderTargetTag)
{
	return m_pTarget_Manager->Clear_RTV(strRenderTargetTag);
}

HRESULT CGameInstance::Copy_RTV(const _wstring& strRenderTargetTag, const _wstring& strRenderTargetTagToCopy)
{
	return m_pTarget_Manager->Copy_RTV(strRenderTargetTag, strRenderTargetTagToCopy);
}

#ifdef _DEBUG
HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_RT_Debug(strRenderTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_RT_Debug(strMRTTag, pShader, pVIBuffer);
}
#endif // _DEBUG






#pragma region EventMgr

HRESULT CGameInstance::Event_Activate(_wstring _EventName)
{
	return m_pEvent_Manager->Event_Activate(_EventName);
}
HRESULT CGameInstance::Add_EventObject(_wstring _EventName, CGameObject* pGaemObject)
{
	return m_pEvent_Manager->Add_EventObject(_EventName,pGaemObject);	
}
HRESULT CGameInstance::Sub_EventObject(_wstring _EventName, CGameObject* pGaemObject)
{
	return m_pEvent_Manager->Sub_EventObject(_EventName, pGaemObject);		
}
HRESULT CGameInstance::Add_DeadObject(_wstring _LayerName, CGameObject* pGaemObject)	
{
	return m_pEvent_Manager->Add_DeadObject(_LayerName, pGaemObject);
}

HRESULT CGameInstance::Add_DeadObjects(_wstring _LayerName, CGameObject* pGaemObject, _uint iCurrentLevel)
{
	return m_pEvent_Manager->Add_DeadObjects(_LayerName, pGaemObject, iCurrentLevel);
}

HRESULT CGameInstance::Add_DeadEffect(CGameObject* pGameObject)
{
	return m_pEvent_Manager->Add_DeadEffect(pGameObject);	
}

void CGameInstance::Set_Level_ForEventManager(_uint _iCurLevel)
{
	m_pEvent_Manager->Set_Level(_iCurLevel);
}

#pragma endregion

#pragma region Effect Manager



HRESULT CGameInstance::Add_Effect(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, EFFECT_NAME _eEffectName, void* _pArg)
{
	return m_pEffect_Manager->Add_Effect(_iPrototypeLevelIndex, _strPrototypeTag, _eEffectName, _pArg);
}

HRESULT CGameInstance::Play_Effect(EFFECT_NAME _eEffectName, _fvector _vPos)
{
	return m_pEffect_Manager->Play_Effect(_eEffectName, _vPos);
}

HRESULT CGameInstance::Play_Effect_With_Timer(EFFECT_NAME _eEffectName, _float _fDuration, _fvector _vPos)
{
	return m_pEffect_Manager->Play_Effect_With_Timer(_eEffectName, _fDuration, _vPos);
}

HRESULT CGameInstance::Play_Effect_Matrix(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix)
{
	return m_pEffect_Manager->Play_Effect_Matrix(_eEffectName, _pMatrix);
}

HRESULT CGameInstance::Play_Effect_Speed_Matrix(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float* _pAnimation_Speed)
{
	return m_pEffect_Manager->Play_Effect_Speed_Matrix(_eEffectName, _pMatrix, _pAnimation_Speed);
}

HRESULT CGameInstance::Play_Effect_With_Timer_Matrix(EFFECT_NAME _eEffectName, _float _fDuration, const _float4x4* _pMatrix)
{
	return m_pEffect_Manager->Play_Effect_With_Timer_Matrix(_eEffectName, _fDuration, _pMatrix);
}

HRESULT CGameInstance::Play_Effect_Dir(EFFECT_NAME _eEffectName, _fvector _vPos, _fvector _vDir)
{
	return m_pEffect_Manager->Play_Effect_Dir(_eEffectName, _vPos, _vDir);
}

HRESULT CGameInstance::Play_Effect_Matrix_Dir(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, _fvector _vDir)
{
	return m_pEffect_Manager->Play_Effect_Matrix_Dir(_eEffectName, _pMatrix, _vDir);
}

HRESULT CGameInstance::Stop_Effect(EFFECT_NAME _eEffectName)
{
	return m_pEffect_Manager->Stop_Effect(_eEffectName);
}

HRESULT CGameInstance::Reset_Effect()
{
	return m_pEffect_Manager->Reset_Effect();
}

HRESULT CGameInstance::Play_Effect_Matrix_OneMoment(EFFECT_NAME _eEffectName, _float4x4 _matMatrix)
{
	return m_pEffect_Manager->Play_Effect_Matrix_OneMoment(_eEffectName, _matMatrix);
}

HRESULT CGameInstance::Play_Effect_Matrix_With_Socket(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float4x4* _pSocketMatrix)
{
	return m_pEffect_Manager->Play_Effect_Matrix_With_Socket(_eEffectName, _pMatrix, _pSocketMatrix);
}

HRESULT CGameInstance::Play_Effect_Matrix_With_Socket_Dir(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float4x4* _pSocketMatrix, _fvector _vDir)
{
	return m_pEffect_Manager->Play_Effect_Matrix_With_Socket_Dir(_eEffectName, _pMatrix, _pSocketMatrix, _vDir);
}

#pragma endregion 

#pragma region Sound Manager 

void CGameInstance::Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume)
{
	m_pSound_Manager->Play_Sound(pSoundKey, eID, fVolume);
}

void CGameInstance::PlayBGM(const _tchar* pSoundKey, float fVolume)
{
	m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::StopSlowly(CHANNELID eID, _float fMaxTime, _float fCurVolume)
{
	m_pSound_Manager->StopSlowly(eID, fMaxTime, fCurVolume);
}

void CGameInstance::StopSound(CHANNELID eID)
{
	m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	m_pSound_Manager->StopAll();
}

void CGameInstance::SetChannelVolume(CHANNELID eID, float fVolume)
{
	m_pSound_Manager->SetChannelVolume(eID, fVolume);
}
bool CGameInstance::CheckSoundPlaying(CHANNELID eID)
{
	return m_pSound_Manager->CheckSoundPlaying(eID);
}
#pragma endregion 
#pragma region FONT_MANAGER
HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}
HRESULT CGameInstance::Render_Font(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale, layerDepth, effects);
}
HRESULT CGameInstance::Render_World(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render_World(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale, layerDepth, effects);
}
HRESULT CGameInstance::Render_Shadow(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render_Shadow(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale, layerDepth, effects);
}
HRESULT CGameInstance::Render_Outline(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render_Outline(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale, layerDepth, effects);
}
HRESULT CGameInstance::Render_Color(const wstring& strFontTag, const _tchar* pText1, const _tchar* pText2, const _float2& vPosition, _float4 vColor1, _float4 vColor2, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render_Color(strFontTag, pText1, pText2, vPosition, vColor1, vColor2, fRotation, vOrigin, fScale, layerDepth, effects);
}
HRESULT CGameInstance::Render_Alpha(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render_Alpha(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale, layerDepth, effects);
}
_float2 CGameInstance::Get_TextSize(const _wstring& strFontTag, const _tchar* pText)
{
	return m_pFont_Manager->Get_TextSize(strFontTag, pText);
}
_bool CGameInstance::isIn_Frustum_WorldSpace(_fvector vWorldPoint, _float fRange, FRUSTUM_TYPE _eType)
{
	return m_pFrustum->isIn_WorldSpace(vWorldPoint, fRange, _eType);
}
_bool CGameInstance::isAABB_InFrustum(const XMFLOAT3& _fMin, const XMFLOAT3& _fMax)
{
	return m_pFrustum->isIn_AABB_Box(_fMin, _fMax);
}
#pragma endregion

#pragma region ITEM_MANAGER
HRESULT CGameInstance::Add_Item(ITEM_TYPE _eItemType, _uint _iItemCount, CItem* _pGameObject, _bool _bTaken)
{
	return m_pItemMgr->Add_Item(_eItemType, _iItemCount, _pGameObject, _bTaken);
}
HRESULT CGameInstance::Drop_Item(ITEM_TYPE _eItemType, _fvector _vDropPosition, CGameObject* _pGameObject, _uint _iDropItemCount)
{
	return m_pItemMgr->Drop_Item(_eItemType, _vDropPosition, _pGameObject, _iDropItemCount);
}
HRESULT CGameInstance::Pop_Item(ITEM_TYPE _eItemType, _fvector _vPopPosition, CGameObject* _pGameObject, _uint _iPopItemCount)
{
	return m_pItemMgr->Pop_Item(_eItemType, _vPopPosition, _pGameObject, _iPopItemCount);
}
HRESULT CGameInstance::Acquire_Item(ITEM_TYPE _eItemType)
{
	return m_pItemMgr->Acquire_Item(_eItemType);
}
void CGameInstance::Clear_ItemInfo()
{
	m_pItemMgr->Clear_ItemInfo();
}
_uint CGameInstance::Get_Item_Count(ITEM_TYPE _eItemType)
{
	return m_pItemMgr->Get_Item_Count(_eItemType);
}
list<ITEM_TYPE>& CGameInstance::Get_Item_Save_Info()
{
	return m_pItemMgr->Get_Item_Save_Info();
}
list<ITEM_TYPE>& CGameInstance::Get_Item_Drop_Info()
{
	return m_pItemMgr->Get_Item_Drop_Info();
}
const map<ITEM_TYPE, pair<_uint, vector<CItem*>>> CGameInstance::Get_Item_Info()
{
	return m_pItemMgr->Get_Item_Info();
}
_bool CGameInstance::Use_Item(ITEM_TYPE _eItemType, _uint iNum)
{
	return m_pItemMgr->Use_Item(_eItemType, iNum);
}
#pragma endregion

#pragma region SHADOW
HRESULT CGameInstance::SetUp_ShadowLight(_fvector vEye, _fvector vAt, _float fLightAngle, _float fAspect, _float fNear, _float fFar, _matrix matInvCam, _fvector vCamInfo, CTransform* pTransform)
{
	return m_pShadow->SetUp_ShadowLight(vEye, vAt, fLightAngle, fAspect, fNear, fFar, matInvCam, vCamInfo, pTransform);

}
HRESULT CGameInstance::Bind_Shadow_Matrices(CShader* pShader, const _char* pViewConstantName, const _char* pProjConstantName)
{
	if (FAILED(m_pShadow->Bind_ViewMatrix(pShader, pViewConstantName)))
		return E_FAIL;
	if (FAILED(m_pShadow->Bind_ProjMatrix(pShader, pProjConstantName)))
		return E_FAIL;

	return S_OK;
}
HRESULT CGameInstance::Bind_LightZ(CShader* pShader)
{
	return m_pShadow->Bind_LightZ(pShader);
}
HRESULT CGameInstance::Bind_LightPos(CShader* pShader, const _char* pConstantName)
{
	return m_pShadow->Bind_LightPos(pShader, pConstantName);
}
HRESULT CGameInstance::Bind_LightDir(CShader* pShader, const _char* pConstantName)
{
	return m_pShadow->Bind_LightDir(pShader, pConstantName);
}
HRESULT CGameInstance::Bind_LightProjDir(CShader* pShader, const _char* pConstantName)
{
	return m_pShadow->Bind_LightDir(pShader, pConstantName);
}
_float2 CGameInstance::Get_LightPos()
{
	return m_pShadow->Get_LightPos();
}
HRESULT CGameInstance::Delete_Shadow(CTransform* pTransform)
{
	return m_pShadow->Delete_Shadow(pTransform);
}
#pragma endregion

#pragma region UI_Manager

HRESULT CGameInstance::Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg)
{
	return m_pUI_Manager->Add_UIObject_To_UIScene(iPrototypeLevelIndex, strPrototypeTag, iSceneIndex, strSceneTag, iUIType, pArg);
}

CUI_Scene* CGameInstance::Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag)
{
	return m_pUI_Manager->Find_UIScene(iSceneIndex, strSceneTag);
}

map<const _wstring, class CUI_Scene*>* CGameInstance::Find_UIScene_ALL()
{
	return m_pUI_Manager->Find_UIScene_ALL();
}

HRESULT CGameInstance::UIScene_UIObject_Render_OnOff(CUI_Scene* pScene, _bool bOpen)
{
	return m_pUI_Manager->UIScene_UIObject_Render_OnOff(pScene, bOpen);
}

_bool CGameInstance::Get_Scene_Render_State(CUI_Scene* pScene)
{
	return m_pUI_Manager->Get_Scene_Render_State(pScene);
}

HRESULT CGameInstance::Set_All_UIObject_Condition_Open(CUI_Scene* pScene, _bool bOpen)
{
	return m_pUI_Manager->Set_All_UIObject_Condition_Open(pScene, bOpen);
}

void CGameInstance::Find_TextBox_PlayerScreen(CUI_Scene* pScene, _uint iGroupID, _uint iCount)
{
	return m_pUI_Manager->Find_TextBox_PlayerScreen(pScene, iGroupID, iCount);
}

void CGameInstance::UIScene_Clear(_uint iScenelIndex)
{
	return m_pUI_Manager->Clear(iScenelIndex);
}

const CUI_Manager::PLAYER_SAVE_STATE& CGameInstance::Get_Player_State_SaveData()
{
	return m_pUI_Manager->Get_Player_State_SaveData();
}

void CGameInstance::Set_Player_State_SaveData(const CUI_Manager::PLAYER_SAVE_STATE& PlayerData)
{
	return m_pUI_Manager->Set_Player_State_SaveData(PlayerData);
}

const CUI_Manager::PLAYER_SAVE_TALENT& CGameInstance::Get_Player_Talent_SaveData()
{
	return m_pUI_Manager->Get_Player_Talent_SaveData();
}

void CGameInstance::Set_Player_Talent_SaveData(const CUI_Manager::PLAYER_SAVE_TALENT& PlayerData)
{
	return m_pUI_Manager->Set_Player_Talent_SaveData(PlayerData);
}

const CUI_Manager::UI_SAVE_TALENT& CGameInstance::Get_UI_Talent_SaveData()
{
	return m_pUI_Manager->Get_UI_Talent_SaveData();
}

void CGameInstance::Set_UI_Talent_SaveData(const CUI_Manager::UI_SAVE_TALENT& PlayerData)
{
	return m_pUI_Manager->Set_UI_Talent_SaveData(PlayerData);
}

const CUI_Manager::UI_SAVE_SKILL& CGameInstance::Get_UI_Skill_SaveData()
{
	return m_pUI_Manager->Get_UI_Skill_SaveData();
}

void CGameInstance::Set_UI_Skill_SaveData(const CUI_Manager::UI_SAVE_SKILL& PlayerData)
{
	return m_pUI_Manager->Set_UI_Skill_SaveData(PlayerData);
}

void CGameInstance::Set_Condition(CUI_Scene* pScene, _int iCondition1, _int iCondition2)
{
	return m_pUI_Manager->Set_Condition(pScene, iCondition1, iCondition2);
}

_int CGameInstance::Get_Condition(CUI_Scene* pScene, _uint iConditionNum)
{
	return m_pUI_Manager->Get_Condition(pScene, iConditionNum);
}

HRESULT CGameInstance::LoadDataFile_UIObj_Info(HWND hWnd, _uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
{
	return m_pUI_Manager->LoadDataFile_UIObj_Info(hWnd, iLevelIndex, iSceneIndex, szSceneName);
}

HRESULT CGameInstance::LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut)
{
	return m_pUI_Manager->LoadDataFile_UIText_Info(hWnd, szSceneName, pOut);
}
#pragma endregion UI_Manager

#pragma region PhysX_Manager
PxRigidDynamic* CGameInstance::Create_Actor(COLLIDER_TYPE _eType, _float3 _Scale, _float3 _Axis, _float _degree, CGameObject* _pGameObject)
{
	return m_pPhysX_Manager->Create_Actor(_eType, _Scale, _Axis, _degree, _pGameObject);
}

HRESULT CGameInstance::IsActorInScene(PxRigidDynamic* pActor)
{
	return m_pPhysX_Manager->IsActorInScene(pActor);
}

HRESULT CGameInstance::Add_Actor_Scene(PxRigidDynamic* pActor)
{
	return m_pPhysX_Manager->Add_Actor_Scene(pActor);	
}

HRESULT CGameInstance::Sub_Actor_Scene(PxRigidDynamic* pActor)
{
	return m_pPhysX_Manager->Sub_Actor_Scene(pActor);	
}

HRESULT CGameInstance::Update_Collider(PxRigidDynamic* Actor, _matrix _WorldMatrix, _vector _vOffSet)
{
	return m_pPhysX_Manager->Update_Collider(Actor, _WorldMatrix, _vOffSet);
}

HRESULT CGameInstance::Set_GlobalPos(PxRigidDynamic* Actor, _vector _fPosition)
{
	return m_pPhysX_Manager->Set_GlobalPos(Actor, _fPosition);
}

HRESULT CGameInstance::Set_CollisionGroup(PxRigidDynamic* pActor, GROUP_TYPE _eMeType, PxU32 _ColliderGroup)
{
	return m_pPhysX_Manager->Set_CollisionGroup(pActor, _eMeType, _ColliderGroup);
}

HRESULT CGameInstance::Clear_Scene()
{
	return m_pPhysX_Manager->Clear_Scene();
}
#pragma endregion PhysX_Manager

#pragma region Trigger
HRESULT CGameInstance::Set_BlackScreen(CUIObject* _pBlackScreen)
{
	return m_pTrigger_Manager->Set_BlackScreen(_pBlackScreen);
}
HRESULT CGameInstance::Add_Trigger(TRIGGER_TYPE _eTriggerType, CGameObject* _pTarget)
{
	return m_pTrigger_Manager->Add_Trigger(_eTriggerType, _pTarget);
}
HRESULT CGameInstance::Activate_Fade(TRIGGER_TYPE _eTriggerType, _float _Duration, _float _fDurationSecond)
{
	return m_pTrigger_Manager->Activate_Fade(_eTriggerType, _Duration, _fDurationSecond);
}
_bool CGameInstance::Is_Fade_Complete(TRIGGER_TYPE _eTriggerType)
{
	return m_pTrigger_Manager->Is_Fade_Complete(_eTriggerType);
}
void CGameInstance::Setting_NULL_BlackScreen()
{
	m_pTrigger_Manager->Setting_NULL_BlackScreen();
}
void CGameInstance::Clear_Trigger()
{
	m_pTrigger_Manager->Trigger_Clear();
}
#pragma endregion

#pragma region MONSTER_MANAGER

HRESULT CGameInstance::Add_Monster(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, MONSTER_CATEGORY _eCategory, void* _pArg)
{
	return m_pMonster_Manager->Add_Monster(_iPrototypeLevelIndex, _strPrototypeTag, _eCategory, _pArg);
}

HRESULT CGameInstance::Add_Delete_Monster(CMonster* pMonster)
{
	return m_pMonster_Manager->Add_Delete_Monster(pMonster);
}

HRESULT CGameInstance::Delete_All_Monster()
{
	return m_pMonster_Manager->Delete_All_Monster();
}

HRESULT CGameInstance::Active_Monster()
{
	return m_pMonster_Manager->Active_Monster();
}
HRESULT CGameInstance::Respawn_Monster(MONSTER_CATEGORY _eCategory)
{
	return m_pMonster_Manager->Respawn_Monster(_eCategory);
}
deque<class CMonster*>& CGameInstance::Get_Check_Monsters()
{
	return m_pMonster_Manager->Get_Check_Monsters();
}

_bool CGameInstance::Get_Boss_Dead() const
{
	return m_pMonster_Manager->Get_Boss_Dead();
}

_bool CGameInstance::Get_Boss_Active() const
{
	return m_pMonster_Manager->Get_Boss_Active();
}

void CGameInstance::Set_Boss_Dead(_bool bCheck)
{
	m_pMonster_Manager->Set_Boss_Dead(bCheck);
}

void CGameInstance::Set_Boss_Active(_bool bCheck)
{
	m_pMonster_Manager->Set_Boss_Active(bCheck);
}

#pragma endregion

#pragma region PROJECTILE_MANAGER
HRESULT CGameInstance::Add_Projectile(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, PROJECTILE_CATEGORY _eCategory, void* _pArg)
{
	return m_pProjectile_Manager->Add_Projectile(_iPrototypeLevelIndex, _strPrototypeTag, _eCategory, _pArg);
}
HRESULT CGameInstance::Fire_Multi_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos, _uint iCount, _bool bReverse)
{
	return m_pProjectile_Manager->Fire_Multi_Projectile(_eCategory, vStartPos, vEndPos, iCount, bReverse);
}

HRESULT CGameInstance::Fire_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos)
{
	return m_pProjectile_Manager->Fire_Projectile(_eCategory, vStartPos, vEndPos);
}

#pragma endregion

#pragma region WATER_MANAGER

void CGameInstance::Add_RippleInfo(_float2 vPos, _float fRippleRange)
{
	return m_pRipple_Manager->Add_RippleInfo(vPos, fRippleRange);
}

void CGameInstance::Set_WaterPos(_float2 vWaterPos)
{
	return m_pRipple_Manager->Set_WaterPos(vWaterPos);
}

HRESULT CGameInstance::Bind_RippleSRV(CShader* pShader)
{
	return m_pRipple_Manager->Bind_RippleSRV(pShader);
}

_bool CGameInstance::IsInWater()
{
	return m_pRipple_Manager->IsInWater();
}

void CGameInstance::Initialize_IsInWater(_bool _bInWater)
{
	m_pRipple_Manager->Initialize_IsInWater(_bInWater);
}

void CGameInstance::Set_bInWater()
{
	m_pRipple_Manager->Set_bInWater();
}

#pragma endregion

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pGraphic_Device);	
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);	
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pMonster_Manager);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pEvent_Manager);	
	Safe_Release(m_pEffect_Manager);
	Safe_Release(m_pProjectile_Manager);
	Safe_Release(m_pRipple_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pItemMgr);	
	Safe_Release(m_pShadow);	
	Safe_Release(m_pTrigger_Manager);
	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pPhysX_Manager);	
	m_pSound_Manager->Release();
	m_pSound_Manager->Free();

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	__super::Free();


}

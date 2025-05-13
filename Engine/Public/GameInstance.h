#pragma once

#include "PipeLine.h"
#include "Renderer.h"
#include "Prototype_Manager.h"
#include "UI_Manager.h"

/* 0. 엔진에서 클라이언트 프로젝트에 직접 보여주는 클래스 : CGameInstance, 부모클래스(CBase, CLevel, CGameObject, CComponent..) */
/* 1. 클라이언트 프로젝트에서 엔진프로젝트에 있는 기능을 호출하고자한다라면 무조건 GameInstance를 통해서 호출할 수 있도록 하겠다.  */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	_uint2 Get_ViewportSize() const {
		return _uint2(m_iViewportWidth, m_iViewportHeight);
	}

public:
	/* 클라에서 호출할 만한 엔진의 기능을 위해 필요한 초기화를 수행한다. */
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Begin(const _float4 & vClearColor);
	HRESULT Draw();
	HRESULT Render_End();
	void Clear(_uint iLevelIndex);

	_float Compute_Random_Normal() 
	{
		return static_cast<_float>(rand()) / RAND_MAX;	
	}

	_float Compute_Random(_float fMin, _float fMax)
	{
		return fMin + Compute_Random_Normal() * (fMax - fMin);
	}

	HRESULT Clear_DepthStencil_View();	

#pragma region TIMER_MANAGER
public: /* For.Timer_Manager */
	_float			Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT			Add_Timer(const _wstring& strTimerTag);
	void			Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Open_Level(_uint iNewLevelIndex, class CLevel* pNewLevel);
	void Set_NextLevel_Open(_bool bOpen, _uint _iNumberLevel);
	_uint Get_Current_Level_Index();	
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr, _char* pName = nullptr);	
	CGameObject* Add_GameObject_To_Layer_Take(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr, _char* pName = nullptr);
	HRESULT Sub_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* _pGameObject);
	CGameObject* Get_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _char* ObjectName);	
	list<class CGameObject*>* Get_LayerGameObject(_uint iLevelIndex, const _wstring& strLayerTag);	
	CComponent* Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName);
	map<const _wstring, class CLayer*>* Get_Layers();	
	_uint	Get_NumLevel();


	HRESULT UIGroup_Render_OnOff(_uint iLevelIndex, const _wstring& strLayerTag, _bool bCheck);
	_bool UIGroup_Render_State(_uint iLevelIndex, const _wstring& strLayerTag);


	HRESULT Delete_GameObject_From_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroupID, class CGameObject* pGameObject);
	void    Set_MotionBlur(_bool _bOnOff);	
	void	Set_Dithering(_bool _OnOff);
	_bool*  Get_Dithering_Ptr();
	void	Set_ZoomBlur_Option(_bool _bOnOff, _float _fStrength);
	void	Set_LightShaftValue(_float4 _vLightShaftValue);
	void	Set_FogFactors(FOGPARAMS _ParamDesc);
#ifdef _DEBUG
#endif // _DEBUG
	void	Set_FogOnOff();
	void	Set_GodRayOnOff();
	void	Set_ShadowOnOff();
	void    Set_ReverseScreenEffect(_bool _OnOff);
	void    Set_ReverseScreenRadius(_float _Radius);
	void	Set_ReverseEnd(_bool _OnOff, _float _ReverseEndStrength);
#pragma endregion

#pragma region PIPELINE
	_float4x4 Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4x4 Get_Transform_Float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_Transform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4 Get_CamPosition() const;
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);

	/* 이전 프레임 뷰와 투영 가져오는 함수*/
	_float4x4 Get_PreTransform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_PreTransform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4x4 Get_PreTransform_Float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_PreTransform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState)const;

	void    Set_Zoom_Blur_Center(_float2 _fPos);	
	_float2 Get_Zoom_Blur_Center();	


	_float4x4 Get_Reflection_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_Reflection_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4x4 Get_Reflection_Transform_Float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_Reflection_Transform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4 Get_Reflection_CamPosition() const;
	void Set_Reflection_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
#pragma endregion


#pragma region INPUT_DEVICE
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);	
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

	_bool   isKeyEnter(_int _iKey);
	_bool	isKeyReleased(_int _iKey);	
	_bool   isKeyPressed(_int _iKey);

	_bool	isAnyEnter();	// 아무 키를 눌렀을 때 반응

	_bool   isMouseEnter(_uint _iKey); 
	void    Push_MouseState(MOUSEKEYSTATE eMouseKeyState);
	_bool   isMousePressed(_uint _iKey);	
	_bool   isMouseRelease(_uint _iKey);	
#pragma endregion

#pragma region LIGHT_MANAGER
	const LIGHT_DESC* Get_LightDesc(_uint iIndex) const;
	HRESULT Add_Light(const LIGHT_DESC& LightDesc, class CTransform* pTransform = nullptr);
	HRESULT Add_Light_Range(const LIGHT_DESC& LightDesc, _float fMAXRange, _float fRangeSpeed);
	HRESULT Add_Light_MINRange(const LIGHT_DESC& LightDesc, _float fMINRange, _float fRangeSpeed);
	void Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Delete_Dynamic_Light(class CTransform* pTransform);
	HRESULT Delete_Static_Light(_uint iCurrentLevel);
	HRESULT Delete_Light_Type(LIGHT_DESC::TYPE _eType);
#pragma endregion

#pragma region TARGET_MANAGER
	HRESULT Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);	
	HRESULT Bind_RT_ShaderResource(const _wstring& strRenderTargetTag, CShader* pShader, const _char* pConstantName);	
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag);	
	HRESULT Begin_MRT(const _wstring& strMRTTag, _bool isClear = true, ID3D11DepthStencilView* pDSV = nullptr);
	HRESULT End_MRT(ID3D11DepthStencilView* _pDSV = nullptr);
	HRESULT Add_Shadow_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor, _uint iArraySize);
	HRESULT Add_UAV_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);

	HRESULT RTV_Compute_LightShaft(const _wstring& strRenderTargetTagToRead, const _wstring& strRenderTargetTagToWrite, class CShader_Compute_Deferred* pCompute_Shader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, void* pArg);
	HRESULT RTV_Compute_Fog(const _wstring& strRenderTargetTagDepth, ID3D11ShaderResourceView* pNoiseSRV, const _wstring& strRenderTargetTagGodRay, const _wstring& strRenderTargetTagFinal, const _wstring& strRenderTargetRangeFogFinal, const _wstring& strRenderTargetTagFog, CShader_Compute_Deferred* pCompute_Shader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, void* pArg);
	
	HRESULT Clear_RTV(const _wstring& strRenderTargetTag);

	HRESULT Copy_RTV(const _wstring& strRenderTargetTag, const _wstring& strRenderTargetTagToCopy);
#ifdef _DEBUG
public:
	HRESULT Ready_RT_Debug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif
	
#pragma endregion


#pragma region EVENT_MANAGER
	HRESULT  Event_Activate(_wstring _EventName);	
	HRESULT  Add_EventObject(_wstring _EventName, CGameObject* pGaemObject);		
	HRESULT  Sub_EventObject(_wstring _EventName, CGameObject* pGaemObject);	
	HRESULT  Add_DeadObject(_wstring _LayerName, CGameObject* pGaemObject);	
	HRESULT  Add_DeadObjects(_wstring _LayerName, CGameObject* pGaemObject, _uint iCurrentLevel);
	HRESULT  Add_DeadEffect(CGameObject* pGameObject);	
	void  Set_Level_ForEventManager(_uint _iCurLevel);
#pragma endregion 


#pragma region EFFECT MANAGER

	HRESULT Add_Effect(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, EFFECT_NAME _eEffectName, void* _pArg = nullptr);
	HRESULT Play_Effect(EFFECT_NAME _eEffectName, _fvector _vPos);
	HRESULT Play_Effect_With_Timer(EFFECT_NAME _eEffectName, _float _fDuration, _fvector _vPos);
	HRESULT Play_Effect_Matrix(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix);
	HRESULT Play_Effect_Speed_Matrix(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float* _pAnimation_Speed);
	HRESULT Play_Effect_With_Timer_Matrix(EFFECT_NAME _eEffectName, _float _fDuration, const _float4x4* _pMatrix);
	HRESULT Play_Effect_Dir(EFFECT_NAME _eEffectName, _fvector _vPos, _fvector _vDir);
	HRESULT Play_Effect_Matrix_Dir(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, _fvector _vDir);
	HRESULT Stop_Effect(EFFECT_NAME _eEffectName);
	HRESULT Reset_Effect();
	HRESULT Play_Effect_Matrix_OneMoment(EFFECT_NAME _eEffectName, _float4x4 _matMatrix);
	HRESULT Play_Effect_Matrix_With_Socket(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float4x4* _pSocketMatrix);
	HRESULT Play_Effect_Matrix_With_Socket_Dir(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float4x4* _pSocketMatrix, _fvector _vDir);

#pragma endregion


#pragma region SOUND MANAGER
	void Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const _tchar* pSoundKey, float fVolume);
	void StopSlowly(CHANNELID eID, _float fMaxTime = 1.f, _float fCurVolume = 0.3f);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
	bool CheckSoundPlaying(CHANNELID eID);
#pragma endregion  



#pragma region Font_Manager
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);
	HRESULT Render_World(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_Shadow(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_Outline(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_Color(const wstring& strFontTag, const _tchar* pText1, const _tchar* pText2, const _float2& vPosition,
		_float4 vColor1 = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f },
		_float4 vColor2 = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f },
		_float fRotation = 0.f,const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);
	
	HRESULT Render_Alpha(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	_float2 Get_TextSize(const _wstring& strFontTag, const _tchar* pText);
#pragma endregion


#pragma region FRUSTUM
	_bool isIn_Frustum_WorldSpace(_fvector vWorldPoint, _float fRange = 0.f, FRUSTUM_TYPE _eType = FRUSTUM_TYPE::FRUSTUM_OBJECT);
	_bool isAABB_InFrustum(const XMFLOAT3& _fMin, const XMFLOAT3& _fMax);
#pragma endregion


#pragma region ItemMgr 
	HRESULT		Add_Item(ITEM_TYPE _eItemType, _uint _iItemCount, class CItem* _pGameObject, _bool _bTaken);
	HRESULT		Drop_Item(ITEM_TYPE _eItemType, _fvector _vDropPosition, class CGameObject* _pGameObject, _uint _iDropItemCount = 1);
	HRESULT		Pop_Item(ITEM_TYPE _eItemType, _fvector _vPopPosition, class CGameObject* _pGameObject, _uint _iPopItemCount);
	HRESULT		Acquire_Item(ITEM_TYPE _eItemType);
	void		Clear_ItemInfo();

	_uint Get_Item_Count(ITEM_TYPE _eItemType);

	list<ITEM_TYPE>& Get_Item_Save_Info(); // 현재 아이템을 획득하는 순간 획득 알림을 띄우기 위한 용도
	list<ITEM_TYPE>& Get_Item_Drop_Info(); // 현재 아이템을 버리는 순간 버림 알림을 띄우기 위한 용도

	const map<ITEM_TYPE, pair<_uint, vector<CItem*>>> Get_Item_Info(); // 저장된 아이템 컨테이너 가져오기 - 유빈
	_bool Use_Item(ITEM_TYPE _eItemType, _uint iNum); // 인벤토리에서 아이템 사용 시 컨테이너에 저장하고 있는 아이템 개수 감소 - 유빈
#pragma endregion 

#pragma region SHADOW
	HRESULT SetUp_ShadowLight(_fvector vEye, _fvector vAt, _float fLightAngle, _float fAspect, _float fNear, _float fFar, _matrix matInvCam, _fvector vCamInfo, CTransform* pTransform);
	HRESULT Bind_Shadow_Matrices(class CShader* pShader, const _char* pViewConstantName, const _char* pProjConstantName);
	HRESULT Bind_LightZ(CShader* pShader);
	HRESULT Bind_LightPos(CShader* pShader, const _char* pConstantName);
	HRESULT Bind_LightDir(CShader* pShader, const _char* pConstantName);
	HRESULT Bind_LightProjDir(CShader* pShader, const _char* pConstantName);
	_float2 Get_LightPos();
	HRESULT Delete_Shadow(class CTransform* pTransform);
#pragma endregion

#pragma region UI_Manager
	HRESULT Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg = nullptr);
	class CUI_Scene* Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag);
	map<const _wstring, class CUI_Scene*>* Find_UIScene_ALL();
	
	HRESULT UIScene_UIObject_Render_OnOff(CUI_Scene* pScene, _bool bOpen); // UI 오브젝트 모드 랜더 하면서 => 해당 오브젝트들을 가지고 있는 씬의 랜더 또한 오픈함
	_bool Get_Scene_Render_State(CUI_Scene* pScene); // 씬이 켜져 있는지 체크
	HRESULT Set_All_UIObject_Condition_Open(CUI_Scene* pScene, _bool bOpen);

	void Find_TextBox_PlayerScreen(CUI_Scene* pScene, _uint iGroupID, _uint iCount); // 몬스터 사망 시 수치 가져오는 부분을 위해...일단 추가 

	void UIScene_Clear(_uint iScenelIndex); // UI 모든 씬 지우기

	const CUI_Manager::PLAYER_SAVE_STATE& Get_Player_State_SaveData();// const { return m_PlayerStat; }
	void Set_Player_State_SaveData(const CUI_Manager::PLAYER_SAVE_STATE& PlayerData);// { m_PlayerStat = PlayerData; }

	const CUI_Manager::PLAYER_SAVE_TALENT& Get_Player_Talent_SaveData();// const { return m_PlayerTalent; }
	void Set_Player_Talent_SaveData(const CUI_Manager::PLAYER_SAVE_TALENT& PlayerData);// { m_PlayerTalent = PlayerData; }
	
	const CUI_Manager::UI_SAVE_TALENT& Get_UI_Talent_SaveData();// const { return m_PlayerTalent; }
	void Set_UI_Talent_SaveData(const CUI_Manager::UI_SAVE_TALENT& PlayerData);// { m_PlayerTalent = PlayerData; }

	const CUI_Manager::UI_SAVE_SKILL& Get_UI_Skill_SaveData();// const { return m_UISkill; }
	void Set_UI_Skill_SaveData(const CUI_Manager::UI_SAVE_SKILL& PlayerData);// { m_UISkill = PlayerData; }

	void Set_Condition(CUI_Scene* pScene, _int iCondition1, _int iCondition2);// { m_iCondition1 = iCondition1, m_iCondition2 = iCondition2; }
	_int Get_Condition(CUI_Scene* pScene, _uint iConditionNum);// { return m_iCondition[iConditionNum]; }

	HRESULT LoadDataFile_UIObj_Info(HWND hWnd, _uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut);

#pragma endregion


#pragma region PhysX_Manager
	PxRigidDynamic* Create_Actor(COLLIDER_TYPE _eType, _float3 _Scale, _float3 _Axis,
		_float _degree, CGameObject* _pGameObject);

	HRESULT IsActorInScene(PxRigidDynamic* pActor);	
	HRESULT	Add_Actor_Scene(PxRigidDynamic* pActor);
	HRESULT Sub_Actor_Scene(PxRigidDynamic* pActor);
	HRESULT Update_Collider(PxRigidDynamic* Actor, _matrix _WorldMatrix, _vector _vOffSet);
	HRESULT Set_GlobalPos(PxRigidDynamic* Actor, _vector _fPosition);
	HRESULT Set_CollisionGroup(PxRigidDynamic* pActor, GROUP_TYPE _eMeType, PxU32 _ColliderGroup);
	HRESULT Clear_Scene();
#pragma endregion 

#pragma region TRIGGER
	HRESULT Set_BlackScreen(class CUIObject* _pBlackScreen);
	HRESULT Add_Trigger(TRIGGER_TYPE _eTriggerType, class CGameObject* _pTarget = nullptr);
	HRESULT Activate_Fade(TRIGGER_TYPE _eTriggerType, _float _Duration, _float _fDurationSecond = 0.0f);
	_bool   Is_Fade_Complete(TRIGGER_TYPE _eTriggerType);
	void   Setting_NULL_BlackScreen();
	void Clear_Trigger();
#pragma endregion

#pragma region MONSTER_MANAGER
	HRESULT Add_Monster(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, MONSTER_CATEGORY _eCategory, void* _pArg);
	HRESULT Add_Delete_Monster(class CMonster* pMonster);
	HRESULT Delete_All_Monster();
	HRESULT Active_Monster();
	HRESULT Respawn_Monster(MONSTER_CATEGORY _eCategory);
	deque<class CMonster*>& Get_Check_Monsters();
	_bool Get_Boss_Dead()const;
	_bool Get_Boss_Active()const;
	void Set_Boss_Dead(_bool bCheck);
	void Set_Boss_Active(_bool bCheck);
#pragma endregion

#pragma region PROJECTILE_MANAGER
	HRESULT Add_Projectile(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, PROJECTILE_CATEGORY _eCategory, void* _pArg);
	HRESULT Fire_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos);
	HRESULT Fire_Multi_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos, _uint iCount, _bool bReverse = false);
#pragma endregion

#pragma region RIPPLE
	void Add_RippleInfo(_float2 vPos, _float fRippleRange);
	void Set_WaterPos(_float2 vWaterPos);

	HRESULT  Bind_RippleSRV(class CShader* pShader);

	_bool   IsInWater();
	void	Initialize_IsInWater(_bool _bInWater);
	void	Set_bInWater();
#pragma endregion

	void	Set_LoadObjects() { m_bLoadObjects = !m_bLoadObjects; }
	_bool	LoadObjects() {
		if (m_bLoadObjects)
		{
			m_bLoadObjects = false;
			return true;
		}

		return false;
	}
private:
	_uint								m_iViewportWidth{}, m_iViewportHeight{};

private:
	class CTimer_Manager*				m_pTimer_Manager      = { nullptr };
	class CGraphic_Device*				m_pGraphic_Device     = { nullptr };
	class CInput_Device*				m_pInput_Device       = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager      = { nullptr };	
	class CObject_Manager*				m_pObject_Manager     = { nullptr };
	class CPrototype_Manager*			m_pPrototype_Manager  = { nullptr };
	class CRenderer*					m_pRenderer           = { nullptr };
	class CPipeLine*					m_pPipeLine           = { nullptr };
	class CLight_Manager*				m_pLight_Manager      = { nullptr };
	class CTarget_Manager*				m_pTarget_Manager     = { nullptr };
	class CEventMgr*					m_pEvent_Manager      = { nullptr };
	class CEffectMgr*					m_pEffect_Manager     = { nullptr };
	class CSoundMgr*					m_pSound_Manager      = { nullptr };
	class CFont_Manager*				m_pFont_Manager       = { nullptr };
	class CFrustum*						m_pFrustum            = { nullptr };
	class CItemMgr*						m_pItemMgr            = { nullptr };
	class CShadow*						m_pShadow             = { nullptr };
	class CUI_Manager*					m_pUI_Manager		  = { nullptr };
	class CPhysX_Manager*				m_pPhysX_Manager	  = { nullptr };	
	class CTriggerManager*				m_pTrigger_Manager	  = { nullptr };
	class CMonster_Manager*				m_pMonster_Manager	  = { nullptr };
	//범승 나중에 추가예정.(투사체 매니저)
	class CProjectile_Manager*			m_pProjectile_Manager = { nullptr };
	class CRippleManager*				m_pRipple_Manager     = { nullptr };

public:
	void   Set_SlowWorld(_bool _OnOff) { m_bSlowWorldOnOFF = _OnOff; }	
	_bool* Get_SlowWorldPtr()          { return &m_bSlowWorldOnOFF; }		

private:
	_bool		m_bSlowWorldOnOFF = { false };	

	_bool		m_bLoadObjects  = { false };
	_bool       m_bPhysicsOnOff = { false };
public:
	void Release_Engine();
	virtual void Free() override;
	
};

END
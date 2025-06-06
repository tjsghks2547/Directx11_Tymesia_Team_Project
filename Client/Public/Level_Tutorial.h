#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Tutorial final : public CLevel
{
private:
	CLevel_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Tutorial() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Structure(const _tchar* pLayerTag);	
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);	
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster();
	HRESULT Ready_Layer_NPC(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Fade(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Button(const _tchar* pLayerTag);

	HRESULT	Ready_Layer_Item(const _tchar* pLayerTag);

	HRESULT Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_Inventory(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_LandingMessage(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_Dialogue(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_Skill(const _tchar* pLayerTag);


	// 테스트용 사다리 레이어 추가
	//HRESULT	Ready_Layer_Ladder(const _tchar* pLayerTag);

private:
	HRESULT Load_Objects(_int iObject_Level);
	HRESULT Load_TriggerObjects(_int iObject_Level);
	//HRESULT Load_InstancingObjects(_int iObject_Level);
	HRESULT Load_SpecificObjects(_int iObject_Level);
	HRESULT Load_Height(_int iObject_Level);
	HRESULT Load_MonsterIndex(_int iMonsterIndex_Level);
	HRESULT Load_Effect(const _tchar* _pEffectFilePath, _uint _iPrototypeLevelIndex, const _tchar* _pEffectPrototypeName, EFFECT_TYPE _eEffectType, EFFECT_NAME _eEffectName, _uint _iEffectCount = 1);

	HRESULT	Load_DestructObjects(_int iObject_Level);
private:
	_bool m_bStopMenuOpen = { false }; // 유빈

	vector<MONSTERSPAWNINFO> m_MonsterSpawnInfos;

public:
	static CLevel_Tutorial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
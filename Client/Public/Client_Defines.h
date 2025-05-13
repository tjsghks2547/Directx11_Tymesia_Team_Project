#pragma once

#include <process.h>
#include <random>

namespace Client
{
	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;

	enum LEVELID {
		LEVEL_STATIC, // UI, Monster, Effect, etc
		LEVEL_LOADING, // Loading(어디로 가냐에 따라 달라질듯)
		LEVEL_LOGO, // Intro Menu
		LEVEL_TUTORIAL, // 튜토맵
		LEVEL_SEAOFTREES, // 나무바다(서커스)
		LEVEL_ROYALGARDEN, // 왕실정원
		LEVEL_OCEAN, // 헤르메스성채
		LEVEL_HILL, //철학자의 집
		LEVEL_END,
	};
	enum UISCENEID {
		UISCENE_LOADING,
		UISCENE_MENU, UISCENE_LEVELUP, UISCENE_TALENT, UISCENE_INTRO,
		UISCENE_INVEN, UISCENE_PLAYERSCREEN, UISCENE_MONSTERHP, UISCENE_ALL, UISCNEN_MESSAGE,
		UISCENE_MAP,UISCENE_SKILL,UISCENE_DIALOGUE, UISCENE_END
	}; // UI 그룹 - 유빈
	
	enum SLOTSTATE { SLOT_OPEN_ON, SLOT_OPEN_OFF, SLOT_CLOSE_ON, SLOT_CLOSE_OFF, SLOT_CLOSE_CONDITION, SLOT_END }; // ui 특성 버튼 상태 관리 용도 - 유빈 
	enum SkILL_SLOTSTATE { SKILL_OPEN_ON, SKILL_OPEN_OFF, SKILL_CLOSE_ON, SKILL_CLOSE_OFF, SKILL_OPEN_IDLE, SKILL_END }; // ui 스킬 버튼 상태 관리 용도 - 유빈 

	enum MONSTER_NAME
	{
		NORMAL_VILLAGE_M0, //일반 남자 도끼병
		NORMAL_VILLAGE_M1, //준일반 남자 도끼방패병
		NORMAL_SCYTHEM,    //준일반 남자 낫병
		NORMAL_VILLAGE_F0, //일반 여자 단도병
		NORMAL_VILLAGE_F1, //준일반 여자 단도병
		ELITE_HARMORLV2,   //엘리트 검병
		ELITE_PUNCH_MAN,   //엘리트 펀치맨
		ELITE_GRACE,       //엘리트 그레이스
		ELITE_JOKER,       //엘리트 조커
		ELITE_REASEARCHER,     //엘리트 리처
		BOSS_VARG,         //보스 바그
		BOSS_ORD,          //보스 오두르
		BOSS_URD,          //보스 우르드
		BOSS_BAT,          //보스 박쥐
	};

	enum TEXTURE_EFFECT { TEXTURE_DIFFUSE,	TEXTURE_NOISE, TEXTURE_MASK, TEXTURE_NORMAL, TEXTURE_END }; //종한 Effect용도

	/* 컬링 거리 */
	const float g_fCullingDistance = 30.f;


}

using namespace Client;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;



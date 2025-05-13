#pragma once

#include <process.h>
#include <random>

namespace Client
{
	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;

	enum LEVELID {
		LEVEL_STATIC, // UI, Monster, Effect, etc
		LEVEL_LOADING, // Loading(���� ���Ŀ� ���� �޶�����)
		LEVEL_LOGO, // Intro Menu
		LEVEL_TUTORIAL, // Ʃ���
		LEVEL_SEAOFTREES, // �����ٴ�(��Ŀ��)
		LEVEL_ROYALGARDEN, // �ս�����
		LEVEL_OCEAN, // �츣�޽���ä
		LEVEL_HILL, //ö������ ��
		LEVEL_END,
	};
	enum UISCENEID {
		UISCENE_LOADING,
		UISCENE_MENU, UISCENE_LEVELUP, UISCENE_TALENT, UISCENE_INTRO,
		UISCENE_INVEN, UISCENE_PLAYERSCREEN, UISCENE_MONSTERHP, UISCENE_ALL, UISCNEN_MESSAGE,
		UISCENE_MAP,UISCENE_SKILL,UISCENE_DIALOGUE, UISCENE_END
	}; // UI �׷� - ����
	
	enum SLOTSTATE { SLOT_OPEN_ON, SLOT_OPEN_OFF, SLOT_CLOSE_ON, SLOT_CLOSE_OFF, SLOT_CLOSE_CONDITION, SLOT_END }; // ui Ư�� ��ư ���� ���� �뵵 - ���� 
	enum SkILL_SLOTSTATE { SKILL_OPEN_ON, SKILL_OPEN_OFF, SKILL_CLOSE_ON, SKILL_CLOSE_OFF, SKILL_OPEN_IDLE, SKILL_END }; // ui ��ų ��ư ���� ���� �뵵 - ���� 

	enum MONSTER_NAME
	{
		NORMAL_VILLAGE_M0, //�Ϲ� ���� ������
		NORMAL_VILLAGE_M1, //���Ϲ� ���� �������к�
		NORMAL_SCYTHEM,    //���Ϲ� ���� ����
		NORMAL_VILLAGE_F0, //�Ϲ� ���� �ܵ���
		NORMAL_VILLAGE_F1, //���Ϲ� ���� �ܵ���
		ELITE_HARMORLV2,   //����Ʈ �˺�
		ELITE_PUNCH_MAN,   //����Ʈ ��ġ��
		ELITE_GRACE,       //����Ʈ �׷��̽�
		ELITE_JOKER,       //����Ʈ ��Ŀ
		ELITE_REASEARCHER,     //����Ʈ ��ó
		BOSS_VARG,         //���� �ٱ�
		BOSS_ORD,          //���� ���θ�
		BOSS_URD,          //���� �츣��
		BOSS_BAT,          //���� ����
	};

	enum TEXTURE_EFFECT { TEXTURE_DIFFUSE,	TEXTURE_NOISE, TEXTURE_MASK, TEXTURE_NORMAL, TEXTURE_END }; //���� Effect�뵵

	/* �ø� �Ÿ� */
	const float g_fCullingDistance = 30.f;


}

using namespace Client;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;



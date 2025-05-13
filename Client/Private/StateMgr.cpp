#include "pch.h"
#include "StateMgr.h"
#include "PlayerIdle.h"
#include "PlayerRun.h"

#pragma region ��Ŭ�� ���� 
#include "Attack_LButton_1.h"
#include "Attack_LButton_2.h"
#include "Attack_LButton_3.h"
#include "Attack_LButton_4.h"
#include "Attack_LButton_5.h"	
#pragma endregion 

#pragma region ��Ŭ�� ���� 
#include "Attack_LongClaw_1.h"	
#include "Attack_LongClaw_2.h"	
#pragma endregion	


#pragma region ���¸�� ���� �̵� 
#include "Rock_On_Run_R.h"
#include "Rock_On_Run_L.h"
#include "Rock_On_Run_FR.h"
#include "Rock_On_Run_FL.h"
#include "Rock_On_Run_BR.h"
#include "Rock_On_Run_BL.h"
#include "Rock_On_Run_B.h"
#include "Rock_On_Run_F.h"

#include "LockOn_Evade_B.h"	
#include "LockOn_Evade_L.h"
#include "LockOn_Evade_R.h"
#include "LockOn_Evade_F.h"
#pragma endregion 

#pragma region �и�
#include "Parry_L.h"
#include "Parry_R.h"
#include "ParryDeflect_Large.h"
#include "ParryDeflect_L.h"
#include "ParryDeflect_LUp.h"
#include "ParryDeflect_R.h"
#include "ParryDeflect_RUp.h"
#pragma endregion 

#pragma region �ǰ� ���� ��
#include "HurtMFL.h"	
#include "HurtMFR.h"
#include "HurtKnockBack.h"
#include "HurtKnockDown.h"
#include "HURTLF.h"
#include "HURTSF.h"
#include "HURTSL.h"
#include "HURTXXLF.h"	
#include "HURT_FallDown.h"	
#include "HURT_FallDown_End.h"	
#include "Rebound_R.h"	
#include "StunnedStart.h"
#include "Varg_Catched.h"	
#pragma endregion 


#pragma region ȸ�� ����
#include "Normal_Evade_R.h"
#include "Normal_Evade_L.h"
#include "Normal_Evade_FR.h"
#include "Normal_Evade_FL.h"
#include "Normal_Evade_F.h"
#include "Normal_Evade_BR.h"
#include "Normal_Evade_BL.h"
#include "Normal_Evade_B.h"
#pragma endregion 


#pragma region ó�� ���� 

#include "HArmorLV1_Execution.h"

#pragma endregion 

#pragma region ���� ���� 

#include "ArchiveSitStart.h"
#include "ArchiveSitLoop.h"
#include "ArchiveGetUp.h"
#include "ArchiveSitLight_Up.h"

#pragma endregion 


#pragma region ��Ŭ�� ���� ���� 

#include "ClawCommon_ChargeStart.h"	
#include "ClawCommon_ChargeLoop.h"	
#include "ClawCommon_ChargeFull.h"	

#pragma endregion 

#pragma region ��ٸ� ���� �̵� 
#include "Ladder_Climb_Start.h"
#include "Ladder_Climb_L_Down.h"
#include "Ladder_Climb_R_Up_End.h"
#include "Ladder_Climb_R_Up_Reverse_End.h"	
#include "Ladder_Climb_L_Up.h"	
#include "Ladder_Climb_R_Up.h"
#include "Ladder_Climb_R_Down.h"

#pragma endregion 


#pragma region ���θ� ó�� ���� 
#include "Magician_LV1_Catched.h"
#include "Magician_LV2_Catched.h"
#pragma endregion 


#pragma region �÷��̾� �޸���		
#include "PlayerSprint.h"	
#pragma endregion 

#pragma region ����ó���� ������ ���� ��Ȳ 
#include "Researcher_Cathced.h"
#pragma endregion 


CStateMgr::CStateMgr()
{

}

HRESULT CStateMgr::Initialize()
{
	m_vecState.reserve(100);

	/* 0�� Idle ���� */
	CPlayerIdle* pStateIdle = CPlayerIdle::Create();
	m_vecState.push_back(pStateIdle);


	/* 1�� Run ������ �ٴ� ���� �ִϸ��̼�  W Ű */
	CPlayerRun* pStateRun = CPlayerRun::Create();
	m_vecState.push_back(pStateRun);

	/* 2�� Attack ��Ŭ�� ���� ù ��° ��� LButton Ű */
	CAttack_LButton_1* pAttackLButton_1 = CAttack_LButton_1::Create();
	m_vecState.push_back(pAttackLButton_1);

	/* 3�� Attack ��Ŭ�� ���� �� ��° ��� LButton Ű */
	CAttack_LButton_2* pAttackLButton_2 = CAttack_LButton_2::Create();
	m_vecState.push_back(pAttackLButton_2);

	/* 4�� Attack ��Ŭ�� ���� �� ��° ��� LButton Ű */
	CAttack_LButton_3* pAttackLButton_3 = CAttack_LButton_3::Create();
	m_vecState.push_back(pAttackLButton_3);

	/* 5�� Attack ��Ŭ�� ���� ù ��° ��� RButton Enter Ű */
	CAttack_LongClaw_1* pAttack_LongClaw_1 = CAttack_LongClaw_1::Create();
	m_vecState.push_back(pAttack_LongClaw_1);

	/* 6�� Attack ��Ŭ�� ���� ù ��° ��� RButton Enter Ű */
	CAttack_LongClaw_2* pAttack_LongClaw_2 = CAttack_LongClaw_2::Create();
	m_vecState.push_back(pAttack_LongClaw_2);

#pragma region ���� ��� �϶��� �����̵� 

	/* 7�� ���¸���϶� 'D' Ű�� ���� �� ������ �̵� ��� */
	CRock_On_Run_R* pRock_On_Run_R = CRock_On_Run_R::Create();
	m_vecState.push_back(pRock_On_Run_R);

	/* 8�� ���¸���϶� 'A' Ű�� ���� �� ������ �̵� ��� */
	CRock_On_Run_L* pRock_On_Run_L = CRock_On_Run_L::Create();
	m_vecState.push_back(pRock_On_Run_L);


	/* 9�� ���¸���϶� 'W','D' Ű�� ���ÿ�  ���� �� �� �밢�� ������  �̵� ��� */
	CRock_On_Run_FR* pRock_On_Run_FR = CRock_On_Run_FR::Create();
	m_vecState.push_back(pRock_On_Run_FR);

	/* 10�� ���¸���϶� 'W','A' Ű�� ���ÿ�  ���� �� �� �밢�� ���� �̵� ��� */
	CRock_On_Run_FL* pRock_On_Run_FL = CRock_On_Run_FL::Create();
	m_vecState.push_back(pRock_On_Run_FL);

	/* 11�� ���¸���϶� 'S','D' Ű�� ���ÿ�  ���� �� �Ʒ� �밢�� ������ �̵� ��� */
	CRock_On_Run_BR* pRock_On_Run_BR = CRock_On_Run_BR::Create();
	m_vecState.push_back(pRock_On_Run_BR);

	/* 12�� ���¸���϶� 'S','A' Ű�� ���ÿ�  ���� �� �Ʒ� �밢�� ������ �̵� ��� */
	CRock_On_Run_BL* pRock_On_Run_BL = CRock_On_Run_BL::Create();
	m_vecState.push_back(pRock_On_Run_BL);

	/* 13�� ���¸���϶� 'S' Ű�� ���� �� �Ʒ� �밢�� ������ �̵� ��� */
	CRock_On_Run_B* pRock_On_Run_B = CRock_On_Run_B::Create();
	m_vecState.push_back(pRock_On_Run_B);

	/* 14�� ���¸���϶� 'W' Ű�� ���� �� �� �̵� ��� */
	CRock_On_Run_F* pRock_On_Run_F = CRock_On_Run_F::Create();
	m_vecState.push_back(pRock_On_Run_F);

	/* 15�� ���¸���϶� 'SPACE' Ű�� ���� �� �ڷ� ȸ�� ��� */
	CLockOn_Evade_B* LockOn_Evade_B = CLockOn_Evade_B::Create();
	m_vecState.push_back(LockOn_Evade_B);

	/* 16�� ���¸���϶� 'A' Ű�� �����鼭 'SPACE' Ű�� ���� �� ������ ȸ�� ���*/
	CLockOn_Evade_L* LockOn_Evade_L = CLockOn_Evade_L::Create();
	m_vecState.push_back(LockOn_Evade_L);

	/* 17�� ���¸���϶� 'D Ű�� �����鼭'SPACE' Ű�� ���� �� ������ ȸ�� ���*/
	CLockOn_Evade_R* LockOn_Evade_R = CLockOn_Evade_R::Create();
	m_vecState.push_back(LockOn_Evade_R);

	/* 18�� ���¸���϶� 'W' Ű�� �����鼭 'SPACE' Ű�� ���� �� ������ ȸ�� ���*/
	CLockOn_Evade_F* LockOn_Evade_F = CLockOn_Evade_F::Create();
	m_vecState.push_back(LockOn_Evade_F);
#pragma endregion 
#pragma region �и� �⺻ ��� 
	/* 19�� �и� ��� 'F' Ű�� ���� �� ���� ���� ������ �缱���� �ö󰡸鼭 �ϴ� �и� ���*/
	CParry_L* Parry_L = CParry_L::Create();
	m_vecState.push_back(Parry_L);

	/* 20�� �и� ��� 'F' Ű�� ���� �� ������ ���� ���� �缱���� �������鼭 �ϴ� �и� ���*/
	CParry_R* Parry_R = CParry_R::Create();
	m_vecState.push_back(Parry_R);

#pragma endregion 
#pragma region �ǰ�
	/* 21�� �ǰ� ��� (�������� ���� ��û�̸鼭 �ڷΰ��� ��� ) */
	CHurtMFL* pHurtMFL = CHurtMFL::Create();
	m_vecState.push_back(pHurtMFL);

	/* 22�� �ǰ� ��� (���������� ���� ��û�̸鼭 �ڷΰ��� ��� ) */
	CHurtMFR* pHurtMFR = CHurtMFR::Create();
	m_vecState.push_back(pHurtMFR);
#pragma endregion 
#pragma region �и� ���� ��� 

	/* 23�� �и� ����  ��� ( �ڷιз� ���鼭 ��û�̴� ��� ) */
	CParryDeflect_Large* pParryDeflect_Large = CParryDeflect_Large::Create();
	m_vecState.push_back(pParryDeflect_Large);

	/* 24�� �и� ���� ���  ( ������ �� �缱���� �������� Į�� �����鼭 ���� �и� ��� )*/
	CParryDeflect_L* pParryDeflect_L = CParryDeflect_L::Create();
	m_vecState.push_back(pParryDeflect_L);

	/* 25�� �и� ���� ���  ( ������ �Ʒ����� ���� �� �밢������ Į�� �ø��鼭 ���� �и� ���)*/
	CParryDeflect_LUp* pParryDeflect_LUp = CParryDeflect_LUp::Create();
	m_vecState.push_back(pParryDeflect_LUp);

	/* 26�� �и� ���� ��� ( ���� �� �缱���� ������ ���� Į�� �����鼭 ���� �и� ���) */
	CParryDeflect_R* pParryDeflect_R = CParryDeflect_R::Create();
	m_vecState.push_back(pParryDeflect_R);

	/* 27�� �и� ���� ��� ( ���� �Ʒ� ���� ������ ���� Į�� �ø��鼭 ���� �и� ��� )*/
	CParryDeflect_RUp* pParryDeflect_RUp = CParryDeflect_RUp::Create();
	m_vecState.push_back(pParryDeflect_RUp);

#pragma endregion 
#pragma region ȸ�� ��� 
	/* 28�� ������ ȸ�� �뽬 ( ������ Ű + �����̽� �� ) ��� */
	CNormal_Evade_R* pNormal_Evade_R = CNormal_Evade_R::Create();
	m_vecState.push_back(pNormal_Evade_R);

	/* 29�� ������ ȸ�� �뽬 ( ������ Ű + �����̽� �� ) ��� */
	CNormal_Evade_L* pNormal_Evade_L = CNormal_Evade_L::Create();
	m_vecState.push_back(pNormal_Evade_L);

	/* 30�� �� ������  �뽬 ( ��Ű +  ������ Ű + �����̽� �� ) ��� */
	CNormal_Evade_FR* pNormal_Evade_FR = CNormal_Evade_FR::Create();
	m_vecState.push_back(pNormal_Evade_FR);

	/* 31�� �� ����  �뽬 ( ��Ű +  ���� Ű + �����̽� �� ) ��� */
	CNormal_Evade_FL* pNormal_Evade_FL = CNormal_Evade_FL::Create();
	m_vecState.push_back(pNormal_Evade_FL);

	/* 32�� �� �뽬 ( ��Ű + �����̽� �� ) ��� */
	CNormal_Evade_F* pNormal_Evade_F = CNormal_Evade_F::Create();
	m_vecState.push_back(pNormal_Evade_F);

	/* 33�� �� ������  �뽬 ( �� Ű +  ������ Ű + �����̽� �� ) ��� */
	CNormal_Evade_BR* pNormal_Evade_BR = CNormal_Evade_BR::Create();
	m_vecState.push_back(pNormal_Evade_BR);

	/* 34�� �� ������  �뽬 ( �� Ű +  ���� Ű + �����̽� �� ) ��� */
	CNormal_Evade_BL* pNormal_Evade_BL = CNormal_Evade_BL::Create();
	m_vecState.push_back(pNormal_Evade_BL);

	/* 35�� �� �뽬 ( �� Ű + �����̽� �� ) ��� */
	CNormal_Evade_B* pNormal_Evade_B = CNormal_Evade_B::Create();
	m_vecState.push_back(pNormal_Evade_B);
#pragma endregion 

#pragma region �߰� �ǰ� ��� 
	/* 36�� �ǰ� ��� �˹�  ��� */
	CHurtKnockBack* pHurtKnockBack = CHurtKnockBack::Create();
	m_vecState.push_back(pHurtKnockBack);

	/* 37�� �ǰ� ��� �˴ٿ�  ��� */
	CHurtKnockDown* pHurtKnockDown = CHurtKnockDown::Create();
	m_vecState.push_back(pHurtKnockDown);

	/* 38�� �ǰ� ��� ���� ����  ��� */
	CHURTLF* pHURTLF = CHURTLF::Create();
	m_vecState.push_back(pHURTLF);

	/* 39�� �ǰ� ��� HURT_SF ����  ��� */
	CHURTSF* pHURTSF = CHURTSF::Create();
	m_vecState.push_back(pHURTSF);

	/* 40�� �ǰ� ��� HURT_XXLF */
	CHURTXXLF* pHURTXXLF = CHURTXXLF::Create();
	m_vecState.push_back(pHURTXXLF);

	/* 41�� �ǰ� ��� */
	CHURTSL* pHURTSL = CHURTSL::Create();
	m_vecState.push_back(pHURTSL);

	/* 42�� �ǰ� ��� */
	CHURT_FallDown* pHURT_FallDown = CHURT_FallDown::Create();
	m_vecState.push_back(pHURT_FallDown);
#pragma endregion 


#pragma region ��Ŭ�� ���� ��� 4Ÿ, 5Ÿ 
	/* 43�� ��Ŭ�� 4Ÿ ���� ��� */
	CAttack_LButton_4* pAttack_LButton_4 = CAttack_LButton_4::Create();
	m_vecState.push_back(pAttack_LButton_4);

	/* 44�� ��Ŭ�� 5Ÿ ���� ��� */
	CAttack_LButton_5* pAttack_LButton_5 = CAttack_LButton_5::Create();
	m_vecState.push_back(pAttack_LButton_5);

#pragma endregion 


#pragma region ó�� ���
	/* 45�� HArmorLV1 ó�� ��� */
	CHArmorLV1_Execution* pHArmorLV1_Execution = CHArmorLV1_Execution::Create();
	m_vecState.push_back(pHArmorLV1_Execution);
#pragma endregion 


#pragma region ���Ͱ� �и��� ������ ��  

	/* 46�� ���Ͱ� �÷��̾� ���� �и� ������ �� */
	CRebound_R* pRebound_R = CRebound_R::Create();
	m_vecState.push_back(pRebound_R);

#pragma endregion 

	/* 47�� ���Ͱ� ���� ������ �־ �÷��̾� ���� ��� */
	CStunnedStart* pStunnedStart = CStunnedStart::Create();
	m_vecState.push_back(pStunnedStart);

#pragma region  ���� ��ȣ�ۿ� ���� 
	/* 48�� ���� �ɴ� ��� ���� */
	CArchiveSitStart* pArchiveSitStart = CArchiveSitStart::Create();
	m_vecState.push_back(pArchiveSitStart);

	/* 49�� ���� �ɰ� �ִ� ���� ��� */
	CArchiveSitLoop* pArchiveSitLoop = CArchiveSitLoop::Create();
	m_vecState.push_back(pArchiveSitLoop);

	/* 50�� ���ڿ��� �Ͼ�� ��� */
	CArchiveGetUp* pArchiveGetUp = CArchiveGetUp::Create();
	m_vecState.push_back(pArchiveGetUp);

	/* 51�� ���ڿ� ���� �� Ű�� ��� */
	CArchiveSitLight_Up* pArchiveSitLight_Up = CArchiveSitLight_Up::Create();
	m_vecState.push_back(pArchiveSitLight_Up);

#pragma endregion 

#pragma region ��Ŭ�� ���� ���� 

	/* 52�� ��Ŭ�� ���� ���� ���� */
	CClawCommon_ChargeStart* pClawCommon_ChargeStart = CClawCommon_ChargeStart::Create();
	m_vecState.push_back(pClawCommon_ChargeStart);

	/* 53�� ��Ŭ�� ���� ���� ��� */
	CClawCommon_ChargeLoop* pClawCommon_ChargeLoop = CClawCommon_ChargeLoop::Create();
	m_vecState.push_back(pClawCommon_ChargeLoop);

	/* 54�� ��Ŭ�� ���� ���� ��� */
	CClawCommon_ChargeFull* pClawCommon_ChargeFull = CClawCommon_ChargeFull::Create();
	m_vecState.push_back(pClawCommon_ChargeFull);


#pragma endregion 


#pragma region �ٱ����� ������ ��� 

	/* 55�� ��Ŭ�� ���� ���� ��� */
	CVarg_Catched* pVarg_Catched = CVarg_Catched::Create();
	m_vecState.push_back(pVarg_Catched);

#pragma endregion 


#pragma region ��ٸ� ���� ��� 
	/* 56�� ��ٸ� ó�� �ö󰡴� start ��� */
	CLadder_Climb_Start* pLadder_Climb_Start = CLadder_Climb_Start::Create();
	m_vecState.push_back(pLadder_Climb_Start);

	/* 57�� ��ٸ� ���� �������� ��� */
	CLadder_Climb_L_Down* pLadder_Climb_L_Down = CLadder_Climb_L_Down::Create();
	m_vecState.push_back(pLadder_Climb_L_Down);

	/* 58�� ���  ���� ���� ������ �ö󰡱� �������� ���  */
	CLadder_Climb_R_Up_End* pLadder_Climb_R_Up_End = CLadder_Climb_R_Up_End::Create();
	m_vecState.push_back(pLadder_Climb_R_Up_End);

	/* 59�� ��ٸ� ó�� �������� ���(����������� ����������) && ���� ���� ������ �ö󰡱� �������� ��� */
	CLadder_Climb_R_Up_Reverse_End* pLadder_Climb_R_Up_Reverse_End = CLadder_Climb_R_Up_Reverse_End::Create();
	m_vecState.push_back(pLadder_Climb_R_Up_Reverse_End);

	/* 60�� ��ٸ� ���� �� �ö󰡴� ��� */
	CLadder_Climb_L_Up* pLadder_Climb_L_Up = CLadder_Climb_L_Up::Create();
	m_vecState.push_back(pLadder_Climb_L_Up);

	/* 61�� ��ٸ� ������ �� �ö󰡴� ��� */
	CLadder_Climb_R_Up* pLadder_Climb_R_Up = CLadder_Climb_R_Up::Create();
	m_vecState.push_back(pLadder_Climb_R_Up);

	/* 62�� ��ٸ� ������ �������� ��� */
	CLadder_Climb_R_Down* pLadder_Climb_R_Down = CLadder_Climb_R_Down::Create();
	m_vecState.push_back(pLadder_Climb_R_Down);

#pragma endregion 

#pragma region ���θ� ���� ���
	/* 63�� ���θ� ���� ��ų �¾Ƽ� ���ϴ� ��� */
	CMagician_LV1_Catched* pMagician_LV1_Catched = CMagician_LV1_Catched::Create();
	m_vecState.push_back(pMagician_LV1_Catched);

#pragma endregion 


#pragma region �÷��̾� �޸��� ��� 
	/*64�� �÷��̾� ������Ʈ �޸��� ��� */
	CPlayerSprint* pPlayerSprint = CPlayerSprint::Create();
	m_vecState.push_back(pPlayerSprint);

#pragma endregion 

#pragma region ���� ���θ� ���� ���
	/* 65�� ���θ� ���� ��ų �¾Ƽ� ���ϴ� ��� */
	CMagician_LV2_Catched* pMagician_LV2_Catched = CMagician_LV2_Catched::Create();
	m_vecState.push_back(pMagician_LV2_Catched);
#pragma endregion


#pragma region ����ó���� ���� ������ ���� ��� 
	/* 66�� ����ó���� ���� ������ ���� ��� */
	CResearcher_Cathced* pResearcher_Cathced = CResearcher_Cathced::Create();
	m_vecState.push_back(pResearcher_Cathced);
#pragma endregion 

	return S_OK;
}

CStateMgr* CStateMgr::Create()
{
	CStateMgr* pInstance = new CStateMgr();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CStateMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateMgr::Free()
{
	__super::Free();

	for (auto& iter : m_vecState)
	{
		Safe_Release(iter);
	}

	m_vecState.clear();

}

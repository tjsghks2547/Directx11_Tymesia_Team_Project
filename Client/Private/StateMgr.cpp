#include "pch.h"
#include "StateMgr.h"
#include "PlayerIdle.h"
#include "PlayerRun.h"

#pragma region 좌클릭 공격 
#include "Attack_LButton_1.h"
#include "Attack_LButton_2.h"
#include "Attack_LButton_3.h"
#include "Attack_LButton_4.h"
#include "Attack_LButton_5.h"	
#pragma endregion 

#pragma region 우클릭 공격 
#include "Attack_LongClaw_1.h"	
#include "Attack_LongClaw_2.h"	
#pragma endregion	


#pragma region 락온모드 방향 이동 
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

#pragma region 패링
#include "Parry_L.h"
#include "Parry_R.h"
#include "ParryDeflect_Large.h"
#include "ParryDeflect_L.h"
#include "ParryDeflect_LUp.h"
#include "ParryDeflect_R.h"
#include "ParryDeflect_RUp.h"
#pragma endregion 

#pragma region 피격 당할 시
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


#pragma region 회피 관련
#include "Normal_Evade_R.h"
#include "Normal_Evade_L.h"
#include "Normal_Evade_FR.h"
#include "Normal_Evade_FL.h"
#include "Normal_Evade_F.h"
#include "Normal_Evade_BR.h"
#include "Normal_Evade_BL.h"
#include "Normal_Evade_B.h"
#pragma endregion 


#pragma region 처형 관련 

#include "HArmorLV1_Execution.h"

#pragma endregion 

#pragma region 의자 관련 

#include "ArchiveSitStart.h"
#include "ArchiveSitLoop.h"
#include "ArchiveGetUp.h"
#include "ArchiveSitLight_Up.h"

#pragma endregion 


#pragma region 우클릭 차지 공격 

#include "ClawCommon_ChargeStart.h"	
#include "ClawCommon_ChargeLoop.h"	
#include "ClawCommon_ChargeFull.h"	

#pragma endregion 

#pragma region 사다리 관련 이동 
#include "Ladder_Climb_Start.h"
#include "Ladder_Climb_L_Down.h"
#include "Ladder_Climb_R_Up_End.h"
#include "Ladder_Climb_R_Up_Reverse_End.h"	
#include "Ladder_Climb_L_Up.h"	
#include "Ladder_Climb_R_Up.h"
#include "Ladder_Climb_R_Down.h"

#pragma endregion 


#pragma region 오두르 처형 관련 
#include "Magician_LV1_Catched.h"
#include "Magician_LV2_Catched.h"
#pragma endregion 


#pragma region 플레이어 달리기		
#include "PlayerSprint.h"	
#pragma endregion 

#pragma region 리서처에게 잡혔을 때의 상황 
#include "Researcher_Cathced.h"
#pragma endregion 


CStateMgr::CStateMgr()
{

}

HRESULT CStateMgr::Initialize()
{
	m_vecState.reserve(100);

	/* 0번 Idle 상태 */
	CPlayerIdle* pStateIdle = CPlayerIdle::Create();
	m_vecState.push_back(pStateIdle);


	/* 1번 Run 앞으로 뛰는 상태 애니메이션  W 키 */
	CPlayerRun* pStateRun = CPlayerRun::Create();
	m_vecState.push_back(pStateRun);

	/* 2번 Attack 좌클릭 공격 첫 번째 모션 LButton 키 */
	CAttack_LButton_1* pAttackLButton_1 = CAttack_LButton_1::Create();
	m_vecState.push_back(pAttackLButton_1);

	/* 3번 Attack 좌클릭 공격 두 번째 모션 LButton 키 */
	CAttack_LButton_2* pAttackLButton_2 = CAttack_LButton_2::Create();
	m_vecState.push_back(pAttackLButton_2);

	/* 4번 Attack 좌클릭 공격 세 번째 모션 LButton 키 */
	CAttack_LButton_3* pAttackLButton_3 = CAttack_LButton_3::Create();
	m_vecState.push_back(pAttackLButton_3);

	/* 5번 Attack 우클릭 공격 첫 번째 모션 RButton Enter 키 */
	CAttack_LongClaw_1* pAttack_LongClaw_1 = CAttack_LongClaw_1::Create();
	m_vecState.push_back(pAttack_LongClaw_1);

	/* 6번 Attack 우클릭 공격 첫 번째 모션 RButton Enter 키 */
	CAttack_LongClaw_2* pAttack_LongClaw_2 = CAttack_LongClaw_2::Create();
	m_vecState.push_back(pAttack_LongClaw_2);

#pragma region 락온 모드 일때의 방향이동 

	/* 7번 락온모드일때 'D' 키만 누를 시 오른쪽 이동 모션 */
	CRock_On_Run_R* pRock_On_Run_R = CRock_On_Run_R::Create();
	m_vecState.push_back(pRock_On_Run_R);

	/* 8번 락온모드일때 'A' 키만 누를 시 오른쪽 이동 모션 */
	CRock_On_Run_L* pRock_On_Run_L = CRock_On_Run_L::Create();
	m_vecState.push_back(pRock_On_Run_L);


	/* 9번 락온모드일때 'W','D' 키를 동시에  누를 시 위 대각선 오른쪽  이동 모션 */
	CRock_On_Run_FR* pRock_On_Run_FR = CRock_On_Run_FR::Create();
	m_vecState.push_back(pRock_On_Run_FR);

	/* 10번 락온모드일때 'W','A' 키를 동시에  누를 시 위 대각선 왼쪽 이동 모션 */
	CRock_On_Run_FL* pRock_On_Run_FL = CRock_On_Run_FL::Create();
	m_vecState.push_back(pRock_On_Run_FL);

	/* 11번 락온모드일때 'S','D' 키를 동시에  누를 시 아래 대각선 오른쪽 이동 모션 */
	CRock_On_Run_BR* pRock_On_Run_BR = CRock_On_Run_BR::Create();
	m_vecState.push_back(pRock_On_Run_BR);

	/* 12번 락온모드일때 'S','A' 키를 동시에  누를 시 아래 대각선 오른쪽 이동 모션 */
	CRock_On_Run_BL* pRock_On_Run_BL = CRock_On_Run_BL::Create();
	m_vecState.push_back(pRock_On_Run_BL);

	/* 13번 락온모드일때 'S' 키를 누를 시 아래 대각선 오른쪽 이동 모션 */
	CRock_On_Run_B* pRock_On_Run_B = CRock_On_Run_B::Create();
	m_vecState.push_back(pRock_On_Run_B);

	/* 14번 락온모드일때 'W' 키를 누를 시 앞 이동 모션 */
	CRock_On_Run_F* pRock_On_Run_F = CRock_On_Run_F::Create();
	m_vecState.push_back(pRock_On_Run_F);

	/* 15번 락온모드일때 'SPACE' 키를 누를 시 뒤로 회피 모션 */
	CLockOn_Evade_B* LockOn_Evade_B = CLockOn_Evade_B::Create();
	m_vecState.push_back(LockOn_Evade_B);

	/* 16번 락온모드일때 'A' 키를 누르면서 'SPACE' 키를 누를 시 옆으로 회피 모션*/
	CLockOn_Evade_L* LockOn_Evade_L = CLockOn_Evade_L::Create();
	m_vecState.push_back(LockOn_Evade_L);

	/* 17번 락온모드일때 'D 키를 누르면서'SPACE' 키를 누를 시 옆으로 회피 모션*/
	CLockOn_Evade_R* LockOn_Evade_R = CLockOn_Evade_R::Create();
	m_vecState.push_back(LockOn_Evade_R);

	/* 18번 락온모드일때 'W' 키를 누르면서 'SPACE' 키를 누를 시 옆으로 회피 모션*/
	CLockOn_Evade_F* LockOn_Evade_F = CLockOn_Evade_F::Create();
	m_vecState.push_back(LockOn_Evade_F);
#pragma endregion 
#pragma region 패링 기본 모션 
	/* 19번 패링 모션 'F' 키를 누를 시 왼쪽 손이 오른쪽 사선으로 올라가면서 하는 패링 모션*/
	CParry_L* Parry_L = CParry_L::Create();
	m_vecState.push_back(Parry_L);

	/* 20번 패링 모션 'F' 키를 누를 시 오른쪽 손이 왼쪽 사선으로 내려가면서 하는 패링 모션*/
	CParry_R* Parry_R = CParry_R::Create();
	m_vecState.push_back(Parry_R);

#pragma endregion 
#pragma region 피격
	/* 21번 피격 모션 (왼쪽으로 고개를 휘청이면서 뒤로가는 모션 ) */
	CHurtMFL* pHurtMFL = CHurtMFL::Create();
	m_vecState.push_back(pHurtMFL);

	/* 22번 피격 모션 (오른쪽으로 고개를 휘청이면서 뒤로가는 모션 ) */
	CHurtMFR* pHurtMFR = CHurtMFR::Create();
	m_vecState.push_back(pHurtMFR);
#pragma endregion 
#pragma region 패링 성공 모션 

	/* 23번 패링 성공  모션 ( 뒤로밀려 가면서 휘청이는 모션 ) */
	CParryDeflect_Large* pParryDeflect_Large = CParryDeflect_Large::Create();
	m_vecState.push_back(pParryDeflect_Large);

	/* 24번 패링 성공 모션  ( 오른쪽 위 사선에서 왼쪽으로 칼을 내리면서 막는 패링 모션 )*/
	CParryDeflect_L* pParryDeflect_L = CParryDeflect_L::Create();
	m_vecState.push_back(pParryDeflect_L);

	/* 25번 패링 성공 모션  ( 오른쪽 아래에서 왼쪽 위 대각선으로 칼을 올리면서 막는 패링 모션)*/
	CParryDeflect_LUp* pParryDeflect_LUp = CParryDeflect_LUp::Create();
	m_vecState.push_back(pParryDeflect_LUp);

	/* 26번 패링 성공 모션 ( 왼쪽 위 사선에서 오른쪽 으로 칼을 내리면서 막는 패링 모션) */
	CParryDeflect_R* pParryDeflect_R = CParryDeflect_R::Create();
	m_vecState.push_back(pParryDeflect_R);

	/* 27번 패링 성공 모션 ( 왼쪽 아래 에서 오른쪽 위로 칼을 올리면서 막는 패링 모션 )*/
	CParryDeflect_RUp* pParryDeflect_RUp = CParryDeflect_RUp::Create();
	m_vecState.push_back(pParryDeflect_RUp);

#pragma endregion 
#pragma region 회피 모션 
	/* 28번 오른쪽 회피 대쉬 ( 오른쪽 키 + 스페이스 바 ) 모션 */
	CNormal_Evade_R* pNormal_Evade_R = CNormal_Evade_R::Create();
	m_vecState.push_back(pNormal_Evade_R);

	/* 29번 오른쪽 회피 대쉬 ( 오른쪽 키 + 스페이스 바 ) 모션 */
	CNormal_Evade_L* pNormal_Evade_L = CNormal_Evade_L::Create();
	m_vecState.push_back(pNormal_Evade_L);

	/* 30번 앞 오른쪽  대쉬 ( 위키 +  오른쪽 키 + 스페이스 바 ) 모션 */
	CNormal_Evade_FR* pNormal_Evade_FR = CNormal_Evade_FR::Create();
	m_vecState.push_back(pNormal_Evade_FR);

	/* 31번 앞 왼쪽  대쉬 ( 위키 +  왼쪽 키 + 스페이스 바 ) 모션 */
	CNormal_Evade_FL* pNormal_Evade_FL = CNormal_Evade_FL::Create();
	m_vecState.push_back(pNormal_Evade_FL);

	/* 32번 앞 대쉬 ( 위키 + 스페이스 바 ) 모션 */
	CNormal_Evade_F* pNormal_Evade_F = CNormal_Evade_F::Create();
	m_vecState.push_back(pNormal_Evade_F);

	/* 33번 뒤 오른쪽  대쉬 ( 뒤 키 +  오른쪽 키 + 스페이스 바 ) 모션 */
	CNormal_Evade_BR* pNormal_Evade_BR = CNormal_Evade_BR::Create();
	m_vecState.push_back(pNormal_Evade_BR);

	/* 34번 뒤 오른쪽  대쉬 ( 뒤 키 +  왼쪽 키 + 스페이스 바 ) 모션 */
	CNormal_Evade_BL* pNormal_Evade_BL = CNormal_Evade_BL::Create();
	m_vecState.push_back(pNormal_Evade_BL);

	/* 35번 뒤 대쉬 ( 뒤 키 + 스페이스 바 ) 모션 */
	CNormal_Evade_B* pNormal_Evade_B = CNormal_Evade_B::Create();
	m_vecState.push_back(pNormal_Evade_B);
#pragma endregion 

#pragma region 추가 피격 모션 
	/* 36번 피격 모션 넉백  모션 */
	CHurtKnockBack* pHurtKnockBack = CHurtKnockBack::Create();
	m_vecState.push_back(pHurtKnockBack);

	/* 37번 피격 모션 넉다운  모션 */
	CHurtKnockDown* pHurtKnockDown = CHurtKnockDown::Create();
	m_vecState.push_back(pHurtKnockDown);

	/* 38번 피격 모션 왼쪽 움찔  모션 */
	CHURTLF* pHURTLF = CHURTLF::Create();
	m_vecState.push_back(pHURTLF);

	/* 39번 피격 모션 HURT_SF 움찔  모션 */
	CHURTSF* pHURTSF = CHURTSF::Create();
	m_vecState.push_back(pHURTSF);

	/* 40번 피격 모션 HURT_XXLF */
	CHURTXXLF* pHURTXXLF = CHURTXXLF::Create();
	m_vecState.push_back(pHURTXXLF);

	/* 41번 피격 모션 */
	CHURTSL* pHURTSL = CHURTSL::Create();
	m_vecState.push_back(pHURTSL);

	/* 42번 피격 모션 */
	CHURT_FallDown* pHURT_FallDown = CHURT_FallDown::Create();
	m_vecState.push_back(pHURT_FallDown);
#pragma endregion 


#pragma region 좌클릭 공격 모션 4타, 5타 
	/* 43번 좌클릭 4타 공격 모션 */
	CAttack_LButton_4* pAttack_LButton_4 = CAttack_LButton_4::Create();
	m_vecState.push_back(pAttack_LButton_4);

	/* 44번 좌클릭 5타 공격 모션 */
	CAttack_LButton_5* pAttack_LButton_5 = CAttack_LButton_5::Create();
	m_vecState.push_back(pAttack_LButton_5);

#pragma endregion 


#pragma region 처형 모션
	/* 45번 HArmorLV1 처형 모션 */
	CHArmorLV1_Execution* pHArmorLV1_Execution = CHArmorLV1_Execution::Create();
	m_vecState.push_back(pHArmorLV1_Execution);
#pragma endregion 


#pragma region 몬스터가 패링에 성공할 시  

	/* 46번 몬스터가 플레이어 공격 패링 성공할 시 */
	CRebound_R* pRebound_R = CRebound_R::Create();
	m_vecState.push_back(pRebound_R);

#pragma endregion 

	/* 47번 몬스터가 스턴 공격을 넣어서 플레이어 기절 모션 */
	CStunnedStart* pStunnedStart = CStunnedStart::Create();
	m_vecState.push_back(pStunnedStart);

#pragma region  의자 상호작용 관련 
	/* 48번 의자 앉는 모션 시작 */
	CArchiveSitStart* pArchiveSitStart = CArchiveSitStart::Create();
	m_vecState.push_back(pArchiveSitStart);

	/* 49번 의자 앉고 있는 중인 모션 */
	CArchiveSitLoop* pArchiveSitLoop = CArchiveSitLoop::Create();
	m_vecState.push_back(pArchiveSitLoop);

	/* 50번 의자에서 일어나는 모션 */
	CArchiveGetUp* pArchiveGetUp = CArchiveGetUp::Create();
	m_vecState.push_back(pArchiveGetUp);

	/* 51번 의자에 조명 불 키는 모션 */
	CArchiveSitLight_Up* pArchiveSitLight_Up = CArchiveSitLight_Up::Create();
	m_vecState.push_back(pArchiveSitLight_Up);

#pragma endregion 

#pragma region 우클릭 차지 공격 

	/* 52번 우클릭 차지 시작 시작 */
	CClawCommon_ChargeStart* pClawCommon_ChargeStart = CClawCommon_ChargeStart::Create();
	m_vecState.push_back(pClawCommon_ChargeStart);

	/* 53번 우클릭 차지 중인 모션 */
	CClawCommon_ChargeLoop* pClawCommon_ChargeLoop = CClawCommon_ChargeLoop::Create();
	m_vecState.push_back(pClawCommon_ChargeLoop);

	/* 54번 우클릭 차지 공격 모션 */
	CClawCommon_ChargeFull* pClawCommon_ChargeFull = CClawCommon_ChargeFull::Create();
	m_vecState.push_back(pClawCommon_ChargeFull);


#pragma endregion 


#pragma region 바그한테 잡히는 모션 

	/* 55번 우클릭 차지 공격 모션 */
	CVarg_Catched* pVarg_Catched = CVarg_Catched::Create();
	m_vecState.push_back(pVarg_Catched);

#pragma endregion 


#pragma region 사다리 관련 모션 
	/* 56번 사다리 처음 올라가는 start 모션 */
	CLadder_Climb_Start* pLadder_Climb_Start = CLadder_Climb_Start::Create();
	m_vecState.push_back(pLadder_Climb_Start);

	/* 57번 사다리 왼쪽 내려가는 모션 */
	CLadder_Climb_L_Down* pLadder_Climb_L_Down = CLadder_Climb_L_Down::Create();
	m_vecState.push_back(pLadder_Climb_L_Down);

	/* 58번 사디리  왼쪽 다음 오른쪽 올라가기 끝낫을때 모션  */
	CLadder_Climb_R_Up_End* pLadder_Climb_R_Up_End = CLadder_Climb_R_Up_End::Create();
	m_vecState.push_back(pLadder_Climb_R_Up_End);

	/* 59번 사다리 처음 내려가는 모션(역재생으로함 내려갈때만) && 왼쪽 다음 오른쪽 올라가기 끝낫을때 모션 */
	CLadder_Climb_R_Up_Reverse_End* pLadder_Climb_R_Up_Reverse_End = CLadder_Climb_R_Up_Reverse_End::Create();
	m_vecState.push_back(pLadder_Climb_R_Up_Reverse_End);

	/* 60번 사다리 왼쪽 손 올라가는 모션 */
	CLadder_Climb_L_Up* pLadder_Climb_L_Up = CLadder_Climb_L_Up::Create();
	m_vecState.push_back(pLadder_Climb_L_Up);

	/* 61번 사다리 오른쪽 손 올라가는 모션 */
	CLadder_Climb_R_Up* pLadder_Climb_R_Up = CLadder_Climb_R_Up::Create();
	m_vecState.push_back(pLadder_Climb_R_Up);

	/* 62번 사다리 오른쪽 내려가는 모션 */
	CLadder_Climb_R_Down* pLadder_Climb_R_Down = CLadder_Climb_R_Down::Create();
	m_vecState.push_back(pLadder_Climb_R_Down);

#pragma endregion 

#pragma region 오두르 관련 모션
	/* 63번 오두르 한테 스킬 맞아서 당하는 모션 */
	CMagician_LV1_Catched* pMagician_LV1_Catched = CMagician_LV1_Catched::Create();
	m_vecState.push_back(pMagician_LV1_Catched);

#pragma endregion 


#pragma region 플레이어 달리기 모션 
	/*64번 플레이어 스프린트 달리기 모션 */
	CPlayerSprint* pPlayerSprint = CPlayerSprint::Create();
	m_vecState.push_back(pPlayerSprint);

#pragma endregion 

#pragma region 변이 오두르 관련 모션
	/* 65번 오두르 한테 스킬 맞아서 당하는 모션 */
	CMagician_LV2_Catched* pMagician_LV2_Catched = CMagician_LV2_Catched::Create();
	m_vecState.push_back(pMagician_LV2_Catched);
#pragma endregion


#pragma region 리서처에게 잡힘 당했을 때의 모션 
	/* 66번 리서처에게 잡힘 당했을 때의 모션 */
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

#include "pch.h" 
#include "Camera_Free.h"
#include "GameInstance.h"
#include "Player.h"
#include "Layer.h"


// 보간(Fade) 함수: 부드러운 변화
float Fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

// 선형 보간(Lerp) 함수	
float Lerp(float a, float b, float t) {
	return a + t * (b - a);
}

// Gradient 계산 함수
float Grad(int hash, float x, float y) {
	int h = hash & 3;  // 0~3 값 사용
	float u = h < 2 ? x : y;
	float v = h < 2 ? y : x;
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

// 펄린 노이즈 계산 함수 (2D)
float PerlinNoise(float x, float y) {
	// 정수 좌표 찾기
	int xi = (int)floor(x) & 255;
	int yi = (int)floor(y) & 255;

	// 소수점 부분 (격자 내 위치)
	float xf = x - floor(x);
	float yf = y - floor(y);

	// 보간 함수 적용
	float u = Fade(xf);
	float v = Fade(yf);

	// 해시 테이블 (임의 값)
	static int p[512];
	static bool init = false;
	if (!init) {
		for (int i = 0; i < 256; ++i) p[256 + i] = p[i] = i;
		init = true;
	}

	// 네 개의 코너 포인트
	int aa = p[p[xi] + yi];
	int ab = p[p[xi] + yi + 1];
	int ba = p[p[xi + 1] + yi];
	int bb = p[p[xi + 1] + yi + 1];

	// Gradient 값 적용
	float gradAA = Grad(aa, xf, yf);
	float gradBA = Grad(ba, xf - 1, yf);
	float gradAB = Grad(ab, xf, yf - 1);
	float gradBB = Grad(bb, xf - 1, yf - 1);

	// 보간 (Interpolation)
	float lerpX1 = Lerp(gradAA, gradBA, u);
	float lerpX2 = Lerp(gradAB, gradBB, u);
	return Lerp(lerpX1, lerpX2, v);
}

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{

	strcpy_s(m_szName, "Camera_Free");

	FREE_CAMERA_DESC* pDesc = static_cast<FREE_CAMERA_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	map<const _wstring, class CLayer*>* mapLayer = m_pGameInstance->Get_Layers();	
	auto& LevelLayer = mapLayer[pDesc->iCurLevel];	/* 애가 LEVEL_TUTORIAL의 Layer 집합 이고 */	


	CLayer* pLayer = LevelLayer.find(TEXT("Layer_Player"))->second;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pLayer->Get_GameObject_List().front());
	m_pPlayer = pPlayer;

	m_pPlayerTransformCom = dynamic_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pPlayerTransformCom);

	_fvector CameraPos =
	{
		 XMVectorGetX(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),	
		 XMVectorGetY(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) + 1.5f, // 머리 높이 보정				
		 XMVectorGetZ(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
		 1.0f
	};

	_matrix RotationMatrix = XMMatrixRotationAxis(XMVector3Normalize(m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT)), XMConvertToRadians(30.f));
	_vector CamDir = XMVector3Normalize(m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	CamDir = XMVector3TransformNormal(CamDir, RotationMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, CamDir);

	/* 해당 CamDir 저장해 놓기  신 마다 */
	XMStoreFloat4(&m_fCamFirstDir, CamDir);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, CameraPos);

	XMStoreFloat4(&m_fLerpPlayerHeadPos, CameraPos); // 이게 Look을 바꿔버리네 이걸 수정해야것다.	

	/* 카메라 툴 내용 읽어오기  */
	vector<Camera_Event> test;

	_ulong dwByte = {};

	//m_mapCamera_Event.emplace("Camera_Test", m_vec)
	HANDLE hFile = CreateFile(TEXT("../Camera_Bin/newExecute3.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);	
	while (true)	
	{
		Camera_Event pEventDesc{};


		ReadFile(hFile, pEventDesc.Cam_Name, MAX_PATH, &dwByte, nullptr);

		if (dwByte == 0)
			break;

		/* 카메라 툴에 있는 캠의 월드 행렬 가져오기*/
		ReadFile(hFile, &pEventDesc.Cam_WorldMatrix, sizeof(_float4x4), &dwByte, nullptr);

		/* 카메라 툴에 있는 상대 위치 가져오기 */
		ReadFile(hFile, &pEventDesc.Cam_RelativePos, sizeof(_float4), &dwByte, nullptr);

		/* 카메라 툴에 있는 보간 시간 가져오기*/
		ReadFile(hFile, &pEventDesc.Cam_LerpTime, sizeof(_float), &dwByte, nullptr);

		/* 카메라 툴에 있는 보간 속도 가져오기*/
		ReadFile(hFile, &pEventDesc.Cam_LerpSpeed, sizeof(_float), &dwByte, nullptr);

		/* 카메라 툴에 있는 보간 방식 가져오기*/
		ReadFile(hFile, &pEventDesc.Cam_LerpMethod, sizeof(_uint), &dwByte, nullptr);

		test.push_back(pEventDesc);
	}
	CloseHandle(hFile);

	m_mapCamera_Event.emplace(TEXT("VillageM1_Execute"), test);	

	return S_OK;
}


void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->isKeyEnter(DIK_F4))
	{
		m_bDebugCameraOnOff = !m_bDebugCameraOnOff;
	}

	if (!m_bDebugCameraOnOff && !(m_pPlayer->Get_PhaseState() & CPlayer::PHASE_INTERACTION) && !m_pPlayer->Get_UI_End())	
	{
#pragma region 카메라 움직임 코드 

		m_fTimeDelta = fTimeDelta;

		if (m_pGameInstance->isMouseEnter(DIM_MB))
		{
			if (m_pTargetMonster == nullptr)
			{
				m_pTargetMonster = Find_LockOnTarget();

				if (m_pTargetMonster == nullptr)
					m_bCamLockOnOff = false;

				else
				{
					m_bCamLockOnOff = true;
					m_pTargetMonster->Set_Locked_On(true);
					m_pPlayer->Set_Lockon(true);
				}
			}

			else
			{
				m_bCamLockOnOff = false;
				m_pTargetMonster->Set_Locked_On(false);
				m_pPlayer->Set_Lockon(false);
				m_pTargetMonster = nullptr;
			}


		}


		/* ============== 카메라 툴에서 만든 컷신 ========================= */

		if (m_bCamera_Cut_Scene_OnOff)
		{
			switch (m_iExectueCameraScene)
			{
			case MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1:
				Camera_Cut_Scene_Activate(TEXT("VillageM1_Execute"));
				break;
			default:
				break;
			}
		}
		/* ======================================= */


		if (m_bCamera_Cut_Scene_OnOff == false)
		{
			_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
			_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

			// 플레이어의 충돌체를 기준으로할까.	

			XMStoreFloat4(&m_fPlayerHeadPos, XMVectorSet(
				XMVectorGetX(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
				XMVectorGetY(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) + 1.f, // 머리 높이 보정	 // 3월 19일 수정
				XMVectorGetZ(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
				1.0f
			));

			//if(m_bCamera_Cut_Scene_OnOff == true)
			if (m_bGetBackCamPos)
			{
				XMStoreFloat4(&m_fLerpPlayerHeadPos, XMVectorSetW(XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&m_fPlayerHeadPos), m_fLerpTime), 1.f));	// 자기 자신을 보간하므로 계속해서 값이 증가하거나 감소해서 변함
				m_bGetBackCamPos = false;
			}
			else
				XMStoreFloat4(&m_fLerpPlayerHeadPos, XMVectorLerp(XMLoadFloat4(&m_fLerpPlayerHeadPos), XMLoadFloat4(&m_fPlayerHeadPos), m_fLerpTime));

			// 카메라 이동 처리
			_vector vCamDir = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			_vector vCamPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector vNewCamPos = XMLoadFloat4(&m_fPlayerHeadPos) - vCamDir * m_fCurCamDistance;

			_vector vLerpCamPos = XMVectorLerp(vCamPosition, vNewCamPos, m_fLerpTime);


			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLerpCamPos);	 // 위치를 이렇게 세팅하니깐 다시 위에서는 vCamPosition 값이 증가하게되어 보간효과 o 


			/* 이동속도를 줌으로써 계산을 진행? */
			float CosTheta = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
			float OppostieCosTheta = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f, XMVectorSet(0.f, -1.f, 0.f, 0.f)));
			//angleRadians = acosf(cosTheta); // 라디안 값	
			float AngleDegrees = XMConvertToDegrees(acosf(CosTheta));         // 각도를 도(degree)로 변환							
			float OppositeAngleDegrees = XMConvertToDegrees(acosf(OppostieCosTheta)); // 각도를 도(degree)로 변환			




			/* 평상시 카메라 */
			if (!m_bCamLockOnOff)
			{

				/*if (m_pGameInstance->isKeyEnter(DIK_TAB))
				{
					if (m_bStop)
						m_bStop = false;
					else
						m_bStop = true;
				}*/

				if (!m_bStop)
				{

					if (MouseMoveX != 0 || MouseMoveY != 0)
					{
						// X축(Y축 기준 회전)
						if (MouseMoveX != 0)
						{
							m_pTransformCom->Orbit_Move(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMoveX * fTimeDelta * m_fMouseSensor, XMLoadFloat4(&m_fLerpPlayerHeadPos));
						}

						// Y축(Right 축 기준 회전)
						if (MouseMoveY != 0)
						{

							if (AngleDegrees > 40.f && OppositeAngleDegrees > 80.f)
								m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, XMLoadFloat4(&m_fLerpPlayerHeadPos));


							else if (AngleDegrees < 40.f && MouseMoveY < 0.f)
							{
								m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, XMLoadFloat4(&m_fLerpPlayerHeadPos));
							}

							else if (OppositeAngleDegrees < 80.f && MouseMoveY > 0.f)
							{
								m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, XMLoadFloat4(&m_fLerpPlayerHeadPos));
							}
						}
					}
				}
			}
			/* ======================================================= */

			/* 락온 카메라 */
			else // Lock On 카메라 상태 
			{

				/* 삭제이벤트가 발생했다면 m_pTargetMonster가 nullptr가 되어야함. */
				if (m_pTargetMonster->Get_Monster_State() == STATE_EXECUTION || m_pTargetMonster->Get_Monster_State() == STATE_DEAD)
				{

					m_bCamLockOnOff = false;

					m_pPlayer->Set_Lockon(false);
					m_pTargetMonster->Set_Locked_On(false);

					m_pTargetMonster = nullptr;

				}

				else if (m_pTargetMonster->Get_Monster_State() != STATE_EXECUTION)
				{
					/* 플레이어 회전 관련 */
					_vector PlayerPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);
					_vector MonsterPos = m_pTargetMonster->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

					_vector vDir = (MonsterPos - PlayerPos);

					_vector vPlayerLook = m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK);

					_vector vLookPlayerXZ = XMVector2Normalize(_fvector{ vPlayerLook.m128_f32[0] ,vPlayerLook.m128_f32[2] }); // 이게 플레이어 xz 룩방향
					_vector vDirLook = XMVector2Normalize(_fvector{ vDir.m128_f32[0],vDir.m128_f32[2] });// 이게 현재 플레이어의 위치에서 몬스터 방향

					_float  dotResult = XMVectorGetX(XMVector2Dot(vLookPlayerXZ, vDirLook));
					dotResult = max(-1.0f, min(dotResult, 1.0f));
					float Radian = acosf(dotResult);

					_vector crossResult = XMVector2Cross(vDirLook, vLookPlayerXZ);
					float crossY = XMVectorGetY(crossResult);
					if (crossY < 0.0f) {
						Radian = -Radian;

					}
					/* =============================================== */


					/* 카메라 회전 관련 */





					/* 카메라 방향 전환 관련 */

					//m_pTransformCom->Orbit_Move_Once(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian* fTimeDelta, m_vLerpPlayerHeadPos);	
					LockOnCameraTurn(fTimeDelta);



					/* 플레이어 방향 전환 */
					//if (abs(Radian) > 0.1f)
					m_pPlayerTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, Radian * fTimeDelta * 10.f);


				}

			}
			/* ===================================================================== */

			// Look 벡터 갱신 (플레이어 머리 위치를 바라보도록 설정)
			_vector CamDir = XMVectorSetW(XMVector3Normalize(XMLoadFloat4(&m_fLerpPlayerHeadPos) - m_pTransformCom->Get_State(CTransform::STATE_POSITION)), 0.f);
			//m_pTransformCom->Set_State(CTransform::STATE_LOOK, CamDir);

			// Right 벡터 갱신 (Up × Look)
			_vector vUp = m_pPlayerTransformCom->Get_State(CTransform::STATE_UP);
			_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, CamDir));

			// Up 벡터 갱신 (Look × Right)
			_vector vNewUp = XMVector3Normalize(XMVector3Cross(CamDir, vRight));

			// Transform 갱신
			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
			m_pTransformCom->Set_State(CTransform::STATE_UP, vNewUp);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, CamDir);


		}

		_vector vCamDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK);


		/* 여기에 이벤트가 들어와야함. */
		if (m_bShakeOnOff)
		{
			_vector CamPos = Camera_Shake(fTimeDelta, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CamPos);
		}


		if (m_bZoomIn)
		{
			if (m_fCamCloseLimitDistance < m_fCurCamDistance)
				m_fCurCamDistance -= fTimeDelta * m_fZoomInSpeed;	

			_vector vNewCamPos = XMLoadFloat4(&m_fLerpPlayerHeadPos) - vCamDir * m_fCurCamDistance;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewCamPos);

		}


		if (m_bZoomOut)
		{
			if (m_fCamFarLimitDistance > m_fCurCamDistance)
				m_fCurCamDistance += fTimeDelta * m_fZoomOutSpeed;	

			_vector vNewCamPos = XMLoadFloat4(&m_fLerpPlayerHeadPos) - vCamDir * m_fCurCamDistance;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewCamPos);

		}



		m_fZoomInSpeed = 1.f;
		m_fZoomOutSpeed = 1.f;
		m_bShakeOnOff = false;
		m_bZoomIn = false;
		m_bZoomOut = false;


		__super::Priority_Update(fTimeDelta);

#pragma endregion 
	}
}

void CCamera_Free::Update(_float fTimeDelta)
{

}

void CCamera_Free::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera_Free::Render()
{

	return S_OK;
}

CGameObject* CCamera_Free::Find_LockOnTarget()
{
	m_maptMonsterDistance.clear();
	/* 여기서 가장 가까운 몬스터를 리턴 해 줘야한다.*/
	for (auto& iter : m_pGameInstance->Get_Check_Monsters())
	{
		_uint iCheck = static_cast<_uint>(m_pGameInstance->Get_Check_Monsters().size());

		// 절두체에 안에 있어야 하며, 그 중 가장 거리가 가까운놈을 가져와야함. 
		_vector MonsterPosition = iter->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

		if (m_pGameInstance->isIn_Frustum_WorldSpace(MonsterPosition))
		{
			_vector PlayerPosition = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

			// 절두체 컬링을 해도 화면에 가려지느냐 안가려지느냐를 체크 안했네. 
			_float  Distance = XMVectorGetX(XMVector3Length(MonsterPosition - PlayerPosition));
			// 거리 제한 
			if (Distance < 25.f)
				m_maptMonsterDistance.insert(pair<_float, CGameObject*>(Distance, iter));
		}
	}

	if (m_maptMonsterDistance.size() >= 1)
	{
		auto& Pair = m_maptMonsterDistance;

		m_pPlayer->Set_Lockon(true);	
		m_pPlayer->Set_LockOnTargetMonsterPtr(Pair.begin()->second);	
		return Pair.begin()->second;
	}

	else if (m_maptMonsterDistance.size() == 0)
	{
		m_pPlayer->Set_Lockon(false);
		return nullptr;
	}

	return nullptr;		
}

void CCamera_Free::LockOnCameraTurn(_float fTimeDelta)
{
	///* 카메라 회전 관련 */
	_vector vCamCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector MonsterPos = m_pTargetMonster->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
	_vector vUp = _fvector{ 0.f,1.f,0.f,0.f };
	_vector vCamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector vCamRightXZ = _fvector{ vCamRight.m128_f32[0],0.f,vCamRight.m128_f32[2] };


	// 플레이어의 look 의 x,z 방향 벡터와 몬스터의 look의 x,z 방향 벡터를 가져와서 내적하고 회전하면 될 것 같다. 
	_vector vDir = (MonsterPos - vCamCurPos);

	_vector vFrontLook = XMVector3Normalize(XMVector3Cross(vCamRightXZ, vUp));
	_vector vDirLook = XMVector3Normalize(_fvector{ vDir.m128_f32[0],0.f,vDir.m128_f32[2] });// 이게 현재 카메라의 위치에서 몬스터 방향					

	// XZ 평면의 2D 벡터 생성 (x는 그대로, y에 z 값을 넣음)
	_vector vFrontLook2D = _fvector{ vFrontLook.m128_f32[0], vFrontLook.m128_f32[2], 0.f, 0.f };
	_vector vDirLook2D = _fvector{ vDirLook.m128_f32[0],   vDirLook.m128_f32[2],   0.f, 0.f };

	_float dotResult = XMVectorGetX(XMVector2Dot(vFrontLook2D, vDirLook2D));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	// 2D cross: XMVector2Cross returns a vector where the z component is the scalar cross product.
	// 하지만 XMVector2Cross는 결과를 (0,0, cross, 0)로 넣습니다.
	_vector crossResult = XMVector2Cross(vDirLook2D, vFrontLook2D);
	float crossScalar = XMVectorGetZ(crossResult);  // 여기서는 Z 성분을 사용합니다.	
	if (crossScalar < 0.0f)
	{
		Radian = -Radian;
	}

	m_pTransformCom->Orbit_Move_Once(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * fTimeDelta * 10.f, XMLoadFloat4(&m_fLerpPlayerHeadPos));

}

void CCamera_Free::ShakeOn(_float _fXaxisShakeSpeed, _float _fZaxisShakeSpeed, _float _fXaxisMoveAmount, _float _fZaxisMoveAmount)
{
	m_bShakeOnOff = true;
	m_fXaxisShakeSpeed = _fXaxisShakeSpeed;
	m_fZaxisShakeSpeed = _fZaxisShakeSpeed;
	m_fXaxisMoveAmount = _fXaxisMoveAmount;
	m_fZaxisShakeSpeed = _fZaxisMoveAmount;
}

void CCamera_Free::ResetZoomInCameraPos(_float _fZoomInSpeed)
{
	const float fEpsilon = 0.1f; // 허용 오차 범위	


	if (fabs(m_fCamDistance - m_fCurCamDistance) > fEpsilon)
	{
		if (m_fCamDistance > m_fCurCamDistance)
			m_fCurCamDistance += m_fTimeDelta * _fZoomInSpeed;
	}

}

void CCamera_Free::ResetZoomOutCameraPos(_float _fZoomOutSpeed)
{
	const float fEpsilon = 0.1f; // 허용 오차 범위		


	if (fabs(m_fCamDistance - m_fCurCamDistance) > fEpsilon)
	{
		if (m_fCamDistance < m_fCurCamDistance)
			m_fCurCamDistance -= m_fTimeDelta * _fZoomOutSpeed;
	}
}

_vector CCamera_Free::Camera_Shake(float deltaTime, XMVECTOR& cameraPosition)
{
	static float shakeTime = 0.0f;
	shakeTime += deltaTime;

	// 펄린 노이즈 기반 흔들림 값 생성
	float offsetX = PerlinNoise(shakeTime * m_fXaxisShakeSpeed, 0.5f) * deltaTime * m_fXaxisMoveAmount;
	float offsetY = 0.f;//PerlinNoise(shakeTime * 240.0f, 1.0f) * deltaTime * 4.f;	
	float offsetZ = PerlinNoise(shakeTime * m_fZaxisShakeSpeed, 1.5f) * deltaTime * m_fZaxisMoveAmount;

	XMVECTOR shakeOffset = XMVectorSet(offsetX, offsetY, offsetZ, 0.0f);
	XMVECTOR newCameraPos = XMVectorAdd(cameraPosition, shakeOffset);


	return newCameraPos;
}

bool CCamera_Free::Camera_Cut_Scene_Activate(_wstring _CutSceneName)
{
	// 여기서 그러면 일단 해당 좌표가 오염되니깐 다른걸로 교체해야할듯 원본은 냅두고 
	auto& iter = m_mapCamera_Event.find(_CutSceneName);

	m_vecCamera_Event = iter->second;
	m_vecPlayingCamera_Event = m_vecCamera_Event;

	if (m_bCutSceneFirst)
	{
		/*플레이어의 xz좌표 */
		_vector pPlayerRightDir = { 1.f ,0.f,0.f,0.f };
		_vector pPlayerCurrentRightDir = XMVector3Normalize(m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT));

		_float dot = XMVectorGetX(XMVector3Dot(pPlayerRightDir, pPlayerCurrentRightDir));
		dot = max(-1.0f, min(dot, 1.0f));
		m_fRadian = acosf(dot);

		_vector crossResult = XMVector3Cross(pPlayerRightDir, pPlayerCurrentRightDir);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			m_fRadian = -m_fRadian;
		}
		m_bCutSceneFirst = false;
	}



	for (auto& iter : m_vecPlayingCamera_Event)
	{
		_float4x4 ConvertedWorldMatrix
			= m_pTransformCom->Orbit_Move_RelativePos(XMLoadFloat4x4(&iter.Cam_WorldMatrix),
				XMLoadFloat4(&iter.Cam_RelativePos),
				_fvector{ 0.f,1.f,0.f,0.f },
				m_fRadian,	 // 이걸 dot 해줘야 함.			
				m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));


		XMStoreFloat4x4(&iter.Cam_WorldMatrix, XMLoadFloat4x4(&ConvertedWorldMatrix));
	}


	/* 여기서 현재 카메라와 카메라컷신의 Cam_1의 보간이 끝나고 진행되어야함 */
#pragma region 카메라 첫번째 보간

	//if (m_bCutSceneFristLerpEnd)
	//{
#pragma endregion 
		/* ===============================================================  */

		m_fCutScene_CurTime += m_vecPlayingCamera_Event.at(m_iPlayCamera_Index).Cam_LerpSpeed * m_fTimeDelta;

		_float  fRatio = m_fCutScene_CurTime / m_vecPlayingCamera_Event.at(m_iPlayCamera_Index).Cam_LerpTime;

		_uint CamCount = static_cast<_uint>(m_vecPlayingCamera_Event.size());

		// 문제가 Cam_0 부터 시작하면 어색함 현재 카메라 위치부터 시작하도록 만들어야함.
		// 흠.
		if (fRatio <= 1.f)
		{

			_vector   vScale, vRotation, vTranslation;
			XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(&m_vecPlayingCamera_Event.at(m_iPlayCamera_Index).Cam_WorldMatrix));

			_vector    vNextScale, vNextRotation, vNextTranslation;
			XMMatrixDecompose(&vNextScale, &vNextRotation, &vNextTranslation, XMLoadFloat4x4(&m_vecPlayingCamera_Event.at(m_iPlayCamera_Index + 1).Cam_WorldMatrix));



			vScale = XMVectorLerp(vScale, vNextScale, fRatio);

			vRotation = XMQuaternionSlerp(vRotation, vNextRotation, fRatio);

			if (m_vecPlayingCamera_Event.at(m_iPlayCamera_Index).Cam_LerpMethod == 0)
				vTranslation = XMVectorSetW(XMVectorLerp(vTranslation, vNextTranslation, fRatio), 1.f);
			else
			{
				vTranslation = XMVectorSetW(CatmullRom_Position_Lerp(m_vecPlayingCamera_Event, fRatio), 1.f);
			}

			_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

			_float4x4 test = {};
			XMStoreFloat4x4(&test, TransformationMatrix);

			m_pTransformCom->Set_WorldMatrix(test);


		}

		else if (fRatio > 1.f)
		{
			if (m_iPlayCamera_Index < (CamCount - 2))
			{
				m_fCutScene_CurTime = 0.f;
				m_iPlayCamera_Index++;
			}

			else
			{
				m_bCutSceneFirst = true;	
				m_bCamera_Cut_Scene_OnOff = false;	
				m_bGetBackCamPos = true;	
				m_fCutScene_CurTime = 0.f;
				m_iPlayCamera_Index = 0;
			}
		}
	//}
	//
	//else
	//{
	//	FirstLerpFinish();
	//}


	return m_bCamera_Cut_Scene_OnOff;
}

void CCamera_Free::Target_Reset()	
{
	if (m_pTargetMonster != nullptr)	
		m_pTargetMonster->Set_Locked_On(false);	

	m_pTargetMonster = nullptr;	
	m_bCamLockOnOff = false;	
}

_vector CCamera_Free::CatmullRom_Position_Lerp(vector<Camera_Event> CameraEvent, _float _fRatio)
{
	_vector  p1 = { CameraEvent.at(m_iPlayCamera_Index - 1).Cam_WorldMatrix._41,
				   CameraEvent.at(m_iPlayCamera_Index - 1).Cam_WorldMatrix._42,
				   CameraEvent.at(m_iPlayCamera_Index - 1).Cam_WorldMatrix._43,
					1.f };

	_vector  p2 = { CameraEvent.at(m_iPlayCamera_Index).Cam_WorldMatrix._41,
				   CameraEvent.at(m_iPlayCamera_Index).Cam_WorldMatrix._42,
				   CameraEvent.at(m_iPlayCamera_Index).Cam_WorldMatrix._43,
					1.f };

	_vector  p3 = { CameraEvent.at(m_iPlayCamera_Index + 1).Cam_WorldMatrix._41,
					CameraEvent.at(m_iPlayCamera_Index + 1).Cam_WorldMatrix._42,
					CameraEvent.at(m_iPlayCamera_Index + 1).Cam_WorldMatrix._43,
					1.f };

	_vector  p4 = { CameraEvent.at(m_iPlayCamera_Index + 2).Cam_WorldMatrix._41,
					CameraEvent.at(m_iPlayCamera_Index + 2).Cam_WorldMatrix._42,
					CameraEvent.at(m_iPlayCamera_Index + 2).Cam_WorldMatrix._43,
					1.f };

	_vector position = 0.5f * ((2 * p2) +
		(-p1 + p3) * _fRatio +
		(2 * p1 - 5 * p2 + 4 * p3 - p4) * (_fRatio * _fRatio) +
		(-p1 + 3 * p2 - 3 * p3 + p4) * (_fRatio * _fRatio * _fRatio));


	position = XMVectorSetW(position, 1.f);

	return position;
}

void CCamera_Free::CutSceneEndLerp(_matrix pWorldMatrix)
{
	m_fEndLerpTime += m_fTimeDelta;

	float fRatio = m_fEndLerpTime / 1.f;

	if (fRatio <= 1.f)
	{

		_vector   vScale, vRotation, vTranslation;
		XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

		_vector    vNextScale, vNextRotation, vNextTranslation;
		XMMatrixDecompose(&vNextScale, &vNextRotation, &vNextTranslation, pWorldMatrix);

		vScale = XMVectorLerp(vScale, vNextScale, fRatio);

		vRotation = XMQuaternionSlerp(vRotation, vNextRotation, fRatio);

		vTranslation = XMVectorSetW(XMVectorLerp(vTranslation, vNextTranslation, fRatio), 1.f);

		_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

		_float4x4 test = {};
		XMStoreFloat4x4(&test, TransformationMatrix);

		m_pTransformCom->Set_WorldMatrix(test);
	}

	else if (fRatio > 1.f)
	{
		m_fEndLerpTime = 0.f;
		m_bGetBackCamPos = false;
	}

}

void CCamera_Free::FirstLerpFinish()
{
	m_fCutScene_CurTime += m_vecPlayingCamera_Event.at(0).Cam_LerpSpeed * m_fTimeDelta;

	_float  fRatio = m_fCutScene_CurTime / m_vecPlayingCamera_Event.at(0).Cam_LerpTime;



	if (fRatio <= 1.0f)
	{

		_vector   vScale, vRotation, vTranslation;
		XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(&m_pCurWorldMatrix)); // 아 이게 계속달라짐.		

		_vector    vNextScale, vNextRotation, vNextTranslation;
		XMMatrixDecompose(&vNextScale, &vNextRotation, &vNextTranslation, XMLoadFloat4x4(&m_vecPlayingCamera_Event.at(1).Cam_WorldMatrix));



		vScale = XMVectorLerp(vScale, vNextScale, fRatio);
		vRotation = XMQuaternionSlerp(vRotation, vNextRotation, fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(vTranslation, vNextTranslation, fRatio), 1.f);



		_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

		_float4x4 test = {};
		XMStoreFloat4x4(&test, TransformationMatrix);



		m_pTransformCom->Set_WorldMatrix(test);


	}

	else
	{
		m_fCutScene_CurTime = 0.f;
		m_bCutSceneFristLerpEnd = true;
	}


}


CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
    m_pGameInstance->Delete_Shadow(m_pTransformCom);

	__super::Free();

	Safe_Release(m_pPlayerTransformCom);
}


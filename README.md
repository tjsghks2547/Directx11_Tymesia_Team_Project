# Tymesia 모작 Team_Project
DirectX 11을 활용한 팀 프로젝트 게임 모작에서 직접 구현한 기능들의 샘플 코드입니다.

<div align="center">
  <img width="400" alt="티메시아사진" src="https://github.com/user-attachments/assets/df90f11b-ba83-4e05-89e8-303e8f67f96d" /> <br><br>
  개발기간 : 2개월 <br><br>
  📌해당 설명서는 팀원 개개인이 개별적으로 작성하기에 팀원 모두의 구현 내용을 담은 것이 아닙니다. <br><br>
</div>

# 동영상


# 👩‍💻담당 파트
- 김선환 : 플레이어, 카메라 , 애니메이션 툴, 카메라 툴, 엣지 네비게이션, 셰이더
- 강범승 : 몬스터 , Ai
- 김유빈 : UI 
- 이종한 : 이펙트 , 이펙트 툴 , 이펙트 셰이더 
- 이상혁 : 맵툴, 환경 셰이더 
- 한유인 : 맵툴, Npc , 상호작용 오브젝트


# 📖기술 스택 및 개발 환경
- DirectX11 SDK
- C++
- ImGui
- Assimp
- PhysX

# 💻구현 컨텐츠 및 기능 설명 
- PhysX 라이브러리를 사용해 게임 내 모든 충돌 처리 구현
  - Collision Enter / Stay / Exit 함수를 연결하여 해당 Object의 충돌 상태 확인
  - 충돌체 레이어를 활용하여 최적화

- 다양한 셰이더 구현
  - Zoom Blur / Color Inversion Shader / Dissolve / Glow

- 플레이어 상태 디자인 패턴을 통한 컨트롤 구현

- 카메라 연출
  - 카메라 툴 ( slerp 및 Cat-MullRom을 활용함) 
  - Perlin Noise 를 이용한 Camera Shake 기능
  - Zoom In / Zoom Out
 
- 애니메이션
   - 애니메이션 툴 ( 속도 및 보간 )
   - 루트 애니메이션


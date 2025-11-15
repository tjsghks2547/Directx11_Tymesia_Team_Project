# Directx11 Tymesia 모작 Team_Project
DirectX11을 활용한 팀 프로젝트 게임 모작에서 직접 구현한 기능들의 샘플 코드입니다.

<div align="center">
  <img width="400" alt="티메시아 사진" src="https://github.com/user-attachments/assets/4d6fc8db-fb7b-412a-9eaa-a48bc1068624" />
  <br><br>
  개발기간 : 2개월 <br><br>
  📌해당 설명서는 팀원 개개인이 개별적으로 작성하기에 팀원 모두의 구현 내용을 담은 것이 아닙니다. <br><br>
    작업기간 :  2025.02.12 ~ 2025.4.13
</div>

# 동영상
[https://youtu.be/_r2ZWqVJgvA]

# 👩‍💻담당 파트
- 김선환 : 플레이어, 카메라, 컷신 애니메이션 툴, 애니메이션, 엣지 네비게이션, 셰이더, 춛돌
- 강범승 : 몬스터 , Ai
- 이상혁 : Map Tool , 환경 셰이더 
- 한유인 : Map Tool,  맵 이동 및 상호작용 
- 김유빈 : UI , Item
- 이종한 : 이펙트 , 이펙트 툴 , 이펙트 셰이더
  
# 📖기술 스택 및 개발 환경
- DirectX9 SDK
- C++
- ImGui
- PhysX Library
- Assimp Library
- HLSL

# 💻구현 컨텐츠 및 기능 설명 

- 애니메이션
   - 애니메이션 툴 ( 애니메이션 속도 및 보간 속도, 시작 프레임위치, 끝나는 프레임 위치등을 설정 )
   - 루트 애니메이션
   - 애니메이션 역재생
   - 애니메이션 키 프레임 구간 속도 조절 기능
   - 애니메이션 프레임 단위로 사운드, 이펙트, 충돌 등의 이벤트를 자동 트리거하는 프레임 기반 이벤트 시스템을 구현.
  <br><br>
  ![Image](https://github.com/user-attachments/assets/4c20bdcb-09ec-494c-821e-5d9cfdcc3564)
     

- PhysX 라이브러리를 사용해 게임 내 모든 충돌 처리 구현
  - Collision Enter / Stay / Exit 함수를 연결하여 해당 Object의 충돌 상태 확인
  - 충돌체 레이어를 활용하여 최적화
<br><br>
![Image](https://github.com/user-attachments/assets/0dc8b743-c306-41cf-8d62-478ee2a4c96b)

- 다양한 셰이더 구현
  - Zoom Blur / Color Inversion Shader / Dissolve / Glow

- 플레이어 상태 디자인 패턴을 통한 컨트롤 구현


- 카메라 연출
  - 카메라 툴 ( slerp 및 Cat-MullRom을 활용함) 
  - Perlin Noise 를 이용한 Camera Shake 기능
  - Zoom In / Zoom Out
    
  - Lock On Camera <br><br>
  ![Image](https://github.com/user-attachments/assets/b00b6c60-c7c8-4e54-a02a-f62cb2ab09bb)


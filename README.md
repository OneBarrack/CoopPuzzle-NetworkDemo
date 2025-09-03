# 🎮 ARPG_ContentDemo
Unreal Engine 5 기반 **멀티플레이 RPG 프로토타입 데모**  
(Interaction / UI / Network Replication / Co-op Puzzle)

---

## 📌 프로젝트 개요
- **목표**: RPG 필수 시스템(상호작용·UI·퍼즐 요소)에 Dedicated Server + 클라이언트 환경 적용
- **개발 기간**: 2025.08.26 ~ 진행 중 (**9/3 기준**)  
- **설계 키워드**: Server-authoritative, RepNotify 중심 동기화, Enhanced Input, Subsystem 모듈화
- **핵심 루프**: 20–30초 시연 가능

---

## ✅ 현재 구현 범위
- **Interaction 시스템**  
  센서(`UACDInteractionSensorComponent`) / 인터페이스(`IACDInteractionInterface`) / **인터랙터블**(`UACDInteractableComponent`)
- **상호작용 액터**  
  문(`AACDDoor`), 레버(`AACDLever`), 상자(`AACDChest`)
- **퍼즐 요소**  
  - **BP-only**: 발판 + 큰 문(임시 개방, 시작 시점 RepNotify + 타임라인 동기 실행)
  - **C++ 액터**: 레버(영구 개방 트리거), 작은 문(열기/닫기 토글)
- **UI**  
  HUD 생성 및 **상호작용 프롬프트** 표시
- **Subsystem**  
  DataRegistry / QuestManager / ItemManager / InteractionManager / UIManager  
  *(현재는 초기화·HUD 보장·로그 중심, 추후 확장 예정)*
- **PlayerState**  
  구조만 마련 *(향후 퀘스트/인벤토리 진행도 저장)*

---

## 🕹️ 시연 루프 (데모 시나리오 — **골격 구현 기준**)

### ✅ 현재 구현된 흐름
1. **발판 + 큰 문 (Co-op 퍼즐)**  
   - A: 발판 밟음 → 큰 문 **임시 개방(BP-only)**  
   - B: 문 통과 후 **레버(C++)** 작동 → 문 **영구 개방(발판 무시)**
2. **레버 상호작용(C++)**  
   - 상태 토글 + 델리게이트 브로드캐스트
3. **상자 상호작용(C++)**  
   - 현재 **“열기” 상호작용만 구현** *(보상 지급/인벤토리 반영 없음)*
4. **UI 표시**  
   - HUD 생성, **프롬프트** 정상 동작

### 🔜 예정된 확장
- **NPC 대화 → 퀘스트 수락**: 현재는 로그/프롬프트 대체, QuestManager 연동
- **퀘스트 진행/완료 처리**: 카운트·완료 UI, PlayerState 연계 저장
- **상자 보상 획득**: 아이템 지급 → 인벤토리 UI 반영
- **토스트 메시지 시스템**: 진행/완료/보상 이벤트 시각화

---

## 🛠️ 시스템 구조

### Interaction
- `UACDInteractionSensorComponent`: 서버에서 후보 탐색·주기 평가, `CurrentTargetActor` **OwnerOnly 복제**, `ForceUpdate` 지원  
- `UACDInteractableComponent`: `Prompt/Action/RemainingUseCount` RepNotify, `OnInteracted`/`OnChangedInteractInfo` 델리게이트  
- `IACDInteractionInterface`: `CanInteract` / `DoInteract` 계약  
- **상호작용 액터 (Door / Lever / Chest)**: C++로 **상태 관리**, **연출은 BP**에서 구현

### 퍼즐 요소
- **발판 (BP-only)**: 서버 권한 Overlap Count → 큰 문 열림/닫힘 시작 트리거  
- **큰 문 (BP-only)**: 서버/클라이언트 **모두 타임라인 재생**  
  - RepNotify로 열림/닫힘 **시작 시점** 동기화  
  - 서버는 충돌(Blocking) 제어 권한 유지, 클라에서는 동일 타임라인으로 시각 연출  
- **레버 (C++, 상호작용)**: `bIsOn` 서버 플래그 → **큰 문 영구 개방**  
- **작은 문 (C++, 상호작용)**: `bIsOpen` 서버 플래그 → **열기/닫기** 토글

> **요약**: 서버가 상태와 시작 시점을 확정 → 서버·클라 모두 같은 타임라인 실행으로 연출·충돌 동기화

### 캐릭터 & 컨트롤러
- `AACDCharacterBase`: 이동/점프/카메라 (Enhanced Input)  
- `AACDCharacter`: Interact 입력 → **서버 RPC** 요청  
- `AACDPlayerController`: Sensor 타깃 변경 수신 → Prompt 갱신 → UIManager 전달, UI 모드 토글

### 매니저 & 서브시스템
- `UACDDataRegistry`: DataTable 초기화(로그)  
- `UACDQuestManager / UACDItemManager / UACDInteractionManager`: **구조 준비**(추후 로직 연동)  
- `UACDUIManager (LocalPlayerSubsystem)`: HUD 보장 생성 + **Prompt API**  
  *(토스트/퀘스트 UI API는 계획 단계)*

### UI & HUD
- `UACDPlayerHUDWidget`: C++ API 시그니처 제공, 연출은 BP에서 구현

---

## 🌐 네트워크 고려 사항
- **입력**: 클라 → 서버 RPC → 서버에서 `CanInteract` 검증 후 실행  
- **상태 동기화**: Interactable 상태는 **서버에서만 변경** → RepNotify/Multicast 반영  
- **Sensor 타깃**: **OwnerOnly 복제**(본인 전용 프롬프트)  
- **Co-op 퍼즐**:  
  - 서버가 열림/닫힘 상태와 시작 시점을 확정  
  - 서버·클라 모두 타임라인을 실행 → 동일한 열림/닫힘 연출  
  - 서버는 충돌 제어 권한 유지

---

## 🔜 네트워크 보강 예정
- 거리/시야(LineTrace) 기반 검증 강화  
- 센서 후보 우선순위 개선  
- 짧은 쿨다운 타이머 적용

---

## 📂 코드 & BP 맵
```plaintext
/Common
  ACDEnum
/Character
  ACDCharacterBase / ACDCharacter
/PlayerController
  ACDPlayerController
/PlayerState
  ACDPlayerState
/Component
  ACDInteractionSensorComponent / ACDInteractableComponent
/Interface
  ACDInteractionInterface
/ObjectActor
  ACDLever / ACDDoor / ACDChest
/Manager
  ACDQuestManager / ACDItemManager / ACDDataRegistry / ACDInteractionManager / ACDUIManager
/UI/HUD
  ACDPlayerHUDWidget
/Puzzle (BP-only)
  BP_PressurePlate / BP_BigDoor

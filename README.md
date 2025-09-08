# 🎮 Co-op Puzzle & Network Demo
Unreal Engine 5 / Dedicated Server 기반 **멀티플레이 시스템 데모**  

---

Note: ARPG에서 반복되는 상호작용-인벤토리-보상-게이트 루프를  
**MVP(Proof-of-Concept) 방식으로 짧고 명확하게 구현한 멀티플레이 예시 데모**입니다.  
상용 수준의 모든 기능을 담기보다는, 핵심 구조를 작지만 완결된 사이클로 검증하는 데 집중했습니다.

## 📌 프로젝트 개요
- **목적**: 짧고 완결된 루프(MVP)를 통해 최신 UE5 네트워크 및 시스템 아키텍처를 **실험·검증**
- **개발 기간**: 2025.08.26 ~ 2025.09.09 (MVP 구현)
- **포커스**: Interaction, Replication, Inventory, Subsystem, UI 연동  
- **의도**: 실무 경험을 확장하고, 최신 엔진 기능을 직접 적용해본 예시 프로젝트

---

## ✅ 구현 범위
- **Interaction 시스템**  
  `UACDInteractionSensorComponent` (Sensor) / `IACDInteractionInterface` (Interface) / `UACDInteractableComponent` (Interactable)
- **상호작용 액터**  
  `AACDDoor` (문), `AACDLever` (레버), `AACDChest` (상자)
- **퍼즐 요소**  
  - **BP-only**: 발판 + 유리벽 + 큰 문 (RepNotify + Timeline 동기 실행)
  - **C++ 액터**: 레버(영구 개방), 작은 문(Key Door, 토글식)
- **UI**  
  HUD 생성 / 상호작용 프롬프트 / 인벤토리 위젯(Key 획득/소모 반영)
- **Subsystem**  
  ItemManager(보상 지급), UIManager(HUD 보장 생성, Prompt/Toast 전달), QuestManager(확장 뼈대)
- **PlayerState**  
  인벤토리 진행도 저장 및 복제 (OwnerOnly)

---

## 🕹️ 게임 플레이 루프 (데모 시나리오 — **골격 구현 기준**)
<img src="https://github.com/user-attachments/assets/8278e243-0fda-46b0-8bd8-b43a6aee2010" width="400" height="225"/>
<img src="https://github.com/user-attachments/assets/f0a8e4c6-c869-45f5-9b07-642d714238a5" width="400" height="225"/>  

- 발판 + 레버 → 큰 문 협동 퍼즐   
- 상자 보상 획득 → 인벤토리 UI 반영   
- Key Door 해금 → Key 소비 후 자유 출입  

▶️ [데모 플레이 영상 (YouTube, 45초)](https://youtu.be/zNMQqJn7SSA)

---

## 🛠️ 시스템 구조

### Interaction
- `UACDInteractionSensorComponent`: 서버에서 후보 탐색 및 주기 평가, `CurrentTargetActor` OwnerOnly 복제, `ForceUpdate` 지원
- `UACDInteractableComponent`: `Prompt/Action/RemainingUseCount` RepNotify, `OnInteracted`/`OnChangedInteractInfo` 델리게이트 제공
- `IACDInteractionInterface`: `CanInteract` / `DoInteract` 계약 보장
- **상호작용 액터** (`AACDDoor`, `AACDLever`, `AACDChest`): 상태는 C++에서 관리, 연출은 BP에서 구현

### 퍼즐 요소
- **발판 (BP-only)**: 서버 권한 Overlap Count → 유리벽 하강/상승 제어
- **큰 문 (BP-only)**: RepNotify로 열림/닫힘 시작 시점을 동기화  
  - 서버는 충돌(Blocking) 권한 유지  
  - 서버·클라 모두 동일 타임라인 재생 → 연출 일치
- **레버 (C++)**: `bIsOn` 서버 플래그 → 큰 문 개방/차단
- **작은 문 (C++)**: 최초 Key 필요, 소비 후 해금 → 이후 자유 출입 가능

### 캐릭터 & 컨트롤러
- `AACDCharacterBase`: 이동/점프/카메라 (Enhanced Input)
- `AACDCharacter`: Interact 입력 처리 → 서버 RPC 요청
- `AACDPlayerController`: Sensor 타깃 변경 수신 → Prompt 갱신 → UIManager 전달, UI 모드 토글

### 매니저 & 서브시스템
- `UACDItemManager`: DataTable 캐시 및 보상 지급
- `UACDUIManager`: HUD 보장 생성, Prompt/Toast/Inventory 전달 API
- `UACDQuestManager`: 뼈대만 구축 (확장 포인트)

### UI & HUD
- `UACDPlayerHUDWidget`: C++ API 시그니처 제공  
  - **Prompt/Inventory는 구현됨**  
  - **Toast/Quest는 API 수준 준비, 추후 확장 포인트**  
- `UACDInventoryWidget`: 인벤토리 반영 (아이콘/수량)

---

## 🌐 네트워크 고려 사항
- **입력 흐름**: 클라 → 서버 RPC → 서버 `CanInteract` 검증 → 실행
- **상태 동기화**: Interactable 상태는 서버에서만 변경, RepNotify/델리게이트로 반영
- **Inventory**: OwnerOnly FastArray 복제 → OnRep 후 UI Delegate로 갱신
- **Sensor 타깃**: OwnerOnly 복제 (본인만 프롬프트 표시)
- **퍼즐 연출**: 서버가 열림/닫힘 시점 확정 → 서버·클라 모두 같은 타임라인 실행
---

## 📂 코드 & BP 맵
```plaintext
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
/Inventory
ACDInventoryComponent / ACDInventoryTypes
/Manager
ACDItemManager / ACDUIManager / ACDQuestManager
/UI/HUD
ACDPlayerHUDWidget / ACDInventoryWidget
/Puzzle (BP)
BP_PressurePlate / BP_GlassWall / BP_BigDoor

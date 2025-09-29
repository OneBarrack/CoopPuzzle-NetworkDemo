# Co-op Puzzle & Network Demo
Unreal Engine 5 / Dedicated Server 기반 **멀티플레이 시스템 데모**  

---

Note: ARPG에서 반복되는 상호작용-인벤토리-보상-게이트 루프를  
**MVP(Proof-of-Concept) 방식으로 짧고 명확하게 구현한 멀티플레이 예시 데모**입니다.  
상용 수준의 모든 기능을 담기보다는, 핵심 시스템 구조(코드·BP)를 작지만 완결된 사이클로 검증하는 데 집중했습니다.

## 프로젝트 개요
- **목적**: 짧고 완결된 루프(MVP)를 통해 최신 UE5 네트워크 및 시스템 아키텍처를 **실험·검증**
- **개발 기간**: 2025.08.26 ~ 2025.09.09 (MVP 구현)
- **포커스**: Interaction, Replication, Inventory, FastArray, Subsystem, EnhancedInput, UI 연동  
- **의도**: 실무 경험을 확장하고, 최신 엔진 기능을 직접 적용해본 예시 프로젝트

---

## 구현 범위
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

## 주요 기술적 도전 과제 및 설계 전략

### (1) 상호작용 시스템 (네트워크 환경)
**기술적 고려 사항**  
멀티플레이 게임에서 플레이어 상호작용(예: 문 열기, 상자 열기)은 클라이언트와 서버 간의 데이터 동기화가 필수적입니다. 단순히 클라이언트에서만 상태를 변경하면 클라이언트 변조(Cheating) 위험이 야기되며, Dedicated Server 환경에서 데이터 일관성 확보가 어렵습니다.

**설계 전략 및 적용 기술**
- 클라이언트-서버 RPC (Authoritative Model): AACDCharacter 클래스에서 클라이언트의 상호작용 요청을 서버로 전달하는 Server_Interact RPC를 구현했습니다. 이로써 모든 상호작용의 최종 권한이 서버에 있음을 보장하고, 보안성을 확보했습니다.
- ReplicatedUsing (RepNotify 활용): AACDLever의 On/Off, AACDChest와 AACDDoor의 Open/Close 상태를 ReplicatedUsing을 통해 서버에서 클라이언트로 동기화하고, 클라이언트에서는 OnRep(RepNotify) 함수를 활용하여 시각적 연출을 즉각 실행하도록 설계했습니다. 이를 통해 서버의 유효한 상태를 정확히 반영하는 동시에 응답성을 확보했습니다.
<br>

### (2) 확장 가능한 시스템 설계
**기술적 고려 사항**  
게임 콘텐츠가 늘어날수록 상호작용 로직이 복잡해지고 관리하기 어려워질 수 있습니다. 새로운 상호작용 객체를 추가할 때마다 기존 코드를 수정해야 하는 높은 결합도(High Coupling)는 개발 속도를 저해합니다.

**설계 전략 및 적용 기술**
- 컴포넌트 + 인터페이스 설계 (낮은 결합도): UACDInteractionSensorComponent가 상호작용 대상을 감지하고, IACDInteractionInterface를 통해 모든 상호작용 가능한 객체(AACDChest, ACDDoor 등)가 통일된 방식으로 탐지/실행 과정을 통일했습니다. 또한, 상호작용 기능 동작을 위해 특정 Actor Class를 상속받는 것이 아닌 InteractableComponent를 보유하도록 하여 상속 기반 의존성을 제거하고 낮은 결합도 구조를 구축했습니다. 이로 인해 새로운 상호작용 객체 추가 및 기능 확장이 유연합니다.
- 서브시스템 기반 매니저: UGameInstanceSubsystem과 ULocalPlayerSubsystem을 활용하여 아이템 및 UI 매니저를 구현했습니다. 이로써 게임의 핵심 매니저들이 모듈화된 형태로 엔진의 수명주기에 맞춰 관리되며, 매니저 간의 의존성은 이벤트(송신) → UI(수신) 단방향으로 제한하여 시스템 간의 결합을 낮췄습니다.
<br>

### (3) 데이터 관리 및 협업
**기술적 고려 사항**  
아이템 정보나 보상 테이블 같은 게임 데이터를 코드에 직접 넣으면 수정이 어렵고, 특히 기획자/디자이너와의 협업 과정에서 데이터와 코드의 의존성 때문에 작업 효율이 떨어집니다.

**설계 전략 및 적용 기술**
- UDeveloperSettings 활용: ACDGameDataSettings 클래스를 만들어 에디터 내에서 Item 및 Reward 데이터 테이블을 설정할 수 있게 했습니다. 이를 통해 개발자는 코드와 데이터를 분리하여 관리하고, 디자이너는 직접 데이터를 수정하며 작업 효율을 높일 수 있습니다.
- 데이터 구조 분리 및 직렬화: ACDStruct.h에서 게임 데이터의 구조체를 독립적으로 정의하고, Data Asset 또는 DataTable에서 사용할 수 있도록 직렬화(Serialization)를 지원했습니다. 이로써 코드와 데이터가 명확하게 분리될 수 있도록 했습니다.
<br>

### (4) 동적 데이터의 효율적 복제 (FastArray)
**기술적 고려 사항**  
플레이어의 인벤토리와 같이 아이템이 빈번하게 추가/제거되는 동적 배열(TArray)을 기본 네트워크 방식으로 복제할 경우, 배열 내용이 조금만 바뀌어도 배열 전체가 재복제되어 네트워크 대역폭(Bandwidth)을 불필요하게 소모합니다. 이는 배열의 크기가 클수록 비효율적입니다.

**설계 전략 및 적용 기술**
- FastArraySerializer 구현: 인벤토리 배열(TArray<FACDInventoryItem>)을 FFastArraySerializer를 상속받는 구조체 내에 래핑했습니다.
- Diff-Only Replication: 서버는 배열의 전체 내용이 아닌 변경사항(추가/삭제/수정)이 있는 항목만 전파합니다. 이는 네트워크 트래픽을 최소화하고 인벤토리 데이터의 실시간 동기화 효율을 극대화합니다.
- Owner-Only 복제: 해당 인벤토리 데이터는 ACDPlayerState에서 DOREPLIFETIME_CONDITION을 사용하여 Owner-Only로 설정하여 보안 및 트래픽 효율을 확보했습니다.

---

## 게임 플레이 루프 (데모 시나리오 — **골격 구현 기준**)
<img src="https://github.com/user-attachments/assets/8278e243-0fda-46b0-8bd8-b43a6aee2010" width="400" height="225"/>
<img src="https://github.com/user-attachments/assets/f0a8e4c6-c869-45f5-9b07-642d714238a5" width="400" height="225"/>  

- 발판 + 레버 → 큰 문 협동 퍼즐   
- 상자 보상 획득 → 인벤토리 UI 반영   
- Key Door 해금 → Key 소비 후 자유 출입  

▶️ [데모 플레이 영상 (YouTube, 45초)](https://youtu.be/zNMQqJn7SSA)

---

## 시스템 구조

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
  - Prompt/Inventory는 구현됨  
  - Toast/Quest는 API 수준 준비, 추후 확장  
- `UACDInventoryWidget`: 인벤토리 반영 (아이콘/수량)

### 테이블 데이터
- `FACDItemRow`: DataTable 기반 아이템 정의 Row (DT_ItemTable로 CSV/JSON 원본 관리)
- `FACDRewardRow`: DataTable 기반 보상 정의 Row (DT_RewardTable로 CSV/JSON 원본 관리)  
  - DataTable은 **에디터에서 로드 후 캐싱**하여 `ItemManager`에서 지급/조회 처리

---

## 네트워크 고려 사항
- **입력 흐름**: 클라 → 서버 RPC → 서버 `CanInteract` 검증 → 실행
- **상태 동기화**: Interactable 상태는 서버에서만 변경, RepNotify/델리게이트로 반영
- **Inventory**: OwnerOnly FastArray 복제 → OnRep 후 UI Delegate로 갱신
- **Sensor 타깃**: OwnerOnly 복제 (본인만 프롬프트 표시)
- **퍼즐 연출**: 서버가 열림/닫힘 시점 확정 → 서버·클라 모두 같은 타임라인 실행
---

## 코드 & BP 맵
```plaintext
[Source]
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
/UI
  ACDPlayerHUDWidget / ACDInventoryWidget

[BP]
/Core
  BP_GameMode_ACD / BP_PlayerState / BP_PlayerController
  /Character
    BP_CharacterBase / BP_Character
/ObjectActor
  BP_GlassBarricade
  /Chest
    BP_InteractableChestBase / BP_Chest
  /Door
    BP_InteractableDoorBase / BP_Door / BP_BigDoor
  /Lever
    BP_InteractableLeverBase / BP_Lever
  /PressurePlate
    BP_PressurePlate
/UI
  WBP_PlayerHUD
  /Inventory
    WBP_Inventory / WBP_ItemSlot
  /Interaction
    WBP_InteractionText

[Data]
/Data
  DT_ItemTable / DT_RewardTable

[Input]
/Input
  IMC_GamePlay / IMC_UI
  /Actions
    IA_Move / IA_Jump / IA_Look / IA_Interact / IA_Inventory

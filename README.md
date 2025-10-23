## 01. 목차

---
## 02. 개요
- **프로젝트소개**
    - 리슨 서버 기반 멀티플레이 RPG 게임
- **개발인원**
  - 1명
- **사용엔진**
  - 언리얼 엔진 5.6.1
- **작업 기간**
  - 2025년 8월 21일 ~
---
## 03. 프로젝트 요약
- **장르**
  - 3D Action RPG Game


- **게임특징**
  - 리슨 서버를 통한 멀티플레이를 구현하였으며 서버와 클라이언트의 역할을 분리하여 최대한 데디케이트 서버 환경과 유사한 구조로 설계하였습니다.
  - Gameplay Ability System 프레임워크를 이용하여 일반적인 온라인 RPC 게임처럼 몬스터를 잡고 레벨을 올리며 능력치를 강화하고 아이템을 장착하는 등의 기능을 구현하였습니다.
  

- **주요기능 요약**
    - **캐릭터 성장 시스템**  
    `Gameplay Ability System`을 이용하여 **Attribute**를 구현하였으며 몬스터를 처치시 경험치를 획득하여 Attribute를 강화하던가 아이템을 장착하여 캐릭터가 더 강해지도록 제작하였습니다.
    - **멀티플레이**  
  리슨서버를 사용하여 멀티플레이를 구현하였으며 **Replication**을 통하여 클라이언트에서도 정상적으로 Actor가 복제되게 설정하였고 RPC를 통하여 서버와 클라이언트간에 데이터를 주고 받아 둘의 역할을 분리시켰습니다. 
    - **저장 및 불러오기**  
  캐릭터의 Attribute나 Map 위치, 인벤토리등을 호스트(서버)의 디스크에 기록하며 `클라이언트에서도 데이터를 저장`하고 가져올 수 있게 매우 간단한 로그인 시스템을 구현하였습니다.
    - **Object Pooling System**  
    `오브젝트 풀`을 사용하여 Projectile이나 Emeny와 같이 자주 소환되는 Actor들은 Object Pool에 저장하였다가 필요시 꺼내 사용하여 성능을 최적화 하였습니다.
  - **모듈화**  
    각각의 기능들을 독립적으로 모듈화하여 Core 모듈을 제외하고는 각각의 모듈들은 서로 의존성이 없어 독립적으로 개발이 가능합니다.
  - **다이얼로그 시스템**  
  노드 기반의 다이얼로그 시스템을 구현하여 다양한 분기의 대화를 쉽게 처리할 수 있도록 하였습니다.
---
## 04. 핵심 기능 및 구현 내용
### 04.1 Gameplay Ability System
GAS를 사용하여 Attribute와 Ability 기능을 구현하였으며 Attribute의 경우 Primary Attribute를 기반으로 Secondary Attribute를 설정하여 과거 주류 RPC 게임처럼 스탯을 올려 캐릭터를 강화시킬 수 있도록 구현하였습니다.

- **Attribute 및 Ability 적용**  
Attribute와 Ability는 서버에서만 적용이 됩니다. Attribute는 Gameplay Effect를 통해 적용되며 최초의 기본 Attribute는 데이터 테이블에서 설정하게 하여 디자이너가 간편하게 초기 Attribute를 설정할 수 있게 하였습니다. 어빌리티는 Data Asset에서 설정되는데 이중 Active어빌리티는 Input Tag와 연결하였으며 또 이 Input Tag는 Input Action과 연결되어 키보드 바인딩이 변경될 일이 있어도 어빌리티 로직 수정 없이 Input Action만 바꾸면 되게 구현하였습니다.

> Github Link 
> 
> - [어빌리티 정보가 저장된 Data Asset Class](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CoreModule/Public/DataAssets/DataAsset_DefaultPlayerAbilities.h)

- **능력치 강화**

![능력치 강화 화면](GameImg/AttributeInfoWindow.png)
캐릭터 인포메이션에 있는 각각의 위젯에는 Gameplay Tag와 FGameplayAttribute 구조체가 연결되어 있어 Gameplay Tag만 지정하면 Attribute 값을 가져올 수 있게 설정하였습니다. 스탯 포인트가 0 이상일 경우 강화 버튼이 활성화 되며 +버튼이나 -버튼을 누르면 강화를 원하는 스탯의 Gameplay Tag와 float 값을 저장하는 TMap에 이러한 정보를 저장하여 서버에 요청을 보내 Attribute를 강화할 수 있도록 구현하였습니다.
```c++
// Widget Controller Class
void UCharacterInfoWidgetController::UpgradeAttribute(const TMap<FGameplayTag, float>& AttributeUpgradeMap)
{
    // 위젯에서 위젯 컨트롤러에 강화할 Attribute의 정보가 담긴 TMap을 보냄. 
    
	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn);
	if (!PlayerInterface) return;

	TArray<FAttributeUpgradePrams> UpgradePrams;
	for (const TPair<FGameplayTag, float>& UpgradeData : AttributeUpgradeMap)
	{
		if (!UpgradeData.Key.IsValid()) continue;
		FAttributeUpgradePrams UpgradePram(UpgradeData.Key, UpgradeData.Value); // TMap을 구조체로 변환

		UpgradePrams.Add(UpgradePram);
		
	}

	PlayerInterface->UpgradeAttribute(UpgradePrams); // 캐릭터 클래스에 데이터 전송
	
}
	
// Character Class
void ABrandNewPlayerCharacter::UpgradeAttribute(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams)
{
	// ... null check 생략
	
	Server_RequestUpgradeAttribute(AttributeUpgradePrams); // 서버 RPC로 Attribute 강화 요청.
	
}

void ABrandNewPlayerCharacter::Server_RequestUpgradeAttribute_Implementation(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams)
{
	// ... Context 및 Spect Handle 생성 부분 생략

	int32 ConsumedStatPoint = 0;
	
	for (const FAttributeUpgradePrams& UpgradePrams : AttributeUpgradePrams)
	{
		if (!UpgradePrams.TagToUpgrade.IsValid()) continue;

		ConsumedStatPoint += UpgradePrams.UpgradeAmount;
		// Set by Caller를 사용해 강화 수치 설정
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,UpgradePrams.TagToUpgrade, UpgradePrams.UpgradeAmount);
	}
	
	// 유효한 강화 시도인지 확인
	if (GetAttributeValueByTag(BrandNewGamePlayTag::Attribute_Experience_AttributePoint) < ConsumedStatPoint) return;
	
	//  ... 실제 Effect 적용 부분 생략
	
}
```
위젯 컨트롤러에서 플레이어 캐릭터로 강화 정보를 보내는 로직입니다. 언리얼 RPC에서는 TMap을 바로 보낼 수 없기 때문에 구조체로 변환하였으며 또한 위젯 모듈과 캐릭터 모듈은 다르기 때문에 인터페이스를 사용하여 데이터를 전송하였습니다.  
서버에서는 강화를 하기 전 Stat Point를 확인하여 부정한 Attribute 강화를 차단하였으며 Gameplay Effect를 통해 Attribute를 강화합니다.

### 04.2 Inventory System
인벤토리 클래스는 Player State에 저장되어 있습니다. 
- **Iten Info Data Table**  

![아이템 정보를 저장하는 데이터 시트](GameImg/IntemInfoSheet.png)

아이템 정보를 데이터 테이블에 저장하여 디자이너가 쉽게 아이템 정보를 설정할 수 있게 하였고 아이템은 Id만 설정하여 이 Id를 통해 데이터 테이블에서 정보를 가져오도록 구현하였습니다.

- **Item 획득**

![아이템 오버랩 이미지](GameImg/ItemOverlap.png)아이템은 몬스터 처치시 일정확률로 스폰되며 이 아이템에 오버랩되면 Id를 통해 아이템 정보를 가져오고 Interaction 키(F)를 눌러 아이템을 획득 할 수 있습니다.
```c++
void ABrandNewPlayerCharacter::AddOverlappedItem(AActor* OverlappedItem)
{
	if (HasAuthority())
	{
		OverlappedItems.Add(OverlappedItem); // 서버에서 실제 오버랩된 아이템된 아이템
	}
	else
	{
		OverlappedItemsForUI.Add(OverlappedItem); // 클라이언트 UI 표시용 오버랩된 아이템
	}
	SendPickupInfoToUi(OverlappedItem, true);
	
}
```
아이템과 오버랩시 호출되는 함수입니다. 서버와 클라이언트간에 아이템을 저장하는 Array가 다른데 아이템 습득시 클라이언트로부터 어떠한 정보를 받는 것이 아니라 오직 서버에만 존재하는 오버랩된 아이템 정보를 가지고 아이템 획득 작업을 진행하여 부정한 아이템 획득을 방지하였습니다.  

다만 클라이언트도 화면에 보여주고 제거할때 오버랩 된 아이템들이 어떤 것인지 알아야하기 때문에 클라이언트 전용 배열에는 저장하였으며 이때 RPC를 사용하지 않은 이유는 단순히 화면에만 보여주는 것이 목적이기 때문에 굳이 RPC를 사용하여 네트워크 트래픽을 증가시킬 필요가 없다고 판단하였기 때문입니다. 

- **Inventory**  
인벤토리 클래스에서는 구조체로 아이템 정보를 저장하고 있습니다.
```c++
////// 구조체 정보//////

/* 인벤토리 슬롯 하나에 담겨 있는 아이템 정보 */
USTRUCT(BlueprintType)
struct FInventorySlotData
{
	GENERATED_BODY()

	/* 아이디를 저장하여 TMap에서 아이템 정보를 가져옴 */
	UPROPERTY(BlueprintReadOnly)
	int32 ItemID = INDEX_NONE;
	
	// 한 슬롯에 저장된 아이템의 수
	UPROPERTY(BlueprintReadOnly)
	int32 Quantity = 0;
	
	// 아이템 장착 여부
	UPROPERTY(BlueprintReadOnly)
	bool bIsEquipped = false;
	
};

/* 전체 인벤토리 */
USTRUCT(BlueprintType)
struct FInventoryContents
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FInventorySlotData> WeaponSlots = TArray<FInventorySlotData>(); // 무기
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FInventorySlotData> ArmorSlots = TArray<FInventorySlotData>(); // 방어구
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FInventorySlotData> EatablesSlots = TArray<FInventorySlotData>(); // 소비
	
};

// Inventory Class에 저장되어있는 인벤토리 변수
UPROPERTY(Replicated)
FInventoryContents ItemInventory;
```
아이템 정보를 담고 있는 FInventorySlotData 구조체가 Integer 변수와 boolean 변수밖에 없기 때문에 전체 아이템 정보를 담고 있는 ItemInventory 변수를 Save Data로 저장이 가능해 인벤토리 세이브를 매우 간편하게 할 수 있습니다.

- **아이템 장착 및 사용**  
아이템 효과는 Gamepaly Effect를 이용하여 적용하였으며 소비 아이템의 경우 Instance 효과를 주었고 장비 아이템의 경우 Infinite 효과를 주어 아이템을 교체하거나 해제시 Effect를 제외할 수 있도록 하였습니다. 장착된 아이템 정보는 슬롯에 저장되어 세이브/로드시 사용됩니다.
> Github Link
> 
> - [아이템 소비 함수](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CharacterModule/Private/Character/BrandNewPlayerCharacter.cpp#L915)  
> - [아이템 장착 함수](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CharacterModule/Private/Character/BrandNewPlayerCharacter.cpp#L951)

### 04.3 Map Travel
- **Level Manager Subsystem**  
맵(Level) 이동을 관리하는 서브시스템입니다.
```c++
// Level Manager Subsystem
// 맵 이동 로직은 아래 함수들을 순서대로 실행한다고 보면 됩니다.

// 맵 경로를 설정하는 함수 (본문 생략)
void UBrandNewLevelManagerSubsystem::SetMapNameToTravelByString();

// Transition Map으로 이동하는 함수 (본문 생략)
void UBrandNewLevelManagerSubsystem::TravelToTransitionMap();

// 맵 비동기 로드 시작
void UBrandNewLevelManagerSubsystem::StartAsyncLoading()
{
	// ... 생략
	
    LoadPackageAsync(
        TargetLevelPath.ToString(),
        FLoadPackageAsyncDelegate::CreateUObject(this, &UBrandNewLevelManagerSubsystem::OnLoadPackageCompleted),
        0,
        PKG_ContainsMap);

}

/**
* 맵 로딩 성공 여부 확인. 성공 여부를 각각의 클라이언트들이 서버에 전송하고
* 서버(호스트)는 모든 클라이언트가 맵 로드에 성공하면 맵을 이동할 수 있게 됩니다.
*/
void UBrandNewLevelManagerSubsystem::OnLoadPackageCompleted(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
	GetWorld()->GetTimerManager().ClearTimer(LoadingPercentTimerHandle);
	if (Result == EAsyncLoadingResult::Succeeded)
	{
		OnAsyncLoadingCompleteDelegate.Broadcast(true);
	}
	else
	{
		OnAsyncLoadingCompleteDelegate.Broadcast(false);
	}
}
```
> 전체 코드 Github Link  
> - [Level Manager Subsystem.h](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CoreModule/Public/Game/Subsystem/BrandNewLevelManagerSubsystem.h)  
> - [Level Manager Subsystem.cpp](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CoreModule/Private/Game/Subsystem/BrandNewLevelManagerSubsystem.cpp)
>
맵 이동 로직은 다음과 같습니다. 먼저 맵 이동전 이동할 맵 경로를 `TargetLevelPath` 변수에 설정합니다. 해당 변수를 설정한 뒤 트랜지션 맵으로 이동하고 이곳에서 비동기적으로 월드 에셋을 로드합니다. 바로 맵을 Open 하는 것이 아닌 비동기 로드를 하기 때문에 게임이 멈추는 시간을 최소화하며 사용자에게 로딩화면을 보여주어 더 나은 게임 환경을 느낄 수 있게 하였습니다.

리슨 서버를 사용하기 때문에 모든 플레이어가 함께 이동하는 것이 중요하고 그렇기 때문에 로딩이 완료될 때 클라이언트들은 서버에 이를 알립니다. 서버에서는 모든 플레이어의 로딩이 끝나면 맵을 이동시킵니다.
```c++
// 메인 메뉴 및 로딩 화면 등에서 사용하는 Player Controller Class

// 클라이언트 또는 호스트에서 로딩이 완료되면 실행하는 함수로 RPC를 통해 서버로 자신이 로딩이 완료되었다는 것을 알립니다.
void AUIPlayerController::BP_NotifyMapLoaded()
{
	const int32 PlayerId = GetPlayerState<APlayerState>()->GetPlayerId();
	
	Server_NotifyMapLoaded(PlayerId);

}

void AUIPlayerController::Server_NotifyMapLoaded_Implementation(const int32 PlayerId)
{
	if (ABrandNewGameState* BrandNewGameState = Cast<ABrandNewGameState>(GetWorld()->GetGameState()))
	{
		BrandNewGameState->RegisterPlayerLoaded(PlayerId);
	}
	
}
```
```c++
// Game State 클래스에서 모든 유저가 로드 되었는지 확인하고 전부 완료되면 맵을 이동합니다.
void ABrandNewGameState::CheckAllPlayersLoaded()
{
	if (!HasAuthority()) return;
	
	if (LoadedPlayerIdSet.Num() >= PlayerArray.Num()) // 로딩 완료된 플레이어 수와 전체 플레이어 수 비교
	{
		LoadedPlayerIdSet.Empty();
		
		// 맵 이동
		const UBrandNewLevelManagerSubsystem* LevelManagerSubsystem = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>();
		check(LevelManagerSubsystem);
		LevelManagerSubsystem->TravelMap();
		
	}
}
```

- **Map Entrance Actor**  
  Map Entrance Actor는 모든 플레이어가 오버랩 되면 레벨 매니저 서브시스템에 이동할 맵 경로를 설정하여 맵 이동 작업을 준비합니다.
```c++
void AMapEntrance::CheckAllPlayersOverlapped()
{
	if (!HasAuthority()) return;

	const ABrandNewGameState* BrandNewGameState = Cast<ABrandNewGameState>(GetWorld()->GetGameState());
	if (!BrandNewGameState) return;
	
	// 오버랩 된 액터와 플레이어의 수가 같으면 맵 이동
	if (OverlappingActors.Num() == BrandNewGameState->PlayerArray.Num())
	{
		UBrandNewLevelManagerSubsystem* LevelManagerSubsystem = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>();
		if (!LevelManagerSubsystem) return;
		
		// 맵 이동 전 플레이어들의 데이터를 저장
		for (TWeakObjectPtr<AActor>& WeakActor : OverlappingActors)
		{
			if (AActor* Actor = WeakActor.Get())
			{
				if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(Actor))
				{
					PlayerInterface->SavePlayerDataForTravel();
				}
				
			}
		}
		
		// 레벨 매니저에 맵 경로를 설정하고 트랜지션 맵으로 이동하며 이후 로직은 레벨 매니저 클래스에서 처리. 
		LevelManagerSubsystem->SetMapNameToTravel(LevelToTravelClass);
		LevelManagerSubsystem->TravelToTransitionMap(TransitionLevelClass);
	}
	
}
```
Map Entrace Class는 아래와 같이 오버랩 된 플레이어와 전체 플레이어 수를 위젯에 보내 화면에 보여주는 역할도 합니다.
```c++
//Widget Update 함수 간단 예시

const int32 OverlappedPlayerCount = OverlappingActors.Num();
const int32 MaxPlayersCount = BrandNewGameState->PlayerArray.Num();
PlayerControllerInterface->HandlePlayerMapEntryOverlap(OverlappedPlayerCount, MaxPlayersCount);
```
- 현재 오버랩 된 플레이어 수를 보여주는 위젯 이미지
![Map Entrance 이미지](GameImg/MapEntrace.png)

> Github Link
> - [Map Entrance.h](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CharacterModule/Public/Actor/Map/MapEntrance.h)
> - [Map Entrance.cpp](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CharacterModule/Private/Actor/Map/MapEntrance.cpp)

### 04.4 Object Pooling
오브젝트 풀링 시스템을 구현하여 자주 사용하는 액터는 Pool에서 관리하도록 하였습니다.
- **Objet Pool Manger**  
오브젝트 풀 매니저 클래스는 액터들을 미리 생성하고 필요시 꺼내어 사용할 수 있도록 돕는 클래스입니다. 간단한 로직 설명은 다음과 같습니다. 게임이 시작되면(Begin Play) 액터들을 미리 원하는 만큼 생성하고 Hidden으로 설정하고 배열에 저장해놓았다가 필요시 풀 매니저 클래스에 접근하여 배열에서 액터들을 불러오는 방식을 사용하였습니다.<br>    
풀 매니저는 게임모드에서 관리하는데 이는 현재 스폰 기능이 서버에서만 작동하게 되어 있으며 또한 월드를 이동할때 풀을 자동으로 초기화 하고 싶어 게임모드에서 이를 관리하도록 하였습니다.<br>  
현재 오브젝트 풀 매니저에서 관리하고 있는 클래스는 Projectile 클래스와 Enemy Class입니다.

> GitHub Link
> - [Object Pool Manager.h](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CoreModule/Public/Manager/Pooling/BrandNewObjectPoolManager.h)
> - [Object Pool Manager.cpp](https://github.com/jinjinchoi/BrandnewGame/blob/main/Source/CoreModule/Private/Manager/Pooling/BrandNewObjectPoolManager.cpp)

### 04.5 Save System


// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIManager_InGameCombat.h"
//#include "BladeII.h"

#include "B2UIManager_InGameHUDChar.h"
#include "B2UIManager_InGameHUDPlayer.h"
#include "B2UIManager.h"
#include "B2UIBattleMain.h"
#include "B2UIMsgPopupCenter.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "BladeIINetPlayer.h"
#include "Event.h"
#include "B2FloatingDamageNum.h"
#include "B2FloatingAbnormalEffect.h"
#include "B2FloatingGoldAcq.h"
#include "B2LobbyGameMode.h"
#include "B2UIMsgPopupBattleMessageNotify.h"

//////////////////////////////////////////////////
//
// 이름만 InGameCombat 이고 Lobby 에서도 사용하는 구석이 좀 있음.
// 아마도 계속 사용될 듯한 DJLegacy UI 일부
//
//////////////////////////////////////////////////

AB2UIManager_InGameCombat::AB2UIManager_InGameCombat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = false;
	NetUpdateFrequency = 10.0f;
	//bHidden = true;
	//bReplicateMovement = false;
	//bCanBeDamaged = false;

	TempCurrentOpenCenterPopup = NULL;
	PlayerHUDManager = NULL;

	
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GMinimalDLCFrontMode) // DLC Front 모드 리소스로딩 최대한 제거. 근데 설마 이건 DLCFront 모드에서 생성되지 않겠지? ㅋ
	{ 
		// 여기서 로딩하는 WidgetClass 설정이 처음 구현시에는 BladeIICharacter 나 BladeIIPlayer 에 있었어서 ini 섹션 이름이 그걸 반영하고 있음. 
		{
			FString DefaultDamageNumWidgetClassPath;
			GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultDamageNumWidgetClass"), DefaultDamageNumWidgetClassPath, GGameIni);
			static ConstructorHelpers::FClassFinder<UB2FloatingDamageNum> DefaultDamageNumWidgetClass(*DefaultDamageNumWidgetClassPath);
			if (DefaultDamageNumWidgetClass.Class != NULL)
			{
				DamageNumWidgetClass_Mob = DefaultDamageNumWidgetClass.Class;
			}
		}
		{
			FString DefaultDamageNumWidgetClassPath;
			GConfig->GetString(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("DefaultDamageNumWidgetClass"), DefaultDamageNumWidgetClassPath, GGameIni);
			static ConstructorHelpers::FClassFinder<UB2FloatingDamageNum> DefaultDamageNumWidgetClass(*DefaultDamageNumWidgetClassPath);
			if (DefaultDamageNumWidgetClass.Class != NULL)
			{
				DamageNumWidgetClass_PC = DefaultDamageNumWidgetClass.Class;
			}
		}
		{
			FString DefaultAbnormalEffectWidgetClassPath;
			GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultAbnormalEffectWidgetClass"), DefaultAbnormalEffectWidgetClassPath, GGameIni);
			static ConstructorHelpers::FClassFinder<UB2FloatingAbnormalEffect> DefaultAbnormalEffectWidgetClass(*DefaultAbnormalEffectWidgetClassPath);
			if (DefaultAbnormalEffectWidgetClass.Class != NULL)
			{
				AbnormalEffectWidgetClass = DefaultAbnormalEffectWidgetClass.Class;
			}
		}
		{
			FString DefaultGoldAcqWidgetClassPath;
			GConfig->GetString(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("DefaultGoldAcqWidgetClass"), DefaultGoldAcqWidgetClassPath, GGameIni);
			static ConstructorHelpers::FClassFinder<UB2FloatingGoldAcq> DefaultGoldAcqWidgetClass(*DefaultGoldAcqWidgetClassPath);
			if (DefaultGoldAcqWidgetClass.Class != NULL)
			{
				GoldAcqDisplayWidgetClass = DefaultGoldAcqWidgetClass.Class;
			}
		}
		checkSlow(DamageNumWidgetClass_Mob && DamageNumWidgetClass_PC && AbnormalEffectWidgetClass && GoldAcqDisplayWidgetClass);
	}
}

void AB2UIManager_InGameCombat::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Some manual update of character widgets

	if (PlayerHUDManager)
	{
		PlayerHUDManager->UpdateManual(DeltaSeconds); // CustomTimeDilation 을 고려해야 하면 GetDeltaTimeAsOtherWorldObj 를.. 여기에 CustomTimeDilation 을 적용할 일이 정말 없을 거 같긴 하지만 
	}

	for (UB2UIManager_InGameHUDChar* CharHUDWidget : AllNPCHUDManager)
	{
		CharHUDWidget->UpdateManual(DeltaSeconds);
	}
}

void AB2UIManager_InGameCombat::PreBeginPlay()
{
	SubscribeEvents();
	InitializeFloatingWidgetPool();
}

void AB2UIManager_InGameCombat::OnPreLoadMap()
{
	// UObject 와 관련된 destroy 는 여기서
	DestroyFloatingWidgetPool();
}

void AB2UIManager_InGameCombat::BeginDestroy()
{
	UnsubscribeEvents();
	Super::BeginDestroy();
}

void AB2UIManager_InGameCombat::Destroyed()
{
	UnsubscribeEvents();
	Super::Destroyed();
}

UB2UIManager_InGameHUDChar* AB2UIManager_InGameCombat::CreateHUDForNewChar(class ABladeIICharacter* InNewCreated)
{
	if (InNewCreated == NULL)
	{
		return NULL;
	}

	UB2UIManager_InGameHUDChar* NewUIManager = NULL;

	//ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(InNewCreated);
	//if (CastedPlayer != UGameplayStatics::GetLocalPlayerCharacter(this)) // 리모트 플레이어는 플레이어 취급 아님.
	//{
	//	CastedPlayer = NULL;
	//}

	//if (CastedPlayer)
	//{
	//	// 이 경우 Player 용 HUD 로
	//	NewUIManager = NewObject<UB2UIManager_InGameHUDPlayer>(this, *FString::Printf(TEXT("UIManager_InGameHUD_%s"), *CastedPlayer->GetName()), RF_Transient);
	//}
	//else
	//{
	//	NewUIManager = NewObject<UB2UIManager_InGameHUDChar>(this, *FString::Printf(TEXT("UIManager_InGameHUD_%s"), *InNewCreated->GetName()), RF_Transient);
	//}

	//if (NewUIManager && NewUIManager->InitUIManager(InNewCreated, this) == false) // PlayerController 구성에 따라 실패할 수 있음.
	//{
	//	NewUIManager->ConditionalBeginDestroy();
	//	NewUIManager = NULL;
	//}

	//if (NewUIManager)
	//{
	//	// 새로 생성한 것을 이쪽에 등록
	//	if (CastedPlayer)
	//	{
	//		if (PlayerHUDManager) // 만에 하나 기존에 있었다면 파괴
	//		{
	//			PlayerHUDManager->DestroyPlayerWidgets();
	//			PlayerHUDManager->ConditionalBeginDestroy();
	//		}

	//		PlayerHUDManager = Cast<UB2UIManager_InGameHUDPlayer>(NewUIManager);
	//	}
	//	else
	//	{
	//		AllNPCHUDManager.Add(NewUIManager);
	//	}		
	//}

	return NewUIManager;
}

void AB2UIManager_InGameCombat::DestroyHUDForDyingChar(class ABladeIICharacter* InDying)
{
//	ABladeIIPlayer* LocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	// Not for local player character. ABladeIIPlayer 라도 이걸 사용하는 경우가 있긴 함.
//	if (InDying == NULL || (LocalPlayer && LocalPlayer == InDying)) 
//	{
//		return;
//	}
//	
//	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	UB2UIManager_InGameHUDChar* WantedUIManager = GetRelevantUIManager(InDying);
//	if (WantedUIManager)
//	{
//		WantedUIManager->DestroyUserWidgets();
//		WantedUIManager->ConditionalBeginDestroy();
//		AllNPCHUDManager.Remove(WantedUIManager);
//	}
}

void AB2UIManager_InGameCombat::DestroyHUDForDyingPlayer(class ABladeIIPlayer* InDying)
{
	if (PlayerHUDManager && InDying == Cast<ABladeIIPlayer>(PlayerHUDManager->GetOwnerChar()))
	{
		PlayerHUDManager->DestroyPlayerWidgets();
		PlayerHUDManager->ConditionalBeginDestroy();
		PlayerHUDManager = NULL;
	}
}

void AB2UIManager_InGameCombat::HideFloatingHPBarForChar(ABladeIICharacter* InChar, bool binHide)
{
	if (InChar == NULL || Cast<ABladeIIPlayer>(InChar)){ // Not for player. InChar->IsA(ABladeIIPlayer::StaticClass())
		return;
	}

	UB2UIManager_InGameHUDChar* WantedUIManager = GetRelevantUIManager(InChar);
	if (WantedUIManager)
		WantedUIManager->HideFloatingHPBar(binHide);
}

// HideFloatingHPBarForChar 함수랑 따로처리(플레이어일때 리턴한 이유가 있을테니..)
void AB2UIManager_InGameCombat::HideFloatingHPBarForPlayer(class ABladeIIPlayer* InChar, bool binHide)
{
	UB2UIManager_InGameHUDChar* WantedUIManager = GetRelevantUIManager(InChar);
	if (WantedUIManager)
		WantedUIManager->HideFloatingHPBar(binHide);
}

void AB2UIManager_InGameCombat::HideMobPointer(bool bInHide)
{
	if (PlayerHUDManager)
	{
		PlayerHUDManager->HideMobPointer(bInHide);
	}
}

void AB2UIManager_InGameCombat::HideVirtualJoystick(bool bInHide)
{
	if (PlayerHUDManager)
	{
		PlayerHUDManager->HideVirtualJoystick(bInHide);
	}
}

bool AB2UIManager_InGameCombat::CanResurrectionGameModeType()
{
	EB2GameMode GameModeType = GetB2GameModeType(this);
	return GameModeType == EB2GameMode::Raid;
}

////////////////////////////////////// FloatingWidgetPool
void AB2UIManager_InGameCombat::InitializeFloatingWidgetPool()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2UIManager_InGameCombat::InitializeFloatingWidgetPool"));

	//int32 PooledObjectPreCreateCount = 5;
	//if (GConfig)
	//{ // 아마 DamageNum_Mob 이 제일 많을 것 같긴 한데 굳이 타입별로 PreCreate 개수를 다르게 가져갈 필요까진 없을 것 같다.
	//	GConfig->GetInt(TEXT("/Script/BladeII"), TEXT("FloatingWidgetPoolPreCreateNum"), PooledObjectPreCreateCount, GGameIni);
	//}

	//// 로비에서 이걸 생성해 놓을 하등의 이유가 없다.
	//AB2LobbyGameMode* B2LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2LobbyGM){
	//	PooledObjectPreCreateCount = 0;
	//}

	//APlayerController* LocalPC = UGameplayStatics::GetLocalPlayerController(this);
	//for (int32 WPI = 0; WPI < (int32)EFloatingWidgetPoolClassType::EFWPCT_End; ++WPI)
	//{
	//	FFloatingWidgetPool NewWidgetPool;
	//	NewWidgetPool.PreInitSetType(static_cast<EFloatingWidgetPoolClassType>(WPI));
	//	NewWidgetPool.InitPool(this, LocalPC, PooledObjectPreCreateCount, BII_WIDGET_ZORDER_FLOATING_DEFAULT);
	//	FloatingWidgetPools.Add(NewWidgetPool);
	//}
	//checkSlow(FloatingWidgetPools.Num() == (int32)EFloatingWidgetPoolClassType::EFWPCT_End);
}
void AB2UIManager_InGameCombat::DestroyFloatingWidgetPool()
{
	for (FFloatingWidgetPool& ThisWidgetPool : FloatingWidgetPools)
	{
		ThisWidgetPool.DestroyAll();
	}
	FloatingWidgetPools.Empty();
}

FFloatingWidgetPool* AB2UIManager_InGameCombat::GetMatchingFloatingWidgetPool(EFloatingWidgetPoolClassType InFloatingWidgetType)
{
	int32 FloatingWidgetTypeInt = static_cast<int32>(InFloatingWidgetType);
	return FloatingWidgetPools.IsValidIndex(FloatingWidgetTypeInt) ? &FloatingWidgetPools[FloatingWidgetTypeInt] : nullptr;
}

UB2FloatingWidget* AB2UIManager_InGameCombat::RentFloatingWidget(EFloatingWidgetPoolClassType InRentWidgetType, APlayerController* InPC, class ABladeIICharacter* InOwnerChar)
{
	FFloatingWidgetPool* MatchingPool = GetMatchingFloatingWidgetPool(InRentWidgetType);
	return MatchingPool ? Cast<UB2FloatingWidget>(MatchingPool->Rent(InPC, InOwnerChar)) : nullptr;
}

void AB2UIManager_InGameCombat::TurnInFloatingWidget(EFloatingWidgetPoolClassType InRentWidgetType, UB2FloatingWidget* InUsedOne)
{
	FFloatingWidgetPool* MatchingPool = GetMatchingFloatingWidgetPool(InRentWidgetType);
	if (MatchingPool)
	{
		MatchingPool->TurnIn(InUsedOne);
	}
}

TSubclassOf<class UB2FloatingDamageNum> AB2UIManager_InGameCombat::GetDamageNumWidgetClassByOwnerChar(class ABladeIICharacter* InOwnerChar)
{
	if (InOwnerChar)
	{
		if (InOwnerChar->IsA(ABladeIIPlayer::StaticClass())){
			return DamageNumWidgetClass_PC;
		}
		else{
			return DamageNumWidgetClass_Mob;
		}
	}
	return nullptr;
}
EFloatingWidgetPoolClassType AB2UIManager_InGameCombat::GetDamageNumWidgetPoolTypeByOwnerChar(class ABladeIICharacter* InOwnerChar)
{
	if (InOwnerChar)
	{
		if (InOwnerChar->IsA(ABladeIIPlayer::StaticClass())) {
			return EFloatingWidgetPoolClassType::EFWPCT_DamageNum_PC;
		}
		else {
			return EFloatingWidgetPoolClassType::EFWPCT_DamageNum_Mob;
		}
	}
	return EFloatingWidgetPoolClassType::EFWPCT_DamageNum_Mob;
}
////////////////////////////////////// FloatingWidgetPool


//[FIXME] [@AKI, 170818] 10개의 몹을 타격하게 될경우 O(N^2)을 하여 100번 루프를 하게 됨 -> 으악 (djsong)
class UB2UIManager_InGameHUDChar* AB2UIManager_InGameCombat::GetRelevantUIManager(class ABladeIICharacter* InChar)
{
	if (InChar == NULL)
	{
		return NULL;
	}

	//ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(InChar);

	//if (CastedPlayer && CastedPlayer == UGameplayStatics::GetLocalPlayerCharacter(this) &&
	//	PlayerHUDManager && PlayerHUDManager->GetOwnerChar() == InChar)
	//{
	//	return PlayerHUDManager;
	//}

	//for (int32 HI = 0; HI < AllNPCHUDManager.Num(); ++HI)
	//{
	//	UB2UIManager_InGameHUDChar* CurrHUDManager = AllNPCHUDManager[HI];

	//	if (InChar == CurrHUDManager->GetOwnerChar())
	//	{
	//		return CurrHUDManager;
	//	}
	//}
	return NULL;
}

void AB2UIManager_InGameCombat::SubscribeEvents()
{
	if (bEventsSubscribed)
		return;

	//// 월드 BeginPlay 이전 시점에 불려야 함
	//// InGameCombat UIManager 하나이므로 ID 저장할 필요없이 추후 UnsubscribeAll 로
	//CAPTURE_UOBJECT(AB2UIManager_InGameCombat);

	//StageBeginTicket = StageBeginClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(StageBegin)
	//		Capture->NotifyStageBegin(); 
	//	END_CAPTURE_OBJECT()
	//);
	//CharacterBeginPlayTicket = CharacterBeginPlayClass<ABladeIICharacter*>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(CharacterBeginPlay, ABladeIICharacter* c)
	//		Capture->NotifyBeginPlay(c); 
	//	END_CAPTURE_OBJECT()
	//);
	//TargetDamagedTicket = TargetDamagedClass<ABladeIICharacter*, int32, float>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(TargetDamaged, ABladeIICharacter* c, int32 i, float f)
	//		Capture->NotifyTargetDamaged(c, i, f); 
	//	END_CAPTURE_OBJECT()
	//);
	//CharacterTakeDamageTicket = CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(CharacterTakeDamage, ABladeIICharacter* c, float f, bool b)
	//		Capture->NotifyTakeDamage(c, f, b); 
	//	END_CAPTURE_OBJECT()
	//);
	//CharacterAbnormalEffectTicket = CharacterAbnormalEffectClass<ABladeIICharacter*, const FLoadedFloatingAbnormalEffectSetupInfo&>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(CharacterAbnormalEffect, ABladeIICharacter* c, const FLoadedFloatingAbnormalEffectSetupInfo& a)
	//		Capture->NotifyAbnormalEffect(c, a); 
	//	END_CAPTURE_OBJECT()
	//);

	//CharacterDeathTicket = CharacterDeathClass<ABladeIICharacter*>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(CharacterDeath, ABladeIICharacter* c)
	//	Capture->NotifyCharacterDeath(c);
	//	END_CAPTURE_OBJECT()
	//	);

	//PlayerTaggedTicket = PlayerTaggedClass<ABladeIIPlayer*, ABladeIIPlayer*>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerTagged, ABladeIIPlayer* a, ABladeIIPlayer* b)
	//		Capture->NotifyPlayerTagged(a, b); 
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerSuckedGoldTicket = PlayerSuckedGoldClass<int32>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerSuckedGold, int32 i)
	//		Capture->NotifyPlayerSuckedGold(i); 
	//	END_CAPTURE_OBJECT()
	//);
	//SetHUDCenterMessageTicket = SetHUDCenterMessageClass<const FText, float, int32>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(SetHUDCenterMessage, const FText t, float f, int32 i)
	//		Capture->SetCenterMessage(t, f, i); 
	//	END_CAPTURE_OBJECT()
	//);
	//HideHUDCenterMessageTicket = HideHUDCenterMessageClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(HideHUDCenterMessage)
	//		Capture->HideCenterMessage(); 
	//	END_CAPTURE_OBJECT()
	//);

	//PlayerHideMobPointerTicket = PlayerHideMobPointerClass<bool>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerHideMobPointer, bool b)
	//		Capture->HideMobPointer(b);
	//	END_CAPTURE_OBJECT()
	//);

	//PlayerHideMobPointerTicket = PlayerHideMobPointerClass<bool>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerHideMobPointer, bool b)
	//	Capture->HideMobPointer(b);
	//END_CAPTURE_OBJECT()
	//	);

	//SetSpawnNotifyMessageTicket = SetSpawnNotifyMessageClass<const FString&, const float>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(SetSpawnNotifyMessage, const FString& Msg, const float time)
	//	Capture->SetSpawnNotifyMessage(Msg, time);
	//END_CAPTURE_OBJECT()
	//	);

	//RemoveSpawnNotifyMessageTicket = RemoveSpawnNotifyMessageClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(RemoveSpawnNotifyMessage)
	//	Capture->RemoveSpawnNotifyMessage();
	//END_CAPTURE_OBJECT()
	//	);

	bEventsSubscribed = true;
}

void AB2UIManager_InGameCombat::UnsubscribeEvents()
{
	if (!bEventsSubscribed)
		return;

	StageBeginClass<>::GetInstance().Unsubscribe(StageBeginTicket);
	CharacterBeginPlayClass<ABladeIICharacter*>::GetInstance().Unsubscribe(CharacterBeginPlayTicket);
	TargetDamagedClass<ABladeIICharacter*, int32, float>::GetInstance().Unsubscribe(TargetDamagedTicket);
	CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Unsubscribe(CharacterTakeDamageTicket);
	CharacterAbnormalEffectClass<ABladeIICharacter*, const FLoadedFloatingAbnormalEffectSetupInfo&>::GetInstance().Unsubscribe(CharacterAbnormalEffectTicket);
	PlayerTaggedClass<ABladeIIPlayer*, ABladeIIPlayer*>::GetInstance().Unsubscribe(PlayerTaggedTicket);
	PlayerSuckedGoldClass<int32>::GetInstance().Unsubscribe(PlayerSuckedGoldTicket);
	SetHUDCenterMessageClass<const FText, float, int32>::GetInstance().Unsubscribe(SetHUDCenterMessageTicket);
	HideHUDCenterMessageClass<>::GetInstance().Unsubscribe(HideHUDCenterMessageTicket);
	PlayerHideMobPointerClass<bool>::GetInstance().Unsubscribe(PlayerHideMobPointerTicket);
	CharacterDeathClass<ABladeIICharacter*>::GetInstance().Unsubscribe(CharacterDeathTicket);
	SetSpawnNotifyMessageClass<const FString&,const float>::GetInstance().Unsubscribe(SetSpawnNotifyMessageTicket);
	RemoveSpawnNotifyMessageClass<>::GetInstance().Unsubscribe(RemoveSpawnNotifyMessageTicket);

	bEventsSubscribed = false;
}

//////////////////////////////////////////////////////////////////////
// In-game UI notifications..

void AB2UIManager_InGameCombat::NotifyBeginPlay(class ABladeIICharacter* InNotifyOwner)
{
	if (!InNotifyOwner)
		return;

	UB2UIManager_InGameHUDChar* WantedUIManager = GetRelevantUIManager(InNotifyOwner);

	if (WantedUIManager)
	{
		WantedUIManager->NotifyBeginPlay();
	}
}

void AB2UIManager_InGameCombat::NotifyStageBegin()
{
	// 일단 필요한 건 플레이어 뿐
	if (PlayerHUDManager)
		PlayerHUDManager->NotifyStageBegin();

}

void AB2UIManager_InGameCombat::NotifyTargetDamaged(class ABladeIICharacter* InNotifyOwner, int32 SuccessiveComboNum, float DamageAmount)
{
	// 지금으로선 플레이어에만 필요.
	if (Cast<ABladeIIPlayer>(InNotifyOwner) && PlayerHUDManager)
	{
		PlayerHUDManager->NotifyTargetDamaged(SuccessiveComboNum, DamageAmount);
	}
}

void AB2UIManager_InGameCombat::NotifyAutoStateChange(bool bInAutoNow)
{
	if (PlayerHUDManager)
	{
		PlayerHUDManager->NotifyAutoStateChange(bInAutoNow);
	}

	//테스트코드(DocBattle로 이벤트 수신처를 바꿔야 한다)
	/*auto BattleUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain);
	if (BattleUI)
	{
		BattleUI->ReceiveAutoStateChange(bInAutoNow);
	}*/
}

void AB2UIManager_InGameCombat::NotifyPlayerTagged(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer)
{
	if (PlayerHUDManager)
	{
		PlayerHUDManager->NotifyPlayerTagged(OldPlayer, NewPlayer);
	}
}

void AB2UIManager_InGameCombat::NotifyPlayerSuckedGold(int32 AcquiredAmount)
{
	if (PlayerHUDManager)
	{
		PlayerHUDManager->NotifyPlayerSuckedGold(AcquiredAmount);
	}
}

void AB2UIManager_InGameCombat::NotifyTakeDamage(class ABladeIICharacter* InNotifyOwner, float Damage, bool bInCritical)
{
	if (!InNotifyOwner)
		return;

	UB2UIManager_InGameHUDChar* WantedUIManager = GetRelevantUIManager(InNotifyOwner);

	if (WantedUIManager)
	{
		WantedUIManager->NotifyTakeDamage(Damage, bInCritical);
	}
}

void AB2UIManager_InGameCombat::NotifyAbnormalEffect(class ABladeIICharacter* InNotifyOwner, const FLoadedFloatingAbnormalEffectSetupInfo& InEffectSetupInfo)
{
	if (!InNotifyOwner)
		return;

	UB2UIManager_InGameHUDChar* WantedUIManager = GetRelevantUIManager(InNotifyOwner);

	if (WantedUIManager)
	{
		WantedUIManager->NotifyAbnormalEffect(InEffectSetupInfo);
	}
}

void AB2UIManager_InGameCombat::NotifyCharacterDeath(class ABladeIICharacter* InNotifyOwner)
{
	if (ABladeIINetPlayer* CastedPlayer = Cast<ABladeIINetPlayer>(InNotifyOwner))
	{
		if (UB2UIManager_InGameHUDChar* WantedUIManager = GetRelevantUIManager(InNotifyOwner))
			WantedUIManager->NotifyCharacterDeath(InNotifyOwner);
	}
}

void AB2UIManager_InGameCombat::SetCenterMessage(const FText& InMessageText, float DisplayTime, int32 AnimIndex)
{
	if (TempCurrentOpenCenterPopup){
		HideCenterMessage(); // 한번에 하나씩. 기존에 자동으로 닫히지 않은 것이 있었다면 닫아준다.
	}

	TempCurrentOpenCenterPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupCenter>(EUIMsgPopup::Center,
		InMessageText,
		FText::FromString(TEXT("")),
		DisplayTime,
		false
	);

	// 오픈 애니메이션인데 적어도 기본 애니메이션은 플레이 해야 메시지가 보인다. AnimIndex 가 잘못되어도 BP 에서 기본 플레이는 할 것.
	UB2UIMsgPopupCenter* CastedPopupCenter = Cast<UB2UIMsgPopupCenter>(TempCurrentOpenCenterPopup);
	if (CastedPopupCenter){
		CastedPopupCenter->PlayOpenAnimation(AnimIndex);
	}

	if (DisplayTime > 0.0f){ // 0 보다 큰 경우 자동으로 닫힐 것이므로 굳이 이걸 들고 있지 않는다.
		TempCurrentOpenCenterPopup = NULL;
	}
}

void AB2UIManager_InGameCombat::HideCenterMessage()
{
	//// 앞서 SetCenterMessage 를 했을 때 DisplayTime 을 0 이하로 주었다면 있어야. 하지만 레벨이 바뀐 후라면 의미 없음. UB2UIManager 는 static 이라 레벨이 바뀌어도 그대로임. 그런 경우는 CloseAllMsgPopups 을 하거나 레퍼런스를 다른 식으로 저장해야..
	//if (TempCurrentOpenCenterPopup)
	//{
	//	UB2UIManager::GetInstance()->CloseMsgPopup(TempCurrentOpenCenterPopup);
	//	TempCurrentOpenCenterPopup = NULL;
	//}

	//if (PlayerHUDManager)
	//{
	//	PlayerHUDManager->HideCenterMessage();
	//}
}

void AB2UIManager_InGameCombat::SetSpawnNotifyMessage(const FString& InMessageText, const float InMessageTime)
{
	if (auto* BattleMessage = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupBattleMessageNotify>(UIFName::BattleMessageNotify))
		return;

	auto* MessagePopUp = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupBattleMessageNotify>(UIFName::BattleMessageNotify);

	if (MessagePopUp)
	{
		MessagePopUp->SetMessage(InMessageText, InMessageTime);
	}
}

void AB2UIManager_InGameCombat::RemoveSpawnNotifyMessage()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::BattleMessageNotify);
}

//////////////////////////////////////////////////////////////////////

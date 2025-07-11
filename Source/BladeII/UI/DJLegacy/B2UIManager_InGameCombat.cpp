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
// �̸��� InGameCombat �̰� Lobby ������ ����ϴ� ������ �� ����.
// �Ƹ��� ��� ���� ���� DJLegacy UI �Ϻ�
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

	
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GMinimalDLCFrontMode) // DLC Front ��� ���ҽ��ε� �ִ��� ����. �ٵ� ���� �̰� DLCFront ��忡�� �������� �ʰ���? ��
	{ 
		// ���⼭ �ε��ϴ� WidgetClass ������ ó�� �����ÿ��� BladeIICharacter �� BladeIIPlayer �� �־�� ini ���� �̸��� �װ� �ݿ��ϰ� ����. 
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
		PlayerHUDManager->UpdateManual(DeltaSeconds); // CustomTimeDilation �� ����ؾ� �ϸ� GetDeltaTimeAsOtherWorldObj ��.. ���⿡ CustomTimeDilation �� ������ ���� ���� ���� �� ���� ������ 
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
	// UObject �� ���õ� destroy �� ���⼭
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
	//if (CastedPlayer != UGameplayStatics::GetLocalPlayerCharacter(this)) // ����Ʈ �÷��̾�� �÷��̾� ��� �ƴ�.
	//{
	//	CastedPlayer = NULL;
	//}

	//if (CastedPlayer)
	//{
	//	// �� ��� Player �� HUD ��
	//	NewUIManager = NewObject<UB2UIManager_InGameHUDPlayer>(this, *FString::Printf(TEXT("UIManager_InGameHUD_%s"), *CastedPlayer->GetName()), RF_Transient);
	//}
	//else
	//{
	//	NewUIManager = NewObject<UB2UIManager_InGameHUDChar>(this, *FString::Printf(TEXT("UIManager_InGameHUD_%s"), *InNewCreated->GetName()), RF_Transient);
	//}

	//if (NewUIManager && NewUIManager->InitUIManager(InNewCreated, this) == false) // PlayerController ������ ���� ������ �� ����.
	//{
	//	NewUIManager->ConditionalBeginDestroy();
	//	NewUIManager = NULL;
	//}

	//if (NewUIManager)
	//{
	//	// ���� ������ ���� ���ʿ� ���
	//	if (CastedPlayer)
	//	{
	//		if (PlayerHUDManager) // ���� �ϳ� ������ �־��ٸ� �ı�
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
//	// Not for local player character. ABladeIIPlayer �� �̰� ����ϴ� ��찡 �ֱ� ��.
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

// HideFloatingHPBarForChar �Լ��� ����ó��(�÷��̾��϶� ������ ������ �����״�..)
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
	//{ // �Ƹ� DamageNum_Mob �� ���� ���� �� ���� �ѵ� ���� Ÿ�Ժ��� PreCreate ������ �ٸ��� ������ �ʿ���� ���� �� ����.
	//	GConfig->GetInt(TEXT("/Script/BladeII"), TEXT("FloatingWidgetPoolPreCreateNum"), PooledObjectPreCreateCount, GGameIni);
	//}

	//// �κ񿡼� �̰� ������ ���� �ϵ��� ������ ����.
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


//[FIXME] [@AKI, 170818] 10���� ���� Ÿ���ϰ� �ɰ�� O(N^2)�� �Ͽ� 100�� ������ �ϰ� �� -> ���� (djsong)
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

	//// ���� BeginPlay ���� ������ �ҷ��� ��
	//// InGameCombat UIManager �ϳ��̹Ƿ� ID ������ �ʿ���� ���� UnsubscribeAll ��
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
	// �ϴ� �ʿ��� �� �÷��̾� ��
	if (PlayerHUDManager)
		PlayerHUDManager->NotifyStageBegin();

}

void AB2UIManager_InGameCombat::NotifyTargetDamaged(class ABladeIICharacter* InNotifyOwner, int32 SuccessiveComboNum, float DamageAmount)
{
	// �������μ� �÷��̾�� �ʿ�.
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

	//�׽�Ʈ�ڵ�(DocBattle�� �̺�Ʈ ����ó�� �ٲ�� �Ѵ�)
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
		HideCenterMessage(); // �ѹ��� �ϳ���. ������ �ڵ����� ������ ���� ���� �־��ٸ� �ݾ��ش�.
	}

	TempCurrentOpenCenterPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupCenter>(EUIMsgPopup::Center,
		InMessageText,
		FText::FromString(TEXT("")),
		DisplayTime,
		false
	);

	// ���� �ִϸ��̼��ε� ��� �⺻ �ִϸ��̼��� �÷��� �ؾ� �޽����� ���δ�. AnimIndex �� �߸��Ǿ BP ���� �⺻ �÷��̴� �� ��.
	UB2UIMsgPopupCenter* CastedPopupCenter = Cast<UB2UIMsgPopupCenter>(TempCurrentOpenCenterPopup);
	if (CastedPopupCenter){
		CastedPopupCenter->PlayOpenAnimation(AnimIndex);
	}

	if (DisplayTime > 0.0f){ // 0 ���� ū ��� �ڵ����� ���� ���̹Ƿ� ���� �̰� ��� ���� �ʴ´�.
		TempCurrentOpenCenterPopup = NULL;
	}
}

void AB2UIManager_InGameCombat::HideCenterMessage()
{
	//// �ռ� SetCenterMessage �� ���� �� DisplayTime �� 0 ���Ϸ� �־��ٸ� �־��. ������ ������ �ٲ� �Ķ�� �ǹ� ����. UB2UIManager �� static �̶� ������ �ٲ� �״����. �׷� ���� CloseAllMsgPopups �� �ϰų� ���۷����� �ٸ� ������ �����ؾ�..
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

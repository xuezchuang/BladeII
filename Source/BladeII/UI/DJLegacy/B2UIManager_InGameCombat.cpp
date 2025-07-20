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
// 捞抚父 InGameCombat 捞绊 Lobby 俊辑档 荤侩窍绰 备籍捞 粱 乐澜.
// 酒付档 拌加 荤侩瞪 淀茄 DJLegacy UI 老何
//
//////////////////////////////////////////////////

AB2UIManager_InGameCombat::AB2UIManager_InGameCombat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = false;
	NetUpdateFrequency = 10.0f;
	//bHidden = true;
	SetReplicateMovement(false);
	SetCanBeDamaged(false);

	TempCurrentOpenCenterPopup = NULL;
	PlayerHUDManager = NULL;

	
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GMinimalDLCFrontMode) // DLC Front 葛靛 府家胶肺爹 弥措茄 力芭. 辟单 汲付 捞扒 DLCFront 葛靛俊辑 积己登瘤 臼摆瘤? せ
	{ 
		// 咯扁辑 肺爹窍绰 WidgetClass 汲沥捞 贸澜 备泅矫俊绰 BladeIICharacter 唱 BladeIIPlayer 俊 乐菌绢辑 ini 冀记 捞抚捞 弊吧 馆康窍绊 乐澜. 
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
		PlayerHUDManager->UpdateManual(DeltaSeconds); // CustomTimeDilation 阑 绊妨秦具 窍搁 GetDeltaTimeAsOtherWorldObj 甫.. 咯扁俊 CustomTimeDilation 阑 利侩且 老捞 沥富 绝阑 芭 鞍变 窍瘤父 
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
	// UObject 客 包访等 destroy 绰 咯扁辑
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
	//if (CastedPlayer != UGameplayStatics::GetLocalPlayerCharacter(this)) // 府葛飘 敲饭捞绢绰 敲饭捞绢 秒鞭 酒丛.
	//{
	//	CastedPlayer = NULL;
	//}

	//if (CastedPlayer)
	//{
	//	// 捞 版快 Player 侩 HUD 肺
	//	NewUIManager = NewObject<UB2UIManager_InGameHUDPlayer>(this, *FString::Printf(TEXT("UIManager_InGameHUD_%s"), *CastedPlayer->GetName()), RF_Transient);
	//}
	//else
	//{
	//	NewUIManager = NewObject<UB2UIManager_InGameHUDChar>(this, *FString::Printf(TEXT("UIManager_InGameHUD_%s"), *InNewCreated->GetName()), RF_Transient);
	//}

	//if (NewUIManager && NewUIManager->InitUIManager(InNewCreated, this) == false) // PlayerController 备己俊 蝶扼 角菩且 荐 乐澜.
	//{
	//	NewUIManager->ConditionalBeginDestroy();
	//	NewUIManager = NULL;
	//}

	//if (NewUIManager)
	//{
	//	// 货肺 积己茄 巴阑 捞率俊 殿废
	//	if (CastedPlayer)
	//	{
	//		if (PlayerHUDManager) // 父俊 窍唱 扁粮俊 乐菌促搁 颇鲍
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
//	// Not for local player character. ABladeIIPlayer 扼档 捞吧 荤侩窍绰 版快啊 乐变 窃.
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

// HideFloatingHPBarForChar 窃荐尔 蝶肺贸府(敲饭捞绢老锭 府畔茄 捞蜡啊 乐阑抛聪..)
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
	//{ // 酒付 DamageNum_Mob 捞 力老 腹阑 巴 鞍变 茄单 被捞 鸥涝喊肺 PreCreate 俺荐甫 促福霸 啊廉哎 鞘夸鳖柳 绝阑 巴 鞍促.
	//	GConfig->GetInt(TEXT("/Script/BladeII"), TEXT("FloatingWidgetPoolPreCreateNum"), PooledObjectPreCreateCount, GGameIni);
	//}

	//// 肺厚俊辑 捞吧 积己秦 初阑 窍殿狼 捞蜡啊 绝促.
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


//[FIXME] [@AKI, 170818] 10俺狼 各阑 鸥拜窍霸 瞪版快 O(N^2)阑 窍咯 100锅 风橇甫 窍霸 凳 -> 栏厩 (djsong)
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

	//// 岿靛 BeginPlay 捞傈 矫痢俊 阂妨具 窃
	//// InGameCombat UIManager 窍唱捞骨肺 ID 历厘且 鞘夸绝捞 眠饶 UnsubscribeAll 肺
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
	// 老窜 鞘夸茄 扒 敲饭捞绢 挥
	if (PlayerHUDManager)
		PlayerHUDManager->NotifyStageBegin();

}

void AB2UIManager_InGameCombat::NotifyTargetDamaged(class ABladeIICharacter* InNotifyOwner, int32 SuccessiveComboNum, float DamageAmount)
{
	// 瘤陛栏肺急 敲饭捞绢俊父 鞘夸.
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

	//抛胶飘内靛(DocBattle肺 捞亥飘 荐脚贸甫 官层具 茄促)
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
		HideCenterMessage(); // 茄锅俊 窍唱究. 扁粮俊 磊悼栏肺 摧洒瘤 臼篮 巴捞 乐菌促搁 摧酒霖促.
	}

	TempCurrentOpenCenterPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupCenter>(EUIMsgPopup::Center,
		InMessageText,
		FText::FromString(TEXT("")),
		DisplayTime,
		false
	);

	// 坷锹 局聪皋捞记牢单 利绢档 扁夯 局聪皋捞记篮 敲饭捞 秦具 皋矫瘤啊 焊牢促. AnimIndex 啊 肋给登绢档 BP 俊辑 扁夯 敲饭捞绰 且 巴.
	UB2UIMsgPopupCenter* CastedPopupCenter = Cast<UB2UIMsgPopupCenter>(TempCurrentOpenCenterPopup);
	if (CastedPopupCenter){
		CastedPopupCenter->PlayOpenAnimation(AnimIndex);
	}

	if (DisplayTime > 0.0f){ // 0 焊促 奴 版快 磊悼栏肺 摧鳃 巴捞骨肺 被捞 捞吧 甸绊 乐瘤 臼绰促.
		TempCurrentOpenCenterPopup = NULL;
	}
}

void AB2UIManager_InGameCombat::HideCenterMessage()
{
	//// 菊辑 SetCenterMessage 甫 沁阑 锭 DisplayTime 阑 0 捞窍肺 林菌促搁 乐绢具. 窍瘤父 饭骇捞 官诧 饶扼搁 狼固 绝澜. UB2UIManager 绰 static 捞扼 饭骇捞 官差绢档 弊措肺烙. 弊繁 版快绰 CloseAllMsgPopups 阑 窍芭唱 饭欺繁胶甫 促弗 侥栏肺 历厘秦具..
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

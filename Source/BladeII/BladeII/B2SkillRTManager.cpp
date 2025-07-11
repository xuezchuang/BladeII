//// Fill out your copyright notice in the Description page of Project Settings.
//// Project BladeII, Action Square
//
//
//#include "B2SkillRTManager.h"
//
////#include "B2PCClassInfoBox.h"
////#include "B2SkillInfo.h"
//#include "B2SkillRTManager.h"
//#include "BladeIIPlayer.h"
//#include "BladeIIPlayerController.h"
//#include "BladeIIGameMode.h"
//#include "B2UIDocHelper.h"
//#include "B2StageManager.h"
//#include "B2StageGameModeBase.h"
//#include "BladeIIUtil.h"
//#include "Event.h"
//#include "EventSubsystem.h"
//#include "B2CounterAttackGameMode.h"
//#include "B2TutorialGameMode.h"
//#include "B2RaidGameMode.h"
////#include "BladeIITMAIPlayer.h"
//#include "B2CombatStatEvaluator.h"
//#include "BladeIIPlayerAIController.h"
//#include "B2Buff_Base.h"
//
//#include "B2GameInstance.h"
//#include "BladeIIGameImpl.h"
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//float Cheat_ForceCooltime_Skill = -1.f;
//float Cheat_ForceCooltime_Tag = -1.f;
//#endif
//
//DEFINE_LOG_CATEGORY_STATIC(LogB2SkillRTManager, Log, All);
//
//void FInputBoundSkillState_Base::InitSkillState(UB2SkillRTManager* SkillRTManager, int32 InSkillID, EPCClass InPCClass, EAttackState InAttackState, float InMaxCooltime, float InEnableWeaponSkillPoint, bool bWeaponSkill)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Base_InitSkillState);
//	check(SkillRTManager);
//
//	SkillID = InSkillID;
//	UsePCClass = InPCClass;
//	MappingAttackState = InAttackState;
//	MaxCooltime = InMaxCooltime;
//	EnableWeaponSkillPoint = InEnableWeaponSkillPoint;
//	IncreaseMotion = UB2SkillRTManager::GetCharacterSkillIncMotionValue(UsePCClass, GetB2GameModeType(SkillRTManager), SkillID, SkillRTManager->GetCurrentCharDataStore());
//	bCurrentSuggest = false;
//
//	ResetSkillState(SkillRTManager);
//}
//
//void FInputBoundSkillState_Base::ResetSkillState(UB2SkillRTManager* SkillRTManager)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Base_ResetSkillState);
//	SetRemainingCooltime(0.0f);
//	SetSkillDisabledByCooltime(false);	
//	SetIsCountingCooltime(false);
//}
//
//float FInputBoundSkillState_Base::GetMaxCooltime(const UB2SkillRTManager* SkillRTManager)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_GetMaxCooltime);
//	check(SkillRTManager);
//
//	// ActiveSkill 에 바인딩 되는 쿨타임 감소.
//	// 현재 ECombatOptionApplyType 이 Subtract_Value 인 구현으로 OneMinusMultiply 로 바뀔 수 있다.
//	return FMath::Clamp(MaxCooltime - SkillRTManager->GetSkillOptionValue(SkillID, ESkillOption::ESO_DecCoolTime), 0.f, MaxCooltime);
//}
//
//void FInputBoundSkillState_Skill::InitSkillState(UB2SkillRTManager* SkillRTManager, int32 InSkillID, EPCClass InPCClass, EAttackState InAttackState, float InMaxCooltime, float InEnableWeaponSkillPoint, bool bWeaponSkill)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_InitSkillState);
//	bIsWeaponSkill = bWeaponSkill;
//
//	FInputBoundSkillState_Base::InitSkillState(SkillRTManager, InSkillID, InPCClass, InAttackState, InMaxCooltime, InEnableWeaponSkillPoint, bWeaponSkill);
//}
//
//void FInputBoundSkillState_Skill::ResetSkillState(UB2SkillRTManager* SkillRTManager)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_ResetSkillState);
//	if (bIsWeaponSkill)
//	{
//		SetSkillPoint(SkillRTManager->GetActiveCharacterWeaponSkillPoint());
//	}
//
//	FInputBoundSkillState_Base::ResetSkillState(SkillRTManager);
//}
//
//
////added by hsh from 20200410 스킬 재사용 최저 계수 추가
//#define MIN_COOLTIME_CAL_VALUE 0.2f  
//float FInputBoundSkillState_Skill::GetMaxCooltime(const UB2SkillRTManager* SkillRTManager)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_GetMaxCooltime);
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (Cheat_ForceCooltime_Skill >= 0.0f)
//	{
//		return Cheat_ForceCooltime_Skill;
//	}
//#endif
//
//	if (SkillRTManager)
//	{
//		// Active Skill 에 바인딩 된 쿨타임 감소 옵션과 Passive 로 적용되는 쿨타임 감소 옵션 따로 있음. 그리고 또한 아예 별도로 특정 스킬에만 적용되는 쿨타임 감소 옵션이 또 있는데 딴 데서 처리.
//		// 현재 ECombatOptionApplyType 이 Subtract_Value 인 구현으로 OneMinusMultiply 로 바뀔 수 있다.
//
//		float ActiveCoolDecAppliedCooltime = FInputBoundSkillState_Base::GetMaxCooltime(SkillRTManager);
//		float fMinCooltime = MaxCooltime * MIN_COOLTIME_CAL_VALUE;
//		fMinCooltime = ((int)(fMinCooltime * 10)) / 10.0f;
//		
//		return FMath::Clamp(ActiveCoolDecAppliedCooltime - SkillRTManager->GetSkillCooltimeDecbyPassive(), fMinCooltime, ActiveCoolDecAppliedCooltime);
//	}
//	return 0.0f;
//}
//
//float FInputBoundSkillState_Tag::GetMaxCooltime(const UB2SkillRTManager* SkillRTManager)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Tag_GetMaxCooltime);
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (Cheat_ForceCooltime_Tag >= 0.0f)
//	{
//		return Cheat_ForceCooltime_Tag;
//	}
//#endif
//
//	if (SkillRTManager)
//	{
//		// Active Skill 에 바인딩 된 쿨타임 감소 옵션과 Passive 로 적용되는 쿨타임 감소 옵션 따로 있음.
//		// 현재 ECombatOptionApplyType 이 Subtract_Value 인 구현으로 OneMinusMultiply 로 바뀔 수 있다.
//		float ActiveCoolDecAppliedCooltime = FInputBoundSkillState_Base::GetMaxCooltime(SkillRTManager);
//		return FMath::Clamp(ActiveCoolDecAppliedCooltime - SkillRTManager->GetTagCooltimeDecbyPassive(), 0.0f, ActiveCoolDecAppliedCooltime);
//	}
//	return 0.0f;
//}
//
//UB2UIDocSkill* FInputBoundSkillState_Skill::GetSkillDoc()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_GetSkillDoc);
//	UB2UIDocBase* test = GetDoc();
//	return Cast<UB2UIDocSkill>(GetDoc());
//}
//
//UB2UIDocBattle* FInputBoundSkillState_Tag::GetBattleDoc()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Tag_GetBattleDoc);
//	return Cast<UB2UIDocBattle>(GetDoc());
//}
//
//bool FInputBoundSkillState_Skill::IsSkillDisabledByPoint() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_IsSkillDisabledByPoint);
//	if (!bIsWeaponSkill)
//		return false;
//
//	return CachedWeaponStackPoint < EnableWeaponSkillPoint;
//}
//
//void FInputBoundSkillState_Skill::SetSkillDisabledByCooltime(bool bIn)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_SetSkillDisabledByCooltime);
///*
//	if (bIsWeaponSkill)
//		return;*/
//
//	bIsSkillDisabledByCooltime = bIn;
//
//	if (auto* DocSkill = GetSkillDoc())
//	{
//		DocSkill->SetIsDisabledByCooltime(bIsSkillDisabledByCooltime);
//	}
//}
//
//void FInputBoundSkillState_Skill::SetSkillPoint(float Point)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_SetSkillPoint);
//	if (!bIsWeaponSkill)
//		return;
//	
//	if (GetSkillDoc())
//	{
//		GetSkillDoc()->SetSkillStackPoint(Point);
//	}
//
//	CachedWeaponStackPoint = Point;
//}
//
//void FInputBoundSkillState_IgnorePoint::SetSkillPoint(float Point)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_IgnorePoint_SetSkillPoint);
//	if (!bIsWeaponSkill)
//		return;
//
//	CachedWeaponStackPoint = Point;
//}
//
//void FInputBoundSkillState_Tag::SetSkillDisabledByCooltime(bool bIn)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Tag_SetSkillDisabledByCooltime);
//	bIsSkillDisabledByCooltime = bIn;
//
//	if (GetBattleDoc())
//	{
//		GetBattleDoc()->SetIsTagDisabledByCooltime(bIsSkillDisabledByCooltime);
//	}
//}
//
//void FInputBoundSkillState_Skill::SetRemainingCooltime(float fIn)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_SetRemainingCooltime);
///*
//	if (bIsWeaponSkill)
//		return;*/
//
//	RemainingCooltime = fIn;
//	if (GetSkillDoc())
//	{
//		GetSkillDoc()->SetRemainingCooltime(RemainingCooltime);
//	}
//}
//
//void FInputBoundSkillState_Tag::SetRemainingCooltime(float fIn)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Tag_SetRemainingCooltime);
//	RemainingCooltime = fIn;
//	if (GetBattleDoc())
//	{
//		GetBattleDoc()->SetRemainingTagCooltime(RemainingCooltime);
//	}
//}
//
//void FInputBoundSkillState_Skill::SetIsCountingCooltime(bool bIn)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_SetIsCountingCooltime);
//	/*if (bIsWeaponSkill)
//		return;*/
//
//	bCountingCooltime = bIn;
//	if (GetSkillDoc())
//	{
//		GetSkillDoc()->SetIsCountingCooltime(bCountingCooltime);
//	}
//}
//
//
//void FInputBoundSkillState_Skill::SetSkillSuggest(bool bSuggest)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_SetSkillSuggest);
//	bCurrentSuggest = bSuggest;
//	if (GetSkillDoc())
//	{
//		GetSkillDoc()->SetIsSuggestSkill(bSuggest);
//	}
//}
//
//
//void FInputBoundSkillState_Skill::SetSkillDisableBySuggest(bool bDisable)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_SetSkillDisableBySuggest);
//	bIsSkillDisabledByOtherSuggest = bDisable;
//	if (GetSkillDoc())
//	{
//		GetSkillDoc()->SetIsDisabledByOtherSuggest(bDisable);
//	}
//}
//
//void FInputBoundSkillState_Tag::SetIsCountingCooltime(bool bIn)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Tag_SetIsCountingCooltime);
//	bCountingCooltime = bIn;
//	if (GetBattleDoc())
//	{
//		GetBattleDoc()->SetIsCountingTagCooltime(bCountingCooltime);
//	}
//}
//
//void FInputBoundSkillState_Skill::SyncToUIDoc()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_SyncToUIDoc);
//	if (GetSkillDoc())
//	{
//		if (bIsWeaponSkill)
//		{
//			GetSkillDoc()->SetSkillStackMaxPoint(EnableWeaponSkillPoint);
//			GetSkillDoc()->SetSkillStackPoint(CachedWeaponStackPoint);
//		}
//
//		GetSkillDoc()->SetIsDisabledByCooltime(bIsSkillDisabledByCooltime);		
//		GetSkillDoc()->SetRemainingCooltime(RemainingCooltime);
//		GetSkillDoc()->SetIsCountingCooltime(bCountingCooltime);
//	}
//}
//
//void FInputBoundSkillState_Tag::SyncToUIDoc()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Tag_SyncToUIDoc);
//	if (GetBattleDoc())
//	{
//		GetBattleDoc()->SetIsTagDisabledByCooltime(bIsSkillDisabledByCooltime);
//		GetBattleDoc()->SetRemainingTagCooltime(RemainingCooltime);
//		GetBattleDoc()->SetIsCountingTagCooltime(bCountingCooltime);
//	}
//}
//
//void FInputBoundSkillState_Skill::UpdateUIDoc(class UB2SkillRTManager* SkillRTManager)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Skill_UpdateUIDoc);
//	if (GetSkillDoc())
//	{
//		GetSkillDoc()->SetFinalCooltime(GetMaxCooltime(SkillRTManager));
//	}
//
//	SyncToUIDoc();
//}
//
//void FInputBoundSkillState_Tag::UpdateUIDoc(class UB2SkillRTManager* SkillRTManager)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInputBoundSkillState_Tag_UpdateUIDoc);
//	if (GetBattleDoc())
//	{ 
//		GetBattleDoc()->SetFinalTagCooltime(GetMaxCooltime(SkillRTManager));
//	}
//
//	SyncToUIDoc();
//}
//
//void FInactivePlayerStateCache::Tick(float DeltaTime)
//{
//	Timer += DeltaTime;
//
//	if (Timer < Interval)
//		return;
//
//	Timer = 0.0f;
//
//	float HPRestoreLimit = MaxHealth * InactiveCharHPRestoreLimit;
//
//	if (CurrentHealth >= HPRestoreLimit || CurrentHealth <= 0.f)
//	{
//		StopTick();
//		return;
//	}
//
//	CurrentHealth += MaxHealth * InactiveCharHPRestoreRate;
//	CurrentHealth  = FMath::Min(CurrentHealth, HPRestoreLimit);
//}
//
//bool FInactivePlayerStateCache::IsTickable() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInactivePlayerStateCache_IsTickable);
//	return bTickable;
//}
//
//TStatId FInactivePlayerStateCache::GetStatId() const
//{
//	RETURN_QUICK_DECLARE_CYCLE_STAT(FInactivePlayerStateCache, STATGROUP_Tickables);
//}
//
//bool FInactivePlayerStateCache::IsTickableWhenPaused() const
//{
//	return false;
//}
//
//bool FInactivePlayerStateCache::IsTickableInEditor() const
//{
//	return false;
//}
//
//void FInactivePlayerStateCache::SetVariablesByRetiringPlayer(class ABladeIIPlayer* RetiringPlayer)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInactivePlayerStateCache_SetVariablesByRetiringPlayer);
//	if (RetiringPlayer)
//	{
//		LastHealth = RetiringPlayer->Health;
//		CurrentHealth = RetiringPlayer->Health; // 바뀐 순간에는 LastHealth 와 CurrentHealth 가 같고, 쉬는 도중에는 HP 가 회복.
//		MaxHealth = RetiringPlayer->GetMaxHealth();
//		LastArmor = RetiringPlayer->Armor;
//		WeaponSkillPoint = RetiringPlayer->GetWeaponSkillPoint();
//		IsUnityTick = RetiringPlayer->GetUnityTick();
//		GuardCoolTime = RetiringPlayer->GetGuardCoolTime();
//		HasNotTagDefence = RetiringPlayer->GetHasNotTagDefence();
//	}
//}
//
//void FInactivePlayerStateCache::SetToNewCommingPlayer(class ABladeIIPlayer* NewCommingPlayer)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FInactivePlayerStateCache_SetToNewCommingPlayer);
//	if (NewCommingPlayer)
//	{
//		NewCommingPlayer->Health = CurrentHealth; // 쉬는 도중 복구된 CurrentHealth 값을 사용.
//		NewCommingPlayer->Armor = LastArmor;
//		NewCommingPlayer->SetWeaponSkillPoint(WeaponSkillPoint);
//		NewCommingPlayer->SetUnityTick(IsUnityTick);
//		NewCommingPlayer->SetGuardCoolTime(GuardCoolTime);
//		NewCommingPlayer->SetHasNotTagDefence(HasNotTagDefence);
//		// MaxHealth 는 Player spawn 시 계산이 되며 InactivePlayerStateCache 에서 가져올 필요 없음 (실은 가져와선 안될 것..). 
//		// InactivePlayerStateCache 의 MaxHealth 는 단지 숨어있는 캐릭터의 UI 쪽 HP 바 표시를 위한 것
//	}
//}
//
//UB2SkillRTManager::UB2SkillRTManager(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_UB2SkillRTManager);
//	CurrentOwnerPlayer = NULL;
//	MainPlayerClass = EPCClass::EPC_End;
//	SubPlayerClass = EPCClass::EPC_End;
//
//	bInitialized = false;
//
//	AutoBattle = AutoBattleType::Skill;
//}
////
////void UB2SkillRTManager::BeginDestroy()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_BeginDestroy);
////	for (auto* ToDelete : IBSkillStates)
////	{
////		delete ToDelete;
////	}
////	IBSkillStates.Empty();
////
////	UnsubscribeEvents();
////
////	Super::BeginDestroy();
////}
////
////void UB2SkillRTManager::SubscribeEvents()
////{
////	UnsubscribeEvents();
////
////	Issues.Add(DeliveryResurrectionClass<FB2Resurrection>::GetInstance().Subscribe2(
////		[this](const FB2Resurrection& ResurrectionResult)
////	{
////		this->ResurrectOnResponse(ResurrectionResult);
////	}
////	));
////
////	Issues.Add(DeliveryRaidResurrectionClass<FB2RaidResurrection>::GetInstance().Subscribe2(
////		[this](const FB2RaidResurrection& ResurrectionResult)
////	{
////		this->RaidResurrectOnResponse(ResurrectionResult);
////	}
////	));
////}
////
////void UB2SkillRTManager::UnsubscribeEvents()
////{
////	Issues.Empty();
////}
////
////void UB2SkillRTManager::InitSkillRTManager(ABladeIIPlayer* InOwnerPlayer)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_InitSkillRTManager);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::InitSkillRTManager"));
////
////	if (GetB2GameModeType(nullptr) != EB2GameMode::Tutorial)
////	{
////		if (bInitialized)
////			return;
////	}
////
////	bInitialized = true;
////
////	SubscribeEvents();
////
////	InitSkillRTManagerInner(InOwnerPlayer);	
////}
////
////void UB2SkillRTManager::InitSkillRTManagerInner(ABladeIIPlayer* InOwnerPlayer)
////{
////	SetCurrentOwnerPlayer(InOwnerPlayer); // CurrentOwnerPlayer 는 게임 플레이를 거치면서 바뀔 수 있다.
////	
////	if (CurrentOwnerPlayer)
////	{
////		CachedSkillInfo = GetSkillInfoOfPlayer(CurrentOwnerPlayer);
////
////		// RTManager 자체가 설계방식이 태그만 고려한듯..
////		// 무조건 Main으로 선택한 캐릭터만 Cache하는 방식이라 팀대전, 길드전 같은 게임모드에서는 제대로 동작 안함
////		EB2GameMode CurrentGameMode = GetB2GameModeType(CurrentOwnerPlayer);
////		if (CurrentGameMode == EB2GameMode::Guild || CurrentGameMode == EB2GameMode::PVP_Team)
////		{
////			MainPlayerClass = CurrentOwnerPlayer->GetCurrentPlayerClass();
////		}
////		else
////		{
////			if (CurrentOwnerPlayer->GetCharacterDataStore())
////			{
////				MainPlayerClass = CurrentOwnerPlayer->GetCharacterDataStore()->GetMainPlayerClass();
////				SubPlayerClass = CurrentOwnerPlayer->GetCharacterDataStore()->GetSubPlayerClass();
////			}
////			else
////			{
////				MainPlayerClass = EPCClass::EPC_End;
////				SubPlayerClass = EPCClass::EPC_End;
////			}
////		}
////
////		// Tutorial
////		if (AllocateSkillState(IBSkillStates) || GetB2GameModeType(CurrentOwnerPlayer) == EB2GameMode::Tutorial)
////			SetPlayerCharacterSkill(CurrentOwnerPlayer, IBSkillStates, CachedSkillOption[0]);
////
////		if (NeedsRebindSkillDoc(CurrentOwnerPlayer))
////			BindSkillDoc(CurrentOwnerPlayer, IBSkillStates);
////
////		// 이걸 처음에 현재 플레이어 것으로 세팅하는 건 단지 UI 단에 뭔가 보여주기 위함. 한번 태그 후에는 제대로 세팅함.
////		// 이 시점에서 OwnerPlayer 의 MaxHealth 는 아직 정식 세팅이 안되어 있을 수 있음 (InitializeCombatStats 가 안 불린 상태라면). 첫 태그 이전에도 정확한 값이 필요하다면 다른 방식으로 세팅 필요 (CombatStatEval::GetEquipAppliedLocalPCHealth 참고)
////		InactivePlayerStateCache.SetVariablesByRetiringPlayer(CurrentOwnerPlayer); // 실제로 여기선 RetiringPlayer 의 의미는 아니다.
////		InactivePlayerStateCache.LastHealth = InactivePlayerStateCache.MaxHealth;
////		InactivePlayerStateCache.CurrentHealth = InactivePlayerStateCache.MaxHealth;
////
////		// 단, ExtraLevel 로 넘어온 상태라면 HP 등을 좀 제대로 세팅 해야함. 현재 활성 캐릭터의 경우 ABladeIIPlayer::InitializeCombatStats 에서
////		if (AB2StageManager::GetCacheStageKeepEssentialData().IsPlayInExtraLevel())
////		{
////			AB2StageManager::GetCacheStageKeepEssentialData().SetPostExtraMapLoadSkillRTManager(CurrentOwnerPlayer, &InactivePlayerStateCache);
////			InactivePlayerStateCache.bNotCachedYet = false; // 이 경우는 처음부터 caching 이 된 거니 bNotCachedYet 을 false 세팅해 줌.
////		}
////	}
////
////	if (MainPlayerClass == EPCClass::EPC_End)
////		MainPlayerClass = EPCClass::EPC_Gladiator;
////	if (SubPlayerClass == EPCClass::EPC_End)
////		SubPlayerClass = EPCClass::EPC_Assassin;
////
////	if (IsMyOwnerLocalPlayer())
////	{
////		// UIDoc 의 부활버프 값들이 남아있으니 처음에는 리셋. 로컬플레이어의 경우만.
////		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
////		if (DocBattle)
////		{
////			DocBattle->SetSelectedResurrectBuffAttack(0);
////			DocBattle->SetSelectedResurrectBuffDefense(0);
////			DocBattle->SetSelectedResurrectBuffHealth(0);
////
////			DocBattle->SetCurResurrectBuffAttackCountByIndex(UB2UIDocBattle::MySlotIndex, 0);
////			DocBattle->SetCurResurrectBuffDefenseCountByIndex(UB2UIDocBattle::MySlotIndex, 0);
////			DocBattle->SetCurResurrectBuffHealthCountByIndex(UB2UIDocBattle::MySlotIndex, 0);
////
////			// 결속 스킬 값들 리셋
////			DocBattle->SetOffenseEtherCoolTime(0.0f);
////			DocBattle->SetDefenseEtherCoolTime(0.0f);
////
////			for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
////			{
////				auto* DocHero = UB2UIDocHelper::GetDocHero(i);
////				DocHero->SetCurOffenseEtherCoolTime(0.0f);
////				DocHero->SetCurDefenseEtherCoolTime(0.0f);
////			}
////		}
////	}
////
////	UpdateSkillDocDataOnPCSwap();
////}
////
////bool UB2SkillRTManager::IsMyOwnerLocalPlayer()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsMyOwnerLocalPlayer);
////	//return (CurrentOwnerPlayer && CurrentOwnerPlayer == Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(CurrentOwnerPlayer)));
////	return true;
////}
////
////#include "BladeIINetPlayer.h"
////void UB2SkillRTManager::SetPlayerCharacterSkill(class ABladeIIPlayer* PlayerCharacter, TArray<FInputBoundSkillState_Base*>& OutIBSkillStates, FCachedSkillOption& OutCachedSkillOption)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_SetPlayerCharacterSkill);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::SetPlayerCharacterSkill"));
////
////	BII_CHECK(PlayerCharacter);
////
////	//Char Skill Setting
////	UB2SkillInfo* SkillInfo = GetSkillInfoOfPlayer(PlayerCharacter);
////	check(SkillInfo);
////
////	const auto PlayerClass = PlayerCharacter->GetCurrentPlayerClass();
////	const bool IsLocalPlayer = CurrentOwnerPlayer && CurrentOwnerPlayer->IsLocalPlayer();
////
////	TArray<int32> RegisteredSkills;
////	GetCharacterRegisteredSkillIDs(PlayerClass, GetB2GameModeType(PlayerCharacter), RegisteredSkills, PlayerCharacter->GetCharacterDataStore());
////	//[@AKI, 170815] Player의 Skill을 구성하는 부분. 결속 스킬에서 변경 되는 것이 있다면 이쪽을 봐야 함
////	for (int32 SkillInputIdx = 0; SkillInputIdx < RegisteredSkills.Num(); ++SkillInputIdx)
////	{
////		auto* SkillBaseObj = OutIBSkillStates[SkillInputIdx];
////
////		const int32 RegisteredSkillID = RegisteredSkills[SkillInputIdx];
////		const bool IsWeaponSkill = (SkillInputIdx == WEAPON_6STAR_SKILL_INDEX) && RegisteredSkillID != 0;
////
////		SkillBaseObj->InitSkillState(
////			this,
////			RegisteredSkillID,
////			PlayerClass,
////			SkillInfo->GetAttackState(RegisteredSkillID),
////			SkillInfo->GetOriginalCoolTime(RegisteredSkillID),
////			SkillInfo->GetEnableWeaponSkillPoint(RegisteredSkillID),
////			IsWeaponSkill);
////	}
////
////	if(auto* TagSkillInfo = SkillInfo->GetTagSkillInfo(PlayerClass))
////	{
////		if (OutIBSkillStates.IsValidIndex(TAG_SKILL_INDEX))
////		{
////			if (auto* TagSkillObj = OutIBSkillStates[TAG_SKILL_INDEX])
////			{
////				const int32 TagSkillId = TagSkillInfo->SkillID;
////				TagSkillObj->InitSkillState(this, TagSkillId, PlayerClass, SkillInfo->GetAttackState(TagSkillId), SkillInfo->GetOriginalCoolTime(TagSkillId), 0);
////
////				if (IsLocalPlayer)
////				{
////					auto* DocBattle = UB2UIDocHelper::GetDocBattle();
////					TagSkillObj->SetDoc(DocBattle); // 태그랑 기타 스킬의 UI Doc 은 따로 쓴다.	
////				}
////			}
////		}
////	}	
////
////	if(auto* CounterSkill = SkillInfo->GetCounterSkillInfo(PlayerClass))
////	{
////		const int32 CounterSkillId = CounterSkill->SkillID;
////		if (auto* CounterSkillObj = OutIBSkillStates[COUNTER_SKILL_INDEX])
////			CounterSkillObj->InitSkillState(this, CounterSkillId, PlayerClass, EAttackState::ECS_Counter01, 0.f, 0);
////	}
////
////	SetPlayerCharacterSkill_ExtraMode(PlayerCharacter, OutIBSkillStates);
////	OutCachedSkillOption.CacheSkillOption(this, PlayerClass, OutIBSkillStates);
////}
////
////void UB2SkillRTManager::SetPlayerCharacterSkill_ExtraMode(class ABladeIIPlayer* PlayerCharacter, TArray<FInputBoundSkillState_Base*>& OutIBSkillStates)
////{
////	const auto PlayerClass = PlayerCharacter->GetCurrentPlayerClass();
////
////	// 1. Tutorial Mode 처리
////	const bool IsTutorialMode = GetB2GameModeType(PlayerCharacter) == EB2GameMode::Tutorial;
////	const bool IsTutorialTargetClass = (PlayerClass == EPCClass::EPC_Wizard || PlayerClass == EPCClass::EPC_Fighter);
////	const bool IsTutorialSepcific = IsTutorialMode && IsTutorialTargetClass;
////
////	// 튜토리얼에서 다른 스킬모션 (마법사 반격, 격투가 스킬)
////	if (IsTutorialSepcific)
////	{
////		for (int32 SkillInputIdx = 0; SkillInputIdx < MAX_ACTIVE_SKILL_INPUT; SkillInputIdx++)
////		{
////			if (OutIBSkillStates.IsValidIndex(SkillInputIdx))
////				if (auto* SkillBaseObj = OutIBSkillStates[SkillInputIdx])
////					SkillBaseObj->SetIncreaseMotion(TUTORIAL_INCREASE_MOTION);
////		}
////	}
////
////	// 튜토리얼 마법사 반격 모션은 전부 다 나가게 최대로
////	if (IsTutorialMode)
////	{
////		if (auto* CounterSkillObj = OutIBSkillStates[COUNTER_SKILL_INDEX])
////			CounterSkillObj->IncreaseMotion = TUTORIAL_INCREASE_MOTION;
////	}
////
////	// 2. 반격던전은 첫번째 모션만 나오게 고정
////	else if (GetB2GameModeType(PlayerCharacter) == EB2GameMode::CounterDungeon)
////	{
////		if (auto* CounterSkillObj = OutIBSkillStates[COUNTER_SKILL_INDEX])
////			CounterSkillObj->IncreaseMotion = 0;
////	}
////}
////
////void UB2SkillRTManager::BindSkillDoc(class ABladeIIPlayer* PlayerCharacter, TArray<FInputBoundSkillState_Base*>& SkillStates)
////{
////	const auto PlayerClass = PlayerCharacter->GetCurrentPlayerClass();
////	for (int32 SkillInputIdx = 0; SkillInputIdx < MAX_ACTIVE_SKILL_INPUT; SkillInputIdx++)
////	{
////		if (auto* SkillBaseObj = SkillStates[SkillInputIdx])
////		{
////			const int32 RegisteredSkillID = SkillBaseObj->SkillID;
////
////			auto* SkillDoc = UB2UIDocHelper::GetDocSkill(static_cast<int32>(PlayerClass), RegisteredSkillID);
////			if (SkillDoc != nullptr)
////				SkillDoc->SetIsExistingSkill(DoesSkillExist(SkillInputIdx));
////
////			SkillBaseObj->SetDoc(SkillDoc);
////		}
////	}
////}
////
////bool UB2SkillRTManager::AllocateSkillState(TArray<FInputBoundSkillState_Base*>& OutSkillStates)
////{
////	if (OutSkillStates.Num() > 0)
////		return false;
////
////	for (int32 SkillInputIdx = 0; SkillInputIdx < MAX_SKILL_INPUT; ++SkillInputIdx)
////	{
////		FInputBoundSkillState_Base* SkillState = nullptr;
////
////		if (SkillInputIdx == TAG_SKILL_INDEX)
////		{
////			SkillState = new FInputBoundSkillState_Tag;
////		}
////		else if (SkillInputIdx == WEAPON_6STAR_SKILL_INDEX && GetB2GameModeType(nullptr) == EB2GameMode::Tutorial)
////		{
////			SkillState = new FInputBoundSkillState_IgnorePoint;
////		}
////		else
////		{
////			SkillState = new FInputBoundSkillState_Skill;
////		}
////
////		check(SkillState);
////		OutSkillStates.Add(SkillState);
////	}
////
////	return true;
////}
////
////bool UB2SkillRTManager::NeedsRebindSkillDoc(class ABladeIIPlayer* PlayerCharacter)
////{
////	//const auto PlayerClass = PlayerCharacter->GetCurrentPlayerClass();
////	//const bool IsLocalPlayer = CurrentOwnerPlayer && CurrentOwnerPlayer->IsLocalPlayer();
////
////	//const bool IsLocalAIMode = GetB2GameModeType(PlayerCharacter) == EB2GameMode::PVP_Team || GetB2GameModeType(PlayerCharacter) == EB2GameMode::Guild;
////	//const bool IsRemoteSpecific = !IsLocalPlayer && IsLocalAIMode;
////	//auto* TeamAIPlayer = Cast<ABladeIITMAIPlayer>(PlayerCharacter);
////	//const bool IsSameTeamAI = IsRemoteSpecific && TeamAIPlayer && TeamAIPlayer->GetIsLocalPlayerTeam();
////
////	//const bool NeedsRegisterSkillDoc = IsLocalPlayer || IsSameTeamAI;
////
////	//return NeedsRegisterSkillDoc;
////	return true;
////}
////
////UB2SkillInfo* UB2SkillRTManager::GetSkillInfoOfPlayer(ABladeIIPlayer* InPlayerChar)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetSkillInfoOfPlayer);
////	// SkillInfo 가 초기 구현에서는 플레이어 캐릭터 별로 따로라 CurrentOwnerPlayer 에서 가져왔으나 현재는 전체 SkillInfoObject 로 구성됨.
////	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(InPlayerChar));
////	UB2PCClassInfoBox* PCInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : NULL;
////	return PCInfoBox ? PCInfoBox->GetAllSkillInfo() : NULL;
////}
////
////void UB2SkillRTManager::UpdateSkillDocDataOnPCSwap()
////{ // 초기화, 혹은 태그 시점에 플레이어 바뀜에 따라 CachedSkillInfo 랑 IBSkillState 스위칭 된 이후.
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_UpdateSkillDocDataOnPCSwap);
////	for (auto* SkillState : IBSkillStates)
////	{
////		SkillState->UpdateUIDoc(this);
////	}
////}
////
////EAttackState UB2SkillRTManager::GetSkillOfInputIndex(int32 SkillInputIndex) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetSkillOfInputIndex);
////	BII_CHECK(CachedSkillInfo);
////
////	if (!IBSkillStates.IsValidIndex(SkillInputIndex))
////		return EAttackState::ECS_None;
////
////	// It will also return ECS_None if nothing found
////	return IBSkillStates[SkillInputIndex]->GetAttackState();
////}
////
////ESkillAnimType UB2SkillRTManager::GetSkillAnimIndex(int32 SkillInputIndex) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetSkillAnimIndex);
////	BII_CHECK(CachedSkillInfo);
////
////	if (!IBSkillStates.IsValidIndex(SkillInputIndex))
////		return ESkillAnimType::ESA_End;
////
////	FInputBoundSkillState_Base* BoundIBSkillState = IBSkillStates[SkillInputIndex];
////	if (BoundIBSkillState)
////	{
////		return GetSkillAnimIndexInternal(BoundIBSkillState->SkillID, BoundIBSkillState->IncreaseMotion, CachedSkillInfo);
////	}
////	return ESkillAnimType::ESA_End;
////}
////
////ESkillAnimType UB2SkillRTManager::GetSkillAnimIndexInternal(int32 InSkillID, int32 InIncMotionValue, UB2SkillInfo* InSkillInfo)
////{
////	if (!InSkillInfo) {
////		return ESkillAnimType::ESA_End;
////	}
////
////	EAttackState MappedSkillAttackState = InSkillInfo->GetAttackState(InSkillID);
////
////	// IncMotion 이 적용되는 스킬이 있고 아닌 스킬이 있다.
////	switch (MappedSkillAttackState)
////	{
////	case EAttackState::ECS_Skill01:
////		return (ESkillAnimType)(InIncMotionValue + (int32)ESkillAnimType::ESA_Skill01_01);
////	case EAttackState::ECS_Skill02:
////	{
////		if (InIncMotionValue != 0) // 모션 딱 하나 있는 스킬
////		{
////			UE_LOG(LogB2SkillRTManager, Warning, TEXT("Invalid Skill Increase Motion Option data"));
////		}
////		return (ESkillAnimType::ESA_Skill02_01);
////	}
////	case EAttackState::ECS_Skill03:
////		return (ESkillAnimType)(InIncMotionValue  + (int32)ESkillAnimType::ESA_Skill03_01);
////	case EAttackState::ECS_Skill04:
////	{
////		if (InIncMotionValue != 0)
////		{
////			UE_LOG(LogB2SkillRTManager, Warning, TEXT("Invalid Skill Increase Motion Option data"));
////		}
////		return (ESkillAnimType::ESA_Skill04_01);
////	}
////	case EAttackState::ECS_Skill05:
////		return (ESkillAnimType)(InIncMotionValue + (int32)ESkillAnimType::ESA_Skill05_01);
////	case EAttackState::ECS_Skill06:
////	{
////		if (InIncMotionValue != 0)
////		{
////			UE_LOG(LogB2SkillRTManager, Warning, TEXT("Invalid Skill Increase Motion Option data"));
////		}
////		return (ESkillAnimType::ESA_Skill06_01);
////	}
////	case EAttackState::ECS_WeaponSkill:
////	{
////		BII_CHECK(InSkillInfo);
////		if (InSkillInfo)
////		{
////			const FSingleSkillInfo* SingleSkillInfo = InSkillInfo->GetSingleInfoOfID(InSkillID);
////			BII_CHECK(SingleSkillInfo);
////			if (SingleSkillInfo)
////			{
////				BII_CHECK(SingleSkillInfo->WeaponSkillType != EWeaponSkillType::EWS_None && SingleSkillInfo->WeaponSkillType != EWeaponSkillType::EWS_End);
////				return (ESkillAnimType)((int32)ESkillAnimType::ESA_Weapon_01 + (int32)SingleSkillInfo->WeaponSkillType - (int32)EWeaponSkillType::EWS_Weapon01);
////			}
////		}
////		break;
////	}
////	}
////
////	return ESkillAnimType::ESA_End;
////}
////
////int32 UB2SkillRTManager::GetCounterMotionInc() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCounterMotionInc);
////	check(IBSkillStates[COUNTER_SKILL_INDEX]);
////	return IBSkillStates[COUNTER_SKILL_INDEX]->IncreaseMotion;
////}
////
////int32 UB2SkillRTManager::GetSkillID(EAttackState SkillAttackState)
////{
////	for (auto* SkillInfo : IBSkillStates)
////	{
////		if (SkillInfo && SkillInfo->GetAttackState() == SkillAttackState)
////			return SkillInfo->SkillID;
////	}
////
////	return INDEX_NONE;
////}
////
////#if !UE_BUILD_SHIPPING
////void UB2SkillRTManager::ChangeSkillInfo(int32 SkillIndexToChange, int32 SkillId)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_ChangeSkillInfo);
////	BII_CHECK(CachedSkillInfo);
////	BII_CHECK(CurrentOwnerPlayer);
////
////	IBSkillStates[SkillIndexToChange]->InitSkillState(
////		this, 
////		SkillId, 
////		CurrentOwnerPlayer->GetCurrentPlayerClass(), 
////		CachedSkillInfo->GetAttackState(SkillId), 
////		CachedSkillInfo->GetOriginalCoolTime(SkillId), 
////		CachedSkillInfo->GetEnableWeaponSkillPoint(SkillId),
////		SkillIndexToChange == WEAPON_6STAR_SKILL_INDEX
////		);
////	auto* SkillDoc = UB2UIDocHelper::GetDocSkill((int32)CurrentOwnerPlayer->GetCurrentPlayerClass(), IBSkillStates[SkillIndexToChange]->SkillID);
////	if (CurrentOwnerPlayer && CurrentOwnerPlayer->IsPlayerControlled())
////		IBSkillStates[SkillIndexToChange]->SetDoc(SkillDoc);
////}
////#endif //!UE_BUILD_SHIPPING
////
////EAttackState UB2SkillRTManager::GetStartableSkill(int32 SkillInputIndex, bool IsPlayerInput)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_StartSkillByInputIndex);
////	if (SkillInputIndex != TAG_SKILL_INDEX && IBSkillStates.IsValidIndex(SkillInputIndex))
////	{
////		if (IsSkillCurrentlyUsable(SkillInputIndex, IsPlayerInput) == false)
////			return EAttackState::ECS_None;
////
////		return GetSkillOfInputIndex(SkillInputIndex);
////	}
////
////	return EAttackState::ECS_None;
////}
////
////const float UB2SkillRTManager::GetSkillShootingRangeForAI(class ABladeIIPlayer* _PlayerCharacter, int32 _SkillInputIndex)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetSkillShootingRangeForAI);
////	float distance = 0.0f;
////	//Char Skill Setting
////	UB2SkillInfo* SkillInfo(GetSkillInfoOfPlayer(_PlayerCharacter));
////	check(SkillInfo);
////
////	auto SkillState = IBSkillStates[_SkillInputIndex];
////
////	distance = SkillInfo->GetOriginalSkillShootingRangeForAI(SkillState->SkillID);
////
////	return distance;
////}
////
////void UB2SkillRTManager::ResetSkillCoolTime()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_SetSkillCoolTimeTimer);
////	BII_CHECK(CurrentOwnerPlayer && CachedSkillInfo);
////
////	for (int32 SI = 0; SI < IBSkillStates.Num(); ++SI)
////	{
////		auto* SkillState = IBSkillStates[SI];
////		SkillState->ResetSkillState(this);
////	}
////}
////
////void UB2SkillRTManager::SetSkillCoolTimeTimer(EAttackState FinishedAttackState)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_SetSkillCoolTimeTimer);
////	BII_CHECK(CurrentOwnerPlayer && CachedSkillInfo);
////	
////	for (int32 SI = 0; SI < IBSkillStates.Num(); ++SI)
////	{
////		auto* SkillState = IBSkillStates[SI];
////
////		// Check which skill input user bound that skill ID.
////		if (FinishedAttackState == SkillState->GetAttackState())
////		{
////			const float CoolTime = SkillState->GetMaxCooltime(this);
////			
////			//BII_CHECK(!SkillState->IsCountingCooltime());// 네트워크 대전에서 다른 경로를 통해 카운트 시작했다가 다시 들어오는 경우가 있는 거 같다. 경고 로그로 대체
////			if (SkillState->IsCountingCooltime())
////			{
////				UE_LOG(LogBladeII, Warning, TEXT("Begin Count SkillCoolTime again.. %.1fs, while already counted %.1fs, SkillIndex %d, AttackState %d"), CoolTime, SkillState->GetRemainingCooltime(), SI, (int32)FinishedAttackState);
////			}
////
////			if (CoolTime <= 0.0f)
////			{
////				SkillState->ResetSkillState(this); // Cooltime 은 미리 얻어놓고 굳이 여기까지 와서 Reset 하고 가는 게 좀 이상하긴 하지만 여튼.
////
////				break;
////			}
////
////			SkillState->SetRemainingCooltime(CoolTime); // Reset the remaining time too.
////			SkillState->SetSkillDisabledByCooltime(true); // 일반 스킬이라면 이미 세팅되어 있어야 함.
////			SkillState->SetIsCountingCooltime(true);
////
////			break;
////		}
////	}
////}
////
////
////void UB2SkillRTManager::OnEnterPlayerSkillState(EAttackState ReservedSkill)
////{
////	for (int32 SI = 0; SI < IBSkillStates.Num(); ++SI)
////	{
////		auto* SkillState = IBSkillStates[SI];
////
////		// Check which skill input user bound that skill ID.
////		if (ReservedSkill == SkillState->GetAttackState())
////		{
////
////			SkillState->SetSkillDisabledByCooltime(true); // True until cooltime timer is set and completed.
////			SkillState->SetSkillPoint(GetActiveCharacterWeaponSkillPoint());
////			// CountRemainingCoolTime 뿐만이 아니라 스킬 발동 시점에도 업데이트를 해 주어야 카운트 대기 상태에서도 원하는 값을 뿌려줄 것.
////			SkillState->SetRemainingCooltime(SkillState->GetMaxCooltime(this));
////		}
////	}
////}
////
////void UB2SkillRTManager::CountRemainingCoolTime(float DeltaSeconds)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_CountRemainingCoolTime);
////	const float NonCustomDelatedDeltaSec = GetDeltaTimeAsOtherWorldObj(DeltaSeconds, CurrentOwnerPlayer);
////
////	for (auto* SkillState : IBSkillStates)
////	{
////		// 만일 여기서 CachedSkillInfo 가 필요한 상황이라면 InactivePlayer 용에 대해 똑같이 처리하면 안 된다.
////		// The skill must not be usable when it needs to count. Timer should be set also.
////		if (!SkillState->IsCountingCooltime())
////		{
////
////			if (CurrentOwnerPlayer->GetAttackState() != SkillState->GetAttackState() && SkillState->GetRemainingCooltime() != 0.f)
////			{
////				//여기에 걸리면 Player SkillState가 꼬여있는 상태
////				//기본적으로 ABladeIIPlayer의 SetSkillState를 통해 임의적으로 State를 바꾸려할 경우 자주 일어나는 것으로 보임.
////				SkillState->SetIsCountingCooltime(true); //SkillState가 꼬였는 경우 강제로 풀어준다.
////			}
////				
////			continue;
////		}			
////
////		// Need to check CustomTimeDilation. 만일 TimeDilation 처리로 인해 뭔가 잘 안 맞으면 언리얼 timer 라도 돌려보든지. 그렇지만 스킬쿨타임은 이렇게 직접 카운트하는 게 여러모로 컨트롤하기 좋을 듯.
////		float RemainingCooltime = SkillState->GetRemainingCooltime() - NonCustomDelatedDeltaSec;
////		SkillState->SetRemainingCooltime(FMath::Max(RemainingCooltime, 0.0f));
////
////		if (SkillState->GetRemainingCooltime() == 0.f)
////			SkillState->ResetSkillState(this);
////	}
////
////	for (auto* SkillState : InactivePlayerStateCache.IBSkillStates)
////	{
////		if (!SkillState->IsCountingCooltime())
////			continue;
////
////		if (SkillState->GetMappingAttackState() == EAttackState::ECS_TagOtherChar)
////			continue;
////		
////		float RemainingCooltime = SkillState->GetRemainingCooltime() - NonCustomDelatedDeltaSec;
////		SkillState->SetRemainingCooltime(FMath::Max(RemainingCooltime, 0.0f));
////
////		if (SkillState->GetRemainingCooltime() == 0.f)
////			SkillState->ResetSkillState(this);
////	}
////}
////
////void UB2SkillRTManager::RealtimeUpdateUIDoc()
////{ // 이건 매 틱 불리는 것이므로 가능한 최소한의 데이터만 여길 통해 업데이트 해야 함.
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_RealtimeUpdateUIDoc);
////	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
////	if (DocBattle)
////	{ // 태그 캐릭터 클래스 등은 BladeIIPlayer 쪽서 업데이트
////		DocBattle->SetTagPCHealth(InactivePlayerStateCache.CurrentHealth);
////		DocBattle->SetTagPCMaxHealth(InactivePlayerStateCache.MaxHealth);
////
////		// 얘네들을 이벤트 때만 업데이트 하려면 좀 뼈빠질 듯 
////		DocBattle->SetIsTagPossible(IsTagPossible());
////		DocBattle->SetIsTagAttackSuggested(IsTagAttackSuggested()); // TagAttackSuggested 를 TagSuggested 보다 먼저 세팅해서 TagSuggested 변경 픽업시 TagAttackSuggested 도 같이 참조할 수 있도록.
////		DocBattle->SetIsTagSuggested(IsTagAttackSuggested() || IsTagSuggestedPlayerCondition());
////	}
////}
////
////float UB2SkillRTManager::GetRemainingCoolTimeRatio(int32 SkillInputIndex) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetRemainingCoolTimeRatio);
////	if (!CachedSkillInfo || !(SkillInputIndex >= 0 && SkillInputIndex < MAX_SKILL_INPUT))
////		return 0.0f;
////
////	if (IBSkillStates[SkillInputIndex]->IsSkillDisabledByCooltime() == false)
////	{
////		// If it is currently usable, we better just returns 0.0. I guess RemainingCooltime in that case would be 0, but just for the case.
////		return 0.0f;
////	}
////
////	const float CoolTime = IBSkillStates[SkillInputIndex]->GetMaxCooltime(this);
////
////	if (CoolTime > 0.0f)
////	{
////		return (IBSkillStates[SkillInputIndex]->GetRemainingCooltime() / CoolTime);
////	}
////
////	return 0.0f;
////}
////
////float UB2SkillRTManager::GetRemainingCoolTime(int32 SkillInputIndex) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetRemainingCoolTime);
////	if (!CachedSkillInfo || !(SkillInputIndex >= 0 && SkillInputIndex < MAX_SKILL_INPUT))
////		return 0.0f; 
////
////	if (IBSkillStates[SkillInputIndex]->IsSkillDisabledByCooltime() == false)
////	{
////		return 0.0f;
////	}
////	else if (IBSkillStates[SkillInputIndex]->IsCountingCooltime() == false)
////	{
////		// If waiting for count, return full cool time.
////		return IBSkillStates[SkillInputIndex]->GetMaxCooltime(this);
////	}
////
////	return IBSkillStates[SkillInputIndex]->GetRemainingCooltime();
////}
////
////int32 UB2SkillRTManager::GetEnablePlayerLevel(int32 SkillInputIndex)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetEnablePlayerLevel);
////	if (!CachedSkillInfo || !DoesSkillExist(SkillInputIndex))
////		return 0;
////
////	// CachedSkillInfo and SkillInputIndex are ensured to be fine by DoesSkillExist
////	return CachedSkillInfo->GetEnablePlayerLevel(IBSkillStates[SkillInputIndex]->SkillID);
////}
////
////bool UB2SkillRTManager::DoesSkillExist(int32 SkillInputIndex) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_DoesSkillExist);
////	EAttackState BoundAttackState = GetSkillOfInputIndex(SkillInputIndex);
////	return BoundAttackState != EAttackState::ECS_None; // Check for ECS_None probably enough.
////}
////
////
////bool UB2SkillRTManager::IsSkillCurrentlyUsable(int32 SkillInputIndex, bool IsPlayerInput) const
////{
////	//bool IsSkill = (AutoBattle & AutoBattleType::Skill) && AutoBattleType::Skill;
////	//침묵
////	if (CurrentOwnerPlayer && CurrentOwnerPlayer->IsAlive() && CurrentOwnerPlayer->IsBuffActive(EBuffType::DeBuff_Silent))
////		return false;
////	
////	bool IsSkill = false;
////
////	if ((AutoBattle & AutoBattleType::Skill) > 0 ||
////		((AutoBattle & AutoBattleType::Attack) > 0 && TAG_SKILL_INDEX == SkillInputIndex))
////		//((AutoBattle & AutoBattleType::NotTag) > 0) && TAG_SKILL_INDEX != SkillInputIndex)
////	{
////		IsSkill = true;
////	}
////
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsSkillCurrentlyUsable);
////	return (
////		(IsSkill || IsPlayerInput)
////		&& DoesSkillExist(SkillInputIndex)
////		&& IBSkillStates[SkillInputIndex]->IsSkillDisabledByCooltime() == false // SkillInputIndex is ensured to be fine by DoesSkillExist
////		&& IBSkillStates[SkillInputIndex]->IsSkillDisabledByPoint() == false
////		&& IBSkillStates[SkillInputIndex]->GetAttackState() != EAttackState::ECS_Counter01
////		);
////}
////
////bool UB2SkillRTManager::IsSkillCurrentlyCountingCooltime(int32 SkillInputIndex) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsSkillCurrentlyCountingCooltime);
////	return (DoesSkillExist(SkillInputIndex) &&
////		// SkillInputIndex is ensured to be fine by DoesSkillExist
////		IBSkillStates[SkillInputIndex]->IsCountingCooltime() == true);
////}
////
////bool UB2SkillRTManager::IsSkillDisabledByCooltime(int32 SkillInputIndex) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsSkillDisabledByCooltime);
////	return (DoesSkillExist(SkillInputIndex) &&
////		// SkillInputIndex is ensured to be fine by DoesSkillExist
////		IBSkillStates[SkillInputIndex]->IsSkillDisabledByCooltime() == true);
////}
////
////bool UB2SkillRTManager::GetCurrentlyAvailableSkillAttackState(TArray<EAttackState>& OutSkillAttackState, bool bIncludeTag)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCurrentlyAvailableSkillAttackState);
////	bool bFoundOne = false;
////	if (!CachedSkillInfo)
////		return bFoundOne;
////
////	for (int32 SI = 0; SI < IBSkillStates.Num(); ++SI)
////	{
////		if (!IsSkillCurrentlyUsable(SI, false))
////			continue;
////
////		EAttackState SkillAttackState = IBSkillStates[SI]->GetAttackState();
////		if (SkillAttackState == EAttackState::ECS_None ||
////			(bIncludeTag == false && SkillAttackState == EAttackState::ECS_TagOtherChar)
////			) // IsSkillCurrentlyUsable would be false if it was ECS_None anyway..
////		{
////			continue;
////		}
////		OutSkillAttackState.Add(SkillAttackState);
////		bFoundOne = true;
////	}
////
////	return bFoundOne;
////}
////
////void UB2SkillRTManager::ForceSetSkillCoolTimerWhenAIPlayerTagging(int32 TagAttackType, EAttackState CurrentAttackState)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_ForceSetSkillCoolTimerWhenAIPlayerTagging);
////	EPCClass NewSpawnClass = GetTagPlayerClass();
////	if (NewSpawnClass == EPCClass::EPC_End)
////		return;
////	
////	if (CurrentAttackState != EAttackState::ECS_None)
////	{
////		SetSkillCoolTimeTimer(CurrentAttackState);
////	}
////
////	check(CachedSkillInfo);
////	for (int32 SI = 0; SI < MAX_ACTIVE_SKILL_INPUT; ++SI)
////	{
////		if (IBSkillStates[SI]->IsSkillDisabledByCooltime() && !IBSkillStates[SI]->IsCountingCooltime())
////		{
////			SetSkillCoolTimeTimer(IBSkillStates[SI]->GetAttackState());
////		}
////	}
////}
////
////bool UB2SkillRTManager::TagToOther(int32 TagAttackType, EAttackState CurrentAttackState /*= EAttackState::ECS_None*/)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_TagToOther);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::TagToOther"));
////	// 태그 입력시 ABladeIIPlayer::StartTag 에서 시작하여 TagNotify 를 거쳐 여기서 본격 동작
////
////	if (CurrentOwnerPlayer == NULL /*|| CurrentOwnerPlayer->IsAlive() == false*/) // 죽은 시점에 Tag 가 필요할 수도 있다.
////	{
////		return false;
////	}
////
////	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(CurrentOwnerPlayer));
////	if (!pGM->IsAllowTag())
////		return false;
////
////	EPCClass NewSpawnClass = GetTagPlayerClass();
////	if (NewSpawnClass != EPCClass::EPC_End)
////	{
////		// Spawn 전에 player controller 쪽에 SpawnMotionState 를 먼저 세팅한다. 
////		// Player character spawn 도중 AnimBP overriding 에 의해 Anim tick 이 한번 처리되는데 그 전에 상태를 넣어둘 필요가 있어서 Player controller 를 통하도록 함.
////		ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(CurrentOwnerPlayer->Controller);
////		if (B2PC)
////		{
////			B2PC->SetPCSpawnMotionState((EPCSpawnMotionState)(TagAttackType + int32(EPCSpawnMotionState::EPST_Tag)));
////		}
////
////		if (CurrentAttackState != EAttackState::ECS_None) // TagAttack 시 현재 사용하고 있는 skill state 가 들어올 것. 다른 경로로 noti 가 오지 않을 것이므로 쿨타임 카운트를 시작.
////		{
////			SetSkillCoolTimeTimer(CurrentAttackState);
////		}
////
////		// 태그 쿨타임 뿐 아니라 스킬 사용 도중 태그 예약으로 인해 스킬 쿨타임 카운트 시작이 안된 채로 진행이 될 수 있어서 혹시라도 그런 게 있다면 바꿔치기 전에 시작해 준다.
////		check(CachedSkillInfo);
////		for (int32 SI = 0; SI < MAX_ACTIVE_SKILL_INPUT; ++SI)
////		{
////			if (IBSkillStates[SI]->IsSkillDisabledByCooltime() && !IBSkillStates[SI]->IsCountingCooltime())
////			{
////				SetSkillCoolTimeTimer(IBSkillStates[SI]->GetAttackState());
////			}
////		}
////		
////		ABladeIIPlayer* OldPlayerBackup = CurrentOwnerPlayer;
////
////		// 로컬플레이어가 바뀌기전에 버프클리어한번 해준다.
////		if (OldPlayerBackup)
////		{
////			OldPlayerBackup->ClearAllBuffs();
////		}
////
////		if (DynSpawnNewPlayerCommon(NewSpawnClass) != NULL)
////		{
////			// 위에서 성공적이었으면 CurrentOwnerPlayer 가 새로 생성한 것으로 바뀌어 있을 것.
////			// 서로에게 각자의 tag 상황을 notify. Auto 인수인계 등 추가적인 뒤치닥꺼리
////			OldPlayerBackup->NotifyTagRetire(CurrentOwnerPlayer);
////			CurrentOwnerPlayer->NotifyTagSpawn(OldPlayerBackup);
////
////			OnPlayerChanged_Tag(OldPlayerBackup, CurrentOwnerPlayer);
////
////			if (IsTagPossibleAtDeadMoment()) // 혼자 남겨진 경우라면 태그 쿨타임을 더 카운트할 필요가 없다.
////			{
////				// 태그 Cooltime 셋업. 태그와 일반 스킬 간에 실질적으로 공유하는 기능.
////				// 여기서 처리하게 되면 바뀐 후 캐릭터 설정으로 태그 타이머가 돌아가게 될 것. 어차피 클래스 따라 달라질 것으로 생각되지는 않지만..
////				// 어차피 태그 시 일반 스킬 쿨타임 처리도 개선을 해야 할 것이므로 그 때 같이 생각해 볼 것.
////				SetSkillCoolTimeTimer(EAttackState::ECS_TagOtherChar);
////			}
////
////			// 바뀐 케릭터한테 어택스테이트 인계
////			CurrentOwnerPlayer->SetAttackState(CurrentAttackState);
////
////			CurrentOwnerPlayer->GetWorld()->DestroyActor(OldPlayerBackup);
////
////			return true;
////		}
////		else
////		{
////			// 실패했다면 뭔가 막장이지만 여튼 PlayerController 쪽에 세팅해 둔 상태를 취소하자.
////			if (B2PC)
////			{
////				B2PC->SetPCSpawnMotionState(EPCSpawnMotionState::EPST_Normal);
////			}
////		}
////	}
////
////	return false;
////}
////
////int32 UB2SkillRTManager::GetMaxResurrectionChance(EResurrectGameModeType ModeType)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetMaxResurrectionChance);
////	return AB2StageManager::GetCacheStageKeepEssentialData().GetMaxResurrectionChance(ModeType);
////}
////
////int32 UB2SkillRTManager::GetRemainingResurrectionChance(EResurrectGameModeType ModeType)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetRemainingResurrectionChance);
////	return AB2StageManager::GetCacheStageKeepEssentialData().GetRemainingResurrectionChance(ModeType);
////}
////
////int32 UB2SkillRTManager::GetResurrectionCost(EResurrectGameModeType ModeType)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetResurrectionCost);
////	return AB2StageManager::GetCacheStageKeepEssentialData().GetResurrectionCost(ModeType);
////}
////
////void UB2SkillRTManager::GetAppliedResurrectionBuffValues(TMap<EResurrectBuffType, int32>& OutBuffValues)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetAppliedResurrectionBuffValues);
////	// 서버에서는 지금까지 여러번의 부활을 통해 선택했던 부활 버프가 모두 올 껀데 여기서는 마지막에 선택한 타입만 사용하는 걸로..
////	EResurrectBuffType LastRequestedBuffType = EResurrectBuffType::RBT_End;
////	if (auto* GMMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(CurrentOwnerPlayer)))
////		LastRequestedBuffType = GMMode->GetLastRequestedStageBuffType();
////
////	const bool bLastRequestedSpecificBuffType = (LastRequestedBuffType != EResurrectBuffType::RBT_None && LastRequestedBuffType != EResurrectBuffType::RBT_End);
////	TMap<EResurrectBuffType, int32> FinalFilteredBuffMap;
////	for (auto ThisBuff : SelectedResurrectionBuffMap)
////	{ 
////		// 딱히 마지막 선택한 정보를 찾을 수 없다면 모두..
////		if (!bLastRequestedSpecificBuffType || ThisBuff.Key == LastRequestedBuffType)
////			FinalFilteredBuffMap.Add(ThisBuff.Key, ThisBuff.Value);
////	}
////	BladeIIGameImpl::GetClientDataStore().GetAppliedResurrectBuff(GetCurResurrectGameModeType(), FinalFilteredBuffMap, OutBuffValues, false);
////}
////
////EResurrectGameModeType UB2SkillRTManager::GetCurResurrectGameModeType()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCurResurrectGameModeType);
////	auto* BladeGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(CurrentOwnerPlayer));
////	if (BladeGameMode == nullptr)
////		return EResurrectGameModeType::EResurrectType_Max;
////
////	return BladeGameMode->GetResurrectGameModeType();
////}
////
////void UB2SkillRTManager::UpdateResurrectionBuffOnResponse(int32 AttackBuffCount, int32 DefenseBuffCount, int32 HealthBuffCount)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_UpdateResurrectionBuffOnResponse);
////	// 여기에서는 서버에서 준 값들 모두 넣어놓는다. 서버에선 지금까지 여러번의 부활에서 선택한 모든 타입이 보내질 것.
////	// 실제로 전투에 적용되는 것은 가장 최근 선택한 타입만이 적용되어야 함.
////	SelectedResurrectionBuffMap.Empty();
////	if (AttackBuffCount > 0){
////		SelectedResurrectionBuffMap.Add(EResurrectBuffType::RBT_Attack, AttackBuffCount);
////	}
////	if (DefenseBuffCount > 0){
////		SelectedResurrectionBuffMap.Add(EResurrectBuffType::RBT_Defense, DefenseBuffCount);
////	}
////	if (HealthBuffCount > 0){
////		SelectedResurrectionBuffMap.Add(EResurrectBuffType::RBT_Health, HealthBuffCount);
////	}
////		
////	// UIDocBattle 에 세팅하는 건 사실상 스테이지 모드의 로컬 플레이어 용이란 가정임. 		
////	TMap<EResurrectBuffType, int32> AppliedResurrectBuff;	// 인게임 전투 UI 에서 표시하는 실제 적용되는 값들
////	GetAppliedResurrectionBuffValues(AppliedResurrectBuff);
////
////	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
////	{
////		// 여기서 세팅한 것은 스테이지 모드의 패배 메뉴에서 쓴다.
////		DocBattle->SetSelectedResurrectBuffAttack(0);
////		DocBattle->SetSelectedResurrectBuffDefense(0);
////		DocBattle->SetSelectedResurrectBuffHealth(0);
////		for (auto ThisBuff : SelectedResurrectionBuffMap)
////		{
////			switch (ThisBuff.Key)
////			{
////			case EResurrectBuffType::RBT_Attack:	DocBattle->SetSelectedResurrectBuffAttack(ThisBuff.Value); break;
////			case EResurrectBuffType::RBT_Defense:	DocBattle->SetSelectedResurrectBuffDefense(ThisBuff.Value); break;
////			case EResurrectBuffType::RBT_Health:	DocBattle->SetSelectedResurrectBuffHealth(ThisBuff.Value); break;
////			}
////		}
////
////		DocBattle->SetCurResurrectBuffAttackCountByIndex(UB2UIDocBattle::MySlotIndex,  AppliedResurrectBuff.Contains(EResurrectBuffType::RBT_Attack) ?  AppliedResurrectBuff[EResurrectBuffType::RBT_Attack] : 0);
////		DocBattle->SetCurResurrectBuffDefenseCountByIndex(UB2UIDocBattle::MySlotIndex, AppliedResurrectBuff.Contains(EResurrectBuffType::RBT_Defense) ? AppliedResurrectBuff[EResurrectBuffType::RBT_Defense] : 0);
////		DocBattle->SetCurResurrectBuffHealthCountByIndex(UB2UIDocBattle::MySlotIndex,  AppliedResurrectBuff.Contains(EResurrectBuffType::RBT_Health) ?  AppliedResurrectBuff[EResurrectBuffType::RBT_Health] : 0);
////
////		DocBattle->SetNeedResurrectBuffText(true);
////	}
////}
////
////void UB2SkillRTManager::SetResurrectionPlayerPosition()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_SetResurrectionPlayerPosition);
////	auto* BladeGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(CurrentOwnerPlayer));
////	if (BladeGameMode == nullptr)
////		return;
////
////	BladeGameMode->SetResurrectPlayerPosition();
////}
////
////bool UB2SkillRTManager::ResurrectOnResponse(const FB2Resurrection& ResurrectionResult)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_ResurrectOnResponse);
////	//int32 SelectedBuffAmount_1 = GET_TUPLE_DATA(FB2ResponseResurrection::selected_count_resurrection_buff1_index, ResurrectionResult);
////	//int32 SelectedBuffAmount_2 = GET_TUPLE_DATA(FB2ResponseResurrection::selected_count_resurrection_buff2_index, ResurrectionResult);
////	//int32 SelectedBuffAmount_3 = GET_TUPLE_DATA(FB2ResponseResurrection::selected_count_resurrection_buff3_index, ResurrectionResult);
////	//int32 UsedGem = GET_TUPLE_DATA(FB2ResponseResurrection::used_cash_index, ResurrectionResult);
////	//
////	//return ResurrectPlayer(EResurrectGameModeType::EResurrectType_Stage, UsedGem, SelectedBuffAmount_1, SelectedBuffAmount_2, SelectedBuffAmount_3);
////	return true;
////}
////
////bool UB2SkillRTManager::RaidResurrectOnResponse(const FB2RaidResurrection& ResurrectionResult)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_RaidResurrectOnResponse);
////	//int32 SelectedBuffAmount_1 = GET_TUPLE_DATA(FB2ResponseRaidResurrection::selected_count_resurrection_buff1_index, ResurrectionResult);
////	//int32 SelectedBuffAmount_2 = GET_TUPLE_DATA(FB2ResponseRaidResurrection::selected_count_resurrection_buff2_index, ResurrectionResult);
////	//int32 SelectedBuffAmount_3 = GET_TUPLE_DATA(FB2ResponseRaidResurrection::selected_count_resurrection_buff3_index, ResurrectionResult);
////	//int32 UsedGem = GET_TUPLE_DATA(FB2ResponseRaidResurrection::used_gem_index, ResurrectionResult);
////
////	//return ResurrectPlayer(EResurrectGameModeType::EResurrectType_Raid, UsedGem, SelectedBuffAmount_1, SelectedBuffAmount_2, SelectedBuffAmount_3);
////	return true;
////}
////
////bool UB2SkillRTManager::ResurrectPlayer(EResurrectGameModeType ModeType, int32 UsedGem/*= -1*/, int32 AttackBuffCount/*= -1*/, int32 DefenseBuffCount/*= -1*/, int32 HealthBuffCount /*= -1*/)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_ResurrectPlayer);
////	// 로컬 플레이어의 경우 서버로 보낸 부활 요청에 대한 응답임. 기타 플레이어 캐릭터 타입이나 ID 에 따른 구분이 필요해 질 수도 있음.
////	if (!IsMyOwnerLocalPlayer()){
////		return false;
////	}
////
////	// 버프 선택한 것은 여기에 저장해 둔다. InitializeCombatStats 에서 적용되도록 !!! (주의 : BornAgain 이전에 호출)
////	if (AttackBuffCount != -1 && DefenseBuffCount != -1 && HealthBuffCount != -1)
////		UpdateResurrectionBuffOnResponse(AttackBuffCount, DefenseBuffCount, HealthBuffCount);
////
////	// 부활할 위치를 재조정해준다. (주의 : BornAgain 이전에 호출)
////	SetResurrectionPlayerPosition();
////
////	if (BornAgain(false))
////	{
////		AB2StageManager::GetCacheStageKeepEssentialData().DecreaseRemainingResurrectionChance(ModeType);
////
////		if (UsedGem != -1)
////		{
////			int32 TotalGem = BladeIIGameImpl::GetClientDataStore().GetGemAmount() - UsedGem;
////			if (TotalGem < 0) TotalGem = 0;
////
////			BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, TotalGem);
////		}
////	}
////	else
////	{
////		checkSlow(false);
////	}
////
////	StopDefeatMenuClass<>::GetInstance().Signal();
////	return true;
////}
////
////bool UB2SkillRTManager::BornAgain(bool bForceSpawnAsMain)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_BornAgain);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::BornAgain"));
////	//
////	//if (CurrentOwnerPlayer == NULL) // 이건 거의 resurrect 용을 의도한 것이므로 당연히 IsAlive 체크를 하지 않는다. 오히려 반대로 IsAlive false 인 걸 체크해야 할 수도.
////	//{
////	//	return false;
////	//}
////
////	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(CurrentOwnerPlayer));
////
////	//// 게임모드가 없을리는 없겠지.
////	//if (!pGM)
////	//	return false;
////
////	//EPCClass NewSpawnClass = bForceSpawnAsMain ? MainPlayerClass : GetCurrentPlayerClass();
////	//if (NewSpawnClass == EPCClass::EPC_End)
////	//{
////	//	return false;
////	//}
////	//
////	//// Spawn 전에 player Controller 쪽에 SpawnMotionState 를 먼저 세팅한다. 
////	//// Player character spawn 도중 AnimBP overriding 에 의해 Anim tick 이 한번 처리되는데 그 전에 상태를 넣어둘 필요가 있어서 Player controller 를 통하도록 함.
////	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(CurrentOwnerPlayer->Controller);
////	//if (B2PC)
////	//{
////	//	B2PC->SetPCSpawnMotionState(pGM->GetBornAgainSpawnMotionState());
////	//}
////
////	//ABladeIINetPlayer* pNetPlayer = Cast<ABladeIINetPlayer>(CurrentOwnerPlayer);
////	//if (pNetPlayer)
////	//	pNetPlayer->SetSpawnMotionState(pGM->GetBornAgainSpawnMotionState());
////
////	//ABladeIIPlayer* OldPlayerBackup = CurrentOwnerPlayer;
////
////	//if (DynSpawnNewPlayerCommon(NewSpawnClass) != NULL)
////	//{
////	//	// 위에서 성공적이었으면 CurrentOwnerPlayer 가 새로 생성한 것으로 바뀌어 있을 것.
////	//	CurrentOwnerPlayer->NotifyBornAgain(OldPlayerBackup);
////	//	OnPlayerchanged_BornAgain(); // 이쪽 처리도
////	//	
////	//	OldPlayerBackup->Role = ROLE_Authority;
////	//	CurrentOwnerPlayer->GetWorld()->DestroyActor(OldPlayerBackup);
////	//	return true;
////	//}
////	//else
////	//{
////	//	// 실패했다면 뭔가 막장이지만 여튼 PlayerController 쪽에 세팅해 둔 상태를 취소하자.
////	//	if (B2PC)
////	//	{
////	//		B2PC->SetPCSpawnMotionState(EPCSpawnMotionState::EPST_Normal);
////	//	}
////	//}
////	//
////
////	return false;
////}
////
////bool UB2SkillRTManager::SimpleRebirth(bool bKeepCurrentClass/* = false*/)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_SimpleRebirth);
////	// 단순히 현재 플레이어 캐릭으로 재시작하는거. 상태 리셋 차원인데 특수한 용도로 사용할 일이 있다.
////	if (!CurrentOwnerPlayer || !CurrentOwnerPlayer->IsLocalPlayer()) 
////	{ 
////		// 스테이지 모드 베이스에서 로컬 플레이어 캐릭 용으로만.
////		return false;
////	}
////
////	ABladeIIPlayer* OldPlayerBackup = CurrentOwnerPlayer;
////	
////	if (DynSpawnNewPlayerCommon(bKeepCurrentClass ? GetCurrentPlayerClass() : GetMainPlayerClass()) != NULL)
////	{
////		// 위에서 성공적이었으면 CurrentOwnerPlayer 가 새로 생성한 것으로 바뀌어 있을 것.
////		// 사후 처리는 부활과 마찬가지로.. 혹시 다른 경우가 생기면.. 따로 처리해야지 뭐.
////		CurrentOwnerPlayer->NotifyBornAgain(OldPlayerBackup);
////		OnPlayerchanged_BornAgain();
////		CurrentOwnerPlayer->GetWorld()->DestroyActor(OldPlayerBackup);
////		return true;
////	}
////	return false;
////}
////
////bool UB2SkillRTManager::LocalPlayerSwitching(EPCClass NewClass)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_LocalPlayerSwitching);
////	if (!CurrentOwnerPlayer || !CurrentOwnerPlayer->IsLocalPlayer())
////	{
////		// 스테이지 모드 베이스에서 로컬 플레이어 캐릭 용으로만.
////		return false;
////	}
////
////	ABladeIIPlayer* OldPlayerBackup = CurrentOwnerPlayer;
////
////	if (DynSpawnLocalPlayerCommon(NewClass) != NULL)
////	{
////		CurrentOwnerPlayer->NotifyBornAgain(OldPlayerBackup);
////		OnPlayerchanged_BornAgain();
////		CurrentOwnerPlayer->GetWorld()->DestroyActor(OldPlayerBackup);
////		return true;
////	}
////	return false;
////}
////
////ABladeIIPlayer* UB2SkillRTManager::DynSpawnNewPlayerCommon(EPCClass ClassToSpawn)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_DynSpawnNewPlayerCommon);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::DynSpawnNewPlayerCommon"));
////
////	BII_CHECK(CurrentOwnerPlayer && ClassToSpawn != EPCClass::EPC_End);
////
////	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(CurrentOwnerPlayer->GetController());
////	ABladeIIPlayerAIController* B2AC = Cast<ABladeIIPlayerAIController>(CurrentOwnerPlayer->GetController());
////	// 로컬플레이어가 아니어두 부활한다. 컨트롤러는 실제 사용하는곳에서 널체크
////	//BII_CHECK(B2PC && B2PC->IsLocalPlayerController());
////
////	FTransform PrevTransform = CurrentOwnerPlayer->GetTransform();
////	FTransform NewTransform(
////		PrevTransform.GetRotation(),
////		PrevTransform.GetTranslation() + FVector(0.0f, 0.0f, 1.0f), // 살짝 위에서
////		PrevTransform.GetScale3D()
////	);
////
////	CurrentOwnerPlayer->SetActorEnableCollision(false); // 지금 자리에 spawn 이 되니 collision 을 꺼 준다.
////	// 반드시 DeferContruction 을 하고 Possess 후 FinishSpawning 을.
////	ABladeIIPlayer* NewSpawned = ABladeIIGameMode::StaticSpawnPCClassInfoPlayer(CurrentOwnerPlayer, ClassToSpawn, NewTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, true);
////	BII_CHECK(NewSpawned);
////
////	// NetPlayer 정보 세팅 시작
////	ABladeIINetPlayer* pNewNetPlayer = Cast<ABladeIINetPlayer>(NewSpawned);
////	ABladeIINetPlayer* pOldNetPlayer = Cast<ABladeIINetPlayer>(CurrentOwnerPlayer);
////	if (pNewNetPlayer && pOldNetPlayer)
////	{
////		UE_LOG(LogB2SkillRTManager, Log, TEXT("pNewNetPlayer NetId =  %d"), pNewNetPlayer->GetNetId());
////		UE_LOG(LogB2SkillRTManager, Log, TEXT("pOldNetPlayer NetId =  %d"), pOldNetPlayer->GetNetId());
////
////		pNewNetPlayer->SetNetId(pOldNetPlayer->GetNetId());
////		pNewNetPlayer->SetNetStatus(pOldNetPlayer->GetNetStatus());
////		pNewNetPlayer->SetSpawnMotionState(pOldNetPlayer->GetSpawnMotionState());
////	}
////
////	NewSpawned->BeginSpawningNoDamageGuard();  // 프로젝타일이 그 자리에 있다거나 하면 FinishSpawning 도중에 맞아서 죽을 수 있고 그렇게 되면 무한 반복 Tag 를 하는 등 문제가 발생할 수 있으니 막음
////	NewSpawned->SetTeamNum(CurrentOwnerPlayer->GetTeamNum());
////
////	NewSpawned->SetUnityTick(CurrentOwnerPlayer->GetUnityTick());
////
////	// NetPlayer 정보 세팅 끝
////	TagPlayerSpawnedClass<ABladeIIPlayer*>::GetInstance().Signal(NewSpawned);
////
////	if (B2PC)
////	{
////		B2PC->SetViewTargetWithBlend(NewSpawned);
////		B2PC->Possess(NewSpawned);
////	}
////
////	if (B2AC)
////	{
////		B2AC->Possess(NewSpawned);
////	}
////
////	SetCurrentOwnerPlayer(NewSpawned);
////
////	// Possess 이후에 BeginPlay 처리가 되도록
////	NewSpawned->FinishSpawning(NewTransform);
////
////	NewSpawned->EndSpawningNoDamageGuard(); // 이건 FinishSpawning 까지의 내부적 보호였고 이후 NoDamageNotifyState 에 의한 spawn 직후 보호가 또 있긴 할 것..
////	
////// 	if (B2PC)
////// 		CurrentOwnerPlayer->DisableInput(B2PC);
////
////	// 부활/태그 상황에 상관없는 공통 처리
////	OnPlayerChanged_Common();
////
////	PlayerChangedClass<ABladeIIPlayer*>::GetInstance().Signal(CurrentOwnerPlayer);
////
////	return NewSpawned;
////}
////
////ABladeIIPlayer* UB2SkillRTManager::DynSpawnLocalPlayerCommon(EPCClass ClassToSpawn)		// New Tutorial 에서 사용. 플레이어를 사용하지 않는다
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_DynSpawnLocalPlayerCommon);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::DynSpawnLocalPlayerCommon"));
////	
////	BII_CHECK(CurrentOwnerPlayer && ClassToSpawn != EPCClass::EPC_End);
////
////	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(CurrentOwnerPlayer->GetController());
////	
////	FTransform PrevTransform = CurrentOwnerPlayer->GetTransform();
////	FTransform NewTransform(
////		PrevTransform.GetRotation(),
////		PrevTransform.GetTranslation() + FVector(0.0f, 0.0f, 1.0f), // 살짝 위에서
////		PrevTransform.GetScale3D()
////	);
////
////	CurrentOwnerPlayer->SetActorEnableCollision(false); // 지금 자리에 spawn 이 되니 collision 을 꺼 준다.
////														// 반드시 DeferContruction 을 하고 Possess 후 FinishSpawning 을.
////	ABladeIIPlayer* NewSpawned = ABladeIIGameMode::StaticSpawnPCClassInfoPlayer(CurrentOwnerPlayer, ClassToSpawn, NewTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, true);
////	BII_CHECK(NewSpawned);
////
////	NewSpawned->BeginSpawningNoDamageGuard();  // 프로젝타일이 그 자리에 있다거나 하면 FinishSpawning 도중에 맞아서 죽을 수 있고 그렇게 되면 무한 반복 Tag 를 하는 등 문제가 발생할 수 있으니 막음
////
////	if (B2PC)
////	{
////		B2PC->SetViewTargetWithBlend(NewSpawned);
////		B2PC->Possess(NewSpawned);
////	}
////
////	SetCurrentOwnerPlayer(NewSpawned);
////
////	// Possess 이후에 BeginPlay 처리가 되도록
////	NewSpawned->FinishSpawning(NewTransform);
////
////	NewSpawned->EndSpawningNoDamageGuard(); // 이건 FinishSpawning 까지의 내부적 보호였고 이후 NoDamageNotifyState 에 의한 spawn 직후 보호가 또 있긴 할 것..
////
////	// 부활/태그 상황에 상관없는 공통 처리
////	OnPlayerChanged_Common();
////
////	PlayerChangedClass<ABladeIIPlayer*>::GetInstance().Signal(CurrentOwnerPlayer);
////
////	return NewSpawned;
////}
////
////void UB2SkillRTManager::OnPlayerChanged_Common()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_OnPlayerChanged_Common);
////	// CurentOwnerPlayer 가 바뀜에 따라 CachedSkillInfo 는 새로 뽑아줌. 추후 요구에 따라 이것도 이중으로 필요하게 되면 달라질 수 있다.
////	// SkillInfo 가 초기 구현에서는 플레이어 캐릭터 별로 따로라 CurrentOwnerPlayer 에서 가져왔으나 현재는 전체 SkillInfoObject 로 구성됨.
////	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
////	UB2PCClassInfoBox* PCClassInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox(this);
////	CachedSkillInfo = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : NULL;
////	BII_CHECK(CachedSkillInfo);
////}
////
////void UB2SkillRTManager::OnPlayerChanged_Tag(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_OnPlayerChanged_Tag);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::OnPlayerChanged_Tag"));
////
////	// NewPlayer 는 이 시점에서는 CurrentOwnerPlayer 이어야
////
////	TArray<FInputBoundSkillState_Base*>& SkillStates = InactivePlayerStateCache.IBSkillStates;
////	if (AllocateSkillState(SkillStates))
////		SetPlayerCharacterSkill(NewPlayer, SkillStates, CachedSkillOption[1]);
////
////	if (NeedsRebindSkillDoc(NewPlayer))
////		BindSkillDoc(NewPlayer, SkillStates);
////
////	FInputBoundSkillState_Base* TempCurrentState;
////	for (int32 SI = 0; SI < MAX_SKILL_INPUT; ++SI)
////	{
////		// 일단 중간 임시 공간에 백업한 후 IBSkillState 복구
////		TempCurrentState = IBSkillStates[SI];
////		IBSkillStates[SI] = InactivePlayerStateCache.IBSkillStates[SI];
////		InactivePlayerStateCache.IBSkillStates[SI] = TempCurrentState;
////	}
////
////	// 한번 이상 캐싱이 된 경우에만 Health 등을 복구. 그게 아니라면 걍 초기값일 테니.
////	if (InactivePlayerStateCache.bNotCachedYet == false)
////	{
////		InactivePlayerStateCache.SetToNewCommingPlayer(NewPlayer);
////	}
////	
////	InactivePlayerStateCache.SetVariablesByRetiringPlayer(OldPlayer); // 퇴장하는 플레이어 상태값들 백업
////	InactivePlayerStateCache.bNotCachedYet = false; // 최소한 한번 캐싱 되었음을 표시
////
////	if (OldPlayer->IsAlive())
////	{// 아직 살아있다면 HP 회복 타이머를 일단 세팅하고, 만일 회복 한도가 차 있는 상태라면 멈추게 될 것
////		InactivePlayerStateCache.StartTick();
////	}
////
////	UpdateSkillDocDataOnPCSwap(); // UI Doc 업데이트.
////		
/////*
////	if (AB2NetGameMode* NGMB = Cast<AB2NetGameMode>(UGameplayStatics::GetGameMode(NewPlayer)))
////	{
////		if (NGMB->GetPeerType() != PEER_RESTRICTED)
////			NGMB->HandleHealingHealth(NewPlayer);
////	}
////*/
////
////	NewPlayer->SetActorRotation(FRotator(0.0f, NewPlayer->GetActorRotation().Yaw, 0.0f)); // 가끔 캐릭터 회전이 꼬이는 일이 있는 거 같다(고 한다). 그래서 이쯤서 한번 roll 과 pitch 를 0 으로.
////}
////
////void UB2SkillRTManager::OnPlayerChangedByTag(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_OnPlayerChangedByTag);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::OnPlayerChangedByTag"));
////
////	// NewPlayer 는 이 시점에서는 CurrentOwnerPlayer 이어야
////
////	if (AllocateSkillState(InactivePlayerStateCache.IBSkillStates))
////		SetPlayerCharacterSkill(NewPlayer, InactivePlayerStateCache.IBSkillStates, CachedSkillOption[1]);
////
////	if (NeedsRebindSkillDoc(NewPlayer))
////		BindSkillDoc(NewPlayer, InactivePlayerStateCache.IBSkillStates);
////	
////	FInputBoundSkillState_Base* TempCurrentState;
////	for (int32 SI = 0; SI < MAX_SKILL_INPUT; ++SI)
////	{
////		// 일단 중간 임시 공간에 백업한 후 IBSkillState 복구
////		TempCurrentState = IBSkillStates[SI];
////		IBSkillStates[SI] = InactivePlayerStateCache.IBSkillStates[SI];
////		InactivePlayerStateCache.IBSkillStates[SI] = TempCurrentState;
////	}
////
////	// 한번 이상 캐싱이 된 경우에만 Health 를 복구. 그게 아니라면 걍 초기값일 테니.
////	if (InactivePlayerStateCache.bNotCachedYet == false)
////	{
////		InactivePlayerStateCache.SetToNewCommingPlayer(NewPlayer);
////	}
////
////	InactivePlayerStateCache.SetVariablesByRetiringPlayer(OldPlayer); // 퇴장하는 플레이어 상태값들 백업
////	InactivePlayerStateCache.bNotCachedYet = false; // 최소한 한번 캐싱 되었음을 표시
////
////	if (OldPlayer->IsAlive())
////	{// 아직 살아있다면 HP 회복 타이머를 일단 세팅하고, 만일 회복 한도가 차 있는 상태라면 멈추게 될 것
////		InactivePlayerStateCache.StartTick();
////	}
/////*
////	if (AB2NetGameMode* NGMB = Cast<AB2NetGameMode>(UGameplayStatics::GetGameMode(NewPlayer)))
////	{
////		if (NGMB->GetPeerType() != PEER_RESTRICTED)
////			NGMB->HandleHealingHealth(NewPlayer);
////	}
////*/
////}
////
////void UB2SkillRTManager::OnPlayerchanged_BornAgain()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_OnPlayerchanged_BornAgain);
////	// 부활인 경우 현재 플레이어 클래스 그대로 가게 되며, 둘 다 죽어야 부활하도록 했으므로 태그 캐릭 상태도 리셋.
////	for (FInputBoundSkillState_Base* ThisSkillState : IBSkillStates)
////	{
////		if (ThisSkillState)
////		{
////			ThisSkillState->ResetSkillState(this);
////		}
////	}
////	
////	for (FInputBoundSkillState_Base* ThisInactiveSkillState : InactivePlayerStateCache.IBSkillStates)
////	{
////		if (ThisInactiveSkillState)
////		{
////			ThisInactiveSkillState->ResetSkillState(this);
////		}
////	}
////
////	InactivePlayerStateCache.CurrentHealth = InactivePlayerStateCache.MaxHealth;
////	InactivePlayerStateCache.LastHealth = InactivePlayerStateCache.MaxHealth;
////
////	PlayStageBGMClass<>::GetInstance().Signal(); // 패배 음악이 나왔을 테니 다시 스테이지 전투 배경음악으로
////
////	UpdateSkillDocDataOnPCSwap(); // 필요 없을지 모르겠는데 여기도..
////}
////
////void UB2SkillRTManager::OverrideTagClass(EPCClass NewClass)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_OverrideTagClass);
////#if !UE_BUILD_SHIPPING
////	if (IsCurrentlyMainCharacter())
////	{
////		SubPlayerClass = NewClass;
////	}
////	else
////	{
////		MainPlayerClass = NewClass;
////	}
////
////	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
////	if (DocBattle) // 치트용 기능이겠지만 가능하면 바꿔주는 게 좋겠지 ㅋ
////	{
////		DocBattle->SetTagPCClass((int32)GetTagPlayerClass());
////	}
////#endif
////}
////
////bool UB2SkillRTManager::IsCurrentlyMainCharacter()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsCurrentlyMainCharacter);
////	return GetCurrentPlayerClass() == MainPlayerClass;
////}
////
////bool UB2SkillRTManager::IsMainPlayerCharacter(ABladeIIPlayer* InPC)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsMainPlayerCharacter);
////	return (InPC && InPC->GetCurrentPlayerClass() == GetMainPlayerClass());
////}
////
////EPCClass UB2SkillRTManager::GetTagPlayerClass()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetTagPlayerClass);
////	EPCClass TagPlayer = GetCurrentPlayerClass() == MainPlayerClass ? SubPlayerClass : MainPlayerClass;
////
////	// Note : 특정 모듈일경우 태그 캐릭터가 없는 경우도 존재함.
////	//BII_CHECK(SubPlayerClass != EPCClass::EPC_End);
////
////	return TagPlayer;
////}
////
////EPCClass UB2SkillRTManager::GetCurrentPlayerClass() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCurrentPlayerClass);
////	// CurrentOwnerPlayer 가 바뀌는 태그 동작 도중에 이게 불리면 타이밍에 따라 의도한 리턴값이 나오지 않을 수 있다.
////	BII_CHECK(CurrentOwnerPlayer);
////
////	return UB2PCClassInfo::FromCharacterTypeToPCClass(CurrentOwnerPlayer->CharacterType);
////}
////
////bool UB2SkillRTManager::IsTagPossibleAtDeadMoment() const
////{ // 플레이어 하나가 죽을 때에 들어주어야 할 소원이므로 가장 원초적인 조건들임. AttackState 나 타이밍에 대한 고려가 들어가지 않도록
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagPossibleAtDeadMoment);
////	return (CurrentOwnerPlayer && InactivePlayerStateCache.CurrentHealth > 0.0f); // InactivePlayerStateCache 의 Health 가 0 이면 홀로 남겨진 극한 상황 되겠다.
////}
////
////bool UB2SkillRTManager::IsTagPossible() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagPossible);
////	return IsTagPossibleAtDeadMoment() // 죽을 때 소원도 못들어준다면 평시에도 들어줄 수 없지.
////			&& IsSkillCurrentlyUsable(TAG_SKILL_INDEX, false)
////			// Prevent tag during KnockBack and BigBounce
////			&& !CurrentOwnerPlayer->HadTagDisabledDamage();
////}
////
////bool UB2SkillRTManager::IsTagPossibleRightNow() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagPossibleRightNow);
//////	return (IsTagPossible() && !IsTagPossibleButDeferred());
////	return IsTagPossible();
////}
////
////bool UB2SkillRTManager::IsTagPossibleButDeferred() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagPossibleButDeferred);
////	if (!IsTagPossible()){ // 기본적인 가능 여부
////		return false;
////	}
////
////	// 연계 태그가 아닌 공격 상태라면 가능은 하지만 pending 될 것. 즉 여기서 return true 하는 경우.
////	return (CurrentOwnerPlayer->GetAttackState() != EAttackState::ECS_None && CurrentOwnerPlayer->TagAttackMode == ETagAttackMode::EAM_DownAttack);
////}
////
////bool UB2SkillRTManager::IsTagSuggestedPlayerCondition() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagSuggestedPlayerCondition);
////	return ( IsTagPossible() && // 기본적으로 가능해야
////		(CurrentOwnerPlayer->IsInCriticalCondition()) );
////}
////
////bool UB2SkillRTManager::IsTagAttackSuggested() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagAttackSuggested);
////	return ( IsTagPossible() && // 기본적으로 가능해야 
////		CurrentOwnerPlayer->TagAttackMode == ETagAttackMode::EAM_DashAttack );
////}
////
////bool UB2SkillRTManager::IsCountingTagCooltime() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsCountingTagCooltime);
////	return IsSkillCurrentlyCountingCooltime(TAG_SKILL_INDEX);
////}
////
////float UB2SkillRTManager::GetRemainingTagCoolTime() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetRemainingTagCoolTime);
////	return GetRemainingCoolTime(TAG_SKILL_INDEX);
////}
////
////float UB2SkillRTManager::GetRemainingTagCoolTimeRatio() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetRemainingTagCoolTimeRatio);
////	return GetRemainingCoolTimeRatio(TAG_SKILL_INDEX);
////}
////
////bool UB2SkillRTManager::GetInactivePlayerNotCachedYet()
////{
////	return InactivePlayerStateCache.bNotCachedYet;
////}
////
////float UB2SkillRTManager::GetInactiveCharacterHealth()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetInactiveCharacterHealth);
////	return InactivePlayerStateCache.CurrentHealth;
////}
////
////void UB2SkillRTManager::InitInactiveCharacterMaxHealth(float InMaxHealth)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_InitInactiveCharacterMaxHealth);
////	if (InactivePlayerStateCache.bNotCachedYet)
////		InactivePlayerStateCache.MaxHealth = InactivePlayerStateCache.CurrentHealth = InMaxHealth;
////}
////
////float UB2SkillRTManager::GetInactiveCharacterMaxHealth()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetInactiveCharacterMaxHealth);
////	return InactivePlayerStateCache.MaxHealth;
////}
////
////float UB2SkillRTManager::GetInactiveCharacterArmor()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetInactiveCharacterArmor);
////	return InactivePlayerStateCache.LastArmor;
////}
////
////float UB2SkillRTManager::GetInactiveCharacterWeaponSkillPoint()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetInactiveCharacterWeaponSkillPoint);
////	return InactivePlayerStateCache.WeaponSkillPoint;
////}
////
////float UB2SkillRTManager::GetActiveCharacterWeaponSkillPoint() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetActiveCharacterWeaponSkillPoint);
////	BII_CHECK(CurrentOwnerPlayer);
////	return CurrentOwnerPlayer->GetWeaponSkillPoint();
////}
////
////float UB2SkillRTManager::GetActiveCharacterMaxWeaponSkillPoint() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetActiveCharacterMaxWeaponSkillPoint);
////	return IBSkillStates.IsValidIndex(WEAPON_6STAR_SKILL_INDEX) ? IBSkillStates[WEAPON_6STAR_SKILL_INDEX]->EnableWeaponSkillPoint : 0.0f;
////}
////
////void UB2SkillRTManager::UpdateWeaponSkillPoint(float SkillPoint)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_UpdateWeaponSkillPoint);
////	if (IBSkillStates.IsValidIndex(WEAPON_6STAR_SKILL_INDEX))
////		IBSkillStates[WEAPON_6STAR_SKILL_INDEX]->SetSkillPoint(SkillPoint);
////}
////
////void UB2SkillRTManager::SetZeroWeaponSkillCoolTime()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_SetZeroWeaponSkillCoolTime);
////	if (IBSkillStates.IsValidIndex(WEAPON_6STAR_SKILL_INDEX))
////		IBSkillStates[WEAPON_6STAR_SKILL_INDEX]->SetRemainingCooltime(0);
////}
////
////void UB2SkillRTManager::UpdateCounterAttackPoint(int32 CAPoint)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_UpdateCounterAttackPoint);
////	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
////	if (DocBattle)
////	{
////		if (DocBattle->GetMaxCounterAttackPoint() != MAX_COUNTER_ATTACK_POINT)
////			DocBattle->SetMaxCounterAttackPoint(MAX_COUNTER_ATTACK_POINT);
////
////		DocBattle->SetCounterAttackPoint(CAPoint);		
////	}
////}
////
////void UB2SkillRTManager::UpdatePlayerBuffChanged(const TArray<UB2Buff_Base*>& BuffList)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_OnPlayerDamageStateChanged);
////
////	ensure(CurrentOwnerPlayer);
////
////	bool HasDebuff = false;
////	bool HasAbnormal = false;
////	for (auto* BuffObj : BuffList)
////	{
////		if (BuffObj != nullptr && BuffObj->IsActive())
////		{
////			HasDebuff |= BuffObj->IsDebuff();
////			HasAbnormal |= BuffObj->GetBuffType() == EBuffType::Debuff_Freezing || BuffObj->GetBuffType() == EBuffType::Debuff_Stun;
////		}
////	}
////
////	for (auto* SkillObj : IBSkillStates)
////	{
////		if (SkillObj)
////		{
////			if (auto* DocSkill = Cast<UB2UIDocSkill>(SkillObj->GetDoc()))
////			{
////				bool PrevAbnormal = DocSkill->GetIsDisabledByDebuff();
////				if (PrevAbnormal != HasAbnormal)
////					DocSkill->SetIsDisabledByDebuff(HasAbnormal);
////			}
////
////			if (SkillObj->GetAttackState() == EAttackState::ECS_Skill04)
////			{
////				const bool InCooltime = SkillObj->IsCountingCooltime();
////				const bool CanStartSkill = CurrentOwnerPlayer->IsAvailableStartSkill(EAttackState::ECS_Skill04);
////				SkillObj->SetSkillSuggest(HasDebuff && CanStartSkill && !InCooltime);
////			}
////		}
////	}
////}
////
////void UB2SkillRTManager::GetCharacterRegisteredSkillIDs(EPCClass InCharClass, EB2GameMode InGMType, TArray<int32>& OutSkillIDs, ICharacterDataStore* InSpecifiedCharDataStore)
////{
////	// 게임모드에 따라서 다르게 가져올 수 있는 캐릭터 별 등록된 SkillID 가져오는 부분
////	// 기본적으로는 CharacterDataStore 가 소스임.
////
////	if (InGMType == EB2GameMode::CounterDungeon)
////	{
////		AB2CounterAttackGameMode::GetFullStackSkillId(InCharClass, OutSkillIDs);
////		checkSlow(OutSkillIDs.Num() == 1); /* 반격 던전에서는 스킬 하나 정해놓고 사용. */
////	}
////	else if (InGMType == EB2GameMode::Tutorial)
////	{
////		AB2TutorialGameMode::GetFullStackSkillId(InCharClass, OutSkillIDs); // 튜토리얼도 정해놓고 쓰겠지..
////	}
////	else
////	{
////		// InSpecifiedCharDataStore 를 제공하지 않으면 로컬 캐릭터 데이터로
////		ICharacterDataStore* FinalUsedCDS = InSpecifiedCharDataStore ? InSpecifiedCharDataStore : &BladeIIGameImpl::GetLocalCharacterData();
////		FinalUsedCDS->GetCharacterRegistredSkills(InCharClass, OutSkillIDs);
////		checkSlow(OutSkillIDs.Num() == MAX_ACTIVE_SKILL_INPUT);
////	}
////}
////
////void UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(UObject* WorldContextObject, EPCClass InCharClass, EB2GameMode InGMType, TArray<ESkillAnimType>& OutSkillAnimTypes, ICharacterDataStore* InSpecifiedCharDataStore)
////{
////	// 특정 캐릭터(DataStore)에 대해 등록된 실제 플레이 될 SkillAnimType 들을 긁어온다.
////	// 일반적인 전투 상황에서 사용될 일은 아마도 없을 것이고 대체로 사전에 pre-load 등의 목적으로 한번에 긁어오기 위함.
////
////	// InSpecifiedCharDataStore 를 제공하지 않으면 로컬 캐릭터 데이터로
////	ICharacterDataStore* FinalUsedCDS = InSpecifiedCharDataStore ? InSpecifiedCharDataStore : &BladeIIGameImpl::GetLocalCharacterData();
////
////	TArray<int32> RegistredSkillIDs;
////	GetCharacterRegisteredSkillIDs(InCharClass, InGMType, RegistredSkillIDs, FinalUsedCDS);
////
////	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
////	UB2PCClassInfoBox* PCClassInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox(WorldContextObject);
////	UB2SkillInfo* AllSkillInfo = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;
////
////	for (int32 SI = 0; SI < RegistredSkillIDs.Num(); ++SI)
////	{
////		int32 ThisSkillID = RegistredSkillIDs[SI];
////
////		// 이게 실제 SkillAnim 정하는 데에 필요.. 하다 보니 뻘짓이 많이 들어감.
////		int32 LocalIncMotionValue = GetCharacterSkillIncMotionValue(InCharClass, InGMType, ThisSkillID, FinalUsedCDS);
////		ESkillAnimType SkillAnimTypeOfThisSkill = GetSkillAnimIndexInternal(ThisSkillID, LocalIncMotionValue, AllSkillInfo);
////		OutSkillAnimTypes.AddUnique(SkillAnimTypeOfThisSkill);
////	}
////}
////
////float UB2SkillRTManager::GetCharacterSkillOptionValue(int32 InSkillId, ESkillOption InSkillOption, ICharacterDataStore* InSpecifiedCharDataStore)
////{
////	// 대체로 SkillRTManager 성격에 맞는 특정 SkillId 에 결부된 Active Skill Option 위주가 될 듯..?
////	// CombatStatEval 쪽은 Passive Skill Option 들임.
////
////	// InSpecifiedCharDataStore 를 제공하지 않으면 로컬 캐릭터 데이터로
////	ICharacterDataStore* FinalUsedCDS = InSpecifiedCharDataStore ? InSpecifiedCharDataStore : &BladeIIGameImpl::GetLocalCharacterData();
////	
////	const FSkillMasterData* SkillMasterData = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(InSkillId);
////	if (SkillMasterData)
////	{
////		if (FinalUsedCDS)
////		{
////			int32 SkillLevel = FinalUsedCDS->GetCharacterSkillLevel(InSkillId);
////
////			TArray<FSkillOptionData> SkillOptionArray;
////			SkillMasterData->LevelOptionData.MultiFind(SkillLevel, SkillOptionArray);
////
////			for (FSkillOptionData& SkillOptionElem : SkillOptionArray)
////			{
////				if (InSkillOption == SvrToCliSkillOption(SkillOptionElem.OptionId))
////				{
////					return SkillOptionElem.OptionValue;
////				}
////			}
////		}
////	}
////	return 0.0f;
////}
////
////int32 UB2SkillRTManager::GetCharacterSkillIncMotionValue(EPCClass InCharClass, EB2GameMode InGMType, int32 InSkillId, ICharacterDataStore* InSpecifiedCharDataStore)
////{
////	// InSpecifiedCharDataStore 를 제공하지 않으면 로컬 캐릭터 데a이터로
////	ICharacterDataStore* FinalUsedCDS = InSpecifiedCharDataStore ? InSpecifiedCharDataStore : &BladeIIGameImpl::GetLocalCharacterData();
////
////	// 혹시 게임모드 등에 따른 구분이 필요하면 넣도록.
////	// 현재 구현에서.. 튜토리얼은 IncMotion 의 특별처리가 없으므로 문제가 될 수도 있다.. 단, 이게 어디까지나 동일 skill 안에서의 조금씩의 variation 이고 튜토리얼은 캐릭터 레벨업 같은 게 안 된 초반에 플레이하는 것이므로 문제가 드러나지 않을 것.
////	// 반격 던전은 AnimBP 의 state 가 따로 구성된 상태에서는 역시나 문제가 안될 것.
////	return (int32)GetCharacterSkillOptionValue(InSkillId, ESkillOption::ESO_IncMotion, FinalUsedCDS);
////}
////
////void UB2SkillRTManager::ResetInitialized(ABladeIIPlayer* InOwnerPlayer)
////{
////	for (auto* ToDelete : IBSkillStates)
////		if(ToDelete != nullptr)
////			delete ToDelete;
////
////	for(auto* ToDelete : InactivePlayerStateCache.IBSkillStates)
////		if (ToDelete != nullptr)
////			delete ToDelete;
////
////	IBSkillStates.Empty();
////	InactivePlayerStateCache.IBSkillStates.Empty();
////
////	InitSkillRTManagerInner(InOwnerPlayer);
////}
////
////void UB2SkillRTManager::SetAutoBattle(int32 InAutoBattle)
////{
////	AutoBattle = InAutoBattle;
////}
////
////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
////void UB2SkillRTManager::Cheat_ForceSetCooltime(bool bIsForTag, float NewCooltime)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_Cheat_ForceSetCooltime);
////	if (bIsForTag)
////	{
////		Cheat_ForceCooltime_Tag = NewCooltime;
////	}
////	else
////	{
////		Cheat_ForceCooltime_Skill = NewCooltime;
////	}
////}
////#endif
////
////// 대체로 SkillRTManager 성격에 맞는 특정 SkillId 에 결부된 Active Skill Option 위주가 될 듯..?
////float UB2SkillRTManager::GetSkillOptionValue(int32 InSkillId, ESkillOption InSkillOption) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetSkillOptionValue);
////	BII_CHECK(CurrentOwnerPlayer);
////
////	if (CurrentOwnerPlayer)
////	{ // 다른 데서도 비슷한 기능을 사용할 일이 있으므로 웬만해서는 여기로 넣을 것..
////		return GetCharacterSkillOptionValue(InSkillId, InSkillOption, CurrentOwnerPlayer->GetCharacterDataStore());
////	}
////
////	return 0.f;
////}
////
////void UB2SkillRTManager::GetAllSkillOptionValues(int32 InSkillId, TArray<FSkillOptionData>& InOutSkillOption) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetAllSkillOptionValues);
////	BII_CHECK(CurrentOwnerPlayer);
////
////	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
////	auto* SkillMasterData = ClientDataStore.GetSkillMasterData(InSkillId);
////
////	if (SkillMasterData)
////	{
////		int32 SkillLevel = CurrentOwnerPlayer->GetCharacterDataStore()->GetCharacterSkillLevel(InSkillId);
////		SkillMasterData->LevelOptionData.MultiFind(SkillLevel, InOutSkillOption);
////	}
////}
////
////void UB2SkillRTManager::GetAllSkillOptionValuesForUnitySkill(TArray<FSkillOptionData>& InOutSkillOption) const
////{
////	//FUnitySkillMissionArray UnityInfos;
////	//
////	//CurrentOwnerPlayer->GetCharacterDataStore()->GetUnitySkillMissionArray(UnityInfos, GetCurrentPlayerClass());
////
////	//for (auto UnityItem : UnityInfos)
////	//{
////	//	const FUnitySkillOption UnityOption = GLOBALUNITYSKILLMANAGER.GetUnitySkillOptionInfo(UnityItem.MainClass, UnityItem.SubClass);
////	//	if (UnityOption.OptionId != 0)
////	//	{
////	//		// CacheSkillOption에서 서버타입 옵션을 원하므로. 바꿔줌 FUnitySkillOption::OptionId는 클라꺼 ESkillOption인듯
////	//		InOutSkillOption.Add(FSkillOptionData(UnityOption.OptionId, UnityOption.OptionValue));
////	//	}
////	//}
////}
////
////// 이거 CombatStatEval 에 있는 함수로 대체 안됨??
////float UB2SkillRTManager::GetPassiveSkillOptionValue(EPCClass CurrentClass, EPassiveType PassiveType, ESkillOption InSkillOption)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetPassiveSkillOptionValue);
////	// 여기서 EPassiveType 에 따른 필터링이 맞는 건지 확실히 모르겠음. PassiveType 이 다른 성격의 SkillOption 을 보유한 스킬들이 있음.
////
////	BII_CHECK(CurrentOwnerPlayer);
////	
////	TArray<int32> PassiveSkillIDs;
////
////	CurrentOwnerPlayer->GetCharacterDataStore()->GetCharacterPassiveSkills(CurrentClass, PassiveSkillIDs);
////
////	auto* AllSkillInfo = GetSkillInfoOfPlayer(CurrentOwnerPlayer);
////
////	TArray<FOptionValue> AllFoundRawValues;
////	for (int32 SkillId : PassiveSkillIDs)
////	{
////		auto* SkillInfo = AllSkillInfo ? AllSkillInfo->GetSingleInfoOfID(SkillId) : nullptr;
////		if (SkillInfo && SkillInfo->PassiveType == PassiveType)
////		{
////			FOptionValue OptionValue = FOptionValue(GetSkillOptionValue(SkillId, InSkillOption));
////			AllFoundRawValues.Add(OptionValue);
////		}
////	}
////	// AllFoundRawValues 에 들어간 값들은 0 ~ 100 스케일의 퍼센트 단위이고 옵션 적용 타입에 따라 0.0 ~ 1.0 스케일의 실제 인게임 적용 값으로 바꿈.
////	return GetEffectivePassiveSkillOptionValue(InSkillOption, AllFoundRawValues);
////}
////// 이거 CombatStatEval 에 있는 함수로 대체 안됨??
////float UB2SkillRTManager::GetPassiveSkillOptionValue(EPCClass CurrentClass, ESkillOption InSkillOption) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetPassiveSkillOptionValue_2);
////	BII_CHECK(CurrentOwnerPlayer);
////
////	TArray<int32> PassiveSkillIDs;
////
////	CurrentOwnerPlayer->GetCharacterDataStore()->GetCharacterPassiveSkills(CurrentClass, PassiveSkillIDs);
////
////	TArray<FOptionValue> AllFoundRawValues;
////	for (int32 SkillId : PassiveSkillIDs)
////	{
////		FOptionValue OptionValue = FOptionValue(GetSkillOptionValue(SkillId, InSkillOption));
////		AllFoundRawValues.Add(OptionValue);
////	}
////	// AllFoundRawValues 에 들어간 값들은 0 ~ 100 스케일의 퍼센트 단위이고 옵션 적용 타입에 따라 0.0 ~ 1.0 스케일의 실제 인게임 적용 값으로 바꿈.
////	return GetEffectivePassiveSkillOptionValue(InSkillOption, AllFoundRawValues);
////}
////
////bool UB2SkillRTManager::IsPercentValue(ESkillOption InSkillOption)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsPercentValue);
////	return !(
////			 InSkillOption == ESkillOption::ESO_DecCoolTime				||
////			 InSkillOption == ESkillOption::ESO_IncMotion				||
////			 InSkillOption == ESkillOption::ESO_DecCoolTimeOtherSkills	||
////			 InSkillOption == ESkillOption::ESO_BuffTime				||
////			 InSkillOption == ESkillOption::ESO_IncMaxNumCharacterBuff	||
////			 InSkillOption == ESkillOption::ESO_EnableTagOnKnockBack	||
////			 InSkillOption == ESkillOption::ESO_EnableTagOnBigBounce
////	); 
////}
////
////float UB2SkillRTManager::GetSkillDamageIncreaseRateByActive(EAttackState InAttackState) const // GetUnitedOptionStatusValue 에 의해 얻어오는 수치에 추가로 적용
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetSkillDamageIncreaseRateByActive);
////	return GetCachedSkillOption().GetActiveSkillOptionData(InAttackState, ESkillOption::ESO_IncAttackDamage);
////}
////
////float UB2SkillRTManager::GetBossDamageIncreaseRateByActive(EAttackState InAttackState) const // GetUnitedOptionStatusValue 에 의해 얻어오는 수치에 추가로 적용
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetBossDamageIncreaseRateByActive);
////	return GetCachedSkillOption().GetActiveSkillOptionData(InAttackState, ESkillOption::ESO_IncAttackDamageToBoss);
////}
////
////float UB2SkillRTManager::GetCriticalRateByActive(EAttackState InAttackState) const // GetUnitedOptionStatusValue 에 의해 얻어오는 수치에 추가로 적용
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCriticalRateByActive);
////	return GetCachedSkillOption().GetActiveSkillOptionData(InAttackState, ESkillOption::ESO_IncCriticalRate);
////}
////
////float UB2SkillRTManager::GetCriticalDamageRateByActive(EAttackState InAttackState) const // GetUnitedOptionStatusValue 에 의해 얻어오는 수치에 추가로 적용
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCriticalDamageRateByActive);
////	return GetCachedSkillOption().GetActiveSkillOptionData(InAttackState, ESkillOption::ESO_IncCriticalDamage);
////}
////
////const UB2SkillRTManager::FCachedSkillOption& UB2SkillRTManager::GetCachedSkillOption() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCachedSkillOption);
////	return CachedSkillOption[GetCurrentPlayerClass() == GetMainPlayerClass() ? 0 : 1];
////}
////
////void UB2SkillRTManager::SetCurrentOwnerPlayer(ABladeIIPlayer* NewOwner)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_SetCurrentOwnerPlayer);
////	check(NewOwner);
////
////	ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(NewOwner);
////	if (pPlayer)
////	{
////		UE_LOG(LogB2SkillRTManager, Warning, TEXT("SetCurrentOwnerPlayer NedId = %d"), pPlayer->GetNetId());
////	}
////
////	CurrentOwnerPlayer = NewOwner;	
////}
////
////void UB2SkillRTManager::FCachedSkillOption::CacheSkillOption(UB2SkillRTManager* SkillRTManager, EPCClass CharClass, const TArray<FInputBoundSkillState_Base*>& InIBSkillStates)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_CacheSkillOption);
////	BII_CHECK(SkillRTManager);
////
////	ICharacterDataStore* OwnerCDS = (SkillRTManager->CurrentOwnerPlayer) ? SkillRTManager->CurrentOwnerPlayer->GetCharacterDataStore() : NULL;
////	// Passive 스킬 옵션 값들은 대부분 다른 곳에 캐싱되는데 스킬과 직접적으로 연관되는 것들은 여기에.
////	SkillCooltimeDecbyPassive = CombatStatEval::GetUnitedOptionStatusValue(CharClass, EUnitedCombatOptions::UCO_Skill_DecSkillCooltime_General, OwnerCDS);
////	TagCooltimeDecbyPassive = CombatStatEval::GetUnitedOptionStatusValue(CharClass, EUnitedCombatOptions::UCO_Misc_DecTagCooltime, OwnerCDS);
////
////	// 특정 ActiveSkill 에 바인딩 되는 옵션값들 캐싱. 
////	for (auto* IBSkillState : InIBSkillStates)
////	{
////		// Cache all skill options related to this 
////		if (IBSkillState->GetAttackState() == EAttackState::ECS_None)
////			continue;
////
////		TArray<FSkillOptionData> SkillOptions;
////		SkillRTManager->GetAllSkillOptionValues(IBSkillState->SkillID, SkillOptions);
////
////		if(SkillOptions.Num() == 0 && IBSkillState->GetAttackState() == EAttackState::ECS_WeaponSkill)
////			SkillRTManager->GetAllSkillOptionValuesForUnitySkill(SkillOptions);
////
////		for (auto& SkillOption : SkillOptions)
////		{
////			bool bIsPercent = IsPercentValue(SvrToCliSkillOption(SkillOption.OptionId));
////			CachedActiveSkillOptionData.Add(GetSkillKey(IBSkillState->GetAttackState(), SvrToCliSkillOption(SkillOption.OptionId)), 
////				bIsPercent ? SkillOption.OptionValue * 0.01f : SkillOption.OptionValue);
////		}
////	}
////}
//

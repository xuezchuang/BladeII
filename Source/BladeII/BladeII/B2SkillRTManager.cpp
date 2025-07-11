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
//	// ActiveSkill �� ���ε� �Ǵ� ��Ÿ�� ����.
//	// ���� ECombatOptionApplyType �� Subtract_Value �� �������� OneMinusMultiply �� �ٲ� �� �ִ�.
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
////added by hsh from 20200410 ��ų ���� ���� ��� �߰�
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
//		// Active Skill �� ���ε� �� ��Ÿ�� ���� �ɼǰ� Passive �� ����Ǵ� ��Ÿ�� ���� �ɼ� ���� ����. �׸��� ���� �ƿ� ������ Ư�� ��ų���� ����Ǵ� ��Ÿ�� ���� �ɼ��� �� �ִµ� �� ���� ó��.
//		// ���� ECombatOptionApplyType �� Subtract_Value �� �������� OneMinusMultiply �� �ٲ� �� �ִ�.
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
//		// Active Skill �� ���ε� �� ��Ÿ�� ���� �ɼǰ� Passive �� ����Ǵ� ��Ÿ�� ���� �ɼ� ���� ����.
//		// ���� ECombatOptionApplyType �� Subtract_Value �� �������� OneMinusMultiply �� �ٲ� �� �ִ�.
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
//		CurrentHealth = RetiringPlayer->Health; // �ٲ� �������� LastHealth �� CurrentHealth �� ����, ���� ���߿��� HP �� ȸ��.
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
//		NewCommingPlayer->Health = CurrentHealth; // ���� ���� ������ CurrentHealth ���� ���.
//		NewCommingPlayer->Armor = LastArmor;
//		NewCommingPlayer->SetWeaponSkillPoint(WeaponSkillPoint);
//		NewCommingPlayer->SetUnityTick(IsUnityTick);
//		NewCommingPlayer->SetGuardCoolTime(GuardCoolTime);
//		NewCommingPlayer->SetHasNotTagDefence(HasNotTagDefence);
//		// MaxHealth �� Player spawn �� ����� �Ǹ� InactivePlayerStateCache ���� ������ �ʿ� ���� (���� �����ͼ� �ȵ� ��..). 
//		// InactivePlayerStateCache �� MaxHealth �� ���� �����ִ� ĳ������ UI �� HP �� ǥ�ø� ���� ��
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
////	SetCurrentOwnerPlayer(InOwnerPlayer); // CurrentOwnerPlayer �� ���� �÷��̸� ��ġ�鼭 �ٲ� �� �ִ�.
////	
////	if (CurrentOwnerPlayer)
////	{
////		CachedSkillInfo = GetSkillInfoOfPlayer(CurrentOwnerPlayer);
////
////		// RTManager ��ü�� �������� �±׸� ����ѵ�..
////		// ������ Main���� ������ ĳ���͸� Cache�ϴ� ����̶� ������, ����� ���� ���Ӹ�忡���� ����� ���� ����
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
////		// �̰� ó���� ���� �÷��̾� ������ �����ϴ� �� ���� UI �ܿ� ���� �����ֱ� ����. �ѹ� �±� �Ŀ��� ����� ������.
////		// �� �������� OwnerPlayer �� MaxHealth �� ���� ���� ������ �ȵǾ� ���� �� ���� (InitializeCombatStats �� �� �Ҹ� ���¶��). ù �±� �������� ��Ȯ�� ���� �ʿ��ϴٸ� �ٸ� ������� ���� �ʿ� (CombatStatEval::GetEquipAppliedLocalPCHealth ����)
////		InactivePlayerStateCache.SetVariablesByRetiringPlayer(CurrentOwnerPlayer); // ������ ���⼱ RetiringPlayer �� �ǹ̴� �ƴϴ�.
////		InactivePlayerStateCache.LastHealth = InactivePlayerStateCache.MaxHealth;
////		InactivePlayerStateCache.CurrentHealth = InactivePlayerStateCache.MaxHealth;
////
////		// ��, ExtraLevel �� �Ѿ�� ���¶�� HP ���� �� ����� ���� �ؾ���. ���� Ȱ�� ĳ������ ��� ABladeIIPlayer::InitializeCombatStats ����
////		if (AB2StageManager::GetCacheStageKeepEssentialData().IsPlayInExtraLevel())
////		{
////			AB2StageManager::GetCacheStageKeepEssentialData().SetPostExtraMapLoadSkillRTManager(CurrentOwnerPlayer, &InactivePlayerStateCache);
////			InactivePlayerStateCache.bNotCachedYet = false; // �� ���� ó������ caching �� �� �Ŵ� bNotCachedYet �� false ������ ��.
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
////		// UIDoc �� ��Ȱ���� ������ ���������� ó������ ����. �����÷��̾��� ��츸.
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
////			// ��� ��ų ���� ����
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
////	//[@AKI, 170815] Player�� Skill�� �����ϴ� �κ�. ��� ��ų���� ���� �Ǵ� ���� �ִٸ� ������ ���� ��
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
////					TagSkillObj->SetDoc(DocBattle); // �±׶� ��Ÿ ��ų�� UI Doc �� ���� ����.	
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
////	// 1. Tutorial Mode ó��
////	const bool IsTutorialMode = GetB2GameModeType(PlayerCharacter) == EB2GameMode::Tutorial;
////	const bool IsTutorialTargetClass = (PlayerClass == EPCClass::EPC_Wizard || PlayerClass == EPCClass::EPC_Fighter);
////	const bool IsTutorialSepcific = IsTutorialMode && IsTutorialTargetClass;
////
////	// Ʃ�丮�󿡼� �ٸ� ��ų��� (������ �ݰ�, ������ ��ų)
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
////	// Ʃ�丮�� ������ �ݰ� ����� ���� �� ������ �ִ��
////	if (IsTutorialMode)
////	{
////		if (auto* CounterSkillObj = OutIBSkillStates[COUNTER_SKILL_INDEX])
////			CounterSkillObj->IncreaseMotion = TUTORIAL_INCREASE_MOTION;
////	}
////
////	// 2. �ݰݴ����� ù��° ��Ǹ� ������ ����
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
////	// SkillInfo �� �ʱ� ���������� �÷��̾� ĳ���� ���� ���ζ� CurrentOwnerPlayer ���� ���������� ����� ��ü SkillInfoObject �� ������.
////	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(InPlayerChar));
////	UB2PCClassInfoBox* PCInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : NULL;
////	return PCInfoBox ? PCInfoBox->GetAllSkillInfo() : NULL;
////}
////
////void UB2SkillRTManager::UpdateSkillDocDataOnPCSwap()
////{ // �ʱ�ȭ, Ȥ�� �±� ������ �÷��̾� �ٲ� ���� CachedSkillInfo �� IBSkillState ����Ī �� ����.
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
////	// IncMotion �� ����Ǵ� ��ų�� �ְ� �ƴ� ��ų�� �ִ�.
////	switch (MappedSkillAttackState)
////	{
////	case EAttackState::ECS_Skill01:
////		return (ESkillAnimType)(InIncMotionValue + (int32)ESkillAnimType::ESA_Skill01_01);
////	case EAttackState::ECS_Skill02:
////	{
////		if (InIncMotionValue != 0) // ��� �� �ϳ� �ִ� ��ų
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
////			//BII_CHECK(!SkillState->IsCountingCooltime());// ��Ʈ��ũ �������� �ٸ� ��θ� ���� ī��Ʈ �����ߴٰ� �ٽ� ������ ��찡 �ִ� �� ����. ��� �α׷� ��ü
////			if (SkillState->IsCountingCooltime())
////			{
////				UE_LOG(LogBladeII, Warning, TEXT("Begin Count SkillCoolTime again.. %.1fs, while already counted %.1fs, SkillIndex %d, AttackState %d"), CoolTime, SkillState->GetRemainingCooltime(), SI, (int32)FinishedAttackState);
////			}
////
////			if (CoolTime <= 0.0f)
////			{
////				SkillState->ResetSkillState(this); // Cooltime �� �̸� ������ ���� ������� �ͼ� Reset �ϰ� ���� �� �� �̻��ϱ� ������ ��ư.
////
////				break;
////			}
////
////			SkillState->SetRemainingCooltime(CoolTime); // Reset the remaining time too.
////			SkillState->SetSkillDisabledByCooltime(true); // �Ϲ� ��ų�̶�� �̹� ���õǾ� �־�� ��.
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
////			// CountRemainingCoolTime �Ӹ��� �ƴ϶� ��ų �ߵ� �������� ������Ʈ�� �� �־�� ī��Ʈ ��� ���¿����� ���ϴ� ���� �ѷ��� ��.
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
////		// ���� ���⼭ CachedSkillInfo �� �ʿ��� ��Ȳ�̶�� InactivePlayer �뿡 ���� �Ȱ��� ó���ϸ� �� �ȴ�.
////		// The skill must not be usable when it needs to count. Timer should be set also.
////		if (!SkillState->IsCountingCooltime())
////		{
////
////			if (CurrentOwnerPlayer->GetAttackState() != SkillState->GetAttackState() && SkillState->GetRemainingCooltime() != 0.f)
////			{
////				//���⿡ �ɸ��� Player SkillState�� �����ִ� ����
////				//�⺻������ ABladeIIPlayer�� SetSkillState�� ���� ���������� State�� �ٲٷ��� ��� ���� �Ͼ�� ������ ����.
////				SkillState->SetIsCountingCooltime(true); //SkillState�� ������ ��� ������ Ǯ���ش�.
////			}
////				
////			continue;
////		}			
////
////		// Need to check CustomTimeDilation. ���� TimeDilation ó���� ���� ���� �� �� ������ �𸮾� timer �� ����������. �׷����� ��ų��Ÿ���� �̷��� ���� ī��Ʈ�ϴ� �� ������� ��Ʈ���ϱ� ���� ��.
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
////{ // �̰� �� ƽ �Ҹ��� ���̹Ƿ� ������ �ּ����� �����͸� ���� ���� ������Ʈ �ؾ� ��.
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_RealtimeUpdateUIDoc);
////	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
////	if (DocBattle)
////	{ // �±� ĳ���� Ŭ���� ���� BladeIIPlayer �ʼ� ������Ʈ
////		DocBattle->SetTagPCHealth(InactivePlayerStateCache.CurrentHealth);
////		DocBattle->SetTagPCMaxHealth(InactivePlayerStateCache.MaxHealth);
////
////		// ��׵��� �̺�Ʈ ���� ������Ʈ �Ϸ��� �� ������ �� 
////		DocBattle->SetIsTagPossible(IsTagPossible());
////		DocBattle->SetIsTagAttackSuggested(IsTagAttackSuggested()); // TagAttackSuggested �� TagSuggested ���� ���� �����ؼ� TagSuggested ���� �Ⱦ��� TagAttackSuggested �� ���� ������ �� �ֵ���.
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
////	//ħ��
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
////	// �±� �Է½� ABladeIIPlayer::StartTag ���� �����Ͽ� TagNotify �� ���� ���⼭ ���� ����
////
////	if (CurrentOwnerPlayer == NULL /*|| CurrentOwnerPlayer->IsAlive() == false*/) // ���� ������ Tag �� �ʿ��� ���� �ִ�.
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
////		// Spawn ���� player controller �ʿ� SpawnMotionState �� ���� �����Ѵ�. 
////		// Player character spawn ���� AnimBP overriding �� ���� Anim tick �� �ѹ� ó���Ǵµ� �� ���� ���¸� �־�� �ʿ䰡 �־ Player controller �� ���ϵ��� ��.
////		ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(CurrentOwnerPlayer->Controller);
////		if (B2PC)
////		{
////			B2PC->SetPCSpawnMotionState((EPCSpawnMotionState)(TagAttackType + int32(EPCSpawnMotionState::EPST_Tag)));
////		}
////
////		if (CurrentAttackState != EAttackState::ECS_None) // TagAttack �� ���� ����ϰ� �ִ� skill state �� ���� ��. �ٸ� ��η� noti �� ���� ���� ���̹Ƿ� ��Ÿ�� ī��Ʈ�� ����.
////		{
////			SetSkillCoolTimeTimer(CurrentAttackState);
////		}
////
////		// �±� ��Ÿ�� �� �ƴ϶� ��ų ��� ���� �±� �������� ���� ��ų ��Ÿ�� ī��Ʈ ������ �ȵ� ä�� ������ �� �� �־ Ȥ�ö� �׷� �� �ִٸ� �ٲ�ġ�� ���� ������ �ش�.
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
////		// �����÷��̾ �ٲ������ ����Ŭ�����ѹ� ���ش�.
////		if (OldPlayerBackup)
////		{
////			OldPlayerBackup->ClearAllBuffs();
////		}
////
////		if (DynSpawnNewPlayerCommon(NewSpawnClass) != NULL)
////		{
////			// ������ �������̾����� CurrentOwnerPlayer �� ���� ������ ������ �ٲ�� ���� ��.
////			// ���ο��� ������ tag ��Ȳ�� notify. Auto �μ��ΰ� �� �߰����� ��ġ�ڲ���
////			OldPlayerBackup->NotifyTagRetire(CurrentOwnerPlayer);
////			CurrentOwnerPlayer->NotifyTagSpawn(OldPlayerBackup);
////
////			OnPlayerChanged_Tag(OldPlayerBackup, CurrentOwnerPlayer);
////
////			if (IsTagPossibleAtDeadMoment()) // ȥ�� ������ ����� �±� ��Ÿ���� �� ī��Ʈ�� �ʿ䰡 ����.
////			{
////				// �±� Cooltime �¾�. �±׿� �Ϲ� ��ų ���� ���������� �����ϴ� ���.
////				// ���⼭ ó���ϰ� �Ǹ� �ٲ� �� ĳ���� �������� �±� Ÿ�̸Ӱ� ���ư��� �� ��. ������ Ŭ���� ���� �޶��� ������ ���������� ������..
////				// ������ �±� �� �Ϲ� ��ų ��Ÿ�� ó���� ������ �ؾ� �� ���̹Ƿ� �� �� ���� ������ �� ��.
////				SetSkillCoolTimeTimer(EAttackState::ECS_TagOtherChar);
////			}
////
////			// �ٲ� �ɸ������� ���ý�����Ʈ �ΰ�
////			CurrentOwnerPlayer->SetAttackState(CurrentAttackState);
////
////			CurrentOwnerPlayer->GetWorld()->DestroyActor(OldPlayerBackup);
////
////			return true;
////		}
////		else
////		{
////			// �����ߴٸ� ���� ���������� ��ư PlayerController �ʿ� ������ �� ���¸� �������.
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
////	// ���������� ���ݱ��� �������� ��Ȱ�� ���� �����ߴ� ��Ȱ ������ ��� �� ���� ���⼭�� �������� ������ Ÿ�Ը� ����ϴ� �ɷ�..
////	EResurrectBuffType LastRequestedBuffType = EResurrectBuffType::RBT_End;
////	if (auto* GMMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(CurrentOwnerPlayer)))
////		LastRequestedBuffType = GMMode->GetLastRequestedStageBuffType();
////
////	const bool bLastRequestedSpecificBuffType = (LastRequestedBuffType != EResurrectBuffType::RBT_None && LastRequestedBuffType != EResurrectBuffType::RBT_End);
////	TMap<EResurrectBuffType, int32> FinalFilteredBuffMap;
////	for (auto ThisBuff : SelectedResurrectionBuffMap)
////	{ 
////		// ���� ������ ������ ������ ã�� �� ���ٸ� ���..
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
////	// ���⿡���� �������� �� ���� ��� �־���´�. �������� ���ݱ��� �������� ��Ȱ���� ������ ��� Ÿ���� ������ ��.
////	// ������ ������ ����Ǵ� ���� ���� �ֱ� ������ Ÿ�Ը��� ����Ǿ�� ��.
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
////	// UIDocBattle �� �����ϴ� �� ��ǻ� �������� ����� ���� �÷��̾� ���̶� ������. 		
////	TMap<EResurrectBuffType, int32> AppliedResurrectBuff;	// �ΰ��� ���� UI ���� ǥ���ϴ� ���� ����Ǵ� ����
////	GetAppliedResurrectionBuffValues(AppliedResurrectBuff);
////
////	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
////	{
////		// ���⼭ ������ ���� �������� ����� �й� �޴����� ����.
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
////	// ���� �÷��̾��� ��� ������ ���� ��Ȱ ��û�� ���� ������. ��Ÿ �÷��̾� ĳ���� Ÿ���̳� ID �� ���� ������ �ʿ��� �� ���� ����.
////	if (!IsMyOwnerLocalPlayer()){
////		return false;
////	}
////
////	// ���� ������ ���� ���⿡ ������ �д�. InitializeCombatStats ���� ����ǵ��� !!! (���� : BornAgain ������ ȣ��)
////	if (AttackBuffCount != -1 && DefenseBuffCount != -1 && HealthBuffCount != -1)
////		UpdateResurrectionBuffOnResponse(AttackBuffCount, DefenseBuffCount, HealthBuffCount);
////
////	// ��Ȱ�� ��ġ�� ���������ش�. (���� : BornAgain ������ ȣ��)
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
////	//if (CurrentOwnerPlayer == NULL) // �̰� ���� resurrect ���� �ǵ��� ���̹Ƿ� �翬�� IsAlive üũ�� ���� �ʴ´�. ������ �ݴ�� IsAlive false �� �� üũ�ؾ� �� ����.
////	//{
////	//	return false;
////	//}
////
////	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(CurrentOwnerPlayer));
////
////	//// ���Ӹ�尡 �������� ������.
////	//if (!pGM)
////	//	return false;
////
////	//EPCClass NewSpawnClass = bForceSpawnAsMain ? MainPlayerClass : GetCurrentPlayerClass();
////	//if (NewSpawnClass == EPCClass::EPC_End)
////	//{
////	//	return false;
////	//}
////	//
////	//// Spawn ���� player Controller �ʿ� SpawnMotionState �� ���� �����Ѵ�. 
////	//// Player character spawn ���� AnimBP overriding �� ���� Anim tick �� �ѹ� ó���Ǵµ� �� ���� ���¸� �־�� �ʿ䰡 �־ Player controller �� ���ϵ��� ��.
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
////	//	// ������ �������̾����� CurrentOwnerPlayer �� ���� ������ ������ �ٲ�� ���� ��.
////	//	CurrentOwnerPlayer->NotifyBornAgain(OldPlayerBackup);
////	//	OnPlayerchanged_BornAgain(); // ���� ó����
////	//	
////	//	OldPlayerBackup->Role = ROLE_Authority;
////	//	CurrentOwnerPlayer->GetWorld()->DestroyActor(OldPlayerBackup);
////	//	return true;
////	//}
////	//else
////	//{
////	//	// �����ߴٸ� ���� ���������� ��ư PlayerController �ʿ� ������ �� ���¸� �������.
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
////	// �ܼ��� ���� �÷��̾� ĳ������ ������ϴ°�. ���� ���� �����ε� Ư���� �뵵�� ����� ���� �ִ�.
////	if (!CurrentOwnerPlayer || !CurrentOwnerPlayer->IsLocalPlayer()) 
////	{ 
////		// �������� ��� ���̽����� ���� �÷��̾� ĳ�� �����θ�.
////		return false;
////	}
////
////	ABladeIIPlayer* OldPlayerBackup = CurrentOwnerPlayer;
////	
////	if (DynSpawnNewPlayerCommon(bKeepCurrentClass ? GetCurrentPlayerClass() : GetMainPlayerClass()) != NULL)
////	{
////		// ������ �������̾����� CurrentOwnerPlayer �� ���� ������ ������ �ٲ�� ���� ��.
////		// ���� ó���� ��Ȱ�� ����������.. Ȥ�� �ٸ� ��찡 �����.. ���� ó���ؾ��� ��.
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
////		// �������� ��� ���̽����� ���� �÷��̾� ĳ�� �����θ�.
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
////	// �����÷��̾ �ƴϾ�� ��Ȱ�Ѵ�. ��Ʈ�ѷ��� ���� ����ϴ°����� ��üũ
////	//BII_CHECK(B2PC && B2PC->IsLocalPlayerController());
////
////	FTransform PrevTransform = CurrentOwnerPlayer->GetTransform();
////	FTransform NewTransform(
////		PrevTransform.GetRotation(),
////		PrevTransform.GetTranslation() + FVector(0.0f, 0.0f, 1.0f), // ��¦ ������
////		PrevTransform.GetScale3D()
////	);
////
////	CurrentOwnerPlayer->SetActorEnableCollision(false); // ���� �ڸ��� spawn �� �Ǵ� collision �� �� �ش�.
////	// �ݵ�� DeferContruction �� �ϰ� Possess �� FinishSpawning ��.
////	ABladeIIPlayer* NewSpawned = ABladeIIGameMode::StaticSpawnPCClassInfoPlayer(CurrentOwnerPlayer, ClassToSpawn, NewTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, true);
////	BII_CHECK(NewSpawned);
////
////	// NetPlayer ���� ���� ����
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
////	NewSpawned->BeginSpawningNoDamageGuard();  // ������Ÿ���� �� �ڸ��� �ִٰų� �ϸ� FinishSpawning ���߿� �¾Ƽ� ���� �� �ְ� �׷��� �Ǹ� ���� �ݺ� Tag �� �ϴ� �� ������ �߻��� �� ������ ����
////	NewSpawned->SetTeamNum(CurrentOwnerPlayer->GetTeamNum());
////
////	NewSpawned->SetUnityTick(CurrentOwnerPlayer->GetUnityTick());
////
////	// NetPlayer ���� ���� ��
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
////	// Possess ���Ŀ� BeginPlay ó���� �ǵ���
////	NewSpawned->FinishSpawning(NewTransform);
////
////	NewSpawned->EndSpawningNoDamageGuard(); // �̰� FinishSpawning ������ ������ ��ȣ���� ���� NoDamageNotifyState �� ���� spawn ���� ��ȣ�� �� �ֱ� �� ��..
////	
////// 	if (B2PC)
////// 		CurrentOwnerPlayer->DisableInput(B2PC);
////
////	// ��Ȱ/�±� ��Ȳ�� ������� ���� ó��
////	OnPlayerChanged_Common();
////
////	PlayerChangedClass<ABladeIIPlayer*>::GetInstance().Signal(CurrentOwnerPlayer);
////
////	return NewSpawned;
////}
////
////ABladeIIPlayer* UB2SkillRTManager::DynSpawnLocalPlayerCommon(EPCClass ClassToSpawn)		// New Tutorial ���� ���. �÷��̾ ������� �ʴ´�
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
////		PrevTransform.GetTranslation() + FVector(0.0f, 0.0f, 1.0f), // ��¦ ������
////		PrevTransform.GetScale3D()
////	);
////
////	CurrentOwnerPlayer->SetActorEnableCollision(false); // ���� �ڸ��� spawn �� �Ǵ� collision �� �� �ش�.
////														// �ݵ�� DeferContruction �� �ϰ� Possess �� FinishSpawning ��.
////	ABladeIIPlayer* NewSpawned = ABladeIIGameMode::StaticSpawnPCClassInfoPlayer(CurrentOwnerPlayer, ClassToSpawn, NewTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, true);
////	BII_CHECK(NewSpawned);
////
////	NewSpawned->BeginSpawningNoDamageGuard();  // ������Ÿ���� �� �ڸ��� �ִٰų� �ϸ� FinishSpawning ���߿� �¾Ƽ� ���� �� �ְ� �׷��� �Ǹ� ���� �ݺ� Tag �� �ϴ� �� ������ �߻��� �� ������ ����
////
////	if (B2PC)
////	{
////		B2PC->SetViewTargetWithBlend(NewSpawned);
////		B2PC->Possess(NewSpawned);
////	}
////
////	SetCurrentOwnerPlayer(NewSpawned);
////
////	// Possess ���Ŀ� BeginPlay ó���� �ǵ���
////	NewSpawned->FinishSpawning(NewTransform);
////
////	NewSpawned->EndSpawningNoDamageGuard(); // �̰� FinishSpawning ������ ������ ��ȣ���� ���� NoDamageNotifyState �� ���� spawn ���� ��ȣ�� �� �ֱ� �� ��..
////
////	// ��Ȱ/�±� ��Ȳ�� ������� ���� ó��
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
////	// CurentOwnerPlayer �� �ٲ� ���� CachedSkillInfo �� ���� �̾���. ���� �䱸�� ���� �̰͵� �������� �ʿ��ϰ� �Ǹ� �޶��� �� �ִ�.
////	// SkillInfo �� �ʱ� ���������� �÷��̾� ĳ���� ���� ���ζ� CurrentOwnerPlayer ���� ���������� ����� ��ü SkillInfoObject �� ������.
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
////	// NewPlayer �� �� ���������� CurrentOwnerPlayer �̾��
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
////		// �ϴ� �߰� �ӽ� ������ ����� �� IBSkillState ����
////		TempCurrentState = IBSkillStates[SI];
////		IBSkillStates[SI] = InactivePlayerStateCache.IBSkillStates[SI];
////		InactivePlayerStateCache.IBSkillStates[SI] = TempCurrentState;
////	}
////
////	// �ѹ� �̻� ĳ���� �� ��쿡�� Health ���� ����. �װ� �ƴ϶�� �� �ʱⰪ�� �״�.
////	if (InactivePlayerStateCache.bNotCachedYet == false)
////	{
////		InactivePlayerStateCache.SetToNewCommingPlayer(NewPlayer);
////	}
////	
////	InactivePlayerStateCache.SetVariablesByRetiringPlayer(OldPlayer); // �����ϴ� �÷��̾� ���°��� ���
////	InactivePlayerStateCache.bNotCachedYet = false; // �ּ��� �ѹ� ĳ�� �Ǿ����� ǥ��
////
////	if (OldPlayer->IsAlive())
////	{// ���� ����ִٸ� HP ȸ�� Ÿ�̸Ӹ� �ϴ� �����ϰ�, ���� ȸ�� �ѵ��� �� �ִ� ���¶�� ���߰� �� ��
////		InactivePlayerStateCache.StartTick();
////	}
////
////	UpdateSkillDocDataOnPCSwap(); // UI Doc ������Ʈ.
////		
/////*
////	if (AB2NetGameMode* NGMB = Cast<AB2NetGameMode>(UGameplayStatics::GetGameMode(NewPlayer)))
////	{
////		if (NGMB->GetPeerType() != PEER_RESTRICTED)
////			NGMB->HandleHealingHealth(NewPlayer);
////	}
////*/
////
////	NewPlayer->SetActorRotation(FRotator(0.0f, NewPlayer->GetActorRotation().Yaw, 0.0f)); // ���� ĳ���� ȸ���� ���̴� ���� �ִ� �� ����(�� �Ѵ�). �׷��� ���뼭 �ѹ� roll �� pitch �� 0 ����.
////}
////
////void UB2SkillRTManager::OnPlayerChangedByTag(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_OnPlayerChangedByTag);
////	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillRTManager::OnPlayerChangedByTag"));
////
////	// NewPlayer �� �� ���������� CurrentOwnerPlayer �̾��
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
////		// �ϴ� �߰� �ӽ� ������ ����� �� IBSkillState ����
////		TempCurrentState = IBSkillStates[SI];
////		IBSkillStates[SI] = InactivePlayerStateCache.IBSkillStates[SI];
////		InactivePlayerStateCache.IBSkillStates[SI] = TempCurrentState;
////	}
////
////	// �ѹ� �̻� ĳ���� �� ��쿡�� Health �� ����. �װ� �ƴ϶�� �� �ʱⰪ�� �״�.
////	if (InactivePlayerStateCache.bNotCachedYet == false)
////	{
////		InactivePlayerStateCache.SetToNewCommingPlayer(NewPlayer);
////	}
////
////	InactivePlayerStateCache.SetVariablesByRetiringPlayer(OldPlayer); // �����ϴ� �÷��̾� ���°��� ���
////	InactivePlayerStateCache.bNotCachedYet = false; // �ּ��� �ѹ� ĳ�� �Ǿ����� ǥ��
////
////	if (OldPlayer->IsAlive())
////	{// ���� ����ִٸ� HP ȸ�� Ÿ�̸Ӹ� �ϴ� �����ϰ�, ���� ȸ�� �ѵ��� �� �ִ� ���¶�� ���߰� �� ��
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
////	// ��Ȱ�� ��� ���� �÷��̾� Ŭ���� �״�� ���� �Ǹ�, �� �� �׾�� ��Ȱ�ϵ��� �����Ƿ� �±� ĳ�� ���µ� ����.
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
////	PlayStageBGMClass<>::GetInstance().Signal(); // �й� ������ ������ �״� �ٽ� �������� ���� �����������
////
////	UpdateSkillDocDataOnPCSwap(); // �ʿ� ������ �𸣰ڴµ� ���⵵..
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
////	if (DocBattle) // ġƮ�� ����̰����� �����ϸ� �ٲ��ִ� �� ������ ��
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
////	// Note : Ư�� ����ϰ�� �±� ĳ���Ͱ� ���� ��쵵 ������.
////	//BII_CHECK(SubPlayerClass != EPCClass::EPC_End);
////
////	return TagPlayer;
////}
////
////EPCClass UB2SkillRTManager::GetCurrentPlayerClass() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCurrentPlayerClass);
////	// CurrentOwnerPlayer �� �ٲ�� �±� ���� ���߿� �̰� �Ҹ��� Ÿ�ֿ̹� ���� �ǵ��� ���ϰ��� ������ ���� �� �ִ�.
////	BII_CHECK(CurrentOwnerPlayer);
////
////	return UB2PCClassInfo::FromCharacterTypeToPCClass(CurrentOwnerPlayer->CharacterType);
////}
////
////bool UB2SkillRTManager::IsTagPossibleAtDeadMoment() const
////{ // �÷��̾� �ϳ��� ���� ���� ����־�� �� �ҿ��̹Ƿ� ���� �������� ���ǵ���. AttackState �� Ÿ�ֿ̹� ���� ����� ���� �ʵ���
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagPossibleAtDeadMoment);
////	return (CurrentOwnerPlayer && InactivePlayerStateCache.CurrentHealth > 0.0f); // InactivePlayerStateCache �� Health �� 0 �̸� Ȧ�� ������ ���� ��Ȳ �ǰڴ�.
////}
////
////bool UB2SkillRTManager::IsTagPossible() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagPossible);
////	return IsTagPossibleAtDeadMoment() // ���� �� �ҿ��� ������شٸ� ��ÿ��� ����� �� ����.
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
////	if (!IsTagPossible()){ // �⺻���� ���� ����
////		return false;
////	}
////
////	// ���� �±װ� �ƴ� ���� ���¶�� ������ ������ pending �� ��. �� ���⼭ return true �ϴ� ���.
////	return (CurrentOwnerPlayer->GetAttackState() != EAttackState::ECS_None && CurrentOwnerPlayer->TagAttackMode == ETagAttackMode::EAM_DownAttack);
////}
////
////bool UB2SkillRTManager::IsTagSuggestedPlayerCondition() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagSuggestedPlayerCondition);
////	return ( IsTagPossible() && // �⺻������ �����ؾ�
////		(CurrentOwnerPlayer->IsInCriticalCondition()) );
////}
////
////bool UB2SkillRTManager::IsTagAttackSuggested() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_IsTagAttackSuggested);
////	return ( IsTagPossible() && // �⺻������ �����ؾ� 
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
////	// ���Ӹ�忡 ���� �ٸ��� ������ �� �ִ� ĳ���� �� ��ϵ� SkillID �������� �κ�
////	// �⺻�����δ� CharacterDataStore �� �ҽ���.
////
////	if (InGMType == EB2GameMode::CounterDungeon)
////	{
////		AB2CounterAttackGameMode::GetFullStackSkillId(InCharClass, OutSkillIDs);
////		checkSlow(OutSkillIDs.Num() == 1); /* �ݰ� ���������� ��ų �ϳ� ���س��� ���. */
////	}
////	else if (InGMType == EB2GameMode::Tutorial)
////	{
////		AB2TutorialGameMode::GetFullStackSkillId(InCharClass, OutSkillIDs); // Ʃ�丮�� ���س��� ������..
////	}
////	else
////	{
////		// InSpecifiedCharDataStore �� �������� ������ ���� ĳ���� �����ͷ�
////		ICharacterDataStore* FinalUsedCDS = InSpecifiedCharDataStore ? InSpecifiedCharDataStore : &BladeIIGameImpl::GetLocalCharacterData();
////		FinalUsedCDS->GetCharacterRegistredSkills(InCharClass, OutSkillIDs);
////		checkSlow(OutSkillIDs.Num() == MAX_ACTIVE_SKILL_INPUT);
////	}
////}
////
////void UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(UObject* WorldContextObject, EPCClass InCharClass, EB2GameMode InGMType, TArray<ESkillAnimType>& OutSkillAnimTypes, ICharacterDataStore* InSpecifiedCharDataStore)
////{
////	// Ư�� ĳ����(DataStore)�� ���� ��ϵ� ���� �÷��� �� SkillAnimType ���� �ܾ�´�.
////	// �Ϲ����� ���� ��Ȳ���� ���� ���� �Ƹ��� ���� ���̰� ��ü�� ������ pre-load ���� �������� �ѹ��� �ܾ���� ����.
////
////	// InSpecifiedCharDataStore �� �������� ������ ���� ĳ���� �����ͷ�
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
////		// �̰� ���� SkillAnim ���ϴ� ���� �ʿ�.. �ϴ� ���� ������ ���� ��.
////		int32 LocalIncMotionValue = GetCharacterSkillIncMotionValue(InCharClass, InGMType, ThisSkillID, FinalUsedCDS);
////		ESkillAnimType SkillAnimTypeOfThisSkill = GetSkillAnimIndexInternal(ThisSkillID, LocalIncMotionValue, AllSkillInfo);
////		OutSkillAnimTypes.AddUnique(SkillAnimTypeOfThisSkill);
////	}
////}
////
////float UB2SkillRTManager::GetCharacterSkillOptionValue(int32 InSkillId, ESkillOption InSkillOption, ICharacterDataStore* InSpecifiedCharDataStore)
////{
////	// ��ü�� SkillRTManager ���ݿ� �´� Ư�� SkillId �� ��ε� Active Skill Option ���ְ� �� ��..?
////	// CombatStatEval ���� Passive Skill Option ����.
////
////	// InSpecifiedCharDataStore �� �������� ������ ���� ĳ���� �����ͷ�
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
////	// InSpecifiedCharDataStore �� �������� ������ ���� ĳ���� ��a���ͷ�
////	ICharacterDataStore* FinalUsedCDS = InSpecifiedCharDataStore ? InSpecifiedCharDataStore : &BladeIIGameImpl::GetLocalCharacterData();
////
////	// Ȥ�� ���Ӹ�� � ���� ������ �ʿ��ϸ� �ֵ���.
////	// ���� ��������.. Ʃ�丮���� IncMotion �� Ư��ó���� �����Ƿ� ������ �� ���� �ִ�.. ��, �̰� �������� ���� skill �ȿ����� ���ݾ��� variation �̰� Ʃ�丮���� ĳ���� ������ ���� �� �� �� �ʹݿ� �÷����ϴ� ���̹Ƿ� ������ �巯���� ���� ��.
////	// �ݰ� ������ AnimBP �� state �� ���� ������ ���¿����� ���ó� ������ �ȵ� ��.
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
////// ��ü�� SkillRTManager ���ݿ� �´� Ư�� SkillId �� ��ε� Active Skill Option ���ְ� �� ��..?
////float UB2SkillRTManager::GetSkillOptionValue(int32 InSkillId, ESkillOption InSkillOption) const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetSkillOptionValue);
////	BII_CHECK(CurrentOwnerPlayer);
////
////	if (CurrentOwnerPlayer)
////	{ // �ٸ� ������ ����� ����� ����� ���� �����Ƿ� �����ؼ��� ����� ���� ��..
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
////	//		// CacheSkillOption���� ����Ÿ�� �ɼ��� ���ϹǷ�. �ٲ��� FUnitySkillOption::OptionId�� Ŭ�� ESkillOption�ε�
////	//		InOutSkillOption.Add(FSkillOptionData(UnityOption.OptionId, UnityOption.OptionValue));
////	//	}
////	//}
////}
////
////// �̰� CombatStatEval �� �ִ� �Լ��� ��ü �ȵ�??
////float UB2SkillRTManager::GetPassiveSkillOptionValue(EPCClass CurrentClass, EPassiveType PassiveType, ESkillOption InSkillOption)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetPassiveSkillOptionValue);
////	// ���⼭ EPassiveType �� ���� ���͸��� �´� ���� Ȯ���� �𸣰���. PassiveType �� �ٸ� ������ SkillOption �� ������ ��ų���� ����.
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
////	// AllFoundRawValues �� �� ������ 0 ~ 100 �������� �ۼ�Ʈ �����̰� �ɼ� ���� Ÿ�Կ� ���� 0.0 ~ 1.0 �������� ���� �ΰ��� ���� ������ �ٲ�.
////	return GetEffectivePassiveSkillOptionValue(InSkillOption, AllFoundRawValues);
////}
////// �̰� CombatStatEval �� �ִ� �Լ��� ��ü �ȵ�??
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
////	// AllFoundRawValues �� �� ������ 0 ~ 100 �������� �ۼ�Ʈ �����̰� �ɼ� ���� Ÿ�Կ� ���� 0.0 ~ 1.0 �������� ���� �ΰ��� ���� ������ �ٲ�.
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
////float UB2SkillRTManager::GetSkillDamageIncreaseRateByActive(EAttackState InAttackState) const // GetUnitedOptionStatusValue �� ���� ������ ��ġ�� �߰��� ����
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetSkillDamageIncreaseRateByActive);
////	return GetCachedSkillOption().GetActiveSkillOptionData(InAttackState, ESkillOption::ESO_IncAttackDamage);
////}
////
////float UB2SkillRTManager::GetBossDamageIncreaseRateByActive(EAttackState InAttackState) const // GetUnitedOptionStatusValue �� ���� ������ ��ġ�� �߰��� ����
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetBossDamageIncreaseRateByActive);
////	return GetCachedSkillOption().GetActiveSkillOptionData(InAttackState, ESkillOption::ESO_IncAttackDamageToBoss);
////}
////
////float UB2SkillRTManager::GetCriticalRateByActive(EAttackState InAttackState) const // GetUnitedOptionStatusValue �� ���� ������ ��ġ�� �߰��� ����
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(UB2SkillRTManager_GetCriticalRateByActive);
////	return GetCachedSkillOption().GetActiveSkillOptionData(InAttackState, ESkillOption::ESO_IncCriticalRate);
////}
////
////float UB2SkillRTManager::GetCriticalDamageRateByActive(EAttackState InAttackState) const // GetUnitedOptionStatusValue �� ���� ������ ��ġ�� �߰��� ����
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
////	// Passive ��ų �ɼ� ������ ��κ� �ٸ� ���� ĳ�̵Ǵµ� ��ų�� ���������� �����Ǵ� �͵��� ���⿡.
////	SkillCooltimeDecbyPassive = CombatStatEval::GetUnitedOptionStatusValue(CharClass, EUnitedCombatOptions::UCO_Skill_DecSkillCooltime_General, OwnerCDS);
////	TagCooltimeDecbyPassive = CombatStatEval::GetUnitedOptionStatusValue(CharClass, EUnitedCombatOptions::UCO_Misc_DecTagCooltime, OwnerCDS);
////
////	// Ư�� ActiveSkill �� ���ε� �Ǵ� �ɼǰ��� ĳ��. 
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

// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UITutorialBattleMain.h"
#include "B2UIDocHelper.h"
#include "B2TutorialGameMode.h"
#include "Event.h"
#include "B2UIBattleSkill.h"
#include "B2UIScenarioDialog.h"
#include "B2UIManager.h"
#include "B2UIDocBattle.h"
#include "B2UIBattleQTE.h"
#include "TutorialManager.h"
#include "B2TutorialGameMode.h"
#include "BladeIIPlayerController.h"

void UB2UITutorialBattleMain::Init()
{
	Super::Init();
	SetClose(false);
	SubscribeEvents();

	bAutoMinoRiddingSignal = false;
}

void UB2UITutorialBattleMain::BindDocAuto()
{
	Super::BindDocAuto();

	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (!DocBattle)return;

	DocBattle->OnIsCountingTagCooltimeChanged.RemoveAll(this);
	DocBattle->OnIsTagPossibleChanged.RemoveAll(this);
	DocBattle->OnIsTagDisabledByCooltimeChanged.RemoveAll(this);
	DocBattle->OnRemainingTagCooltimeChanged.RemoveAll(this);
	DocBattle->OnIsTagSuggestedChanged.RemoveAll(this);
	DocBattle->OnCounterAttackPointChanged.RemoveAll(this);
	DocBattle->OnQTEEnableStateChanged.RemoveAll(this);
	DocBattle->OnMaxCounterAttackPointChanged.RemoveAll(this);
	DocBattle->OnCounterAttackPointChanged.RemoveAll(this);

	DocBattle->OnQTEEnableStateChanged.AddUObject(this, &UB2UITutorialBattleMain::OnChangedQTEEnableState);

	if (TB_StageName.IsValid())
		TB_StageName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Tutorial_StageName")));

	SetVisibilitySkipButton(true);
}

void UB2UITutorialBattleMain::RequestAsyncLoadWidgets()
{
	if (auto* AssetLoader = UB2AssetLoader::Get())
	{
		auto LoadCallback = FB2AsyncLoadingComplete::CreateUObject(this, &UB2UITutorialBattleMain::OnCompleteAsyncLoad);

		AssetLoader->RequestAsyncLoad(GetQTEAsyncLoadName(), QTEUIPClass, LoadCallback);
	}
}

void UB2UITutorialBattleMain::SubscribeEvents()
{
	CAPTURE_UOBJECT(UB2UITutorialBattleMain);

	OpenDialogTicket = OpenDialogClass<FName, bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(OpenDialog, FName InDialogCodeName, bool bMaintainBattleUI)
		Capture->OnOpenScenarioDialog(InDialogCodeName);
	END_CAPTURE_OBJECT()
		);

	CloseDialogTicket = CloseDialogClass<FName>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(CloseDialog, FName InDialogCodeName)
		Capture->OnCloseScenarioDialog(InDialogCodeName);
	END_CAPTURE_OBJECT()
		);
}

void UB2UITutorialBattleMain::UnsubscribeEvents()
{
	OpenDialogClass<FName, bool>::GetInstance().Unsubscribe(OpenDialogTicket);
	CloseDialogClass<FName>::GetInstance().Unsubscribe(CloseDialogTicket);
}

void UB2UITutorialBattleMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_StageName);
	GET_SLOT(UB2Button, BTN_Skip);

	GET_SLOT(UImage, IMG_GuardLock);

	GET_SLOT(UCanvasPanel, CP_GuardSuggest);
	GET_SLOT(UCanvasPanel, CP_QTESuggest);
	GET_SLOT(UCanvasPanel, CP_AttackSuggest);
	GET_SLOT(UCanvasPanel, CP_MountAttackSuggest);
	GET_SLOT(UCanvasPanel, CP_SuggestPortraitSub);
	GET_SLOT(UCanvasPanel, CP_MoveSuggest);

	if (CP_MoveSuggest.IsValid())
		CP_MoveSuggest->SetVisibility(ESlateVisibility::Hidden);

	if (CP_QTESuggest.IsValid())
		CP_QTESuggest->SetVisibility(ESlateVisibility::Hidden);

	if (CP_SuggestPortraitSub.IsValid())
		CP_SuggestPortraitSub->SetVisibility(ESlateVisibility::Hidden);

	if (CP_AttackSuggest.IsValid())
		CP_AttackSuggest->SetVisibility(ESlateVisibility::Hidden);

	if (CP_MountAttackSuggest.IsValid())
		CP_MountAttackSuggest->SetVisibility(ESlateVisibility::Hidden);

	SetGuardWidgetState(ETutorialWidgetState::Lock);

	CP_SuggestSkills.AddDefaulted(MAX_ACTIVE_SKILL_INPUT);
	GET_SLOT_BYNAME(UCanvasPanel, CP_SuggestSkills[0], CP_SuggestSkill0);
	GET_SLOT_BYNAME(UCanvasPanel, CP_SuggestSkills[1], CP_SuggestSkill1);
	GET_SLOT_BYNAME(UCanvasPanel, CP_SuggestSkills[2], CP_SuggestSkill2);
	GET_SLOT_BYNAME(UCanvasPanel, CP_SuggestSkills[3], CP_SuggestSkill3);

	for (auto SuggestSkillWidget : CP_SuggestSkills)
	{
		if (SuggestSkillWidget.IsValid())
			SuggestSkillWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UITutorialBattleMain::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);

	for (auto& WidgetGroup : WidgetGroupList)
		WidgetGroup.WidgetList.Empty();

	WidgetGroupList.Empty();
}

void UB2UITutorialBattleMain::BindDelegates()
{
	Super::BindDelegates();

	if (BTN_Skip.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(BTN_Skip, &UB2UITutorialBattleMain::OnClickBtnSkip)
	}
}

void UB2UITutorialBattleMain::CloseWidgetDelegate()
{
	// nothing
}

void UB2UITutorialBattleMain::SetVisibilitySkipButton(bool bVisibility)
{
	if (BTN_Skip.IsValid())
	{
		BTN_Skip->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2UITutorialBattleMain::TagCommon(EPCClass MainClass, EPCClass SubClass)
{
	UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	bool bIsTagAttack = (DocBattle ? DocBattle->GetbTagAttackPending() : false);

	Tag_BP(MainClass, SubClass, bIsTagAttack);
}

void UB2UITutorialBattleMain::SetLevelMainChar(int32 InValue)
{
	InValue = 1; // 1로 고정
	if (TB_MainCharLevel.IsValid())
		TB_MainCharLevel->SetText(FText::AsNumber(InValue));
}

void UB2UITutorialBattleMain::HideTutorialWidgetGroup(const FString& WidgetGroupName)
{
	SetVisibilityWidgetGroup(WidgetGroupName, ESlateVisibility::Hidden);
}

void UB2UITutorialBattleMain::RestoreTutorialWidgetGroup(const FString& WidgetGroupName)
{
	RestoreVisibilityWidgetGroup(WidgetGroupName);

	if (WidgetGroupName == TEXT("Attack"))
	{
		SetAttackWidgetState(ETutorialWidgetState::Suggest);
		EventAttackSuggestChanged(true);
	}
}

void UB2UITutorialBattleMain::SetTagWidgetState(ETutorialWidgetState WidgetState)
{
	ESlateVisibility SuggestVisible = ESlateVisibility::HitTestInvisible;

	SetVisibilityTagWidget(true);

	if (WidgetState == ETutorialWidgetState::Suggest)
	{
		PlayTagSuggestAnim_BP();
		PlayTagSuggestByPlayerConditionAnim_BP();

		SetEnabledTagBtn(true);
		SetEnabledSubPortrait(true);
	}
	else
	{
		SuggestVisible = ESlateVisibility::Hidden;

		StopTagSuggestAnim_BP();
		StopTagSuggestByPlayerConditionAnim_BP();

		SetEnabledTagBtn(false);
		SetEnabledSubPortrait(false);
	}
	if (CP_TagSuggest_TagCharChallenge.IsValid())
	{
		CP_TagSuggest_TagCharChallenge->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CP_TagAttackSuggestSet.IsValid())
	{
		CP_TagAttackSuggestSet->SetVisibility(SuggestVisible);
	}
	if (IMG_FX_TagSuggest.IsValid())
	{
		IMG_FX_TagSuggest->SetVisibility(SuggestVisible);
	}

	if (CP_SuggestPortraitSub.IsValid())
		CP_SuggestPortraitSub->SetVisibility(SuggestVisible);
}

void UB2UITutorialBattleMain::SetMoveWidgetState(ETutorialWidgetState WidgetState)
{
	bool bShowSuggest = ETutorialWidgetState::Suggest == WidgetState ? true : false;
	if (CP_MoveSuggest.IsValid())
		CP_MoveSuggest->SetVisibility(bShowSuggest ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	EventMoveSuggestChanged(bShowSuggest);
}

void UB2UITutorialBattleMain::SetAttackWidgetState(ETutorialWidgetState WidgetState)
{
	bool bShowSuggest = ETutorialWidgetState::Suggest == WidgetState ? true : false;
	if (CP_AttackSuggest.IsValid())
		CP_AttackSuggest->SetVisibility(bShowSuggest ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	EventAttackSuggestChanged(bShowSuggest);
}

void UB2UITutorialBattleMain::SetMountAttackWidgetState(ETutorialWidgetState WidgetState)
{
	bool bShowSuggest = ETutorialWidgetState::Suggest == WidgetState ? true : false;
	if (CP_MountAttackSuggest.IsValid())
		CP_MountAttackSuggest->SetVisibility(bShowSuggest ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	EventMountAttackSuggestChanged(bShowSuggest);
}

void UB2UITutorialBattleMain::SetSkillWidgetSuggest(int32 SkillIndex, bool bSuggest)
{
	if (CP_SuggestSkills.IsValidIndex(SkillIndex) && CP_SuggestSkills[SkillIndex].IsValid())
	{
		CP_SuggestSkills[SkillIndex]->SetVisibility(bSuggest ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		EventSkillSuggestChanged(SkillIndex, bSuggest);
	}
}

void UB2UITutorialBattleMain::SetSkillWidgetClickable(bool bClickable, bool bSkipWeapon/* = true */)
{
	for (UB2UIBattleSkill* SkillWidget : SkillAssets)
	{
		if (bSkipWeapon && SkillWidget && SkillWidget->GetIndex() == WEAPON_6STAR_SKILL_INDEX)
		{
			SkillWidget->SetEnabledBtn(false);
			continue;
		}
		if (SkillWidget)
			SkillWidget->SetEnabledBtn(bClickable);
	}
}

void UB2UITutorialBattleMain::SetUnitySkillWidgetClickable(bool bUnityClickable, bool bOtherClickable)
{
	for (UB2UIBattleSkill* SkillWidget : SkillAssets)
	{
		if (SkillWidget && SkillWidget->GetIndex() == WEAPON_6STAR_SKILL_INDEX)
		{
			SkillWidget->SetEnabledBtn(bUnityClickable);
			continue;
		}

		if (SkillWidget)
			SkillWidget->SetEnabledBtn(bOtherClickable);
	}
}

void UB2UITutorialBattleMain::SetUIEnable(bool bEnable)
{
	SetAttackEnable(bEnable);
	SetSkillWidgetClickable(bEnable);
	SetGuardEnable(bEnable);
}

void UB2UITutorialBattleMain::SetGuardWidgetState(ETutorialWidgetState WidgetState)
{
	ESlateVisibility GuardLockVisible = ESlateVisibility::SelfHitTestInvisible;
	ESlateVisibility GuardSuggestVisible = ESlateVisibility::SelfHitTestInvisible;
	ESlateVisibility GuardButtonVisible = ESlateVisibility::Visible;

	switch (WidgetState)
	{
	case ETutorialWidgetState::Suggest:
		GuardLockVisible = ESlateVisibility::Hidden;
		GuardButtonVisible = ESlateVisibility::Visible;

		EventGuardSuggestChanged(true);

		break;
	case ETutorialWidgetState::Lock:
		GuardLockVisible = ESlateVisibility::Visible;
		GuardSuggestVisible = ESlateVisibility::Hidden;
		GuardButtonVisible = ESlateVisibility::Hidden;

		break;
	case ETutorialWidgetState::Enable:
		GuardLockVisible = ESlateVisibility::Hidden;
		GuardSuggestVisible = ESlateVisibility::Hidden;

		EventGuardSuggestChanged(false);

	default: break;
	}

	if (IMG_GuardLock.IsValid())
		IMG_GuardLock->SetVisibility(GuardLockVisible);

	if (CP_GuardSuggest.IsValid())
		CP_GuardSuggest->SetVisibility(GuardSuggestVisible);

	if (BTN_Guard.IsValid())
		BTN_Guard->SetVisibility(GuardButtonVisible);

	EventGuardSuggestChanged(GuardSuggestVisible == ESlateVisibility::SelfHitTestInvisible ? true : false);
}

void UB2UITutorialBattleMain::SetQTEWidgetState(ETutorialWidgetState WidgetState)
{
	ESlateVisibility QTESuggestVisible = ESlateVisibility::SelfHitTestInvisible;
	bool bQTEVisible = true;

	switch (WidgetState)
	{
	case ETutorialWidgetState::Suggest:
		QTESuggestVisible = ESlateVisibility::SelfHitTestInvisible;

		EventQTESuggestChanged(true);
		break;

	case ETutorialWidgetState::Enable:

		QTESuggestVisible = ESlateVisibility::Hidden;
		EventQTESuggestChanged(false);

		break;
	default:

		bQTEVisible = false;
		break;
	}

	if (CP_QTESuggest.IsValid())
		CP_QTESuggest->SetVisibility(QTESuggestVisible);

	// 초기 튜토리얼 구현 시 First/Second QTE Attack 구분이 없었음.. 
	//SetQTEEnable(bQTEVisible ? EQTEUIAttackEnableTimingState::FirstAttack : EQTEUIAttackEnableTimingState::None);
}

void UB2UITutorialBattleMain::AddWidgetToWidgetGroup(const FString& InGroupName, const TArray<UWidget*>& GroupWidgetList)
{
	for (UWidget* WidgetObject : GroupWidgetList)
	{
		if (WidgetObject != nullptr)
		{
			FB2WidgetGroup& WidgetGroup = GetOrAddWidgetGroupRef(InGroupName);
			FB2CachedWidgetStatus NewWidgetStatus(WidgetObject);

			WidgetGroup.WidgetList.Add(NewWidgetStatus);
		}
	}
}

FB2WidgetGroup& UB2UITutorialBattleMain::GetOrAddWidgetGroupRef(const FString& InGroupName)
{
	for (auto& WidgetGroup : WidgetGroupList)
	{
		if (WidgetGroup.GroupName == InGroupName)
			return WidgetGroup;
	}

	FB2WidgetGroup NewWidgetGroup;
	NewWidgetGroup.GroupName = InGroupName;

	const int32 AddIndex = WidgetGroupList.Add(NewWidgetGroup);
	return WidgetGroupList[AddIndex];
}

void UB2UITutorialBattleMain::SetVisibilityWidgetGroup(const FString& InGroupName, ESlateVisibility GroupVisibility)
{
	FB2WidgetGroup& WidgetGroup = GetOrAddWidgetGroupRef(InGroupName);
	for (FB2CachedWidgetStatus& WidgetStatus : WidgetGroup.WidgetList)
	{
		if (WidgetStatus.WidgetPtr)
			WidgetStatus.WidgetPtr->SetVisibility(GroupVisibility);
	}
}

void UB2UITutorialBattleMain::RestoreVisibilityWidgetGroup(const FString& InGroupName)
{
	FB2WidgetGroup& WidgetGroup = GetOrAddWidgetGroupRef(InGroupName);
	for (FB2CachedWidgetStatus& WidgetStatus : WidgetGroup.WidgetList)
	{
		if (WidgetStatus.WidgetPtr)
			WidgetStatus.WidgetPtr->SetVisibility(WidgetStatus.OrgVisibility);
	}
}

void UB2UITutorialBattleMain::OnOpenScenarioDialog(FName OpenDialogName)
{
	/*
	if (BTN_ScenarioTouch.IsValid())
	BTN_ScenarioTouch->SetVisibility(ESlateVisibility::Visible);
	*/
}

void UB2UITutorialBattleMain::OnCloseScenarioDialog(FName CloseDialogName)
{
	/*
	if (BTN_ScenarioTouch.IsValid())
	BTN_ScenarioTouch->SetVisibility(ESlateVisibility::Hidden);
	*/
}

void UB2UITutorialBattleMain::OnChangedQTEEnableState(class UB2UIDocBase* Sender, EQTEUIAttackEnableTimingState CurrQTEState, EQTEUIAttackEnableTimingState PrevQTEState)
{
	if (CurrQTEState != EQTEUIAttackEnableTimingState::None && !bAutoMinoRiddingSignal)
	{
		bAutoMinoRiddingSignal = true;
		SetQTEWidgetState(ETutorialWidgetState::Suggest);

		AutoMinoRiddingClass<>::GetInstance().Signal();
	}

	SetQTEEnable(CurrQTEState);
}

void UB2UITutorialBattleMain::HideHUDForQTE(bool bHide)
{
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetOwningPlayer());

	if (bHide)
	{
		this->ForceHide();

		if (B2PC)
		{
			B2PC->SetVirtualJoystickVisibility(false);
		}
	}
	else
	{
		if (B2PC && !B2PC->IsInCinematicMode())
		{
			this->RestoreFromForceHidden();
			B2PC->SetVirtualJoystickVisibility(true);
		}
	}

	if (CreatedQTEUIP)
	{
		CreatedQTEUIP->SetVisibilityFromTutorialBattleMainUI(ESlateVisibility::Hidden);
	}
}

void UB2UITutorialBattleMain::OnClickBtnSkip()
{
	AB2TutorialGameMode* B2TGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (B2TGM)
	{
		B2TGM->SetFinishTutorial();
	}
}

void UB2UITutorialBattleMain::CloseBattleMainUIEvent(bool Value)
{
	SetClose(false);
}

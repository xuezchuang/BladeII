// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRaidBattle.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2UIDocBattleStage.h"
#include "B2UIBossStatus.h"
#include "B2UIManager.h"
#include "B2UIBattleCombo.h"
#include "B2RaidGameMode.h"
#include "B2UIPAutoBattleIcon.h"


void UB2UIRaidBattle::Init()
{
	Super::Init();
}

void UB2UIRaidBattle::BindDocAuto()
{
	Super::BindDocAuto();

	// UIBattleMain 에서 사용안하는 것들은 꺼주자.
	SetWidgetForRaidBattle();

	uint8 SlotIndex = 1;
	for (auto& ThisSlot : TeamCharSlots)
	{
		ThisSlot->CreateResurrectBuffIcon(EResurrectBuffType::RBT_None, SlotIndex);
		SlotIndex++;
	}

	UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (DocRaid)
	{	
		DocRaid->OnCharName0Changed.AddUObject(this, &UB2UIRaidBattle::OnChangedCharName);
		DocRaid->OnRaidBattleTimeChanged.AddUObject(this, &UB2UIRaidBattle::OnChangedRaidBattleTime);
		DocRaid->OnRaidContribution0Changed.AddUObject(this, &UB2UIRaidBattle::OnChangedMainPCContribution);
		DocRaid->OnVisibleButtonForDefeatChanged.AddUObject(this, &UB2UIRaidBattle::OnVisibleBtnForDefeat);

		if (TeamCharSlots[0].IsValid()){
			DocRaid->OnCharName1Changed.AddUObject(TeamCharSlots[0].Get(), &UB2UITeamMatchBattleSlot::OnChangedCharName);
			DocRaid->OnPCHealth1Changed.AddUObject(TeamCharSlots[0].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
			DocRaid->OnPCShield1Changed.AddUObject(TeamCharSlots[0].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
			DocRaid->OnRaidContribution1Changed.AddUObject(TeamCharSlots[0].Get(), &UB2UITeamMatchBattleSlot::OnChanagedContribution);
		}
		if (TeamCharSlots[1].IsValid()){
			DocRaid->OnCharName2Changed.AddUObject(TeamCharSlots[1].Get(), &UB2UITeamMatchBattleSlot::OnChangedCharName);
			DocRaid->OnPCHealth2Changed.AddUObject(TeamCharSlots[1].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
			DocRaid->OnPCShield2Changed.AddUObject(TeamCharSlots[1].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
			DocRaid->OnRaidContribution2Changed.AddUObject(TeamCharSlots[1].Get(), &UB2UITeamMatchBattleSlot::OnChanagedContribution);
		}
		if (TeamCharSlots[2].IsValid()){
			DocRaid->OnCharName3Changed.AddUObject(TeamCharSlots[2].Get(), &UB2UITeamMatchBattleSlot::OnChangedCharName);
			DocRaid->OnPCHealth3Changed.AddUObject(TeamCharSlots[2].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
			DocRaid->OnPCShield3Changed.AddUObject(TeamCharSlots[2].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
			DocRaid->OnRaidContribution3Changed.AddUObject(TeamCharSlots[2].Get(), &UB2UITeamMatchBattleSlot::OnChanagedContribution);
		}

		if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
			UpdateAttackButtonStyleToPCClass(IntToPCClass(DocBattle->GetCurPCClass()));
	}
}

void UB2UIRaidBattle::UnbindDoc()
{
	Super::UnbindDoc();

	UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (DocRaid)
	{
		DocRaid->OnRaidBattleTimeChanged.RemoveAll(this);
		DocRaid->OnRaidContribution0Changed.RemoveAll(this);
		DocRaid->OnVisibleButtonForDefeatChanged.RemoveAll(this);

		if (TeamCharSlots[0].IsValid()){
			DocRaid->OnCharName1Changed.RemoveAll(TeamCharSlots[0].Get());
			DocRaid->OnPCLevel1Changed.RemoveAll(TeamCharSlots[0].Get());
			DocRaid->OnPCHealth1Changed.RemoveAll(TeamCharSlots[0].Get());
			DocRaid->OnPCShield1Changed.RemoveAll(TeamCharSlots[0].Get());
			DocRaid->OnRaidContribution1Changed.RemoveAll(TeamCharSlots[0].Get());
		}
		if (TeamCharSlots[1].IsValid()){
			DocRaid->OnCharName2Changed.RemoveAll(TeamCharSlots[1].Get());
			DocRaid->OnPCLevel2Changed.RemoveAll(TeamCharSlots[1].Get());
			DocRaid->OnPCHealth2Changed.RemoveAll(TeamCharSlots[1].Get());
			DocRaid->OnPCShield2Changed.RemoveAll(TeamCharSlots[1].Get());
			DocRaid->OnRaidContribution2Changed.RemoveAll(TeamCharSlots[1].Get());
		}
		if (TeamCharSlots[2].IsValid()){
			DocRaid->OnCharName3Changed.RemoveAll(TeamCharSlots[2].Get());
			DocRaid->OnPCLevel3Changed.RemoveAll(TeamCharSlots[2].Get());
			DocRaid->OnPCHealth3Changed.RemoveAll(TeamCharSlots[2].Get());
			DocRaid->OnPCShield3Changed.RemoveAll(TeamCharSlots[2].Get());
			DocRaid->OnRaidContribution3Changed.RemoveAll(TeamCharSlots[2].Get());
		}
	}
}

void UB2UIRaidBattle::DestroySelf(class UB2UIManager* InUIManager)
{
	for (auto& ThisSlot : TeamCharSlots)
		ThisSlot->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}

void UB2UIRaidBattle::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_BattleTime);
	GET_SLOT(UTextBlock, TB_MainContribution);
	GET_SLOT(UTextBlock, TB_ResurrectHelper);
	GET_SLOT(UTextBlock, TB_RaidStep);

	TeamCharSlots.SetNumUninitialized(3);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, TeamCharSlots[0], CharSlot1);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, TeamCharSlots[1], CharSlot2);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, TeamCharSlots[2], CharSlot3);

	for (auto& ThisSlot : TeamCharSlots)
	{
		if (ThisSlot.IsValid())
		{
			ThisSlot->Init();
			ThisSlot->FOnChangedHealthAndShieldCommon.BindUObject(this, &UB2UIRaidBattle::OnSlotChangedHealthAndShieldCommon);
			ThisSlot->FOnClickCharacter.BindUObject(this, &UB2UIRaidBattle::OnSlotChangedOnClickCharacter);
		}
	}

	GET_SLOT(UPanelWidget, CP_SkillSet0);
	GET_SLOT(UPanelWidget, CP_SkillSet1);
	GET_SLOT(UPanelWidget, CP_SkillSet2);
	GET_SLOT(UPanelWidget, CP_SkillSet3);
	GET_SLOT(UPanelWidget, CP_Attack);
	GET_SLOT(UPanelWidget, CP_Guard);

	GET_SLOT(UImage, IMG_SkillSetBG);
	GET_SLOT(UImage, IMG_TouchRing_AttackBtn);
	GET_SLOT(UImage, IMG_TouchRing_GuardBtn);
}

void UB2UIRaidBattle::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UIRaidBattle::BindDelegates()
{
	Super::BindDelegates();

	if (BTN_Pause.IsValid())
		BTN_Pause->OnPressed.RemoveAll(this);

	if (BTN_Pause2.IsValid())
		BTN_Pause2->OnPressed.RemoveAll(this);

	BIND_PRESS_FUNC_TO_BTN(BTN_Pause, &UB2UIRaidBattle::OnPressedRaidPauseBtn);
	BIND_PRESS_FUNC_TO_BTN(BTN_Pause2, &UB2UIRaidBattle::OnPressedRaidPauseBtn);
}

void UB2UIRaidBattle::CloseWidgetDelegate()
{
	OnPressedRaidPauseBtn();
}

void UB2UIRaidBattle::SetWidgetForRaidBattle()
{
	CurrentGameModeType = EB2GameMode::Raid;

	// 1. Close Control(Widget)
	SetVisibilityTagWidget(false);
	SetVisibilityGoldWidget(false);
	SetVisibilityPlayTimeWidget(false);
	SetVisibilityCRTInfoBGImage_01(false);

	// 2. Hide Control(Widget)
	if (UIP_BattleCombo.IsValid())
		UIP_BattleCombo->ForceHide();
	
	// 3. Remove/Delete Doc Delegate
	UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
	{
		DocBattle->SetIsTagPossible(false);
		DocBattle->SetIsTagAttackSuggested(false);
		DocBattle->SetIsTagSuggested(false);

		DocBattle->OnTagPCHealthChanged.RemoveAll(this);
		DocBattle->OnTagPCMaxHealthChanged.RemoveAll(this);
		DocBattle->OnIsCountingTagCooltimeChanged.RemoveAll(this);
		DocBattle->OnIsTagPossibleChanged.RemoveAll(this);
		DocBattle->OnIsTagDisabledByCooltimeChanged.RemoveAll(this);
		DocBattle->OnRemainingTagCooltimeChanged.RemoveAll(this);
		DocBattle->OnMaxCounterAttackPointChanged.RemoveAll(this);
		DocBattle->OnCounterAttackPointChanged.RemoveAll(this);
	}

	UB2UIDocBattleStage* DocBS = Cast<UB2UIDocBattleStage>(UB2UIDocHelper::GetDocBattleStage());
	if (DocBS)
		DocBS->OnStagePlayTimeChanged.RemoveAll(this);

	auto DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
		DocUser->OnGoldChanged.RemoveAll(this);

}

void UB2UIRaidBattle::SetEntry(int32 SlotIdx, EPCClass CharacterClass, int32 Lv)
{
	if (!TeamCharSlots.IsValidIndex(SlotIdx) || !TeamCharSlots[SlotIdx].IsValid())
		return;

	if (CharacterClass != EPCClass::EPC_End)
		TeamCharSlots[SlotIdx]->SetData(CharacterClass, Lv, true, SlotIdx);
}

void UB2UIRaidBattle::SetContributionValue(int32 SlotIdx, int32 ContributionValue)
{
	if (!TeamCharSlots.IsValidIndex(SlotIdx) || !TeamCharSlots[SlotIdx].IsValid())
		return;

	TeamCharSlots[SlotIdx]->SetContributionValue(ContributionValue);
}

void UB2UIRaidBattle::OnSlotChangedHealthAndShieldCommon(bool bMyTeam, int32 SlotIdx)
{
	UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (DocRaid)
	{
		if (TeamCharSlots.IsValidIndex(SlotIdx) && TeamCharSlots[SlotIdx].IsValid())
		{
			TeamCharSlots[SlotIdx]->SetHpAndShield(DocRaid->GetPCHealthByIndex(SlotIdx+1),
													DocRaid->GetPCMaxHealthByIndex(SlotIdx+1), 
													DocRaid->GetPCShieldByIndex(SlotIdx+1));
		}
	}
}

void UB2UIRaidBattle::OnSlotChangedOnClickCharacter(bool bMyTeam, int32 SlotIdx)
{

}

void UB2UIRaidBattle::InitEntryData()
{
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		for (int32 i = 0; i < 3; ++i)
		{
			EPCClass PCClass =  DocRaid->GetPCClassByIndex(i + 1);
			TeamCharSlots[i]->SetVisibility((PCClass != EPCClass::EPC_End) ? ESlateVisibility::Visible :ESlateVisibility::Hidden );			
			if (PCClass != EPCClass::EPC_End)
				SetEntry(i, DocRaid->GetPCClassByIndex(i + 1), DocRaid->GetPCLevelByIndex(i + 1));
		}

		if (TB_RaidStep.IsValid())
			TB_RaidStep->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("counterDungeon_Step")), FText::AsNumber(DocRaid->GetLastRaidStep())));
	}
}

void UB2UIRaidBattle::SetDisableSlot(int32 SlotIdx)
{
	if (!TeamCharSlots.IsValidIndex(SlotIdx) || !TeamCharSlots[SlotIdx].IsValid())
		return;

	TeamCharSlots[SlotIdx]->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIRaidBattle::OnChangedRaidBattleTime(class UB2UIDocBase* Sender, int32 CurrentTime, int32 PrevTime)
{
	if (TB_BattleTime.IsValid() == false)
		return;

	TB_BattleTime->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(CurrentTime, TEXT("m:ss"))));

	if (CurrentTime <= 10)
	{
		TB_BattleTime->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_NonSatisfy);
	}
	else
	{
		TB_BattleTime->SetColorAndOpacity(FSlateColor(FColor::White));
	}

	if (CurrentTime == 10)
	{
		PlayAnimationNotEnoughTime_IMP();
	}
}

void UB2UIRaidBattle::OnChangedCharName(class UB2UIDocBase* Sender, FText CurrentName, FText PrevName)
{
	SetNameMainChar(CurrentName);
}

void UB2UIRaidBattle::OnChangedMainPCContribution(class UB2UIDocBase* Sender, int32 CurContribution, int32 PrevContribution)
{
	if (TB_MainContribution.IsValid() == false)
		return;

	TB_MainContribution->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), CurContribution)));
}

void UB2UIRaidBattle::OnPressedRaidPauseBtn()
{
	// Raid의 경우 PauseButton 클릭시 게임이 멈추지 않는다.
	if (UGameplayStatics::IsGamePaused(GetOwningPlayer()) == false)
	{
		// Pause 메뉴 widget 생성. 여기서 post process DOF 설정을 하므로 이것이 적용되도록 하기 위해 실제 pause 는 한 틱 늦춘다.
		StartPauseMenuClass<EB2GameMode>::GetInstance().Signal(GetB2GameModeType(this)); // UIManager_InGameMenu 쪽에 등록한 StartPauseMenu 를 콜하게 될 것.
	}
	else
	{
		// 사실 Pause 상태에서는 다시 여기의 PuaseButton 을 누르는 것이 아니라 Pause 메뉴에서 Continue 버튼을 눌러야 돌아가게 될 것.
		UGameplayStatics::SetGamePaused(GetOwningPlayer(), false);
	}
}

void UB2UIRaidBattle::OnVisibleBtnForDefeat(class UB2UIDocBase* Sender, bool CurVisible, bool PrevVisible)
{
	if (CP_SkillSet0.IsValid())
		CP_SkillSet0->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (CP_SkillSet1.IsValid())
		CP_SkillSet1->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (CP_SkillSet2.IsValid())
		CP_SkillSet2->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (CP_SkillSet3.IsValid())
		CP_SkillSet3->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (CP_Attack.IsValid())
		CP_Attack->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (CP_Guard.IsValid())
		CP_Guard->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	
	if (UIP_AutoBattleIcon.IsValid())
	{
		UIP_AutoBattleIcon->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IMG_SkillSetBG.IsValid())
		IMG_SkillSetBG->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
/*
	if (IMG_TouchRing_AttackBtn.IsValid())
		IMG_TouchRing_AttackBtn->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (IMG_TouchRing_GuardBtn.IsValid())
		IMG_TouchRing_GuardBtn->SetVisibility(CurVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);*/
}

void UB2UIRaidBattle::SetResurrectHelperNotice(FText userName)
{
	FText Msg = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_Resurrection")), userName);
	if (TB_ResurrectHelper.IsValid())
		TB_ResurrectHelper->SetText(Msg);

	ShowResurrectHelperNotice_IMP();
}

void UB2UIRaidBattle::ShowResultImage(const ENetMatchResult& result)
{
	//[@AKI, 170821] [B2CLT-1888] 모드별 승패 연출 통일 개선
	PlayAnimationEvent_AnimResultAction(result);
}
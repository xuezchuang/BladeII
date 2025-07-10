// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIAdventureModSlot.h"
#include "B2UIManager.h"
#include "B2ChapterInfo.h"


void UB2UIAdventureModSlot::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UImage, IMG_Main);
	GET_SLOT(UImage, IMG_DisableMain);
	GET_SLOT(UTextBlock, TB_Desc);
	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UB2Button, BTN_Lock);
	GET_SLOT(UTextBlock, TB_Scenario);
	GET_SLOT(UTextBlock, TB_Scenario2);
	GET_SLOT(UTextBlock, TB_PlayScenario);
	GET_SLOT(UTextBlock, TB_PlayAdventure);
	GET_SLOT(UTextBlock, TB_Adventure);
	GET_SLOT(UTextBlock, TB_Adventure2);
	GET_SLOT(UOverlay, O_NoticeScenario);
	GET_SLOT(UOverlay, O_NoticeScenarioComplete);
	GET_SLOT(UOverlay, O_NoticeAdventure);
	GET_SLOT(UOverlay, O_NoticeAdventureDifficulty);
	GET_SLOT(UOverlay, O_Lock);
	GET_SLOT(UB2Button, BTN_StageDifficulty);
	GET_SLOT(UB2Image, IMG_StageDifficulty);
	GET_SLOT(UTextBlock, TB_StageDifficulty);
}

void UB2UIAdventureModSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_StageDifficulty, &UB2UIAdventureModSlot::OnClickBTN_Sub);
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIAdventureModSlot::OnClickBTN_Select);
	BIND_CLICK_FUNC_TO_BTN(BTN_Lock, &UB2UIAdventureModSlot::OnClickBTN_Lock);
}

void UB2UIAdventureModSlot::Init()
{
	Super::Init();

	if (TB_PlayAdventure.IsValid())
		TB_PlayAdventure->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AdventureMod_PlayAdventure"))));

	if (TB_PlayScenario.IsValid())
		TB_PlayScenario->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AdventureMod_PlayScenario"))));

	if (TB_Scenario2.IsValid())
		TB_Scenario2->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_InProgress"))));

	if (TB_Adventure2.IsValid())
		TB_Adventure2->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AdventureMod_ClearConditionMessage"))));

	SetLockSlot(false);
	SetIsCompleteScenarioMode(false);
	SetVisiblityAdventureDifficultyButton(false);

}

void UB2UIAdventureModSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIAdventureModSlot::OnClickBTN_Sub()
{
	FOnClickBTNSub.ExecuteIfBound(CurAdventureModeType);
}

void UB2UIAdventureModSlot::OnClickBTN_Lock()
{
	FOnClickBTNLock.ExecuteIfBound(CurAdventureModeType);
}

void UB2UIAdventureModSlot::OnClickBTN_Select()
{
	FOnClickBTNSelect.ExecuteIfBound(CurAdventureModeType);
}

void UB2UIAdventureModSlot::SetAdventureModeType(EAdventureModeType InAdventureModeType)
{
	CurAdventureModeType = InAdventureModeType;

	SetAdventureModeType_BP(InAdventureModeType);
}

void UB2UIAdventureModSlot::SetAdventureModeInfo(FAdventureModeSetting InAdventureModeInfo)
{
	SetAdventureModeType(InAdventureModeInfo.ModeType);

	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), InAdventureModeInfo.ModeTitle_GeneralTextKey.ToString()));
	
	if (TB_Desc.IsValid())
		TB_Desc->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), InAdventureModeInfo.ModeDesc_GeneralTextKey.ToString()));

	if (IMG_Main.IsValid())
		IMG_Main->SetBrushFromMaterial(InAdventureModeInfo.MainImageMaterial);

	if (IMG_DisableMain.IsValid())
		IMG_DisableMain->SetBrushFromMaterial(InAdventureModeInfo.DisableMainImageMaterial);
}

void UB2UIAdventureModSlot::SetNoticeText(EAdventureModeType InAdventureModeType, int32 ChapterNum, int32 SubChapterNum)
{
	if (InAdventureModeType == EAdventureModeType::Tutorial)
		return;

	auto TextWeekPtr = (InAdventureModeType == EAdventureModeType::Scenario) ? TB_Scenario : TB_Adventure;
	if (TextWeekPtr.IsValid() == false)
		return;

	if (ChapterNum == 0) ChapterNum = 1;
	if (SubChapterNum == 0) SubChapterNum = 1;
	
	TextWeekPtr->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_n-n")), FText::AsNumber(ChapterNum), FText::AsNumber(SubChapterNum)));
}

void UB2UIAdventureModSlot::SetIsCompleteScenarioMode(bool IsComplete)
{
	if (O_NoticeScenario.IsValid())
		O_NoticeScenario->SetVisibility(IsComplete ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (O_NoticeScenarioComplete.IsValid())
		O_NoticeScenarioComplete->SetVisibility(IsComplete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIAdventureModSlot::SetVisiblityAdventureDifficultyButton(bool IsVisbility)
{
	if (O_NoticeAdventure.IsValid())
		O_NoticeAdventure->SetVisibility(IsVisbility ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (O_NoticeAdventureDifficulty.IsValid())
		O_NoticeAdventureDifficulty->SetVisibility(IsVisbility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIAdventureModSlot::SetLockSlot(bool IsLock)
{
	if (O_Lock.IsValid())
		O_Lock->SetVisibility(IsLock ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_Main.IsValid())
		IMG_Main->SetVisibility(IsLock ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

	if (IMG_DisableMain.IsValid())
		IMG_DisableMain->SetVisibility(IsLock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UIAdventureModSlot::SetLastStageDifficulty(EStageDifficulty LastStageDifficulty)
{
	if (LastStageDifficulty <= EStageDifficulty::ESD_Normal)
		return;

	int32 BPIdx = ((int32)LastStageDifficulty - 2);

	if (IMG_StageDifficulty.IsValid() && AdventureDifficultyIcon.IsValidIndex(BPIdx))
		IMG_StageDifficulty->SetBrushFromMaterial(AdventureDifficultyIcon[BPIdx]);

	if (UB2ChapterInfo* ChapterInfoTable = StaticFindChapterInfo())
	{
		if (TB_StageDifficulty.IsValid())
			TB_StageDifficulty->SetText(ChapterInfoTable->GetLocalizedDifficultyName(LastStageDifficulty));
	}
}


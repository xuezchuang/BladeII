#include "B2UICounterAttackSelectCharacterSlot.h"
#include "B2UIDocHelper.h"
#include "BladeIILocText.h"

void UB2UICounterSlot::Init()
{
	Super::Init();

	SetImage();
	SetBestRecordVisibility(false);
}

void UB2UICounterSlot::CacheAssets()
{
	GET_SLOT(UOverlay, O_Lock);
	GET_SLOT(UOverlay, O_On);
	GET_SLOT(UOverlay, O_Off);

	GET_SLOT(UImage, IMG_Lock);
	GET_SLOT(UImage, IMG_On);
	GET_SLOT(UImage, IMG_Off);
	GET_SLOT(UB2Button, BTN_Select);
	

	GET_SLOT(UTextBlock, TB_Lv);
	GET_SLOT(UTextBlock, STB_RecommendStat);
	GET_SLOT(UTextBlock, TB_RecommendStat);

	GET_SLOT(UOverlay, O_BestRecord);
	GET_SLOT(UTextBlock, TB_BestRecord);
}

void UB2UICounterSlot::UpdateStaticText()
{
	if (STB_RecommendStat.IsValid())
		STB_RecommendStat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_RecomCombatPower")));

	if(TB_BestRecord.IsValid())
		TB_BestRecord->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_HighestRecord")));

}

void UB2UICounterSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UICounterSlot::OnClickSelect)
}

void UB2UICounterSlot::SetImage()
{
	if (IMG_Lock.IsValid())
		IMG_Lock->SetBrushFromMaterial(LockImage);

	if (IMG_On.IsValid())
		IMG_On->SetBrushFromMaterial(OnImage);

	if (IMG_Off.IsValid())
		IMG_Off->SetBrushFromMaterial(OffImage);
		
}

void UB2UICounterSlot::SetSlotLine(UB2UICounterSlotLine* SlotLine)
{
	UIP_SlotLine = SlotLine;

	if (UIP_SlotLine.IsValid())
		UIP_SlotLine->Init();
}

void UB2UICounterSlot::SetSlotState(EDungeonLevelState LevelState)
{
	bool IsLockLine = true;
	FSlateColor TextColor;

	if (EDungeonLevelState::EDLS_ON == LevelState)
	{
		SetSlotVisibilty(O_On.Get());
		TextColor = OnColor;
	}
	else if (EDungeonLevelState::EDLS_OFF == LevelState)
	{
		SetSlotVisibilty(O_Off.Get());
		TextColor = OffColor;
	}
	else if (EDungeonLevelState::EDLS_LOCK == LevelState)
	{
		SetSlotVisibilty(O_Lock.Get());
		TextColor = LockColor;
		IsLockLine = false;
	}

	if (UIP_SlotLine.IsValid())
		UIP_SlotLine->SetLine(IsLockLine);

	if (TB_Lv.IsValid())
		TB_Lv->SetColorAndOpacity(TextColor);
}

void UB2UICounterSlot::SetSlotVisibilty(UOverlay* SlotPointer)
{
	if (O_Lock.IsValid())
		O_Lock->SetVisibility(SlotPointer == O_Lock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (O_On.IsValid())
		O_On->SetVisibility(SlotPointer == O_On ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (O_Off.IsValid())
		O_Off->SetVisibility(SlotPointer == O_Off ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UICounterSlot::SetDifficulty(int32 InDifficulty)
{
	DifficultyLv = InDifficulty;

	if (TB_Lv.IsValid())
		TB_Lv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("counterDungeon_Step")), FText::AsNumber(DifficultyLv)));
}

void UB2UICounterSlot::SetRecommendStat(int32 Combat, int32 InRecommendAttack, int32 InRecommendDefense)
{
	CurrentRecommandPower = Combat;
	RecommandAttack = InRecommendAttack;
	RecommandDefense = InRecommendDefense;

	if (TB_RecommendStat.IsValid())
	{
		TB_RecommendStat->SetText(FText::AsNumber(Combat));
	}
}

void UB2UICounterSlot::SetServerStageID(int32 InStageID)
{
	ServerStageID = InStageID;
}

void UB2UICounterSlot::SetBestRecordVisibility(bool InVisible)
{
	if (O_BestRecord.IsValid())
	{
		O_BestRecord->SetVisibility(InVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UICounterSlot::OnClickSelect()
{
	FOnClickDelegate.Execute(DifficultyLv, CurrentRecommandPower, RecommandAttack, RecommandDefense);
}

void UB2UICounterSlotLine::Init()
{
	Super::Init();
}

void UB2UICounterSlotLine::CacheAssets()
{
	GET_SLOT(UOverlay, O_On);
	GET_SLOT(UOverlay, O_Off);
}

void UB2UICounterSlotLine::UpdateStaticText()
{

}

void UB2UICounterSlotLine::BindDelegates()
{

}

void UB2UICounterSlotLine::SetLine(bool IsLine)
{
	if (O_Off.IsValid())
		O_Off->SetVisibility(IsLine ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	if (O_On.IsValid())
		O_On->SetVisibility(IsLine ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UICounterAttackSelectCharacterSlot::Init()
{
	Super::Init();
}

void UB2UICounterAttackSelectCharacterSlot::CacheAssets()
{
	Super::CacheAssets();
	//GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UOverlay, O_SelectOverlay);
	GET_SLOT(UButton, BTN_Select);

	IMG_DifficultyWidgets.AddUninitialized(6);
	GET_SLOT_BYNAME(UImage, IMG_DifficultyWidgets[0], IMG_Easy);
	GET_SLOT_BYNAME(UImage, IMG_DifficultyWidgets[1], IMG_Normal);
	GET_SLOT_BYNAME(UImage, IMG_DifficultyWidgets[2], IMG_Difficulty);
	GET_SLOT_BYNAME(UImage, IMG_DifficultyWidgets[3], IMG_Easy_1);
	GET_SLOT_BYNAME(UImage, IMG_DifficultyWidgets[4], IMG_Normal_1);
	GET_SLOT_BYNAME(UImage, IMG_DifficultyWidgets[5], IMG_Difficulty_1);
}

void UB2UICounterAttackSelectCharacterSlot::UpdateStaticText()
{

}

void UB2UICounterAttackSelectCharacterSlot::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UICounterAttackSelectCharacterSlot::OnClickButton)
}

void UB2UICounterAttackSelectCharacterSlot::OnClickButton()
{
	FOnClickDelegate.Execute(Difficulty, TotalAttack, TotalDefence, CurrentRecommandPower);
}

void UB2UICounterAttackSelectCharacterSlot::SettingDifficulty(EUIBattleDifficulty InDifficulty)
{
	Difficulty = static_cast<int32>(InDifficulty);
	EventDifficulty(Difficulty);
}

void UB2UICounterAttackSelectCharacterSlot::SettingDifficulty(int32 InDifficulty)
{
	Difficulty = InDifficulty - 1;
	EventDifficulty(Difficulty);
}

void UB2UICounterAttackSelectCharacterSlot::SetRecommendStat(int32 Combat)
{
	//TotalAttack = Att;
	//TotalDefence = Def;
	CurrentRecommandPower = Combat;
	//if (TB_CombatPower.IsValid())
		//TB_CombatPower->SetText(FText::AsNumber(Combat));
}

/*void UB2UICounterAttackSelectCharacterSlot::SetVisibliltySelectOverlay(bool IsVisible)
{
	if (O_SelectOverlay.IsValid())
		O_SelectOverlay->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}*/

void UB2UICounterAttackSelectCharacterSlot::SetEnableSlot_Raid(bool bEnalble, bool bVisibleRecomStat)
{

}

void UB2UIDungeonDifficulty::Init()
{
	Super::Init();

	SetCurrentDifficulty(1);			//趣矫葛甫 檬扁蔼
}

void UB2UIDungeonDifficulty::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_DifficultyLeft1);
	GET_SLOT(UB2Button, BTN_DifficultyLeft2);
	GET_SLOT(UB2Button, BTN_DifficultyRight1);
	GET_SLOT(UB2Button, BTN_DifficultyRight2);

	GET_SLOT(UTextBlock, TB_DifficultyDesc);
	GET_SLOT(UTextBlock, TB_Difficulty);
	GET_SLOT(UTextBlock, TB_Max);
}

void UB2UIDungeonDifficulty::UpdateStaticText()
{
	if (TB_DifficultyDesc.IsValid())
		TB_DifficultyDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("RaidMain_DifficultyChoice")));
	if (TB_Max.IsValid())
		TB_Max->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Maximum_ValueMark")));
}

void UB2UIDungeonDifficulty::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_DifficultyLeft1, &UB2UIDungeonDifficulty::OnClickLeft);
	BIND_CLICK_FUNC_TO_BTN(BTN_DifficultyLeft2, &UB2UIDungeonDifficulty::OnClickLeft);
	BIND_CLICK_FUNC_TO_BTN(BTN_DifficultyRight1, &UB2UIDungeonDifficulty::OnClickRight);
	BIND_CLICK_FUNC_TO_BTN(BTN_DifficultyRight2, &UB2UIDungeonDifficulty::OnClickRight);
}


void UB2UIDungeonDifficulty::SetDifficultyText(int32 DifficultyText)
{
	if (TB_Difficulty.IsValid())
		TB_Difficulty->SetText(FText::AsNumber(DifficultyText));
}

void UB2UIDungeonDifficulty::SetCurrentDifficulty(int32 InCurDifficulty)
{
	CurrentDifficulty = InCurDifficulty;
	DifficultyDelegate.ExecuteIfBound(CurrentDifficulty);
	SetDifficultyText(InCurDifficulty);

	bool IsMax = (CurrentDifficulty == MaxDifficulty);
	if (TB_Max.IsValid())
		TB_Max->SetVisibility(IsMax ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UIDungeonDifficulty::OnClickLeft()
{
	if (CurrentDifficulty <= 1)
		return;

	SetCurrentDifficulty(CurrentDifficulty - 1);
}

void UB2UIDungeonDifficulty::OnClickRight()
{
	if (CurrentDifficulty >= MaxDifficulty)
		return;

	SetCurrentDifficulty(CurrentDifficulty + 1);
}

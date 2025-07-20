// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIFairyMain.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUser.h"

#include "FairyManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIFairyStepIcon.h"
#include "B2UIFairyButton.h"
#include "B2UIFairyBlessSlot.h"
#include "B2UIFairyBlessPopup.h"
#include "B2UIFairyLevelUpPopup.h"
#include "B2UIFairyBlessConfirmPopup.h"
#include "B2Inventory.h"

void UB2UIFairyMain::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UGridPanel, X_BelssItemList);
	GET_SLOT(UB2Button, BTN_GoToInven);
	GET_SLOT(UTextBlock, STB_GoToInven);
	GET_SLOT(UB2Button, BTN_GoToLevelUp);
	GET_SLOT(UTextBlock, STB_GoToLevelUp);
	GET_SLOT(UB2Button, BTN_GoToBless);
	GET_SLOT(UTextBlock, STB_GoToBless);

	GET_SLOT(UTextBlock, TB_Title);

	GET_SLOT(UTextBlock, TB_FairyName); 
	GET_SLOT(UTextBlock, TB_FairyLevel);
	GET_SLOT(UTextBlock, TB_NextLevel); 
	GET_SLOT(UB2RichTextBlock, TB_NextOption);

	const int32 MaxStep = 10;
	FairyStepIconList.AddDefaulted(MaxStep);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[0], Step_1);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[1], Step_2);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[2], Step_3);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[3], Step_4);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[4], Step_5);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[5], Step_6);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[6], Step_7);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[7], Step_8);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[8], Step_9);
	GET_SLOT_BYNAME(UB2UIFairyStepIcon, FairyStepIconList[9], Step_10);
	for (int32 i = 0; i < MaxStep; ++i)
	{
		if (FairyStepIconList[i].IsValid())
		{
			FairyStepIconList[i]->Init();
		}
	}

	GET_SLOT(UB2UIFairyButton, BTN_Fairy_Niece);
	GET_SLOT(UB2UIFairyButton, BTN_Fairy_Pang); 
	GET_SLOT(UB2UIFairyButton, BTN_Fairy_Rio);
	if (BTN_Fairy_Niece.IsValid()) BTN_Fairy_Niece->Init();
	if (BTN_Fairy_Pang.IsValid()) BTN_Fairy_Pang->Init();
	if (BTN_Fairy_Rio.IsValid()) BTN_Fairy_Rio->Init();

	const int32 MaxSlot = 6;
	FairyBlessSlotList.AddDefaulted(MaxSlot);
	GET_SLOT_BYNAME(UB2UIFairyBlessSlot, FairyBlessSlotList[0], FairyBelss_1);
	GET_SLOT_BYNAME(UB2UIFairyBlessSlot, FairyBlessSlotList[1], FairyBelss_2);
	GET_SLOT_BYNAME(UB2UIFairyBlessSlot, FairyBlessSlotList[2], FairyBelss_3);
	GET_SLOT_BYNAME(UB2UIFairyBlessSlot, FairyBlessSlotList[3], FairyBelss_4);
	GET_SLOT_BYNAME(UB2UIFairyBlessSlot, FairyBlessSlotList[4], FairyBelss_5);
	GET_SLOT_BYNAME(UB2UIFairyBlessSlot, FairyBlessSlotList[5], FairyBelss_6);
	for (int32 i = 0; i < MaxSlot; ++i)
	{
		if (FairyBlessSlotList[i].IsValid())
		{
			FairyBlessSlotList[i]->Init();
		}
	}

	GET_SLOT(UB2UIFairyLevelUpPopup, UIP_FairyLevelUpPopup);
	if (UIP_FairyLevelUpPopup.IsValid())
		UIP_FairyLevelUpPopup->Init();

	GET_SLOT(UOverlay, P_DailyReward);
	GET_SLOT(UOverlay, P_DailyRewardOn);
	GET_SLOT(UB2Button, BTN_DailyReward);
}

void UB2UIFairyMain::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_GoToInven, &UB2UIFairyMain::OnClickBTN_GoToInven);
	BIND_CLICK_FUNC_TO_BTN(BTN_GoToLevelUp, &UB2UIFairyMain::OnClickBTN_GoToLevelUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_GoToBless, &UB2UIFairyMain::OnClickBTN_GoToBless);
	BIND_CLICK_FUNC_TO_BTN(BTN_DailyReward, &UB2UIFairyMain::OnClickBTN_DailyReward);

	if (BTN_Fairy_Niece.IsValid())
	{
		BTN_Fairy_Niece->SetFairyBtnHandler(FFairyButtonOnClick::CreateLambda([this](EFairyType Fairy) {
			OnChangedFairy(Fairy);
		}));
	}
	if (BTN_Fairy_Pang.IsValid())
	{
		BTN_Fairy_Pang->SetFairyBtnHandler(FFairyButtonOnClick::CreateLambda([this](EFairyType Fairy) {
			OnChangedFairy(Fairy);
		}));
	}
	if (BTN_Fairy_Rio.IsValid())
	{
		BTN_Fairy_Rio->SetFairyBtnHandler(FFairyButtonOnClick::CreateLambda([this](EFairyType Fairy) {
			OnChangedFairy(Fairy);
		}));
	}
}

void UB2UIFairyMain::OnChangedFairy(EFairyType Fairy)
{
	if(Fairy != CurrentFairyType)
		OnChangeFairyCharacterClass<EFairyType>::GetInstance().Signal(Fairy);

	CurrentFairyType = Fairy;
	CachedBlessSlotRowIndex = -1;

	RefreshFairyStatus();
}

void UB2UIFairyMain::Init()
{
	Super::Init();
	CachedBlessSlotRowIndex = -1;
	bLoopReceiveAutoLevelUp = false;
	CurrentFairyType = EFairyType::FAIRY_NIECE;
}

void UB2UIFairyMain::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);
	SubscribeEvent();
}

void UB2UIFairyMain::OnClose(bool RightNow)
{
	Super::OnClose(RightNow);
	// 烙矫内靛
	if (UIP_FairyLevelUpPopup.IsValid())
	{
		UIP_FairyLevelUpPopup->OnClose(RightNow);
	}
	UnSubscribeEvent();
}

void UB2UIFairyMain::SubscribeEvent()
{
	if (bSubscribed == false)
	{
		UnSubscribeEvent();
	
		Issues.Add(UpdateFairyStatusClass<>::GetInstance().Subscribe2(
			[this]()
		{
			RefreshFairyStatus();
			RefreshFairyGift();
		}
		));
		Issues.Add(UpdateFairyBlessClass<EFairyType>::GetInstance().Subscribe2(
			[this](EFairyType FairyType)
		{
			RefreshFairyStatus();
			OnReceivedBless();
		}
		));
		Issues.Add(UpdateFairyAutoLevelUpClass<bool>::GetInstance().Subscribe2(
			[this](bool IsRunning)
		{
			OnChangeAutoLevelUp(IsRunning);
		}
		));
	}

	bSubscribed = true;

	FairyManager::GetInstance().RequestGetFairy();
}

void UB2UIFairyMain::OnUpdateFairyBless(EFairyType FairyType)
{
	if (CachedFairyBlessList.IsValidIndex(CachedBlessSlotRowIndex) == false)
		return;

	FB2FairyBlessSlotInfo &SlotInfo = CachedFairyBlessList[CachedBlessSlotRowIndex];

	if (FairyBlessSlotList.IsValidIndex(CachedBlessSlotRowIndex))
	{
		FairyBlessSlotList[CachedBlessSlotRowIndex]->StartBlessAnimation();
	}

}

void UB2UIFairyMain::UnSubscribeEvent()
{
	Issues.Empty();

	bSubscribed = false;
}

void UB2UIFairyMain::UpdateStaticText()
{
	Super::UpdateStaticText();

	if(TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessSkill")));

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Fairy")));
}

void UB2UIFairyMain::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIFairyMain::OnClickBTN_GoToInven()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UIFairyMain::OnClickBTN_GoToLevelUp()
{
	if (UIP_FairyLevelUpPopup.IsValid())
	{
		if (UIP_FairyLevelUpPopup->GetVisibility() != ESlateVisibility::Visible)
		{
			UIP_FairyLevelUpPopup->SetVisibility(ESlateVisibility::Visible);
			UIP_FairyLevelUpPopup->OnOpen(true);
			UIP_FairyLevelUpPopup->SetFairyInfo(CurrentFairyType);
		}
		else
		{
			UIP_FairyLevelUpPopup->SetVisibility(ESlateVisibility::Hidden);
			UIP_FairyLevelUpPopup->OnClickBTN_Close();
		}
	}
}

void UB2UIFairyMain::OnClickBTN_GoToBless()
{
	if (CachedFairyBlessList.IsValidIndex(CachedBlessSlotRowIndex) == false)
		return;

	const FB2FairyBlessSlotInfo& SlotInfo = CachedFairyBlessList[CachedBlessSlotRowIndex];

	// 绵汗吝牢霸 乐栏搁 扑诀栏肺 拱绢航
	FB2FairyStatusInfo StatusInfo;
	if (FairyManager::GetInstance().GetFairyInfo(SlotInfo.FairyType, StatusInfo) && StatusInfo.ActivedOptoinId > 0)
	{
		if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
		{
			UB2UIFairyBlessConfirmPopup *BlessPopup = pUIManagerInstance->OpenUI<UB2UIFairyBlessConfirmPopup>(UIFName::FairyBlessConfirmPopup);
			BlessPopup->SetBlessOption(SlotInfo.OptionId, SlotInfo.OptionValue);
			BlessPopup->SetConfirmBtnHandler(
				FFairyBlessConfirmOnClick::CreateLambda([SlotInfo]() {
				FairyManager::GetInstance().RequestBlessFairy(SlotInfo.FairyType, SlotInfo.OptionId);
			}));
		}
	}
	else
	{
		FairyManager::GetInstance().RequestBlessFairy(SlotInfo.FairyType, SlotInfo.OptionId);
	}	
}

void UB2UIFairyMain::OnClickBTN_DailyReward()
{
	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_GiftInfo")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);
}

void UB2UIFairyMain::RefreshFairyStatus()
{
	RefreshFairyBlessInfo();
	RefreshSelectedFairyInfo();
	RefreshFairyButtons();
	RefreshBottomButtons();
}

void UB2UIFairyMain::RefreshFairyGift()
{
	FairyManager &Instance = FairyManager::GetInstance();

	if (P_DailyReward.IsValid() && P_DailyRewardOn.IsValid())
	{
		const bool OpendFairyRio = Instance.GetIsOpenedFairy(EFairyType::FAIRY_RIO);

		P_DailyReward->SetVisibility(OpendFairyRio ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		if (OpendFairyRio)
		{
			const bool IsPossibleGetFairyGift = BladeIIGameImpl::GetClientDataStore().GetPossibleFairyGift();
			P_DailyRewardOn->SetVisibility(IsPossibleGetFairyGift ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
	}
}

void UB2UIFairyMain::RefreshFairyBlessInfo()
{
	//FairyBlessSlotList
	FairyManager &Instance = FairyManager::GetInstance();
	if (Instance.GetFairyBlessSlotInfoAll(CurrentFairyType, CachedFairyBlessList))
	{
		for (int32 i = 0; i < CachedFairyBlessList.Num(); ++i)
		{
			if (FairyBlessSlotList.IsValidIndex(i) && FairyBlessSlotList[i].IsValid())
			{
				FairyBlessSlotList[i]->SetBlessSlotInfo(i, CachedFairyBlessList[i]);
				FairyBlessSlotList[i]->SetFairyBlessSlotHandler(
					FFairyBlessSlotOnClick::CreateLambda([this](int32 RowIndex) {
						OnFairyBlessSlotCallback(RowIndex);
				}));
				FairyBlessSlotList[i]->SetFairyBlessIconHandler(
					FFairyBlessIconOnClick::CreateLambda([this](int32 RowIndex) {
					OnFairyBlessSlotIconCallback(RowIndex);
				}));
			}
		}
	}

}

void UB2UIFairyMain::OnFairyBlessSlotIconCallback(int32 RowIndex)
{
	UE_LOG(LogBladeII, Log, TEXT("@@@@@@@@@@@ OnFairyBlessSlotIconCallback >> %d"), RowIndex);
	if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
	{
		UB2UIFairyBlessPopup *BlessPopup = pUIManagerInstance->OpenUI<UB2UIFairyBlessPopup>(UIFName::FairyBlessPopup);

		FB2FairyBlessSlotInfo &SlotInfo = CachedFairyBlessList[RowIndex];
		BlessPopup->SetBlessInfo(SlotInfo.FairyType, SlotInfo.OptionId);
	}
}

void UB2UIFairyMain::OnFairyBlessSlotCallback(int32 RowIndex)
{
	CachedBlessSlotRowIndex = RowIndex;
	UE_LOG(LogBladeII, Log, TEXT("@@@@@@@@@@@ OnFairyBlessSlotCallback >> %d"), RowIndex);
	FB2FairyBlessSlotInfo &SlotInfo = CachedFairyBlessList[RowIndex];

	for (int32 i = 0; i < FairyBlessSlotList.Num(); ++i)
	{
		FairyBlessSlotList[i]->SetSlotStaus(EFairyBlessSlotStatus::Type::Disabled);
	}

	if (FairyBlessSlotList.IsValidIndex(RowIndex) && FairyBlessSlotList[RowIndex].IsValid())
	{
		FairyBlessSlotList[RowIndex]->SetSlotStaus(EFairyBlessSlotStatus::Type::Selected);
	}

	RefreshBottomButtons();
}

void UB2UIFairyMain::OnReceivedBless()
{
	if (FairyBlessSlotList.IsValidIndex(CachedBlessSlotRowIndex) && FairyBlessSlotList[CachedBlessSlotRowIndex].IsValid())
	{
		FairyBlessSlotList[CachedBlessSlotRowIndex]->StartBlessAnimation();
	}
}

////////////////////////////////////////////////////////////////////////////////
void UB2UIFairyMain::RefreshSelectedFairyInfo()
{
	FairyManager &Instance = FairyManager::GetInstance();

	FB2FairyStatusInfo StatusInfo;
	if (Instance.GetFairyInfo(CurrentFairyType, StatusInfo))
	{
		if (TB_FairyName.IsValid())
			TB_FairyName->SetText(Instance.GetFairyName(CurrentFairyType));

		if (TB_FairyLevel.IsValid())
			TB_FairyLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(StatusInfo.Level))));
		
		if (TB_NextLevel.IsValid())
			TB_NextLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SpecialOptionTitle")), FText::AsNumber(StatusInfo.Level + 1)));
		
		const int32 MaxCount = 10;
		if (StatusInfo.Level >= StatusInfo.MaxLevel)
		{
			if (TB_NextLevel.IsValid())
				TB_NextLevel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_NextLevel")));

			if (TB_NextOption.IsValid())
				TB_NextOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_MaxLevel")));
		}
		else
		{
			FText OptionName;
			FMD_FairyInfo NextFairyInfo;
			if (Instance.GetFairyTargetBlessInfo(StatusInfo.FairyType, StatusInfo.Level, NextFairyInfo))
			{
				OptionName = Instance.GetBlessOptionName(NextFairyInfo.OptionId);
			}
			
			if (TB_NextOption.IsValid())
				TB_NextOption->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessEnhance")), OptionName));
		}
	}

	UpdateLevelStepIcons();
}

void UB2UIFairyMain::UpdateLevelStepIcons()
{
	FB2FairyStatusInfo StatusInfo;
	if (FairyManager::GetInstance().GetFairyInfo(CurrentFairyType, StatusInfo))
	{
		const int32 MaxCount = 10;
		if (StatusInfo.Level >= StatusInfo.MaxLevel)
		{
			for (int32 i = 0; i < MaxCount; ++i)
			{
				if (FairyStepIconList.IsValidIndex(i))
					FairyStepIconList[i]->SetStatus(EFairyStepIconStatus::Type::LEVEL_MAX);
			}
		}
		else
		{
			for (int32 i = 0; i < StatusInfo.EnhancePoint; ++i)
			{
				if (FairyStepIconList.IsValidIndex(i))
					FairyStepIconList[i]->SetStatus(EFairyStepIconStatus::Type::ON);
			}

			for (int32 i = StatusInfo.EnhancePoint; i < MaxCount; ++i)
			{
				if (FairyStepIconList.IsValidIndex(i))
					FairyStepIconList[i]->SetStatus(EFairyStepIconStatus::Type::OFF);
			}
		}
	}
}

void UB2UIFairyMain::RefreshFairyButtons()
{
	FairyManager &Instance = FairyManager::GetInstance();
	for (int32 i = CliToSvrFairyType(EFairyType::FAIRY_NIECE); i < CliToSvrFairyType(EFairyType::FAIRY_MAX); ++i)
	{
		EFairyType FairyType = SvrToCliFairyType(i);

		FB2FairyStatusInfo StatusInfo;
		if (Instance.GetFairyInfo(FairyType, StatusInfo))
		{
			switch (FairyType)
			{
			case EFairyType::FAIRY_NIECE:
				BTN_Fairy_Niece->SetFairyInfo(StatusInfo);
				break;
			case EFairyType::FAIRY_PANG:
				BTN_Fairy_Pang->SetFairyInfo(StatusInfo);
				break;
			case EFairyType::FAIRY_RIO:
				BTN_Fairy_Rio->SetFairyInfo(StatusInfo);
				break;
			}
		}
	}

	// 困俊辑 盎脚窍搁辑 促 厚劝己拳 矫难淋
	switch (CurrentFairyType)
	{
	case EFairyType::FAIRY_NIECE:
		if (BTN_Fairy_Niece.IsValid()) BTN_Fairy_Niece->SetFairyButtonStatus(EFairyButtonStatus::Type::ENABLED);
		break;
	case EFairyType::FAIRY_PANG:
		if (BTN_Fairy_Pang.IsValid()) BTN_Fairy_Pang->SetFairyButtonStatus(EFairyButtonStatus::Type::ENABLED);
		break;
	case EFairyType::FAIRY_RIO:
		if (BTN_Fairy_Rio.IsValid()) BTN_Fairy_Rio->SetFairyButtonStatus(EFairyButtonStatus::Type::ENABLED);
		break;
	}
}

void UB2UIFairyMain::RefreshBottomButtons()
{
	// 磊悼 饭骇诀 吝捞搁 公矫
	if (bLoopReceiveAutoLevelUp)
		return;

	FairyManager &Instance = FairyManager::GetInstance();

	if (BTN_GoToInven.IsValid())
		BTN_GoToInven->SetIsEnabled(true);

	if (BTN_GoToLevelUp.IsValid())
	{
		BTN_GoToLevelUp->SetIsEnabled(Instance.GetIsOpenedFairy(CurrentFairyType));
	}
	
	bool BlessEnabled = false;
	if (CachedFairyBlessList.IsValidIndex(CachedBlessSlotRowIndex))
	{
		BlessEnabled = !CachedFairyBlessList[CachedBlessSlotRowIndex].bEquiped;
	}
	if(BTN_GoToBless.IsValid())
		BTN_GoToBless->SetIsEnabled(CachedBlessSlotRowIndex > -1 && BlessEnabled);
}

void UB2UIFairyMain::OnChangeAutoLevelUp(bool IsRunning)
{
	bLoopReceiveAutoLevelUp = IsRunning;
	if (IsRunning)
	{
		if (BTN_GoToInven.IsValid())
			BTN_GoToInven->SetIsEnabled(false);
		if (BTN_GoToLevelUp.IsValid())
			BTN_GoToLevelUp->SetIsEnabled(false);
		if (BTN_GoToBless.IsValid())
			BTN_GoToBless->SetIsEnabled(false);
	}
	else
	{
		RefreshBottomButtons();
	}
}
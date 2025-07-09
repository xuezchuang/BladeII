// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIFairyLevelUpPopup.h"
#include "BladeIIGameImpl.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"
#include "FairyManager.h"
#include "B2LobbyInventory.h"
#include "B2UIFairyLevelUpResultPopup.h"
#include "B2UIFairyStepIcon.h"
#include "B2UIClientIndicator.h"
#include "B2UIHeader.h"
#include "Commonstruct.h"
#include "B2ServerResultCodeTable.h"

void UB2UIFairyLevelUpPopup::Init()
{
	Super::Init();

	RequestTime = 0.0f;
	ResponseTime = 0.0f;
	CachedEnhancePoint = 0;
}

void UB2UIFairyLevelUpPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIFairyLevelUpPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close); 
	GET_SLOT(UB2Button, BTN_AutoLevelUp); 
	GET_SLOT(UB2Button, BTN_LevelUp);

	GET_SLOT(UImage, IMG_MaterialIcon);
	GET_SLOT(UTextBlock, TB_MaterialInfo); 

	GET_SLOT(UB2RichTextBlock, RTB_Condition);

	GET_SLOT(UTextBlock, TB_PrevewLevel); 
	GET_SLOT(UB2RichTextBlock, RTB_PreviewInfo);

	GET_SLOT(UTextBlock, TB_Notice);
	GET_SLOT(UTextBlock, TB_SubTitle);

	GET_SLOT(UTextBlock, TB_AutoLevelUp); 
	GET_SLOT(UTextBlock, TB_LevelUp); 
	GET_SLOT(UTextBlock, TB_LevelUpCostNum);

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
}

void UB2UIFairyLevelUpPopup::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_Notice.IsValid())
		TB_Notice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_LevelUp")));
	if (TB_SubTitle.IsValid())
		TB_SubTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_NextLevel")));

	if (TB_AutoLevelUp.IsValid())
		TB_AutoLevelUp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_LevelUpAuto")));
	if (TB_LevelUp.IsValid())
		TB_LevelUp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_LevelUp")));
}

void UB2UIFairyLevelUpPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIFairyLevelUpPopup::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_AutoLevelUp, &UB2UIFairyLevelUpPopup::OnClickBTN_AutoLevelUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_LevelUp, &UB2UIFairyLevelUpPopup::OnClickBTN_LevelUp);
}

void UB2UIFairyLevelUpPopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UIFairyLevelUpPopup::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);

	OnOpen_BP();

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);

	SubscribeEvent();
}

void UB2UIFairyLevelUpPopup::OnClose(bool RightNow)
{
	Super::OnClose(RightNow);

	if (bLoopReceiveAutoLevelUp)
	{
		StopAutoLevelUp();
	}

	OnClickBTN_Close();

	UnSubscribeEvent();
}

void UB2UIFairyLevelUpPopup::SubscribeEvent()
{
	if (bSubscribed == false)
	{
		UnSubscribeEvent();

		Issues.Add(UpdateFairyLevelUpClass<EFairyType, bool>::GetInstance().Subscribe2(
			[this](EFairyType FairyType, bool IsLevelUp)
		{
			OnLevelUpFairy(FairyType, IsLevelUp);
		}
		));
		
		Issues.Add(UpdateFairyLevelUpErrorClass<int32>::GetInstance().Subscribe2(
			[this](int32 ErrorCode)
		{
			OnErrorLevelUpFairy(ErrorCode);
		}
		)); 

		bSubscribed = true;
	}
}

void UB2UIFairyLevelUpPopup::UnSubscribeEvent()
{
	Issues.Empty();

	bSubscribed = false;
}

void UB2UIFairyLevelUpPopup::UpdateLevelUpButton()
{
	if (TB_AutoLevelUp.IsValid())
	{
		TB_AutoLevelUp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, bLoopReceiveAutoLevelUp ? TEXT("Fairy_LevelUpAutoCancel") : TEXT("Fairy_LevelUpAuto")));
	}

	if (bAnimationLevelUp)
	{
		if (BTN_AutoLevelUp.IsValid())
			BTN_AutoLevelUp->SetIsEnabled(bLoopReceiveAutoLevelUp);
		if (BTN_LevelUp.IsValid())
			BTN_LevelUp->SetIsEnabled(false);
	}
	else
	{
		FB2FairyStatusInfo Status;
		if (FairyManager::GetInstance().GetFairyInfo(CachedFairy, Status))
		{
			if (bLoopReceiveAutoLevelUp == false)
			{
				if (BTN_AutoLevelUp.IsValid())
					BTN_AutoLevelUp->SetIsEnabled(Status.Level < Status.MaxLevel);
				if (BTN_LevelUp.IsValid())
					BTN_LevelUp->SetIsEnabled(Status.Level < Status.MaxLevel);
			}
			else
			{
				if (BTN_AutoLevelUp.IsValid())
					BTN_AutoLevelUp->SetIsEnabled(true);
				if (BTN_LevelUp.IsValid())
					BTN_LevelUp->SetIsEnabled(false);
			}
		}

	}	
}

void UB2UIFairyLevelUpPopup::OnClickBTN_Close()
{
	if (bLoopReceiveAutoLevelUp)
	{
		StopAutoLevelUp();

		ShowLevelUpAutoCancelNotice();

		UpdateHeaderState(!bLoopReceiveAutoLevelUp);
		UpdateLevelUpButton();
	}
	else
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

		SetVisibility(ESlateVisibility::Hidden);
		//CloseSelf();
	}
}

void UB2UIFairyLevelUpPopup::ShowLevelMaxNotice()
{
	ShowSimplePopup(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_LevelUpAutoCancelNotice")));
}

void UB2UIFairyLevelUpPopup::ShowLevelUpAutoCancelNotice()
{
	ShowSimplePopup(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_LevelUpAutoCancelNotice")));
}

void UB2UIFairyLevelUpPopup::ShowSimplePopup(FText MessageText)
{
	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		MessageText,
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);
}

void UB2UIFairyLevelUpPopup::OnClickBTN_AutoLevelUp()
{
	if (bLoopReceiveAutoLevelUp == false)
	{
		bLoopReceiveAutoLevelUp = true;
		UpdateFairyAutoLevelUpClass<bool>::GetInstance().Signal(true);

		OnClickBTN_LevelUp();
	}
	else
	{
		StopAutoLevelUp();
		ShowLevelUpAutoCancelNotice();
	}

	UpdateHeaderState(!bLoopReceiveAutoLevelUp);
	UpdateLevelUpButton();
}

void UB2UIFairyLevelUpPopup::UpdateHeaderState(bool NewState)
{
	if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
	{
		UB2UIHeader *UIHeader = pUIManagerInstance->OpenUI<UB2UIHeader>(UIFName::Header);
		if (UIHeader)
		{
			bool IsBlocking = !NewState;
			UIHeader->SetForcedBlockHeader(IsBlocking, IsBlocking ? FLinearColor(0.0f, 0.0f, 0.0f, 0.0f) : FLinearColor(0.0f, 0.0f, 0.0f, 0.7f));
		}
	}
}

void UB2UIFairyLevelUpPopup::OnClickBTN_LevelUp()
{
	//if (IsEnoughMaterial() == false)
	//{
	//	StopAutoLevelUp();
	//	HandleServerErrorGoodsShortageClass<const uint32, const EGoodsButtonType>::GetInstance().Signal(FItemRefIDHelper::GetGoodsID_Dismantle(), EGoodsButtonType::EGoodsButtonType_ShortageShortcut);
	//	UpdateHeaderState(!bLoopReceiveAutoLevelUp);
	//	UpdateLevelUpButton();
	//	return;
	//}

	//if (IsMaxLevel() == false)
	//{
	//	StopAutoLevelUp();
	//	ShowLevelMaxNotice();
	//	UpdateHeaderState(!bLoopReceiveAutoLevelUp);
	//	UpdateLevelUpButton();
	//	return;
	//}

	//if (GetWorld())
	//	RequestTime = GetWorld()->GetTimeSeconds();
	//FairyManager::GetInstance().RequestLevelUpFairy(CachedFairy); 
}

bool UB2UIFairyLevelUpPopup::IsMaxLevel()
{
	FB2FairyStatusInfo Status;
	if (FairyManager::GetInstance().GetFairyInfo(CachedFairy, Status))
	{
		if (Status.Level < Status.MaxLevel)
			return true;
	}
	return false;
}

bool UB2UIFairyLevelUpPopup::IsEnoughMaterial()
{
	FB2FairyStatusInfo Status;
	if (FairyManager::GetInstance().GetFairyInfo(CachedFairy, Status))
	{
		int32 NeedCost = FairyManager::GetInstance().GetFairyLevelUpCost(Status.Level);
		int32 DecompositionItem = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_DISMANTLE_ITEM);

		if (NeedCost <= DecompositionItem)
			return true;
	}
	return false;
}

bool UB2UIFairyLevelUpPopup::CheckAutoLevelUp()
{
	if (bLoopReceiveAutoLevelUp)
	{
		if (IsMaxLevel() == false)
			return true;
		if (IsEnoughMaterial() == true)
			return true;
	}

	return false;
}

void UB2UIFairyLevelUpPopup::StopAutoLevelUp()
{
	bLoopReceiveAutoLevelUp = false;
	bAnimationLevelUp = false;

	UpdateLevelUpButton();
	
	UpdateFairyAutoLevelUpClass<bool>::GetInstance().Signal(false);

}

void UB2UIFairyLevelUpPopup::OnLevelUpFairy(EFairyType Fairy, bool IsLevelUp)
{
	if (GetWorld())
	{
		ResponseTime = GetWorld()->GetTimeSeconds();
	}

	CachedIsLevelUp = IsLevelUp;
	bAnimationLevelUp = true;

	UpdateLevelUpButton();
	PlayLevelUpAnimation();
}

void UB2UIFairyLevelUpPopup::OnErrorLevelUpFairy(int32 ErrorCode)
{
	StopAutoLevelUp();
	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		switch (ErrorCode)
		{
		case B2ResultCode::FAILURE_NEED_MORE_ITEM_AMOUNT:
		case B2ResultCode::FAILURE_ITEM_AMOUNT_UNDERFLOW:
			//HandleServerErrorGoodsShortageClass<const uint32, const EGoodsButtonType>::GetInstance().Signal(FItemRefIDHelper::GetGoodsID_Dismantle(), EGoodsButtonType::EGoodsButtonType_ShortageShortcut);
			break;
		default:
			pUIManager->OpenMsgPopupFromErrorCode(ErrorCode);
			break;
		}
		UpdateHeaderState(true);
	}

}

void UB2UIFairyLevelUpPopup::SetFairyInfo(EFairyType Fairy)
{
	CachedFairy = Fairy;

	FB2FairyStatusInfo Status;
	if (FairyManager::GetInstance().GetFairyInfo(Fairy, Status))
	{
		if (RTB_Condition.IsValid())
			RTB_Condition->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ProgressInfo")), FText::AsNumber(Status.Level), FText::AsNumber(Status.MaxLevel)));

		if (TB_PrevewLevel.IsValid())
		{
			TB_PrevewLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
			TB_PrevewLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SpecialOptionTitle")), FText::AsNumber(Status.Level + 1)));
		}
		
		FMD_FairyInfo FairyInfo;
		if (FairyManager::GetInstance().GetFairyNextBlessInfo(Fairy, Status.Level, FairyInfo))
		{
			EItemOption CurrOption = SvrToCliOptionType(FairyInfo.OptionId);
			FText OptionName = GetLOCTextOfItemOption(CurrOption, EPCClass::EPC_End);

			if (FairyManager::GetInstance().IsSameFairyBlessInfo(Fairy, Status.Level, Status.Level + 1))
			{
				if (RTB_PreviewInfo.IsValid())
					RTB_PreviewInfo->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessEnhance")), OptionName));
			}
			else
			{
				if (RTB_PreviewInfo.IsValid())
					RTB_PreviewInfo->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessUnlock")), OptionName));
			}
		}

		const int32 NeedCost = FairyManager::GetInstance().GetFairyLevelUpCost(Status.Level);
		const int32 DecompositionItem = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_DISMANTLE_ITEM);

		if (Status.Level >= Status.MaxLevel)
		{
			if (TB_PrevewLevel.IsValid())
				TB_PrevewLevel->SetVisibility(ESlateVisibility::Hidden);
			if (RTB_PreviewInfo.IsValid())
				RTB_PreviewInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_MaxLevel")));

			if (TB_LevelUpCostNum.IsValid())
			{
				TB_LevelUpCostNum->SetText(FText());
				TB_LevelUpCostNum->SetColorAndOpacity(FLinearColor::White);
			}
		}
		else
		{
			if (TB_LevelUpCostNum.IsValid())
			{
				FText CostText = FText::FromString(FString::FromInt(NeedCost));
				TB_LevelUpCostNum->SetText(CostText);
				TB_LevelUpCostNum->SetColorAndOpacity(DecompositionItem >= NeedCost ? FLinearColor::White : FLinearColor::Red);
			}
		}

		
		if (TB_MaterialInfo.IsValid())
		{
			TB_MaterialInfo->SetText(FText::AsNumber(DecompositionItem)); 
		}
	}

	UpdateLevelStepIcons();
	UpdateLevelUpButton();
}

void UB2UIFairyLevelUpPopup::UpdateLevelStepIcons()
{
	FB2FairyStatusInfo Status;
	if (FairyManager::GetInstance().GetFairyInfo(CachedFairy, Status))
	{
		const int32 MaxCount = 10;
		if (Status.Level >= Status.MaxLevel)
		{
			for (int32 i = 0; i < MaxCount; ++i)
			{
				if (FairyStepIconList.IsValidIndex(i))
					FairyStepIconList[i]->SetStatus(EFairyStepIconStatus::Type::LEVEL_MAX);
			}
		}
		else
		{
			for (int32 i = 0; i < Status.EnhancePoint; ++i)
			{
				if (FairyStepIconList.IsValidIndex(i))
					FairyStepIconList[i]->SetStatus(EFairyStepIconStatus::Type::ON);
				if (i > CachedEnhancePoint - 1)
					FairyStepIconList[i]->StartOnAnimation();
			}

			CachedEnhancePoint = Status.EnhancePoint;

			for (int32 i = Status.EnhancePoint; i < MaxCount; ++i)
			{
				if (FairyStepIconList.IsValidIndex(i))
					FairyStepIconList[i]->SetStatus(EFairyStepIconStatus::Type::OFF);
			}
		}

	}
}

void UB2UIFairyLevelUpPopup::OnFinishedLevelUpAnimation()
{
	UpdateFairyStatusClass<>::GetInstance().Signal();
	SetFairyInfo(CachedFairy);

	bAnimationLevelUp = false;

	if (bLoopReceiveAutoLevelUp == true)
	{
		if (CheckAutoLevelUp() == false)
		{
			StopAutoLevelUp();
			UpdateLevelUpButton();
			ShowLevelUpAutoCancelNotice();
			UpdateHeaderState(true);
			return;
		}
		else
		{
			if (CachedIsLevelUp == false)
			{
				float DelayTime = GetBackOffTime(RequestTime, ResponseTime);
				if (DelayTime <= 0)
				{
					OnClickBTN_LevelUp();
				}
				else
				{
					GetWorld()->GetTimerManager().SetTimer(LoopReceiveAutoLevelUpTH, this, &UB2UIFairyLevelUpPopup::OnClickBTN_LevelUp, DelayTime, false);
				}
			}
		}
	}
	else
	{
		UpdateLevelUpButton();
	}

	if (CachedIsLevelUp)
	{
		if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
		{
			UB2UIFairyLevelUpResultPopup *ResultPopup = pUIManagerInstance->OpenUI<UB2UIFairyLevelUpResultPopup>(UIFName::FairyLevelUpResultPopup);
			if (ResultPopup)
			{
				ResultPopup->ShowResult(CachedFairy, CachedIsLevelUp);
				ResultPopup->SetFinishedHandler(FOnFairyLevelUpResultFinished::CreateLambda([this]() {
					if (bLoopReceiveAutoLevelUp == true)
						OnClickBTN_LevelUp();
				}));
			}
		}
	}
}
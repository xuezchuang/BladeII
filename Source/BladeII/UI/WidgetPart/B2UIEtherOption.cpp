// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEtherOption.h"
#include "B2UIEtherOptionSlot.h"
#include "B2EtherManager.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "B2UIEtherInfoNotify.h"
#include "B2EtherScrollOptionSlot.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"


void FEtherOption::Init()
{
	if (IMG_Check.IsValid())
	{
		IMG_Check->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IMG_UnCheck.IsValid())
	{
		IMG_UnCheck->SetVisibility(ESlateVisibility::Visible);
	}

	if (PW_DefenceSet.IsValid())
	{
		PW_DefenceSet->SetVisibility(ESlateVisibility::Hidden);
	}
}

void FEtherOption::SetEnableSetOption(bool InEnable)
{
	if (RTB_DefenceSetInfo.IsValid())
	{
		RTB_DefenceSetInfo->SetIsEnabled(InEnable);
		RTB_DefenceSetInfo->SetText(GetEtherSetOptionColorText(EtherSetID, InEnable));
	}
}

void FEtherOption::SetInfo(int32 InSetID, EPCClass InPCClass)
{
	EtherSetID = InSetID;
	PCClass = InPCClass;

	if (PW_DefenceSet.IsValid())
	{
		PW_DefenceSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (RTB_DefenceSetInfo.IsValid())
	{
		RTB_DefenceSetInfo->SetText(GetEtherSetOptionColorText(EtherSetID,  true));
	}
}

void FEtherOption::MarkCheckBox()
{
	if (IMG_Check.IsValid())
	{
		IMG_Check->SetVisibility(ESlateVisibility::Visible);
	}

	if (IMG_UnCheck.IsValid())
	{
		IMG_UnCheck->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void FEtherOption::OnClick()
{
	data_trader::Retailer::GetInstance().RequestSelectAetherDefenseOption(EtherSetID, CliToSvrPCClassType(PCClass));
}

bool FEtherOption::IsChecked()
{
	bool Result = false;
	if (IMG_Check.IsValid())
	{
		Result = IMG_Check->GetVisibility() == ESlateVisibility::Visible;
	}
	return Result;
}

FText FEtherOption::GetEtherSetOptionColorText(int32 EtherSetId, bool IsFadeIn)
{
	FText Result = FText::GetEmpty();

	FString TextName = IsFadeIn ? FString(TEXT("Ether_SetOption_Color_1")) : FString(TEXT("Ether_SetOption_Color_2"));
	Result = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), TextName)
		, B2EtherManager::GetInstance().GetEtherSetOptionText(EtherSetId));

	return Result;
}

void UB2UIEtherOption::SetInfo(EPCClass InPCClass)
{
	PCClass = InPCClass;

	if (X_RTB_Title.IsValid())
	{ 
		X_RTB_Title->SetText(GetPCClassLocalizedText(PCClass));
	}
	
	TMap<EItemOption, float> Options;
	Options.Empty();

	FLocalCharacterData& CData = BladeIIGameImpl::GetLocalCharacterData();
	CData.GetAllEthers().GetEquippedEtherOption(Options, PCClass);

	for (int IDX = 0; IDX < UIP_EtherOptionSlots.Num(); ++IDX)
	{
		UIP_EtherOptionSlots[IDX]->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (X_SB_OptionScroll.IsValid())
	{
		X_SB_OptionScroll->SetScrollOffset(0);

		int IDX = 0;
		for (TPair<EItemOption, float>& Elem : Options)
		{
			UB2EtherScrollOptionSlot* NewItem;
			if (UIP_EtherOptionSlots.IsValidIndex(IDX))
			{
				NewItem = UIP_EtherOptionSlots[IDX].Get();
			}
			else
			{
				NewItem = CreateWidget<UB2EtherScrollOptionSlot>(GetWorld(), EtherOptionSlotClass);
				UIP_EtherOptionSlots.Add(NewItem);
			}
			X_SB_OptionScroll->AddChild(NewItem);
			NewItem->SetVisibility(ESlateVisibility::Visible);
			NewItem->Init();
			NewItem->SetInfo(Elem.Key, Elem.Value, PCClass);
			IDX++;
		}
	}

	Options.Empty();

	OffenseID = CData.GetEtherOffenseSetID(PCClass);
	
	if (X_H_OffenseSet.IsValid())
	{
		if (OffenseID == 0)
		{
			X_H_OffenseSet->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			X_H_OffenseSet->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (X_CB_OffenseSetInfo.IsValid())
			{
				X_CB_OffenseSetInfo->SetIsChecked(true);
			}
			if (X_RTB_OffenseSetInfo.IsValid())
			{
				X_RTB_OffenseSetInfo->SetText(B2EtherManager::GetInstance().GetEtherSetOptionText(OffenseID));
			}
		}
	}

	UpdateDefenseInfo();
}

#define SET_ETHER_OPTION(Index); \
GET_SLOT_BYNAME(UB2RichTextBlock, EtherOptions[Index].RTB_DefenceSetInfo, X_RTB_DefenceSetInfo##Index); \
GET_SLOT_BYNAME(UB2Image, EtherOptions[Index].IMG_Check, X_IMG_Check##Index); \
GET_SLOT_BYNAME(UB2Image, EtherOptions[Index].IMG_UnCheck, X_IMG_UnCheck##Index); \
GET_SLOT_BYNAME(UPanelWidget, EtherOptions[Index].PW_DefenceSet, X_PW_DefenceSet##Index); \
GET_SLOT_BYNAME(UB2Button, EtherOptions[Index].BTN_Select, X_BTN_DefenceSet##Index);\
GET_SLOT_BYNAME(UB2Button, EtherOptions[Index].BTN_Info, X_BTN_DefenceSetInfo##Index);

void UB2UIEtherOption::CacheAssets()
{
	EtherOptions.SetNum(MAX_DEFENCE_SET);
	SET_ETHER_OPTION(0);
	SET_ETHER_OPTION(1);
	SET_ETHER_OPTION(2);

	GET_SLOT(UB2ScrollBox, X_SB_OptionScroll);
	GET_SLOT(UB2RichTextBlock, X_RTB_Title);
	GET_SLOT(UB2RichTextBlock, X_RTB_SetOption);
	if (X_RTB_SetOption.IsValid())
	{
		X_RTB_SetOption->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_OptionName_2"))));
	}
	GET_SLOT(UB2RichTextBlock, X_RTB_NormalOption);
	if (X_RTB_NormalOption.IsValid())
	{
		X_RTB_NormalOption->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_OptionName_1"))));
	}
	GET_SLOT(UB2RichTextBlock, X_RTB_OffenseSet);
	if (X_RTB_OffenseSet.IsValid())
	{
		X_RTB_OffenseSet->SetText(FText::Format(FText::FromString(TEXT("<{0}>")), BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_1")))));
	}
	GET_SLOT(UB2RichTextBlock, X_RTB_OffenseSetInfo);
	GET_SLOT(UB2Button, X_BTN_OffenseSetInfo);
	GET_SLOT(UCheckBox, X_CB_OffenseSetInfo);
	GET_SLOT(UHorizontalBox, X_H_OffenseSet);
	GET_SLOT(UB2RichTextBlock, X_RTB_DefenceSet);
	if (X_RTB_DefenceSet.IsValid())
	{
		X_RTB_DefenceSet->SetText(FText::Format(FText::FromString(TEXT("<{0}>")), BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_2")))));
	}

	GET_SLOT(UB2Button, X_BTN_Help);
}

void UB2UIEtherOption::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (int i = 0; i < UIP_EtherOptionSlots.Num(); ++i)
	{
		UIP_EtherOptionSlots[i]->DestroySelf(InUIManager);
	}

	UIP_EtherOptionSlots.Empty(); 
	EtherOptions.Empty();

	DeliverySelectEtherDefenseOptionClass<FB2SelectEtherDefenseOptionPtr>::GetInstance().Unsubscribe(SelectEtherDefenseOptionEventID);
}

void UB2UIEtherOption::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(EtherOptions[0].BTN_Select, &UB2UIEtherOption::OnClickFirstDefInfo);
	BIND_CLICK_FUNC_TO_BTN(EtherOptions[1].BTN_Select, &UB2UIEtherOption::OnClickSecondDefInfo);
	BIND_CLICK_FUNC_TO_BTN(EtherOptions[2].BTN_Select, &UB2UIEtherOption::OnClickThirdDefInfo);

	BIND_CLICK_FUNC_TO_BTN(EtherOptions[0].BTN_Info, &UB2UIEtherOption::OnClickFirstDefSetInfo);
	BIND_CLICK_FUNC_TO_BTN(EtherOptions[1].BTN_Info, &UB2UIEtherOption::OnClickSecondDefSetInfo);
	BIND_CLICK_FUNC_TO_BTN(EtherOptions[2].BTN_Info, &UB2UIEtherOption::OnClickThirdDefSetInfo);

 	BIND_CLICK_FUNC_TO_BTN(X_BTN_OffenseSetInfo, &UB2UIEtherOption::OnClickAttSetInfo);
	
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Help, &UB2UIEtherOption::OnClickHelp);
	
	SelectEtherDefenseOptionEventID = DeliverySelectEtherDefenseOptionClass<FB2SelectEtherDefenseOptionPtr>::GetInstance().Subscribe([this](const FB2SelectEtherDefenseOptionPtr EtherPtr)
	{
		BladeIIGameImpl::GetLocalCharacterData().SetEtherDefenseSetID(this->PCClass, EtherPtr->selected_defense_set_option_id);
		this->UpdateDefenseInfo();
		UB2UIEtherInfoNotify* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherInfoNotify>(UIFName::EtherInfoNotify);
		if (Popup)
		{
			Popup->SetInfoNotify(FText::Format(FText::FromString(TEXT("{0} {1}")),
				B2EtherManager::GetInstance().GetEtherSetOptionText(EtherPtr->selected_defense_set_option_id),
				BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetOption_Activation_Notice")))));
		}
	});
}

void UB2UIEtherOption::SetEnableCheckText(const FEtherOption& InEtherOption)
{
	for (auto& EtherOption : EtherOptions)
	{
		if (auto IsSameSetOption = InEtherOption.EtherSetID == EtherOption.EtherSetID)
		{
			if(IsSameSetOption)
				EtherOption.OnClick();

			EtherOption.SetEnableSetOption(IsSameSetOption);
		}
	}
}

void UB2UIEtherOption::OnClickFirstDefInfo()
{
	if (EtherOptions[0].IsChecked() == false)
	{
		SetEnableCheckText(EtherOptions[0]);
	}
}

void UB2UIEtherOption::OnClickSecondDefInfo()
{
	if (EtherOptions[1].IsChecked() == false)
	{
		SetEnableCheckText(EtherOptions[1]);
	}
}

void UB2UIEtherOption::OnClickThirdDefInfo()
{
	if (EtherOptions[2].IsChecked() == false)
	{
		SetEnableCheckText(EtherOptions[2]);
	}
}

void UB2UIEtherOption::OnClickAttSetInfo()
{
	OpenEtherSetToolTipClass<int32, EPCClass>::GetInstance().Signal(OffenseID, PCClass);
}

void UB2UIEtherOption::OnClickFirstDefSetInfo()
{
	OpenEtherSetToolTipClass<int32, EPCClass>::GetInstance().Signal(EtherOptions[0].EtherSetID, EtherOptions[0].PCClass);
}

void UB2UIEtherOption::OnClickSecondDefSetInfo()
{
	OpenEtherSetToolTipClass<int32, EPCClass>::GetInstance().Signal(EtherOptions[1].EtherSetID, EtherOptions[1].PCClass);
}

void UB2UIEtherOption::OnClickThirdDefSetInfo()
{
	OpenEtherSetToolTipClass<int32, EPCClass>::GetInstance().Signal(EtherOptions[2].EtherSetID, EtherOptions[2].PCClass);
}

void UB2UIEtherOption::OnClickHelp()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::EtherHelp);
}

void UB2UIEtherOption::UpdateDefenseInfo()
{
	FLocalCharacterData& CData = BladeIIGameImpl::GetLocalCharacterData();

	int32 DefenceID = CData.GetEtherDefenceSetID(PCClass);

	for (int i = 0; i < MAX_DEFENCE_SET; ++i)
	{
		EtherOptions[i].Init();
	}
	
	TArray<int32> DefenseIDs = CData.GetAllEthers().GetDefenseSetIds(PCClass);
	for (int32 IDX = 0; IDX < DefenseIDs.Num(); ++IDX)
	{
		EtherOptions[IDX].SetInfo(DefenseIDs[IDX], PCClass);
		if (DefenseIDs[IDX] == DefenceID)
		{
			EtherOptions[IDX].MarkCheckBox();
		}
		else
		{
			EtherOptions[IDX].SetEnableSetOption(false);
		}
	}
}


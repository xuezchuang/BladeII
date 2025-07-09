// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIItemDetailOption.h"
#include "B2UICommonTab.h"
#include "B2UIManager.h"
#include "B2DynText_ItemDetailOption.h"
#include "B2Inventory.h"


UB2UIItemDetailOption::UB2UIItemDetailOption(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ItemOptions.Empty();
	ItemRandomOptions.Empty();
	IsEquip = false;
	ItemAllowedPCClass = 0;
	ItemStatIncColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));
	ItemStatSameColor = FSlateColor(FLinearColor(10.0f, 10.0f, 10.0f));
	ItemStatDecColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f));
}

void UB2UIItemDetailOption::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
	UpdateStaticText();
	BindDelegates();
}

void UB2UIItemDetailOption::DestroySelf()
{
	Super::DestroySelf();

	for (const TPair<int32, TWeakObjectPtr<UB2UICommonTab>>& Elem : UIP_Tabs)
	{
		if (Elem.Value.IsValid())
		{
			Elem.Value->DestroySelf(UB2UIManager::GetInstance());
		}
	}

	for (const TWeakObjectPtr<UB2DynText_ItemDetailOption>& Elem : OptionInfos)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf();
		}
	}
}

void UB2UIItemDetailOption::SetItemData(
	const FB2Item& InItem,
	const FB2Item& InSamePlaceItem,
	bool InIsEquip,
	FSlateColor InItemStatIncColor,
	FSlateColor InItemStatSameColor,
	FSlateColor InItemStatDecColor)
{
	IsEquip = InIsEquip;
	ItemAllowedPCClass = InItem.AllowedPCClass;
	ItemStatIncColor = InItemStatIncColor;
	ItemStatSameColor = InItemStatSameColor;
	ItemStatDecColor = InItemStatDecColor;

	SamePlaceItemOptions.Empty();
	SamePlaceItemOptions = InSamePlaceItem.IntrinsicOptions;

	SamePlaceItemRandomOptions.Empty();
	SamePlaceItemRandomOptions = InSamePlaceItem.RandomOptions;

	ItemOptions.Empty();
	ItemOptions = InItem.IntrinsicOptions;

	ItemRandomOptions.Empty();
	ItemRandomOptions = InItem.RandomOptions;

	SealOptions.Empty();
	SealOptions = InItem.SealOptions;

	SamePlaceItemSealOptions.Empty();
	for (FSealOption Elem : InSamePlaceItem.SealOptions)
	{
		SamePlaceItemSealOptions.Add(Elem.OptionInfo);
	}

	if (UIP_Tabs[OPTION].IsValid())
	{
		if (ItemOptions.Num() > 0)
		{
			UIP_Tabs[OPTION]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UIP_Tabs[OPTION]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UIP_Tabs[RANDOMOPTION].IsValid())
	{
		if (ItemRandomOptions.Num() > 0)
		{
			UIP_Tabs[RANDOMOPTION]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UIP_Tabs[RANDOMOPTION]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UIP_Tabs[SEAL].IsValid())
	{
		bool IsOpenSealTab = false;

		for (const FSealOption& Elem : SealOptions)
		{
			if (Elem.IsOpened == true)
			{
				IsOpenSealTab = true;
				break;
			}
		}

		if (IsOpenSealTab)
		{
			UIP_Tabs[SEAL]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UIP_Tabs[SEAL]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UIP_Tabs[OPTION].IsValid() && UIP_Tabs[OPTION]->GetVisibility() != ESlateVisibility::Collapsed)
	{
		OnClickOptionTab();
	}
	else if (UIP_Tabs[RANDOMOPTION].IsValid() && UIP_Tabs[RANDOMOPTION]->GetVisibility() != ESlateVisibility::Collapsed)
	{
		OnClickRandomOptionTab();
	}
	else if (UIP_Tabs[SEAL].IsValid() && UIP_Tabs[SEAL]->GetVisibility() != ESlateVisibility::Collapsed)
	{
		OnClickSealTab();
	}
}

void UB2UIItemDetailOption::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UScrollBox, OptionScroll);

	UIP_Tabs.Add(OPTION);
	GET_SLOT_BYNAME(UB2UICommonTab, UIP_Tabs[OPTION], UIP_OptionTab);

	UIP_Tabs.Add(RANDOMOPTION);
	GET_SLOT_BYNAME(UB2UICommonTab, UIP_Tabs[RANDOMOPTION], UIP_RandomOptionTab);

	UIP_Tabs.Add(SEAL);
	GET_SLOT_BYNAME(UB2UICommonTab, UIP_Tabs[SEAL], UIP_SealTab);
}

void UB2UIItemDetailOption::Init()
{
	for (const TPair<int32, TWeakObjectPtr<UB2UICommonTab>>& Elem : UIP_Tabs)
	{
		if (Elem.Value.IsValid())
		{
			Elem.Value->Init();
		}
	}
}

void UB2UIItemDetailOption::UpdateStaticText()
{
	if (UIP_Tabs[OPTION].IsValid())
	{
		UIP_Tabs[OPTION]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyItemDetail_IntrinsicOptionPanelTitle")));
	}

	if (UIP_Tabs[RANDOMOPTION].IsValid())
	{
		UIP_Tabs[RANDOMOPTION]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyItemDetail_RandOptionPanelTitle")));
	}

	if (UIP_Tabs[SEAL].IsValid())
	{
		UIP_Tabs[SEAL]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Seal")));
	}
}

void UB2UIItemDetailOption::BindDelegates()
{
	CAPTURE_UOBJECT(UB2UIItemDetailOption);
	UIP_Tabs[OPTION]->TabButtonDelegateBindLambda([Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->OnClickOptionTab();
		}
	});

	UIP_Tabs[RANDOMOPTION]->TabButtonDelegateBindLambda([Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->OnClickRandomOptionTab();
		}
	});

	UIP_Tabs[SEAL]->TabButtonDelegateBindLambda([Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->OnClickSealTab();
		}
	});
}

void UB2UIItemDetailOption::OnClickOptionTab()
{
	SelectTabs(OPTION);
	UpdateOptionInfos(ItemOptions, SamePlaceItemOptions);
}

void UB2UIItemDetailOption::OnClickRandomOptionTab()
{
	SelectTabs(RANDOMOPTION);
	UpdateOptionInfos(ItemRandomOptions, SamePlaceItemRandomOptions);
}

void UB2UIItemDetailOption::OnClickSealTab()
{
	SelectTabs(SEAL);
	UpdateOptionInfos(SealOptions, SamePlaceItemSealOptions);
}

void UB2UIItemDetailOption::SelectTabs(int32 InValue)
{
	for (const TPair<int32, TWeakObjectPtr<UB2UICommonTab>>& Elem : UIP_Tabs)
	{
		if (Elem.Value.IsValid())
		{
			if (Elem.Key == InValue)
			{
				Elem.Value->SetTabSelected(true);
			}
			else
			{
				Elem.Value->SetTabSelected(false);
			}
		}
	}
}

void UB2UIItemDetailOption::UpdateOptionInfos(const TArray<FItemOption>& InOptionInfos, const TArray<FItemOption>& InSameItemOptionInfos)
{
	OffUIOptionInfos();

	for (int i = 0; i < InOptionInfos.Num(); i++)
	{
		UpdateOptionInfo(i, InOptionInfos[i], InSameItemOptionInfos);
	}
}

void UB2UIItemDetailOption::UpdateOptionInfos(const TArray<FSealOption>& InOptionInfos, const TArray<FItemOption>& InSameItemOptionInfos)
{
	OffUIOptionInfos();

	for (int i = 0; i < InOptionInfos.Num(); i++)
	{
		if (InOptionInfos[i].IsOpened)
		{
			if (InOptionInfos[i].OptionInfo.OptionType != EItemOption::EIO_End)
			{
				UpdateOptionInfo(InOptionInfos[i].SealSlotNumber, InOptionInfos[i].OptionInfo, InSameItemOptionInfos);
			}
			else
			{
				EmptySealInfo(InOptionInfos[i].SealSlotNumber);
			}
		}
	}
}

void UB2UIItemDetailOption::UpdateOptionInfo(int32 InIndex, const FItemOption& InOptionInfo, const TArray<FItemOption>& InSameItemOptionInfos)
{
	bool IsSameOption = false;
	FItemOption SameOption;
	for (const FItemOption& Elem : InSameItemOptionInfos)
	{
		if (InOptionInfo.OptionType == Elem.OptionType)
		{
			SameOption = Elem;
			IsSameOption = true;
			break;
		}
	}

	UB2DynText_ItemDetailOption* NewItem = GetOptionSlot(InIndex);
	if (NewItem != nullptr)
	{
		NewItem->UpdateOptionTextSet(
			IsEquip,
			InIndex,
			ItemAllowedPCClassEnum(ItemAllowedPCClass),
			&InOptionInfo,
			IsSameOption ? &SameOption : NULL,
			ItemStatIncColor,
			ItemStatSameColor,
			ItemStatDecColor);
	}
}

void UB2UIItemDetailOption::EmptySealInfo(int32 InIndex)
{
	UB2DynText_ItemDetailOption* NewItem = GetOptionSlot(InIndex);
	if (NewItem != nullptr)
	{
		NewItem->UpdateOptionTextSet(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_SealSlot")));
	}
}

UB2DynText_ItemDetailOption* UB2UIItemDetailOption::GetOptionSlot(int32 InIndex)
{
	UB2DynText_ItemDetailOption* NewItem = nullptr;

	if (OptionInfos.IsValidIndex(InIndex))
	{
		NewItem = OptionInfos[InIndex].Get();
	}
	else
	{
		NewItem = CreateWidget<UB2DynText_ItemDetailOption>(GetWorld(), OptionInfoClass);
		OptionInfos.Add(NewItem);
	}

	if (NewItem != nullptr)
	{
		NewItem->SetVisibility(ESlateVisibility::Visible);

		if (OptionScroll.IsValid())
		{
			OptionScroll->AddChild(NewItem);
		}
	}

	return NewItem;
}

void UB2UIItemDetailOption::OffUIOptionInfos()
{
	for (const TWeakObjectPtr<UB2DynText_ItemDetailOption>& Elem : OptionInfos)
	{
		if (Elem.IsValid())
		{
			Elem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (OptionScroll.IsValid())
	{
		OptionScroll->SetScrollOffset(0);
	}
}

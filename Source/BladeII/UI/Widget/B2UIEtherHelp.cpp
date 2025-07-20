// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherHelp.h"
#include "B2UIManager.h"
#include "B2UIEtherHelpSlot.h"
#include "B2EtherInfo.h"
#include "B2EtherManager.h"
#include "B2GameDefine.h"
#include "BladeIIUtil.h"

void UB2UIEtherHelp::Init()
{
	Super::Init();

	if (X_SB_Scroll.IsValid())
	{
		X_SB_Scroll->SetScrollOffset(0);
		
		if (StaticFindEtherInfoTable() != NULL)
		{
			struct SlotInfo
			{
				int TypeID;
				int SetType;
				SlotInfo()
				{
					TypeID = 0;
					SetType = 0;
				}
			};

			TArray<SlotInfo> SlotInfos;
			TArray<int32> Types = StaticFindEtherInfoTable()->GetTypes();
			SlotInfos.SetNum(Types.Num());
			for (int i = 0; i < Types.Num(); ++i)
			{
				SlotInfos[i].TypeID = Types[i];
				b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(SlotInfos[i].TypeID, 1); //tier绰 1肺 劝侩
				if (SetOptionPtr != nullptr)
				{
					if (SetOptionPtr->min_apply_count == EtherSetCount::DEFENCE) //规绢 技飘
					{
						SlotInfos[i].SetType = 0;
					}
					else if (SetOptionPtr->min_apply_count == EtherSetCount::OFFENCE) // 傍拜 技飘
					{
						SlotInfos[i].SetType = 1;
					}
				}
			}

			SlotInfos.Sort([](const SlotInfo& A, const SlotInfo& B)
			{
				bool Result = false;
				if (A.TypeID != 0 && B.TypeID != 0)
				{
					if (A.SetType > B.SetType)
					{
						Result = true;
					}
					else if (A.SetType == B.SetType)
					{
						if (A.TypeID < B.TypeID)
						{
							Result = true;
						}
					}
				}
				return Result;
			});

			int MaxSlot = SlotInfos.Num();
			for (int i = 0; i < MaxSlot; i++)
			{
				UB2UIEtherHelpSlot* NewItem;
				NewItem = CreateWidget<UB2UIEtherHelpSlot>(GetWorld(), HelpSlotClass);
				NewItem->SetVisibility(ESlateVisibility::Visible);
				NewItem->Init();
				NewItem->SetInfo(SlotInfos[i].TypeID);

				UIP_HelpSlots.Add(NewItem);
				X_SB_Scroll->AddChild(NewItem);
			}
		}
	}
}

void UB2UIEtherHelp::CacheAssets()
{
	GET_SLOT(UB2Button, X_BTN_Close);

	GET_SLOT(UB2RichTextBlock, X_RTB_OffenseSet);
	if (X_RTB_OffenseSet.IsValid())
	{
		X_RTB_OffenseSet->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_1_Skin"))));
	}
	GET_SLOT(UB2RichTextBlock, X_RTB_OffenseSetDesc);
	if (X_RTB_OffenseSetDesc.IsValid())
	{
		X_RTB_OffenseSetDesc->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetOption_Definition_1"))));
	}

	GET_SLOT(UB2RichTextBlock, X_RTB_DefenceSet);
	if (X_RTB_DefenceSet.IsValid())
	{
		X_RTB_DefenceSet->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_2_Skin"))));
	}
	GET_SLOT(UB2RichTextBlock, X_RTB_DefenceSetDesc);
	if (X_RTB_DefenceSetDesc.IsValid())
	{
		X_RTB_DefenceSetDesc->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetOption_Definition_2"))));
	}

	GET_SLOT(UTextBlock, X_TB_Title);
	if (X_TB_Title.IsValid())
	{
		X_TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_OptionName_2"))));
	}

	GET_SLOT(UB2ScrollBox, X_SB_Scroll);
}

void UB2UIEtherHelp::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2UIEtherHelp::OnClickClose);
}

void UB2UIEtherHelp::DestroySelf(class UB2UIManager* InUIManager)
{
	for (int i = 0; i < UIP_HelpSlots.Num(); ++i)
	{
		UIP_HelpSlots[i]->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIEtherHelp::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::EtherHelp);
}

void UB2UIEtherHelp::CloseWidgetDelegate()
{
	OnClickClose();
}
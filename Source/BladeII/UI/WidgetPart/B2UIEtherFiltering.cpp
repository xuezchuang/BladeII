// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherFiltering.h"
#include "B2UIEtherFilteringSlot.h"
#include "B2UIEtherFilteringRow.h"
#include "B2UIEther.h"
#include "B2EtherManager.h"
#include "BladeIIUtil.h"
#include "B2EtherInfo.h"
#include "B2AndroidBackManager.h"
#include "B2GameDefine.h"


void UB2UIEtherFiltering::Init()
{
	Super::Init();

	TArray<int32> Types;

	if (StaticFindEtherInfoTable() != NULL)
	{
		Types = StaticFindEtherInfoTable()->GetTypes();

		SlotInfos.SetNum(Types.Num() + 1);
		SlotInfo All; // 모두 보기는 직접 넣어줌
		SlotInfos[0] = All;
		for (int i = 0; i < Types.Num(); ++i)
		{
			SlotInfos[i].TypeID = Types[i];
			b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(SlotInfos[i].TypeID, 1); //tier는 1로 활용
			if (SetOptionPtr != nullptr)
			{
				if (SetOptionPtr->min_apply_count == EtherSetCount::DEFENCE) //방어 세트
				{
					SlotInfos[i].SetType = 0;
				}
				else if (SetOptionPtr->min_apply_count == EtherSetCount::OFFENCE) // 공격 세트
				{
					SlotInfos[i].SetType = 1;
				}
			}
		}

		SlotInfos.Sort([](const SlotInfo& A, const SlotInfo& B)
		{
			bool Result = false;
			if (A.TypeID == 0)
			{
				Result = true;
			}
			else if(A.TypeID != 0 && B.TypeID != 0)
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
	}

	if (X_SB_Scroll.IsValid() && SlotInfos.Num() > 0)
	{
		X_SB_Scroll->SetScrollOffset(0);

		int MaxSlot = SlotInfos.Num();

		int MaxRow = 0;
		if (MaxSlot % FilTERING_MAX_SLOT > 0)
		{
			MaxRow = MaxSlot / FilTERING_MAX_SLOT + 1;
		}
		else
		{
			MaxRow = MaxSlot / FilTERING_MAX_SLOT;
		}

		for (int i = 0; i < MaxRow; i++)
		{
			UB2UIEtherFilteringRow* NewItem;
			if (UIP_FilteringSlots.IsValidIndex(i))
			{
				NewItem = UIP_FilteringSlots[i].Get();
			}
			else
			{
				NewItem = CreateWidget<UB2UIEtherFilteringRow>(GetWorld(), FilteringRowSlotClass);
				UIP_FilteringSlots.Add(NewItem);
			}

			NewItem->SetVisibility(ESlateVisibility::Visible);
			NewItem->Init();
			X_SB_Scroll->AddChild(NewItem);
		}
	}
}

void UB2UIEtherFiltering::SetReceiver(class UB2UIEther* InReceiver)
{
	if (X_SB_Scroll.IsValid())
	{
		for (int i = 0; i < UIP_FilteringSlots.Num(); ++i)
		{
			UIP_FilteringSlots[i]->SetReceiver(InReceiver, X_SB_Scroll.Get());
		}
	}
}

void UB2UIEtherFiltering::SetSlotInfo(bool IsIcon)
{
	for (int i = 0; i < UIP_FilteringSlots.Num(); ++i)
	{
		int MaxSlot = SlotInfos.Num();
		int StartIDX = i * FilTERING_MAX_SLOT;
		int EndIDX = ((i + 1) * FilTERING_MAX_SLOT) - 1 > (MaxSlot - 1) ? (MaxSlot - 1) : ((i + 1) * FilTERING_MAX_SLOT) - 1;
		
		TArray<int32> TypeIDs;

		for (int IDX = StartIDX; IDX <= EndIDX; ++IDX)
		{
			TypeIDs.Add(SlotInfos[IDX].TypeID);
		}

		if (IsIcon == true)
		{
			UIP_FilteringSlots[i]->SetSlotIcon(TypeIDs);
		}
		UIP_FilteringSlots[i]->SetSlotCount(TypeIDs);
	}
}

void UB2UIEtherFiltering::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (int i = 0; i < UIP_FilteringSlots.Num(); ++i)
	{
		UIP_FilteringSlots[i]->DestroySelf(InUIManager);
	}
}

void UB2UIEtherFiltering::CacheAssets()
{
	GET_SLOT(UB2Button, X_BTN_Close);
	GET_SLOT(UB2ScrollBox, X_SB_Scroll);
	GET_SLOT(UTextBlock, X_TB_Title);
	if (X_TB_Title.IsValid())
	{
		X_TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Filter"))));
	}
}

void UB2UIEtherFiltering::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2UIEtherFiltering::OnClickClose);
}

void UB2UIEtherFiltering::OnClickClose()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIEtherFiltering::CloseWidgetDelegate()
{
	OnClickClose();
}
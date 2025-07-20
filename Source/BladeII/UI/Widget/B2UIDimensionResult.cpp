// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIDimensionResult.h"
#include "B2UIManager.h"
#include "B2UIRewardEventSet.h"
#include "B2DimensionGameMode.h"
#include "B2UIDocDimension.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"



void UB2UIDimensionResult::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Ok);
	GET_SLOT(UTextBlock, TB_Step);
	GET_SLOT(UTextBlock, TB_RecordSec);

	GET_SLOT(UHorizontalBox, HB_Rewards);

	GET_SLOT(UTextBlock, STB_RecordSec);
	GET_SLOT(UTextBlock, STB_NewRecord);
	GET_SLOT(UTextBlock, STB_Reward);
	GET_SLOT(UTextBlock, STB_Ok);
}

void UB2UIDimensionResult::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Ok, &UB2UIDimensionResult::OnClickBTN_Ok);
}

void UB2UIDimensionResult::Init()
{
	Super::Init();

	auto* pDoc = UB2UIDocHelper::GetDocDimension();
	if (pDoc)
	{
		if (TB_Step.IsValid())
			TB_Step->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_NDifficulty")), pDoc->GetDifficulty()));
		if (TB_RecordSec.IsValid())
		{
			TB_RecordSec->SetText(FText::FromString(UB2UIManager::FormatMilliSecondsAsTime(pDoc->m_nCurrentRecordTimeMilliSecForResult, TEXT("hh:mm:ss.ms"))));
		}

		TArray<FDimensionReward> NewRewards = pDoc->GetClearRewards();

		if (HB_Rewards.IsValid())
		{
			HB_Rewards->ClearChildren();

			for (auto Item : NewRewards)
			{
				auto* CreatedWidget = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardItemClass);
				if (CreatedWidget != NULL)
				{
					FText Desc = Item.Count > 0 ? FText::AsNumber(Item.Count) : FText::GetEmpty();
					CreatedWidget->Init();
					CreatedWidget->UpdateRewardIconByItemIndex(Item.ItemIndex, Desc);
					HB_Rewards->AddChild(CreatedWidget);
				}
			}
		}
	}

	OnShowResult();
}

void UB2UIDimensionResult::DestroySelf(UB2UIManager* InUIManager)
{
	//DeliveryGetDimensionClass<FB2ResponseGetDimensionPtr>::GetInstance().Unsubscribe(DeliveryGetDimensionTicket);

	//Super::DestroySelf(InUIManager);
}

void UB2UIDimensionResult::OnClickBTN_Ok()
{
	//GoToMapClass<>::GetInstance().Signal();
}

void UB2UIDimensionResult::AnimEnd_End()
{
	bool NewRecord = false;
	auto* pDoc = UB2UIDocHelper::GetDocDimension();
	if (pDoc)
		NewRecord = pDoc->GetIsNewRecord();

	if (NewRecord)
		AnimPlay_NewRecord();
	else
		SetButtonEnable(true);
}

void UB2UIDimensionResult::AnimEnd_NewRecord()
{
	SetButtonEnable(true);
}

void UB2UIDimensionResult::SetButtonEnable(bool bEnable)
{
	if (BTN_Ok.IsValid())
	{
		BTN_Ok->SetIsEnabled(bEnable);
	}
}

void UB2UIDimensionResult::OnShowResult()
{
	//SetButtonEnable(false);

	//auto* GameMode = Cast<AB2DimensionGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (GameMode)
	//{
	//	GameMode->PlayVictoryBGM();
	//	AnimPlay_Open();
	//}
}

void UB2UIDimensionResult::CloseWidgetDelegate()
{
	if (BTN_Ok.IsValid())
	{
		if(BTN_Ok->GetIsEnabled())
			OnClickBTN_Ok();
	}
}

void UB2UIDimensionResult::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_RecordSec.IsValid())
		STB_RecordSec->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_ClearTime")));
	if (STB_NewRecord.IsValid())
		STB_NewRecord->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_NewRecord")));
	if (STB_Reward.IsValid())
		STB_Reward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_SubRewardTitle")));
	if (STB_Ok.IsValid())
		STB_Ok->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Quit")));
}
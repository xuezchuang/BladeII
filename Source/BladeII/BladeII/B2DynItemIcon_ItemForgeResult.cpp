// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "BladeIIUtil.h"
#include "UI/DJLegacy/B2DynItemIcon_ItemForge.h" 
#include "BladeIIGameImpl.h"
#include "B2UIItemForge.h"
#include "B2DynItemIcon_ItemForgeResult.h" 


void UB2DynItemIcon_ItemForgeResult::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2DynItemIcon_ItemForge, UIP_ResultItem);
	GET_SLOT(UB2RichTextBlock, TB_SuccessResult);
}

void UB2DynItemIcon_ItemForgeResult::BindDelegates()
{
	Super::BindDelegates();
}

void UB2DynItemIcon_ItemForgeResult::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2DynItemIcon_ItemForgeResult::Init()
{
	Super::Init();

	beenSkipped = false;
}

void UB2DynItemIcon_ItemForgeResult::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2DynItemIcon_ItemForgeResult::OnShowingResultAnimEnd()
{
	//if(beenSkipped==false)
		ItemForgeMainUI->ContinueShowingResult(resultType);
}

void UB2DynItemIcon_ItemForgeResult::SetOwnerItemForgeUI(class UB2UIItemForge* MainUI_, EItemForgeResultType resultType_, FB2Item item_, bool isSkipping, int32 actualQuantity)
{
	GET_SLOT(UB2DynItemIcon_ItemForge, UIP_ResultItem);
	GET_SLOT(UB2RichTextBlock, TB_SuccessResult);
	beenSkipped = isSkipping;
	ItemForgeMainUI = MainUI_;
	resultType = resultType_;

	if (item_.ItemRefID > 0)
	{
		if (UIP_ResultItem.IsValid())
		{
			UIP_ResultItem->Initialize();
			UIP_ResultItem->NativeConstruct();
			UIP_ResultItem->InitItemForgeSlot(EItemForgeSlot::MainMaterialItem, EItemForgeItemIconUsage::EIFI_TargetItem, item_.ItemRefID);

			UIP_ResultItem->UpdateTargetItemData(item_.ItemRefID, item_.ConsumingAmount == 0 ? 1 : actualQuantity);
		}
	}
	else
	{
		if (UIP_ResultItem.IsValid())
		{
			//UIP_ResultItem->SetVisibility(ESlateVisibility::Hidden);
			UIP_ResultItem->Initialize();
			UIP_ResultItem->NativeConstruct();
			UIP_ResultItem->ShowJustEmptySlot();
		}
	}

	switch (resultType_)
	{
	case EItemForgeResultType::Fail:
		if (TB_SuccessResult.IsValid())
		{
			TB_SuccessResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Fail")));
		}
		OnShowingResultFail_BP();
		break;
	case EItemForgeResultType::FailRefund:
		if (TB_SuccessResult.IsValid())
		{
			TB_SuccessResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_FailRefund")));
		}
		OnShowingResultFail_BP();
		break;
	case EItemForgeResultType::Success:
		if (TB_SuccessResult.IsValid())
		{
			TB_SuccessResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Success")));
		}
		OnShowingResultSuccess_BP();
		break;
	case EItemForgeResultType::HugeSuccess:
		if (TB_SuccessResult.IsValid())
		{
			TB_SuccessResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_HugeSuccess")));
		}
		OnShowingResultHugeSuccess_BP();
		break;
	case EItemForgeResultType::HugeSuccessReward:
		if (TB_SuccessResult.IsValid())
		{
			TB_SuccessResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_HugeSuccessReward")));
		}
		OnShowingResultHugeSuccess_BP();
		break;
	}
	/*
	if (beenSkipped == true)
	{
		ItemForgeMainUI->ContinueShowingResult(resultType);
	}
	*/
}

bool UB2DynItemIcon_ItemForgeResult::IsSkipping()
{
	return beenSkipped;
}
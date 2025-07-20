#include "B2UIRewardEventSet.h"
#include "B2UIManager.h"
#include "B2ClientDataStore.h"
#include "B2RewardInfo.h"
#include "B2UIDocHelper.h"
#include "B2ItemInfo.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"


UB2UIRewardIcon::UB2UIRewardIcon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// BP俊辑 汲沥沁栏搁 弊吧肺 酒捞能捞尔 捞抚 剁况霖促.
	RewardIdFromBP = -1;

	// RewardIdFromBP 汲沥 登菌栏搁 荐樊档 BP沥焊肺 剁况霖促. -1捞搁 救剁快绊
	RewardAmountFromBP = -1;
}

void UB2UIRewardIcon::Init()
{
	Super::Init();

	if (RewardIdFromBP != -1)
	{
		UpdateRewardIcon(RewardIdFromBP, RewardAmountFromBP != -1 ? FText::AsNumber(RewardAmountFromBP) : FText::FromString(TEXT("")));
	}

	if (TextSwitcher.IsValid())
		TextSwitcher->SetActiveWidgetIndex(0);

	SetGrade(0);
}

void UB2UIRewardIcon::CacheAssets()
{
	GET_SLOT(UImage, IMG_RewardMainIcon);
	GET_SLOT(UTextBlock, TB_RewardAmount);
	GET_SLOT(UWidgetSwitcher, TextSwitcher);
	GET_SLOT(UTextBlock, TB_HaveCount);
	GET_SLOT(UTextBlock, TB_NeedCount);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_Goods);
	GET_SLOT(UWidgetSwitcher, WS_GradeBG);
}

UMaterialInterface* UB2UIRewardIcon::GetIconMaterial(int32 RewardID)
{
	return StaticFindRewardInfo() ? StaticFindRewardInfo()->GetRewardIconMaterial(RewardID) : nullptr;
}

UMaterialInterface* UB2UIRewardIcon::GetIconMaterial(ERewardType Type)
{
	int32 RewardID = BladeIIGameImpl::GetClientDataStore().GetRewardID(Type);
	return GetIconMaterial(RewardID);
}

FText UB2UIRewardIcon::GetRewardText(int32 RewardID)
{
	FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetInfoData(RewardID) : nullptr;
	if (RewardInfo)
	{
		return BladeIIGetLOCText(RewardInfo->TextCategory, RewardInfo->RewardNameForTextTable);
	}

	return FText::FromString(TEXT(""));
}

void UB2UIRewardIcon::SetIconAndText(class UMaterialInterface* Image, const FText& Text)
{
	if (IMG_RewardMainIcon.IsValid())
	{
		IMG_RewardMainIcon->SetBrushFromMaterial(Image);
	}
	if (TB_RewardAmount.IsValid())
	{
		if (Text.IsEmpty())
		{
			TB_RewardAmount->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			TB_RewardAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TB_RewardAmount->SetText(Text);
		}
	}
}

void UB2UIRewardIcon::SetIcon(class UMaterialInterface* Image)
{
	if (IMG_RewardMainIcon.IsValid())
		IMG_RewardMainIcon->SetBrushFromMaterial(Image);
}

void UB2UIRewardIcon::SetDetailText(const FText& Text)
{
	if (TB_RewardAmount.IsValid())
		TB_RewardAmount->SetText(Text);
}

// 怖 荐樊酒聪备 眠啊咆胶飘 钎扁侩档肺档 荤侩窃.
void UB2UIRewardIcon::SetAmount(const FText& AmountText)
{
	if (TB_RewardAmount.IsValid())
		TB_RewardAmount->SetText(AmountText);
}

void UB2UIRewardIcon::UpdateRewardIconAndName(int32 RewardID)
{
	FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetInfoData(RewardID) : nullptr;

	ActiveGoodsButton(RewardID);
}

void UB2UIRewardIcon::UpdateRewardIconByItemIndex(int32 ItemIndex, const FText& Text)
{
	// 酒捞袍 牢器俊 绝绰 酒捞袍捞搁 RewardInfo俊辑 茄锅歹 茫档废 荐沥
	UMaterialInterface* IconMtrl = nullptr;
	IconMtrl = StaticFindItemInfo() ? StaticFindItemInfo()->GetItemIcon(ItemIndex) : nullptr;
	if (IconMtrl == nullptr)
	{
		IconMtrl = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetRewardIconMaterial(ItemIndex) : nullptr;
	}

	SetIconAndText(IconMtrl, Text);

	if (BTN_Goods.IsValid())
	{
		BTN_Goods->SetItemInfo(ItemIndex);
		BTN_Goods->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UIRewardIcon::UpdateRewardIcon(int32 RewardID, const FText& Text)
{
	UMaterialInterface* IconMtrl = GetIconMaterial(RewardID);
	SetIconAndText(IconMtrl, Text);
	ActiveGoodsButton(RewardID);
}

void UB2UIRewardIcon::UpdateRewardIcon(b2network::B2RewardPtr RewardInfoPtr)
{
	if (RewardInfoPtr)
	{
		UpdateRewardIcon(RewardInfoPtr->raw_reward->id, FText::AsNumber(RewardInfoPtr->raw_reward->count));
	}
}

void UB2UIRewardIcon::UpdateRewardIcon(ERewardType Type, const FText& Text)
{
	UMaterialInterface* IconMtrl = GetIconMaterial(Type);
	SetIconAndText(IconMtrl, Text);
	ActiveGoodsButton(static_cast<int32>(Type));
}

void UB2UIRewardIcon::ActiveGoodsButton(int32 RewardID)
{
	if (BTN_Goods.IsValid())
	{
		FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetInfoData(RewardID) : nullptr;
		if (RewardInfo != nullptr)
		{
			BTN_Goods->SetItemInfo(RewardInfo->ItemInfoIndex);
			BTN_Goods->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2UIRewardIcon::SetNeedCount(int32 nCount, int32 nNeedCount)
{
	if (TextSwitcher.IsValid())
		TextSwitcher->SetActiveWidgetIndex(1);

	if (TB_HaveCount.IsValid())
		TB_HaveCount->SetText(FText::AsNumber(nCount));
	if (TB_NeedCount.IsValid())
		TB_NeedCount->SetText(FText::AsNumber(nNeedCount));
	
	if (TB_HaveCount.IsValid())
	{
		if (nCount < nNeedCount)
			TB_HaveCount->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
		else
			TB_HaveCount->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}
}

void UB2UIRewardIcon::SetGrade(int32 nGrade)
{
	if (WS_GradeBG.IsValid())
	{
		WS_GradeBG->SetActiveWidgetIndex(nGrade);
	}
}

void UB2UIRewardIcon::SetCountColor(FSlateColor ApplyColor)
{
	if(TB_RewardAmount.IsValid())
		TB_RewardAmount->SetColorAndOpacity(ApplyColor);
}
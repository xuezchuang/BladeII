// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UISelectiveLotteryCostumeDT.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UICollectBookIcon.h"
#include "B2ClientDataStore.h"

#include "B2UICollectBookItemDetail.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UISelectiveLotteryCostumeDT::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_DetailParts);
	GET_SLOT(UTextBlock, TB_ItemName);
	GET_SLOT(UTextBlock, TB_PrimaryOptionName);
	GET_SLOT(UTextBlock, TB_PrimaryOptionValue);
	GET_SLOT(UTextBlock, TB_CombatName);
	GET_SLOT(UTextBlock, TB_CombatValue);
	GET_SLOT(UB2Button, BTN_Close);
	
	GET_SLOT(UB2UICollectBookIcon, UIP_CollectBookIcon);
	if (UIP_CollectBookIcon.IsValid())
	{
		UIP_CollectBookIcon->Init();
		UIP_CollectBookIcon->SetNewIcon(false);
		UIP_CollectBookIcon->SetIconCollectionState(EItemCollectionState::Lottery);
		UIP_CollectBookIcon->HideStarGrade();
	}
}

void UB2UISelectiveLotteryCostumeDT::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UISelectiveLotteryCostumeDT::OnClickBTN_Close);
}

void UB2UISelectiveLotteryCostumeDT::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_CombatName.IsValid())
		TB_CombatName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
}

void UB2UISelectiveLotteryCostumeDT::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UISelectiveLotteryCostumeDT::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UIP_CollectBookIcon.IsValid())
		UIP_CollectBookIcon->DestroySelf(InUIManager);

	DetailCloseDelegate.Unbind();

	Super::DestroySelf(InUIManager);
}

void UB2UISelectiveLotteryCostumeDT::UpdateCostumeData(FB2Item& InItemInfo)
{
	ChachedItem = InItemInfo;

	UpdateCostumeIcon();
	UpdateCostumeSubWidget();
}

void UB2UISelectiveLotteryCostumeDT::UpdateCostumeIcon()
{
	if (UIP_CollectBookIcon.IsValid())
	{
		UIP_CollectBookIcon->InitCollectBookIcon(ChachedItem.ItemClass, ChachedItem.ItemRefID);
		UIP_CollectBookIcon->SetIconCollectionState(EItemCollectionState::Collected);

		if (auto* BGMtrl = StaticFindItemMiscInfo()->GetCostumeIconBGMtrl(ChachedItem.StarGrade))
		{
			UIP_CollectBookIcon->SetIconDefaultTemplate(ChachedItem.StarGrade, BGMtrl);
		}
	}
}

void UB2UISelectiveLotteryCostumeDT::UpdateCostumeSubWidget()
{
	if (ChachedItem.ItemRefID == 0)
		return;

	const int32 CostumeQuality = 1;
	const int32 CostumeItemLevel = 1;

	float PrimaryPoint = GetPreviewPrimaryPoint(ChachedItem.PrimaryPointType, ChachedItem.ItemFactorLevel, CostumeQuality, CostumeItemLevel);

	if (TB_PrimaryOptionName.IsValid())
		TB_PrimaryOptionName->SetText(GetLOCTextOfPrimPointType(ChachedItem.PrimaryPointType));

	if (TB_PrimaryOptionValue.IsValid())
		TB_PrimaryOptionValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)PrimaryPoint)));

	DestroyDetailParts();

	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	const FMD_ItemSingleElem* ItemInfo = DataStore.GetItemMasterDetailInfo(ChachedItem.ItemRefID);
	if (ItemInfo)
	{
		FText ItemName = StaticFindItemInfo()->GetItemName(ChachedItem.ItemRefID);

		if (TB_ItemName.IsValid())
			TB_ItemName->SetText(ItemName);

		UB2UICollectBookDetailPart* DetailPart = CreateDetailPart(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_ItemAbilityTitle")));
		if (DetailPart)
 		{
			float CombatPower = DataStore.GetOptionWeight(ItemPrimaryPointTypeToEItemOptionType(ChachedItem.PrimaryPointType)) * FMath::FloorToFloat(PrimaryPoint);

			for (auto& ItemOpt : ChachedItem.IntrinsicOptions)
			{
				DetailPart->AddItemOptionTextLine(ItemOpt.OptionType, IntToPCClass(ChachedItem.AllowedPCClass), ItemOpt.RawOptionAmount);
				CombatPower += ItemOpt.RawOptionAmount * DataStore.GetOptionWeight(ItemOpt.OptionType);
			}

			if (TB_CombatValue.IsValid())
				TB_CombatValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)CombatPower)));

			if (ChachedItem.IntrinsicOptions.Num() == 0)
				DetailPart->HideTitleText();
		}
	}}

class UB2UICollectBookDetailPart* UB2UISelectiveLotteryCostumeDT::CreateDetailPart(const FText& DetailPartTitle)
{
	UB2UICollectBookDetailPart* DetailPartWidget = Cast<UB2UICollectBookDetailPart>(DynCreateInVerticalBox(DetailPartClass, this, VB_DetailParts.Get()));
	if (DetailPartWidget)
	{
		DetailPartWidget->Init();

		DetailPartWidget->SetTitleText(DetailPartTitle);
		DetailPartWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		DetailPartWidgets.Add(DetailPartWidget);
	}

	return DetailPartWidget;
}

void UB2UISelectiveLotteryCostumeDT::DestroyDetailParts()
{
	for (auto* PartWidget : DetailPartWidgets)
	{
		if (PartWidget != nullptr)
			PartWidget->DestroySelf(UB2UIManager::GetInstance());
	}

	DetailPartWidgets.Empty();
}

void UB2UISelectiveLotteryCostumeDT::OnClickBTN_Close()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	DetailCloseDelegate.ExecuteIfBound();

 	DestroyDetailParts();
 	SetVisibility(ESlateVisibility::Hidden);
}

void UB2UISelectiveLotteryCostumeDT::SetDetailViewCloseDelegate(const FOnClickCostumeDetailClose& Delegate)
{
	DetailCloseDelegate = Delegate;
}
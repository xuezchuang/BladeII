// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2ClientDataStore.h"
#include "B2UICollectBookMain.h"
#include "B2UICollectBookIcon.h"

#include "B2UIStarGradePart.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UICollectBookIcon::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UImage, IMG_Background);
	GET_SLOT(UImage, IMG_ItemIcon);
	GET_SLOT(UOverlay, O_RequestReward);
	GET_SLOT(UTextBlock, TB_GetReward);
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UB2UIWidgetBase, IMG_NewIcon);
	GET_SLOT(UImage, IMG_Selected);
	GET_SLOT(UImage, IMG_UnCollected);
	GET_SLOT(UImage, IMG_Rewarded);
	GET_SLOT(UImage, IMG_ExtraSlotEffect);

	IMG_Stars.AddDefaulted(FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);
	GET_SLOT_BYNAME(UImage, IMG_Stars[0], IMG_Star1);
	GET_SLOT_BYNAME(UImage, IMG_Stars[1], IMG_Star2);
	GET_SLOT_BYNAME(UImage, IMG_Stars[2], IMG_Star3);
	GET_SLOT_BYNAME(UImage, IMG_Stars[3], IMG_Star4);
	GET_SLOT_BYNAME(UImage, IMG_Stars[4], IMG_Star5);
	GET_SLOT_BYNAME(UImage, IMG_Stars[5], IMG_Star6);

	GET_SLOT(UOverlay, O_DetailView);
	GET_SLOT(UB2Button, BTN_DetailView);
	GET_SLOT(UTextBlock, TB_DetailView);
	
	GET_SLOT(UScaleBox,	SB_StarGrade);
	GET_SLOT(UB2UIStarGradePart, StarGradePart);
	if (StarGradePart.IsValid())
	{
		StarGradePart->Init();
	}
}

void UB2UICollectBookIcon::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UICollectBookIcon::OnClickBTN_Area);
	BIND_CLICK_FUNC_TO_BTN(BTN_DetailView, &UB2UICollectBookIcon::OnClickBTN_DetailView);
}

void UB2UICollectBookIcon::UpdateStaticText()
{
	if (TB_GetReward.IsValid())
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Common_RewardedTitle")));

	if (TB_DetailView.IsValid())
		TB_DetailView->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_ViewItemDetail")));
}

void UB2UICollectBookIcon::Init()
{
	Super::Init();

	SelectedID = 0;
	SelectedItemClass = EItemClass::EIC_End;
	bSelected = false;
	CollectionState = EItemCollectionState::None;

	if (DefaultBackgroundMtrl)
	{
		SetIconDefaultTemplate(DefaultStarGrade, DefaultBackgroundMtrl);
	}

	UpdateSelectState(false);
}

void UB2UICollectBookIcon::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}


void UB2UICollectBookIcon::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITOR

	if (IMG_Background.IsValid() == false)
		CacheAssets();

	if (DefaultBackgroundMtrl)
	{
		SetIconDefaultTemplate(DefaultStarGrade, DefaultBackgroundMtrl);
	}

#endif
}

void UB2UICollectBookIcon::OnClickBTN_Area()
{
	if (CollectBookMainUI)
		CollectBookMainUI->OnClickedItemIcon(this);
}

void UB2UICollectBookIcon::OnClickBTN_DetailView()
{
	if (CollectBookMainUI)
		CollectBookMainUI->OnClickedDetailViewIcon(this);
}

void UB2UICollectBookIcon::SelectClearCollectBookIcon()
{
	SelectedItemClass = EItemClass::EIC_End;
	SelectedID = 0;
	UpdateSelectState(false);
}

void UB2UICollectBookIcon::HideCollectBookIcon()
{
	SetVisibility(ESlateVisibility::Hidden);
}

/* 
	일반 Item의 경우 ID == ItemTemplateID
	셋트 Item의 경우 ID == SetUniqueKey
*/

void UB2UICollectBookIcon::InitCollectBookIcon(EItemClass ItemClass, int32 ID)
{
	SetIconCollectionState(EItemCollectionState::None);
	SetNewIcon(false);

	SelectedItemClass = ItemClass;
	SelectedID = ID;

	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	UMaterialInterface* IconMtrl = nullptr;
	if (ItemInfoTable)
	{
		FSingleItemInfoData* ItemInfoDataOfID = NULL;
		if (ItemClass != EItemClass::EIC_SetItem)
		{
			ItemInfoDataOfID = ItemInfoTable->GetInfoData(ID);
		}

		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
		const FCollectionItemData* CollectionInfo = DataStore.GetCollectionItem(ItemClass, SelectedID);
		if (CollectionInfo)
		{
			SetIconCollectionState(CollectionInfo->CollectionState);
		}			

		if (ItemClass == EItemClass::EIC_SetItem)
		{
			IconMtrl = ItemInfoTable->GetSetItemIcon(ID);
		}
		else
		{
			IconMtrl = ItemInfoTable->GetItemIcon(ID);
		}
		
		if (IMG_ExtraSlotEffect.IsValid())
		{
			IMG_ExtraSlotEffect->SetVisibility((ItemInfoDataOfID && ItemInfoDataOfID->bHasExtraSlotEffect) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
	}

	SetIconMaterial(IconMtrl);
}

void UB2UICollectBookIcon::SetIconDefaultTemplate(int32 StarGrade, UMaterialInterface* BgMaterial)
{
	if (IMG_Background.IsValid())
	{
		IMG_Background->SetBrushFromMaterial(BgMaterial);
	}

	InitStarGrade(StarGrade);
}

void UB2UICollectBookIcon::SetIconMaterial(UMaterialInterface* IconMaterial)
{
	if (IMG_ItemIcon.IsValid())
	{
		IMG_ItemIcon->SetBrushFromMaterial(IconMaterial);
	}
}

void UB2UICollectBookIcon::SetIconCollectionState(EItemCollectionState InCollectionState)
{
	if (IMG_UnCollected.IsValid())
	{
		bool bVisible = InCollectionState == EItemCollectionState::None ? true : false;
		IMG_UnCollected->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	if (O_RequestReward.IsValid())
	{
		bool bVisibleRequestReward = InCollectionState == EItemCollectionState::Enhanced ? true : false;
		O_RequestReward->SetVisibility(bVisibleRequestReward ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	if (IMG_Rewarded.IsValid())
	{
		bool bVisibleRewarded = InCollectionState == EItemCollectionState::Rewarded ? true : false;
		IMG_Rewarded->SetVisibility(bVisibleRewarded ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	SetNewIcon(InCollectionState == EItemCollectionState::Enhanced ? true : false);

	CollectionState = InCollectionState;
}

void UB2UICollectBookIcon::SetNewIcon(bool bVisible)
{
	if (IMG_NewIcon.IsValid())
	{
		IMG_NewIcon->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UICollectBookIcon::SetEnableSlotEffect(bool bVisible)
{
	if (IMG_ExtraSlotEffect.IsValid())
	{
		IMG_ExtraSlotEffect->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UICollectBookIcon::InitStarGrade(int32 StarGrade)
{
	if (StarGradePart.IsValid())
	{
		StarGradePart->UpdateStar(StarGrade, false);
	}
}

void UB2UICollectBookIcon::SetStarGrade(const int32 StarGrade, const bool IsSurpass)
{
	if (StarGradePart.IsValid())
	{
		StarGradePart->UpdateStar(StarGrade, IsSurpass);
	}
}

void UB2UICollectBookIcon::HideStarGrade()
{
	if (SB_StarGrade.IsValid())
	{
		SB_StarGrade->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UICollectBookIcon::UpdateSelectState(bool bInSelected)
{
	bSelected = bInSelected;
	if (IMG_Selected.IsValid())
	{
		IMG_Selected->SetVisibility(bSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (O_DetailView.IsValid())
	{
		O_DetailView->SetVisibility(bSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UICollectBookIcon::SetOwnerCollectBookMainUI(class UB2UICollectBookMain* OwnerUI)
{
	CollectBookMainUI = OwnerUI;
}

void UB2UICollectBookIconRow::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ItemRowName);
	ItemIcons.AddDefaulted(FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);

	for (int32 i = 0; i < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; ++i)
	{
		ItemIcons[i] = GetCachedWidget<UB2UICollectBookIcon>(FName(*FString::Printf(TEXT("CollectItemStar%d"), i + 1)));

	}

	for (auto IconWidget : ItemIcons)
	{
		if (IconWidget.IsValid())
			IconWidget->Init();
	}
}

void UB2UICollectBookIconRow::Init()
{
	Super::Init();
}

void UB2UICollectBookIconRow::DestroySelf(class UB2UIManager* InUIManager)
{
	for (auto Icon : ItemIcons)
	{
		if (Icon.IsValid())
			Icon->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UICollectBookIconRow::SetOwnerCollectBookMainUI(class UB2UICollectBookMain* OwnerUI)
{
	for (auto& IconWidget : ItemIcons)
	{
		if (IconWidget.IsValid())
		{
			IconWidget->SetOwnerCollectBookMainUI(OwnerUI);
		}
	}
}

UB2UICollectBookIcon* UB2UICollectBookIconRow::GetIconWidget(int32 Grade)
{
	int32 IconIndex = GetSuitableIconIndex(Grade);
	if (IconIndex != INDEX_NONE)
	{
		return ItemIcons[IconIndex].Get();
	}

	return nullptr;
}

void UB2UICollectBookIconRow::SetCollectBookIconInfo(EItemClass ItemClass, int32 Grade, int32 ID)
{
	int32 IconIndex = GetSuitableIconIndex(Grade);
	if (ItemIcons.IsValidIndex(IconIndex))
	{
		auto& IconWidget = ItemIcons[IconIndex];
		if (IconWidget.IsValid())
		{
			IconWidget->InitCollectBookIcon(ItemClass, ID);
		}
	}
}

void UB2UICollectBookIconRow::ClearCollectBookIcon(int32 Grade)
{
	int32 IconIndex = GetSuitableIconIndex(Grade);
	if (ItemIcons.IsValidIndex(IconIndex))
	{
		auto& IconWidget = ItemIcons[IconIndex];
		if (IconWidget.IsValid())
		{
			IconWidget->HideCollectBookIcon();
		}
	}
}

void UB2UICollectBookIconRow::SetRowTitleText(const FText& TitleText)
{
	if (TB_ItemRowName.IsValid())
		TB_ItemRowName->SetText(TitleText);
}

int32 UB2UICollectBookIconRow::GetSuitableIconIndex(int32 Grade)
{
	for (int32 RowIndex = 0; RowIndex < ItemIcons.Num(); RowIndex++)
	{
		auto& IconWidget = ItemIcons[RowIndex];
		if (IconWidget.IsValid() && IconWidget->GetGrade() == Grade)
			return RowIndex;
	}
	
	return INDEX_NONE;
}

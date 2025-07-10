// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICollectBookItemDetail.h"
#include "B2UIManager.h"
#include "B2ClientDataStore.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "B2AndroidBackManager.h"
#include "B2UICollectBookIcon.h"
#include "B2DynText_Multiple.h"
#include "B2DynText_ItemDetailOption.h"




#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UICollectBookItemDetail::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_DetailParts);

	GET_SLOT(UCanvasPanel, CP_LevelControl);
	GET_SLOT(UB2UICollectBookIcon, UIP_CollectBookIcon);
	if (UIP_CollectBookIcon.IsValid())
		UIP_CollectBookIcon->Init();

	GET_SLOT(UB2Button, BTN_LevelDown);
	GET_SLOT(UB2Button, BTN_LevelUp);

	GET_SLOT(UTextBlock, TB_EquipmentEnhance);
	GET_SLOT(UTextBlock, TB_Enhance);

	GET_SLOT(UTextBlock, TB_EquipmentLv);
	GET_SLOT(UTextBlock, TB_Level);

	GET_SLOT(UTextBlock, TB_PrimaryOptionName);
	GET_SLOT(UTextBlock, TB_PrimaryOptionValue);

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UTextBlock, TB_ItemName);

	GET_SLOT(USlider, SD_Level);
	GET_SLOT(UTextBlock, TB_CombatValue);
	GET_SLOT(UTextBlock, TB_CombatName);


	GET_SLOT(UPanelWidget, P_LevelSlide);
	GET_SLOT(UPanelWidget, P_EnhanceSlide);
	GET_SLOT(UImage, IMG_BackGround);

	GET_SLOT(USlider,   SD_Enhance);
	GET_SLOT(UB2Button, BTN_EnhanceDown);
	GET_SLOT(UB2Button, BTN_EnhanceUp);
}

void UB2UICollectBookItemDetail::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_LevelDown, &UB2UICollectBookItemDetail::OnClickBTN_LevelDown);
	BIND_CLICK_FUNC_TO_BTN(BTN_LevelUp, &UB2UICollectBookItemDetail::OnClickBTN_LevelUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceDown, &UB2UICollectBookItemDetail::OnClickBTN_EnhanceDown);
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceUp, &UB2UICollectBookItemDetail::OnClickBTN_EnhanceUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UICollectBookItemDetail::OnClickBTN_Close);

	if (SD_Level.IsValid())
	{
		SD_Level->OnValueChanged.AddDynamic(this, &UB2UICollectBookItemDetail::OnChangeLevelSlider);
	}
	if (SD_Enhance.IsValid())
	{
		SD_Enhance->OnValueChanged.AddDynamic(this, &UB2UICollectBookItemDetail::OnChangeEnhanceSlider);
	}
}

void UB2UICollectBookItemDetail::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_EquipmentEnhance.IsValid())
		TB_EquipmentEnhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_EnhanceLevelDesc")));

	if(TB_CombatName.IsValid())
		TB_CombatName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));

	if (TB_EquipmentLv.IsValid())
		TB_EquipmentLv->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemLevelUpLevel")));
}

void UB2UICollectBookItemDetail::Init()
{
	Super::Init();
}

void UB2UICollectBookItemDetail::DestroySelf(UB2UIManager* InUIManager)
{
	if (UIP_CollectBookIcon.IsValid())
		UIP_CollectBookIcon->DestroySelf(InUIManager);

	DestroyDetailParts();
	DetailCloseDelegate.Unbind();

	if (this->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}

	Super::DestroySelf(InUIManager);
}

class UB2UICollectBookDetailPart* UB2UICollectBookItemDetail::CreateDetailPart(const FText& DetailPartTitle)
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

void UB2UICollectBookItemDetail::SetDetailViewItemLevel(int32 CurrentLevel, int32 InMaxLevel)
{
	if (TB_Level.IsValid())
	{
		FText LevelFormat = FText::FromString(TEXT("{0} / {1}"));
		TB_Level->SetText(FText::Format(LevelFormat, FText::AsNumber(CurrentLevel), FText::AsNumber(InMaxLevel)));
	}

	if (SD_Level.IsValid())
	{
		int32 RealCurrentValue = CurrentLevel - 1;
		float SliderValue = 1.f / float(InMaxLevel - 1);
		SD_Level->SetStepSize(SliderValue);
		SD_Level->SetValue(SliderValue * RealCurrentValue);
	}

}

void UB2UICollectBookItemDetail::SetDetailViewItemEnhance(int32 CurrentEnhance, int32 InMaxEnhance)
{
	if (TB_Enhance.IsValid())
	{
		FText LevelFormat = FText::FromString(TEXT("{0} / {1}"));
		TB_Enhance->SetText(FText::Format(LevelFormat, FText::AsNumber(CurrentEnhance), FText::AsNumber(InMaxEnhance)));
	}

	if (SD_Enhance.IsValid())
	{
		int32 RealCurrentValue = CurrentEnhance;
		float SliderValue = 1.f / float(InMaxEnhance);
		SD_Enhance->SetStepSize(SliderValue);
		SD_Enhance->SetValue(SliderValue * RealCurrentValue);
	}

}

void UB2UICollectBookItemDetail::SetDetailViewSingleItem(int32 ItemTemplateID, EPCClass ClassType, const int32 SurpassLevel/* = 0*/, FB2Item OriginalItem)
{
	if (ItemTemplateID == 0)
		return;

	DestroyDetailParts();
	ShowItemLevelControl(true);

	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	const FMD_ItemSingleElem* ItemInfo = DataStore.GetItemMasterDetailInfo(ItemTemplateID);
	if (ItemInfo)
	{
		UMaterialInstance* IconMtrl = nullptr;
		FText ItemName;
		StaticFindItemInfo()->GetItemIconAndName(ItemTemplateID, IconMtrl, ItemName);
		SetItemDisplayInfo(ItemName, IconMtrl);

		//MaxLevel = ItemInfo->MaxLevel + SurpassLevel;
		MaxLevel = 30 + SurpassLevel;	//기본 아이템 최대 레벨이 30이라고 가정한다. 그 이상의 것도 서버에서 최대레벨 제한이 걸려서 숫자를 맞추기 위해 이렇게 진행한다.
		CurrentLevelupFactor = 1;
		CurrentEnhanceFactor = 0;

		MaxEnhance = DataStore.GetMaxEnhanceLevelForSetEffect();
		CachedItem = OriginalItem;
		FB2Item tempItem = CachedItem;


		SetDetailViewItemLevel(CurrentLevelupFactor, MaxLevel);
		SetDetailViewItemEnhance(CurrentEnhanceFactor, MaxEnhance);

		UB2UICollectBookDetailPart* DetailPart = CreateDetailPart(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_ItemAbilityTitle")));
		if (DetailPart)
		{
			// Primary Option
			PrimaryPointType = ItemInfo->PrimaryType;
			CurrentItemLevel = ItemInfo->ItemFactorLevel;

			CachedSurpassLevel = SurpassLevel;

			tempItem.EnhanceLevel = CurrentEnhanceFactor;
			tempItem.SurpassCount = CachedSurpassLevel;

			FB2Item minItem = tempItem;
			minItem.Quality = 1;
			minItem.PrimaryPointType = PrimaryPointType;
			minItem.SetPrimaryPoint(GetPreviewPrimaryPoint(PrimaryPointType, CurrentItemLevel, 1, CurrentLevelupFactor));
			FB2Item maxItem = tempItem;
			maxItem.Quality = MAX_ITEM_QUALITY;
			maxItem.PrimaryPointType = PrimaryPointType;
			maxItem.SetPrimaryPoint(GetPreviewPrimaryPoint(PrimaryPointType, CurrentItemLevel, MAX_ITEM_QUALITY, CurrentLevelupFactor));

			GetCalculateItem_PreviewItemEnhance(minItem, minItem, CurrentEnhanceFactor, CurrentItemLevel);
			GetCalculateItem_PreviewItemEnhance(maxItem, maxItem, CurrentEnhanceFactor, CurrentItemLevel);

			//float MinQualityPrimaryPoint = GetPreviewPrimaryPoint(PrimaryPointType, CurrentItemLevel, 1, CurrentLevelupFactor);
			//float MaxQualityPrimaryPoint = GetPreviewPrimaryPoint(PrimaryPointType, CurrentItemLevel, MAX_ITEM_QUALITY, CurrentLevelupFactor);
			float MinQualityPrimaryPoint = minItem.GetPrimaryPoint();
			float MaxQualityPrimaryPoint = maxItem.GetPrimaryPoint();

			ItemOptionCombatValue = 0;
			for (auto& ItemOpt : ItemInfo->UniqueOptions)
			{
				DetailPart->AddItemOptionTextLine(ItemOpt.OptionType, ClassType, ItemOpt.RawOptionAmount);
				ItemOptionCombatValue += ItemOpt.RawOptionAmount * DataStore.GetOptionWeight(ItemOpt.OptionType);
			}

			// UniqueOption이 없으면 RandomOption 텍스트가 HeaderTitle을 대신 함
			if (ItemInfo->UniqueOptions.Num() == 0)
				DetailPart->HideTitleText();

			if (ItemInfo->RandomOptCount > 0)
			{
				DetailPart->AddSubTitleLine(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_RandomOption")));
				for (int32 RandOpt = 0; RandOpt < ItemInfo->RandomOptCount; RandOpt++)
					DetailPart->AddRandOptionTextLine();
			}

			if (ItemInfo->SealSlotOpen > 0)
			{
				DetailPart->AddSubTitleLine(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Seal")));
				for (int32 SealOpt = 0; SealOpt < ItemInfo->SealSlotOpen; SealOpt++)
				{
					DetailPart->AddSealTextLine();
				}
			}

			SetItemPrimaryOption(ItemInfo->PrimaryType, MinQualityPrimaryPoint, MaxQualityPrimaryPoint); //전투력계산을 해야하기때문에 제일 밑으로 돌리겠습니다
		}

	}
}

void UB2UICollectBookItemDetail::SetDetailViewSetAbility(int32 SetUniqueKey, EPCClass ClassType)
{
	if (SetUniqueKey == 0)
		return;

	DestroyDetailParts();
	ShowItemLevelControl(false);

	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	FMD_SetInfo* SetItemInfo = DataStore.GetSetItemInfo(ClassType, SetUniqueKey);

	if (SetItemInfo)
	{
		UMaterialInterface* IconMtrl = nullptr;
		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable && SetUniqueKey)
		{
			IconMtrl = ItemInfoTable->GetSetItemIcon(SetUniqueKey);
		}

		const int32 GroupID = GetSetGroupIDFromUniqueKey(SetUniqueKey);
		FText ItemName = GetLOCTextOfSetItemName(GroupID);
		SetItemDisplayInfo(ItemName, IconMtrl);

		for (int32 SetOptionID : SetItemInfo->SetOptionIDs)
		{
			const FMD_ItemSetOptionElem* SetOption = DataStore.GetItemSetOptionInfo(SetOptionID);
			if (SetOption && SetOption->HaveSetItemOption())
			{
				EItemInvenType SetInvenType = GetSetOptionInvenType(SetOptionID);
				FText DetailPartTitle;
				int32 OptionCount = 0;

				if (SetInvenType == EItemInvenType::EIIVT_Protection)
				{
					DetailPartTitle = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_SetArmorAbility"));
					OptionCount = ESetItemOptionCount::LAST_COUNT;
				}
				else if (SetInvenType == EItemInvenType::EIIVT_Accessory)
				{
					DetailPartTitle = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_SetAccAbility"));
					OptionCount = ESetItemOptionCount::ACCESSORY_COUNT;
				}

				UB2UICollectBookDetailPart* DetailPart = CreateDetailPart(DetailPartTitle);
				if (DetailPart)
				{
					for (int32 SetItemCount = ESetItemOptionCount::FIRST_COUNT; SetItemCount <= OptionCount; ++SetItemCount)
					{
						FItemSetOptionData OptionData;
						if (SetOption->GetSetItemOption(SetItemCount, OptionData))
							DetailPart->AddSetEffectTextLine(SetItemCount, OptionData.SetItemOption, ClassType, OptionData.SetItemOptionValue);
					}
				}
			}
		}

		UB2UICollectBookDetailPart* SetCompositionPart = CreateDetailPart(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_SetAbilityTitle")));
		if (SetCompositionPart)
		{
			for (int32 ItemTemplateID : SetItemInfo->SetCompositionItems)
			{
				EItemClass ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
				bool bHasItem = DataStore.GetCollectionItem(ItemClass, ItemTemplateID) != nullptr ? true : false;
				SetCompositionPart->AddSetCompositionTextLine(ItemTemplateID, bHasItem);
			}
		}
	}
}

void UB2UICollectBookItemDetail::SetDetailViewSetComposition(int32 SetUniqueKey, EPCClass ClassType)
{
	if (SetUniqueKey == 0)
		return;

	DestroyDetailParts();
	ShowItemLevelControl(false);

	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	FMD_SetInfo* SetItemInfo = DataStore.GetSetItemInfo(ClassType, SetUniqueKey);

	if (SetItemInfo)
	{
		UMaterialInterface* IconMtrl = nullptr;
		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable && SetUniqueKey)
		{
			IconMtrl = ItemInfoTable->GetSetItemIcon(SetUniqueKey);
		}

		const int32 GroupID = GetSetGroupIDFromUniqueKey(SetUniqueKey);
		FText ItemName = GetLOCTextOfSetItemName(GroupID);
		SetItemDisplayInfo(ItemName, IconMtrl);

		UB2UICollectBookDetailPart* DetailPart = CreateDetailPart(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_SetAbilityTitle")));
		if (DetailPart)
		{
			for (int32 ItemTemplateID : SetItemInfo->SetCompositionItems)
			{
				EItemClass ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
				bool bHasItem = DataStore.GetCollectionItem(ItemClass, ItemTemplateID) != nullptr ? true : false;
				DetailPart->AddSetCompositionTextLine(ItemTemplateID, bHasItem);
			}
		}
	}
}

void UB2UICollectBookItemDetail::SetItemIconDefaultTemplate(class UB2UICollectBookIcon* ItemIconWidget)
{
	SetItemIconDefaultTemplate(ItemIconWidget->GetGrade(), ItemIconWidget->GetBackgroumdMaterial(), EItemCollectionState::Collected);
}

void UB2UICollectBookItemDetail::SetItemIconDefaultTemplate(int32 ItemGrade, UMaterialInterface* BGMaterial, EItemCollectionState ItemCollectionState)
{
	if (UIP_CollectBookIcon.IsValid())
	{
		UIP_CollectBookIcon->SetIconDefaultTemplate(ItemGrade, BGMaterial);

		UIP_CollectBookIcon->SetIconCollectionState(ItemCollectionState);
	}
}

void UB2UICollectBookItemDetail::SetItemDisplayInfo(const FText& ItemName, UMaterialInterface* IconMtrl)
{
	if (IconMtrl && UIP_CollectBookIcon.IsValid())
	{
		UIP_CollectBookIcon->SetIconMaterial(IconMtrl);
	}

	if (TB_ItemName.IsValid())
		TB_ItemName->SetText(ItemName);
}

void UB2UICollectBookItemDetail::SetItemPrimaryOption(EItemPrimaryPointType PrimaryOptionType, float MinValue, float MaxValue)
{
	if (TB_PrimaryOptionName.IsValid())
	{
		TB_PrimaryOptionName->SetText(GetLOCTextOfPrimPointType(PrimaryOptionType));
	}

	if (TB_PrimaryOptionValue.IsValid())
	{
		FText ValueText;

		if (MinValue == MaxValue)
		{
			ValueText = FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(static_cast<int32>(MinValue)));
		}
		else
		{
			ValueText = FText::Format(FText::FromString(TEXT("{0} ~ {1}")), FText::AsNumber(static_cast<int32>(MinValue)), FText::AsNumber(static_cast<int32>(MaxValue)));
		}
		
		TB_PrimaryOptionValue->SetText(ValueText);
	}

	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();

	if (TB_CombatValue.IsValid())
	{
		float MaxCombatValue = FMath::FloorToFloat(MaxValue * 100);
		float MinCombatValue = FMath::FloorToFloat(MinValue * 100);

		MaxCombatValue = ItemOptionCombatValue + (DataStore.GetOptionWeight(ItemPrimaryPointTypeToEItemOptionType(PrimaryOptionType)) * (MaxCombatValue / 100));
		MinCombatValue = ItemOptionCombatValue + (DataStore.GetOptionWeight(ItemPrimaryPointTypeToEItemOptionType(PrimaryOptionType)) * (MinCombatValue / 100));

		FText CombatValueText;
		if (MinValue == MaxValue)
		{
			CombatValueText = FText::Format(FText::FromString(TEXT("{0}")),	FText::AsNumber(FMath::RoundToInt(MaxCombatValue)));
		}
		else
		{
			CombatValueText = FText::Format(FText::FromString(TEXT("{0} ~ {1}")),
				FText::AsNumber(FMath::FloorToInt(MinCombatValue)), FText::AsNumber(FMath::RoundToInt(MaxCombatValue)));
		}

		TB_CombatValue->SetText(CombatValueText);
	}
}

void UB2UICollectBookItemDetail::SetItemIconStarGrade(const int32 ItemGrade, const bool IsSurpass)
{
	if (UIP_CollectBookIcon.IsValid())
	{
		UIP_CollectBookIcon->SetStarGrade(ItemGrade, IsSurpass);
	}
}

void UB2UICollectBookItemDetail::OpenPopup()
{
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

void UB2UICollectBookItemDetail::SetDetailViewCloseDelegate(const FOnClickDetailClose& Delegate)
{
	DetailCloseDelegate = Delegate;
}

void UB2UICollectBookItemDetail::SetCollectBookIcon(EItemClass ItemClass, int32 RefID)
{
	if (UIP_CollectBookIcon.IsValid())
		UIP_CollectBookIcon->InitCollectBookIcon(ItemClass, RefID);
}

void UB2UICollectBookItemDetail::SetSelectiveLottery()
{
	if (UIP_CollectBookIcon.IsValid())
	{
		UIP_CollectBookIcon->SetNewIcon(false);
		UIP_CollectBookIcon->SetIconCollectionState(EItemCollectionState::Lottery);
	}
	
	//선택 뽑기권에서 레벨, 강화를 조절하는걸 막는 구문인데, 기획쪽에서 선택 뽑기권에서도 데모로 보여주자고 해서 주석처리 함.
	/*
	if (P_LevelSlide.IsValid())
	{
		P_LevelSlide->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (P_EnhanceSlide.IsValid())
	{
		P_EnhanceSlide->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (TB_Enhance.IsValid())
	{
		//기존에 +0 구문 자체가 블루프린트 하드코딩이였다. 그러므로 여기도 그냥 하드코딩으로 덮어버리자.
		TB_Enhance->SetText(FText::FromString(TEXT("+0")));
	}
	*/
}

void UB2UICollectBookItemDetail::SetEnableSlotEffect(bool bVisible)
{
	if (UIP_CollectBookIcon.IsValid())
	{
		UIP_CollectBookIcon->SetEnableSlotEffect(bVisible);
	}
}

void UB2UICollectBookItemDetail::SetEnableBackGround(bool bVisible)
{
	if (IMG_BackGround.IsValid())
	{
		IMG_BackGround->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible :  ESlateVisibility::Collapsed);
	}
}

void UB2UICollectBookItemDetail::DestroyDetailParts()
{
	for (auto* PartWidget : DetailPartWidgets)
	{
		if (PartWidget != nullptr)
			PartWidget->DestroySelf(UB2UIManager::GetInstance());
	}

	DetailPartWidgets.Empty();
}

void UB2UICollectBookItemDetail::ShowItemLevelControl(bool bVisible)
{
	if (CP_LevelControl.IsValid() && VB_DetailParts.IsValid())
	{
		Cast<UVerticalBoxSlot>(VB_DetailParts->AddChild(CP_LevelControl.Get()));
		CP_LevelControl->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UICollectBookItemDetail::OnChangedLevel(int32 NewLevel)
{
	CurrentLevelupFactor = FMath::Clamp<int32>(NewLevel, 1, MaxLevel);

	FB2Item minItem = CachedItem;
	minItem.Quality = 1;
	minItem.EnhanceLevel = CurrentEnhanceFactor;
	minItem.SurpassCount = CachedSurpassLevel;
	minItem.Level = CurrentLevelupFactor;
	minItem.PrimaryPointType = PrimaryPointType;
	minItem.SetPrimaryPoint(GetPreviewPrimaryPoint(PrimaryPointType, CurrentItemLevel, 1, CurrentLevelupFactor));

	FB2Item maxItem = CachedItem;
	maxItem.Quality = MAX_ITEM_QUALITY;
	maxItem.EnhanceLevel = CurrentEnhanceFactor;
	maxItem.SurpassCount = CachedSurpassLevel;
	maxItem.Level = CurrentLevelupFactor;
	maxItem.PrimaryPointType = PrimaryPointType;
	maxItem.SetPrimaryPoint(GetPreviewPrimaryPoint(PrimaryPointType, CurrentItemLevel, MAX_ITEM_QUALITY, CurrentLevelupFactor));

	GetCalculateItem_PreviewItemEnhance(minItem, minItem, 0, CurrentItemLevel);
	GetCalculateItem_PreviewItemEnhance(maxItem, maxItem, 0, CurrentItemLevel);

	float MinQualityPrimaryPoint = minItem.GetPrimaryPoint();
	float MaxQualityPrimaryPoint = maxItem.GetPrimaryPoint();

	SetItemPrimaryOption(PrimaryPointType, MinQualityPrimaryPoint, MaxQualityPrimaryPoint);
	SetDetailViewItemLevel(CurrentLevelupFactor, MaxLevel);
}

void UB2UICollectBookItemDetail::OnChangedEnhance(int32 NewEnhance)
{
	CurrentEnhanceFactor = FMath::Clamp<int32>(NewEnhance, 0, MaxEnhance);

	FB2Item minItem = CachedItem;
	minItem.Quality = 1;
	minItem.EnhanceLevel = CurrentEnhanceFactor;
	minItem.SurpassCount = CachedSurpassLevel;
	minItem.Level = CurrentLevelupFactor;
	minItem.PrimaryPointType = PrimaryPointType;
	minItem.SetPrimaryPoint(GetPreviewPrimaryPoint(PrimaryPointType, CurrentItemLevel, 1, CurrentLevelupFactor));

	FB2Item maxItem = CachedItem;
	maxItem.Quality = MAX_ITEM_QUALITY;
	maxItem.EnhanceLevel = CurrentEnhanceFactor;
	maxItem.SurpassCount = CachedSurpassLevel;
	maxItem.Level = CurrentLevelupFactor;
	maxItem.PrimaryPointType = PrimaryPointType;
	maxItem.SetPrimaryPoint(GetPreviewPrimaryPoint(PrimaryPointType, CurrentItemLevel, MAX_ITEM_QUALITY, CurrentLevelupFactor));

	GetCalculateItem_PreviewItemEnhance(minItem, minItem, 0, CurrentItemLevel);
	GetCalculateItem_PreviewItemEnhance(maxItem, maxItem, 0, CurrentItemLevel);

	float MinQualityPrimaryPoint = minItem.GetPrimaryPoint();
	float MaxQualityPrimaryPoint = maxItem.GetPrimaryPoint();

	SetItemPrimaryOption(PrimaryPointType, MinQualityPrimaryPoint, MaxQualityPrimaryPoint);
	SetDetailViewItemEnhance(CurrentEnhanceFactor, MaxEnhance);
}

void UB2UICollectBookItemDetail::OnClickBTN_LevelDown()
{
	if(CurrentLevelupFactor > 1 )
		OnChangedLevel(CurrentLevelupFactor - 1);
}

void UB2UICollectBookItemDetail::OnClickBTN_LevelUp()
{
	if (CurrentLevelupFactor < MaxLevel)
		OnChangedLevel(CurrentLevelupFactor + 1);
}

void UB2UICollectBookItemDetail::OnClickBTN_EnhanceDown()
{
	if (CurrentEnhanceFactor > 0)
		OnChangedEnhance(CurrentEnhanceFactor - 1);
}

void UB2UICollectBookItemDetail::OnClickBTN_EnhanceUp()
{
	if (CurrentEnhanceFactor < MaxEnhance)
		OnChangedEnhance(CurrentEnhanceFactor + 1);
}

void UB2UICollectBookItemDetail::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UICollectBookItemDetail::OnClickBTN_Close()
{
	DetailCloseDelegate.ExecuteIfBound();

	DestroyDetailParts();
	SetVisibility(ESlateVisibility::Hidden);
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
}

void UB2UICollectBookItemDetail::OnChangeLevelSlider(float InValue)
{
	OnChangedLevel(InValue * MaxLevel);
}

void UB2UICollectBookItemDetail::OnChangeEnhanceSlider(float InValue)
{
	OnChangedEnhance(InValue * MaxEnhance);
}

void UB2UICollectBookDetailPart::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UOverlay, O_PartTitle);
	GET_SLOT(UVerticalBox, VB_DetailOptions);
	GET_SLOT(UTextBlock, TB_HeaderTitle);
}

void UB2UICollectBookDetailPart::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UICollectBookDetailPart::Init()
{
	Super::Init();
}

void UB2UICollectBookDetailPart::DestroySelf(class UB2UIManager* InUIManager)
{
	DestroyTextLines();

	Super::DestroySelf(InUIManager);
}

void UB2UICollectBookDetailPart::SetTitleText(const FText& TitleText)
{
	if (TB_HeaderTitle.IsValid())
	{
		TB_HeaderTitle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TB_HeaderTitle->SetText(TitleText);
	}
}

void UB2UICollectBookDetailPart::HideTitleText()
{
	if (O_PartTitle.IsValid())
		O_PartTitle->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UICollectBookDetailPart::AddItemOptionTextLine(EItemOption OptionType, EPCClass ClassType, float Value)
{
	if (Value == 0.f)
		return;

	UB2DynText_Multiple* TextLine = CreateTextLine(DefaultLineTextClass);
	if (TextLine)
	{
		FText OptionValueText = GetItemOptionValueDisplayText(OptionType, Value, true);
		TextLine->SetDynText(0, GetLOCTextOfItemOption(OptionType, ClassType), OptionValueText);
	}
}

void UB2UICollectBookDetailPart::AddSubTitleLine(const FText& SubTitleText)
{
	UB2DynText_Multiple* TextLine = CreateTextLine(SubTitleTextClass);
	if (TextLine)
	{
		TextLine->SetDynText(0, SubTitleText);
	}
}

void UB2UICollectBookDetailPart::AddRandOptionTextLine()
{
	UB2DynText_Multiple* TextLine = CreateTextLine(DefaultLineTextClass);
	if (TextLine)
	{
		FText GetRandomOptionText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_RandomOption"));
		TextLine->SetDynText(0, GetRandomOptionText, FText::FromString(TEXT("?")));
	}
}

void UB2UICollectBookDetailPart::AddSealTextLine()
{
	UB2DynText_Multiple* TextLine = CreateTextLine(DefaultLineTextClass);
	if (TextLine)
	{
		FText GetRandomOptionText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_SealSlot"));
		TextLine->SetDynText(0, GetRandomOptionText, FText::FromString(TEXT("")));
	}
}

void UB2UICollectBookDetailPart::AddSetEffectTextLine(int32 SetCount, EItemOption OptionType, EPCClass ClassType, float IncreaseValue)
{
	if (IncreaseValue == 0.f)
		return;

	UB2DynText_Multiple* TextLine = CreateTextLine(DefaultLineTextClass);
	if (TextLine)
	{
		FText SetEffectTextFormat = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_SetEffectDescription"));
		FText SetEffectText = FText::Format(SetEffectTextFormat, FText::AsNumber(SetCount), GetLOCTextOfItemOption(OptionType, ClassType));
		FText OptionValueText = GetItemOptionValueDisplayText(OptionType, IncreaseValue, true);

		TextLine->SetDynText(0, SetEffectText, OptionValueText);
	}
}


void UB2UICollectBookDetailPart::AddSetCompositionTextLine(int32 ItemTemplateID, bool bHasItem)
{
	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable && ItemTemplateID)
	{
		UB2DynText_Multiple* TextLine = CreateTextLine(SingleLineTextClass);
		if (TextLine)
		{
			const FText& ItemName = ItemInfoTable->GetItemName(ItemTemplateID);

			if (bHasItem)
			{
				TextLine->SetDynText(0, ItemName);
			}
			else
			{
				FSlateColor UnCollectColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.4f));
				TextLine->SetDynText(0, ItemName, UnCollectColor);
			}
		}
	}
}

void UB2UICollectBookDetailPart::DestroyTextLines()
{
	for (auto* TextLine : CachedTextLines)
	{
		if (TextLine)
			TextLine->DestroySelf();
	}

	CachedTextLines.Empty();
}

UB2DynText_Multiple* UB2UICollectBookDetailPart::CreateTextLine(const TSubclassOf<class UB2DynText_Multiple>& TextClass)
{
	UB2DynText_Multiple* TextLine = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(TextClass, this, VB_DetailOptions.Get()));
	if (TextLine)
	{
		CachedTextLines.Add(TextLine);
	}

	return TextLine;
}

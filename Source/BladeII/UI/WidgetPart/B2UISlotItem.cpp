#include "B2UISlotItem.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2ItemInfo.h"
#include "B2Inventory.h"
#include "B2EtherManager.h"
#include "B2EtherInfo.h"
#include "B2UIStarGradePart.h"
#include "BladeIIGameImpl.h"

UB2UISlotItem::UB2UISlotItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Count = 0;
	bCount_0_Text_Invisible = false;
	bCount_0_Icon_Invisible = false;
	bSelected = false;
	bShowCount = true;
	bUseSimpleBGImage = false;
}

void UB2UISlotItem::SynchronizeProperties()
{
	if (!TB_ItemCount.IsValid())
		CacheAssets();

	SetCount(Count);

	Super::SynchronizeProperties();
}

void UB2UISlotItem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Reading_Glasses);
	GET_SLOT(UImage, IMG_ItemQualityBG);
	GET_SLOT(UImage, IMG_ItemBGPanel);
	GET_SLOT(UTextBlock, TB_ItemName);
	GET_SLOT(UTextBlock, TB_ItemLevel);
	GET_SLOT(UTextBlock, TB_ItemCount);
	GET_SLOT(UTextBlock, TB_EnhanceLevel);
	GET_SLOT(UTextBlock, TB_PrimaryPoint);
	GET_SLOT(UPanelWidget, O_Selected);
	GET_SLOT(UB2UIStarGradePart, UIP_Stars);
	GET_SLOT(UImage, IMG_StarIcon);
	GET_SLOT(UImage, IMG_BossItemIcon);
	GET_SLOT(UTextBlock, TB_StarCount);
	GET_SLOT(UImage, IMG_EtherPos);
	GET_SLOT(UPanelWidget, CP_Stars);
	GET_SLOT(UPanelWidget, OV_ItemQualitySet);
	GET_SLOT(UTextBlock, TB_ItemQuality);
	GET_SLOT(UImage, IMG_ExtraSlotEffect);
	GET_SLOT(UTextBlock, TB_ItemQuality);
}

void UB2UISlotItem::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UISlotItem::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UISlotItem::Init()
{
	Super::Init();

	if (UIP_Stars.IsValid())
		UIP_Stars->Init();

	//���׸��� ���� ���� �����ؼ� �ʱ�ȭ�� �ʿ���
	if (IMG_Icon.IsValid())
	{
		IMG_Icon->SetColorAndOpacity(FLinearColor::White);
	}

	SetItemCountTextColor(FLinearColor::White);
}

void UB2UISlotItem::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (UIP_Stars.IsValid())
	{
		UIP_Stars->DestroySelf(InUIManager);
	}
}

void UB2UISlotItem::BindDoc(const FB2Item& InItemData, int32 quantity)
{
	UnbindDoc();
	SetDocStruct(InItemData);

	class UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable)
	{
		BindItemInfoData(ItemInfoTable->GetInfoData(InItemData.ItemRefID), InItemData.Quality);
	}

	// �Ʒ��� �ǹ̻� BindItemInfoData �� ���� ������ �͵�.
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo(); // RefID �� ���ӵ��� ���� ��Ÿ ������ ���ҽ���
	if (IMG_ItemBGPanel.IsValid())
	{
		UMaterialInterface* BGMtrl = ItemMiscInfo ?
			ItemMiscInfo->GetItemIconBGMtrl(
			(IsEquipmentItemByType(InItemData) || IsEssenceItem(InItemData)) ? InItemData.StarGrade : 1,  // ���� �������̳� ������ ������ ���� ����..
				bUseSimpleBGImage,
				(InItemData.InventoryType == EItemInvenType::EIIVT_Consumables))
			: NULL;
		if (BGMtrl)
		{
			IMG_ItemBGPanel->SetBrushFromMaterial(BGMtrl);
		}
	}
	if (IMG_ItemQualityBG.IsValid())
	{
		UMaterialInterface* QualityBGMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemQualityBGMtrl(InItemData.Quality) : NULL;
		if (QualityBGMtrl)
		{
			IMG_ItemQualityBG->SetBrushFromMaterial(QualityBGMtrl);
		}
	}
	if (TB_ItemName.IsValid())
	{
		if (ItemMiscInfo)
		{
			TB_ItemName->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(InItemData.StarGrade));
		}
	}

	SetItemLevel(InItemData.ItemFactorLevel);
	SetStarGrade(InItemData);
	SetItemType(InItemData.ItemClass);
	if(quantity>0) SetEnhanceLevel(quantity,true);
	else SetEnhanceLevel(InItemData.Level);
	SetPrimaryPoint(InItemData.GetPrimaryPoint());
	SetPrimaryPointType(InItemData.PrimaryPointType);

	const bool bIsEquipmentItem = IsEquipmentItem(InItemData);
	const bool bIsEssenceItem = IsEssenceItem(InItemData);
	const bool bIsAnvilItem = IsAnvilItem(InItemData);

	// �Ϲ� ���� ������(Ȥ�� ����������)�� �ƴ� ��� ���ܾ� �� �͵�
	if (TB_ItemLevel.IsValid()) {
		TB_ItemLevel->SetVisibility(bIsEquipmentItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (TB_EnhanceLevel.IsValid()) {
		if (quantity > 0 || bIsEquipmentItem)
			TB_EnhanceLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
		else
			TB_EnhanceLevel->SetVisibility(ESlateVisibility::Hidden);
		
	}
	if (TB_PrimaryPoint.IsValid()) {
		TB_PrimaryPoint->SetVisibility(bIsEquipmentItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (CP_Stars.IsValid()) {
		CP_Stars->SetVisibility((bIsEquipmentItem || bIsEssenceItem || bIsAnvilItem) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (OV_ItemQualitySet.IsValid()) {
		OV_ItemQualitySet->SetVisibility(bIsEquipmentItem && InItemData.Quality > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (TB_ItemQuality.IsValid()) {
		TB_ItemQuality->SetText(GetLOCTextOfItemQuality(InItemData.Quality));
	}
	if (IMG_EtherPos.IsValid())
	{
		IMG_EtherPos->SetVisibility(ESlateVisibility::Hidden);
	}

	SetQuality_BP(InItemData.Quality);
	ShowCount(true);
	SetCount(InItemData.ConsumingAmount);
	SetCountTextColor(InItemData.ItemRefID);

#if !UE_BUILD_SHIPPING
	if (IsDevModeContent() && TB_ItemQuality.IsValid())
		TB_ItemQuality->SetText(FText::FromString(FString::Printf(TEXT("RefId : %d"), InItemData.ItemRefID)));
#endif
}

void UB2UISlotItem::BindDoc(const FB2RewardItemPreviewInfo& ItemPreviewInfoStruct)
{
	UnbindDoc();
	SetDocStruct(ItemPreviewInfoStruct.SomeCommonData);

	class UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable)
	{
		BindItemInfoData(ItemInfoTable->GetInfoData(ItemPreviewInfoStruct.SomeCommonData.ItemRefID), ItemPreviewInfoStruct.SomeCommonData.Quality);
	}

	// �Ʒ��� �ǹ̻� BindItemInfoData �� ���� ������ �͵�.
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo(); // RefID �� ���ӵ��� ���� ��Ÿ ������ ���ҽ���
	if (IMG_ItemBGPanel.IsValid())
	{
		UMaterialInterface* BGMtrl = ItemMiscInfo ?
			ItemMiscInfo->GetItemIconBGMtrl(
			(IsEquipmentItemByType(ItemPreviewInfoStruct.SomeCommonData) || IsEssenceItem(ItemPreviewInfoStruct.SomeCommonData)) ? ItemPreviewInfoStruct.MaxGrade : 1,  // ���� �������̳� ������ ������ ���� ����..
				bUseSimpleBGImage,
				(ItemPreviewInfoStruct.SomeCommonData.InventoryType == EItemInvenType::EIIVT_Consumables))
			: NULL;
		if (BGMtrl)
		{
			IMG_ItemBGPanel->SetBrushFromMaterial(BGMtrl);
		}
	}
	if (IMG_ItemQualityBG.IsValid())
	{
		UMaterialInterface* QualityBGMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemQualityBGMtrl(ItemPreviewInfoStruct.SomeCommonData.Quality) : NULL;
		if (QualityBGMtrl)
		{
			IMG_ItemQualityBG->SetBrushFromMaterial(QualityBGMtrl);
		}
	}
	if (TB_ItemName.IsValid())
	{
		if (ItemMiscInfo)
		{
			TB_ItemName->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(ItemPreviewInfoStruct.MaxGrade));
		}
	}

	SetItemLevel(ItemPreviewInfoStruct.SomeCommonData.ItemFactorLevel);
	SetStageInfoStarGrade(ItemPreviewInfoStruct.MinGrade, ItemPreviewInfoStruct.MaxGrade);
	SetItemType(ItemPreviewInfoStruct.SomeCommonData.ItemClass);
	SetEnhanceLevel(ItemPreviewInfoStruct.SomeCommonData.Level);
	SetPrimaryPoint(ItemPreviewInfoStruct.SomeCommonData.GetPrimaryPoint());
	SetPrimaryPointType(ItemPreviewInfoStruct.SomeCommonData.PrimaryPointType);

	const bool bIsEquipmentItem = IsEquipmentItem(ItemPreviewInfoStruct.SomeCommonData);
	const bool bIsEssenceItem = IsEssenceItem(ItemPreviewInfoStruct.SomeCommonData);
	const bool bIsAnvilItem = IsAnvilItem(ItemPreviewInfoStruct.SomeCommonData);

	// �Ϲ� ���� ������(Ȥ�� ����������)�� �ƴ� ��� ���ܾ� �� �͵�
	if (TB_ItemLevel.IsValid()) {
		TB_ItemLevel->SetVisibility(bIsEquipmentItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (TB_EnhanceLevel.IsValid()) {
		TB_EnhanceLevel->SetVisibility(bIsEquipmentItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (TB_PrimaryPoint.IsValid()) {
		TB_PrimaryPoint->SetVisibility(bIsEquipmentItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (CP_Stars.IsValid()) {
		CP_Stars->SetVisibility((bIsEquipmentItem || bIsEssenceItem || bIsAnvilItem) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (OV_ItemQualitySet.IsValid()) {
		OV_ItemQualitySet->SetVisibility(bIsEquipmentItem && ItemPreviewInfoStruct.SomeCommonData.Quality > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (TB_ItemQuality.IsValid()) {
		TB_ItemQuality->SetText(GetLOCTextOfItemQuality(ItemPreviewInfoStruct.SomeCommonData.Quality));
	}
	if (IMG_EtherPos.IsValid())
	{
		IMG_EtherPos->SetVisibility(ESlateVisibility::Hidden);
	}

	SetQuality_BP(ItemPreviewInfoStruct.SomeCommonData.Quality);
	ShowCount(true);
	SetCount(ItemPreviewInfoStruct.SomeCommonData.ConsumingAmount);
	SetCountTextColor(ItemPreviewInfoStruct.SomeCommonData.ItemRefID);

#if !UE_BUILD_SHIPPING
	if (IsDevModeContent() && TB_ItemQuality.IsValid())
		TB_ItemQuality->SetText(FText::FromString(FString::Printf(TEXT("RefId : %d"), ItemPreviewInfoStruct.SomeCommonData.ItemRefID)));
#endif
}

void UB2UISlotItem::BindByRefID(int32 RefID)
{
	UnbindDoc();

	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(RefID) : NULL;
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo(); // RefID �� ���ӵ��� ���� ��Ÿ ������ ���ҽ���

	BindItemInfoData(ThisItemInfo, 1);

	if (IMG_ItemBGPanel.IsValid())
	{
		UMaterialInterface* BGMtrl = NULL;
		if (BGMtrl)
		{
			IMG_ItemBGPanel->SetBrushFromMaterial(BGMtrl);
		}
	}
	if (IMG_ItemQualityBG.IsValid())
	{
		UMaterialInterface* QualityBGMtrl = NULL;
		if (QualityBGMtrl)
		{
			IMG_ItemQualityBG->SetBrushFromMaterial(QualityBGMtrl);
		}
	}
	if (TB_ItemName.IsValid())
	{
		if (ItemMiscInfo)
		{
			TB_ItemName->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(6));	//�׳� 6���� �ش��ϴ� ���� �ش�.
		}
	}

	if (TB_EnhanceLevel.IsValid())
		TB_EnhanceLevel->SetVisibility(ESlateVisibility::Hidden);
	if (OV_ItemQualitySet.IsValid())
		OV_ItemQualitySet->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UISlotItem::BindEther(const FB2Item& InItemData)
{
	UnbindDoc();

	if (StaticFindEtherInfoTable() != NULL)
	{
		SetIcon(StaticFindEtherInfoTable()->GetEtherIcon(InItemData.Quality));							// Quality = Type
		FSlateColor Color = StaticFindItemMiscInfo()->GetEtherTierTextColor(InItemData.Level);			// Level = Tier
		if (IMG_Icon.IsValid())
		{
			IMG_Icon->SetColorAndOpacity(Color.GetSpecifiedColor());
		}
	}

	SetItemName(B2EtherManager::GetInstance().GetEtherName(InItemData.Quality));
	SetStarGrade(InItemData);

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (IMG_ItemBGPanel.IsValid())
	{
		if (ItemMiscInfo)
		{
			IMG_ItemBGPanel->SetBrushFromMaterial(ItemMiscInfo->GetEtherTierBGMtrl(InItemData.Level));
		}
	}
	if (IMG_ItemQualityBG.IsValid())
	{
		UMaterialInterface* QualityBGMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemQualityBGMtrl(InItemData.Quality) : NULL;
		if (QualityBGMtrl)
		{
			IMG_ItemQualityBG->SetBrushFromMaterial(QualityBGMtrl);
		}
	}
	if (IMG_EtherPos.IsValid())
	{
		if (ItemMiscInfo)
		{
			IMG_EtherPos->SetVisibility(ESlateVisibility::HitTestInvisible);
			IMG_EtherPos->SetBrushFromMaterial(ItemMiscInfo->GetEtherPosBGMtrl(InItemData.SurpassCount));	// SurpassCount = Slot 
		}
	}
	if (TB_ItemName.IsValid())
	{
		if (ItemMiscInfo)
		{
			TB_ItemName->SetColorAndOpacity(ItemMiscInfo->GetEtherTierTextColor(InItemData.Level));
		}
	}

	if (TB_EnhanceLevel.IsValid())
	{
		TB_EnhanceLevel->SetVisibility(ESlateVisibility::Hidden);
	}
	if (OV_ItemQualitySet.IsValid())
	{
		OV_ItemQualitySet->SetVisibility(ESlateVisibility::Hidden);
	}
	if (IMG_ExtraSlotEffect.IsValid())
	{
		IMG_ExtraSlotEffect->SetVisibility(ESlateVisibility::Hidden);
	}

	ShowCount(true);
	SetCount(InItemData.ConsumingAmount);
	SetCountTextColor(InItemData.ItemRefID);
}

void UB2UISlotItem::BindEther(const FB2AetherItemPreviewInfo& ItemPreviewInfoStruct)
{
	UnbindDoc();
	// BattleStageInfo ���� ���������� ���� ���׸� Ÿ�Ը� ������ ĳ��.
	FB2Item InItemData;
	InItemData.Quality = ItemPreviewInfoStruct.AetherType;
	SetDocStruct(InItemData);

	if (StaticFindEtherInfoTable() != NULL)
	{
		SetIcon(StaticFindEtherInfoTable()->GetEtherIcon(ItemPreviewInfoStruct.AetherType));
	}

	SetItemName(B2EtherManager::GetInstance().GetEtherName(ItemPreviewInfoStruct.AetherType));
	SetStageInfoStarGrade(ItemPreviewInfoStruct.MinGrade, ItemPreviewInfoStruct.MaxGrade);

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (IMG_ItemBGPanel.IsValid())
	{
		if (ItemMiscInfo)
		{
			IMG_ItemBGPanel->SetBrushFromMaterial(ItemMiscInfo->GetItemIconBGMtrl(1, bUseSimpleBGImage, false));
		}
	}
	if (TB_ItemName.IsValid())
	{
		if (ItemMiscInfo)
		{
			TB_ItemName->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(1));
		}
	}

	if (TB_EnhanceLevel.IsValid())
	{
		TB_EnhanceLevel->SetVisibility(ESlateVisibility::Hidden);
	}
	if (OV_ItemQualitySet.IsValid())
	{
		OV_ItemQualitySet->SetVisibility(ESlateVisibility::Hidden);
	}
	if (TB_ItemCount.IsValid())
	{
		TB_ItemCount->SetVisibility(ESlateVisibility::Hidden);
	}
	if (IMG_ExtraSlotEffect.IsValid())
	{
		IMG_ExtraSlotEffect->SetVisibility(ESlateVisibility::Hidden);
	}
	if (IMG_EtherPos.IsValid())
	{
		IMG_EtherPos->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UISlotItem::BindItemInfoData(struct FSingleItemInfoData* InItemInfoData, int32 ItemQuality)
{
	// �ǻ� ItemInfo ���� �����͸� ���⼭ �� �����Ϸ��� �߰� �Ű��������� �ʿ��ϴ�.. �̰� �� ������� ���ΰ�.
	if (InItemInfoData)
	{
		SetIcon(InItemInfoData->GetIconMaterial(StaticFindItemInfo()));
		SetItemName(InItemInfoData->GetLocalizedName(false, ItemQuality)); // ItemQuality �ϴ� �̸� ��ü�� �ٿ��� ǥ�������� �ʴ� �ɷ�.		

		if (IMG_ExtraSlotEffect.IsValid())
			IMG_ExtraSlotEffect->SetVisibility((InItemInfoData->bHasExtraSlotEffect) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	else
	{
		UMaterialInterface* NullMtrl = nullptr;
		SetIcon(NullMtrl); // ���� �̷� �ʿ䰡 �ֳ�..
		SetItemName(FText::FromString(TEXT("")));
	}
}

void UB2UISlotItem::UnbindDoc()
{
	BindedStruct.InstanceUID = 0;
	BindedStruct.ItemRefID = 0;
	BindedStruct.SetID = 0;
	BindedStruct.ItemClass = EItemClass::EIC_End;

	BindItemInfoData(nullptr, 0);
}

void UB2UISlotItem::SetItemLevel(int32 InValue)
{
	if (TB_ItemLevel.IsValid())
		TB_ItemLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SlotItem_Level"))), FText::AsNumber(InValue)));
}

void UB2UISlotItem::ShowItemLevel(bool bShow)
{
	if (TB_ItemLevel.IsValid())
		bShow ? TB_ItemLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible) : TB_ItemLevel->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UISlotItem::SetItemName(const FText& InText)
{
	if (TB_ItemName.IsValid())
		TB_ItemName->SetText(InText);
}

void UB2UISlotItem::SetItemType(EItemClass ItemType)
{
	//������ Ÿ�Կ� ���� ó���� �ʿ��ϸ� ���ش�.
}

void UB2UISlotItem::SetStarGrade(const FB2Item& InValue)
{
	if (UIP_Stars.IsValid())
		UIP_Stars->UpdateStar(InValue);

	SetStarGrade_BP(InValue.StarGrade);
}

void UB2UISlotItem::SetStarGrade(const int32 InItemGrade, const bool InIsSurpass)
{
	if (UIP_Stars.IsValid())
		UIP_Stars->UpdateStar(InItemGrade, InIsSurpass);
}

void UB2UISlotItem::SetStageInfoStarGrade(int32 MinValue, int32 MaxValue)
{
	if (TB_StarCount.IsValid())
	{
		if (MinValue == MaxValue)
		{
			TB_StarCount->SetText(FText::AsNumber(MaxValue));
		}
		else
		{
			TB_StarCount->SetText(FText::Format(FText::FromString(TEXT("{0} ~ {1}")), MinValue, MaxValue));
		}
	}
}

void UB2UISlotItem::SetEnhanceLevel(int32 InValue, bool showQuantity)
{
	if (showQuantity == false)
	{
		if (TB_EnhanceLevel.IsValid())
			TB_EnhanceLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SlotItem_Level"))), FText::AsNumber(InValue)));
	}
	else
	{
		if (TB_EnhanceLevel.IsValid())
			TB_EnhanceLevel->SetText(FText::Format(FText::FromString(TEXT("{0}")), InValue));
	}
}

void UB2UISlotItem::SetPrimaryPoint(int32 InValue)
{
	if (TB_PrimaryPoint.IsValid())
		TB_PrimaryPoint->SetText(FText::AsNumber(InValue));
}

void UB2UISlotItem::SetPrimaryPointType(EItemPrimaryPointType InType)
{
	SetPrimaryPointType_BP(InType);
}

void UB2UISlotItem::SetVisibleStar(ESlateVisibility _visible)
{
	if (UIP_Stars.IsValid())
		UIP_Stars->SetVisibility(_visible);

	if (_visible == ESlateVisibility::Collapsed)
		SetStarGrade_BP(0);
}

void UB2UISlotItem::SetVisibleStageInfoItemIcon(ESlateVisibility _visible, bool isBossItem)
{
	if (isBossItem)
	{
		if (IMG_StarIcon.IsValid())
			IMG_StarIcon->SetVisibility(ESlateVisibility::Collapsed);
		if (IMG_BossItemIcon.IsValid())
			IMG_BossItemIcon->SetVisibility(_visible);
	}
	else
	{
		if (IMG_StarIcon.IsValid())
			IMG_StarIcon->SetVisibility(_visible);
		if (IMG_BossItemIcon.IsValid())
			IMG_BossItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (TB_StarCount.IsValid())
		TB_StarCount->SetVisibility(_visible);
}

void UB2UISlotItem::SetCount(const int32 InNum)
{
	Count = InNum;

	if (bCount_0_Icon_Invisible && Count <= 0)
		ShowIcon(false);
	else
		ShowIcon(true);

	if (TB_ItemCount.IsValid())
	{
		auto CountText = FText::AsNumber(InNum).ToString();
		TB_ItemCount->SetText(bUseCountPrefix ? FText::FromString(TEXT("x") + CountText) : FText::AsNumber(InNum));
		((bCount_0_Text_Invisible && InNum <= 0) || !bShowCount) ?
			TB_ItemCount->SetVisibility(ESlateVisibility::Collapsed) :
			TB_ItemCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UISlotItem::ShowCount(const bool bShow)
{
	if (TB_ItemCount.IsValid())
	{
		bShow ?
			TB_ItemCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible) :
			TB_ItemCount->SetVisibility(ESlateVisibility::Collapsed);
		bShowCount = bShow;
	}
}

void UB2UISlotItem::SetCountTextColor(int32 ItemRefID)
{
	if (TB_ItemCount.IsValid())
	{
		if (ItemRefID == FItemRefIDHelper::GetGoodsID_Gold())
			SetItemCountTextColor(UB2UIManager::GetInstance()->TextColor_Gold);
	}
}

void UB2UISlotItem::SetSelected(bool InbSelected)
{
	if (O_Selected.IsValid())
		InbSelected ? O_Selected->SetVisibility(ESlateVisibility::HitTestInvisible) : O_Selected->SetVisibility(ESlateVisibility::Collapsed);

	SetSelected_BP(InbSelected);
}

FB2RewardItemPreviewInfo UB2UISlotItem::GetStageRewardPreviewInfo()
{
	FB2RewardItemPreviewInfo RetInfo;
	RetInfo.SomeCommonData = BindedStruct;
	RetInfo.PrimPointMaxValue = ExtraStageRewardInfo.PrimPointMaxValue;
	RetInfo.PrimPointMinValue = ExtraStageRewardInfo.PrimPointMinValue;
	RetInfo.RandomOptionCount = ExtraStageRewardInfo.RandomOptionCount;

	return RetInfo;
}

UMaterialInterface* UB2UISlotItem::GetActiveItemBGPanelMaterial() const
{
	return IMG_ItemBGPanel.IsValid() ? Cast<UMaterialInterface>(IMG_ItemBGPanel->GetBrush().GetResourceObject()) : NULL;
}

UMaterialInterface* UB2UISlotItem::GetActiveEtherItemBGPanelMaterial() const
{
	return IMG_EtherPos.IsValid() ? Cast<UMaterialInterface>(IMG_EtherPos->GetBrush().GetResourceObject()) : NULL;
}

void UB2UISlotItem::SetShowOverlayInfoOnly(bool bOverlayInfoOnly)
{
	// ���� �������̶� ��׶��� �г� ���� 3D ����Ʈ�� ǥ���ϰ� �̰� �ؽ�Ʈ�� ��Ÿ UI �� ǥ���ϱ� ���� �͸� ǥ���ϱ� ���� �뵵.
	if (IMG_Icon.IsValid())
	{
		IMG_Icon->SetVisibility(bOverlayInfoOnly ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	}

	if (IMG_ItemBGPanel.IsValid())
	{
		IMG_ItemBGPanel->SetVisibility(bOverlayInfoOnly ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	}
	if (UIP_Stars.IsValid())
	{
		UIP_Stars->SetVisibility(bOverlayInfoOnly ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	}
	if (IMG_EtherPos.IsValid())
	{
		IMG_EtherPos->SetVisibility(bOverlayInfoOnly ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	}
}

void UB2UISlotItem::OnSetupManualScrollBoxSender(UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox)
{
	// ��ũ�ѹڽ� �ȿ� ���� ���� ������ ���� �̰� �ʿ�� �ϰ� �� ��.
	// NativeOn** �� �޵��� �ڱ� �ڽ��� Visible ��, ��ư�� Invisible ��..
	if (BTN_Area.IsValid())
	{
		BTN_Area->SetVisibility(ESlateVisibility::HitTestInvisible);

		// OnClickBtnArea ���ε��� ����.
		//BTN_Area->OnClicked.RemoveDynamic(this, &UB2UISlotIcon::OnClickBtnArea); ��, protected ��.. ������ HitTestInvisible �̴� ���ֺ���.
	}
	this->SetVisibility(ESlateVisibility::Visible);
}

//====================================================================================
//									Click Handler
//====================================================================================


void UB2UISlotItem::OnClickedSenderBTNArea()
{ // ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�. ManualScrollBoxSender �¾��� �Ǿ� �ִ� ��츸 ���� ���ؼ�
	Super::OnClickedSenderBTNArea();

	OnClickBtnArea(); // ���� Ŭ�� �ڵ鸵
}
void UB2UISlotItem::OnPressedSenderBTNArea()
{// ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�
	Super::OnPressedSenderBTNArea();

	OnPressedBtnAreaBP();
}
void UB2UISlotItem::OnReleasedSenderBTNArea()
{// ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�
	Super::OnReleasedSenderBTNArea();

	OnReleasedBtnAreaBP();
}

void UB2UISlotItem::OnClickBtnArea()
{
	if (GetClickable())
		OnSlotItemClickDelgate.ExecuteIfBound(this);

	OnClickedBtnAreaBP();
}

void UB2UISlotItem::SetItemCountTextColor(FSlateColor sColor)
{
	if (TB_ItemCount.IsValid())
		TB_ItemCount->SetColorAndOpacity(sColor);
}

void UB2UISlotItem::SetEnableReadingGlasses(bool Enable)
{
	if (IMG_Reading_Glasses.IsValid())
		IMG_Reading_Glasses->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UISlotItem::SetBTNArea(bool InValue)
{
	if (BTN_Area.IsValid())
	{
		BTN_Area->SetVisibility(InValue ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
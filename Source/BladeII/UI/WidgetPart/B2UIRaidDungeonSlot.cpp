#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIDocHelper.h"
#include "B2RichTextBlock.h"
#include "B2ItemInfo.h"
#include "B2RewardInfo.h"
#include "B2UIRaidDungeonSlot.h"


#include "BladeIIUtil.h"

void UB2UIRaidDungeonSlot::Init()
{
	Super::Init();

	RaidMapId = -1;
	RewardItemIdx = 0;
	bIsEnableButton = false;
}

void UB2UIRaidDungeonSlot::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_DungeonTitle);
	GET_SLOT(UTextBlock, TB_DungeonTitle2);
	GET_SLOT(UTextBlock, TB_RewardItem);
	GET_SLOT(UTextBlock, TB_RewardItem2);
	GET_SLOT(UB2RichTextBlock, RTB_ClosePossibilityTime);
	GET_SLOT(UButton, BTN_Select);
	GET_SLOT(UImage, IMG_BG);
	GET_SLOT(UImage, IMG_BG2);
	GET_SLOT(UImage, IMG_Item);
	GET_SLOT(UImage, IMG_Item2);
	GET_SLOT(UHorizontalBox, H_RewardItem2);
}

void UB2UIRaidDungeonSlot::UpdateStaticText()
{
}

void UB2UIRaidDungeonSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIRaidDungeonSlot::OnClickSelect)
}

void UB2UIRaidDungeonSlot::OnClickSelect()
{
	if (!bIsEnableButton)
		return;

	FOnClickBtnSelect.ExecuteIfBound(RaidMapId);
}

void UB2UIRaidDungeonSlot::SetEnableSlot(bool IsEnable, FText Contenttext)
{
	SetEnableSlot_BP(IsEnable);

	if (!IsEnable)
	{
		if (RTB_ClosePossibilityTime.IsValid())
		{
			RTB_ClosePossibilityTime->SetText(Contenttext);
		}
	}

	bIsEnableButton = IsEnable;
}

void UB2UIRaidDungeonSlot::SetBGImage(int32 InRaidMapId)
{
	if (InRaidMapId == -1)
		return;

	int32 ImageIdx = InRaidMapId-1;
	if (BGTextures.IsValidIndex(ImageIdx) == false || ImageIdx < 0)
		return;
	
	if (IMG_BG.IsValid())
		IMG_BG->SetBrushFromMaterial(BGTextures[ImageIdx]);
	if (IMG_BG2.IsValid())
		IMG_BG2->SetBrushFromMaterial(BGTextures[ImageIdx]);
}

void UB2UIRaidDungeonSlot::SetRewardItemAccesory()
{
	FString ItemName = FString();
	auto* ItemMiscInfo = StaticFindItemMiscInfo();
	if (ItemMiscInfo)
		ItemName = ItemMiscInfo->GetEquipCategoryName(EEquipCategoryType::ACCESSORY).ToString();

	if (IMG_Item.IsValid())
		IMG_Item->SetBrushFromMaterial(GetIconMaterial(EEquipCategoryType::ACCESSORY));
	if (IMG_Item2.IsValid())
		IMG_Item2->SetBrushFromMaterial(GetIconMaterial(EEquipCategoryType::ACCESSORY));

	if (TB_RewardItem.IsValid())
		TB_RewardItem->SetText(FText::FromString(ItemName));
	if (TB_RewardItem2.IsValid())
		TB_RewardItem2->SetText(FText::FromString(ItemName));
}

void UB2UIRaidDungeonSlot::SetRewardItem(int32 RewardId)
{
	UMaterialInterface* IconImg = nullptr;
	FText ItemName = FText::GetEmpty();

	auto* RewardInfoTable = StaticFindRewardInfo();
	if (RewardInfoTable)
	{
		IconImg = RewardInfoTable->GetRewardIconMaterial(RewardId);

		auto* RewardInfo = RewardInfoTable->GetInfoData(RewardId);
		if(RewardInfo)
			ItemName = BladeIIGetLOCText(RewardInfo->TextCategory, RewardInfo->RewardNameForTextTable);
	}

	if (IMG_Item.IsValid())
		IMG_Item->SetBrushFromMaterial(IconImg);
	if (IMG_Item2.IsValid())
		IMG_Item2->SetBrushFromMaterial(IconImg);
	if (TB_RewardItem.IsValid())
		TB_RewardItem->SetText(ItemName);
	if (TB_RewardItem2.IsValid())
		TB_RewardItem2->SetText(ItemName);
}

void UB2UIRaidDungeonSlot::SetRaidName(FText InDungeonName)
{
	if (InDungeonName.IsEmpty())
		return;

	if (TB_DungeonTitle.IsValid())
		TB_DungeonTitle->SetText(InDungeonName);
	if (TB_DungeonTitle2.IsValid())
		TB_DungeonTitle2->SetText(InDungeonName);
}

void UB2UIRaidDungeonSlot::SetRaidId(int32 InRaidId)
{
	RaidMapId = InRaidId;
	SetBGImage(InRaidId);	
}

UMaterialInterface* UB2UIRaidDungeonSlot::GetIconMaterial(EEquipCategoryType EquipCategory)
{
	return StaticFindItemMiscInfo() ? StaticFindItemMiscInfo()->GetEquipCategoryIcon(EquipCategory) : nullptr;
}

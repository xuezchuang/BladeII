// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynItemIcon_LobbyEquip.h"
//#include "BladeII.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"
#include "B2UIUtil.h"

UB2DynItemIcon_LobbyEquip::UB2DynItemIcon_LobbyEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EquipItemIconType = ELobbyEquipItemIconType::EEIIT_LobbyInven;

	IMG_StatInc_NRef = NULL;
	TB_StatIncAmount_NRef = NULL;
}

void UB2DynItemIcon_LobbyEquip::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ItemLevel);
	GET_SLOT(UTextBlock, TB_EnhanceLevel);
	GET_SLOT(UTextBlock, TB_StartGrade);
	
	GET_SLOT(UImage, IMG_GradeBG);
	GET_SLOT(UImage, IMG_SelectFx);
	GET_SLOT(UImage, IMG_Start); 

	SubscribeEvents();
}

void UB2DynItemIcon_LobbyEquip::UpdateItemData(const FB2Item& InItem)
{
	Super::UpdateItemData(InItem);

	if (TB_ItemLevel.IsValid())
	{
		TB_ItemLevel->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(NativeItemData.Level)))
			);
	}

	if (TB_EnhanceLevel.IsValid())
	{
		TB_EnhanceLevel->SetText(
			NativeItemData.EnhanceLevel > 0 ? FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_EnhanceLevel"))), FText::FromString(FString::FromInt(NativeItemData.EnhanceLevel))) : FText()
		);
	}
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (ItemMiscInfo)
	{
		if (IMG_Start.IsValid())
		{
			UMaterialInterface* StarMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemStarGradeImage(InItem.SurpassCount > 0) : NULL;
			if (StarMtrl)
				IMG_Start->SetBrushFromMaterial(StarMtrl);
		}

		if (TB_StartGrade.IsValid())
		{
			TB_StartGrade->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(InItem.StarGrade));
			TB_StartGrade->SetText(FText::AsNumber(InItem.StarGrade));
		}
	}
}

void UB2DynItemIcon_LobbyEquip::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
{
	ensure(!bForConsumables); // 만일 이게 들어왔다면 소모품을 장착하려고 했다기보단 아마도 뭔가 더미 데이터로 표시하려는데 값이 잘못 세팅되어 있다거나.. 걍 신경쓰지 않았던 값일 듯..?

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (IMG_GradeBG.IsValid())
	{
		UMaterialInterface* BGMtrl = ItemMiscInfo ? ItemMiscInfo->GetEquipIconBGMtrl(InItemGrade) : NULL;
		if (BGMtrl)
		{
			IMG_GradeBG->SetBrushFromMaterial(BGMtrl);
		}
	}
}

void UB2DynItemIcon_LobbyEquip::DestroySelf()
{
	UnsubscribeEvents();

	Super::DestroySelf();
}

void UB2DynItemIcon_LobbyEquip::UpdateStatIncSigns(FB2Item& CurrenEquipToCompare)
{
	if (EquipItemIconType == ELobbyEquipItemIconType::EEIIT_AutoEquipSuggest &&  // 장착권장용 아이콘인 경우에 대해서만 비교하는 이미지 띄움.
		NativeItemData.InstanceUID > 0 && NativeItemData.ItemRefID > 0 // ID 들이 세팅되었는지 확인 차원에서..
		//CurrenEquipToCompare.EquipPlace == NativeItemData.EquipPlace && CurrenEquipToCompare.bCurrentlyEquipped // 실제로 같은 위치에 장착되어 있는 것인지.. 는 빈 것과 비교할 수도 있으므로 그냥 두자.
		) 
	{
		int32 PrimPointInc = B2UIUtil::DifferenceItemPrimaryPoint(NativeItemData, CurrenEquipToCompare);
		if (PrimPointInc > 0) // PrimPointInc 가 0 이하라면 뭔가 잘못된 거
		{
			if (IMG_StatInc_NRef){ 
				IMG_StatInc_NRef->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			if (TB_StatIncAmount_NRef){
				TB_StatIncAmount_NRef->SetVisibility(ESlateVisibility::HitTestInvisible);
				TB_StatIncAmount_NRef->SetText(FText::FromString(FString::Printf(
					TEXT("+%d"), PrimPointInc
					)));
			}
		}
	}
}

void UB2DynItemIcon_LobbyEquip::SetEquipItemIconType(ELobbyEquipItemIconType InType)
{
	EquipItemIconType = InType;


	if (IMG_SelectFx.IsValid())
		IMG_SelectFx->SetVisibility(InType == ELobbyEquipItemIconType::EEIIT_AutoEquipCurrent ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (EquipItemIconType != ELobbyEquipItemIconType::EEIIT_AutoEquipSuggest)
	{ // AutoEquipSuggest 라면 UpdateStatIncSigns 를 통해 세팅해 주어야 함.
		if (IMG_StatInc_NRef){
			IMG_StatInc_NRef->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TB_StatIncAmount_NRef){
			TB_StatIncAmount_NRef->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	//woong
}

void UB2DynItemIcon_LobbyEquip::OnDetailInfoButtonPressed()
{
	// 현재 NativeItemData 를 사용해서 세부 정보창 팝업.
	// UB2DynItemIcon_LobbyInven::OnDetailInfoButtonPressed 과 마찬가지로..
	OnClickDetailInfo.ExecuteIfBound();

	if (EquipItemIconType != ELobbyEquipItemIconType::EEIIT_LobbyInven)
	{ // 자동 장착 창이면 처음부터 눌리지 않도록 하는게 좋을 것.
		return;
	}

	CreateLobbyItemDetailPopupCommon(GetOwningPlayer(), NativeItemData, true); // Equip 용으로 detail 창 생성
}

void UB2DynItemIcon_LobbyEquip::UpdateItemEquipSlotVisible(const bool& bIsVisibleCostumeSlot)
{
	const bool bIsItemEquipSlotVisible = bIsVisibleCostumeSlot ? false : true;

	PlayChangeAnimEquipSlotOrCostumeSlot_BP(bIsItemEquipSlotVisible);
}

void UB2DynItemIcon_LobbyEquip::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(CostumeEquipSlotVisibleClass<bool>::GetInstance().Subscribe2(
	[this](bool IsVisible)
	{
		this->UpdateItemEquipSlotVisible(IsVisible);
	}
	));
}

void UB2DynItemIcon_LobbyEquip::UnsubscribeEvents()
{
	Issues.Empty();
}
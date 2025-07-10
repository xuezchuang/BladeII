// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UILobbyCharInfo.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "B2DynItemIcon_LobbyEquip.h"
#include "B2LobbyUI_CharStatDetail.h"



#include "BladeIIGameImpl.h"

#define GET_SLOT_ITEMSLOT(EquipPlace)		\
	GET_SLOT(UB2Button, BTN_##EquipPlace);	\
	if (BTN_##EquipPlace.IsValid())			\
		ItemCanvasPanels.Add(EItemEquipPlace::EIEP_##EquipPlace, Cast<UCanvasPanel>(BTN_##EquipPlace->GetParent()));	\
	GET_SLOT(UImage, IMG_Selected_##EquipPlace);	\
	if (IMG_Selected_##EquipPlace.IsValid())		\
		SelectedImages.Add(EItemEquipPlace::EIEP_##EquipPlace, IMG_Selected_##EquipPlace.Get());	\
	GET_SLOT(UTextBlock, TB_##EquipPlace);	\
	if (TB_##EquipPlace.IsValid())		\
		SlotEquipNames.Add(EItemEquipPlace::EIEP_##EquipPlace, TB_##EquipPlace.Get());

void UB2UILobbyCharInfo::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_BG);

	GET_SLOT_ITEMSLOT(Helmet);
	GET_SLOT_ITEMSLOT(BodyArmor);
	GET_SLOT_ITEMSLOT(Glove);
	GET_SLOT_ITEMSLOT(Shoes);
	GET_SLOT_ITEMSLOT(Weapon);
	GET_SLOT_ITEMSLOT(Belt);
	GET_SLOT_ITEMSLOT(Necklace);
	GET_SLOT_ITEMSLOT(Ring);

	GET_SLOT(UTextBlock, TB_CharName);
	GET_SLOT(UTextBlock, TB_CharLevel);
	GET_SLOT(UTextBlock, TB_CharExp);

	GET_SLOT(UImage, IMG_CharTitle);
	GET_SLOT(UTextBlock, TB_CharTitle);
	GET_SLOT(UProgressBar, PB_CharExp);

	GET_SLOT(UTextBlock, TB_HP);

	GET_SLOT(UTextBlock, TB_ATK);

	GET_SLOT(UTextBlock, TB_DEF);

	GET_SLOT(UB2Button, BTN_ToggleCostume);
	GET_SLOT(UB2Button, BTN_DetailStat);

	GET_SLOT(UB2Button, BTN_ItemDetailBG);
	GET_SLOT(UB2LobbyUI_ItemDetailPostOp, UIP_ItemDetail);	

	GET_SLOT(UB2Button, BTN_CharDetailBG);
	GET_SLOT(UB2LobbyUI_CharStatDetail, UIP_CharDetail);

	GET_SLOT(UTextBlock, TB_ToggleCostume);
}

void UB2UILobbyCharInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Helmet, &UB2UILobbyCharInfo::OnClickBTN_Helmet);
	BIND_CLICK_FUNC_TO_BTN(BTN_BodyArmor, &UB2UILobbyCharInfo::OnClickBTN_BodyArmor);
	BIND_CLICK_FUNC_TO_BTN(BTN_Glove, &UB2UILobbyCharInfo::OnClickBTN_Glove);
	BIND_CLICK_FUNC_TO_BTN(BTN_Shoes, &UB2UILobbyCharInfo::OnClickBTN_Shoes);
	BIND_CLICK_FUNC_TO_BTN(BTN_Weapon, &UB2UILobbyCharInfo::OnClickBTN_Weapon);
	BIND_CLICK_FUNC_TO_BTN(BTN_Belt, &UB2UILobbyCharInfo::OnClickBTN_Belt);
	BIND_CLICK_FUNC_TO_BTN(BTN_Necklace, &UB2UILobbyCharInfo::OnClickBTN_Necklace);
	BIND_CLICK_FUNC_TO_BTN(BTN_Ring, &UB2UILobbyCharInfo::OnClickBTN_Ring);
	BIND_CLICK_FUNC_TO_BTN(BTN_ToggleCostume, &UB2UILobbyCharInfo::OnClickBTN_ToggleCostume);
	BIND_CLICK_FUNC_TO_BTN(BTN_DetailStat, &UB2UILobbyCharInfo::OnClickBTN_DetailStat);

	if (UIP_ItemDetail.IsValid())
	{
		UIP_ItemDetail->OnConfirmButtonClickedDelegate.RemoveAll(this);
		UIP_ItemDetail->OnConfirmButtonClickedDelegate.AddDynamic(this, &UB2UILobbyCharInfo::ClosePopupItemInfo);
	}

	if (UIP_CharDetail.IsValid())
	{
		UIP_CharDetail->SetDelegateOnClose(FDelegateCharStatDetailClose::CreateLambda([this](){this->ClosePopupCharStat(); }));
	}
}

void UB2UILobbyCharInfo::Init()
{
	Super::Init();

	ClosePopupCharStat();
	ClosePopupItemInfo();
}

void UB2UILobbyCharInfo::OpenCharInfo(EPCClass PCClassToShow)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	OnOpenAnim();

	CurrentPCClass = PCClassToShow;

	SetShowCostume(false);	
}

void UB2UILobbyCharInfo::CloseCharInfo()
{
	ClosePopupCharStat();
	ClosePopupItemInfo();

	OnCloseAnim();
}

void UB2UILobbyCharInfo::OnFinishedCloseAnim()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

UB2Button* UB2UILobbyCharInfo::GetBGButton()
{
	return BTN_BG.Get();
}

void UB2UILobbyCharInfo::SetInvetorySlot()
{
	EquippedItems.Empty();
	
	auto& CharacterDataStore = BladeIIGameImpl::GetLocalCharacterData();

	if (bShowCostume)
	{

	}
	else
	{
		CharacterDataStore.GetEquippedItems(CurrentPCClass, EquippedItems);
	}	
	
	for (int32 idx = 0; idx < static_cast<int32>(EItemEquipPlace::EIEP_End); ++idx)
	{
		EItemEquipPlace CurrentEquipPlace = static_cast<EItemEquipPlace>(idx);

		{//기존 아이콘 삭제
			UB2DynItemIcon_LobbyEquip** DynIcon = DynIcons.Find(CurrentEquipPlace);
			if (DynIcon)
			{
				UCanvasPanel** ParentPanel = ItemCanvasPanels.Find(CurrentEquipPlace);
				if (ParentPanel && *ParentPanel)
				{
					(*ParentPanel)->RemoveChild(*DynIcon);
				}

				DynIcons.Remove(CurrentEquipPlace);
			}			
		}

		{//슬롯 아이콘 생성
			FB2Item* EquippedItem = EquippedItems.Find(CurrentEquipPlace);
			if (EquippedItem)
			{
				auto* ItemIcon = CreateWidget<UB2DynItemIcon_LobbyEquip>(GetWorld(), EquipItemIconWidgetClass);
				if (ItemIcon)
				{
					UCanvasPanel** ParentPanel = ItemCanvasPanels.Find(CurrentEquipPlace);
					if (ParentPanel && *ParentPanel)
					{
						auto* CanvasSlot = Cast<UCanvasPanelSlot>((*ParentPanel)->AddChild(ItemIcon));
						BII_CHECK(CanvasSlot);
						if (CanvasSlot)
						{
							CanvasSlot->SetSize(FVector2D::ZeroVector);
							CanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
						}
					}

					ItemIcon->SetEquipItemIconType(ELobbyEquipItemIconType::EEIIT_LobbyInven);
					ItemIcon->UpdateItemData(*EquippedItem);

					ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible); //Hit처리는 따로함.

					DynIcons.Add(CurrentEquipPlace, ItemIcon);
				}
			}

			UTextBlock** EquipText = SlotEquipNames.Find(CurrentEquipPlace);
			if (EquipText && *EquipText)
			{
				(*EquipText)->SetVisibility(EquippedItem ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
			}
		}

	}

	if (TB_CharName.IsValid())
		TB_CharName->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(CurrentPCClass));

	if (TB_CharLevel.IsValid())
		TB_CharLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(CharacterDataStore.GetCharacterLevel(CurrentPCClass))));

	if (TB_CharExp.IsValid())
	{
		FNumberFormattingOptions FormatOption;
		FormatOption.MaximumFractionalDigits = 1;
		TB_CharExp->SetText(FText::Format(FText::FromString(TEXT("({0}%)")), FText::AsNumber(CharacterDataStore.GetCharacterExpPercent(CurrentPCClass), &FormatOption)));
	}
		

	if (PB_CharExp.IsValid())
		PB_CharExp->SetPercent(CharacterDataStore.GetCharacterExpPercent(CurrentPCClass));

	auto* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(CurrentPCClass));
	if (DocHero)
	{
		if (TB_HP.IsValid())
			TB_HP->SetText(FText::AsNumber(DocHero->GetHealthPoint()));

		if (TB_ATK.IsValid())
			TB_ATK->SetText(FText::AsNumber(DocHero->GetAttackPower()));

		if (TB_DEF.IsValid())
			TB_DEF->SetText(FText::AsNumber(DocHero->GetDefensePower()));
	}
		
	if (TB_CharTitle.IsValid())
	{
		int32 CurrentCharBrevetRank = CharacterDataStore.GetCharBrevetRank(CurrentPCClass);
		FString RankNameKey = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(CurrentCharBrevetRank);
		TB_CharTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), RankNameKey));
	}
	
	if (IMG_CharTitle.IsValid())
	{
		int32 CurrentCharBrevetRank = CharacterDataStore.GetCharBrevetRank(CurrentPCClass);
		UMaterialInterface* pMI = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(CurrentCharBrevetRank, false);
		IMG_CharTitle->SetVisibility(pMI ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		IMG_CharTitle->SetBrushFromMaterial(pMI);
	}
}

void UB2UILobbyCharInfo::OnClickBTN_Helmet()
{	
	auto* Item = EquippedItems.Find(EItemEquipPlace::EIEP_Helmet);
	if (Item)
	{
		ShowPopupItemInfo(*Item);		
	}	
}

void UB2UILobbyCharInfo::OnClickBTN_BodyArmor()
{
	auto* Item = EquippedItems.Find(EItemEquipPlace::EIEP_BodyArmor);
	if (Item)
	{
		ShowPopupItemInfo(*Item);
	}
}

void UB2UILobbyCharInfo::OnClickBTN_Glove()
{
	auto* Item = EquippedItems.Find(EItemEquipPlace::EIEP_Glove);
	if (Item)
	{
		ShowPopupItemInfo(*Item);
	}
}

void UB2UILobbyCharInfo::OnClickBTN_Shoes()
{
	auto* Item = EquippedItems.Find(EItemEquipPlace::EIEP_Shoes);
	if (Item)
	{
		ShowPopupItemInfo(*Item);
	}
}

void UB2UILobbyCharInfo::OnClickBTN_Weapon()
{
	auto* Item = EquippedItems.Find(EItemEquipPlace::EIEP_Weapon);
	if (Item)
	{
		ShowPopupItemInfo(*Item);
	}
}

void UB2UILobbyCharInfo::OnClickBTN_Belt()
{
	auto* Item = EquippedItems.Find(EItemEquipPlace::EIEP_Belt);
	if (Item)
	{
		ShowPopupItemInfo(*Item);
	}
}

void UB2UILobbyCharInfo::OnClickBTN_Necklace()
{
	auto* Item = EquippedItems.Find(EItemEquipPlace::EIEP_Necklace);
	if (Item)
	{
		ShowPopupItemInfo(*Item);
	}
}

void UB2UILobbyCharInfo::OnClickBTN_Ring()
{
	auto* Item = EquippedItems.Find(EItemEquipPlace::EIEP_Ring);
	if (Item)
	{
		ShowPopupItemInfo(*Item);
	}
}

void UB2UILobbyCharInfo::OnClickBTN_ToggleCostume()
{
	SetShowCostume(!bShowCostume);
}

void UB2UILobbyCharInfo::OnClickBTN_DetailStat()
{
	ShowPopupCharStat();
}

void UB2UILobbyCharInfo::ShowPopupCharStat()
{
	if (UIP_CharDetail.IsValid())
	{
		UIP_CharDetail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UIP_CharDetail->UpdateStaticTexts(); // Init 시점이어야 하지만 딱히 적당한 장소가 없군. 여기서 DJLegacy 를 쓰려니..
		UIP_CharDetail->UpdateStats(CurrentPCClass);
	}		

	if (BTN_CharDetailBG.IsValid())
		BTN_CharDetailBG->SetVisibility(ESlateVisibility::Visible);
}

void UB2UILobbyCharInfo::ClosePopupCharStat()
{
	if (UIP_CharDetail.IsValid())
	{
		UIP_CharDetail->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (BTN_CharDetailBG.IsValid())
		BTN_CharDetailBG->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UILobbyCharInfo::SetShowCostume(bool bToSet)
{
	bShowCostume = bToSet;

	if (TB_ToggleCostume.IsValid())
	{
		if (bShowCostume)
			TB_ToggleCostume->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterSimpleInfo_ToggleCostume_ToEquipped")));
		else
			TB_ToggleCostume->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterSimpleInfo_ToggleCostume_ToCostume")));
	}

	SetInvetorySlot();
}

void UB2UILobbyCharInfo::ShowPopupItemInfo(const FB2Item& ItemToShow)
{
	if (CurrentViewEquipPlace == ItemToShow.EquipPlace)
		return;

	CurrentViewEquipPlace = ItemToShow.EquipPlace;

	if (BTN_ItemDetailBG.IsValid())
		BTN_ItemDetailBG->SetVisibility(ESlateVisibility::Visible);

	if (UIP_ItemDetail.IsValid())
		UIP_ItemDetail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UIP_ItemDetail->UpdateItemData(ItemToShow, ItemToShow);
	
	for (TMap<EItemEquipPlace, UImage*>::TConstIterator It(SelectedImages); It; ++It)
	{
		UImage* SelectedImage = It.Value();
		SelectedImage->SetVisibility(ItemToShow.EquipPlace == It.Key() ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	OnItemClickedAnimStart(ItemToShow.EquipPlace);
}

void UB2UILobbyCharInfo::ClosePopupItemInfo()
{
	if (BTN_ItemDetailBG.IsValid())
		BTN_ItemDetailBG->SetVisibility(ESlateVisibility::Collapsed);

	if (UIP_ItemDetail.IsValid())
		UIP_ItemDetail->SetVisibility(ESlateVisibility::Collapsed);

	for (TMap<EItemEquipPlace, UImage*>::TConstIterator It(SelectedImages); It; ++It)
	{
		UImage* SelectedImage = It.Value();
		SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	CurrentViewEquipPlace = EItemEquipPlace::EIEP_End;
}

#undef GET_SLOT_ITEMSLOT
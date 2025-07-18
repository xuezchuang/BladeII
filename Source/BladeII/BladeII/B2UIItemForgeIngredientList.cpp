// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIItemForgeIngredientList.h"

#include "B2UIManager.h"
#include "B2UIItemForge.h"
#include "B2UIStarGradePart.h"
#include "B2LobbyGameMode.h"
#include "B2LobbySceneManager.h"
#include "B2LobbyInventory.h"
#include "B2UIItemForgePopup.h"
#include "BladeIIUtil.h"
#include "B2ItemInfo.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUser.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
#include "Event.h"


void UB2UIItemForgeIngredientList::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UImage, IMG_SelectedFrame);
	GET_SLOT(UImage, IMG_ItemIcon);
	GET_SLOT(UScaleBox, SB_StarGrade);
	GET_SLOT(UB2UIStarGradePart, StarGradePart);
	if (StarGradePart.IsValid())
	{
		StarGradePart->Init();
	}
	GET_SLOT(UTextBlock, TB_ItemName);

	GET_SLOT(UTextBlock, TB_Quantity);

	GET_SLOT(UTextBlock, TB_Select);
	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UHorizontalBox, HB_ButtonBox);	//디자인쪽에서 버튼 안에다가 안넣는 바람에 따로 뺀걸 조작하는걸로 함.

	GET_SLOT(UB2Button, BTN_Confirm);
}

void UB2UIItemForgeIngredientList::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIItemForgeIngredientList::OnClickBTN_Select);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIItemForgeIngredientList::OnClickBTN_Confirm);
}

void UB2UIItemForgeIngredientList::UpdateStaticText()
{

}

void UB2UIItemForgeIngredientList::Init()
{
	Super::Init();

	requiredQuantity = 1;
	isConsumable = false;
}

void UB2UIItemForgeIngredientList::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIItemForgeIngredientList::OnClickBTN_Select()
{
	//ask for item change
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(slotClass);

	UB2UIItemForgePopup* ingredientPopup = CreateWidget<UB2UIItemForgePopup>(GetWorld(), result);
	ingredientPopup->InitializeForgePopup(slotClass, forgeID, IngredientSlot, mainWantsThisMany, firstMaterialID, originalRequirement,this,mainItemEnhanceLevel,mainItemLevel);

	ingredientPopup->SetVisibility(ESlateVisibility::Visible);
	ingredientPopup->AddToViewport(100);
}

void UB2UIItemForgeIngredientList::OnClickBTN_Confirm()
{

}

void UB2UIItemForgeIngredientList::SetAnotherMaterialItem(FB2Item setAsThis, int32 newRequiredQuantity)
{
	requiredQuantity = 1;
	isConsumable = false;

	SelectedItem = setAsThis;
	singleRequiredQuantity = newRequiredQuantity;
	requiredQuantity = newRequiredQuantity;

	if (SelectedItem.ConsumingAmount > 0) isConsumable = true;
		

	UMaterialInstance* IconMtrl = nullptr;
	FText ItemName;
	StaticFindItemInfo()->GetItemIconAndName(SelectedItem.ItemRefID, IconMtrl, ItemName);
	SetIconMaterial(IconMtrl);
	SetStarGrade(SelectedItem.StarGrade, SelectedItem.SurpassCount > 0);
	TB_ItemName->SetText(ItemName);

	SetQuantity(true);
}

void UB2UIItemForgeIngredientList::SetOwnerItemForgeUI(class UB2UIItemForge* OwnerUI, EItemForgeSlot IngredientSlot_, int32 materialID_, int32 forgeID_, EPCClass pcClass, int32 pleaseForgeThisMany, int32 FirstRequiredQuantity, int32 requiredEnhanceLevel, int32 requiredItemLevel)
{
	SetStarGrade(0, false);
	ItemForgeMainUI = OwnerUI;
	IngredientSlot = IngredientSlot_;
	mainWantsThisMany = pleaseForgeThisMany;
	forgeID = forgeID_;
	slotClass = pcClass;

	mainItemEnhanceLevel = requiredEnhanceLevel;
	mainItemLevel = requiredItemLevel;

	SelectedItem.Empty();

	singleRequiredQuantity = FirstRequiredQuantity;
	originalRequirement = FirstRequiredQuantity;
	firstMaterialID = materialID_;

	bool weGotThisBro = false;

	SelectedItem = FindItemByRefID(materialID_);
	if (isConsumable == true)
		weGotThisBro = true;

	isConsumable = false;

	TArray<FItemForgeMaterialTransInfo> materialTransTable;
	BladeIIGameImpl::GetClientDataStore().GetItemForgeMaterialTransInfo(forgeID, materialTransTable);

	if (IMG_SelectedFrame.IsValid())
		IMG_SelectedFrame->SetVisibility(ESlateVisibility::Collapsed);

	if(HB_ButtonBox.IsValid())
		HB_ButtonBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TB_Quantity->SetText(FText::GetEmpty());
	
	if (SelectedItem.IsEmpty())
	{
		bool thereItIs = false;
		for (int32 i = 0; i < materialTransTable.Num(); i++)
		{
			switch (IngredientSlot_) {
			case EItemForgeSlot::MainMaterialItem: SelectedItem = FindItemByRefID(materialTransTable[i].mainMaterialItemID); if (isConsumable == true) requiredQuantity = materialTransTable[i].mainMaterialItemQuantity; break;
			case EItemForgeSlot::Sub1stMaterialItem: SelectedItem = FindItemByRefID(materialTransTable[i].material1stItemID); if (isConsumable == true) requiredQuantity = materialTransTable[i].material1stItemQuantity; break;
			case EItemForgeSlot::Sub2ndMaterialItem: SelectedItem = FindItemByRefID(materialTransTable[i].material2ndItemID); if (isConsumable == true) requiredQuantity = materialTransTable[i].material2ndItemQuantity; break;
			case EItemForgeSlot::Sub3rdMaterialItem: SelectedItem = FindItemByRefID(materialTransTable[i].material3rdItemID); if (isConsumable == true) requiredQuantity = materialTransTable[i].material3rdItemQuantity; break;
			case EItemForgeSlot::Sub4thMaterialItem: SelectedItem = FindItemByRefID(materialTransTable[i].material4thItemID); if (isConsumable == true) requiredQuantity = materialTransTable[i].material4thItemQuantity; break;
			}
			if (!SelectedItem.IsEmpty())
			{
				thereItIs = true;
				singleRequiredQuantity = requiredQuantity;
				break;
			}
		}

		if (thereItIs == false)
		{

			//mate, player doesn't have a damn.
			UMaterialInstance* IconMtrl = nullptr;
			FText ItemName;
			StaticFindItemInfo()->GetItemIconAndName(materialID_, IconMtrl, ItemName);
			TB_ItemName->SetText(ItemName);
			SetIconMaterial(IconMtrl);
			int32 starGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(materialID_);
			FB2Item dummyItem;
			SetStarGrade(starGrade, requiredItemLevel>dummyItem.ItemNormalMaxLevel ? true : false);	//영원의 장신구의 경우도 그냥 30 넘어가면 초월 별로 띄우게 됨.

			if (starGrade == 0)
			{
				if (BTN_Select.IsValid())
				{
					BTN_Select->SetVisibility(ESlateVisibility::Collapsed);
					HB_ButtonBox->SetVisibility(ESlateVisibility::Collapsed);
				}
				TB_Quantity->SetText(FText::FromString(FString::FromInt(0) + " / " + FString::FromInt(singleRequiredQuantity * mainWantsThisMany)));
				TB_Quantity->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
			}
			else
			{
				if (BTN_Select.IsValid())
				{
					BTN_Select->SetIsEnabled(false);
				}
				TB_Quantity->SetText(FText::GetEmpty());
			}

			return;
		}
	}
	if (weGotThisBro == true)
	{
		requiredQuantity = FirstRequiredQuantity;
		requiredQuantity *= mainWantsThisMany;
		isConsumable = true;
	}

	HB_ButtonBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (BTN_Select.IsValid())
	{
		BTN_Select->SetIsEnabled(true);
	}

	//okay, player has it.
	UMaterialInstance* IconMtrl = nullptr;
	FText ItemName;
	StaticFindItemInfo()->GetItemIconAndName(SelectedItem.ItemRefID, IconMtrl, ItemName);
	SetIconMaterial(IconMtrl);
	SetStarGrade(SelectedItem.StarGrade, SelectedItem.SurpassCount > 0);
	TB_ItemName->SetText(ItemName);

	if (TB_Select.IsValid())
	{
		if (isConsumable==true)
			TB_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_IngredientReplace")));
		else
			TB_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_IngredientSelect")));
	}

	SetQuantity(false);

	FB2Item tempitem;
	bool tempbool = isConsumable;
	isConsumable = false;
	bool hasthat = false;
	for (int32 i = 0; i < materialTransTable.Num(); i++)
	{
		switch (IngredientSlot_) {
		case EItemForgeSlot::MainMaterialItem: tempitem = FindItemByRefID(materialTransTable[i].mainMaterialItemID); break;
		case EItemForgeSlot::Sub1stMaterialItem: tempitem = FindItemByRefID(materialTransTable[i].material1stItemID); break;
		case EItemForgeSlot::Sub2ndMaterialItem: tempitem = FindItemByRefID(materialTransTable[i].material2ndItemID); break;
		case EItemForgeSlot::Sub3rdMaterialItem: tempitem = FindItemByRefID(materialTransTable[i].material3rdItemID); break;
		case EItemForgeSlot::Sub4thMaterialItem: tempitem = FindItemByRefID(materialTransTable[i].material4thItemID); break;
		}
		if (!tempitem.IsEmpty())
		{
			hasthat = true;
			break;
		}
	}
	if (hasthat == false)
	{
		if (tempbool == true)
		{
			if (BTN_Select.IsValid())
			{
				HB_ButtonBox->SetVisibility(ESlateVisibility::Collapsed);
				BTN_Select->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}


	isConsumable = tempbool;
}

void UB2UIItemForgeIngredientList::SetIconMaterial(UMaterialInterface* IconMaterial)
{
	if (IMG_ItemIcon.IsValid())
	{
		IMG_ItemIcon->SetBrushFromMaterial(IconMaterial);
	}
}

void UB2UIItemForgeIngredientList::HideStarGrade()
{
	if (SB_StarGrade.IsValid())
	{
		SB_StarGrade->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIItemForgeIngredientList::InitStarGrade(int32 StarGrade)
{
	if (StarGradePart.IsValid())
	{
		StarGradePart->UpdateStar(StarGrade, false);
	}
}

void UB2UIItemForgeIngredientList::SetStarGrade(const int32 StarGrade, const bool IsSurpass)
{
	if (StarGradePart.IsValid())
	{
		if (StarGrade == 0)
			StarGradePart->SetVisibility(ESlateVisibility::Collapsed);
		else
		{
			StarGradePart->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			StarGradePart->UpdateStar(StarGrade, IsSurpass);
		}
	}
}

void UB2UIItemForgeIngredientList::SetQuantity(bool isReady)
{
	switch (SelectedItem.ItemClass)
	{
	case EItemClass::EIC_Weapon:
	case EItemClass::EIC_WeaponEssence:
	case EItemClass::EIC_Helmet:
	case EItemClass::EIC_BodyArmor:
	case EItemClass::EIC_Glove:
	case EItemClass::EIC_Shoes:
	case EItemClass::EIC_ProtectionEssence:
	case EItemClass::EIC_Necklace:
	case EItemClass::EIC_Belt:
	case EItemClass::EIC_Ring:
	case EItemClass::EIC_AccessoryEssence:
	case EItemClass::EIC_CostumeWeapon:
	case EItemClass::EIC_CostumeHat:
	case EItemClass::EIC_CostumeArmor:
		if (TB_Quantity.IsValid())
			TB_Quantity->SetText(FText::GetEmpty());
		if (isReady == true)
			if (IMG_SelectedFrame.IsValid())
				IMG_SelectedFrame->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (ItemForgeMainUI)
			ItemForgeMainUI->OnSetMaterialID(IngredientSlot, SelectedItem, isReady, singleRequiredQuantity,true);
		break;

	default:
		if (TB_Quantity.IsValid())
			TB_Quantity->SetText(FText::FromString(FString::FromInt(SelectedItem.ConsumingAmount) + " / " + FString::FromInt(requiredQuantity)));
		if (SelectedItem.ConsumingAmount < requiredQuantity)
		{
			if (TB_Quantity.IsValid())
				TB_Quantity->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
			if (ItemForgeMainUI)
				ItemForgeMainUI->OnSetMaterialID(IngredientSlot, SelectedItem, false, singleRequiredQuantity, false, SelectedItem.ConsumingAmount);
		}
		else
		{
			if (TB_Quantity.IsValid())
				TB_Quantity->SetColorAndOpacity(FLinearColor::White);
			if (ItemForgeMainUI)
				ItemForgeMainUI->OnSetMaterialID(IngredientSlot, SelectedItem, true, singleRequiredQuantity,false, SelectedItem.ConsumingAmount);
		}

		break;
	}
}

FB2Item UB2UIItemForgeIngredientList::FindItemByRefID(int32 RefID)
{
	FB2Item tempItem;
	EPCClass backupClass;

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		backupClass = IntToPCClass(DocBattle->GetCurPCClass());
	}

	LobbyCharObvForcedChangeClass<EPCClass>::GetInstance().Signal(slotClass);

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	CachedInventory = LobbyGM->GetLobbyInventory();

	TArray<FB2Item> allSearchedItem;
	CachedInventory->GetStoredItemList(allSearchedItem, EItemInvenType::EIIVT_Weapon);

	bool foundItem = false;

	//find from weapon
	for (int32 i = 0; i < allSearchedItem.Num(); i++)
	{
		if (allSearchedItem[i].ItemRefID == RefID)
		{
			if (IngredientSlot == EItemForgeSlot::MainMaterialItem)
			{
				if (mainItemLevel <= allSearchedItem[i].Level)
				{
					if (mainItemEnhanceLevel <= allSearchedItem[i].EnhanceLevel)
					{
						tempItem = allSearchedItem[i];
						foundItem = true;
						break;
					}
				}
			}
			else
			{
				tempItem = allSearchedItem[i];
				foundItem = true;
				break;
			}
		}
	}

	//find from armor
	if (foundItem == false)
	{
		allSearchedItem.Empty();
		CachedInventory->GetStoredItemList(allSearchedItem, EItemInvenType::EIIVT_Protection);

		for (int32 i = 0; i < allSearchedItem.Num(); i++)
		{
			if (allSearchedItem[i].ItemRefID == RefID)
			{
				if (IngredientSlot == EItemForgeSlot::MainMaterialItem)
				{
					if (mainItemLevel <= allSearchedItem[i].Level)
					{
						if (mainItemEnhanceLevel <= allSearchedItem[i].EnhanceLevel)
						{
							tempItem = allSearchedItem[i];
							foundItem = true;
							break;
						}
					}
				}
				else
				{
					tempItem = allSearchedItem[i];
					foundItem = true;
					break;
				}
			}
		}
	}

	//find from accessory
	if (foundItem == false)
	{
		allSearchedItem.Empty();
		CachedInventory->GetStoredItemList(allSearchedItem, EItemInvenType::EIIVT_Accessory);
		for (int32 i = 0; i < allSearchedItem.Num(); i++)
		{
			if (allSearchedItem[i].ItemRefID == RefID)
			{
				if (IngredientSlot == EItemForgeSlot::MainMaterialItem)
				{
					if (mainItemLevel <= allSearchedItem[i].Level)
					{
						if (mainItemEnhanceLevel <= allSearchedItem[i].EnhanceLevel)
						{
							tempItem = allSearchedItem[i];
							foundItem = true;
							break;
						}
					}
				}
				else
				{
					tempItem = allSearchedItem[i];
					foundItem = true;
					break;
				}
			}
		}
	}

	//find from consumable
	if (foundItem == false)
	{
		allSearchedItem.Empty();
		CachedInventory->GetStoredItemList(allSearchedItem, EItemInvenType::EIIVT_Consumables);
		for (int32 i = 0; i < allSearchedItem.Num(); i++)
		{
			if (allSearchedItem[i].ItemRefID == RefID)
			{
				tempItem = allSearchedItem[i];
				foundItem = true;
				isConsumable = true;
				break;
			}
		}
	}

	//find from Costume weapon
	if (foundItem == false)
	{
		allSearchedItem.Empty();
		CachedInventory->GetStoredItemList(allSearchedItem, EItemInvenType::EIIVT_CostumeWeapon);
		for (int32 i = 0; i < allSearchedItem.Num(); i++)
		{
			if (allSearchedItem[i].ItemRefID == RefID)
			{
				if (IngredientSlot == EItemForgeSlot::MainMaterialItem)
				{
					if (mainItemLevel <= allSearchedItem[i].Level)
					{
						if (mainItemEnhanceLevel <= allSearchedItem[i].EnhanceLevel)
						{
							tempItem = allSearchedItem[i];
							foundItem = true;
							break;
						}
					}
				}
				else
				{
					tempItem = allSearchedItem[i];
					foundItem = true;
					break;
				}
			}
		}
	}

	//find from Costume armor
	if (foundItem == false)
	{
		allSearchedItem.Empty();
		CachedInventory->GetStoredItemList(allSearchedItem, EItemInvenType::EIIVT_CostumeArmor);
		for (int32 i = 0; i < allSearchedItem.Num(); i++)
		{
			if (allSearchedItem[i].ItemRefID == RefID)
			{
				if (IngredientSlot == EItemForgeSlot::MainMaterialItem)
				{
					if (mainItemLevel <= allSearchedItem[i].Level)
					{
						if (mainItemEnhanceLevel <= allSearchedItem[i].EnhanceLevel)
						{
							tempItem = allSearchedItem[i];
							foundItem = true;
							break;
						}
					}
				}
				else
				{
					tempItem = allSearchedItem[i];
					foundItem = true;
					break;
				}
			}
		}
	}

	//wait, maybe it has been equipped.
	if (foundItem == false)
	{
		allSearchedItem.Empty();
		CachedInventory->GetAllEquippedItem(allSearchedItem, slotClass);
		for (int32 i = 0; i < allSearchedItem.Num(); i++)
		{
			if (allSearchedItem[i].ItemRefID == RefID)
			{
				tempItem = allSearchedItem[i];
				foundItem = true;
				break;
			}
		}
	}

	if (foundItem == false)
	{
		allSearchedItem.Empty();
		CachedInventory->GetAllEquippedCostumeItem(allSearchedItem, slotClass);
		for (int32 i = 0; i < allSearchedItem.Num(); i++)
		{
			if (allSearchedItem[i].ItemRefID == RefID)
			{
				tempItem = allSearchedItem[i];
				foundItem = true;
				break;
			}
		}
	}


	// i don't think player has it.
	LobbyCharObvForcedChangeClass<EPCClass>::GetInstance().Signal(backupClass);


	if (foundItem == false)
	{
		//UE_LOG(LogTemp, Error, TEXT("Couldn't find any item in the inventory!!!"));
		return nullptr;
	}

	return tempItem;
}

void UB2UIItemForgeIngredientList::RecalculateQuantity(int32 thisMuchPlease, bool uiOnly)
{
	if (isConsumable == true)
	{
		requiredQuantity = singleRequiredQuantity;

		mainWantsThisMany = thisMuchPlease;
		requiredQuantity *= mainWantsThisMany;

		if (uiOnly == false)
		{
			SetQuantity(false);
		}
		else
		{
			if(SelectedItem.AllowedPCClass == 4)	//공용함 아이템일 경우에만
				TB_Quantity->SetText(FText::FromString(FString::FromInt(SelectedItem.ConsumingAmount) + " / " + FString::FromInt(requiredQuantity)));
		}
	}
}

int32 UB2UIItemForgeIngredientList::GetRequiredQuantity()
{
	return requiredQuantity;
}
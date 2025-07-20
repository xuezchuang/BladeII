// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynItemIconRow_LobbyInven.h"
//#include "BladeII.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_InventoryMain.h"
#include "B2UIManager_Lobby.h"
#include "B2LobbyGameMode.h"

UB2DynItemIconRow_LobbyInven::UB2DynItemIconRow_LobbyInven(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MainPanelNativeRef = NULL;
	ItemPlacementMargin = 10.0f;

	SetInventorySlotMaxCount(DynItemIconRowMaxCount::DefaultInven);
}

void UB2DynItemIconRow_LobbyInven::UpdateItemIcons(const TArray<FB2Item>& AllItemsArray, int32 InStartIndex, UB2LobbyUI_InventoryMain* ContainingInvenPage)
{
	DestroyItemIcons(); // 扁粮俊 父电 芭 力芭.

	//AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	//UB2UIManager_Lobby* DJLegacyUIMgr = LobbyGM ? LobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;
	//// 咯扁辑狼 酒捞袍 酒捞能 widget class 绰 谍单辑档 曼炼且 老捞 乐绢辑 漂喊洒 B2UIManager_Lobby 芭媚辑 啊廉柯促
	//UClass* ItemIconWidgetClass = DJLegacyUIMgr ? DJLegacyUIMgr->GetInventoryItemIconClass(false) : nullptr;

	//if (ItemIconWidgetClass == NULL || MainPanelNativeRef == NULL)
	//{
	//	return;
	//}

	//const int32 StartIndex = FMath::Clamp(InStartIndex, 0, AllItemsArray.Num() - 1);
	//const int32 EndIndex = FMath::Clamp(StartIndex + InventoryRowSlotCount - 1, 0, AllItemsArray.Num() - 1);
	//
	//const FVector2D AllowedSingleSlotSize = GetAllowedSingleItemIconSize(); // Main panel 备己俊 蝶弗 倾侩 荤捞令
	//APlayerController* OwningPC = GetOwningPlayer();

	//for (int32 II = StartIndex; II <= EndIndex; ++II)
	//{
	//	// 积己且 酒捞能 widget 狼 困摹 魂沥. 啊肺 margin 捞 酒贰贸烦 馆康登绊 技肺 margin 篮 咯扁 main panel 磊眉 备己俊 狼秦..
	//	const int32 PlacementIndex = II % InventoryRowSlotCount;
	//	const float PosX = (ItemPlacementMargin * 0.5f) + (float)(PlacementIndex * (AllowedSingleSlotSize.X + ItemPlacementMargin));
	//	
	//	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	//	// MainPanelNativeRef 困俊 弥辆利牢 酒捞袍 酒捞能 widget 积己窍绰单.. WidgetPool 俊辑 烙措瞒 拌距
	//	if (DJLegacyUIMgr)
	//	{
	//		UB2DynItemIcon_LobbyInven* NewItemIcon =
	//			Cast<UB2DynItemIcon_LobbyInven>(DJLegacyUIMgr->RentInvenDynItemIconWidget(false, OwningPC, this, nullptr)); // ParentPanel 阑 咯扁辑 逞败林瘤 富绊.. 老窜 烙措瞒 拌距 饶俊

	//		if(NewItemIcon)
	//		{
	//			// 利例茄 困摹俊 冠酒持扁
	//			PutWidgetInCanvasPanel(this, NewItemIcon, MainPanelNativeRef, DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedSingleSlotSize,
	//				FVector2D(PosX, 0.0f), 0, false);
	//			NewItemIcon->SetVisibility(ESlateVisibility::Visible);
	//			NewItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_LobbyInven); // 扁夯蔼捞变 窍瘤父 犬角洒 窍扁 困秦.. UpdateItemData 捞傈俊 技泼秦具 窃.
	//			NewItemIcon->UpdateItemData(AllItemsArray[II]); // 俺喊 酒捞袍 沥焊甫 逞败霖促.

	//			CreatedItemIcons.Add(NewItemIcon);
	//			//UB2LobbyInventory::SetChangeNewItemTofalse(AllItemsArray[II].InstanceUID);//惶酒霖酒捞袍篮 false肺 官层林磊
	//		}
	//	}
	//}
}

void UB2DynItemIconRow_LobbyInven::OverrideItemIconUsage(ELobbyInvenItemIconUsage InNewUsage)
{
	for (UB2DynItemIcon_LobbyInven* ThisIcon : CreatedItemIcons)
	{
		if (ThisIcon)
		{
			ThisIcon->SetIconUsageType(InNewUsage);
		}
	}
}

FVector2D UB2DynItemIconRow_LobbyInven::GetAllowedSingleItemIconSize()
{
	FVector2D ReturnSize(0.0f, 0.0f);

	// Anchor 汲沥俊 蝶扼 GetSize 绰 盔窍绰 蔼捞 救 唱棵 巴捞骨肺 林狼.
	UCanvasPanelSlot* MainPanelSlot = MainPanelNativeRef ? Cast<UCanvasPanelSlot>(MainPanelNativeRef->Slot) : NULL;;
	if (MainPanelSlot)
	{
		FVector2D MainPanelSize = MainPanelSlot->GetSize();

		// MainPanelSize 客 Margin 俊 蝶扼..
		ReturnSize.X = ((MainPanelSize.X / (float)InventoryRowSlotCount) - ItemPlacementMargin);
		ReturnSize.Y = MainPanelSize.Y;
	}

	return ReturnSize;
}

UB2DynItemIcon_LobbyInven* UB2DynItemIconRow_LobbyInven::FindSingleItemIcon(int64 ItemInstanceUID)
{
	for (UB2DynItemIcon_LobbyInven* CurrIcon : CreatedItemIcons)
	{
		if (CurrIcon && CurrIcon->GetItemInstanceUID() == ItemInstanceUID)
		{
			return CurrIcon;
		}
	}
	return NULL;
}

UB2DynItemIcon_LobbyInven* UB2DynItemIconRow_LobbyInven::FindSingleItemIconFromIndex(int32 iIndex)
{
	return CreatedItemIcons.IsValidIndex(iIndex) ? CreatedItemIcons[iIndex] : nullptr;
}

void UB2DynItemIconRow_LobbyInven::GetAllCreatedIcons(TArray<class UB2DynItemIcon_LobbyInven*>& OutIcons)
{
	OutIcons.Append(CreatedItemIcons);
}

void UB2DynItemIconRow_LobbyInven::DestroyItemIcons()
{
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UB2UIManager_Lobby* DJLegacyUIMgr = LobbyGM ? LobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;
	
	for (int32 II = 0; II < CreatedItemIcons.Num(); ++II)
	{
		UB2DynItemIcon_LobbyInven* ThisOne = CreatedItemIcons[II];
		if (ThisOne)
		{
			if (DJLegacyUIMgr)
			{ // 烙措拌距 辆丰.
				DJLegacyUIMgr->TurnInDynItemIconWidget(false, ThisOne);
			}
			else
			{
				ThisOne->DestroySelf();
			}
		}
	}
	CreatedItemIcons.Empty();
}

void UB2DynItemIconRow_LobbyInven::DestroySelf()
{
	Super::DestroySelf();

	DestroyItemIcons();
}

void UB2DynItemIconRow_LobbyInven::PlayEnhanceResultAnims()
{
	for (int32 II = 0; II < CreatedItemIcons.Num(); ++II)
	{
		if (CreatedItemIcons[II])
		{
			CreatedItemIcons[II]->PlayEnhanceResultAnim();
		}
	}
}

void UB2DynItemIconRow_LobbyInven::SetInventorySlotMaxCount(const int32 MaxCount)
{
	InventoryRowSlotCount = FMath::Clamp(MaxCount, DynItemIconRowMaxCount::DefaultInven, DynItemIconRowMaxCount::AutoLevelupResult);
}
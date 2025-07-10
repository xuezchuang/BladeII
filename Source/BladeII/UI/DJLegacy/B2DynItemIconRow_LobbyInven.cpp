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
	DestroyItemIcons(); // 기존에 만든 거 제거.

	//AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	//UB2UIManager_Lobby* DJLegacyUIMgr = LobbyGM ? LobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;
	//// 여기서의 아이템 아이콘 widget class 는 딴데서도 참조할 일이 있어서 특별히 B2UIManager_Lobby 거쳐서 가져온다
	//UClass* ItemIconWidgetClass = DJLegacyUIMgr ? DJLegacyUIMgr->GetInventoryItemIconClass(false) : nullptr;

	//if (ItemIconWidgetClass == NULL || MainPanelNativeRef == NULL)
	//{
	//	return;
	//}

	//const int32 StartIndex = FMath::Clamp(InStartIndex, 0, AllItemsArray.Num() - 1);
	//const int32 EndIndex = FMath::Clamp(StartIndex + InventoryRowSlotCount - 1, 0, AllItemsArray.Num() - 1);
	//
	//const FVector2D AllowedSingleSlotSize = GetAllowedSingleItemIconSize(); // Main panel 구성에 따른 허용 사이즈
	//APlayerController* OwningPC = GetOwningPlayer();

	//for (int32 II = StartIndex; II <= EndIndex; ++II)
	//{
	//	// 생성할 아이콘 widget 의 위치 산정. 가로 margin 이 아래처럼 반영되고 세로 margin 은 여기 main panel 자체 구성에 의해..
	//	const int32 PlacementIndex = II % InventoryRowSlotCount;
	//	const float PosX = (ItemPlacementMargin * 0.5f) + (float)(PlacementIndex * (AllowedSingleSlotSize.X + ItemPlacementMargin));
	//	
	//	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	//	// MainPanelNativeRef 위에 최종적인 아이템 아이콘 widget 생성하는데.. WidgetPool 에서 임대차 계약
	//	if (DJLegacyUIMgr)
	//	{
	//		UB2DynItemIcon_LobbyInven* NewItemIcon =
	//			Cast<UB2DynItemIcon_LobbyInven>(DJLegacyUIMgr->RentInvenDynItemIconWidget(false, OwningPC, this, nullptr)); // ParentPanel 을 여기서 넘겨주지 말고.. 일단 임대차 계약 후에

	//		if(NewItemIcon)
	//		{
	//			// 적절한 위치에 박아넣기
	//			PutWidgetInCanvasPanel(this, NewItemIcon, MainPanelNativeRef, DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedSingleSlotSize,
	//				FVector2D(PosX, 0.0f), 0, false);
	//			NewItemIcon->SetVisibility(ESlateVisibility::Visible);
	//			NewItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_LobbyInven); // 기본값이긴 하지만 확실히 하기 위해.. UpdateItemData 이전에 세팅해야 함.
	//			NewItemIcon->UpdateItemData(AllItemsArray[II]); // 개별 아이템 정보를 넘겨준다.

	//			CreatedItemIcons.Add(NewItemIcon);
	//			//UB2LobbyInventory::SetChangeNewItemTofalse(AllItemsArray[II].InstanceUID);//뽑아준아이템은 false로 바꿔주자
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

	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
	UCanvasPanelSlot* MainPanelSlot = MainPanelNativeRef ? Cast<UCanvasPanelSlot>(MainPanelNativeRef->Slot) : NULL;;
	if (MainPanelSlot)
	{
		FVector2D MainPanelSize = MainPanelSlot->GetSize();

		// MainPanelSize 와 Margin 에 따라..
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
			{ // 임대계약 종료.
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
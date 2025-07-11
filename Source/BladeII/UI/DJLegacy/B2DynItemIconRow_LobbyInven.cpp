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
	DestroyItemIcons(); // ������ ���� �� ����.

	//AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	//UB2UIManager_Lobby* DJLegacyUIMgr = LobbyGM ? LobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;
	//// ���⼭�� ������ ������ widget class �� �������� ������ ���� �־ Ư���� B2UIManager_Lobby ���ļ� �����´�
	//UClass* ItemIconWidgetClass = DJLegacyUIMgr ? DJLegacyUIMgr->GetInventoryItemIconClass(false) : nullptr;

	//if (ItemIconWidgetClass == NULL || MainPanelNativeRef == NULL)
	//{
	//	return;
	//}

	//const int32 StartIndex = FMath::Clamp(InStartIndex, 0, AllItemsArray.Num() - 1);
	//const int32 EndIndex = FMath::Clamp(StartIndex + InventoryRowSlotCount - 1, 0, AllItemsArray.Num() - 1);
	//
	//const FVector2D AllowedSingleSlotSize = GetAllowedSingleItemIconSize(); // Main panel ������ ���� ��� ������
	//APlayerController* OwningPC = GetOwningPlayer();

	//for (int32 II = StartIndex; II <= EndIndex; ++II)
	//{
	//	// ������ ������ widget �� ��ġ ����. ���� margin �� �Ʒ�ó�� �ݿ��ǰ� ���� margin �� ���� main panel ��ü ������ ����..
	//	const int32 PlacementIndex = II % InventoryRowSlotCount;
	//	const float PosX = (ItemPlacementMargin * 0.5f) + (float)(PlacementIndex * (AllowedSingleSlotSize.X + ItemPlacementMargin));
	//	
	//	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	//	// MainPanelNativeRef ���� �������� ������ ������ widget �����ϴµ�.. WidgetPool ���� �Ӵ��� ���
	//	if (DJLegacyUIMgr)
	//	{
	//		UB2DynItemIcon_LobbyInven* NewItemIcon =
	//			Cast<UB2DynItemIcon_LobbyInven>(DJLegacyUIMgr->RentInvenDynItemIconWidget(false, OwningPC, this, nullptr)); // ParentPanel �� ���⼭ �Ѱ����� ����.. �ϴ� �Ӵ��� ��� �Ŀ�

	//		if(NewItemIcon)
	//		{
	//			// ������ ��ġ�� �ھƳֱ�
	//			PutWidgetInCanvasPanel(this, NewItemIcon, MainPanelNativeRef, DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedSingleSlotSize,
	//				FVector2D(PosX, 0.0f), 0, false);
	//			NewItemIcon->SetVisibility(ESlateVisibility::Visible);
	//			NewItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_LobbyInven); // �⺻���̱� ������ Ȯ���� �ϱ� ����.. UpdateItemData ������ �����ؾ� ��.
	//			NewItemIcon->UpdateItemData(AllItemsArray[II]); // ���� ������ ������ �Ѱ��ش�.

	//			CreatedItemIcons.Add(NewItemIcon);
	//			//UB2LobbyInventory::SetChangeNewItemTofalse(AllItemsArray[II].InstanceUID);//�̾��ؾ������� false�� �ٲ�����
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

	// Anchor ������ ���� GetSize �� ���ϴ� ���� �� ���� ���̹Ƿ� ����.
	UCanvasPanelSlot* MainPanelSlot = MainPanelNativeRef ? Cast<UCanvasPanelSlot>(MainPanelNativeRef->Slot) : NULL;;
	if (MainPanelSlot)
	{
		FVector2D MainPanelSize = MainPanelSlot->GetSize();

		// MainPanelSize �� Margin �� ����..
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
			{ // �Ӵ��� ����.
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
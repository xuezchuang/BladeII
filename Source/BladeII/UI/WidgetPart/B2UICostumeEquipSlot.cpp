// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UICostumeEquipSlot.h"

#include "BladeIIGameImpl.h"
#include "B2DynItemIcon_LobbyEquip.h"
#include "B2LobbyInventory.h"
#include "B2UIManager.h"
#include "B2UICostumeItemInfoPopup.h"
#include "B2ItemInfo.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Slate/SceneViewport.h"
#include "BladeIIUtil.h"
#include "B2LobbyGameMode.h"
#include "B2UIOtherUserInfo.h"

void UB2UICostumeEquipSlotList::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UICostumeEquipSlotList::InitCostumeEquipSlotList_BP()
{
	this->Init();

	SubscribeEvents();

	this->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UICostumeEquipSlotList::SubscribeEvents()
{
	UnsubscribeEvents();

	CAPTURE_UOBJECT(UB2UICostumeEquipSlotList);

	Issues.Add(CostumeEquipSlotVisibleClass<bool>::GetInstance().Subscribe2(
		[Capture](bool IsVisible)
	{
		if (Capture.IsValid())
		{
			Capture->UpdateVisibility(IsVisible);
		}
	}
	));
}

void UB2UICostumeEquipSlotList::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UICostumeEquipSlotList::UpdateVisibility(const bool& bIsVisible)
{
	if(bIsVisible)
	{
		this->SetVisibility(ESlateVisibility::Visible);
		OnOpen();
	}
	else
	{
		OnClose();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void UB2UICostumeEquipSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_EquipSlot);

	GET_SLOT(UWidgetSwitcher, WS_EquipSlotIMG);

	GET_SLOT(UB2Image, IMG_ItemIcon);
	GET_SLOT(UB2Image, IMG_ItemGrade);

	GET_SLOT(UTextBlock, TB_EnhanceLevel);
}

void UB2UICostumeEquipSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_EquipSlot, &UB2UICostumeEquipSlot::OnClickBTN_EquipSlot);
}

void UB2UICostumeEquipSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UICostumeEquipSlot::InitCostumeEquipSlot_BP()
{
	this->Init();

	InitEquipSlot();
	SubscribeEvents();
}

void UB2UICostumeEquipSlot::InitEquipSlot()
{
	if (WS_EquipSlotIMG.IsValid())
	{
		int32 iActiveWidgetIndex = static_cast<int32>(eCostumeEquipPlace);

		iActiveWidgetIndex = ExtractNthDigitR<1>(iActiveWidgetIndex);

		WS_EquipSlotIMG->SetActiveWidgetIndex(iActiveWidgetIndex);
	}

	if (IMG_ItemIcon.IsValid())
	{
		IMG_ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	if (IMG_ItemGrade.IsValid())
	{
		IMG_ItemGrade->SetVisibility(ESlateVisibility::Hidden);
	}
	if (TB_EnhanceLevel.IsValid())
	{
		TB_EnhanceLevel->SetText(FText::GetEmpty());
	}


	UpdateBTNEquipSlot(false);
}

void UB2UICostumeEquipSlot::SubscribeEvents()
{
	UnsubscribeEvents();

	CAPTURE_UOBJECT(UB2UICostumeEquipSlot);

	Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
	[Capture](EPCClass SelectionPCClass)
	{
		if (Capture.IsValid())
		{
			Capture->UpdateFromSelectionPCClass(SelectionPCClass);
		}
	}
	));

	Issues.Add(UpdateLobbyInventoryControlClass<>::GetInstance().Subscribe2(
	[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->UpdateFromCurrentPCClass();
		}
	}
	));

	Issues.Add(LobbyHeroFindAccountClass<EPCClass>::GetInstance().Subscribe2(
	[Capture](EPCClass InPCClass)
	{
		if (Capture.IsValid())
		{
			Capture->UpdateFromFindAccountClass(InPCClass);
		}
	}
	));
}

void UB2UICostumeEquipSlot::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UICostumeEquipSlot::UpdateFromSelectionPCClass(EPCClass SelectionPCClass)
{
	TArray<FB2Item> ArrayLocalPlayerEquippedCostumeItem;
	UB2LobbyInventory::GetAllEquippedCostumeItem(ArrayLocalPlayerEquippedCostumeItem, SelectionPCClass);

	UpdateEquipSlotFromArrayFB2Item(ArrayLocalPlayerEquippedCostumeItem);
}

void UB2UICostumeEquipSlot::UpdateFromCurrentPCClass()
{
	if (AB2LobbyGameMode* pLobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer())))
	{
		const EPCClass CurrentPCClass = pLobbyGameMode->GetHeroMgmtModePCClass();
		UpdateFromSelectionPCClass(CurrentPCClass);
	}
}

void UB2UICostumeEquipSlot::UpdateFromFindAccountClass(EPCClass InPCClass)
{
	FB2ModPlayerInfo& rOtherPlayerInfo = BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo();

	TArray<FB2Item> ArrayOtherPlayerEquippedCostumeItem;
	rOtherPlayerInfo.GetEquippedCostumeItems(InPCClass, ArrayOtherPlayerEquippedCostumeItem);

	UpdateEquipSlotFromArrayFB2Item(ArrayOtherPlayerEquippedCostumeItem);
}

void UB2UICostumeEquipSlot::UpdateEquipSlotFromArrayFB2Item(const TArray<FB2Item>& rArrayItem)
{
	InitEquipSlot();

	const int32 iArrayMax = rArrayItem.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (rArrayItem.IsValidIndex(i) == false)
		{
			continue;
		}

		if (eCostumeEquipPlace == rArrayItem[i].CostumeEquipPlace)
		{
			CachedEquipB2ItemInfo = rArrayItem[i];
			UpdateIMGItemIcon(CachedEquipB2ItemInfo);
			UpdateIMGItemGrade(CachedEquipB2ItemInfo.StarGrade);
			UpdateEnhanceLevelText(CachedEquipB2ItemInfo.EnhanceLevel);
			UpdateBTNEquipSlot(true);
			break;
		}
	}
}

void UB2UICostumeEquipSlot::UpdateIMGItemIcon(const FB2Item& InItem)
{
	if (UB2ItemInfo* pAllItemInfo = StaticFindItemInfo())
	{
		if (FSingleItemInfoData* pItemInfo = pAllItemInfo->GetInfoData(InItem.ItemRefID))
		{
			if (IMG_ItemIcon.IsValid())
			{
				IMG_ItemIcon->SetBrushFromMaterial(pItemInfo->GetIconMaterial(pAllItemInfo));
				IMG_ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
}

void UB2UICostumeEquipSlot::UpdateIMGItemGrade(const uint8& StarGrade)
{
	if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
	{
		UMaterialInterface* pItemIconBGMtrl = pAllItemMiscInfo->GetEquipIconBGMtrl(StarGrade);
		if (pItemIconBGMtrl && IMG_ItemGrade.IsValid())
		{
			IMG_ItemGrade->SetBrushFromMaterial(pItemIconBGMtrl);
			IMG_ItemGrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UB2UICostumeEquipSlot::UpdateEnhanceLevelText(const int32& nEnhanceLevel)
{
	if (TB_EnhanceLevel.IsValid())
	{
		if (nEnhanceLevel < 1)
			TB_EnhanceLevel->SetText(FText::GetEmpty());
		else
			TB_EnhanceLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::FromString(FString::FromInt(nEnhanceLevel))));
	}
}

void UB2UICostumeEquipSlot::UpdateBTNEquipSlot(const bool& IsEnable)
{
	if (BTN_EquipSlot.IsValid())
	{
		BTN_EquipSlot->SetIsEnabled(IsEnable);
	}
}

void UB2UICostumeEquipSlot::OnClickBTN_EquipSlot()
{
	float fPoupSizeX = 0.f;

	const float fConvertScale = 1.f / UWidgetLayoutLibrary::GetViewportScale(this);

	FDisplayMetrics Metrics;
	FSlateApplication::Get().GetDisplayMetrics(Metrics);
	FVector2D vSafePaddingSize(Metrics.TitleSafePaddingSize);

	vSafePaddingSize.X *= fConvertScale;

	FVector2D vPopupPostion(vSafePaddingSize.X, 0.0f);

	if (UGameViewportClient* ViewportClient = this->GetWorld()->GetGameViewport())
	{
		if (FSceneViewport* SceneViewport = ViewportClient->GetGameViewport())
		{
			fPoupSizeX = SceneViewport->GetCachedGeometry().GetLocalSize().X * fConvertScale * 0.5;
			fPoupSizeX -= vSafePaddingSize.X;
		}
	}

	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		auto* pCostumeItemInfoPopup = pUIManager->OpenUI<UB2UICostumeItemInfoPopup>(UIFName::CostumeItemInfoPopupLeft);
		pCostumeItemInfoPopup->InitCostumeItemInfoPopup(vPopupPostion, fPoupSizeX, CachedEquipB2ItemInfo);
	}
}
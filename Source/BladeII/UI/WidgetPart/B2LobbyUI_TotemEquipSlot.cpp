// Fill out your copyright notice in the Description page of Project Settings.


#include "B2LobbyUI_TotemEquipSlot.h"

#include "B2UI_TotemInfoPopup.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Slate/SceneViewport.h"

///////////////////////////////////////////////////////////////////////////////////////
//
// UB2LobbyUI_TotemEquipSlotList
//
///////////////////////////////////////////////////////////////////////////////////////

void UB2LobbyUI_TotemEquipSlotList::Init()
{
	Super::Init();
}

void UB2LobbyUI_TotemEquipSlotList::DestroySelf(class UB2UIManager* InUIManager)
{
	for (int i = 0; i < MaxEquipSlot; ++i)
	{
		if (UIP_TotemEquipSlots.IsValidIndex(i) == true)
		{
			UIP_TotemEquipSlots[i]->DestroySelf(InUIManager);
		}
	}

	UIP_TotemEquipSlots.Empty();
	Super::DestroySelf(InUIManager);
}

void UB2LobbyUI_TotemEquipSlotList::CacheAssets()
{
	Super::CacheAssets();

	UIP_TotemEquipSlots.Empty();
	UIP_TotemEquipSlots.SetNum(MaxEquipSlot);
	GET_SLOT_BYNAME(UB2LobbyUI_TotemEquipSlot, UIP_TotemEquipSlots[0], UIP_TotemEquipSlot_1);
	GET_SLOT_BYNAME(UB2LobbyUI_TotemEquipSlot, UIP_TotemEquipSlots[1], UIP_TotemEquipSlot_2);
	GET_SLOT_BYNAME(UB2LobbyUI_TotemEquipSlot, UIP_TotemEquipSlots[2], UIP_TotemEquipSlot_3);
	for (int i = 0; i < MaxEquipSlot; ++i)
	{
		if (UIP_TotemEquipSlots[i].IsValid() == true)
		{
			UIP_TotemEquipSlots[i]->Init();
		}
	}
}

void UB2LobbyUI_TotemEquipSlotList::BindDelegates()
{
	Super::BindDelegates();
}

///////////////////////////////////////////////////////////////////////////////////////
//
// UB2LobbyUI_TotemEquipSlot
//
///////////////////////////////////////////////////////////////////////////////////////

void UB2LobbyUI_TotemEquipSlot::Init()
{
	Super::Init();

	InitTotemEquipSlot();
	SubscribeEvents();
}

void UB2LobbyUI_TotemEquipSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button,			BTN_EquipSlot);

	GET_SLOT(UWidgetSwitcher,	WS_EquipSlotIMG);

	GET_SLOT(UB2Image,			IMG_TotemIcon);
	GET_SLOT(UB2Image,			IMG_TotemGrade);

	GET_SLOT(UTextBlock,		TB_SmeltingLevel);
}

void UB2LobbyUI_TotemEquipSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_EquipSlot, &UB2LobbyUI_TotemEquipSlot::OnClickBTN_EquipSlot);
}

void UB2LobbyUI_TotemEquipSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2LobbyUI_TotemEquipSlot::InitTotemEquipSlot()
{
	if (IMG_TotemIcon.IsValid())
	{
		IMG_TotemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	if (IMG_TotemGrade.IsValid())
	{
		IMG_TotemGrade->SetVisibility(ESlateVisibility::Hidden);
	}
	if (TB_SmeltingLevel.IsValid())
	{
		TB_SmeltingLevel->SetVisibility(ESlateVisibility::Hidden);
	}

	UpdateSlotState(EquipSlotState);
	UpdateBTNEquipSlot(false);
}

void UB2LobbyUI_TotemEquipSlot::UpdateSlotState(ETotemEquipSlotState InEquipSlotState)
{
	if (WS_EquipSlotIMG.IsValid())
	{
		int32 iActiveWidgetIndex = static_cast<int32>(EquipSlotState);
		WS_EquipSlotIMG->SetActiveWidgetIndex(iActiveWidgetIndex);
	}
}

void UB2LobbyUI_TotemEquipSlot::SubscribeEvents()
{
	UnsubscribeEvents();

	//Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
	//	[this](EPCClass SelectionPCClass)
	//{
	//	this->UpdateFromSelectionPCClass(SelectionPCClass);
	//}
	//));

	//Issues.Add(UpdateLobbyInventoryControlClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->UpdateFromCurrentPCClass();
	//}
	//));
}

void UB2LobbyUI_TotemEquipSlot::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2LobbyUI_TotemEquipSlot::UpdateFromSelectionPCClass(EPCClass SelectionPCClass)
{
	TArray<FB2Totem> EquippedTotems;
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInven)
	{
		LobbyInven->GetEquippedTotemList(EquippedTotems, SelectionPCClass);
	}

	UpdateEquipSlotFromArrayFB2Totem(EquippedTotems);
}

void UB2LobbyUI_TotemEquipSlot::UpdateFromCurrentPCClass()
{
	if (AB2LobbyGameMode* pLobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer())))
	{
		const EPCClass CurrentPCClass = pLobbyGameMode->GetHeroMgmtModePCClass();
		UpdateFromSelectionPCClass(CurrentPCClass);
	}
}

void UB2LobbyUI_TotemEquipSlot::UpdateIMGTotemIcon(const FB2Totem& InTotem)
{
	if (UB2ItemInfo* pAllItemInfo = StaticFindItemInfo())
	{
		if (FSingleItemInfoData* pItemInfo = pAllItemInfo->GetInfoData(InTotem.RefID))
		{
			if (IMG_TotemIcon.IsValid())
			{
				IMG_TotemIcon->SetBrushFromMaterial(pItemInfo->GetIconMaterial(pAllItemInfo));
				IMG_TotemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
}

void UB2LobbyUI_TotemEquipSlot::UpdateIMGTotemGrade(const ETotemGrade& TotemGrade)
{
	if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
	{
		UMaterialInterface* pItemIconBGMtrl = pAllItemMiscInfo->GetEquipIconBGMtrl(TotemGradeToItemStarGrade(TotemGrade));
		if (pItemIconBGMtrl && IMG_TotemGrade.IsValid())
		{
			IMG_TotemGrade->SetBrushFromMaterial(pItemIconBGMtrl);
			IMG_TotemGrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UB2LobbyUI_TotemEquipSlot::UpdateIMGTotemSmeltingLevel(const uint8& SmeltingLevel)
{
	bool ShowSmeltingLevel = (SmeltingLevel > 0);

	if (TB_SmeltingLevel.IsValid())
	{
		TB_SmeltingLevel->SetVisibility(ShowSmeltingLevel ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		if (ShowSmeltingLevel == false)
		{
			return;
		}

		FText LevelFormat = FText::FromString(TEXT("+{0}"));
		TB_SmeltingLevel->SetText(FText::Format(LevelFormat, FText::AsNumber(SmeltingLevel)));
	}
}

void UB2LobbyUI_TotemEquipSlot::UpdateBTNEquipSlot(const bool& IsEnable)
{
	if (BTN_EquipSlot.IsValid())
	{
		BTN_EquipSlot->SetIsEnabled(IsEnable);
	}
}

void UB2LobbyUI_TotemEquipSlot::UpdateEquipSlotFromArrayFB2Totem(const TArray<FB2Totem>& rArrayTotem)
{
	InitTotemEquipSlot();

	const int32 iArrayMax = rArrayTotem.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (rArrayTotem.IsValidIndex(i) == false)
		{
			continue;
		}

		if (TotemEquipPlace == rArrayTotem[i].EquipSlot)
		{
			CachedEquipB2TotemInfo = rArrayTotem[i];
			UpdateIMGTotemIcon(CachedEquipB2TotemInfo);
			UpdateIMGTotemGrade(CachedEquipB2TotemInfo.Grade);
			UpdateIMGTotemSmeltingLevel(CachedEquipB2TotemInfo.RefineLevel);
			UpdateBTNEquipSlot(true);
			break;
		}
	}
}

void UB2LobbyUI_TotemEquipSlot::OnClickBTN_EquipSlot()
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
		auto* pTotemInfoPopup = pUIManager->OpenUI<UB2UI_TotemInfoPopup>(UIFName::TotemInfoPopupLeft);
		pTotemInfoPopup->InitTotemInfoPopup(vPopupPostion, fPoupSizeX, CachedEquipB2TotemInfo);
	}
}
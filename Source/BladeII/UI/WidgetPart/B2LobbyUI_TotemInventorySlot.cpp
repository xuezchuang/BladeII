// Fill out your copyright notice in the Description page of Project Settings.


#include "B2LobbyUI_TotemInventorySlot.h"

#include "B2UIManager.h"
#include "B2TotemData.h"
#include "BladeIIUtil.h"
#include "B2ItemInfo.h"
#include "B2UI_TotemInfoPopup.h"
#include "B2LobbyUI_TotemInventoryMain.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Slate/SceneViewport.h"
#include "B2LobbyGameMode.h"
#include "BladeIIGameImpl.h"

#if !UE_BUILD_SHIPPING
extern bool gIsDevUI;
#endif //UE_BUILD_SHIPPING

const int32 iTOTEM_SMELING_STATE_COMPLETE = 0;
const int32 iTOTEM_SMELING_STATE_NOT_COMPLETE = 1;

UB2LobbyUI_TotemInventorySlot::UB2LobbyUI_TotemInventorySlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsOpenTotemInfoPopup = false;
}

void UB2LobbyUI_TotemInventorySlot::Init()
{
	Super::Init();

	SubscribeEvents();
}

void UB2LobbyUI_TotemInventorySlot::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2LobbyUI_TotemInventorySlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage,			IMG_TotemIcon);
	GET_SLOT(UImage,			IMG_TotemGrade);
	GET_SLOT(UTextBlock,		TB_TotemName);
	GET_SLOT(UTextBlock,		TB_SmeltingLevel);

	GET_SLOT(UB2Button,			BTN_TotemInventorySlot);

	GET_SLOT(UImage,			IMG_LockState);
	GET_SLOT(UOverlay,			O_EquipMark);
	GET_SLOT(UTextBlock,		TB_EquipMark);

	GET_SLOT(UWidgetSwitcher,	WS_SmeltingInfo);

	GET_SLOT(UOverlay,			O_IsSmelting);
	GET_SLOT(UTextBlock,		TB_IsSmelting);

	GET_SLOT(UTextBlock,		TB_Option0);
	GET_SLOT(UTextBlock,		TB_Option1);
	GET_SLOT(UTextBlock,		TB_Option2);

	GET_SLOT(UB2UIWidgetBase,	UIP_RedDot);

	// 치트키용 ID 확인 ////////////////////////////
	GET_SLOT(UPanelWidget, P_DevUI);
#if !UE_BUILD_SHIPPING
	GET_SLOT(UTextBlock, TB_ItemRefId_Dev);
	GET_SLOT(UTextBlock, TB_ItemUId_Dev);
	GET_SLOT(UB2Button, BTN_ItemRefId_Dev);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemRefId_Dev, &UB2LobbyUI_TotemInventorySlot::OnPressedBTNItemRefid);
	GET_SLOT(UB2Button, BTN_ItemUId_Dev);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemUId_Dev, &UB2LobbyUI_TotemInventorySlot::OnPressedBTNItemUid);
#endif
	////////////////////////////////////////////////
}

void UB2LobbyUI_TotemInventorySlot::UpdateStaticText()
{
	if (TB_EquipMark.IsValid())
	{
		TB_EquipMark->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Equiped_Preset")));
	}

	if (TB_IsSmelting.IsValid())
	{
		TB_IsSmelting->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_smelting")));
	}
}

void UB2LobbyUI_TotemInventorySlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_TotemInventorySlot, &UB2LobbyUI_TotemInventorySlot::OnClickBTN_TotemInventorySlot);
}

void UB2LobbyUI_TotemInventorySlot::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(ReturnTotemInventoryClass<int64>::GetInstance().Subscribe2(
		[this](int64 InstanceID)
	{
		if (this->CachedB2TotemInfo.ID == InstanceID)
		{
			this->OpenTotemInfoPopup(this->CachedB2TotemInfo);
		}
	}
	));
}

void UB2LobbyUI_TotemInventorySlot::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2LobbyUI_TotemInventorySlot::OnSetupManualScrollBoxSender(UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox)
{
	if (BTN_TotemInventorySlot.IsValid())
	{
		BTN_TotemInventorySlot->SetIsEnabled(false);
	}
}

void UB2LobbyUI_TotemInventorySlot::OnClickedSenderBTNArea()
{
	Super::OnClickedSenderBTNArea();

	PlayClickSound();

	OpenTotemInfoPopup(CachedB2TotemInfo);
}

void UB2LobbyUI_TotemInventorySlot::OnClickBTN_TotemInventorySlot()
{
	if (IsOpenTotemInfoPopup)
		OpenTotemInfoPopup(CachedB2TotemInfo, true);
}

void UB2LobbyUI_TotemInventorySlot::UpdateTotemSlot_FromFB2Totem(const FB2Totem& InTotem)
{
	CachedB2TotemInfo = InTotem;

	// 제련 완료 유무로 갈라서 아이콘 업데이트 분기를 가져야한다.
	if (UB2ItemInfo* pAllItemInfo = StaticFindItemInfo())
	{
		if (FSingleItemInfoData* pItemInfo = pAllItemInfo->GetInfoData(InTotem.RefID))
		{
			UpdateIMGTotemIcon(pAllItemInfo, pItemInfo);
			UpdateTBTotemName(pItemInfo, CliToSvrTotemGrade(InTotem.Grade));
		}
	}

	UpdateLockState(InTotem.IsLock);
	UpdateIsSmelting(InTotem);
	UpdateSmeltingLevel(InTotem.RefineLevel);
	UpdateIMGTotemGrade(CliToSvrTotemGrade(InTotem.Grade));
	UpdateEquipMark(InTotem);
	UpdateRedDot(InTotem);

#if !UE_BUILD_SHIPPING
	if (P_DevUI.IsValid())
		P_DevUI->SetVisibility(ShouldShowDevUI() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (ShouldShowDevUI())
	{
		if (TB_ItemRefId_Dev.IsValid())
			TB_ItemRefId_Dev->SetText(GetItemRefId());

		if (TB_ItemUId_Dev.IsValid())
			TB_ItemUId_Dev->SetText(GetItemUId());
	}
#else
	if (P_DevUI.IsValid()) {
		P_DevUI->SetVisibility(ESlateVisibility::Collapsed);
	}
#endif
}

void UB2LobbyUI_TotemInventorySlot::SetIsOpenItemInfoPopup(bool OpenInfoPopup)
{
	IsOpenTotemInfoPopup = OpenInfoPopup;
}

UMaterialInstanceConstant* UB2LobbyUI_TotemInventorySlot::GetBGPanelMIC()
{
	return IMG_TotemGrade.IsValid() ? Cast<UMaterialInstanceConstant>(IMG_TotemGrade->GetBrush().GetResourceObject()) : NULL;
}

UMaterialInstanceConstant* UB2LobbyUI_TotemInventorySlot::GetMainIconMIC()
{
	return IMG_TotemIcon.IsValid() ? Cast<UMaterialInstanceConstant>(IMG_TotemIcon->GetBrush().GetResourceObject()) : NULL;
}

void UB2LobbyUI_TotemInventorySlot::UpdateLockState(const bool& LockState)
{
	if (IMG_LockState.IsValid())
	{
		IMG_LockState->SetVisibility(LockState ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_TotemInventorySlot::UpdateIsSmelting(const FB2Totem& InTotem)
{
	if (InTotem.IsSmeltingComplete())
	{
		WS_SmeltingInfo->SetActiveWidgetIndex(iTOTEM_SMELING_STATE_NOT_COMPLETE);

		if (TB_Option0.IsValid() && InTotem.SubOptionDetails.IsValidIndex(0))
		{
			TB_Option0->SetText(FText::AsNumber(InTotem.SubOptionDetails[0].RefineCount));
		}
		if (TB_Option1.IsValid() && InTotem.SubOptionDetails.IsValidIndex(1))
		{
			TB_Option1->SetText(FText::AsNumber(InTotem.SubOptionDetails[1].RefineCount));
		}
		if (TB_Option2.IsValid() && InTotem.SubOptionDetails.IsValidIndex(2))
		{
			TB_Option2->SetText(FText::AsNumber(InTotem.SubOptionDetails[2].RefineCount));
		}
	}
	else
	{
		WS_SmeltingInfo->SetActiveWidgetIndex(iTOTEM_SMELING_STATE_COMPLETE);

		if (TB_IsSmelting.IsValid())
		{
			TB_IsSmelting->SetVisibility(InTotem.IsSmelting() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

void UB2LobbyUI_TotemInventorySlot::UpdateSmeltingLevel(const uint8& SmeltingLevel)
{
	if (TB_SmeltingLevel.IsValid())
	{
		if (SmeltingLevel <= 0)
		{
			TB_SmeltingLevel->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			TB_SmeltingLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TB_SmeltingLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::FromString(FString::FromInt(SmeltingLevel))));
		}
	}
}

void UB2LobbyUI_TotemInventorySlot::UpdateIMGTotemIcon(class UB2ItemInfo* pAllItemInfo, struct FSingleItemInfoData* pItemInfo)
{
	if (pAllItemInfo == nullptr || pItemInfo == nullptr)
	{
		return;
	}

	if (IMG_TotemIcon.IsValid())
	{
		if (CachedB2TotemInfo.IsSmeltingComplete())
		{
			IMG_TotemIcon->SetBrushFromMaterial(pItemInfo->GetIconMaterial(pAllItemInfo));
		}
		else
		{
			if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
			{
				IMG_TotemIcon->SetBrushFromMaterial(pAllItemMiscInfo->GetTotemIconNotSmelted());
			}
		}
	}
}

void UB2LobbyUI_TotemInventorySlot::UpdateIMGTotemGrade(const uint8& Grade)
{
	if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
	{
		const bool IsSimpleBG = false;
		const bool IsForConsumables = false;

		UMaterialInterface* pItemIconBGMtrl = pAllItemMiscInfo->GetItemIconBGMtrl(Grade, false, false);
		if (pItemIconBGMtrl && IMG_TotemGrade.IsValid())
		{
			IMG_TotemGrade->SetBrushFromMaterial(pItemIconBGMtrl);
		}
	}
}

void UB2LobbyUI_TotemInventorySlot::UpdateTBTotemName(struct FSingleItemInfoData* pItemInfo, const uint8& Grade)
{
	if (pItemInfo == nullptr)
	{
		return;
	}

	if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
	{
		if (TB_TotemName.IsValid())
		{
			TB_TotemName->SetText(BladeIIGameImpl::GetTotemDataStore().GetTotemName(CachedB2TotemInfo));
			TB_TotemName->SetColorAndOpacity(pAllItemMiscInfo->GetItemNameTextColor(Grade).GetSpecifiedColor());
		}
	}
}

void UB2LobbyUI_TotemInventorySlot::UpdateEquipMark(const FB2Totem& InTotem)
{
	if (O_EquipMark.IsValid())
	{
		O_EquipMark->SetVisibility(InTotem.IsEquipped() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_TotemInventorySlot::UpdateRedDot(const FB2Totem& InTotem)
{
	if (UIP_RedDot.IsValid())
		UIP_RedDot->SetVisibility(InTotem.IsNew ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UB2LobbyUI_TotemInventorySlot::ShouldShowDevUI() const
{
#if !UE_BUILD_SHIPPING
	return gIsDevUI ? true : false;
#endif
	return false;
}

FText UB2LobbyUI_TotemInventorySlot::GetItemRefId() const
{
#if !UE_BUILD_SHIPPING
	return FText::FromString(FString::Printf(TEXT("RefId: %d"), CachedB2TotemInfo.RefID));
#endif 

	return FText();
}

FText UB2LobbyUI_TotemInventorySlot::GetItemUId() const
{
#if !UE_BUILD_SHIPPING
	return FText::FromString(FString::Printf(TEXT("UId:%I64d"), CachedB2TotemInfo.ID));
#endif 

	return FText();
}

void UB2LobbyUI_TotemInventorySlot::OnPressedBTNItemUid()
{
//#if !UE_BUILD_SHIPPING
//	FPlatformMisc::ClipboardCopy(*FString::Printf(TEXT("%I64d"), CachedB2TotemInfo.ID));
//	UE_LOG(LogBladeII, Log, TEXT("ClipboardCopy Success! TotemUID : %I64d"), CachedB2TotemInfo.ID);
//#endif
}

void UB2LobbyUI_TotemInventorySlot::OnPressedBTNItemRefid()
{
//#if !UE_BUILD_SHIPPING
//	FPlatformMisc::ClipboardCopy(*FString::Printf(TEXT("%d"), CachedB2TotemInfo.RefID));
//	UE_LOG(LogBladeII, Log, TEXT("ClipboardCopy Success! TotemRefID : %d"), CachedB2TotemInfo.RefID);
//#endif
}

void UB2LobbyUI_TotemInventorySlot::OpenTotemInfoPopup(const FB2Totem& InTotem, const bool& IsLeftPopup /*= false*/)
{
	float fPoupSizeX = 0.f;

	const float fConvertScale = 1.f / UWidgetLayoutLibrary::GetViewportScale(this);

	FDisplayMetrics Metrics;
	FSlateApplication::Get().GetDisplayMetrics(Metrics);
	FVector2D vSafePaddingSize(Metrics.TitleSafePaddingSize);

	vSafePaddingSize.X *= fConvertScale;

	FVector2D vPopupPostion(vSafePaddingSize.X, 0);

	if (UGameViewportClient* ViewportClient = this->GetWorld()->GetGameViewport())
	{
		if (FSceneViewport* SceneViewport = ViewportClient->GetGameViewport())
		{
			fPoupSizeX = SceneViewport->GetCachedGeometry().GetLocalSize().X * fConvertScale * 0.5;
			fPoupSizeX -= vSafePaddingSize.X;
		}
	}

	UB2UI_TotemInfoPopup* pTotemInfoPopup = nullptr;

	if (IsLeftPopup)
	{
		pTotemInfoPopup = UB2UIManager::GetInstance()->OpenUI<UB2UI_TotemInfoPopup>(UIFName::TotemInfoPopupLeft);
	}
	else
	{
		vPopupPostion.X += fPoupSizeX;
		pTotemInfoPopup = UB2UIManager::GetInstance()->OpenUI<UB2UI_TotemInfoPopup>(UIFName::TotemInfoPopup);
	}

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	// 해당 user widget 생성 신호.
	// 생성된 Popup widget 을 UIP 처럼 배치해 넣을 패널 정보를 만들어준다.
	UB2LobbyUI_TotemInventoryMain* InventoryUI = LobbyGM ? Cast<UB2LobbyUI_TotemInventoryMain>(LobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	UPanelWidget* PopupAddPanel = InventoryUI ? InventoryUI->GetItemDetailPopupParentFullPanel() : nullptr;

	if (pTotemInfoPopup && PopupAddPanel)
	{
		PopupAddPanel->AddChild(pTotemInfoPopup);

		UOverlaySlot* CastedCPSlot = Cast<UOverlaySlot>(pTotemInfoPopup->Slot);
		if (CastedCPSlot)
		{
			CastedCPSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
			CastedCPSlot->SetVerticalAlignment(VAlign_Fill);
			CastedCPSlot->SetHorizontalAlignment(HAlign_Fill);
		}
			
		pTotemInfoPopup->InitTotemInfoPopup(vPopupPostion, fPoupSizeX, InTotem);
	}
}
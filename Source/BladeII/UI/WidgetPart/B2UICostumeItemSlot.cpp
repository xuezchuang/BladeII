// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UICostumeItemSlot.h"

#include "BladeIIUtil.h"
#include "B2ItemInfo.h"

#include "B2UIManager.h"
#include "B2UICostumeItemInfoPopup.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Slate/SceneViewport.h"

UB2UICostumeItemSlot::UB2UICostumeItemSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OpenItemInfoPopup = false;
}

void UB2UICostumeItemSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Image, IMG_ItemIcon);
	GET_SLOT(UB2Image, IMG_ItemGrade);
	GET_SLOT(UB2RichTextBlock, RTB_ItemName);

	GET_SLOT(UB2Image, IMG_ItemLock);

	GET_SLOT(UB2Button, BTN_CostumeItemSlot);

	GET_SLOT(UOverlay, O_EquipMark);
	GET_SLOT(UB2RichTextBlock, RTB_EquipMark);

	GET_SLOT(UTextBlock, TB_ItemEnhanceLevel);
	GET_SLOT(UTextBlock, TB_ItemCount);

	GET_SLOT(UB2UIWidgetBase, UIP_RedDot);
}

void UB2UICostumeItemSlot::UpdateStaticText()
{
	if (RTB_EquipMark.IsValid())
	{
		RTB_EquipMark->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Equiped_Preset")));
	}
	if (TB_ItemEnhanceLevel.IsValid())
	{
		TB_ItemEnhanceLevel->SetText(FText::GetEmpty());
	}
	if (TB_ItemCount.IsValid())
	{
		TB_ItemCount->SetText(FText::GetEmpty());
	}
}

void UB2UICostumeItemSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_CostumeItemSlot, &UB2UICostumeItemSlot::OnClickBTN_CostumeItemSlot);
}

void UB2UICostumeItemSlot::OnSetupManualScrollBoxSender(UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox)
{
	if (BTN_CostumeItemSlot.IsValid())
	{
		BTN_CostumeItemSlot->SetIsEnabled(false);
	}
}

void UB2UICostumeItemSlot::OnClickedSenderBTNArea()
{
	Super::OnClickedSenderBTNArea();

	PlayClickSound();

	OpenCostumeItemInfoPopup(CachedB2ItemInfo);
}

void UB2UICostumeItemSlot::OnClickBTN_CostumeItemSlot()
{
	if(OpenItemInfoPopup)
		OpenCostumeItemInfoPopup(CachedB2ItemInfo, true);
}

void UB2UICostumeItemSlot::UpdateCostumeItemSlot_FromFB2Item(const FB2Item& InItem)
{
	if (IsNotNeedUpdateCostumeItemSlot(InItem))
	{
		return;
	}

	CachedB2ItemInfo = InItem;

	if (UB2ItemInfo* pAllItemInfo = StaticFindItemInfo())
	{
		if (FSingleItemInfoData* pItemInfo = pAllItemInfo->GetInfoData(InItem.ItemRefID))
		{
			UpdateIMGItemIcon(pAllItemInfo, pItemInfo);
			UpdateRTBItemName(pItemInfo, InItem.StarGrade);
		}
	}

	UpdateIMGItemGrade(InItem.StarGrade);
	UpdateEquipMark(InItem);
	UpdateLockImage(InItem);
	UpdateEnhanceText(InItem);
	UpdateItemCountText(InItem);
	UpdateRedDot(InItem);
}

void UB2UICostumeItemSlot::SetOpenItemInfoPopup(bool OpenInfoPopup)
{
	OpenItemInfoPopup = OpenInfoPopup;
}

void UB2UICostumeItemSlot::UpdateIMGItemIcon(class UB2ItemInfo* pAllItemInfo, struct FSingleItemInfoData* pItemInfo)
{
	if (pAllItemInfo == nullptr || pItemInfo == nullptr)
	{
		return;
	}

	if (IMG_ItemIcon.IsValid())
	{
		IMG_ItemIcon->SetBrushFromMaterial(pItemInfo->GetIconMaterial(pAllItemInfo));
	}
}

void UB2UICostumeItemSlot::UpdateIMGItemGrade(const uint8& StarGrade)
{
	if (IMG_ItemGrade.IsValid())
	{
		// 일단 디폴트로 세팅해놓고..
		IMG_ItemGrade->SetBrushFromMaterial(DefaultIconBGMtrl);

		if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
		{
			const bool IsSimpleBG = false;
			const bool IsForConsumables = false;

			UMaterialInterface* pItemIconBGMtrl = pAllItemMiscInfo->GetCostumeIconBGMtrl(StarGrade);
			if (pItemIconBGMtrl)
			{
				IMG_ItemGrade->SetBrushFromMaterial(pItemIconBGMtrl);
			}
		}
	}
}

void UB2UICostumeItemSlot::UpdateRTBItemName(struct FSingleItemInfoData* pItemInfo, const uint8& StarGrade)
{
	if (pItemInfo == nullptr)
	{
		return;
	}

	if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
	{
		if (RTB_ItemName.IsValid())
		{
			RTB_ItemName->SetText(pItemInfo->GetLocalizedName());
			RTB_ItemName->SetColorAndOpacity(pAllItemMiscInfo->GetItemNameTextColor(StarGrade).GetSpecifiedColor());
		}
	}	
}

void UB2UICostumeItemSlot::UpdateEquipMark(const FB2Item& InItem)
{
	if (O_EquipMark.IsValid())
		O_EquipMark->SetVisibility(InItem.bCurrentlyEquipped ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UICostumeItemSlot::UpdateLockImage(const FB2Item& InItem)
{
	if (IMG_ItemLock.IsValid())
		IMG_ItemLock->SetVisibility(InItem.bIsLocked ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UICostumeItemSlot::UpdateEnhanceText(const FB2Item& InItem)
{
	if (TB_ItemEnhanceLevel.IsValid())
	{
		if (InItem.EnhanceLevel < 1)
			TB_ItemEnhanceLevel->SetText(FText::GetEmpty());
		else
			TB_ItemEnhanceLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::FromString(FString::FromInt(InItem.EnhanceLevel))));
	}
}

void UB2UICostumeItemSlot::UpdateItemCountText(const FB2Item& InItem)
{
	if (TB_ItemCount.IsValid())
	{
		if (InItem.InventoryType != EItemInvenType::EIIVT_Consumables)
			TB_ItemCount->SetText(FText::GetEmpty());
		else
		{
			if (InItem.ConsumingAmount >= 10000)
				TB_ItemCount->SetText(FText::FromString(TEXT("9999+")));
			else
				TB_ItemCount->SetText(FText::AsNumber(InItem.ConsumingAmount));
		}
	}
}

void UB2UICostumeItemSlot::UpdateRedDot(const FB2Item & InItem)
{
	if (UIP_RedDot.IsValid())
	{
		bool bVisible = InItem.IsNew && (InItem.InventoryType == EItemInvenType::EIIVT_Consumables);
		UIP_RedDot->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

bool UB2UICostumeItemSlot::IsNotNeedUpdateCostumeItemSlot(const FB2Item& InItem)
{
	if (CachedB2ItemInfo.InstanceUID &&	//이값이 0일 경우 프리뷰 아이템으로 무조건 업데이트 해야함
		CachedB2ItemInfo.InstanceUID == InItem.InstanceUID &&
		CachedB2ItemInfo.IsNew == InItem.IsNew &&
		CachedB2ItemInfo.bIsLocked == InItem.bIsLocked &&
		CachedB2ItemInfo.ConsumingAmount == InItem.ConsumingAmount
		)
	{
		return true;
	}

	return false;
}

void UB2UICostumeItemSlot::OpenCostumeItemInfoPopup(const FB2Item& InItem, const bool& IsLeftPopup /*= false*/)
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

	UB2UICostumeItemInfoPopup* pCostumeItemInfoPopup = nullptr;

	if (IsLeftPopup)
	{
		pCostumeItemInfoPopup = UB2UIManager::GetInstance()->OpenUI<UB2UICostumeItemInfoPopup>(UIFName::CostumeItemInfoPopupLeft);
	}
	else
	{
		vPopupPostion.X += fPoupSizeX;
		pCostumeItemInfoPopup = UB2UIManager::GetInstance()->OpenUI<UB2UICostumeItemInfoPopup>(UIFName::CostumeItemInfoPopup);
	}

	if (pCostumeItemInfoPopup)
	{
		pCostumeItemInfoPopup->InitCostumeItemInfoPopup(vPopupPostion, fPoupSizeX, InItem);
	}
}
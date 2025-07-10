// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationResultPopup.h"
#include "B2UIDocHelper.h"
#include "B2UIDocDonation.h"
#include "B2UIManager.h"
#include "Retailer.h"
#include "B2UISlotItem.h"
#include "B2UIDonationProductCost.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "B2RewardInfo.h"


void UB2UIDonationResultPopup::Init()
{
	Super::Init();

	IsTen = false;

	for (int i = 0; i < UIP_TenDonationResultItems.Num(); i++)
	{
		if (UIP_TenDonationResultItems[i].IsValid())
		{
			UIP_TenDonationResultItems[i]->Init();
			UIP_TenDonationResultItems[i]->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIDonationResultPopup::OnClickItem);
		}
	}

	if (UIP_FixedItem.IsValid())
	{
		UIP_FixedItem->Init();
	}

	if (UIP_OneDonationResultItem.IsValid())
	{
		UIP_OneDonationResultItem->Init();
		UIP_OneDonationResultItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIDonationResultPopup::OnClickItem);
	}

	if (UIP_DonationProductCost.IsValid())
	{
		UIP_DonationProductCost->Init();
	}

	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		DocDonation->SetIsResultPopup(true);
	}

	InitCostIcon();
}

void UB2UIDonationResultPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	Issues.Empty();

	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		DocDonation->SetIsResultPopup(false);
	}

	for (int i = 0; i < UIP_TenDonationResultItems.Num(); i++)
	{
		if (UIP_TenDonationResultItems[i].IsValid())
		{
			UIP_TenDonationResultItems[i]->DestroySelf(InUIManager);
		}
	}

	if (UIP_OneDonationResultItem.IsValid())
	{
		UIP_OneDonationResultItem->DestroySelf(InUIManager);
	}

	if (UIP_FixedItem.IsValid())
	{
		UIP_FixedItem->DestroySelf(InUIManager);
	}

	if (UIP_DonationProductCost.IsValid())
	{
		UIP_DonationProductCost->DestroySelf(InUIManager);
	}

	if (UIP_ItemDetailPopup.IsValid())
	{
		UIP_ItemDetailPopup->OnConfirmButtonClickedDelegate.RemoveAll(this);
		UIP_ItemDetailPopup->DestroySelf();
	}
}

void UB2UIDonationResultPopup::CacheAssets()
{
	Super::CacheAssets();

	UIP_TenDonationResultItems.SetNum(10);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[0], UIP_Item00);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[1], UIP_Item01);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[2], UIP_Item02);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[3], UIP_Item03);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[4], UIP_Item04);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[5], UIP_Item05);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[6], UIP_Item06);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[7], UIP_Item07);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[8], UIP_Item08);
	GET_SLOT_BYNAME(UB2UISlotItem, UIP_TenDonationResultItems[9], UIP_Item09);

	GET_SLOT(UB2UISlotItem, UIP_FixedItem);
	GET_SLOT(UB2UISlotItem, UIP_OneDonationResultItem);
	GET_SLOT(UB2UIDonationProductCost, UIP_DonationProductCost);
	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UB2Button, BTN_OneMore);
	GET_SLOT(UTextBlock, TB_OneMore);
	GET_SLOT(UTextBlock, TB_Confirm);
	GET_SLOT(UTextBlock, TB_CostAmount);
	GET_SLOT(UTextBlock, TB_DonationPointTitle);
	GET_SLOT(UTextBlock, TB_DoantionPoint);
	GET_SLOT(UTextBlock, TB_FixedItem);
	GET_SLOT(UB2LobbyUI_ItemDetailPostOp, UIP_ItemDetailPopup);
	GET_SLOT(UCanvasPanel, CP_DetailPopup);
	GET_SLOT(UImage, IMG_Icon);
	
	ANIM_FinalResult = GetAnimation(this, "ANIM_FinalResult");
}

void UB2UIDonationResultPopup::BindDelegates()
{
	Super::BindDelegates();

	Issues.Add(UpdateDonationResultPopupClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->UpdateUIInfo();
	}
	));

	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIDonationResultPopup::OnClickConfirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_OneMore, &UB2UIDonationResultPopup::OnClickOneMore);

	if (UIP_ItemDetailPopup.IsValid())
	{
		UIP_ItemDetailPopup->OnConfirmButtonClickedDelegate.RemoveAll(this);
		UIP_ItemDetailPopup->OnConfirmButtonClickedDelegate.AddDynamic(this, &UB2UIDonationResultPopup::CloseItemDetailPopup);
	}
}

void UB2UIDonationResultPopup::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_OneMore.IsValid())
	{
		TB_OneMore->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_OneMoreButton")));
	}

	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_ConfirmButton")));
	}

	if (TB_DonationPointTitle.IsValid())
	{
		TB_DonationPointTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_UserPointTitle")));
	}

	if (TB_FixedItem.IsValid())
	{
		TB_FixedItem->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_CertainRewardTitle")));
	}
}

void UB2UIDonationResultPopup::CloseWidgetDelegate()
{
	OnClickConfirm();
}

void UB2UIDonationResultPopup::OnClickConfirm()
{
	data_trader::Retailer::GetInstance().RequestGetDonationEvent();
}

void UB2UIDonationResultPopup::OnClickOneMore()
{
	UB2UIDocDonation* DonationDoc = UB2UIDocHelper::GetDocDonation();
	if (DonationDoc)
	{
		int32 DonationID = DonationDoc->GetDonationID();
		bool IsFree = false;
		if (IsTen == false && DonationDoc->GetFreeDonationCount() > 0)
		{
			IsFree = true;
		}

		data_trader::Retailer::GetInstance().RequestMakeDonation(DonationID, IsTen, true, IsFree, DonationDoc->GetLastDonationKey());
	}
}

void UB2UIDonationResultPopup::OnClickItem(UB2UISlotItem* ClickedItem)
{
	if (ClickedItem && IsEquipmentItemByType(ClickedItem->GetDocStruct()))
	{
		if (ANIM_FinalResult.IsValid())
		{
			PlayAnimation(ANIM_FinalResult.Get());
		}

		if (CP_DetailPopup.IsValid())
		{
			CP_DetailPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (UIP_ItemDetailPopup.IsValid())
		{
			UIP_ItemDetailPopup->UpdateItemData(ClickedItem->GetDocStruct(), ClickedItem->GetDocStruct());
		}
	}
}

void UB2UIDonationResultPopup::CloseItemDetailPopup()
{
	if (CP_DetailPopup.IsValid())
	{
		CP_DetailPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIDonationResultPopup::GetResultItems(OUT TArray<FB2Item>& OutItem)
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		TArray<FDonationPresent> RewardPresents;
		RewardPresents.Empty();
		DocDonation->GetDonationPresents(RewardPresents);

		//장비 아이템을 제외한 나머지
		for (int i = 0; i < RewardPresents.Num(); i++)
		{
			if (RewardPresents[i].DonationConsumableType != b2network::B2DonationConsumableType::UNDEFINED)
			{
				FB2Item RewardPreset = FB2Item(BladeIIGameImpl::GetClientDataStore().GetItemMasterDetailInfo(RewardPresents[i].ItemTemplateID));
				RewardPreset.ConsumingAmount = RewardPresents[i].Amount;
				OutItem.Add(RewardPreset);
			}
		}

		//장비 아이템
		TArray<FB2Item> RewardItems;
		RewardItems.Empty();
		DocDonation->GetDonationRewardItem(RewardItems);
		for (FB2Item& Elem : RewardItems)
		{
			if (IsEquipmentItemByType(Elem))
			{
				OutItem.Add(Elem);
			}
		}
	}
}

void UB2UIDonationResultPopup::InitCostIcon()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		if (IMG_Icon.IsValid())
		{
			int32 CostType = DocDonation->GetCostType();
			switch (CostType)
			{
			case b2network::B2DonationCostType::GEM:
				IMG_Icon->SetBrushFromMaterial(GemIcon.LoadSynchronous());
				break;
			case b2network::B2DonationCostType::GOLD:
				IMG_Icon->SetBrushFromMaterial(GoldIcon.LoadSynchronous());
				break;
			}
		}
	}
}

void UB2UIDonationResultPopup::UpdateUIInfo()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		if (TB_DoantionPoint.IsValid())
		{
			TB_DoantionPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_UserPoint")), DocDonation->GetDonationPoint()));
		}

		if (TB_CostAmount.IsValid())
		{
			int32 CostType = DocDonation->GetCostType();
			switch (CostType)
			{
			case b2network::B2DonationCostType::GEM:
				TB_CostAmount->SetText(FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetGemAmount()));
				break;
			case b2network::B2DonationCostType::GOLD:
				TB_CostAmount->SetText(FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetGoldAmount()));
				break;
			}
		}

		TArray<FB2Item> ResultItems;
		ResultItems.Empty();
		GetResultItems(ResultItems);
		
		if (ResultItems.Num() > 1)
		{
			IsTen = true;
			UpdateTenUIInfo(ResultItems);
		}
		else
		{
			IsTen = false;
			if (ResultItems.IsValidIndex(0))
			{
				UpdateOneUIInfo(ResultItems[0]);
			}
		}
	}
}

void UB2UIDonationResultPopup::UpdateTenUIInfo(TArray<FB2Item>& InResultItems)
{
	for (int i = 0; i < UIP_TenDonationResultItems.Num(); i++)
	{
		if (UIP_TenDonationResultItems[i].IsValid() && InResultItems.IsValidIndex(i))
		{
			UIP_TenDonationResultItems[i]->BindDoc(InResultItems[i]);
		}
	}
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		if (UIP_FixedItem.IsValid())
		{
			UB2RewardInfo* RewardInfo = StaticFindRewardInfo();
			if (RewardInfo)
			{
				FBundleFixedReward BundleFixedReward = DocDonation->GetBundleFixedReward();
				int32 ItemTemplateID = RewardInfo->GetItemInfoIndex(BundleFixedReward.ID);
				FB2Item RewardFixedItem = FB2Item(BladeIIGameImpl::GetClientDataStore().GetItemMasterDetailInfo(ItemTemplateID));
				RewardFixedItem.ConsumingAmount = BundleFixedReward.Amount;

				UIP_FixedItem->BindDoc(RewardFixedItem);
			}
		}

		if (UIP_DonationProductCost.IsValid())
		{
			UIP_DonationProductCost->SetCost(DocDonation->GetOneMoreTenCost());
		}
	}

	PlayTenOpen();
}

void UB2UIDonationResultPopup::UpdateOneUIInfo(FB2Item& InResultItem)
{
	if (UIP_OneDonationResultItem.IsValid())
	{
		UIP_OneDonationResultItem->BindDoc(InResultItem);
	}

	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		if (UIP_DonationProductCost.IsValid())
		{
			UIP_DonationProductCost->SetCost(DocDonation->GetOneMoreOneCost(), DocDonation->GetFreeDonationCount());
		}
	}

	PlayOneOpen();
}

#if WITH_EDITOR
void UB2UIDonationResultPopup::EditorOnPostLoad()
{
	GemIcon.LoadSynchronous();
	GoldIcon.LoadSynchronous();
}
#endif

// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UISummonItemDisplay.h"
#include "B2UIManager.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2UISlotItem.h"
#include "B2UIDocHelper.h"

#include "B2UIStoreProductCost.h"
#include "Retailer.h"
#include "TutorialManager.h"
#include "B2PCClassInfoBox.h"
////#include "Matinee/MatineeActor.h"
//#include "Animation/SkeletaMeshActor."
#include "Engine/Engine.h"
#include "B2LobbyUi_ItemDetail.h"
#include "B2UIUtil.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIMiniStorePopup.h"
#include "B2UIUtil.h"

const FName UB2UISummonItemDisplaySlot::Name_FxMID_ItemBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2UISummonItemDisplaySlot::Name_FxMID_ItemIcon(TEXT("MAT_ItemIcon"));

UB2UISummonItemDisplaySlot::UB2UISummonItemDisplaySlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemCardFX = nullptr;
	ItemCardBackFX = nullptr;
}

void UB2UISummonItemDisplaySlot::Init()
{
	DeleteCard_FrontFX();
	DeleteCard_BackFX();

	Super::Init();

	ShowVisibility_ItemDataBP(false);

	bFlipped = false;
	SoftAttachedBoneName = NAME_None;

}

void UB2UISummonItemDisplaySlot::BeginDestroy()
{
	DeleteCard_FrontFX();
	DeleteCard_BackFX();

	Super::BeginDestroy();
}

void UB2UISummonItemDisplaySlot::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Flip);
	GET_SLOT(UB2UISlotItem, UIP_ItemDetail);
	if (UIP_ItemDetail.IsValid())
		UIP_ItemDetail->Init();
}

void UB2UISummonItemDisplaySlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Flip, &UB2UISummonItemDisplaySlot::OnClickBTN_Flip);
}

void UB2UISummonItemDisplaySlot::BindBTNFlip(const FOnFlipped& Delegate)
{
	OnFlip = Delegate;
}

void UB2UISummonItemDisplaySlot::BindBTNViewDetail(const FOnViewDetailItem& Delegate)
{
	OnViewDetail = Delegate;
}

void UB2UISummonItemDisplaySlot::DisplaySlotHidden(bool IsHidden)
{
	bShowDetatil = IsHidden;

	if (ItemCardFX)
		DeleteCard_FrontFX();
	if (ItemCardBackFX)
		DeleteCard_BackFX();

	SetVisibility(IsHidden ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	bFlipped = false;
}

void UB2UISummonItemDisplaySlot::UpdateItemData(const FB2Item& InItem)
{
	ItemInfoData = InItem;

	if (UIP_ItemDetail.IsValid())
		UIP_ItemDetail->BindDoc(InItem);
}

void UB2UISummonItemDisplaySlot::SetWorldTransform(const FVector& WorldLoc)
{
	ItemCardWorldLoc = WorldLoc;

	if (ItemCardFX)
	{
		ItemCardFX->SetWorldTransform(GetTransform());
	}

	if (ItemCardBackFX)
	{
		ItemCardBackFX->SetWorldTransform(GetTransform());
	}
}

void UB2UISummonItemDisplaySlot::SetWorldTransform(const FTransform& InTransform)
{
	if (ItemCardFX)
	{
		ItemCardFX->SetWorldTransform(InTransform);
	}

	if (ItemCardBackFX)
	{
		ItemCardBackFX->SetWorldTransform(InTransform);
	}
}

void UB2UISummonItemDisplaySlot::SetWorldTransform(const FMatrix& InMatrix)
{
	ItemCardMatrix = InMatrix;
	if (ItemCardFX)
	{
		ItemCardFX->SetWorldTransform(FTransform(CameraRotator + FRotator(0.f, 90.f, 0.f), InMatrix.GetOrigin(), InMatrix.GetScaleVector()));
	}

	if (ItemCardBackFX)
	{
		ItemCardBackFX->SetWorldTransform(FTransform(CameraRotator + FRotator(0.f, 90.f, 0.f), InMatrix.GetOrigin(), InMatrix.GetScaleVector()));
	}
}

void UB2UISummonItemDisplaySlot::SetCameraRotator(FRotator InRotator)
{
	CameraRotator = InRotator;
}

FTransform UB2UISummonItemDisplaySlot::GetTransform()
{
	return FTransform(CameraRotator + FRotator(0.f, 90.f, 0.f), ItemCardMatrix.GetOrigin(), ItemCardMatrix.GetScaleVector());
}

void UB2UISummonItemDisplaySlot::OnClickBTN_Flip()
{
	if (bFlipped)
	{
		OnViewDetail.ExecuteIfBound(ItemInfoData);
	}
	else
	{
		ShowItemCard_Front();

		bFlipped = true;
		OnFlip.ExecuteIfBound();
	}
}

void UB2UISummonItemDisplaySlot::ShowItemCard_Front()
{
	bShowDetatil = false;
	ShowVisibility_ItemDataBP(true);

	if (UIP_ItemDetail.IsValid())
	{
		UIP_ItemDetail->ShowIcon(false);
	}
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	DeleteCard_BackFX();

	CreateCard_FrontFX();
}

void UB2UISummonItemDisplaySlot::ShowItemCard_Back()
{
	ShowVisibility_ItemDataBP(false);

	DeleteCard_FrontFX();

	CreateCard_BackFX();
}

void UB2UISummonItemDisplaySlot::CreateCard_FrontFX()
{
	if (!ItemCardFXs.IsValidIndex(ItemInfoData.StarGrade - 1))
		return;

	ItemCardFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemCardFXs[ItemInfoData.StarGrade - 1], GetTransform());

	// 아이템 material 을 셋업한다
	SetupNamedMIDForFxComp(ItemCardFX);

	if (UIP_ItemDetail.IsValid())
	{
		// ParticleSystem 셋업이 Grade 별로 따로 들어간다고 하니 이 Background panel 셋업은 필요하지 않을 수도.
		UMaterialInstanceDynamic* ItemBGPanelMID = GetFxNamedMID(ItemCardFX, Name_FxMID_ItemBGPanel);
		UMaterialInstanceConstant* ItemBGPanelMIC_Ref = Cast<UMaterialInstanceConstant>(UIP_ItemDetail->GetActiveItemBGPanelMaterial());
		SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);

		UMaterialInstanceDynamic* ItemIconMID = GetFxNamedMID(ItemCardFX, Name_FxMID_ItemIcon);
		UMaterialInstanceConstant* ItemIconMIC_Ref = Cast<UMaterialInstanceConstant>(UIP_ItemDetail->GetIconMaterial());
		SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
	}
}

void UB2UISummonItemDisplaySlot::DeleteCard_FrontFX()
{
	if (ItemCardFX)
	{
		ItemCardFX->ConditionalBeginDestroy();
		ItemCardFX = nullptr;
	}
}

void UB2UISummonItemDisplaySlot::CreateCard_BackFX()
{
	if (ItemCardBackFXs.Num() < 2)
		return;

	if (ItemCardBackFX)
		DeleteCard_BackFX();

	ItemCardBackFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemCardBackFXs[ItemInfoData.StarGrade < ITEM_SUMMON_JACKPOT_GRADE_LV1 ? 0 : 1], GetTransform());
}

void UB2UISummonItemDisplaySlot::DeleteCard_BackFX()
{
	if (ItemCardBackFX)
	{
		ItemCardBackFX->ConditionalBeginDestroy();
		ItemCardBackFX = nullptr;
	}
}

void UB2UISummonItemDisplay::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2LobbyUI_ItemDetailPostOp, UIP_SummonedDetail);

	UIP_SummonedItems.AddZeroed(10 - UIP_SummonedItems.Num());
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[0], UIP_Summoned01);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[1], UIP_Summoned02);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[2], UIP_Summoned03);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[3], UIP_Summoned04);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[4], UIP_Summoned05);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[5], UIP_Summoned06);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[6], UIP_Summoned07);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[7], UIP_Summoned08);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[8], UIP_Summoned09);
	GET_SLOT_BYNAME(UB2UISummonItemDisplaySlot, UIP_SummonedItems[9], UIP_Summoned10);

	for (auto& UIP : UIP_SummonedItems)
	{
		if (UIP.IsValid())
			UIP->Init();
	}

	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UTextBlock, TB_Confirm);
	GET_SLOT(UCanvasPanel, CP_Confirm);
	if (CP_Confirm.IsValid())
		CP_Confirm->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UOverlay, OV_SummonItemDetail);
	if (OV_SummonItemDetail.IsValid())
		OV_SummonItemDetail->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UOverlay, OV_EssenceDetail);
	if (OV_EssenceDetail.IsValid())
		OV_EssenceDetail->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UB2Button, BTN_EssenceClose);
	GET_SLOT(UB2LobbyUI_ItemDetail, Lobby_ItemDetailCommonBP);

	if (OV_EssenceDetail.IsValid())
		OV_EssenceDetail->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UOverlay, OV_SummonItems);
	if (OV_SummonItems.IsValid())
		OV_SummonItems->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UB2Button, BTN_OneMore);
	GET_SLOT(UCanvasPanel, CP_OneMore);
	if (CP_OneMore.IsValid())
		CP_OneMore->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UB2UIStoreProductCost, UIP_OneMoreCost);
	if (UIP_OneMoreCost.IsValid())
		UIP_OneMoreCost->Init();

	GET_SLOT(UB2Button, BTN_OpenTouch);
	GET_SLOT(UTextBlock, TB_ItemCount);
	GET_SLOT(UTextBlock, TB_OneMore);

	GET_SLOT(UPanelWidget, CP_Gem);
	if (CP_Gem.IsValid())
	{
		CP_Gem->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UTextBlock, TB_GemAmount);
	if (TB_GemAmount.IsValid())
	{
		TB_GemAmount->SetText(FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetGemAmount()));
	}

	GET_SLOT(UOverlay, OV_SaleForSummon);
	if (OV_SaleForSummon.IsValid())
		OV_SaleForSummon->SetVisibility(ESlateVisibility::Collapsed);
	GET_SLOT(UTextBlock, TB_SaleForSummon);
}

void UB2UISummonItemDisplay::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UISummonItemDisplay::OnClickBTN_Confirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_OneMore, &UB2UISummonItemDisplay::OnClickBTN_SummonOneMore);
	BIND_CLICK_FUNC_TO_BTN(BTN_OpenTouch, &UB2UISummonItemDisplay::OnClickBTN_OpenTouch);

	if (UIP_SummonedDetail.IsValid())
	{
		UIP_SummonedDetail->OnConfirmButtonClickedDelegate.RemoveAll(this);
		UIP_SummonedDetail->OnConfirmButtonClickedDelegate.AddDynamic(this, &UB2UISummonItemDisplay::CloseDetailSummonedItem);
	}

	if (Lobby_ItemDetailCommonBP.IsValid())
		Lobby_ItemDetailCommonBP->OnCloseButtonClickedDelegate.AddDynamic(this, &UB2UISummonItemDisplay::CloseDetailSummonedItem);
}

void UB2UISummonItemDisplay::CloseWidgetDelegate()
{
	if (BTN_OpenTouch.IsValid() && BTN_OpenTouch->GetVisibility() == ESlateVisibility::Visible)
	{
		OnClickBTN_OpenTouch();
	}
	else if (CP_Confirm.IsValid() && CP_Confirm->GetVisibility() == ESlateVisibility::Visible)
	{
		OnClickBTN_Confirm();
	}
}

UB2UISummonItemDisplay::UB2UISummonItemDisplay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CardSlotSizeScale_Single = 0.9f;
	CardSlotSizeScale_Multiple10 = 0.65f;
}

void UB2UISummonItemDisplay::Init()
{
	Super::Init();

	bFinishedFlip = false;
	bCardOpenTouch = false;
	bStoreFinish = false;
	bOpenCards = false;
	OpenCardNumberResult = 0;
	bOpenCardsTime = 0.f;

	BTN_OpenTouch->SetVisibility(ESlateVisibility::Visible);
}

void UB2UISummonItemDisplay::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISummonItemDisplay::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	CAPTURE_UOBJECT(UB2UISummonItemDisplay);

	EnterShopTicket = EnterShopClass<int32>::GetInstance().Subscribe(			//영혼잼 모잘라서 상점갔다가 뒤로가기했을시 오면XX 예외처리
		USE_CAPTURE_OBJECT(int32 ShopWhere)
		Capture->EnterShopException();
	END_CAPTURE_OBJECT()
		);
}

void UB2UISummonItemDisplay::UpdateStaticText()
{
	if (TB_OneMore.IsValid())
		TB_OneMore->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_OneMore")));

	if (TB_SaleForSummon.IsValid())
		TB_SaleForSummon->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_SaleForSummon")));
}

void UB2UISummonItemDisplay::OnClose(bool RightNow)
{
	for (auto& UIP : UIP_SummonedItems)
	{
		if (UIP.IsValid())
		{
			UIP->DeleteCard_FrontFX();
			UIP->DeleteCard_BackFX();
		}
	}

	EnterShopClass<int32>::GetInstance().Unsubscribe(EnterShopTicket);

	Super::OnClose(RightNow);
}

void UB2UISummonItemDisplay::OpenDetailSummonedItem(const FB2Item& InItem)
{
	/*if (InItem.ItemClass == EItemClass::EIC_WeaponEssence ||
	InItem.ItemClass == EItemClass::EIC_AccessoryEssence ||
	InItem.ItemClass == EItemClass::EIC_ProtectionEssence)
	{
	if (OV_EssenceDetail.IsValid())
	OV_EssenceDetail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Lobby_ItemDetailCommonBP.IsValid())
	{
	Lobby_ItemDetailCommonBP->UpdateItemData(InItem);
	Lobby_ItemDetailCommonBP->SetVisibility(ESlateVisibility::Visible);
	Lobby_ItemDetailCommonBP->SetFindUserInfoButtonHidden();
	}

	}
	else
	{*/
	if (OV_SummonItemDetail.IsValid())
		OV_SummonItemDetail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (UIP_SummonedDetail.IsValid())
	{
		UIP_SummonedDetail->UpdateItemData(InItem, InItem);
		B2AndroidBackManager::GetInstance()->AddBackWidget(UIP_SummonedDetail.Get());
	}

	PlayAnimSummonOneFlip();

	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(bFinishedFlip ? BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")) : BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_FlipAll")));
	}

	if (CP_OneMore.IsValid())
	{
		CP_OneMore->SetVisibility((SummonedItemFrom == ESummonedItemFrom::SummonItemShop &&  bFinishedFlip) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		if (TutorialManager::GetInstance().HasAnyTutorial())
			CP_OneMore->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (bFinishedFlip)
	{
		/*if (CP_Confirm.IsValid())
		CP_Confirm->SetVisibility(ESlateVisibility::SelfHitTestInvisible);*/
	}
}

void UB2UISummonItemDisplay::CloseDetailSummonedItem()
{
	if (OV_SummonItemDetail.IsValid())
		OV_SummonItemDetail->SetVisibility(ESlateVisibility::Collapsed);

	if (UIP_SummonedDetail.IsValid())
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(UIP_SummonedDetail.Get());
	}

	if (OV_EssenceDetail.IsValid())
		OV_EssenceDetail->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UISummonItemDisplay::OnClickBTN_OpenTouch()
{
	if (bCardOpenTouch)
	{
		if (OpenCardNumber == 0)
		{
			if (OpenCardMaxNumber == 1)
			{
				UIP_SummonedItems[OpenCardNumber]->OnClickBTN_Flip();
			}
			else
			{
				for (int32 Idx = 0; Idx < UIP_SummonedItems.Num(); ++Idx)			//우선 두번만 돌도록 설정
				{
					UIP_SummonedItems[Idx]->DisplaySlotHidden(true);
				}
				auto GameMode = Cast<AB2LobbyGameMode>(GetWorld()->GetAuthGameMode());

				if (!GameMode)
					return;
				OpenCardNumber = 10;
				GameMode->PlaySummonItemDisplayMatineeResult();

				PlaySoundOpenAllCard();
				bCardOpenTouch = false;
			}
			return;
		}
		else
		{
			if (OpenCardMaxNumber == 1)
				PlaySoundOpenSingleCard();
			else
				PlaySoundOpenEachCard();
		}

		if (OpenCardNumber > 0)
		{
			UIP_SummonedItems[OpenCardNumber - 1]->DisplaySlotHidden(true);
		}

		if (UIP_SummonedItems.IsValidIndex(OpenCardNumber))
		{
			UIP_SummonedItems[OpenCardNumber]->DeleteCard_FrontFX();
			UIP_SummonedItems[OpenCardNumber]->DisplaySlotHidden(true);
		}

		bCardOpenTouch = false;
		SummonitemOpenCardMatineePlayClass<int32>::GetInstance().Signal(OpenCardNumber);
		UpdateButtonState(OpenCardNumber - 1);
	}
}

EStoreItemCost UB2UISummonItemDisplay::GetStoreItemCostType()
{
	EStoreItemCost CostType = EStoreItemCost::Gem;

	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int32 SlotHashKey = DocStore->GetCurrentSummonItemSlotHashKey();

		if (auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SlotHashKey))
		{
			CostType = (static_cast<ESummonItemCost>(Data->GetCostType()) == ESummonItemCost::Gem) ? EStoreItemCost::Gem : EStoreItemCost::FriendshipPoint;
		}
	}

	return CostType;
}

bool UB2UISummonItemDisplay::OpenAppStoreReview()
{
	if (GetStoreItemCostType() != EStoreItemCost::FriendshipPoint
		&& OpenCardMaxNumber >= 10)
	{
		if (UB2UIManager::GetInstance()->OpenPopupAppStoreReview())
		{
			return true;
		}
	}

	return false;
}

void UB2UISummonItemDisplay::OnClickBTN_Confirm()
{
	if (bStoreFinish)
	{
		if (SummonedItemFrom == ESummonedItemFrom::SummonItemShop)
		{
			if (OpenAppStoreReview())
			{
				return;
			}
			else
			{
				LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SummonItemStore);
			}
		}
		else if (SummonedItemFrom == ESummonedItemFrom::LotteryMail)
		{
			auto* DocSome = UB2UIDocHelper::GetDocSome();
			if (DocSome)
				DocSome->SetOpenedMailTab(MailTabIndex::EMailTab_Ticket);

			data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(MailTabIndex::EMailTab_Ticket));
		}

		auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
		auto* LobbyInventory = LobbyGameMode ? LobbyGameMode->GetLobbyInventory() : nullptr;
		if (LobbyInventory)
		{
			LobbyInventory->ConditionalCheckForBetterEquipSuggest();
		}
	}
	else
	{
		for (int32 Idx = 0; Idx < UIP_SummonedItems.Num(); ++Idx)
		{
			UIP_SummonedItems[Idx]->DisplaySlotHidden(true);
		}

		auto GameMode = Cast<AB2LobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (!GameMode)
			return;

		OpenCardNumber = 10;
		GameMode->PlaySummonItemDisplayMatineeResult();

		PlaySoundOpenAllCard();

		bCardOpenTouch = false;
		/*for (int32 Idx = 0; Idx < UIP_SummonedItems.Num(); ++Idx)
		{
		if (UIP_SummonedItems[Idx].IsValid() && !UIP_SummonedItems[Idx]->IsFlipped())
		{
		UIP_SummonedItems[Idx]->OnClickBTN_Flip();
		}
		}*/
	}
}

void UB2UISummonItemDisplay::OnClickBTN_SummonOneMore()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	auto* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocStore && DocUser)
	{
		int32 SlotHashKey = DocStore->GetCurrentSummonItemSlotHashKey();
		if (B2UIUtil::IsLimitedBuySummonSlot(SlotHashKey))
			return;

		auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SlotHashKey);
		if (Data)
		{
			// 연속뽑기이고 10회뽑기권이 있다면 10회뽑기권을 우선으로 소모
			int32 CurrentCostType = static_cast<int32>(Data->GetCostType()) + b2network::B2ShopDrawPriceType::GEM;

			bool IsTenTicket = (BladeIIGameImpl::GetClientDataStore().GetShopTenLotteryTicket() > 0 &&
				Data->IsMultiple()
				&& Data->GetCostType() == static_cast<int32>(ESummonItemCost::Gem));

			if (IsTenTicket) // 10회 뽑기권일 경우 예외처리
				CurrentCostType = b2network::B2ShopDrawPriceType::SHOP_TEN_LOTTERY_TICKET;

			bool bIsSummonItemBuy = true;
			if (CurrentCostType == b2network::B2ShopDrawPriceType::GEM)
			{
				int32 UserCost = DocUser->GetGem();
				int32 BuyCost = Data->GetCost();
				int32 CalculatedCost = Data->GetCalculatedCost();

				if (CalculatedCost > 0)
				{
					if (UserCost < CalculatedCost)
						bIsSummonItemBuy = false;
				}
				else
				{
					if (UserCost < BuyCost)
						bIsSummonItemBuy = false;
				}
			}

			if (bIsSummonItemBuy || IsTenTicket)
			{
				EPCClass PCClass = DocStore->GetCurrentSummonItemSelectedPCClass();
				UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, PCClass) ? InventoryFullSimplePopup(PCClass) :
					data_trader::Retailer::GetInstance().RequestDrawShopLottery(PCClass, CurrentCostType, Data->IsMultiple(), Data->IsAgain(), DocStore->GetLastLotteryKey(), false);
			}
			else
			{
				UB2UIManager::GetInstance()->OpenUI<UB2UIMiniStorePopup>(UIFName::MiniStorePopup);
			}
				
		}
	}
}

void UB2UISummonItemDisplay::InventoryFullSimplePopup(EPCClass PCClass)
{
	B2UIUtil::OpenInventoryFullPopup(PCClass, FMsgPopupOnClick::CreateLambda([this, PCClass]() {
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SummonItemStore);
		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
		LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);
	}));
}

float UB2UISummonItemDisplay::GetCardSlotSizeScale() const
{ // bCardResult 가 큰거 하나 보여줄지 작은 거 여러개 보여줄 지 그건 듯
	return bCardResult ? CardSlotSizeScale_Multiple10 : CardSlotSizeScale_Single;
}

void UB2UISummonItemDisplay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bNeedCardLocationUpdate)
		UpdateLocationSummonItemFXs();

	/*if (bOpenCards)
	{
	bOpenCardsTime += InDeltaTime;
	if (bOpenCardsTime > 0.1f)
	{
	UIP_SummonedItems[OpenCardNumberResult]->OnClickBTN_Flip();
	++OpenCardNumberResult;

	bOpenCardsTime = 0;

	if (OpenCardNumberResult >= 10)
	{

	bOpenCards = false;
	bStoreFinish = true;
	BTN_OpenTouch->SetVisibility(ESlateVisibility::Hidden);
	UpdateButtonState(OpenCardNumber);
	}
	}

	}*/

	Super::NativeTick(MyGeometry, InDeltaTime);
}

//#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
//#include "Matinee/InterpData.h"

void UB2UISummonItemDisplay::ChangeSummonItemCardMatineeActorToFX(const TArray<FB2Item>& Items, ALevelSequenceActor * Matinee)
{
	check(Items.Num() > 0);

	//if (OV_SummonItemDetail.IsValid())
	//	OV_SummonItemDetail->SetVisibility(ESlateVisibility::Collapsed);

	//if (OV_SummonItems.IsValid())
	//	OV_SummonItems->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//SummonBoxActor = nullptr;
	//SummonBoxActorBG = nullptr;

	//ACameraActor*	CameraInfo = nullptr;
	//FRotator CameraRotatorInfo(0.f, 0.f, 0.f);
	//for (UInterpGroupInst* ThisGroupInst : Matinee->GroupInst)
	//{
	//	if (ThisGroupInst && ThisGroupInst->Group)
	//	{
	//		if (ThisGroupInst->Group->GroupName == (OpenCardNumber == 1 ? TEXT("Loot_Card") : TEXT("Loot_Card10")))
	//			SummonBoxActor = Cast<ASkeletalMeshActor>(ThisGroupInst->GetGroupActor());					// 뽑힌 카드 가져오고


	//		if (ThisGroupInst->Group->GroupName == TEXT("Loot_Card10BG"))
	//			SummonBoxActorBG = Cast<ASkeletalMeshActor>(ThisGroupInst->GetGroupActor());				//대기중인카드도 얻어오고

	//		if (ThisGroupInst->Group->GroupName == TEXT("Cam01"))
	//			CameraInfo = Cast<ACameraActor>(ThisGroupInst->GetGroupActor());			//카메라가 각도가 다르다 ㅠㅠ 카메라도 얻어오자...
	//	}
	//}

	//if (CameraInfo)
	//	CameraRotatorInfo = FRotator(CameraInfo->GetActorRotation().Pitch * -1, CameraInfo->GetActorRotation().Yaw + 90.f, CameraInfo->GetActorRotation().Roll * -1);

	//checkfSlow(SummonBoxActor != nullptr, TEXT("%s matinee doesn't have group named %s"), *Matinee->GetName(), (Items.Num() == 1 ? TEXT("Loot Card") : TEXT("Loot Card")));

	//bCardResult = Matinee->GetName() == TEXT("LootBox_C10_Allopen") ? true : false;

	//float AdjustUISize = GetCardSlotSizeScale();

	//int32 idxNumber = bCardResult == false ? ((OpenCardNumber == 1 || !SummonBoxActorBG.Get()) ? OpenCardNumber - 1 : OpenCardNumber - 2) : 0;

	//UpdateButtonState(!SummonBoxActorBG.Get() ? OpenCardNumber : OpenCardNumber - 1);

	//for (int32 Idx = 0; Idx < UIP_SummonedItems.Num(); ++Idx)			//우선 두번만 돌도록 설정
	//{
	//	if (UIP_SummonedItems[Idx].IsValid())
	//	{
	//		if (Items.IsValidIndex(Idx))
	//		{
	//			if (idxNumber <= Idx && OpenCardNumber > Idx)
	//			{
	//				UIP_SummonedItems[Idx]->SetRenderScale(FVector2D(AdjustUISize, AdjustUISize));
	//				UIP_SummonedItems[Idx]->SetCameraRotator(CameraRotatorInfo);
	//				UIP_SummonedItems[Idx]->BindBTNFlip(FOnFlipped::CreateLambda([this]() {
	//					this->CheckFlippedCardIfFinished();
	//					this->SetItemsCount(this->GetItemsCount() - 1);
	//				}));
	//				UIP_SummonedItems[Idx]->BindBTNViewDetail(FOnViewDetailItem::CreateLambda([this](const FB2Item& InItem) {this->OpenDetailSummonedItem(InItem); }));

	//				UIP_SummonedItems[Idx]->UpdateItemData(Items[Idx]);
	//				UIP_SummonedItems[Idx]->ShowItemCard_Back();
	//				UIP_SummonedItems[Idx]->SetVisibility(ESlateVisibility::HitTestInvisible);

	//				if (bCardResult)
	//					UIP_SummonedItems[Idx]->SetAttachBoneName(*FString::Printf(TEXT("Mesh-Card%c"), 'A' + Idx));
	//				else
	//				{
	//					UIP_SummonedItems[Idx]->SetAttachBoneName(*FString::Printf(TEXT("Mesh-CardD")));
	//				}
	//			}
	//			else
	//			{
	//				UIP_SummonedItems[Idx]->SetAttachBoneName(*FString::Printf(TEXT("NAME_None")));
	//				UIP_SummonedItems[Idx]->DisplaySlotHidden(true);
	//			}
	//		}
	//		else
	//		{
	//			UIP_SummonedItems[Idx]->DisplaySlotHidden(true);
	//			UIP_SummonedItems[Idx]->SetAttachBoneName(*FString::Printf(TEXT("NAME_None")));
	//		}
	//	}
	//}

	//UpdateLocationSummonItemFXs();

	//bNeedCardLocationUpdate = true;
}

void UB2UISummonItemDisplay::UpdateLocationSummonItemFXs()
{
	//BII_CHECK(SummonBoxActor.IsValid())
	//	if (bCardResult)
	//	{
	//		for (int32 i = 0; i < OpenCardMaxNumber; ++i)
	//		{
	//			if (SummonBoxActor.IsValid())
	//			{
	//				UpdateLocationSummonItemFX(SummonBoxActor->GetSkeletalMeshComponent(), i);
	//			}
	//		}
	//		return;
	//	}

	//if (SummonBoxActor.IsValid())
	//{
	//	UpdateLocationSummonItemFX(SummonBoxActor->GetSkeletalMeshComponent(), OpenCardNumber - 1);
	//}
	//if (SummonBoxActorBG.IsValid())
	//{
	//	UpdateLocationSummonItemFX(SummonBoxActorBG->GetSkeletalMeshComponent(), OpenCardNumber - 2);
	//}
}

void UB2UISummonItemDisplay::UpdateLocationSummonItemFX(USkeletalMeshComponent* Mesh, int32 Index)
{
	if (Mesh)
	{
		if (!UIP_SummonedItems[Index].IsValid() || UIP_SummonedItems[Index]->GetAttachedBoneName() == TEXT("NAME_None"))
			return;

		UIP_SummonedItems[Index]->SetWorldTransform(Mesh->GetBoneMatrix(Mesh->GetBoneIndex(UIP_SummonedItems[Index]->GetAttachedBoneName())));
	}
}


#include "Blueprint/WidgetLayoutLibrary.h"
void UB2UISummonItemDisplay::UpdateLocationSummonItemUIs()
{
	//int32 ItemNum = 0;

	//APlayerController* LocalPlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
	//if (LocalPlayerController)
	//{
	//	// UMG 에서 쓸 UIPosition 계산 시에 현재 해상도가 아닌 기준 해상도 기준으로 넣어주어야 하므로 뷰 스케일이 필요함.
	//	float DPIScale = UWidgetLayoutLibrary::GetViewportScale(LocalPlayerController);

	//	ULocalPlayer* const LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
	//	if (LP && LP->ViewportClient)
	//	{
	//		// get the projection data
	//		FSceneViewProjectionData ProjectionData;
	//		if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
	//		{
	//			FMatrix ViewProjMat = ProjectionData.ComputeViewProjectionMatrix();
	//			int32 idxNumber = bCardResult == false ? ((OpenCardNumber == 1 || !SummonBoxActorBG.Get()) ? OpenCardNumber - 1 : OpenCardNumber - 2) : 0;


	//			for (int32 Idx = 0; Idx < UIP_SummonedItems.Num(); ++Idx)
	//			{
	//				if (idxNumber > Idx && OpenCardNumber <= Idx)
	//					continue;

	//				if (UIP_SummonedItems[Idx].IsValid() && UIP_SummonedItems[Idx]->IsVisible())
	//				{
	//					APlayerController* OwningPC = GetOwningPlayer();
	//					OpenCardMaxNumber == 1 ? BTN_OpenTouch->SetVisibility(ESlateVisibility::Hidden) : BTN_OpenTouch->SetVisibility(ESlateVisibility::Visible);
	//					// 위치 조절을 위해 CanvasPanelSlot 에 배치해야 함.
	//					UCanvasPanelSlot* ThisSlot = Cast<UCanvasPanelSlot>(UIP_SummonedItems[Idx]->Slot);
	//					if (OwningPC && ThisSlot)
	//					{
	//						FVector2D vProjectedLocation(0.f, 0.f);
	//						OwningPC->ProjectWorldLocationToScreen(UIP_SummonedItems[Idx]->GetWorldLocation(), vProjectedLocation);

	//						// 여기서부터는 기준 해상도 사이즈로..
	//						vProjectedLocation /= (DPIScale > KINDA_SMALL_NUMBER ? DPIScale : 1.0f);

	//						// 슬롯 사이즈만큼 보정. Size to Content 했을 시 GetDesiredSize 로.. 
	//						FVector2D ItemSlotSize = ThisSlot->GetAutoSize() ? UIP_SummonedItems[Idx]->GetDesiredSize() : ThisSlot->GetSize();
	//						// 스케일이 추가로 있어서.. 근데 GetDesiredSize 면 Scale 이 반영되어 있을 듯..?
	//						ItemSlotSize *= ThisSlot->GetAutoSize() ? 1.0f : GetCardSlotSizeScale();
	//						vProjectedLocation.X -= (ItemSlotSize.X * 0.5f);
	//						vProjectedLocation.Y -= (ItemSlotSize.Y * 0.5f);

	//						ThisSlot->SetAnchors(FAnchors(0.f, 0.f)); // 이런 건 진작에 맞춰놓는 걸로..
	//						ThisSlot->SetPosition(vProjectedLocation);
	//					}
	//				}
	//			}

	//			if (bCardResult == false)
	//			{
	//				UIP_SummonedItems[OpenCardNumber - 1]->OnClickBTN_Flip();
	//			}
	//		}
	//	}
	//}

	//CheckFlippedCardIfFinished();

	//bNeedCardLocationUpdate = false;
}

void UB2UISummonItemDisplay::EnterShopException()
{
	UB2UIManager::GetInstance()->RemoveUISceneHistory(EUIScene::SummonItemResult);
}

void UB2UISummonItemDisplay::CheckFlippedCardIfFinished()
{
	bool Check = true;
	for (int32 Idx = 0; Idx < UIP_SummonedItems.Num(); ++Idx)
	{
		if (UIP_SummonedItems[Idx].IsValid() && UIP_SummonedItems[Idx]->IsVisible())
		{
			Check &= UIP_SummonedItems[Idx]->IsFlipped();
		}
	}

	if (Check != bFinishedFlip || Check)
	{
		bFinishedFlip = Check;

		if (OpenCardMaxNumber == 1)
		{
			bStoreFinish = true;
		}
	}

	UpdateButtonState(OpenCardNumber);

	TUTORIAL
		if (TutorialManager::GetInstance().HasAnyTutorial())
		{
			if (CP_OneMore.IsValid())
				CP_OneMore->SetVisibility(ESlateVisibility::Collapsed);
		}
}

void UB2UISummonItemDisplay::NotifyOpenCard(int32 Inindex)
{
	if (bOpenCards)
	{
		if (UIP_SummonedItems.Num() > Inindex)
		{
			UIP_SummonedItems[Inindex]->OnClickBTN_Flip();
			++OpenCardNumberResult;

			if (OpenCardNumberResult >= 10)
			{
				bOpenCards = false;
				bStoreFinish = true;
				BTN_OpenTouch->SetVisibility(ESlateVisibility::Hidden);
				UpdateButtonState(OpenCardNumber);
			}
		}
	}
}

void UB2UISummonItemDisplay::OnFinishedSummonItemDisplayResultMatinee()
{
	bOpenCards = true;
	OpenCardNumberResult = 0;
	bOpenCardsTime = 0.f;
	bStoreFinish = false;

	UpdateLocationSummonItemUIs();
}

void UB2UISummonItemDisplay::UpdateButtonState(int32 CardNumber)
{
	if (TB_Confirm.IsValid())
		TB_Confirm->SetText(!bCardResult && OpenCardMaxNumber == 10 ? BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_FlipAll")) : BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));

	bool VisibilityButtonState = false;

	if (CP_Confirm.IsValid() && TB_Confirm.IsValid())
	{
		if (TB_ItemCount.IsValid())
		{
			int32 RemainCard = OpenCardNumber;
			TB_ItemCount->SetVisibility(bStoreFinish || bCardResult || !RemainCard ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
			TB_ItemCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), RemainCard)));
		}

		if (bStoreFinish)		//결과창 10장짜리일때
		{
			TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
			VisibilityButtonState = true;
		}
		else
		{
			if (OpenCardMaxNumber == 10 && CardNumber >= 0 && !bCardResult) // 10장짜리 처음에
				VisibilityButtonState = true;
		}
		CP_Confirm->SetVisibility(VisibilityButtonState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (CP_OneMore.IsValid())
		CP_OneMore->SetVisibility(bStoreFinish ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	EStoreItemCost CostType = EStoreItemCost::Gem;
	if (DocStore && UIP_OneMoreCost.IsValid())
	{
		int32 SlotHashKey = DocStore->GetCurrentSummonItemSlotHashKey();
		auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SlotHashKey);
		BII_CHECK(Data);
		
		int32 Cost = Data->GetCost();
		int32 CalculatedCost = Data->GetCalculatedCost();
		if (static_cast<ESummonItemCost>(Data->GetCostType()) == ESummonItemCost::Gem)
		{
			if (BladeIIGameImpl::GetClientDataStore().GetShopTenLotteryTicket() > 0 && Data->IsMultiple())
			{
				CostType = EStoreItemCost::TenLotteryTicket;
				CalculatedCost = 0;
			}
			else
			{
				CostType = EStoreItemCost::Gem;
			}
		}
		else
		{
			CostType = EStoreItemCost::FriendshipPoint;
		}

		SetSummonOneMoreCost(Cost, CalculatedCost, CostType);

		if (OV_SaleForSummon.IsValid())
		{
			bool bIsTenCostGem = Data->IsMultiple() && CostType == EStoreItemCost::Gem ? true : false;
			OV_SaleForSummon->SetVisibility(bStoreFinish && bIsTenCostGem ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}

	if (CP_Gem.IsValid())
		CP_Gem->SetVisibility(bStoreFinish && CostType == EStoreItemCost::Gem ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

// Tutorial - 300_5 소환 아이템 Click시 Detail정보가 뜨는것을 방지하기 위한 하드코딩
TUTORIAL void UB2UISummonItemDisplay::Tutorial_SetSummonSlotFix()
{
	if (UB2UISummonItemDisplaySlot* SummonSlot = UIP_SummonedItems[0].Get())
	{
		FOnViewDetailItem EmptyDelegate;
		SummonSlot->BindBTNViewDetail(EmptyDelegate);
	}
}

void UB2UISummonItemDisplay::OnStartDrawShopLottery(const int32 nLotteryCount)
{
	OnStartDrawShopLottery_BP(nLotteryCount);
}

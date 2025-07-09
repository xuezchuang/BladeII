// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_TotemInventoryMain.h"

#include "B2LobbyUI_TotemEquipSlot.h"
#include "B2LobbyUI_TotemInventorySlot.h"
#include "B2LobbyUI_HeroMgmtCommon.h"
#include "B2UI_TotemInvenFilterPopup.h"
#include "BladeIIGameImpl.h"
#include "B2UIManager.h"
#include "B2ScrollBoxWithGridPanel.h"
#include "B2AndroidBackManager.h"
#include "Retailer.h"

#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"

UB2LobbyUI_TotemInventoryMain::UB2LobbyUI_TotemInventoryMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeroMgmtCommonBP = NULL;
}

void UB2LobbyUI_TotemInventoryMain::Init()
{
	BindDelegates();
	UpdateStaticText();

	SubscribeEvents();
	BladeIIGameImpl::GetClientDataStore().RequestGetAccountTotem();
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

void UB2LobbyUI_TotemInventoryMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	Init();

	// HeroMgmtCommon 값의 각종 위젯을 토템 인벤토리에 맞게 세팅
	if (HeroMgmtCommonBP.IsValid())
	{
		HeroMgmtCommonBP->StartFromLobby(InUIManager, InGM);
		HeroMgmtCommonBP->ShowCharacterButtons(true);
		HeroMgmtCommonBP->ShowEnhanceItemSetButton(false);
		HeroMgmtCommonBP->ShowWingButton(false);
		HeroMgmtCommonBP->ShowHeadButton(false);
		HeroMgmtCommonBP->ShowDetailedInfoExtraButtons(true);
		HeroMgmtCommonBP->SetHidePresetControls();
	}

	DoMarkRedDot();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Totem")));
}

void UB2LobbyUI_TotemInventoryMain::DestroySelf()
{
	if (BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Totem())
	{
		data_trader::Retailer::GetInstance().RequestModifyTotemNewFlag();
		BladeIIGameImpl::GetLocalCharacterData().SetAllTotemsNewFlagOff();
	}

	DestroyAllTotemIcons();

	if (HeroMgmtCommonBP.IsValid())
	{
		HeroMgmtCommonBP->DestroySelf();
	}
	
	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		pUIManager->CloseAllStandaloneUIs();

		if (UIP_ScrollBoxWithGridPanelBP.IsValid())
		{
			UIP_ScrollBoxWithGridPanelBP->DestroySelf(pUIManager);
		}
		if (UIP_TotemEquipSlotList.IsValid())
		{
			UIP_TotemEquipSlotList->DestroySelf(pUIManager);
		}
	}

	UnsubscribeEvents();
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	Super::DestroySelf();
}

void UB2LobbyUI_TotemInventoryMain::DoMarkRedDot()
{
	Super::DoMarkRedDot();

	if (HeroMgmtCommonBP.IsValid())
	{
		HeroMgmtCommonBP->DoMarkRedDot();
	}
}

void UB2LobbyUI_TotemInventoryMain::UpdateStoredTotem()
{
	if (UIP_ScrollBoxWithGridPanelBP.IsValid() == false)
	{
		return;
	}

	DestroyAllTotemIcons();

	TArray<FB2Totem> CurrentTotems;
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInven)
	{
		LobbyInven->GetUnEquippedTotemList(CurrentTotems);
	}

	SortTotemSlot(CurrentTotems);

	const int32 iTotemListArrayMax = CurrentTotems.Num();
	for (int32 i = 0; i < iTotemListArrayMax; ++i)
	{
		if (CurrentTotems.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayTotemSlot.IsValidIndex(i) == false)
		{
			auto* pTotemSlot = CreateWidget<UB2LobbyUI_TotemInventorySlot>(GetOwningPlayer(), TotemSlotClass);
			pTotemSlot->Init();

			if (auto pGridSlot = Cast<UUniformGridSlot>(UIP_ScrollBoxWithGridPanelBP->UniformGridPanelAddChild(pTotemSlot)))
			{
				pGridSlot->SetRow(i / iHorizontalMax);
				pGridSlot->SetColumn(i % iHorizontalMax);
			}

			ArrayTotemSlot.Add(pTotemSlot);
		}

		ArrayTotemSlot[i]->SetupManualScrollBoxSender_byReceiver(this, UIP_ScrollBoxWithGridPanelBP->GetB2ScrollBox());
		ArrayTotemSlot[i]->SetVisibility(ESlateVisibility::Visible);
		ArrayTotemSlot[i]->UpdateTotemSlot_FromFB2Totem(CurrentTotems[i]);
	}

	const int32 iTotemSlotArrayMax = ArrayTotemSlot.Num();
	for (int32 i = iTotemListArrayMax; i < iTotemSlotArrayMax; ++i)
	{
		if (ArrayTotemSlot.IsValidIndex(i))
		{
			ArrayTotemSlot[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2LobbyUI_TotemInventoryMain::DestroyAllTotemIcons()
{
	if (UIP_ScrollBoxWithGridPanelBP.IsValid() == false)
	{
		return;
	}

	for (int32 i = 0; i < ArrayTotemSlot.Num() ; ++i)
	{
		if (ArrayTotemSlot.IsValidIndex(i))
		{
			ArrayTotemSlot[i]->DestroySelf(UB2UIManager::GetInstance());
		}
	}

	ArrayTotemSlot.Empty();
	UIP_ScrollBoxWithGridPanelBP->SetUniformGridPanelClearChildren();
}

void UB2LobbyUI_TotemInventoryMain::CloseWidgetDelegate()
{
	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		UIManager->ChangeUISceneBack();
	}
}

void UB2LobbyUI_TotemInventoryMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock,				TB_TotemInventory);

	GET_SLOT(UTextBlock,				TB_AutoDecomposition);
	GET_SLOT(UTextBlock,				TB_SellAll);

	GET_SLOT(UB2Button,					BTN_AutoDecomposition);
	GET_SLOT(UB2Button,					BTN_SellAll);

	GET_SLOT(UCanvasPanel,				X_LeftPopupPanel);
	GET_SLOT(UCanvasPanel,				X_RightPopupPanel);

	GET_SLOT(UB2LobbyUI_HeroMgmtCommon,	HeroMgmtCommonBP);
	GET_SLOT(UB2ScrollBoxWithGridPanel, UIP_ScrollBoxWithGridPanelBP);
	if (UIP_ScrollBoxWithGridPanelBP.IsValid())
	{
		UIP_ScrollBoxWithGridPanelBP->Init();
	}

	GET_SLOT(UB2LobbyUI_TotemEquipSlotList,	UIP_TotemEquipSlotList);
	if (UIP_TotemEquipSlotList.IsValid())
	{
		UIP_TotemEquipSlotList->Init();
	}
}

void UB2LobbyUI_TotemInventoryMain::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_AutoDecomposition,	&UB2LobbyUI_TotemInventoryMain::OnClickBTN_AutoDecomposition);
	BIND_CLICK_FUNC_TO_BTN(BTN_SellAll,				&UB2LobbyUI_TotemInventoryMain::OnClickBTN_SellAll);

	// 준비되면 레드닷 추가 필요
	//REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_OpenInvenPage.Get()), &UB2UILobby_CostumeMain::RedDotCondition_Inven);
}

void UB2LobbyUI_TotemInventoryMain::UpdateStaticText()
{
	if (TB_TotemInventory.IsValid())
	{
		TB_TotemInventory->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_inventory")));
	}
	if (TB_AutoDecomposition.IsValid())
	{
		TB_AutoDecomposition->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoDecomposition")));
	}
	if (TB_SellAll.IsValid())
	{
		TB_SellAll->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SellAll")));
	}
}

void UB2LobbyUI_TotemInventoryMain::InitTotemScrollOffset()
{
	if (UIP_ScrollBoxWithGridPanelBP.IsValid())
	{
		const float fInitScrollOffset = 0.f;
		UIP_ScrollBoxWithGridPanelBP->SetB2ScrollOffset(fInitScrollOffset);
	}
}

void UB2LobbyUI_TotemInventoryMain::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
		[this](EPCClass SelectionPCClass)
	{
		this->CloseAllStandaloneUIFromUIManager();
	}
	));

	Issues.Add(SuccesGetAccountTotemClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->UpdateStoredTotem();
	}
	));

	Issues.Add(UpdateLobbyInventoryControlClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->UpdateStoredTotem();
	}
	));

	Issues.Add(DeliveryModifyTotemNewFlagClass<FB2ModifyTotemNewFlagPtr>::GetInstance().Subscribe2(
		[this](const FB2ModifyTotemNewFlagPtr& RefineTotemPtr)
	{
		this->UpdateStoredTotem();
	}
	));
}

void UB2LobbyUI_TotemInventoryMain::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2LobbyUI_TotemInventoryMain::SortTotemSlot(TArray<FB2Totem>& InOutSortingTotems)
{
	InOutSortingTotems.Sort([](const FB2Totem& A, const FB2Totem& B)
	{
		// 뉴 > 잠금 > 제련 완료 > 등급 > 제련중인지 > 제련레벨
		bool Result = false;
		if (A.IsNew == true && B.IsNew == false)
		{
			Result = true;
		}
		else if (A.IsNew == B.IsNew)
		{
			if (A.IsLock == true && B.IsLock == false)
			{
				Result = true;
			}
			else if (A.IsLock == B.IsLock)
			{
				if (A.IsSmeltingComplete() == true && B.IsSmeltingComplete() == false)
				{
					Result = true;
				}
				else if (A.IsSmeltingComplete() == B.IsSmeltingComplete())
				{
					if (A.Grade > B.Grade)
					{
						Result = true;
					}
					else if (A.Grade == B.Grade)
					{
						if (A.IsSmelting() == true && B.IsSmelting() == false)
						{
							Result = true;
						}
						else if (A.IsSmelting() == B.IsSmelting())
						{
							if (A.RefineLevel > B.RefineLevel)
							{
								Result = true;
							}
						}
					}
				}
			}
		}
		return Result;
	});
}

void UB2LobbyUI_TotemInventoryMain::CloseAllStandaloneUIFromUIManager()
{
	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		pUIManager->CloseAllStandaloneUIs();
	}
}

FText UB2LobbyUI_TotemInventoryMain::GetContainerSizeText() const
{
	return FText::AsNumber(MAX_TOTEM_INVENTORY_SLOT);
}

FText UB2LobbyUI_TotemInventoryMain::GetStoredItemNumText() const
{
	return FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetAllTotemsCount());
}

FSlateColor UB2LobbyUI_TotemInventoryMain::GetStoredItemNumTextColor() const
{
	return BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().IsOverTotemInventory() ? StoredTotemNumTextColor_Over : StoredTotemNumTextColor_Normal;
}

void UB2LobbyUI_TotemInventoryMain::OnClickBTN_AutoDecomposition()
{
	UB2UI_TotemInvenDecompositionAll* pTotemDecompositionAllPopup = nullptr;

	pTotemDecompositionAllPopup = UB2UIManager::GetInstance()->OpenUI<UB2UI_TotemInvenDecompositionAll>(UIFName::TotemDecompositionAllPopup);
	if (pTotemDecompositionAllPopup)
	{
		pTotemDecompositionAllPopup->InitTotemInvenFilterPopup();
	}
}

void UB2LobbyUI_TotemInventoryMain::OnClickBTN_SellAll()
{
	UB2UI_TotemInvenSellAll* pTotemSellAllPopup = nullptr;

	pTotemSellAllPopup = UB2UIManager::GetInstance()->OpenUI<UB2UI_TotemInvenSellAll>(UIFName::TotemSellAllPopup);
	if (pTotemSellAllPopup)
	{
		pTotemSellAllPopup->InitTotemInvenFilterPopup();
	}
}

UCanvasPanel* UB2LobbyUI_TotemInventoryMain::GetItemDetailPopupParentPanel(bool bOfLeftSide) const
{
	return bOfLeftSide ? X_LeftPopupPanel.Get() :
		// 우측팝업은 HeroMgmt 공통파트의 헤더파트에서 나오는 모드 알림을 가리지 않도록 HeroMgmt 쪽 Panel 을 사용한다.
		// 여기 widget 들 배치 시 순서 및 z-order 주의해야 함.
		(HeroMgmtCommonBP.Get() ? HeroMgmtCommonBP->GetInfoPopupParentPanel(bOfLeftSide) : X_RightPopupPanel.Get());
}

UOverlay* UB2LobbyUI_TotemInventoryMain::GetItemDetailPopupParentFullPanel() const
{
	return (HeroMgmtCommonBP.Get() ? HeroMgmtCommonBP->GetInfoPopupParentFullPanel() : nullptr);
}

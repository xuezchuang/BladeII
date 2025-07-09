// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_ExtendInventory.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "Event.h"
#include "B2ClientDataStore.h"

#include "BladeIIGameImpl.h"

UB2LobbyUI_ExtendInventory::UB2LobbyUI_ExtendInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrSelectedPCClass = EPCClass::EPC_End;
	CurrSelectedInvenType = EItemInvenType::EIIVT_End;

}

void UB2LobbyUI_ExtendInventory::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	check(CachedLobbyGM);
	UB2LobbyInventory* LobbyInven = CachedLobbyGM->GetLobbyInventory();
	check(LobbyInven); // 없으면 안됨 ㅋ
	
	CurrSelectedPCClass = CachedLobbyGM->GetHeroMgmtModePCClass();
	CurrSelectedInvenType = LobbyInven->GetMainInvenViewCurrentTab();

	//구조가 다른 위젯...
	CalculateData();
	BindDelegates();
	UpdateStaticText();
	
	if (IMG_Portrait.IsValid())
		IMG_Portrait->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(PortraitInfo, CurrSelectedPCClass));

	UpdateWidgets(0);

	OnOpen_BP();
}

void UB2LobbyUI_ExtendInventory::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_CurrentLabel);
	GET_SLOT(UTextBlock, TB_PostExtendLabel);
	GET_SLOT(UTextBlock, TB_CurrentNumber);
	GET_SLOT(UTextBlock, TB_PostExtendNumber);
	GET_SLOT(UTextBlock, TB_MaxSlotCountInfo);
	GET_SLOT(UTextBlock, TB_ExtendBtn);
	GET_SLOT(UTextBlock, TB_ExtendCost);
	GET_SLOT(UTextBlock, TB_MaxExtend);

	GET_SLOT(UB2Button, BTN_Down);
	GET_SLOT(UB2Button, BTN_Up);
	GET_SLOT(UB2Button, BTN_MaxExtend);
	
	GET_SLOT(USlider, S_CashExtendSlider);

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_Extend);

	GET_SLOT(UImage, IMG_Portrait);
}

void UB2LobbyUI_ExtendInventory::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ExtendInventory::OnClickBtnClose)
	BIND_CLICK_FUNC_TO_BTN(BTN_Extend, &UB2LobbyUI_ExtendInventory::OnClickBtnExtend)
	
	BIND_CLICK_FUNC_TO_BTN(BTN_Down, &UB2LobbyUI_ExtendInventory::OnClickBtnDown)
	BIND_CLICK_FUNC_TO_BTN(BTN_Up, &UB2LobbyUI_ExtendInventory::OnClickBtnUp)
	BIND_CLICK_FUNC_TO_BTN(BTN_MaxExtend, &UB2LobbyUI_ExtendInventory::OnClickBtnMaxExtend)

	if (S_CashExtendSlider.IsValid())
		S_CashExtendSlider->OnValueChanged.AddDynamic(this, &UB2LobbyUI_ExtendInventory::OnChangedExtendCount);
}

void UB2LobbyUI_ExtendInventory::UpdateStaticText()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenExtend_Title")));
	}
	if (TB_CurrentLabel.IsValid())
	{
		TB_CurrentLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenExtend_CurrentLabel")));
	}
	if (TB_PostExtendLabel.IsValid())
	{
		TB_PostExtendLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenExtend_PostExtendLabel")));
	}

	if (TB_MaxSlotCountInfo.IsValid())
	{
		TB_MaxSlotCountInfo->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenExtend_MaxSlotCountInfo"))), FText::AsNumber(SlotSliderLogic.GetMaxAllowedSlotCount()))
		);
	}
	if (TB_ExtendBtn.IsValid())
	{
		TB_ExtendBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenExtend_SingleExtendBtn")));
	}
	if (TB_MaxExtend.IsValid())
	{
		TB_MaxExtend->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Maximum")));
	}
}

void UB2LobbyUI_ExtendInventory::UpdateWidgets(int32 NewExtendCount)
{
	int32 PostExtendSlotCount = SlotSliderLogic.GetSlotCount(NewExtendCount);
	float SliderValue = SlotSliderLogic.ExtendCountToSliderValue(NewExtendCount);

	if (S_CashExtendSlider.IsValid())
	{
		S_CashExtendSlider->SetStepSize(SlotSliderLogic.GetSliderStepSize());
		S_CashExtendSlider->SetValue(SliderValue);
	}

	if (TB_CurrentNumber.IsValid())
	{
		TB_CurrentNumber->SetText(FText::AsNumber(SlotSliderLogic.GetCurrentSlotCount()));
	}
	if (TB_PostExtendNumber.IsValid())
	{
		TB_PostExtendNumber->SetText(FText::AsNumber(PostExtendSlotCount));
	}

	if (TB_ExtendCost.IsValid())
	{
		TB_ExtendCost->SetText(FText::AsNumber(GetRealExtendCount() * CashPerSingleExtend));
	}

	if (BTN_Extend.IsValid())
	{
		BTN_Extend->SetIsEnabled(!SlotSliderLogic.IsAlreadyMaxSlotCount()); // 사실상 Disable 되는 상황에서 이게 열리지는 않아야 함.
	}
	if (BTN_MaxExtend.IsValid())
	{
		BTN_MaxExtend->SetIsEnabled(PostExtendSlotCount < SlotSliderLogic.GetMaxAllowedSlotCount()); // 사실상 Disable 되는 상황에서 이게 열리지는 않아야 함.
	}
}

void UB2LobbyUI_ExtendInventory::CalculateData()
{
	if (CachedLobbyGM && CachedLobbyGM->GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_HeroManagement) // 장비에서만 체크 하면 될듯...?
	{
		if (CurrSelectedPCClass != EPCClass::EPC_End && CurrSelectedInvenType != EItemInvenType::EIIVT_End && CurrSelectedInvenType != EItemInvenType::EIIVT_Consumables) // 소모품 탭에서는 이 메뉴가 열려선 안된다.
		{
			int32 CurrentSlotCount, SlotCountPerSingleExtend, MaxAllowedSlotCount;
			CurrentSlotCount = UB2LobbyInventory::GetItemContainerSize(CurrSelectedInvenType, CurrSelectedPCClass);
			BladeIIGameImpl::GetClientDataStore().GetInventoryExtendReferenceData(CashPerSingleExtend, SlotCountPerSingleExtend, MaxAllowedSlotCount);
			SlotSliderLogic.SetData(CurrentSlotCount, SlotCountPerSingleExtend, MaxAllowedSlotCount);
		}
	}
	else
	{
		if (CurrSelectedPCClass != EPCClass::EPC_End) 
		{
			CurrSelectedInvenType = EItemInvenType::EIIVT_Weapon;
			int32 CurrentSlotCount, SlotCountPerSingleExtend, MaxAllowedSlotCount;
			CurrentSlotCount = UB2LobbyInventory::GetItemContainerSize(CurrSelectedInvenType, CurrSelectedPCClass);
			BladeIIGameImpl::GetClientDataStore().GetInventoryExtendReferenceData(CashPerSingleExtend, SlotCountPerSingleExtend, MaxAllowedSlotCount);
			SlotSliderLogic.SetData(CurrentSlotCount, SlotCountPerSingleExtend, MaxAllowedSlotCount);
		}
	}
}

int32 UB2LobbyUI_ExtendInventory::GetSliderExtendCount()
{
	if (S_CashExtendSlider.IsValid())
		return SlotSliderLogic.SliderValueToExtendCount(S_CashExtendSlider->GetValue());

	return 0;
}

int32 UB2LobbyUI_ExtendInventory::GetRealExtendCount()
{
	if (S_CashExtendSlider.IsValid())
		return SlotSliderLogic.GetRealExtendCount(S_CashExtendSlider->GetValue());

	return 0;
}

void UB2LobbyUI_ExtendInventory::OnClickBtnDown()
{
	UpdateWidgets(GetSliderExtendCount() - 1);
}

void UB2LobbyUI_ExtendInventory::OnClickBtnUp()
{
	UpdateWidgets(GetSliderExtendCount() + 1);
}

void UB2LobbyUI_ExtendInventory::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2LobbyUI_ExtendInventory::OnChangedExtendCount(float ChangeValue)
{
	UpdateWidgets(GetSliderExtendCount());
}

void UB2LobbyUI_ExtendInventory::OnClickBtnClose()
{
	CloseMe();
}

void UB2LobbyUI_ExtendInventory::OnClickBtnExtend()
{
	UB2LobbyInventory::ReqForExtendInventory(CurrSelectedPCClass, CurrSelectedInvenType, GetRealExtendCount()); // 안에서 추가 예외처리를 할 것..

	CloseMe(); // 유지하는 걸 원한다면 이거 대신 UpdateWidgets 를.

	//// 여기서 필요한 건 꼬락 MaxPossibleExtendNum ㅋ
	//int32 CurrentSlotNum = 0, PostExtendSlotNum = 0, SingleExtendCost = 0, MaxAllowedSlotNum = 0, MaxPossibleExtendNum = 0, MaxExtendCost = 0;
	//GetAllNecessaryData(CurrentSlotNum, PostExtendSlotNum, SingleExtendCost, MaxAllowedSlotNum, MaxPossibleExtendNum, MaxExtendCost);

	//UB2LobbyInventory::ReqForExtendInventory(CurrSelectedPCClass, CurrSelectedInvenType, MaxPossibleExtendNum);

	//CloseMe();
}

void UB2LobbyUI_ExtendInventory::OnClickBtnMaxExtend()
{
	UpdateWidgets(SlotSliderLogic.GetMaxExtendCount());
}

void UB2LobbyUI_ExtendInventory::CloseMe()
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ExtendInventory);
}
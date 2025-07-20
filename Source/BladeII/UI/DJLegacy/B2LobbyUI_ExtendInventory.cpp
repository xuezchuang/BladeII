// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_ExtendInventory.h"
//#include "BladeII.h"

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
	check(LobbyInven); // 绝栏搁 救凳 せ
	
	CurrSelectedPCClass = CachedLobbyGM->GetHeroMgmtModePCClass();
	CurrSelectedInvenType = LobbyInven->GetMainInvenViewCurrentTab();

	//备炼啊 促弗 困连...
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
		BTN_Extend->SetIsEnabled(!SlotSliderLogic.IsAlreadyMaxSlotCount()); // 荤角惑 Disable 登绰 惑炔俊辑 捞霸 凯府瘤绰 臼酒具 窃.
	}
	if (BTN_MaxExtend.IsValid())
	{
		BTN_MaxExtend->SetIsEnabled(PostExtendSlotCount < SlotSliderLogic.GetMaxAllowedSlotCount()); // 荤角惑 Disable 登绰 惑炔俊辑 捞霸 凯府瘤绰 臼酒具 窃.
	}
}

void UB2LobbyUI_ExtendInventory::CalculateData()
{
	if (CachedLobbyGM && CachedLobbyGM->GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_HeroManagement) // 厘厚俊辑父 眉农 窍搁 瞪淀...?
	{
		if (CurrSelectedPCClass != EPCClass::EPC_End && CurrSelectedInvenType != EItemInvenType::EIIVT_End && CurrSelectedInvenType != EItemInvenType::EIIVT_Consumables) // 家葛前 徘俊辑绰 捞 皋春啊 凯妨急 救等促.
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
	UB2LobbyInventory::ReqForExtendInventory(CurrSelectedPCClass, CurrSelectedInvenType, GetRealExtendCount()); // 救俊辑 眠啊 抗寇贸府甫 且 巴..

	CloseMe(); // 蜡瘤窍绰 吧 盔茄促搁 捞芭 措脚 UpdateWidgets 甫.

	//// 咯扁辑 鞘夸茄 扒 部遏 MaxPossibleExtendNum せ
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
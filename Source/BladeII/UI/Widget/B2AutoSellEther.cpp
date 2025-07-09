// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2AutoSellEther.h"
#include "B2UIManager.h"
#include "B2EtherManager.h"
#include "Retailer.h"
#include "B2UIDecompositionObtainPopup.h"
#include "B2UIClientIndicator.h"
#include "B2UIFilterCheckBox.h"
#include "B2UIDocHelper.h"
#include "BladeIIGameImpl.h"
#include "Global.h"
#include "B2GameDefine.h"


void UB2AutoSellEther::CacheAssets()
{
	GET_SLOT(UB2Button, X_BTN_Close);
	GET_SLOT(UTextBlock, X_TB_EtherSellPriceCount);
	if (X_TB_EtherSellPriceCount.IsValid())
	{
		X_TB_EtherSellPriceCount->SetText(FText::AsNumber(0));
	}
	GET_SLOT(UB2Button, X_BTN_Sell);
	if (X_BTN_Sell.IsValid())
	{
		X_BTN_Sell->SetIsEnabled(false);
	}

	GET_SLOT(UTextBlock, X_TB_EtherSellPriceNumber);
	if (X_TB_EtherSellPriceNumber.IsValid())
	{
		X_TB_EtherSellPriceNumber->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Receive"))));
	}
	GET_SLOT(UTextBlock, X_TB_Sell);
	if (X_TB_Sell.IsValid())
	{
		X_TB_Sell->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Sell"))));
	}

	GET_SLOT(UTextBlock, X_TB_Title);
	if (X_TB_Title.IsValid())
	{
		X_TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_SellAll"))));
	}

	GET_SLOT(UTextBlock, X_TB_Description);
	if (X_TB_Description.IsValid())
	{
		X_TB_Description->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AetherSellAllNote"))));
	}

	GET_SLOT(UTextBlock, X_TB_Tier);
	if (X_TB_Tier.IsValid())
	{
		X_TB_Tier->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Tier"))));
	}

	GET_SLOT(UTextBlock, X_TB_SelectNumber);
	if (X_TB_SelectNumber.IsValid())
	{
		X_TB_SelectNumber->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_CountUnit"))));
	}

	GET_SLOT(UTextBlock, X_TB_Select);
	if (X_TB_Select.IsValid())
	{
		X_TB_Select->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Select"))));
	}
	GET_SLOT(UTextBlock, X_TB_SelectCount);

	GET_SLOT(UB2Button, X_BTN_Reset);
	GET_SLOT(UTextBlock, X_TB_Reset);
	if (X_TB_Reset.IsValid())
	{
		X_TB_Reset->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Reset"))));
	}

	for (int32 i = 1; i <= ETHER_MAX_STAR; ++i)
	{
		Filter_Grade.Add(i);
		Filter_Grade[i] = GetCachedWidget<UB2UIFilterCheckBox>(FName(*FString::Printf(TEXT("UIP_EtherGrade_%d"), i)));
		if (Filter_Grade[i].IsValid())
		{
			Filter_Grade[i]->Init();
			Filter_Grade[i]->SetLabel(FText::FromString(FString::FormatAsNumber(i)));
			AllFilters.Add(Filter_Grade[i]);
		}

	}

	Filter_Tier.Add(b2network::B2AetherTierType::SEAL);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, Filter_Tier[b2network::B2AetherTierType::SEAL], UIP_Tier1);
	if (Filter_Tier[b2network::B2AetherTierType::SEAL].IsValid())
	{
		Filter_Tier[b2network::B2AetherTierType::SEAL]->Init();
		AllFilters.Add(Filter_Tier[b2network::B2AetherTierType::SEAL]);
	}

	Filter_Tier.Add(b2network::B2AetherTierType::ANTI_DEVIL);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, Filter_Tier[b2network::B2AetherTierType::ANTI_DEVIL], UIP_Tier2);
	if (Filter_Tier[b2network::B2AetherTierType::ANTI_DEVIL].IsValid())
	{
		Filter_Tier[b2network::B2AetherTierType::ANTI_DEVIL]->Init();
		AllFilters.Add(Filter_Tier[b2network::B2AetherTierType::ANTI_DEVIL]);
	}

	Filter_Tier.Add(b2network::B2AetherTierType::EXTERMINATE_DEVIL);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, Filter_Tier[b2network::B2AetherTierType::EXTERMINATE_DEVIL], UIP_Tier3);
	if (Filter_Tier[b2network::B2AetherTierType::EXTERMINATE_DEVIL].IsValid())
	{
		Filter_Tier[b2network::B2AetherTierType::EXTERMINATE_DEVIL]->Init();
		AllFilters.Add(Filter_Tier[b2network::B2AetherTierType::EXTERMINATE_DEVIL]);
	}

	Filter_Tier.Add(b2network::B2AetherTierType::ETERNAL_LIFE);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, Filter_Tier[b2network::B2AetherTierType::ETERNAL_LIFE], UIP_Tier4);
	if (Filter_Tier[b2network::B2AetherTierType::ETERNAL_LIFE].IsValid())
	{
		Filter_Tier[b2network::B2AetherTierType::ETERNAL_LIFE]->Init();
		AllFilters.Add(Filter_Tier[b2network::B2AetherTierType::ETERNAL_LIFE]);
	}

	if (Filter_Tier[b2network::B2AetherTierType::SEAL].IsValid())
		Filter_Tier[b2network::B2AetherTierType::SEAL]->SetLabel(B2EtherManager::GetInstance().GetEtherTierName(b2network::B2AetherTierType::SEAL));

	if (Filter_Tier[b2network::B2AetherTierType::ANTI_DEVIL].IsValid())
		Filter_Tier[b2network::B2AetherTierType::ANTI_DEVIL]->SetLabel(B2EtherManager::GetInstance().GetEtherTierName(b2network::B2AetherTierType::ANTI_DEVIL));

	if (Filter_Tier[b2network::B2AetherTierType::EXTERMINATE_DEVIL].IsValid())
		Filter_Tier[b2network::B2AetherTierType::EXTERMINATE_DEVIL]->SetLabel(B2EtherManager::GetInstance().GetEtherTierName(b2network::B2AetherTierType::EXTERMINATE_DEVIL));

	if (Filter_Tier[b2network::B2AetherTierType::ETERNAL_LIFE].IsValid())
		Filter_Tier[b2network::B2AetherTierType::ETERNAL_LIFE]->SetLabel(B2EtherManager::GetInstance().GetEtherTierName(b2network::B2AetherTierType::ETERNAL_LIFE));

	GET_SLOT(UB2UIFilterCheckBox, UIP_Enhance);
	if (UIP_Enhance.IsValid())
	{
		UIP_Enhance->Init();
		UIP_Enhance->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Enhancement"))));
		AllFilters.Add(UIP_Enhance);
	}


	for (auto &Element : AllFilters)
	{
		if (Element.IsValid())
			Element->OnCheckStateChanged.AddDynamic(this, &UB2AutoSellEther::OnClickCheckBox);
	}
}

void UB2AutoSellEther::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2AutoSellEther::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Reset, &UB2AutoSellEther::OnClickReset);
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Sell, &UB2AutoSellEther::OnClickSell);

	//Grade Wraning Popup
	TArray<TPair<FText, TWeakObjectPtr<UB2UIFilterCheckBox>>> WarningFilters;

	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Grade)
	{
		if (Elem.Key < WarningPopupStartGrade)
			continue;

		if (Elem.Value.IsValid())
		{
			int32 Grade = Elem.Key;
			WarningFilters.Add(TPair<FText, TWeakObjectPtr<UB2UIFilterCheckBox>>(GetEtherGradeText(Grade), Elem.Value));
		}
	}
	//Tier Wraning Popup
	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Tier)
	{
		if (Elem.Key < WarningPopupTierGrade)
			continue;

		if (Elem.Value.IsValid())
		{
			int32 Tier = Elem.Key;
			WarningFilters.Add(TPair<FText, TWeakObjectPtr<UB2UIFilterCheckBox>>(B2EtherManager::GetInstance().GetEtherTierName(Tier), Elem.Value));
		}
	}

	for (auto& Elem : WarningFilters)
	{
		OnPreSelectFilter OnPreSelect;
		FText PopUpTextParam = Elem.Key;
		OnPreSelect.BindLambda(
			[PopUpTextParam, this](UB2UIFilterCheckBox* FilterCheckBox) {
			if (FilterCheckBox && FilterCheckBox->IsChecked())
				return;

			this->OpenCheckWarningPopUp(PopUpTextParam, FilterCheckBox);
		});

		Elem.Value->SetPreFilterCheck(OnPreSelect);
	}
}
//LobbyInven_Grade

void UB2AutoSellEther::DestroySelf(class UB2UIManager* InUIManager)
{
	if (SellEtherEventID != -1)
	{
		DeliverySellEtherClass<FB2SellEtherPtr>::GetInstance().Unsubscribe(SellEtherEventID);
	}

	if (SellEtherErrorEventID != -1)
	{
		DeliverySellEtherErrorClass<>::GetInstance().Unsubscribe(SellEtherErrorEventID);
	}
}

void UB2AutoSellEther::Init()
{
	Super::Init();
	SellEtherEventID = -1;
	SellEtherErrorEventID = -1;
	RequestTime = 0;
	ResponseTime = 0;

	LoadFilterInfo();
}

void UB2AutoSellEther::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::EtherAutoSell);
}

void UB2AutoSellEther::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2AutoSellEther::OnClickSell()
{
	EtherSellPriceAmount = 0;

	TArray<int32> StarFilter;
	TArray<int32> TierFilter;
	bool IsEnhance;
	GetFiliterList(StarFilter, TierFilter, IsEnhance);

	FilteredEtherIDs.Empty();
	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetFilterEtherID(FilteredEtherIDs, StarFilter, TierFilter, IsEnhance);

	SaveFilterInfo();

	StartTimerRequestSellEther();
	UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);

	SellEtherEventID = DeliverySellEtherClass<FB2SellEtherPtr>::GetInstance().Subscribe([this](const FB2SellEtherPtr EtherPtr)
	{
		EtherSellPriceAmount += this->SendEtherSellPrice;

		if (this->GetSendEther())
		{
			if (GetWorld())
			{
				ResponseTime = GetWorld()->GetTimeSeconds();
			}
			StartTimerRequestSellEther();
		}
		else
		{
			this->EndLoopRequest();
		}
	});

	SellEtherErrorEventID = DeliverySellEtherErrorClass<>::GetInstance().Subscribe([this]()
	{
		this->EndLoopRequest();

		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Network_Retry")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
			);
		}
	});
}

void UB2AutoSellEther::OnClickReset()
{
	for (auto &Element : AllFilters)
	{
		if (Element.IsValid())
			Element->SetIsChecked(false);
	}
}

void UB2AutoSellEther::OnClickCheckBox(bool bCheck)
{
	EtherSellPriceAmount = 0;
	int32 EtherSellCount = 0;

	TArray<int32> StarFilter;
	TArray<int32> TierFilter;
	TArray<int64> EtherIDs;
	bool IsEnhance = false;
	GetFiliterList(StarFilter, TierFilter, IsEnhance);
	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetFilterEtherID(EtherIDs, StarFilter, TierFilter, IsEnhance);

	EtherSellCount = EtherIDs.Num();
	for (int64 ElemID : EtherIDs)
	{
		EtherSellPriceAmount += B2EtherManager::GetInstance().GetEtherSellPrice(ElemID);
	}

	if (X_TB_SelectCount.IsValid())
	{
		X_TB_SelectCount->SetText(FText::AsNumber(EtherSellCount));
	}

	if (X_TB_EtherSellPriceCount.IsValid())
	{
		X_TB_EtherSellPriceCount->SetText(FText::AsNumber(EtherSellPriceAmount));
	}

	if (X_BTN_Sell.IsValid())
	{
		if (EtherSellPriceAmount > 0)
		{
			X_BTN_Sell->SetIsEnabled(true);
		}
		else
		{
			X_BTN_Sell->SetIsEnabled(false);
		}
	}
}

void UB2AutoSellEther::GetFiliterList(TArray<int32> &OutStartFilter, TArray<int32> &OutTierFilter, bool& IsEnhance)
{
	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Grade)
	{
		if (Elem.Value->IsChecked())
		{
			OutStartFilter.Add(Elem.Key); // Star 는 1부터 시작.
		}
	}

	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Tier)
	{
		if (Elem.Value->IsChecked())
		{
			OutTierFilter.Add(Elem.Key); // Star 는 1부터 시작.
		}
	}

	IsEnhance = UIP_Enhance.IsValid() ? UIP_Enhance->IsChecked() : false;
}

void UB2AutoSellEther::GetSendEtherList(TArray<int64>& OutSendIDs)
{
	int MaxCount = 10;
	if (FilteredEtherIDs.Num() < MaxCount)
	{
		MaxCount = FilteredEtherIDs.Num();
	}

	SendEtherSellPrice = 0;

	for (int i = 0; i < MaxCount; i++)
	{
		int64 ID = FilteredEtherIDs.Pop();
		SendEtherSellPrice += B2EtherManager::GetInstance().GetEtherSellPrice(ID);
		OutSendIDs.Add(ID);
	}
}

void UB2AutoSellEther::EndLoopRequest()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);

	RefreshScrollUIClass<>::GetInstance().Signal();

	ResultAllSellEtherItemClass<int32>::GetInstance().Signal(EtherSellPriceAmount);
	
	OnClickClose();
}

void UB2AutoSellEther::StartTimerRequestSellEther()
{
	float DelayTime = GetBackOffTime(RequestTime, ResponseTime);
	if (DelayTime <= 0)
	{
		EndTimerRequestSellEther();
	}
	else
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(LoopRequestSellEther, this, &UB2AutoSellEther::EndTimerRequestSellEther, DelayTime, false);
		}
	}
}

void UB2AutoSellEther::EndTimerRequestSellEther()
{
	auto MailWorld = GetWorld();
	if (MailWorld)
	{
		MailWorld->GetTimerManager().ClearTimer(LoopRequestSellEther);
		RequestTime = MailWorld->GetTimeSeconds();
	}

	TArray<int64> SendEtherIDs;
	this->GetSendEtherList(SendEtherIDs);
	data_trader::Retailer::GetInstance().RequestSellAethers(SendEtherIDs);
}

FText UB2AutoSellEther::GetEtherGradeText(int32 Grade)
{
	return FText::Format(FText::FromString(TEXT("{0} {1}")), Grade, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_Grade"))));
}

void UB2AutoSellEther::OpenCheckWarningPopUp(FText Parameter, UB2UIFilterCheckBox* FilterCheckBox)
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
		FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_WarningEtherFilter"))),
			Parameter, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("BattleStageInfo_AutoSell")))),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([FilterCheckBox]() {
		if (FilterCheckBox)
			FilterCheckBox->SetIsChecked(true);
	}));
}


void UB2AutoSellEther::SaveFilterInfo()
{
	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Grade)
	{
		if (Elem.Value.IsValid())
		{
			int32 Grade = Elem.Key;
			GConfig->SetBool(TEXT("EtherSell"), *FString::Printf(TEXT("Grade%d"), Grade), Elem.Value->IsChecked(), GB2UserSavedStateIni);
		}
	}

	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Tier)
	{
		if (Elem.Value.IsValid())
		{
			int32 Tier = Elem.Key;
			GConfig->SetBool(TEXT("EtherSell"), *FString::Printf(TEXT("Tier%d"), Tier), Elem.Value->IsChecked(), GB2UserSavedStateIni);
		}
	}

	if (UIP_Enhance.IsValid())
	{
		GConfig->SetBool(TEXT("EtherSell"), *FString::Printf(TEXT("Enhance")), UIP_Enhance->IsChecked(), GB2UserSavedStateIni);
	}

	GConfig->Flush(false, GB2UserSavedStateIni);
}

void UB2AutoSellEther::LoadFilterInfo()
{
	bool CheckedValue = false;

	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Grade)
	{
		if (Elem.Value.IsValid())
		{
			CheckedValue = false;
			int32 Grade = Elem.Key;
			GConfig->GetBool(TEXT("EtherSell"), *FString::Printf(TEXT("Grade%d"), Grade), CheckedValue, GB2UserSavedStateIni);
			Elem.Value->SetIsChecked(CheckedValue);
		}
	}

	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Tier)
	{
		if (Elem.Value.IsValid())
		{
			CheckedValue = false;
			int32 Tier = Elem.Key;
			GConfig->GetBool(TEXT("EtherSell"), *FString::Printf(TEXT("Tier%d"), Tier), CheckedValue, GB2UserSavedStateIni);
			Elem.Value->SetIsChecked(CheckedValue);
		}
	}

	if (UIP_Enhance.IsValid())
	{
		CheckedValue = false;
		GConfig->GetBool(TEXT("EtherSell"), *FString::Printf(TEXT("Enhance")), CheckedValue, GB2UserSavedStateIni);
		UIP_Enhance->SetIsChecked(CheckedValue);
	}
}

bool UB2AutoSellEther::GetSendEther()
{
	bool Result = false;

	if (FilteredEtherIDs.Num() > 0)
	{
		Result = true;
	}

	return Result;
}

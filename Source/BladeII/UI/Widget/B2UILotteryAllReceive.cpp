// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UILotteryAllReceive.h"
#include "B2UILotteryTab.h"
#include "B2UIFilterCheckBox.h"
#include "B2UIManager.h"
#include "B2UIMailContent.h"


void UB2UILotteryAllReceive::Init()
{
	Super::Init();

	OnClickMax();

	OnClickLotteryTab Delegate;
	Delegate.BindUObject(this, &UB2UILotteryAllReceive::SetLotteryTabs);
	for (const TPair<EPCClass, TWeakObjectPtr<UB2UILotteryTab>>& Elem : LotteryTabs)
	{
		if (Elem.Value.IsValid())
		{
			Elem.Value->Init();
			Elem.Value->SetOnClickLotteryTabDelegate(Delegate);
		}
	}

	if (UIP_WeaponCheckBox.IsValid())
	{
		UIP_WeaponCheckBox->Init();
		UIP_WeaponCheckBox->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Weapon"))));
		UIP_WeaponCheckBox->SetIsChecked(true, true);
	}

	if (UIP_ProtectionCheckBox.IsValid())
	{
		UIP_ProtectionCheckBox->Init();
		UIP_ProtectionCheckBox->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Protection"))));
		UIP_ProtectionCheckBox->SetIsChecked(true, true);
	}

	if (UIP_AccessoryCheckBox.IsValid())
	{
		UIP_AccessoryCheckBox->Init();
		UIP_AccessoryCheckBox->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Accessory"))));
		UIP_AccessoryCheckBox->SetIsChecked(true, true);
	}

	if (UIP_WeaponEssenceCheckBox.IsValid())
	{
		UIP_WeaponEssenceCheckBox->Init();
		UIP_WeaponEssenceCheckBox->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickWeaponEssence"))));
		UIP_WeaponEssenceCheckBox->SetIsChecked(true, true);
	}

	if (UIP_ProtectionEssenceCheckBox.IsValid())
	{
		UIP_ProtectionEssenceCheckBox->Init();
		UIP_ProtectionEssenceCheckBox->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickArmorEssence"))));
		UIP_ProtectionEssenceCheckBox->SetIsChecked(true, true);
	}

	if (UIP_AccessoryEssenceCheckBox.IsValid())
	{
		UIP_AccessoryEssenceCheckBox->Init();
		UIP_AccessoryEssenceCheckBox->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickAccessoryEssence"))));
		UIP_AccessoryEssenceCheckBox->SetIsChecked(true, true);
	}
	
	SetLotteryTabs(EPCClass::EPC_End);
}

void UB2UILotteryAllReceive::DestroySelf(class UB2UIManager* InUIManager)
{
	for (const TPair<EPCClass, TWeakObjectPtr<UB2UILotteryTab>>& Elem : LotteryTabs)
	{
		if (Elem.Value.IsValid())
		{
			Elem.Value->DestroySelf(InUIManager);
		}
	}

	if (UIP_WeaponCheckBox.IsValid())
	{
		UIP_WeaponCheckBox->DestroySelf(InUIManager);
	}

	if (UIP_ProtectionCheckBox.IsValid())
	{
		UIP_ProtectionCheckBox->DestroySelf(InUIManager);
	}

	if (UIP_AccessoryCheckBox.IsValid())
	{
		UIP_AccessoryCheckBox->DestroySelf(InUIManager);
	}

	if (UIP_WeaponEssenceCheckBox.IsValid())
	{
		UIP_WeaponEssenceCheckBox->DestroySelf(InUIManager);
	}

	if (UIP_ProtectionEssenceCheckBox.IsValid())
	{
		UIP_ProtectionEssenceCheckBox->DestroySelf(InUIManager);
	}

	if (UIP_AccessoryEssenceCheckBox.IsValid())
	{
		UIP_AccessoryEssenceCheckBox->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UILotteryAllReceive::SetLotteryMailInfo(const TArray<TWeakObjectPtr<UB2UIMailContent>>& InMailInfo)
{
	for (TWeakObjectPtr<UB2UIMailContent> Elem : InMailInfo)
	{
		LotteryMailInfo MailInfo;
		MailInfo.MailID = Elem->GetMailId(); 
		MailInfo.LotteryIDs = Elem->GetItemRefId();
		MailInfo.GiftType = Elem->GetGiftType();

		LotteryMainInfos.Add(MailInfo);
	}
}

void UB2UILotteryAllReceive::CacheAssets()
{
	LotteryTabs.Add(EPCClass::EPC_Gladiator);
	GET_SLOT_BYNAME(UB2UILotteryTab, LotteryTabs[EPCClass::EPC_Gladiator], TAB_Gladiator);
	LotteryTabs[EPCClass::EPC_Gladiator]->SetTabPCClass(EPCClass::EPC_Gladiator);

	LotteryTabs.Add(EPCClass::EPC_Assassin);
	GET_SLOT_BYNAME(UB2UILotteryTab, LotteryTabs[EPCClass::EPC_Assassin], TAB_Assassin);
	LotteryTabs[EPCClass::EPC_Assassin]->SetTabPCClass(EPCClass::EPC_Assassin);

	LotteryTabs.Add(EPCClass::EPC_Wizard);
	GET_SLOT_BYNAME(UB2UILotteryTab, LotteryTabs[EPCClass::EPC_Wizard], TAB_Wizard);
	LotteryTabs[EPCClass::EPC_Wizard]->SetTabPCClass(EPCClass::EPC_Wizard);

	LotteryTabs.Add(EPCClass::EPC_Fighter);
	GET_SLOT_BYNAME(UB2UILotteryTab, LotteryTabs[EPCClass::EPC_Fighter], TAB_Fighter);
	LotteryTabs[EPCClass::EPC_Fighter]->SetTabPCClass(EPCClass::EPC_Fighter);

	GET_SLOT(UB2UIFilterCheckBox, UIP_WeaponCheckBox);
	GET_SLOT(UB2UIFilterCheckBox, UIP_ProtectionCheckBox);
	GET_SLOT(UB2UIFilterCheckBox, UIP_AccessoryCheckBox);
	GET_SLOT(UB2UIFilterCheckBox, UIP_WeaponEssenceCheckBox);
	GET_SLOT(UB2UIFilterCheckBox, UIP_ProtectionEssenceCheckBox);
	GET_SLOT(UB2UIFilterCheckBox, UIP_AccessoryEssenceCheckBox);
	GET_SLOT(UB2Button, BTN_OK);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_MailCountUp);
	GET_SLOT(UB2Button, BTN_MailCountDown);
	GET_SLOT(UB2Button, BTN_Max);
	GET_SLOT(USlider, S_MailCount);
	GET_SLOT(UTextBlock, TB_Count);
	GET_SLOT(UTextBlock, TB_Max);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Description);
	GET_SLOT(UTextBlock, TB_OK);
}

void UB2UILotteryAllReceive::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_OK, &UB2UILotteryAllReceive::OnClickOK);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UILotteryAllReceive::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_MailCountUp, &UB2UILotteryAllReceive::OnClickMailCountUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_MailCountDown, &UB2UILotteryAllReceive::OnClickMailCountDown);
	BIND_CLICK_FUNC_TO_BTN(BTN_Max, &UB2UILotteryAllReceive::OnClickMax);

	if (S_MailCount.IsValid())
	{
		S_MailCount->OnValueChanged.AddDynamic(this, &UB2UILotteryAllReceive::OnChangedMailCount);
	}
}

void UB2UILotteryAllReceive::UpdateStaticText()
{
	if (TB_Max.IsValid())
	{
		TB_Max->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Maximum"))));
	}

	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickSetting"))));
	}

	if (TB_Description.IsValid())
	{
		TB_Description->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickDesc"))));
	}
	
	if (TB_OK.IsValid())
	{
		TB_OK->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickGetReward"))));
	}
}

void UB2UILotteryAllReceive::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UILotteryAllReceive::OnClickOK()
{
	if (SelectEPCClass == EPCClass::EPC_End)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MailLottery_QuickErrorCharType")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);
		return;
	}

	TArray<ELotteryFilterType> CheckedFiltering;
	SelectMailIDs.Empty();
	
	GetCheckedFiltering(CheckedFiltering);

	if (CheckedFiltering.Num() <= 0)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MailLottery_QuickErrorItemType")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);
		return;
	}
	
	if (ReceiveCount == 0)
	{
		return;
	}

	for (LotteryMailInfo MailInfo : LotteryMainInfos)
	{
		if (SelectMailIDs.Num() >= ReceiveCount)
		{
			break;
		}

		if (MailInfo.GiftType == MailGiftType::LotteryTicket)
		{
			for (int i = 0; i < LotteryTicketFilteringInfos.Num(); i++)
			{
				if (LotteryTicketFilteringInfos[i].LotteryIDs.Contains(MailInfo.LotteryIDs) && CheckedFiltering.Contains(LotteryTicketFilteringInfos[i].FilterType))
				{
					SelectMailIDs.Add(MailInfo.MailID);
				}
			}
		}
		else if (MailInfo.GiftType == MailGiftType::Fixed_Grade_Lottey)
		{
			for (int j = 0; j < FixedGradeLotteyFilteringInfos.Num(); j++)
			{
				if (FixedGradeLotteyFilteringInfos[j].LotteryIDs.Contains(MailInfo.LotteryIDs) && CheckedFiltering.Contains(FixedGradeLotteyFilteringInfos[j].FilterType))
				{
					SelectMailIDs.Add(MailInfo.MailID);
				}
			}
		}
	}

	SetLotteryAllFilteringInfoClass<TArray<int64>&, EPCClass, int32>::GetInstance().Signal(SelectMailIDs, SelectEPCClass, 0);
	OnClickClose();
}

void UB2UILotteryAllReceive::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::LotteryAllReceive);
}

void UB2UILotteryAllReceive::OnChangedMailCount(float ChangeValue)
{
	ReceiveCount = FMath::FloorToInt(ChangeValue * MaxCountLotteryMail);

	if (ReceiveCount < 1)
	{
		ReceiveCount = 1;
	}

	SetTBCount(ReceiveCount);
}

void UB2UILotteryAllReceive::OnClickMailCountDown()
{
	ReceiveCount -= 1;
	if (ReceiveCount < 1)
	{
		ReceiveCount = 1;
	}

	SetValueMailCountSlider(ReceiveCount);
	SetTBCount(ReceiveCount);
}

void UB2UILotteryAllReceive::OnClickMailCountUp()
{
	ReceiveCount += 1;
	if (ReceiveCount > MaxCountLotteryMail)
	{
		ReceiveCount = MaxCountLotteryMail;
	}

	SetValueMailCountSlider(ReceiveCount);
	SetTBCount(ReceiveCount);
}

void UB2UILotteryAllReceive::OnClickMax()
{
	ReceiveCount = MaxCountLotteryMail;
	SetValueMailCountSlider(ReceiveCount);
	SetTBCount(ReceiveCount);
}

void UB2UILotteryAllReceive::SetLotteryTabs(EPCClass InPCClass)
{
	SelectEPCClass = InPCClass;

	for (const TPair<EPCClass, TWeakObjectPtr<UB2UILotteryTab>>& Elem : LotteryTabs)
	{
		if (Elem.Value.IsValid())
		{
			Elem.Value->SetTabVisible(false);
		}
	}

	if (LotteryTabs.Contains(SelectEPCClass) && LotteryTabs[SelectEPCClass].IsValid())
	{
		LotteryTabs[SelectEPCClass]->SetTabVisible(true);
	}
}

void UB2UILotteryAllReceive::SetTBCount(int32 InValue)
{
	if (TB_Count.IsValid())
	{
		FText CountFormat = FText::FromString(TEXT("{0} / {1}"));
		TB_Count->SetText(FText::Format(CountFormat, FText::AsNumber(InValue), FText::AsNumber(MaxCountLotteryMail)));
	}
}

void UB2UILotteryAllReceive::SetValueMailCountSlider(int32 InValue)
{
	if(S_MailCount.IsValid())
	{
		S_MailCount->SetValue(static_cast<float>(InValue) / static_cast<float>(MaxCountLotteryMail));
	}
}

void UB2UILotteryAllReceive::GetCheckedFiltering(TArray<ELotteryFilterType>& OutCheckedFiltering)
{
	if (UIP_WeaponCheckBox.IsValid() && UIP_WeaponCheckBox->IsChecked())
	{
		OutCheckedFiltering.Add(ELotteryFilterType::Weapon);

		switch (SelectEPCClass)
		{
		case EPCClass::EPC_Gladiator:
			OutCheckedFiltering.Add(ELotteryFilterType::GladiatorWeapon);
			break;
		case EPCClass::EPC_Assassin:
			OutCheckedFiltering.Add(ELotteryFilterType::AssassinWeapon);
			break;
		case EPCClass::EPC_Wizard:
			OutCheckedFiltering.Add(ELotteryFilterType::WizardWeapon);
			break;
		case EPCClass::EPC_Fighter:
			OutCheckedFiltering.Add(ELotteryFilterType::FighterWeapon);
			break;
		}
	}

	if (UIP_ProtectionCheckBox.IsValid() && UIP_ProtectionCheckBox->IsChecked())
	{
		OutCheckedFiltering.Add(ELotteryFilterType::Protection);

		switch (SelectEPCClass)
		{
		case EPCClass::EPC_Gladiator:
			OutCheckedFiltering.Add(ELotteryFilterType::GladiatorProtection);
			break;
		case EPCClass::EPC_Assassin:
			OutCheckedFiltering.Add(ELotteryFilterType::AssassinProtection);
			break;
		case EPCClass::EPC_Wizard:
			OutCheckedFiltering.Add(ELotteryFilterType::WizardProtection);
			break;
		case EPCClass::EPC_Fighter:
			OutCheckedFiltering.Add(ELotteryFilterType::FighterProtection);
			break;
		}
	}

	if (UIP_AccessoryCheckBox.IsValid() && UIP_AccessoryCheckBox->IsChecked())
	{
		OutCheckedFiltering.Add(ELotteryFilterType::Accessory);

		switch (SelectEPCClass)
		{
		case EPCClass::EPC_Gladiator:
			OutCheckedFiltering.Add(ELotteryFilterType::GladiatorAccessory);
			break;
		case EPCClass::EPC_Assassin:
			OutCheckedFiltering.Add(ELotteryFilterType::AssassinAccessory);
			break;
		case EPCClass::EPC_Wizard:
			OutCheckedFiltering.Add(ELotteryFilterType::WizardAccessory);
			break;
		case EPCClass::EPC_Fighter:
			OutCheckedFiltering.Add(ELotteryFilterType::FighterAccessory);
			break;
		}
	}

	if (UIP_WeaponEssenceCheckBox.IsValid() && UIP_WeaponEssenceCheckBox->IsChecked())
	{
		OutCheckedFiltering.Add(ELotteryFilterType::WeaponEssence);
		OutCheckedFiltering.Add(ELotteryFilterType::FixedWeaponEssence);
	}

	if (UIP_ProtectionEssenceCheckBox.IsValid() && UIP_ProtectionEssenceCheckBox->IsChecked())
	{
		OutCheckedFiltering.Add(ELotteryFilterType::ProtectionEssence);
		OutCheckedFiltering.Add(ELotteryFilterType::FixedProtectionEssence);
	}

	if (UIP_AccessoryEssenceCheckBox.IsValid() && UIP_AccessoryEssenceCheckBox->IsChecked())
	{
		OutCheckedFiltering.Add(ELotteryFilterType::AccessoryEssence);
		OutCheckedFiltering.Add(ELotteryFilterType::FixedAccessoryEssence);
	}
}

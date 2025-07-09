// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIUpgradeStoneAllReceive.h"
#include "B2UIManager.h"
#include "B2UIMailContent.h"
#include "B2UISelectiveLotteryPopUp.h"
#include "BladeIIUtil.h"
#include "B2UIMail.h"

UB2UIUpgradeStoneAllReceive::UB2UIUpgradeStoneAllReceive(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxCountUpgradeStoneMail = 100;
	RefID = -1;
}

void UB2UIUpgradeStoneAllReceive::Init()
{
	Super::Init();

	for (TWeakObjectPtr<UB2UISelectiveLotteryItemSlot> Elem : Slots)
	{
		if (Elem.IsValid())
		{
			Elem->Init();
		}
	}

	if (BTN_OK.IsValid())
	{
		BTN_OK->SetIsEnabled(false);
	}

	// TODO 네트워크로 셋팅된 값을 이곳에서 셋팅한다.
	ReceiveCount = 1;
	SetValueMailCountSlider(ReceiveCount);
	SetTBCount(ReceiveCount);
}

void UB2UIUpgradeStoneAllReceive::SetMax()
{
	
}

void UB2UIUpgradeStoneAllReceive::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (TWeakObjectPtr<UB2UISelectiveLotteryItemSlot> Elem : Slots)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}
}

void UB2UIUpgradeStoneAllReceive::SetUpgradeStoneMailInfo(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailInfo)
{
	MailIDs.Empty();

	TArray<int32> UpgradeStoneRefIds;
	UpgradeStoneRefIds.Empty();

	mailbox = nullptr;

	for (TWeakObjectPtr<UB2UIMailContent> Elem : InMailInfo)
	{
		if (mailbox == nullptr) mailbox = Elem->GetMailBox();

		if (Elem->GetGiftType() == MailGiftType::Selective_Item_Lottery &&
			//Elem->GetSelectItemRefIDs()[0] != 8000100
			Elem->GetItemRefId() != 8
			)								// 하드 코딩 고급 승급석 예외 처리
		{
			MailIDs.Add(Elem->GetMailId());

			if (UpgradeStoneRefIds.Num() <= 0)
			{
				UpgradeStoneRefIds = Elem->GetSelectItemRefIDs();
			}
		}
	}

	if (mailbox)
	{
		UE_LOG(LogBladeII, Log, TEXT("mailbox count %d"), mailbox->upgradestone_mail_totalcount);
		ReceiveCount = mailbox->upgradestone_mail_totalcount;
		MaxCountUpgradeStoneMail = mailbox->upgradestone_mail_totalcount;
		SetValueMailCountSlider(ReceiveCount);
		SetTBCount(ReceiveCount);
		mailbox->IsRare = false;
	}

	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].IsValid())
		{
			if (UpgradeStoneRefIds.IsValidIndex(i))
			{
				Slots[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Slots[i]->SetItemData(UpgradeStoneRefIds[i]);
			}
			else
			{
				Slots[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UB2UIUpgradeStoneAllReceive::SetRareUpgradeStoneMailInfo(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailInfo)
{
	MailIDs.Empty();

	TArray<int32> UpgradeStoneRefIds;
	UpgradeStoneRefIds.Empty();

	mailbox = nullptr;

	for (TWeakObjectPtr<UB2UIMailContent> Elem : InMailInfo)
	{
		if(mailbox == nullptr) mailbox = Elem->GetMailBox();

		if (Elem->GetGiftType() == MailGiftType::Selective_Item_Lottery &&
			Elem->GetItemRefId() == 8										// 하드코딩 고급 승급석 예외 처리를 위함.
			)
		{
			MailIDs.Add(Elem->GetMailId());

			if (UpgradeStoneRefIds.Num() <= 0)
			{
				UpgradeStoneRefIds = Elem->GetSelectItemRefIDs();
			}
		}
	}

	if (mailbox)
	{
		UE_LOG(LogBladeII, Log, TEXT("mailbox rare count %d"), mailbox->rare_upgradestone_mail_totalcount);
		ReceiveCount = mailbox->rare_upgradestone_mail_totalcount;
		MaxCountUpgradeStoneMail = mailbox->rare_upgradestone_mail_totalcount;
		SetValueMailCountSlider(ReceiveCount);
		SetTBCount(ReceiveCount);
		mailbox->IsRare = true;
	}


	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].IsValid())
		{
			if (UpgradeStoneRefIds.IsValidIndex(i))
			{
				Slots[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Slots[i]->SetItemData(UpgradeStoneRefIds[i]);
			}
			else
			{
				Slots[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UB2UIUpgradeStoneAllReceive::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_OK);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_MailCountUp);
	GET_SLOT(UB2Button, BTN_MailCountDown);
	GET_SLOT(UB2Button, BTN_Max);
	GET_SLOT(USlider, S_MailCount);
	GET_SLOT(UTextBlock, TB_Count);
	GET_SLOT(UTextBlock, TB_Max);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_OK);
	GET_SLOT(UTextBlock, TB_SelectFullText);
	GET_SLOT(UTextBlock, TB_Guide);
	Slots.Empty();
	Slots.SetNum(3);
	GET_SLOT_BYNAME(UB2UISelectiveLotteryItemSlot, Slots[0], UIP_ItemSlot01);
	GET_SLOT_BYNAME(UB2UISelectiveLotteryItemSlot, Slots[1], UIP_ItemSlot02);
	GET_SLOT_BYNAME(UB2UISelectiveLotteryItemSlot, Slots[2], UIP_ItemSlot03);
}

void UB2UIUpgradeStoneAllReceive::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_OK, &UB2UIUpgradeStoneAllReceive::OnClickOK);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIUpgradeStoneAllReceive::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_MailCountUp, &UB2UIUpgradeStoneAllReceive::OnClickMailCountUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_MailCountDown, &UB2UIUpgradeStoneAllReceive::OnClickMailCountDown);
	BIND_CLICK_FUNC_TO_BTN(BTN_Max, &UB2UIUpgradeStoneAllReceive::OnClickMax);

	if (S_MailCount.IsValid())
	{
		S_MailCount->OnValueChanged.AddDynamic(this, &UB2UIUpgradeStoneAllReceive::OnChangedMailCount);
	}

	for (TWeakObjectPtr<UB2UISelectiveLotteryItemSlot>& Elme : Slots)
	{
		if (Elme.IsValid())
		{
			Elme->SetSelectDelegate(FOnSelectItemSlot::CreateLambda([this](int32 InValue) {this->SetSelectedItemRefId(InValue); }));
		}
	}
}

void UB2UIUpgradeStoneAllReceive::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickSetting"))));
	}

	if (TB_Max.IsValid())
	{
		TB_Max->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Maximum"))));
	}
	
	if (TB_OK.IsValid())
	{
		TB_OK->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickGetReward"))));
	}
	
	if (TB_SelectFullText.IsValid())
	{
		TB_SelectFullText->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_GetRewardAll_Guide"))));
	}

	if (TB_Guide.IsValid())
	{
		TB_Guide->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_GetRewardAll_GradeStoneGuide"))));
	}
}

void UB2UIUpgradeStoneAllReceive::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIUpgradeStoneAllReceive::OnClickOK()
{
	if (ReceiveCount > 0 && RefID != -1)
	{
		TArray<int64> SelectMailIDs;
		SelectMailIDs.Empty();

		int32 MaxMailCount = FMath::Min(ReceiveCount, MailIDs.Num());
		for (int32 i = 0; i< MaxMailCount; i++)
		{
			SelectMailIDs.Add(MailIDs[i]);
		}

		SetLotteryAllFilteringInfoClass<TArray<int64>&, EPCClass, int32>::GetInstance().Signal(SelectMailIDs, EPCClass::EPC_End, RefID);
		OnClickClose();
	}
}

void UB2UIUpgradeStoneAllReceive::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::UpgradeStoneAllReceive);
}

void UB2UIUpgradeStoneAllReceive::OnChangedMailCount(float ChangeValue)
{
	ReceiveCount = CheckReceiveCount(FMath::FloorToInt(ChangeValue * MaxCountUpgradeStoneMail));
	SetTBCount(ReceiveCount);
}

void UB2UIUpgradeStoneAllReceive::OnClickMailCountDown()
{
	ReceiveCount = CheckReceiveCount(ReceiveCount - 1);
	SetValueMailCountSlider(ReceiveCount);
	SetTBCount(ReceiveCount);

	if (mailbox)
	{
		if (mailbox->IsRare)
			mailbox->changed_rare_upgradestone = MaxCountUpgradeStoneMail - ReceiveCount;
		else
			mailbox->changed_upgradestone = MaxCountUpgradeStoneMail - ReceiveCount;
	}
}

void UB2UIUpgradeStoneAllReceive::OnClickMailCountUp()
{
	ReceiveCount = CheckReceiveCount(ReceiveCount + 1);
	SetValueMailCountSlider(ReceiveCount);
	SetTBCount(ReceiveCount);

	if (mailbox)
	{
		if (mailbox->IsRare)
			mailbox->changed_rare_upgradestone = MaxCountUpgradeStoneMail - ReceiveCount;
		else
			mailbox->changed_upgradestone = MaxCountUpgradeStoneMail - ReceiveCount;
	}
}

void UB2UIUpgradeStoneAllReceive::OnClickMax()
{
	ReceiveCount = MaxCountUpgradeStoneMail;
	SetValueMailCountSlider(ReceiveCount);
	SetTBCount(ReceiveCount);
}

void UB2UIUpgradeStoneAllReceive::SetTBCount(int32 InValue)
{
	if (TB_Count.IsValid())
	{
		FText CountFormat = FText::FromString(TEXT("{0} / {1}"));
		TB_Count->SetText(FText::Format(CountFormat, FText::AsNumber(InValue), FText::AsNumber(MaxCountUpgradeStoneMail)));
	}
}

int32 UB2UIUpgradeStoneAllReceive::CheckReceiveCount(int32 InValue)
{
	if (InValue < 1)
	{
		InValue = 1;
	}

	if (InValue > MaxCountUpgradeStoneMail)
	{
		InValue = MaxCountUpgradeStoneMail;
	}

	return InValue;
}

void UB2UIUpgradeStoneAllReceive::SetValueMailCountSlider(int32 InValue)
{
	if (S_MailCount.IsValid())
	{
		S_MailCount->SetValue(static_cast<float>(InValue) / static_cast<float>(MaxCountUpgradeStoneMail));
	}
}

void UB2UIUpgradeStoneAllReceive::SetSelectedItemRefId(int32 InValue)
{
	bool bSelected = false;
	RefID = InValue;

	for (TWeakObjectPtr<UB2UISelectiveLotteryItemSlot>& Elme : Slots)
	{
		if (Elme.IsValid())
		{
			bool bSelectedItem = Elme->GetItemRefId() == InValue;
			bSelected |= bSelectedItem;
			Elme->SetSelected(bSelectedItem);
		}
	}

	if (BTN_OK.IsValid())
	{
		BTN_OK->SetIsEnabled(bSelected);
	}
}

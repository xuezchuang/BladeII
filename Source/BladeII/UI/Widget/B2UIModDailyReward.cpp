// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIModDailyReward.h"

#include "B2ClientDataStore.h"
#include "B2UIManager.h"
#include "B2UIRewardSlot.h"



#include "BladeIIGameImpl.h"

UB2UIModDailyReward::UB2UIModDailyReward(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentSlotIndex = 0;
}

void UB2UIModDailyReward::Init()
{
	Super::Init();
}

void UB2UIModDailyReward::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_DailyRewardTitle);
	GET_SLOT(UTextBlock, TB_DailyRewardRequire);
	
	RewardSlotList.SetNumUninitialized(3);
	GET_SLOT_BYNAME(UB2UIRewardSlot, RewardSlotList[0], UIP_RewardSlot1);
	GET_SLOT_BYNAME(UB2UIRewardSlot, RewardSlotList[1], UIP_RewardSlot2);
	GET_SLOT_BYNAME(UB2UIRewardSlot, RewardSlotList[2], UIP_RewardSlot3);

	for (auto ThisSlot : RewardSlotList)
	{
		if (ThisSlot.IsValid())
		{
			ThisSlot->Init();
			ThisSlot->SetVisibility(ESlateVisibility::Hidden); // �ϴ� ���ܳ���
		}
	}

	GET_SLOT(UB2Button, BTN_Close);
}

void UB2UIModDailyReward::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIModDailyReward::OnClickButtonClose);
}

void UB2UIModDailyReward::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	for (auto& ThisSlot : RewardSlotList)
	{
		if (ThisSlot.IsValid())
		{
			ThisSlot->DestroySelf(InUIManager);
		}
	}

	RewardSlotList.Empty();
}

void UB2UIModDailyReward::UpdateStaticText()
{
	if (TB_DailyRewardTitle.IsValid())
	{
		TB_DailyRewardTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mod_DailyRewardTitle")));
	}
}

FReply UB2UIModDailyReward::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Reply = Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
	UB2UIManager::GetInstance()->CloseUI(UIFName::ModMatchRewardList_Day);

	return Reply;
}

FReply UB2UIModDailyReward::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	UB2UIManager::GetInstance()->CloseUI(UIFName::ModMatchRewardList_Day);

	return Reply;
}

void UB2UIModDailyReward::OpenDailyReward(int32 MatchType)
{
	const TArray<FModDailyReward>& DailyRewardList = BladeIIGameImpl::GetClientDataStore().GetModDailyRewardList();
	for (const auto& DailyReward : DailyRewardList)
	{
		if (DailyReward.Match_Type == MatchType)
		{
			// �Լ� ȣ���� ������� ������ ���ĵǾ� 3�������� ���. �߰� ��½� UI �� ������ �ʿ�
			// �߰� �����ϰ� ���� RewardType(ItemGiftType)�� B2UIRewardSlot�� ��ӹ��� UIWidget�� RewardDisplayInfo���� �߰� �����־�� �Ѵ�. - Icon, Loctext��
			
			SetRewardSlotFromServerData(MailGiftType::Gold, DailyReward);
			SetRewardSlotFromServerData(MailGiftType::Gem, DailyReward);
			SetRewardSlotFromServerData(MailGiftType::BladePoint, DailyReward);
			SetRewardSlotFromServerData(MailGiftType::Item, DailyReward);

			if (TB_DailyRewardRequire.IsValid())
			{
				TB_DailyRewardRequire->SetText(
					FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_DailyRewardRequire"))), FText::FromString(FString::FromInt(DailyReward.Require_Count))));
			}

			break;
		}
	}
}

void UB2UIModDailyReward::SetRewardSlotFromServerData(MailGiftType RewardType, const FModDailyReward& ServerRewardInfo)
{
	if (CurrentSlotIndex < RewardSlotList.Num())
	{
		TWeakObjectPtr<UB2UIRewardSlot>& RewardSlot = RewardSlotList[CurrentSlotIndex];
		if (RewardSlot.IsValid())
		{
			RewardSlot->InitRewardSlotTemplate(RewardType); // SlotTemplate[ Icon & Localize Text ] ����
			int32 Amount = 0;

			switch (RewardType)
			{
			case MailGiftType::Gem: Amount = ServerRewardInfo.Reward_Gem; break;
			case MailGiftType::Gold: Amount = ServerRewardInfo.Reward_Gold; break;
			case MailGiftType::BladePoint: Amount = ServerRewardInfo.Reward_BladePoint; break;
			case MailGiftType::Item:
				if (ServerRewardInfo.Reward_ItemId != 0)
				{
					RewardSlot->SetRewardSlotItem(ServerRewardInfo.Reward_ItemId);
					Amount = 1; // ����� �Ѱ��� ���������ϸ� Item ������ �������� �޾ƿ� �� �ְԵǸ� ���� �� ����.
				}
				break;
			}

			if (Amount > 0)
			{
				RewardSlot->SetRewardAmount(Amount);
				RewardSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				CurrentSlotIndex++;
			}
		}
	}
}

void UB2UIModDailyReward::OnClickButtonClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ModMatchRewardList_Day);
}

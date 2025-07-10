// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPQuestSlot.h"
#include "B2UIRewardEventSet.h"
#include "BladeIIUtil.h"
#include "QuestDataTable.h"
#include "B2UIQuestDialog.h"
#include "B2UIManager.h"
#include "QuestManager.h"



void UB2UIPQuestSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	for (QeustSlotRewardIcon& Icon : Icons)
	{
		Icon.Icon->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIPQuestSlot::SetInfo(b2network::B2mdMainQuestInfoPtr InQuestInfoPtr, EQuestSlotType InType)
{
	CurrentQuestType = InQuestInfoPtr->main_quest_type;
	SetQuestInfo(InQuestInfoPtr->id, InType);

	for (QeustSlotRewardIcon& Icon : Icons)
	{
		Icon.Panel->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int i = 0; i < InQuestInfoPtr->rewards.Num(); i++)
	{
		if (Icons.IsValidIndex(i))
		{
			Icons[i].Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Icons[i].Icon->UpdateRewardIcon(InQuestInfoPtr->rewards[i]);
		}
	}

	SetTypeUI(InType);
}

void UB2UIPQuestSlot::CacheAssets()
{
	GET_SLOT(UImage, IMG_Portrait);
	GET_SLOT(UPanelWidget, PW_RewardFrame);
	GET_SLOT(UB2RichTextBlock, RTB_QuestName);
	GET_SLOT(UB2RichTextBlock, RTB_QuestType);
	GET_SLOT(UB2RichTextBlock, RTB_QuestInfo);
	GET_SLOT(UPanelWidget, PW_Link);
	GET_SLOT(UTextBlock, TB_Link);
	GET_SLOT(UPanelWidget, PW_Reward);
	GET_SLOT(UTextBlock, TB_Reward);
	GET_SLOT(UPanelWidget, PW_Completed);
	GET_SLOT(UB2Image, IMG_Lock);
	GET_SLOT(UImage, BG_Disabled);
	GET_SLOT(UImage, BG_Active);
	GET_SLOT(UB2Button, BTN_Link);

	Icons.SetNum(3);
	GET_SLOT_BYNAME(UPanelWidget, Icons[0].Panel, PW_RewardIcon1);
	GET_SLOT_BYNAME(UB2UIRewardIcon, Icons[0].Icon, UIP_RewardIcon1);
	if (Icons[0].Icon.IsValid())
	{
		Icons[0].Icon->Init();
	}

	GET_SLOT_BYNAME(UPanelWidget, Icons[1].Panel, PW_RewardIcon2);
	GET_SLOT_BYNAME(UB2UIRewardIcon, Icons[1].Icon, UIP_RewardIcon2);
	if (Icons[1].Icon.IsValid())
	{
		Icons[1].Icon->Init();
	}

	GET_SLOT_BYNAME(UPanelWidget, Icons[2].Panel, PW_RewardIcon3);
	GET_SLOT_BYNAME(UB2UIRewardIcon, Icons[2].Icon, UIP_RewardIcon3);
	if (Icons[2].Icon.IsValid())
	{
		Icons[2].Icon->Init();
	}
}

void UB2UIPQuestSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Link, &UB2UIPQuestSlot::OnClickLinkButton);
}

void UB2UIPQuestSlot::UpdateStaticText()
{
	if (TB_Link.IsValid())
	{
		TB_Link->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_ShortOut"))));
	}

	if (TB_Reward.IsValid())
	{
		TB_Reward->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_GetReward"))));
	}
}

void UB2UIPQuestSlot::OnClickLinkButton()
{
	UB2UIQuestDialog* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog);
	if (QuestDialogUI != nullptr)
	{
		QuestDialogUI->OpenDialogBySlotIndex(CurrentQuestType);
	}
}

void UB2UIPQuestSlot::SetTypeUI(EQuestSlotType InType)
{
	switch (InType)
	{
	case EQuestSlotType::Reward:
		if (PW_RewardFrame.IsValid())
		{
			PW_RewardFrame->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (PW_Link.IsValid())
		{
			PW_Link->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_Lock.IsValid())
		{
			IMG_Lock->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (PW_Completed.IsValid())
		{
			PW_Completed->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BG_Disabled.IsValid())
		{
			BG_Disabled->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BG_Active.IsValid())
		{
			BG_Active->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (PW_Reward.IsValid())
		{
			PW_Reward->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (BTN_Link.IsValid())
		{
			BTN_Link->SetVisibility(ESlateVisibility::Visible);
		}

		break;
	case EQuestSlotType::Ongoing:
		if (PW_RewardFrame.IsValid())
		{
			PW_RewardFrame->SetVisibility(ESlateVisibility::Hidden);
		}

		if (PW_Link.IsValid())
		{
			PW_Link->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (IMG_Lock.IsValid())
		{
			IMG_Lock->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (PW_Completed.IsValid())
		{
			PW_Completed->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BG_Disabled.IsValid())
		{
			BG_Disabled->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BG_Active.IsValid())
		{
			BG_Active->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (PW_Reward.IsValid())
		{
			PW_Reward->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_Link.IsValid())
		{
			BTN_Link->SetVisibility(ESlateVisibility::Visible);
		}

		break;
	case EQuestSlotType::Complete:
		if (PW_RewardFrame.IsValid())
		{
			PW_RewardFrame->SetVisibility(ESlateVisibility::Hidden);
		}

		if (PW_Link.IsValid())
		{
			PW_Link->SetVisibility(ESlateVisibility::Hidden);
		}

		if (IMG_Lock.IsValid())
		{
			IMG_Lock->SetVisibility(ESlateVisibility::Hidden);
		}

		if (PW_Completed.IsValid())
		{
			PW_Completed->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (BG_Disabled.IsValid())
		{
			BG_Disabled->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (BG_Active.IsValid())
		{
			BG_Active->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (PW_Reward.IsValid())
		{
			PW_Reward->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_Link.IsValid())
		{
			BTN_Link->SetVisibility(ESlateVisibility::Collapsed);
		}

		break;
	case EQuestSlotType::Lock:
		if (PW_RewardFrame.IsValid())
		{
			PW_RewardFrame->SetVisibility(ESlateVisibility::Hidden);
		}

		if (PW_Link.IsValid())
		{
			PW_Link->SetVisibility(ESlateVisibility::Hidden);
		}

		if (IMG_Lock.IsValid())
		{
			IMG_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (PW_Completed.IsValid())
		{
			PW_Completed->SetVisibility(ESlateVisibility::Hidden);
		}
		
		if (BG_Disabled.IsValid())
		{
			BG_Disabled->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BG_Active.IsValid())
		{
			BG_Active->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (PW_Reward.IsValid())
		{
			PW_Reward->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_Link.IsValid())
		{
			BTN_Link->SetVisibility(ESlateVisibility::Collapsed);
		}

		break;
	default:
		break;
	}
}

void UB2UIPQuestSlot::SetQuestInfo(int32 QuestID, EQuestSlotType InType)
{
	UB2QuestInfoTable* QuestTable = StaticFindQuestTable();

	if (QuestTable != nullptr)
	{
		FQuestCommonInfo* QuestInfo = nullptr;
		QuestInfo = QuestTable->GetQuestMainInfo(QuestID);
		if (QuestInfo != nullptr)
		{
			FText OrgQuestText = BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestInfo->OutLine));

			TArray<FText> ParsedTextList;
			ParseTextFromSpecialChar(OrgQuestText, TEXT("/div"), ParsedTextList);

			if (RTB_QuestType.IsValid() && ParsedTextList.IsValidIndex(0))
			{
				RTB_QuestType->SetText(ParsedTextList[0]);
			}

			if (RTB_QuestInfo.IsValid() && ParsedTextList.IsValidIndex(1))
			{
				FText CompressedText = GetCompressedText(ParsedTextList[1], CompressTextLimitLength);
				if (InType == EQuestSlotType::Ongoing || InType == EQuestSlotType::Reward)
				{
					FText ProgressText = GetProgressText(QuestManager::GetInstance().GetQuest(CurrentQuestType));
					CompressedText = FText::Format(FText::FromString(TEXT("{0} {1}")), CompressedText, ProgressText);
				}

				RTB_QuestInfo->SetText(CompressedText);
			}

			if (RTB_QuestName.IsValid())
			{
				RTB_QuestName->SetText(BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestInfo->QuestName)));
			}

			if (IMG_Portrait.IsValid())
			{
				TSoftObjectPtr<UTexture2D>* FoundAssetPtr = NPCIcon.Find(QuestInfo->StartNPC);
				if (FoundAssetPtr)
				{
					if (UTexture2D* IconTexture = (*FoundAssetPtr).LoadSynchronous())
					{
						IMG_Portrait->SetBrushFromTexture(IconTexture);
					}
				}
			}
		}
	}
}


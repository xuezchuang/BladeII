// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIBlockList.h"
#include "B2UIManager.h"

#include "BladeIIGameImpl.h"
#include "B2UIBlockUser.h"
#include "B2Chatting.h"

void UB2UIBlockList::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_BlockUserList);
	GET_SLOT(UTextBlock, TB_BlockUserCount);
	GET_SLOT(UTextBlock, TB_BlockUserCoutValue);
	GET_SLOT(UTextBlock, TB_MaxBlockUserCount);
	GET_SLOT(UTextBlock, TB_NobodyBlockUser);

	GET_SLOT(UB2ScrollBox, SB_BlockScrollBox);
	GET_SLOT(UB2Button, BTN_Close);
}

void UB2UIBlockList::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIBlockList::Init()
{
	Super::Init();

	if (SB_BlockScrollBox.IsValid())
	{
		const auto& BlockUsers =  BladeIIGameImpl::GetChatStore().GetBlockUserList();

		for (const auto& UserInfo : BlockUsers)
		{
			auto* BlockUserItem = CreateWidget<UB2UIBlockUser>(GetWorld(), BlockUserSlotClass);
			BlockUserItem->Init();
			BlockUserItem->SetVisibility(ESlateVisibility::Visible);
			BlockUserItem->SetInfo(UserInfo.Value, UserInfo.Key);

			SB_BlockScrollBox->AddChild(BlockUserItem);
		}

		SetBlockUserText();
	}
}

void UB2UIBlockList::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIBlockList::SetBlockUserText()
{
	if (TB_BlockUserList.IsValid())
	{
		TB_BlockUserList->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BlockUserList")));
	}

	if (TB_BlockUserCount.IsValid())
	{
		TB_BlockUserCount->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BlockUserCount")));
	}

	if (TB_BlockUserCoutValue.IsValid())
	{
		TB_BlockUserCoutValue->SetText(FText::AsNumber(BladeIIGameImpl::GetChatStore().GetCurrentUserBlockCount()));
	}

	if (TB_MaxBlockUserCount.IsValid())
	{
		TB_MaxBlockUserCount->SetText(FText::AsNumber(BladeIIGameImpl::GetChatStore().GetMaxUserBlockCount()));
	}

	if (TB_NobodyBlockUser.IsValid() && SB_BlockScrollBox.IsValid())
	{
		TB_NobodyBlockUser->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EmptyBlockList")));
		TB_NobodyBlockUser->SetVisibility(BladeIIGameImpl::GetChatStore().GetCurrentUserBlockCount() > 0 ?
			ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}
}

void UB2UIBlockList::RemoveBlockUser(int64 InAccountId)
{
	if (SB_BlockScrollBox.IsValid())
	{
		for (int32 i = 0; i < SB_BlockScrollBox->GetChildrenCount(); ++i)
		{
			const auto* BlockUser = Cast<UB2UIBlockUser>(SB_BlockScrollBox->GetChildAt(i));

			if (BlockUser && BlockUser->GetAccountId())
			{
				SB_BlockScrollBox->RemoveChildAt(i);
				SetBlockUserText();
				return;
			}
		}
	}
}

void UB2UIBlockList::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::BlockUserList);
}



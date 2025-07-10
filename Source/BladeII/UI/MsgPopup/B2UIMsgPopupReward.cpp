#include "B2UIMsgPopupReward.h"
#include "B2UIManager.h"
#include "B2UIQuestDialog.h"
#include "B2ClientDataStore.h"
#include "B2UIRewardEventSet.h"




#include "BladeIIGameImpl.h"


#define MAX_SHOW_REWARDICON_COUNT	4


void UB2UIMsgPopupReward::Init()
{
	Super::Init();

	bOpenNickNamePopup = false;
	bIsJackpotReward = false;
}

void UB2UIMsgPopupReward::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Background);
	GET_SLOT(UButton, BTN_Confirm);
	GET_SLOT(UTextBlock, TB_Confirm);
	GET_SLOT(UButton, BTN_Close);
	GET_SLOT(UTextBlock, TB_RewardTitle);
	GET_SLOT(UTextBlock, TB_Description);

	GET_SLOT(UTextBlock, TB_MailDescription);

	RewardSlots.SetNumUninitialized(MAX_SHOW_REWARDICON_COUNT);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardSlots[0], UIP_RewardIcon1);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardSlots[1], UIP_RewardIcon2);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardSlots[2], UIP_RewardIcon3);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardSlots[3], UIP_RewardIcon4);

	for (auto ThisSlot : RewardSlots)
	{
		if (ThisSlot.IsValid())
		{
			ThisSlot->Init();
			ThisSlot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	GET_SLOT(UCanvasPanel, X_CP_Jackpot);
	if (X_CP_Jackpot.IsValid())
	{
		X_CP_Jackpot->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UTextBlock, TB_Jackpot);
}

void UB2UIMsgPopupReward::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIMsgPopupReward::OnClickConfirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupReward::OnClickClose);
}

void UB2UIMsgPopupReward::UpdateStaticText()
{
	InitRewardMsgPopupText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Common_RewardedTitle")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Common_RewardedDesc")));

	if (TB_MailDescription.IsValid())
	{
		TB_MailDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Common_RewardMailDescription")));
	}
	if (TB_Jackpot.IsValid())
	{
		TB_Jackpot->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Enhance_Jackpot")));
	}
}

void UB2UIMsgPopupReward::CloseWidgetDelegate()
{
	OnClickConfirm();
}

bool UB2UIMsgPopupReward::HasAnyRewardToShow()
{
	if (CurrentRewardPage + 1 < TotalRewardPage)
		return true;

	return false;
}

void UB2UIMsgPopupReward::OnClickConfirm()
{
	if (bForceApplyRewardToAccount)
		ApplyCurrentPageReward(CurrentRewardPage);

	if (HasAnyRewardToShow())
	{
		ShowNextReward();
		return;
	}

	ShowRewardList.Empty();

	if (ConfirmDelegate.ExecuteIfBound())
		ConfirmDelegate.Unbind();

	if (bOpenNickNamePopup)
	{
		// 이름 변경권사용하기 위해 인벤토리로 이동
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("NameChange_Link"))),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([this]() {
			LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
			ForcedInvenTabOpenClass<EItemInvenType>::GetInstance().Signal(EItemInvenType::EIIVT_Consumables);

		})
		);
	}

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupReward::OnClickClose()
{
	DestroySelf(UB2UIManager::GetInstance());
}

bool UB2UIMsgPopupReward::IsJackpotReward()
{
	return bIsJackpotReward;
}

void UB2UIMsgPopupReward::ShowNextReward()
{
	UpdateRewardPage(CurrentRewardPage + 1);
	OpenRewardAnimationBP();
}

void UB2UIMsgPopupReward::ShowMailDescription(bool bVisible)
{
	// 일부 보상은 우편함으로 지급 문구는 우편함에서 열면 나오지 않게

	if (TB_MailDescription.IsValid() && bVisible == false)
	{
		TB_MailDescription->SetText(FText::GetEmpty());
	}
}

void UB2UIMsgPopupReward::ApplyCurrentPageReward(int32 RewardPage)
{
	const int32 RewardIdx = RewardPage * MAX_SHOW_REWARDICON_COUNT;
	const int32 MaxRewardIdx = RewardIdx + MAX_SHOW_REWARDICON_COUNT;
	for (int32 Idx = RewardIdx; Idx < MaxRewardIdx; Idx++)
	{
		if (ShowRewardList.IsValidIndex(Idx))
		{
			const FCommonRewardInfo& RewardInfo = ShowRewardList[Idx];

			if (RewardInfo.RewardID && RewardInfo.RewardAmount > 0)
			{
				ApplyRewardToClientDataStore(RewardInfo);
			}
		}
	}
}

void UB2UIMsgPopupReward::ApplyRewardToClientDataStore(const FCommonRewardInfo& RewardInfo)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	DataStore.ApplyRewardInfo(RewardInfo);
}

void UB2UIMsgPopupReward::ApplyRewardToClientDataStore(b2network::B2RewardPtr RewardPtr)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	DataStore.ApplyRewardInfo(RewardPtr);
}

void UB2UIMsgPopupReward::AddRewardItem_Internal(const FCommonRewardInfo& RewardInfo)
{
	bool bAlreadyExistRewardInfo = false;
	for (int32 Idx = 0; Idx < ShowRewardList.Num(); Idx++)
	{
		if (ShowRewardList[Idx].RewardID == RewardInfo.RewardID)
		{
			ShowRewardList[Idx].RewardAmount += RewardInfo.RewardAmount;
			bAlreadyExistRewardInfo = true;

			break;
		}
	}

	if (bAlreadyExistRewardInfo == false)
	{
		ShowRewardList.Add(RewardInfo);
	}
}

void UB2UIMsgPopupReward::UpdateRewardPage(int32 ShowRewardPage)
{
	int32 CurrRewardIndex = ShowRewardPage * MAX_SHOW_REWARDICON_COUNT;

	for (auto& RewardSlot : RewardSlots)
	{
		if (RewardSlot.IsValid())
		{
			bool bVisibleSlot = ShowRewardList.IsValidIndex(CurrRewardIndex) && ShowRewardList[CurrRewardIndex].RewardAmount > 0;
			if (bVisibleSlot)
			{
				const FCommonRewardInfo& RewardInfo = ShowRewardList[CurrRewardIndex];
				if (RewardInfo.bTemplateId)
				{
					RewardSlot->UpdateRewardIconByItemIndex(RewardInfo.RewardID, FText::AsNumber(RewardInfo.RewardAmount));
				}
				else
				{
					RewardSlot->UpdateRewardIcon(RewardInfo.RewardID, RewardInfo.RewardAmount);
				}

				//RewardSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				RewardSlot->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				RewardSlot->SetVisibility(ESlateVisibility::Collapsed);
			}

			CurrRewardIndex++;
		}
	}

	CurrentRewardPage = ShowRewardPage;
}

UB2UIMsgPopupReward::UB2UIMsgPopupReward(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TotalRewardPage = 0;
	CurrentRewardPage = 0;
}

void UB2UIMsgPopupReward::DestroySelf(class UB2UIManager* InUIManager)
{
	for (auto ThisSlot : RewardSlots)
	{
		if (ThisSlot.IsValid())
			ThisSlot->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupReward::InitRewardMsgPopupText(const FText& TitleText, const FText& BtmDescText)
{
	if (TB_RewardTitle.IsValid())
	{
		TB_RewardTitle->SetText(TitleText);
	}

	if (TB_Description.IsValid())
	{
		TB_Description->SetText(BtmDescText);
	}
}

void UB2UIMsgPopupReward::InitRewardMsgPopup(const TArray<b2network::B2RewardPtr>& RewardList)
{
	FText ButtonText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChapterStarRewardGet"));
	AddRewardItems(RewardList);
}

void UB2UIMsgPopupReward::AddRewardItems(const TArray<b2network::B2RewardPtr>& RewardList, bool bShowRewardNow)
{
	for (b2network::B2RewardPtr RewardPtr : RewardList)
	{
		if (RewardPtr && RewardPtr->item)
		{
			if (RewardPtr->item->inventory_type == b2network::B2InventoryType::Consumables)
			{
				ApplyRewardToClientDataStore(RewardPtr);
			}
		}

		if (RewardPtr && RewardPtr->raw_reward->id && RewardPtr->raw_reward->count > 0)
			AddRewardItem_Internal(FCommonRewardInfo(RewardPtr));
	}

	if (bShowRewardNow)
		ShowRewardPopup();
}

void UB2UIMsgPopupReward::AddSingleRewardItem(ERewardType RewardType, int32 Count)
{
	if (Count > 0)
	{
		const int32 RewardID = BladeIIGameImpl::GetClientDataStore().GetRewardID(RewardType);
		FCommonRewardInfo SingleRewardInfo(RewardID, Count);
		AddRewardItem_Internal(SingleRewardInfo);
	}
}

void UB2UIMsgPopupReward::AddSingleRewardItem(int32 ItemTemplateId, int32 Count)
{
	if (Count > 0)
	{
		//RewardInfoTable 에 있는 RewarId가 아닌 key값을 Id로 사용하고 있어서 key값을 받아와서 적용
		const int32 RewardID = BladeIIGameImpl::GetClientDataStore().GetRewardIDKeyValue(ItemTemplateId);
		FCommonRewardInfo SingleRewardInfo(RewardID, Count);
		AddRewardItem_Internal(SingleRewardInfo);
	}
}

void UB2UIMsgPopupReward::AddSingleRewardItem(const b2network::B2RewardPtr RewardPtr)
{
	if (RewardPtr && RewardPtr->raw_reward->id && RewardPtr->raw_reward->count > 0)
		AddRewardItem_Internal(FCommonRewardInfo(RewardPtr));

	ShowRewardPopup();
}

void UB2UIMsgPopupReward::AddSingleRewardItemByTemplateId(int32 ItemTemplateId, int32 Count)
{
	if (Count > 0)
	{
		FCommonRewardInfo SingleRewardInfo(ItemTemplateId, Count, true);
		AddRewardItem_Internal(SingleRewardInfo);
	}
}

void UB2UIMsgPopupReward::SetConfirmCallback(const FMsgPopupOnClick& Handler)
{
	ConfirmDelegate = Handler;
}

void UB2UIMsgPopupReward::ShowRewardPopup(bool bApplyRewardToAccount /* optional = false */)
{
	const int32 TotalRewardCount = ShowRewardList.Num();

	bForceApplyRewardToAccount = bApplyRewardToAccount;

	TotalRewardPage = TotalRewardCount / MAX_SHOW_REWARDICON_COUNT;
	TotalRewardPage += (TotalRewardCount % MAX_SHOW_REWARDICON_COUNT > 0) ? 1 : 0;	// 짜투리가 있으면 페이지 증가

	UpdateRewardPage(0);
	OpenRewardAnimationBP();
}


void UB2UIMsgPopupReward::UpdateDescriptionText(const FText& DescText)
{
	if (TB_MailDescription.IsValid())
		TB_MailDescription->SetText(DescText);
}

void UB2UIMsgPopupReward::SetOpenNickNameChangePopup()
{
	bOpenNickNamePopup = true;
}

void UB2UIMsgPopupReward::SetJackpotReward(bool bJackpot)
{
	bIsJackpotReward = bJackpot;
}
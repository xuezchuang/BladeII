// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPage.h"

#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "B2PlatformAPI.h"
#include "B2UIEventSlot.h"
#include "Retailer.h"
#include "RewardEventManager.h"
#include "B2UIMailRewardPopUp.h"
#include "B2RedDotManager.h"
#include "B2UIManager.h"
#include "Blueprint/AsyncTaskDownloadImage.h"

void UB2UIEventPage::InitEventPage(const int32 iEventId, const FB2ResponseGetEventPtr CahcedReciveGetEvent)
{
	SetEventID(iEventId);
	Init();
	InitEventPageMasterData();
	UpdateEventPage(CahcedReciveGetEvent);
	SortEventSlotList();
	UpdateEventSlotListVisibleState();
}

void UB2UIEventPage::Init()
{
	Super::Init();

	InitURL();
	UpdateURLBtnState();
	InitBannerFromURL();

	SubscribeEvents_EventPage();
}

void UB2UIEventPage::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents_EventPage();

	Super::DestroySelf(InUIManager);
}

void UB2UIEventPage::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_GotoURL);
	GET_SLOT(UTextBlock, TB_URL);
	GET_SLOT(UImage, IMG_Banner);
}

void UB2UIEventPage::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GotoURL, &UB2UIEventPage::OnClickBTN_GotoURL);
}

void UB2UIEventPage::InitURL()
{
	if (const b2network::B2mdEventInfo* pEventInfo = RewardEventManager::GetInstance().GetEnableEventInfo(iEventID))
	{
		if (TB_URL.IsValid())
			TB_URL->SetText(FText::FromString(pEventInfo->link_text));

		strCahcedLinkURL = pEventInfo->link_url;
		strCahcedBannerUrl = pEventInfo->banner_url;
	}
}

void UB2UIEventPage::InitBannerFromURL()
{
	ImageDownloader = NewObject<UAsyncTaskDownloadImage>(this, NAME_None, RF_Transient);

	if (ImageDownloader)
	{
		ImageDownloader->OnSuccess.AddDynamic(this, &UB2UIEventPage::OnSucessedDownloadBanner);
		ImageDownloader->OnFail.AddDynamic(this, &UB2UIEventPage::OnFailedDownloadBanner);
		ImageDownloader->Start(strCahcedBannerUrl);
	}
}

void UB2UIEventPage::UpdateURLBtnState()
{
	if (strCahcedLinkURL.IsEmpty() ||
		strCahcedLinkURL == TEXT("null"))
	{
		BTN_GotoURL->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIEventPage::OpenMailPopup()
{
	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_EVENT });

	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		pUIManager->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);
	}
}

void UB2UIEventPage::OnSucessedDownloadBanner(class UTexture2DDynamic* pTexture)
{
	if (IMG_Banner.IsValid())
	{
		IMG_Banner->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (pTexture)
		{
			IMG_Banner->SetBrushFromTextureDynamic(pTexture);
		}
	}
}

void UB2UIEventPage::OnFailedDownloadBanner(class UTexture2DDynamic* pTexture)
{
	if (iTryCountDownBannerImage > 0)
	{
		iTryCountDownBannerImage -= 1;

		if (ImageDownloader)
		{
			ImageDownloader->Start(strCahcedBannerUrl);
		}
	}
}

void UB2UIEventPage::UpdateEventSlotListVisibleState()
{
	for (int32 i = 0; i < ArrayEventSlotList.Num(); ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i))
			ArrayEventSlotList[i]->UpdateVisibleState();
	}
}

void UB2UIEventPage::OnClickBTN_GotoURL()
{
	if (!BTN_GotoURL.IsValid())
		return;

	B2P_ShowOpenURL(strCahcedLinkURL);
}

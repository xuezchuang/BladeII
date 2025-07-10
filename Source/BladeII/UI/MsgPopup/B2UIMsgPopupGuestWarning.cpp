// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIMsgPopupGuestWarning.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"
#include "EventSubsystem.h"
#include "Retailer.h"
#include "B2PlatformAPI.h"


#if PLATFORM_ANDROID
#include "AndroidRuntimeSettings.h"
#include "Android/AndroidJNI.h"
#endif
#if PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#include "IOSRuntimeSettings.h"
#endif


void UB2UIMsgPopupGuestWarning::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UB2RichTextBlock, TB_Content);

	GET_SLOT(UB2Button, BTN_Cancel);
	GET_SLOT(UTextBlock, TB_Cancel);
	GET_SLOT(UB2Button, BTN_Buy);
	GET_SLOT(UTextBlock, TB_Buy);
	GET_SLOT(UB2Button, BTN_Change);
	GET_SLOT(UTextBlock, TB_Change);

	GET_SLOT(UB2UISettingMsgPopup, UI_MsgPopupSetting);
	if (UI_MsgPopupSetting.IsValid())
	{
		UI_MsgPopupSetting->Init();
		UI_MsgPopupSetting->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIMsgPopupGuestWarning::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	if (TB_Content.IsValid())
		TB_Content->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_GuestWarning_Content")));

	if (TB_Cancel.IsValid())
		TB_Cancel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Cancel")));
	if (TB_Buy.IsValid())
		TB_Buy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Buy")));
	if (TB_Change.IsValid())
		TB_Change->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_GuestWarning_Switch")));
}

void UB2UIMsgPopupGuestWarning::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Change, &UB2UIMsgPopupGuestWarning::OnClickBTN_Change);
}

void UB2UIMsgPopupGuestWarning::Init()
{
	Super::Init();

	SubscribeEvents();

	// kakao 로그인 delegate 등록
	m_KakaoLinkDelegateCalled.AtomicSet(false);	
#if PLATFORM_ANDROID 
	FJavaWrapper::OnKakaoLinkCompletedDelegate.Remove(OnKakaoLinkHandler);
	OnKakaoLinkHandler = FJavaWrapper::OnKakaoLinkCompletedDelegate.AddUObject(this, &UB2UIMsgPopupGuestWarning::KakaoLinkDelegate);
#elif PLATFORM_IOS
	FKakaoTalkPlugin::OnConnectDelegate.Remove(OnKakaoLinkHandler);
	OnKakaoLinkHandler = FKakaoTalkPlugin::OnConnectDelegate.AddUObject(this, &UB2UIMsgPopupGuestWarning::KakaoLinkDelegate);
#endif
}

void UB2UIMsgPopupGuestWarning::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_KakaoLinkDelegateCalled)
	{
		FString sIDPCode = TEXT("kakaocapri");

		UE_LOG(LogClass, Log, TEXT("####RequestKakaoAccountConversionComplete sIDPCode!!!!!!!!!!!!!! = [[[%s]]]"), *sIDPCode);
		data_trader::Retailer::GetInstance().RequestKakaoAccountConversionComplete(sIDPCode);
		m_KakaoLinkDelegateCalled.AtomicSet(false);
	}
}

void UB2UIMsgPopupGuestWarning::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupGuestWarning::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryKakaoAccountConversionPrepareClass<FB2ReciveKakaoAccountConversionPrepare>::GetInstance().Subscribe2(
		[this](FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing)
	{
		this->OnReceiveKakaoAccountConversionPrepare(KakaoMessageProcessing);
	}
	));

	Issues.Add(DeliveryKakaoAccountConversionCompleteClass<FB2ReciveKakaoAccountConversionComplete>::GetInstance().Subscribe2(
		[this](FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing)
	{
		this->OnReceiveKakaoAccountConversionComplete(KakaoMessageProcessing);
	}
	));
}

void UB2UIMsgPopupGuestWarning::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIMsgPopupGuestWarning::CloseWidgetDelegate()
{
	ClosePopup();
}

void UB2UIMsgPopupGuestWarning::OpenPopup()
{
	if (this->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
}

void UB2UIMsgPopupGuestWarning::ClosePopup()
{
	if (this->GetVisibility() != ESlateVisibility::Collapsed)
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIMsgPopupGuestWarning::KakaoConnect()
{
	UI_MsgPopupSetting->SetVisibility(ESlateVisibility::Hidden);
	B2P_KakaoAccountLink();
}

void UB2UIMsgPopupGuestWarning::KakaoPopupHidden()
{
	UI_MsgPopupSetting->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIMsgPopupGuestWarning::OnClickBTN_Change()
{
	// 서버 쪽으로 우선 검증을 위하여 보냄
	FString sIDPCode = TEXT("kakaocapri");
	data_trader::Retailer::GetInstance().RequestKakaoAccountConversionPrepare(sIDPCode);
}

void UB2UIMsgPopupGuestWarning::OnReceiveKakaoAccountConversionPrepare(FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing)
{
	//TArray<b2network::B2RawRewardPtr> RawRewardPtr = GET_TUPLE_DATA(FB2ResponseKakaoAccountConversionPrepare::rewards_index, KakaoMessageProcessing);

	//for (auto RawRewardSet : RawRewardPtr)
	//{
	//	UI_MsgPopupSetting->SetRewardItem(RawRewardSet->id, RawRewardSet->count);
	//}

	//UI_MsgPopupSetting->SetOwnerGuestWarningUI(this);
	//UI_MsgPopupSetting->SetPopupType(EPopupTypeSetting::EPopupType_GuestWarningLink);
	//UI_MsgPopupSetting->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIMsgPopupGuestWarning::OnReceiveKakaoAccountConversionComplete(FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing)
{
	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoAccountConversionComplete RESPONSE SUCESS."));

	//TArray<b2network::B2RawRewardPtr> RawRewardPtr = GET_TUPLE_DATA(FB2ResponseKakaoAccountConversionComplete::rewards_index, KakaoMessageProcessing);

	//for (auto RawRewardSet : RawRewardPtr)
	//{
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet id: %d"), RawRewardSet->id);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet count.: %d"), RawRewardSet->count);
	//}
}

/* Kakao */
#if PLATFORM_ANDROID
void UB2UIMsgPopupGuestWarning::KakaoLinkDelegate(JNIEnv *env, jobject thiz)
{
	UE_LOG(LogClass, Log, TEXT("####KakaoLinkDelegate !!!!!!!!!!!!!! = [[[]]]"));
	MarkKakaoLink();

}
#elif PLATFORM_IOS
void UB2UIMsgPopupGuestWarning::KakaoLinkDelegate(bool bIsSuccess)
{
	UE_LOG(LogClass, Log, TEXT("####KakaoLinkDelegate !!!!!!!!!!!!!! = [[[]]]"));
	if (bIsSuccess)
	{
		MarkKakaoLink();
	}
}
#endif

void UB2UIMsgPopupGuestWarning::MarkKakaoLink()
{
	m_KakaoLinkDelegateCalled.AtomicSet(true);
}



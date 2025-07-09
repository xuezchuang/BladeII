// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UISettingMsgPopup.h"
#include "B2UIMsgPopupGuestWarning.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupGuestWarning : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:

	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	void OpenPopup();
	void ClosePopup();

	void KakaoConnect();
	void KakaoPopupHidden();

protected:

	UFUNCTION()
		void OnClickBTN_Change();

private:

	void OnReceiveKakaoAccountConversionPrepare(FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing);
	void OnReceiveKakaoAccountConversionComplete(FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing);

private:

	// Kakao
	FThreadSafeBool m_KakaoLinkDelegateCalled;
#if PLATFORM_ANDROID
	FDelegateHandle OnKakaoLinkHandler;
	void KakaoLinkDelegate(JNIEnv *env, jobject thiz);
#elif PLATFORM_IOS
	FDelegateHandle OnKakaoLinkHandler;
	void KakaoLinkDelegate(bool bIsSuccess);
#endif
	void MarkKakaoLink();

private:

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UB2RichTextBlock> TB_Content;

	TWeakObjectPtr<UB2Button> BTN_Cancel;
	TWeakObjectPtr<UTextBlock> TB_Cancel;
	TWeakObjectPtr<UB2Button> BTN_Buy;
	TWeakObjectPtr<UTextBlock> TB_Buy;
	TWeakObjectPtr<UB2Button> BTN_Change;
	TWeakObjectPtr<UTextBlock> TB_Change;

	TWeakObjectPtr<UB2UISettingMsgPopup> UI_MsgPopupSetting;

private:

	TArray<std::shared_ptr<class issue>> Issues;

};

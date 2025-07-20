// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2PlatformAPI.h"
#include "B2UIDLCFrontPopup.h"
#include "CommonStruct.h"
#include "../B2GameEnum.h"
#include "B2UISettingAccount.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISettingAccount : public UB2UIWidget
{
	GENERATED_BODY()
	friend class UB2UISetting;
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	/* Events that occur when finished completely close. */
	virtual void OnCloseComplete() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void KakaoGoogleConnect();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void KakaoGoogleDisConnect();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void KakaoConnect();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void KakaoDisConnect();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void GameSecessionBtn();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void ShowAchievementsView();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	EPlatformIDPCode GetIDPCode();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void SetClipBoard();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void OpenKakaoPopup();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void OnShowCSView();


protected:
	UFUNCTION()
	void OnClickB2Button_358();
	UFUNCTION()
	void OnClickBTN_Facebook();
	UFUNCTION()
	void OnClickBTN_Google_GameCenter();
	UFUNCTION()
	void OnClickBTN_Logout();
	UFUNCTION()
	void OnClickBTN_DeleteAccount();


private:
	TWeakObjectPtr<UB2Button> B2Button_358;
	TWeakObjectPtr<UB2Image> B2Image_0;
	TWeakObjectPtr<UB2Button> BTN_Facebook;
	TWeakObjectPtr<UB2Button> BTN_Google_GameCenter;
	TWeakObjectPtr<UB2Button> BTN_Logout;
	TWeakObjectPtr<UB2Button> BTN_DeleteAccount;
	TWeakObjectPtr<UB2Button> B2Button_Kakaogoogle;
	TWeakObjectPtr<UTextBlock> TXT_UserID;
	TWeakObjectPtr<UB2Button> BTN_KakaoConnect;
	TWeakObjectPtr<UTextBlock> TXT_Google;
	TWeakObjectPtr<UTextBlock> TXT_GameVer;
	TWeakObjectPtr<UTextBlock> TXT_AccLink;

	TWeakObjectPtr<UB2Image> IMG_KakaoLogo;

	TWeakObjectPtr<UB2UIDLCFrontPopup> UI_MsgPopupSetting01;

	FThreadSafeBool m_KakaoLinkDelegateCalled;
	uint8	DeliveryKakaoAccountConversionPrepareTicket;
	uint8	DeliveryKakaoAccountConversionCompleteTicket;
	bool bSubscribed;
#if PLATFORM_ANDROID
	FDelegateHandle OnKakaoLinkHandler;
	void KakaoLinkDelegate(JNIEnv* env, jobject thiz);
#elif PLATFORM_IOS
	FDelegateHandle OnKakaoLinkHandler;
	void KakaoLinkDelegate(bool bIsSuccess);
#endif


	void ReloadUI();
	void MarkKakaoLink();
	void SubscribeEvent();
	void UnSubscribeEvent();

	void OnReceiveKakaoAccountConversionPrepare(FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing);
	void OnReceiveKakaoAccountConversionComplete(FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing);
protected:
	void SetVersionInfo();

public:
	class UB2UISetting* owner_;
	void SetOwnerUI(class  UB2UISetting* owner);
	void KakaoPopupHidden();
};

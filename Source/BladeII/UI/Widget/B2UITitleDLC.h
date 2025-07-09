#pragma once

#include "B2UIDLCFrontBase.h"
#include "B2UIBackWidget.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif
#include "CommonStruct.h"
#include "B2UITitleDLC.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITitleDLC : public UB2UIDLCFrontBase, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	UB2UITitleDLC(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void CacheAssets() override;
	
	virtual void DestroySelf() override;

	void TurnOffStartMovies();
	void SetSelectedMediaSourceInfo(class UMediaSource* InSelectedSource, EPCClass inSelectedClass);

protected:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void DeferredPlatformServerConnectionHandling(float InDeferSecond);
	void PlatformServerConnectionHandling();
	void PlayTouchAnimHandling();
	void OnMouseDownHandling();

#if PLATFORM_ANDROID 	
	void None_Kakao_Sdk_Login();
#endif 

	TWeakObjectPtr<class UB2UITitleDesign> UIP_TitleDesign;
	
private:
	virtual void CloseWidgetDelegate() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	uint32 DoneToGetAccountTicket;
	uint32 DeliveryGiveTutorialRewardTicket;

	bool m_firstRequestGetAccount;
	FThreadSafeBool m_LoginDelegateCalled;
	FString ServiceType;

	FTimerHandle DeferredPlatformServerConnectTH;

#if PLATFORM_ANDROID 
	FDelegateHandle OnKakaoLoginHandler;
	void KakaoLoginDelegate(JNIEnv *env, jobject thiz, jstring userID, jstring accessToken);
	FDelegateHandle OnKakaoLoginFailHandler;
	void KakaoLoginfailDelegate(JNIEnv *env, jobject thiz, jint resultCode);	
#endif
#if PLATFORM_IOS 
	FDelegateHandle OnKakaoLoginHandler;
	void KakaoLoginDelegate(bool IsSuccess, int32 ErrorCode);
#endif
	void LoadLocalIni();

	void PostKakaoLoginInit();

};

int32 GetFirstCharacterViewState();

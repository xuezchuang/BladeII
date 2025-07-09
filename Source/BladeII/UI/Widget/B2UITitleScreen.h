#pragma once

#include "B2UIWidget.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif
#include "B2UITitleScreen.generated.h"

/************************************************************************/
/* 
 [B2UITitleScreen]
 Once was the main title UI, having login capabilities too.
 but not much going on in here after UITitleDLC is introduced.
*/
/************************************************************************/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITitleScreen : public UB2UIWidget
{
	GENERATED_BODY()

	TWeakObjectPtr<class UB2UITitleDesign> UIP_TitleDesign;
	TWeakObjectPtr<UImage> IMG_Alternative_LobbyLoading;
	/** True use simple image, instead of title movie. */
	UPROPERTY(EditAnywhere)
	bool bUseAlternativeLoadingImage;

	TWeakObjectPtr<UPanelWidget> PN_DLC_COOK;
	
	TWeakObjectPtr<UTextBlock> TB_ExtraLoadingMessage;

public:
	UB2UITitleScreen(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void CacheAssets() override;

protected:
	
	void ShowExtraLoadingMessage(bool bShow);
	void ShowAlternativeLoadingScreen();

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void PlatformServerConnectionHandling();
#if BUILD_STANDALONE_TEST_APK
	void GoToStandaloneTestScene();
#endif

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UPROPERTY(Transient)
	class UB2UIDummyDLCFront* DevDummyDLCFront;

private:
	void SetFailResponseMasterData();
	void CheckNewTutorial();
	void GoToTutorial();
	void GoToLobby();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	uint32 DoneToGetAccountTicket;
	uint32 FailResponseMasterDataTicket;

	bool m_firstRequestGetAccount;
	FThreadSafeBool m_LoginDelegateCalled;

#if PLATFORM_ANDROID
	FDelegateHandle OnKakaoLoginHandler;
	void KakaoLoginDelegate(JNIEnv *env, jobject thiz, jstring userID, jstring accessToken);
#endif
#if PLATFORM_IOS
	FDelegateHandle OnKakaoLoginHandler;
	void KakaoLoginDelegate(bool IsSuccess, int32 ErrorCode);
#endif

	void MarkKakaoLogin();
	void PostKakaoLoginInit();

	FTimerHandle DelayedGameLobbyStartTH;
	void DelayedGameLobbyStartDirect();
	void GameLobbyStartDirect();
	
	FTimerHandle CloseMiscLoadingIndicatorTH;
	void CloseMiscLoadingIndicator();

	//////////////////////////////////////////////////
	// Fake DLCUpdate and Kakao login handling.
	void BeginFakeKakaoLogin();

	UFUNCTION()
	void OnCompleteDummyDLCFrontFakeDLCUpdate();
	UFUNCTION()
	void OnCompleteDummyDLCFrontFakeKakaoLogin();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	void CreateDummyDLCFront();
	void DestroyDummyDLCFront();
#endif
	//////////////////////////////////////////////////

#if !UE_BUILD_SHIPPING
	bool CheckAndSetupForTitleMovieCaptureMode();
#endif
};


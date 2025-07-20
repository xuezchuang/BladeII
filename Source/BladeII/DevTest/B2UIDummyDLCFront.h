#pragma once
#include "bladeII.h"
#include "B2UnitedWidgetBase.h"
#include "B2UIDummyDLCFront.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnFakeDLCUpdateDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnFakeKakaoLoginDelegate);

/**
 * UB2UIDummyDLCFront
 * Only for development purpose, it mimics DLC download process (without the real operation), 
 * to help initial login process similar to the formal DLC process as much as possible.
 * 
 * Additionally provide fake Kakao login process too, which is a pat of LobbyGameMode, not the DLCFront.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIDummyDLCFront : public UB2UnitedWidgetBase
{
	GENERATED_BODY()

	TWeakObjectPtr<UPanelWidget> P_FakeDLCUpdate;
	TWeakObjectPtr<UB2Button> BTN_FakeDLCUpdate;
	
	TWeakObjectPtr<UPanelWidget> P_FakeKakaoConnect;
	TWeakObjectPtr<UB2Button> BTN_FakeKakaoConnect;

public:

	UB2UIDummyDLCFront(const FObjectInitializer& ObjectInitializer);

	FOnFakeDLCUpdateDelegate OnFakeUpdate;
	FOnFakeKakaoLoginDelegate OnFakeKakaoLogin;

	void ShowFakeDLCUpdate(bool bShow);
	void ShowFakeKakaoLogin(bool bShow);

	UFUNCTION()
	void OnClickBtnFakeUpdate();
	UFUNCTION()
	void OnClickBtnFakeKakaoConnect();
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	void FakeKakaoLoginHandlingCommon();
	
	/** Tracking if the real DLC Front has been loaded once. Even with NonDLCCookRun environment, it might goes through DLCFrontMap, for example, PC test. */
	static bool bWentThroughRealDLCFront;
	
	FTimerHandle FakeUpdateCompleteTH;
	void OnFakeUpdateComplete();
	
	void BeginFakeKakaoLogin();
	
	FTimerHandle FakeKakaoLoginTH;
	void OnFakeKakaoLoginComplete();

	static TSubclassOf<UB2UIDummyDLCFront> GetUniqueClass();
	static int32 GetViewportZOrder();

	void DestroySelf();

protected:
	virtual void NativeConstruct() override;
	void CacheAssets();
	
#endif
};

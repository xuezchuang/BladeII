// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UISelectLogin.generated.h"

// #if  PLATFORM_IOS
// #include "OnlineSubsystemFacebook.h"
// #include "Online.h" 
// #endif

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISelectLogin : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UB2UISelectLogin(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	virtual void CacheAssets() override;	
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnclickBtnGameCenter();
	UFUNCTION()
		void OnclickBtnFacebook();
	UFUNCTION()
		void OnclickBtnGoogle();
	UFUNCTION()
		void OnclickBtnGuest();
	UFUNCTION()
		void OnclickBtnExit();
	
#if PLATFORM_IOS
	void ReturnFacebookAuthToken(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void ReturnGameCenterAuthToken(TSharedPtr<const FUniqueNetId> UniqueId, int LocalUserNum);
#endif

protected:
	TWeakObjectPtr<UButton>			Facebook_btn;
	TWeakObjectPtr<UButton>			Google_btn;
	TWeakObjectPtr<UButton>			Guest_btn;	
	TWeakObjectPtr<UButton>			Exit_btn;
	TWeakObjectPtr<UButton>			GameCenter_btn;
};

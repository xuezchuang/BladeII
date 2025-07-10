// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIDLCFrontBase.h"
#include "B2UICheckPermission.h"
#include "B2UIDLCFrontPopup.h"
#include "B2UIActionLogo.generated.h"


UCLASS()
class BLADEII_API UB2UIActionLogo : public UB2UIDLCFrontBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
	void GotoPatchCheck();

	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		bool GetMediaTimeSet(float fTime);
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void SetMovieStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "ContentsSub")
		void CheckMediaTime();

	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		bool IsTablet();

	void OpenPermissionNotice();

	void OpenTitleMain();

	FThreadSafeBool m_PermissionDelegateCalled;

#if PLATFORM_ANDROID
	FDelegateHandle OnCheckPermissionHandler;
	void CheckPermissionDelegate(JNIEnv *env, jobject thiz, jboolean bResult);
#endif

	void RetryPlatformPermission();
	void GameExit();
protected:

private:
	bool m_bMovieTime;
	TWeakObjectPtr<UB2Image> B2Image_0;
	TWeakObjectPtr<UB2UICheckPermission> UI_CheckPermission;
	TWeakObjectPtr<UB2UIDLCFrontPopup> UI_MsgPopup01;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};

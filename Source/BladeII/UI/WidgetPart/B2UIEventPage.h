// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "EventSubsystem.h"
#include "B2UIEventPage.generated.h"

UCLASS()
class BLADEII_API UB2UIEventPage : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void InitEventPage(const int32 iEventId, const FB2ResponseGetEventPtr CahcedReciveGetEvent);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

protected:
	virtual void SubscribeEvents_EventPage() {}
	void UnsubscribeEvents_EventPage() { Issues_EventPage.Empty(); }

protected:
	void InitURL();
	void InitBannerFromURL();
	void UpdateURLBtnState();
	void OpenMailPopup();

protected:
	UFUNCTION()
	void OnSucessedDownloadBanner(class UTexture2DDynamic* pTexture);

	UFUNCTION()
	void OnFailedDownloadBanner(class UTexture2DDynamic* pTexture);

public:
	UFUNCTION()
	void OnClickBTN_GotoURL();

public:
	virtual void InitEventPageMasterData() {}
	virtual void SortEventSlotList() {}
	void UpdateEventSlotListVisibleState();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) {}

public:
	void SetEventID(int32 ieventid) { iEventID = ieventid; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString strCahcedBannerUrl;

protected:
	TWeakObjectPtr<UB2Button>		BTN_GotoURL;

	TWeakObjectPtr<UTextBlock>		TB_URL;

	TWeakObjectPtr<UImage>			IMG_Banner;

	int32	iTryCountDownBannerImage = 3;
	
	int32	iEventID;

	FString	strCahcedLinkURL;

	TArray<TWeakObjectPtr<class UB2UIEventSlot>>	ArrayEventSlotList;

	UPROPERTY(Transient)
	class UAsyncTaskDownloadImage*	ImageDownloader;

protected:
	TArray<issue_ptr>	Issues_EventPage;
};

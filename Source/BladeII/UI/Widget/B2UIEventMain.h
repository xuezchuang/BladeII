// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIEventMain.generated.h"

//b2network::B2EventCategory
UENUM(BlueprintType)
enum class EEVENT_CATEGORY : uint8
{
	mNULL =0,
	POINT_REWARD	= 1,
	LEVEL_UP		= 3,
	STAGE_CLEAR		= 5,
	POINT_SHOP		= 6,
	GEM_PURCHASE	= 7,
	SHOP_LOTTERY	= 8,
	SPEND_GEM		= 9,
	SHOP_LOTTERY_SAVINGS = 10,
	DICE			= 11,
	//SEASON_MISSION= 4,	not event page
	//HOT_TIME_BUFF = 2,	not event page
};

USTRUCT(BlueprintType)
struct FEventPageData
{
	GENERATED_USTRUCT_BODY()

	//b2network::B2EventCategory
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEVENT_CATEGORY eEventCategory;

	//have duplicate enum values...
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32	iEventSubCategory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<class UB2UIEventPage>	EventPageSubClass;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEventPageInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEventPageData>	TArrayEventPageData;

public:
	TSoftClassPtr<class UB2UIEventPage>* FindEventPageClass(const int32 iEventCategory, const int32 iEventSubCategory);
};

struct tEventTab
{
	TWeakObjectPtr<class UB2UIEventPage>	EventPage;
	TWeakObjectPtr<class UB2UIEventTapPart>	EventTabPart;
};

UCLASS()
class BLADEII_API UB2UIEventMain : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void DoMarkRedDot() final;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	/* Events that occur when finished completely open. */
	virtual void OnOpenComplete() override;
	/* Events that occur when finished completely close. */
	virtual void OnCloseComplete() override;

public:
	void OnSelectEventTap(int32 iEventID);
	UB2UIEventPage* CreateDynamicEventPage(const TSoftClassPtr<UB2UIEventPage>* pCreateWidgetClass,
		UB2UIEventMain* InOwnerUserWidget, UCanvasPanel* CreateParentPanel, int32 iEventID);

	void IsNoEnableEventPage(bool bVisibleNoEnableText);
	void UpdateEventMainPage();

	UFUNCTION(BlueprintCallable, Category = "B2UIEventMain")
	void UpdateNoEnableEventText_BP();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	void CreateEventTaps();
	void SelectEventTap(int32 iEventID);
	void SetVisibeEventTap(tEventTab& EventTab, bool bVisible);
	void UpdateSelectedEventPage();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "B2UIEventMain")
	void StartAnimTransparencyBlock_BP();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UB2UIEventTapPart>			EventTapPartClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventPageInfo>	EventPageInfo;

private:
	TWeakObjectPtr<UVerticalBox>	VB_TabList;

	TWeakObjectPtr<UCanvasPanel>	P_EventPage;

	TArray<tEventTab>				ArrayEventTab;

	bool							bSubscribed;

	TWeakObjectPtr<UB2RichTextBlock> RTB_NoEnableEvent;

	bool							IsCreatedEventTabs;
	int32							SelectedEventTabEventID;

private:
	TArray<issue_ptr>	Issues;
};

// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UILobbyNoticeSlot.h"
#include "../DataTrader/OnlineWholesaler.h"
#include "EventSubsystem.h"
#include "B2UIModeOpenNoticeSlot.generated.h"

struct FCheckModeInfo
{
	FCheckModeInfo(EModeOpenType eType, float nStart, float nEnd)
		: ModeType(eType)
		, StartTime(nStart)
		, EndTime(nEnd)
	{
	}


	explicit FCheckModeInfo()
		: ModeType(EModeOpenType::EModeOpenType_Raid)
		, StartTime(0.f)
		, EndTime(0.f)
	{
	}

	EModeOpenType ModeType;
	float StartTime;
	float EndTime;
};

UCLASS()
class BLADEII_API UB2UIModeOpenNotice : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIModeOpenNotice(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SubscribeEvent();
	void UnSubscribeEvent();

protected:
	virtual void CacheAssets() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void CreateOrRemoveSlot(TArray<EModeOpenType>& AddModeType, TArray<EModeOpenType>& DeleteModeType);
	void RefreshCurrentTime();
	void AddModeOpenSlot(EModeOpenType eModeType);
	void DeleteModeOpenSlot(EModeOpenType eModeType);
	bool IsValidSlotWidget(EModeOpenType eModeType);
	void ClearInfoWidgets();
	void OnReceiveCheckModeOpen(const FB2ResponseCheckModeOpenPtr& fb2r_CheckModeOpen);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ModeOpenNotice")
	TSubclassOf<class UB2UIModeOpenNoticeSlot> ModeOpenNoticeSlotWidget;

private:
	TWeakObjectPtr<UPanelWidget>	P_ContentNotice;

	bool							m_bSubscribed;

	TArray<FCheckModeInfo>	CheckInfos;
	TArray<issue_ptr>	Issues;

	float  CurrentTimeStemp;
	bool bActiveTick;
};

UCLASS()
class BLADEII_API UB2UIModeOpenNoticeSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	UB2UIModeOpenNoticeSlot(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetInfo(EModeOpenType ModeNum);
	FORCEINLINE EModeOpenType GetModeType() const { return CurrentOpenType; }

private:
	void SetIcon(EModeOpenType ModeNum);

protected:
	UFUNCTION()
	void OnClickBTN_ModeOpen();

private:
	TWeakObjectPtr<UTextBlock> TB_ModeName;
	TWeakObjectPtr<UB2Button> BTN_ModeOpen;
	TWeakObjectPtr<UWidgetSwitcher> SW_ModeImages;

	EModeOpenType CurrentOpenType;
};

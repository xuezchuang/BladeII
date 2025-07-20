// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIAttendance.h"
#include "../B2RichTextBlock.h"
#include "B2UIAttendanceMain.generated.h"

UCLASS()
class BLADEII_API UB2UIAttendanceMain : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void RegisterUIMarkForRedDot() final;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) final;

	UFUNCTION()
		bool RedDotCondition_AttendanceMain();
	UFUNCTION()
		bool RedDotCondition_DailyAttendance();
	UFUNCTION()
		bool RedDotCondition_ConnectReward();
	UFUNCTION()
		bool RedDotCondition_EventAttendance();
	UFUNCTION()
		bool RedDotCondition_EventNewUser();

	bool RedDotCondition_BySubMode(const EAttendanceTab& EMode);
	void SetDelayOpenEventTap(const EAttendanceTab& eIndex);
	void SetExitButtonEnable(bool Enable);
	void SetExitButtonVisibility(bool InVisible);
	void SetAttandanceTabEnable(bool Enable);
	void SelectAttendanceTab(EAttendanceTab eIndex, bool bForce = false);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	/* Events that occur when finished completely open. */
	virtual void OnOpenComplete() override;
	/* Events that occur when finished completely close. */
	virtual void OnCloseComplete() override;
	virtual void CloseWidgetDelegate() override;

	void SetAttendanceTap();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickBTN_Attendance();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickBTN_ConnectReward();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickBTN_EventAttendance();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickBTN_NewUser();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickBTN_Event_2();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickBtn_Exit();

public:
	TWeakObjectPtr<UB2RichTextBlock>		TB_Title;


private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	void SetEnableTab(EAttendanceTab eIndex, bool Enable);

	bool IsValidTab(EAttendanceTab eIndex) const;

	int32 GetIndex(EAttendanceTab eIndex);
	EAttendanceTab GetAutoChangeEnableTab(EAttendanceTab eIndex) const;
	EAttendanceTab GetAvailableTab() const;

private:
	struct AttendanceTab
	{
		EAttendanceTab TabType;
		TWeakObjectPtr<UPanelWidget> P_ParentPanel;
		TWeakObjectPtr<UButton>		Button;
		TWeakObjectPtr<UImage>		SelectedImage;
		TWeakObjectPtr<UImage>		UnselectedImage;
		TWeakObjectPtr<UTextBlock>	SelectedText;
		TWeakObjectPtr<UTextBlock>	UnselectedText;
	};

	TWeakObjectPtr<UOverlay>					O_Tab_EventAttendance;
	TWeakObjectPtr<UB2RichTextBlock>			TB_Noti;
	TWeakObjectPtr<UB2Button>					BTN_Exit;
	TArray<AttendanceTab>						m_AttendanceTabs;
	TArray<TWeakObjectPtr<class UB2UIWidget>>	m_AttendanceParts;
	
	FTimerHandle			TimeOpenPopup;

	bool					IsEnableAttendanceTab;
	// For Delay Open
	EAttendanceTab			DelayOpenTap;
	bool					OpenEventAfterDaily;
	float					DelayForOpenEventTap;
	uint32					ReceiveAttendanceExitEnableTicket;
	uint32					ReceiveAttendanceExitVisibilityTicket;
};

// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIPVPModFindingMatch.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPVPModFindingMatch : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	int32 CachedCountDownNumber;

	UB2UIPVPModFindingMatch(const FObjectInitializer& ObjectInitializer);
		
public:
	virtual void Init() override;
	void FindMatch(bool bSet);

	void SetCount(int32 Count);
	void SetReady(int32 Count);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnCountChangedBP(int32 Count);

	void SetUserData(bool bLeft, const FString& UserNick, int32 Level, int32 Grade, int32 Star, int32 WinStreak,  int32 CombatValue,
		const FString& GuildName, int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor, int32 InAttackPower = 0, int32 InDefense = 0);

	void SetEnableCancel(bool bToSet);

	void OnCancelMatch();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

private:
	void InitSetLocalUserData_UsePvpOnly();

protected:
	UFUNCTION()
	void OnClickedCancelBtn();

	UFUNCTION(BlueprintImplementableEvent)
	void Finding_BP();

	UFUNCTION(BlueprintImplementableEvent)
	void SetReady_BP();

private:
	TWeakObjectPtr<UTextBlock> TB_Wait;
	TWeakObjectPtr<UTextBlock> TB_Count;
	TWeakObjectPtr<UImage> IMG_WheelFinding;
	TWeakObjectPtr<UImage> IMG_Versus;
	TWeakObjectPtr<class UB2UIPVP1on1MatchInfo> UIP_LeftMatchInfo;
	TWeakObjectPtr<class UB2UIPVP1on1MatchInfo> UIP_RightMatchInfo;
	TWeakObjectPtr<class UButton> BTN_Cancel;

private:
	bool bFindMatch;

	class UB2UIMsgPopupSimple* OpendCancleMsgPopup;
};
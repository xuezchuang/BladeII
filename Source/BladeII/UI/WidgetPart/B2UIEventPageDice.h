// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "UI/MsgPopup/B2UIMsgPopupSimple.h"
#include "B2UIEventPageDice.generated.h"

UCLASS()
class BLADEII_API UB2UIMsgPopupBuyDicePoint : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()
};


UCLASS()
class BLADEII_API UB2UIDiceEventRollEffect : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void InitDiceEventRollEffect(TWeakObjectPtr<class UB2UIEventPageDice> pWidget);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "B2UIDiceEventRollEffect")
	void PlayAnimRollEffect_BP(int32 DiceNumber);

	UFUNCTION(BlueprintCallable, Category = "B2UIDiceEventRollEffect")
	void EndAnimRollEffect_BP();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 iDiceEventRollEffectZOrder_BP;

private:
	TWeakObjectPtr<class UB2UIEventPageDice> pEventPageDice;
};


struct FDiceEventValue
{
private:
	int32 iValue;

public:
	int32 Get() 
	{ 
		return iValue; 
	}
	void Set(const int32 iVal) 
	{ 
		iValue = iVal;
	}
};

struct FDiceEventStatus
{
public:
	FDiceEventValue	point;
	FDiceEventValue	free_dice;
	FDiceEventValue	position;
	FDiceEventValue	finish_count;
	FDiceEventValue	buy_point_count;
	FDiceEventValue	roll_dice_count;
public:
	FDiceEventValue dice_number;
	FDiceEventValue current_position;
	FDiceEventValue mileage_rewarded_value;
	FDiceEventValue IsReward;
public:
	FDiceEventValue VisibleStartIcon;
};


UCLASS()
class BLADEII_API UB2UIEventPageDice : public UB2UIEventPage
{
	GENERATED_BODY()

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void SubscribeEvents_EventPage();
	
public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;
	void ResponseRollDiceEvent(FB2ResponseRollDiceEventPtr pReciveEventReward);
	void ResponseBuyDiceEventPoint(FB2ResponseBuyDiceEventPointPtr pReciveDiceEventPoint);
	void ResponseRewardDiceEventMileage(FB2ResponseRewardDiceEventMileagePtr pReciveDiceEventMileage);
	void UpdateDiceEventPageAfterRollDiceAnim();

private:
	void CacheAssetsEventSlotDiceCell();
	void InitDiceEventMasterData_Cell();
	void InitDiceEventMasterData_Mileage();
	void InitDiceEventMasterData_BuyPoint();

private:
	void PlayAnimDiceRollEffect();
	void UpdateDiceMovePositionAnim(int32 iCellNumber, bool IsFirstStart = false);
	void UpdateFromFinishArriveAnim(int32 iCellNumber);
	void UpdateDiceEventMain(int32 iFreeDiceCount, int32 iGetDicePoint, int32 iRollDiceCount, int32 iFinishCount);
	void UpdateDiceEventExplain(int32 iFinishCount);
	void UpdateDiceEventSlot(int32 iPosition);
	void UpdateDiceSlotStartIcon(bool IsStarted);
	void UpdateDiceMileage(int32 iRollDiceCount, int32 MileageRewardedValue);
	void UpateDiceBuyPointBtn(int32 iBuyPointCount);

	UFUNCTION()
	void OnClickBTN_BuyDicePoint();

	void RequestBuyDiceEventPoint();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "B2UIEventPageDice")
	void PlayAnimPossibleBuyPoint_BP(bool IsPossibleBuyPointCount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIDiceEventRollEffect> UIDiceEventRollEffectClass;

private:
	TWeakObjectPtr<class UB2UIDiceEventMain>		UIP_DiceEventMain;
	TWeakObjectPtr<class UB2UIDiceEventExplain>		UIP_DiceEventExplain;
	TWeakObjectPtr<class UB2UIDiceEventMileage>		UIP_DiceEventMileage;
	TWeakObjectPtr<class UB2UIDiceEventRollEffect>	pDiceEventRollEffect;

	TWeakObjectPtr<UB2Button>						BTN_BuyDicePoint;
	TWeakObjectPtr<UTextBlock>						TB_BtnBuyDicePoint;

	TWeakObjectPtr<UTextBlock>						TB_NpcDialog;
	
private:
	FDiceEventStatus EventStatus;
	
private:
	TArray<b2network::B2mdDiceEventBuyPoint>	CachedDiceBuyPointMD;
	int32										CachedStartSlotIndex;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ScrollBox.h"
#include "Framework/Layout/Overscroll.h"
#include "B2ScrollBox.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EScrollAdjustType : uint8
{
	ESAT_NONE,
	ESAT_MID,
	ESAT_START,
	ESAT_END
};

UCLASS()
class BLADEII_API UB2ScrollBox : public UScrollBox
{
	GENERATED_UCLASS_BODY()
	
public:
	
	// Only works in Canvas Panel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "B2ScrollBox", meta = (DisplayName = "Scroll Adjust Type"))
	EScrollAdjustType ScrollAdjustType;

	int32 LastAdjustItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "B2ScrollBox", meta = (DisplayName = "Use Deacceleration"))
	uint32 bUseScrollDeacceleration:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "B2ScrollBox", meta = (DisplayName = "Natual Move"))
	uint32 bNatualmove:1;

protected:
	virtual void CreateMyScrollBox();

	virtual void EndUserScrolling();

	void CheckScrollAdjustType();

	void OnUserScrolled(float ScrolledDelta);
	void OnUserScrolledEnd();

	FOnUserScrolled OnScrolled;
	FSimpleDelegate OnScrolledEnd;
public:
	void SetAdjustFromSlotIndex(int32 SlotIndex);

	void SetAnimateScroll(bool bNewAnimateScroll);

	float GetScrollVelocity();

	bool IsScrolling();

	float GetScrollPhysicalOffset();

	FReply OnSBMouseButtonDownFromSender(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply OnSBMouseButtonUpFromSender(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply OnSBMouseMoveFromSender(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void OnSBMouseLeaveFromSender(const FPointerEvent& InMouseEvent);
	FReply OnSBTouchStartedFromSender(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	FReply OnSBTouchMovedFromSender(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	FReply OnSBTouchEndedFromSender(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);

	void SetOnUserScrolled(const FOnUserScrolled& InHandler);

	void SetOnUserScrolledEnd(const FSimpleDelegate& InHandler);
	float GetScrollOffsetMax();
};

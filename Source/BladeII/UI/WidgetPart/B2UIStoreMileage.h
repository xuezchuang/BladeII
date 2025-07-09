#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIStoreMileage.generated.h"

UENUM(BlueprintType)
enum class EUIStoreMileageRewardState : uint8
{
	NotReached		UMETA(DisplayName = "NotReached"),
	Reached			UMETA(DisplayName = "Reached"),
	Rewarded		UMETA(DisplayName = "Rewarded"),
};

UENUM(BlueprintType)
enum class EUIStoreMileageGrade : uint8
{
	BClass		UMETA(DisplayName = "BClass"),
	AClass		UMETA(DisplayName = "AClass"),
	SClass		UMETA(DisplayName = "SClass"),
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStoreMileage : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:

	UB2Button* GetBoxButtonPtr(EUIStoreMileageGrade BoxGrade);

	void SetMileageData(int32 MileageType);

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void SetReachedState(EUIStoreMileageGrade BoxIndex, EUIStoreMileageRewardState RewardState);

	UFUNCTION()
		void UpdateMileage();

private:

	TWeakObjectPtr<UTextBlock>		TB_Mileage;
	TWeakObjectPtr<UTextBlock>		TB_MileageValue;
	TWeakObjectPtr<UProgressBar>	PB_Mileage;
	TWeakObjectPtr<UTextBlock>		TB_MileageS;
	TWeakObjectPtr<UTextBlock>		TB_MileageA;
	TWeakObjectPtr<UTextBlock>		TB_MileageB;
	TWeakObjectPtr<UTextBlock>		TB_MaxValue01;
	TWeakObjectPtr<UTextBlock>		TB_MaxValue02;
	TWeakObjectPtr<UTextBlock>		TB_MaxValue03;
	TWeakObjectPtr<UB2Button>		BTN_Box01Area;
	TWeakObjectPtr<UB2Button>		BTN_Box02Area;
	TWeakObjectPtr<UB2Button>		BTN_Box03Area;

	TWeakObjectPtr<UOverlay>		O_MileageS;
	TWeakObjectPtr<UOverlay>		O_MileageA;
	TWeakObjectPtr<UOverlay>		O_MileageB;

	FDelegateHandle MileageChangedHandler;

};
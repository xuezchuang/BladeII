#pragma once

#include "B2UIChapterStarRewardPreview.h"
#include "B2UIStoreMileageRewardPopup.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStoreMileageRewardPopup : public UB2UIChapterStarRewardPreview
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	void SetRewardPopup(int32 MileageType);

protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	//UFUNCTION()
		void OnClickBtnClose();

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIStoreMileageRewardPopupSlot> RewardPopupSlot;

private:

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_Summary;
	TWeakObjectPtr<UTextBlock> TB_Caution1;
	TWeakObjectPtr<UTextBlock> TB_Caution2;
	TWeakObjectPtr<UHorizontalBox> HB_RewardList;

	TWeakObjectPtr<UButton> BTN_Close;

private:

	int32 CurrentMileageType;

};
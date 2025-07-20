#pragma once

#include "B2UIWidget.h"
#include "B2UISelectiveLotteryConsumeItemResultPopup.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISelectiveLotteryConsumeItemResultPopup : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void OpenPopup(int32 ItemRefId, int32 Num);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	void SetItemData(int32 ItemRefId, int32 Num);

	UFUNCTION()
		void CloseMe();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void OnOpenedPopup();

private:
	TWeakObjectPtr<class UB2UISelectiveLotteryItemSlot> UIP_ItemResult;
	TWeakObjectPtr<UB2Button> BTN_Close;
};

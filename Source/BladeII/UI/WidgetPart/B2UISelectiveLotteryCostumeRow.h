// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UISelectiveLotteryCostumeSlot.h"
#include "B2UISelectiveLotteryCostumeRow.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UISelectiveLotteryCostumeRow : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetLotteryCostumePopup(class UB2UIAllSelectiveLotteryCostumePopup* InLotteryPopup) { LotteryPopup = InLotteryPopup; }
	class UB2UISelectiveLotteryCostumeSlot* SetLotteryCostumeSlotInfo(int32 InIndex, int32 RefId);
	void SetItemCharacterClass(EPCClass InEPCClass) { CharacterClass = InEPCClass; }
	EPCClass GetItemCharacterClass() const { return CharacterClass; }

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void SelectItem(UB2UISelectiveLotteryCostumeSlot* ClickedItem);

private:
	TArray<TWeakObjectPtr<class UB2UISelectiveLotteryCostumeSlot>> LotteryCostumeSlots;

	class UB2UIAllSelectiveLotteryCostumePopup* LotteryPopup;
	EPCClass CharacterClass;
};
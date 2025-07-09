// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupSelectPCClass.generated.h"

class UB2UITeamEntryCharacter;
/*
* Inputable Popup. and it has two buttons.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupSelectPCClass : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()

public:
	UB2UIMsgPopupSelectPCClass(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnOpen(bool RightNow = false) override;

	void OnClickSelectSlotEntry(EPCClass PCClass);
	EPCClass GetSelectPCClass() { return SelectPCClass; }

	void ResponseStartRaid();

	void GoRaidGameMode(const FString& InvitedRoomId, EPCClass ChangeMainPCClass, int32 RaidType, int32 RaidStep);

protected:
	virtual void CacheAssets() override;

protected:
	TArray<TWeakObjectPtr<class UB2UICharacterSelectPageSlot>>	PCClassList;
	TWeakObjectPtr<UTextBlock>									TB_Title;
	TWeakObjectPtr<UTextBlock>									TB_AutoClose;

private:
	EPCClass	SelectPCClass;
	int32		RemainAutoCloseSecond;

	uint8		DeliveryStartRaidTicket;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIBattleNotice.generated.h"


enum class ESlotAnimation : uint8
{
	Idle,
	Up,
	Down,

	Max
};

UCLASS()
class BLADEII_API UB2UIBattleNoticeSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
protected:

	virtual void CacheAssets() override;

public:

	UB2UIBattleNoticeSlot(const FObjectInitializer& ObjectInitializer);

	virtual void AnimateSlot(ESlotAnimation Animation);

	
	virtual void SetText(const FText& SlotText);

	FORCEINLINE bool IsValidSlot() const { return bValidSlot; }

	
	virtual void Show();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void Empty();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnNoticeShow();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnNoticeHide();

protected:

	TWeakObjectPtr<class UB2RichTextBlock>		RTB_BattleText;
	bool bValidSlot;
};

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIBattleNotice : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:

	virtual void CacheAssets() override;

	void Subscribes();
	void Unsubscribes();

public:

	UB2UIBattleNotice(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(UB2UIManager* InUIManager) override;

	virtual void AddSingleEmptySlot();
	virtual void UpdateNoticeText(const FText& Text);
	virtual void OnUpdatedNotice();

	UB2UIBattleNoticeSlot* GetSwapSlotToAdd();
	int32 GetValidSlotCount();

	void OnEtherActivated(int32 EtherSetID, class ABladeIIPlayer* Owner, class ABladeIICharacter* Causer);

	UFUNCTION()
	void ActivatedTimer_NoticeHide();

protected:

	TWeakObjectPtr<UB2ScrollBox>					SB_NoticeSlot;
	TArray<TWeakObjectPtr<UB2UIBattleNoticeSlot>>	NoticeSlots;

	FTimerHandle	NoticeHideHandler;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32	MaxNoticeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoticeSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HideSeconds;
	

	bool bSubscribed;
	uint32 OnEtherActivatedTicket;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIGameInviteList.h"
#include "B2UIGameInviteSlot.generated.h"

DECLARE_DELEGATE_TwoParams(FB2UIInviteRequestFunc, const FText&, int32);
DECLARE_DELEGATE_OneParam(FB2UIInviteStopTimerFunc, const FText&);

enum class EGameInviteSlotIndex : uint8
{
	Invitable,
	Inviting,
	AlreadyInvite,
	DisabledInvite,
	Offline,
	Penalty
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGameInviteSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	const uint8 JOINCOUNTDOWN_SECOND = 20;

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
public:
	UFUNCTION()
	void OnClick_BTNInvite();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void SetSlotState_BP(int32 SlotStateIdx);	// ERaidInviteSlotState

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void SetInviteTimerCount_BP(int32 idx);

public:
	void SetSlotEntryData(FB2CharacterSummaryPtr ptrFriend, int32 RemainInvitingTime = -1, bool bIsPenalty = false);
	void SetSlotEntryData(const FText& CharName, int32 CharLevel, EPCClass PCType, int32 CombatPoint);
	void SetSlotState(EGameInviteSlotIndex State, int32 RemainInvitingTime = -1);
	void UpdateSlotState(const FText& CharName, bool IsOffline, int32 RemainInvitingTime, bool bIsPenalty);
	void StartJoinTimer(int32 RemainInvitingTime = -1);
	void SetSlotIndex(int32 InSlotIndex);

	const FText GetNickName();

private:
	bool IsAlreadyInviteUser(const FText& CharName);
	void UpdateJoinTime();

public:
	TWeakObjectPtr<UImage>					IMG_Character;
	TWeakObjectPtr<UTextBlock>				TB_CharLevel;
	TWeakObjectPtr<UTextBlock>				TB_CharName;
	TWeakObjectPtr<UTextBlock>				TB_CombatPower;
	TWeakObjectPtr<UB2Button>				BTN_Invite;

	TWeakObjectPtr<UTextBlock>				STB_Invite;
	TWeakObjectPtr<UTextBlock>				STB_Invite2;
	TWeakObjectPtr<UTextBlock>				STB_InviteSuccess;
	TWeakObjectPtr<UTextBlock>				STB_CannotInvite;
	TWeakObjectPtr<UTextBlock>				STB_OffLine;
	TWeakObjectPtr<UTextBlock>				STB_RaidBreaker;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInterface*>	CharterTexture;

	// Delegates
public:
	FB2UIInviteRequestFunc			FClickBTNInvite;
	FB2UIInviteStopTimerFunc		FStopInviteTimer;

private:
	FTimerHandle	InviteTimerHandle;
	int32			CurCountDownNum;

	EGameInviteSlotIndex	SlotState;
	int32					SlotIndex;
};



UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRaidInviteList : public UB2UIGameInviteList
{
	GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRaidInviteSlot : public UB2UIGameInviteSlot
{
	GENERATED_BODY()
};

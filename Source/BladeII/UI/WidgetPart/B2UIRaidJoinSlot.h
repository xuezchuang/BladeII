// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "B2UIDocRaid.h"
#include "B2UIRaidJoinSlot.generated.h"

DECLARE_DELEGATE(FB2UIRaidJoinSlot_UpdateSlotState);
DECLARE_DELEGATE_OneParam(FB2UIRaidJoinSlot_OnClickBtnSelect, int32);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRaidJoinSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	// blueprint
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void SetSlotState_BP(int32 idx);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void SetJoinTimerCount_BP(int32 idx);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void SetHost_BP(bool IsHost);

	UFUNCTION()
	void OnClick_BTN_Select();

	UFUNCTION()
	void OnClick_BTN_Close();

	UFUNCTION()
	void OnClick_BTN_Close2();

	// interface
public:
	void SetSlotNum(int32 SlotNum);
	void SetSlotState(ERaidJoinSlotState SlotState);	
	void SetVisibleSelectBtn(bool IsVisible);	
	void SetSlotEntryData(FText CharName, int32 CharLevel, EPCClass PCType, int32 CombatPower, bool IsMyGuild);
	
	bool IsBattleReady();
	void SetIsMySlot(bool InIsMySlot);
	void SetIsVisibleBTNCancel(bool IsEnable);

	// event
public:
	void OnChanagedMySlotIndex(UB2UIDocBase* Sender, int32 CurMySlotIdx, int32 PreMySlotIdx);
	void OnChanagedSlotState(UB2UIDocBase* Sender, ERaidJoinSlotState CurSlotState, ERaidJoinSlotState PreSlotState);
	void OnChanagedPCClass(UB2UIDocBase* Sender, EPCClass PCClass, EPCClass PrevPCClass);
	/*void OnChanagedTotalAttackPoint(UB2UIDocBase* Sender, / *int32 CombatPower,* / int32 PreAttackPoint);
	void OnChanagedTotalDefensePoint(UB2UIDocBase* Sender, / *int32 CombatPower,* / int32 PreDefensePoint);*/
	void OnChanagedCombatPoint(UB2UIDocBase* Sender, int32 CombatPower,  int32 PreDefensePoint); 
	void OnChanagedCharLevel(UB2UIDocBase* Sender, int32 CurLevel, int32 PreLevel);
	void OnChanagedCharName(UB2UIDocBase* Sender, FText CurName, FText PreName);
	void OnChangedMyGuildMember(UB2UIDocBase* Sender, bool CurMyGuild, bool PreMyGuild);

protected:
	TWeakObjectPtr<UB2Button>				BTN_Select;
	TWeakObjectPtr<UImage>					IMG_MainCharacter;
	TWeakObjectPtr<UImage>					IMG_Me;
	TWeakObjectPtr<UTextBlock>				TBS_CharLevel;
	TWeakObjectPtr<UTextBlock>				TBS_CharName;
	TWeakObjectPtr<UB2Button>				BT_Close_01;
	TWeakObjectPtr<UB2Button>				BT_Close_02;

	TWeakObjectPtr<UTextBlock>				TB_CombatPower;
	TWeakObjectPtr<UTextBlock>				STB_Invite;
	TWeakObjectPtr<UTextBlock>				STB_Invite2;
	TWeakObjectPtr<UTextBlock>				STB_Search;

	TWeakObjectPtr<UTextBlock>				STB_GuildMan;
	TWeakObjectPtr<UPanelWidget>			P_GuildMan;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInterface*>	CharterTexture;

	// delegates
public:
	FB2UIRaidJoinSlot_UpdateSlotState			FUpdateSlot;
	FB2UIRaidJoinSlot_OnClickBtnSelect			FClickBTNSelect;

protected:
	bool				IsMySlot;
	
	int32				SlotNum;
	ERaidJoinSlotState	SlotState;
};

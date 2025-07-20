// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildBuffWidget.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildBuffWidget : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void CloseWidgetDelegate() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;
	void SubscribeEvents();
	void UnsubscribeEvents();

	void ResponseUpdate(b2network::B2GuildSkillPtr InSkill, int32  InGold);

protected:
	UFUNCTION()
		void OnClickBTN_Close();
	UFUNCTION()
		void OnClickBTN_BuffLevelUp();
	UFUNCTION()
		void OnClickBTN_BuffBuy();
	UFUNCTION()
		void OnClickBTN_OneBTNBuy();

	UFUNCTION(BlueprintImplementableEvent)
		void LevelUpPlaySoundEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void BuyPlaySoundEvent();

	void UpdateGuildSkillInfo(int32 SlotNumber);
	void UpdateGuildSkillInfo(int32 InSkillId, int32 InLv);

	void SetGuildSkillBuffText(EItemOption BuffOption, float Param1, float Param2, float Param3);

	void UpdateHeroCombatPower();
	void BuyOneBTNBuff();

	int32 GetOneBTNPrice();

private:
	void OnDelayBuffLevelUpCallback();

private:
	TWeakObjectPtr<UB2Button> BTN_Close;
	TArray<TWeakObjectPtr<class UB2UIGuildSkillSlot>>		GuildSkillSlotArr;


	TWeakObjectPtr<UImage>					IMG_Buff;
	TWeakObjectPtr<UTextBlock>				TB_BuffText;
	TWeakObjectPtr<UTextBlock>				TB_BuffLv;
	TWeakObjectPtr<UB2RichTextBlock>		TB_CurrentEffect;
	TWeakObjectPtr<UB2RichTextBlock>		TB_NextEffect;


	TWeakObjectPtr<UB2Button>				BTN_BuffLevelUp;
	TWeakObjectPtr<UB2Button>				BTN_BuffBuy;
	TWeakObjectPtr<UB2Button>				BTN_OneBTNBuy;

	TWeakObjectPtr<UTextBlock>				TB_LvelUpCost;
	TWeakObjectPtr<UTextBlock>				TB_BuffBuyCost;

	TWeakObjectPtr<UPanelWidget>			CP_BuffBuy;
	TWeakObjectPtr<UPanelWidget>			CP_OneMore;
	TWeakObjectPtr<UPanelWidget>			CP_OneBTNBuy;

	//Static Text
	TWeakObjectPtr<UTextBlock>				TB_GuildBuffNotice;
	TWeakObjectPtr<UTextBlock>				TB_BuffActive;
	TWeakObjectPtr<UTextBlock>				TB_GuildBuff;

	TWeakObjectPtr<UTextBlock>				TB_BuffEffect;
	TWeakObjectPtr<UTextBlock>				TB_CurrentOption;
	TWeakObjectPtr<UTextBlock>				TB_NextOption;
	TWeakObjectPtr<UTextBlock>				TB_LevelUp;
	TWeakObjectPtr<UTextBlock>				TB_Buy;
	TWeakObjectPtr<UTextBlock>				TB_OneBTNBuy;
	TWeakObjectPtr<UTextBlock>				TB_OneBTNCost;
	TWeakObjectPtr<UTextBlock>				TB_LevelRequire;

	TWeakObjectPtr<UScaleBox>				SB_OneBTNBuy;


private:
	int32 SelectSkillID;
	int32 purchasableBuffCount = 3;
	int32 oneBTNBuyNumber = 6;
	int32 countAbleHour = 2;	//概 滚橇付促 棵扼啊绰 矫埃(Hr)

	int32 purchasedBuffCount = 0;
	int32 oneBTNBuyCount = 1;

	TArray<std::shared_ptr<class issue>>	Issues;
	FTimerHandle DelayCallbackTH;
};

// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIGuildSkillSlot.generated.h"


DECLARE_DELEGATE_OneParam(FGuildSkillInfoDelegate, int32)
DECLARE_DELEGATE_TwoParams(FGuildBuffWidgetUpdateDelegate,  int32, int32)

UCLASS()
class BLADEII_API UB2UIGuildSkillSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		int32 SlotNumber;

public:
	void SetGuildSkillInfoDelegate(FGuildSkillInfoDelegate InGuildSkilldelegate);
	void SetGuildBuffWidgetUpdateDelegate(FGuildBuffWidgetUpdateDelegate InGuildBuffdelegate);
	void OnSelectedSlot(bool bSelected);
	void SetSlotInfo(const b2network::B2GuildSkillPtr& SkillInfo, int32 InSkillID);

	void SetBuffText(EItemOption BuffOption);
	void SetGuildLvText(int32 Lv , int64 BuffEndTime);
	void SetBuffLvText(int32 Lv, int64 BuffEndTime);
	void SetUseOnBuff(int32 InBuffTime, int32 InSkillLv);
	void RemainTime(int64 timeTick);

	int32 GetSkillID() { return SkillID; }
	int32 GetSkillLv() { return SkillLv; }
	int64 GetRemainTime() { return EndTime; }

	//return value: 1: off, 2: on, 3: using
	int32 GetSkillState();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayShotAni();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayLoopAni();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayLvUpAni();


	UFUNCTION()
		void OnClickSelect();

protected:

	TWeakObjectPtr<UImage>				IMG_OnBuffIcon;
	TWeakObjectPtr<UTextBlock>			TB_OnBuffName;
	TWeakObjectPtr<UTextBlock>			TB_OnBuffLv;
	TWeakObjectPtr<UTextBlock>			TB_OnRemainTime;

	TWeakObjectPtr<UPanelWidget>		OV_OffBuff;

	TWeakObjectPtr<UImage>				IMG_OffBuffIcon;
	TWeakObjectPtr<UTextBlock>			TB_OffBuffName;
	TWeakObjectPtr<UTextBlock>			TB_OffBuffLv;

	TWeakObjectPtr<UPanelWidget>		OV_OnBuff;
	TWeakObjectPtr<UTextBlock>			TB_BuffTime;

	TWeakObjectPtr<UB2Button>			BTN_Select;

	TWeakObjectPtr<UPanelWidget>		OV_Select;

	TWeakObjectPtr<UPanelWidget>		OV_UseOnBuff;
	TWeakObjectPtr<UImage>				IMG_UseOnBuffIcon;
	TWeakObjectPtr<UTextBlock>			TB_UseOnBuffName;
	TWeakObjectPtr<UTextBlock>			TB_UseOnBuffLv;
	TWeakObjectPtr<UTextBlock>			TB_AvailableMaxTime;


	FGuildSkillInfoDelegate				GuildSkillInfoDelegate;
	FGuildBuffWidgetUpdateDelegate		GuildBuffWidgetUpdateDelegate;


private:
	int64 BuffTime;
	int32 SkillID;
	int32 SkillLv;

	int64 EndTime=0;

};

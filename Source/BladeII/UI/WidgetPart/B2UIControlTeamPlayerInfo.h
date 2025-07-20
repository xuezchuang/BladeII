// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIControlTeamPlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlTeamPlayerInfo : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	UB2UIControlTeamPlayerInfo(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetPlayer(class ABladeIINetPlayer* pPlayer);
	void SetKillCount(int32 nCount);

	void SetRemainResurrectTime(float fRemainTime);

	ABladeIINetPlayer* GetTargetPlayer();
	uint32 GetTargetPlayerNetId();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_NormalState();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_DeadState();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_LocalPlayerState();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_Intrusion();

	UFUNCTION(BlueprintImplementableEvent)
	void AddMessageWidget(class UB2UIWidget* pWidget);

protected:
	virtual void CacheAssets() override;

private:
	void UpdateRemainResurrectTime(float InDeltaTime);
	
protected:
	TWeakObjectPtr<UTextBlock> TB_LvName;
	TWeakObjectPtr<UTextBlock> TB_KillInfo;
	TWeakObjectPtr<UImage> IMG_CharacterImage;
	TWeakObjectPtr<UTextBlock> TB_RemainResurrectTime;
	
	TWeakObjectPtr<UProgressBar> PB_HP_R;
	TWeakObjectPtr<UProgressBar> PB_HP_B;

	UProgressBar* CurrentHpBar;
	
private:
	TWeakObjectPtr<class ABladeIINetPlayer> TargetPlayer;
	uint32 TargetPlayerNetId;

	bool IsPlayerAlive;

	float m_fRemainResurrectTime;
	
};

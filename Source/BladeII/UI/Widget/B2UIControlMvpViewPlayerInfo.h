// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIControlMvpViewPlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlMvpViewPlayerInfo : public UB2UIWidget
{
	GENERATED_BODY()
public:
	UB2UIControlMvpViewPlayerInfo(const FObjectInitializer& ObjectInitializer);

	void SetTargetActor(class AActor* TargetActor);
	void SetPlayer(class ABladeIIPlayer* pPlayer);
	void SetContribution(int32 InContribution);
	void SetMvp();
	void SetHiddenKillCount();
	void SetVisibleContribution();

	void SetAskFriendBtnHidden();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickAskFriend();

protected:
	TWeakObjectPtr<UB2RichTextBlock> TB_LvNickName;
	TWeakObjectPtr<UTextBlock> TB_KillCount;
	TWeakObjectPtr<UTextBlock> TB_Contribution;
	TWeakObjectPtr<UCanvasPanel> C_Mvp;
	TWeakObjectPtr<UB2Button> BTN_AskFriend;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
		FVector2D AdjustPosition;
	
public:
	int32 m_nKillCount;
	uint32 m_nPlayerNetId;

private:
	class AActor* m_pTargetActor;
};

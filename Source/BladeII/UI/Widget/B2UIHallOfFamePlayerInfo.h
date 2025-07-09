// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIHallOfFamePlayerInfo.generated.h"

UENUM(BlueprintType)
enum class EHallOfFameClass : uint8
{
	FIRST_Gladiator = 0,
	FIRST_Assassin,
	FIRST_Wizard,
	FIRST_Fighter,
	SECOND,
	THIRD,
	END,
};

UCLASS()
class BLADEII_API UB2UIHallOfFamePlayerInfo : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
		void OnClick_Praise();
	UFUNCTION()
		void OnClick_RankerInfo();

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	EHallOfFameClass GetHallOfFameClass(EPCClass InPcClass, int32 InRanking);

	void SetVisible(bool InVisible);
	void SetVisiblePraise(bool InPraiseButton, bool InPraiseCompleteWidget);
	void SetUserInfo(const FRankerInfo& RankerInfo);
	void SetAdjustPosition();
	void SetTargetActor(class AActor* InTargetActor) { TargetActor = InTargetActor; }
	void UpdatePosition(bool InForce = false);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	class AActor* TargetActor;
	int32 Ranking;
	int64 AccountID;
	int32 MatchMode;
	EPCClass CharacterClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
		TMap<EHallOfFameClass, FVector2D> AdjustPositions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
		FVector2D AdjustPosition;

	TWeakObjectPtr<UButton>         BTN_Praise;
	TWeakObjectPtr<UB2Button>		BTN_OpenRankerInfo;

	TWeakObjectPtr<UTextBlock>		TB_UserName;
	TWeakObjectPtr<UTextBlock>		TB_PraiseCount;
	TWeakObjectPtr<UTextBlock>		TB_AttackPower;
	TWeakObjectPtr<UTextBlock>		TB_MVPNUM;

	TWeakObjectPtr<UPanelWidget>    OV_Ranking1;
	TWeakObjectPtr<UPanelWidget>    OV_Ranking2;
	TWeakObjectPtr<UPanelWidget>    OV_Ranking3;
	TWeakObjectPtr<UPanelWidget>    OV_MVP;

	TWeakObjectPtr<UImage>			IMG_ConfirmPraise;
	TWeakObjectPtr<UImage>			IMG_ConfirmPraiseEffect;

	TWeakObjectPtr<UWidgetAnimation>	ANIM_Open;
	TWeakObjectPtr<UWidgetAnimation>	ANIM_Like;

	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_PVPGrade_Middle;
};

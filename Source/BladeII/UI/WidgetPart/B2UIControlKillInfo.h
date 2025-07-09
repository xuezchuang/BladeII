// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIUtil.h"
#include "B2UIControlKillInfo.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlKillInfo : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	UB2UIControlKillInfo(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetKillInfo(ETeamType KillerTeam, FText& txtKiller, FText& txtDeadMan, EPCClass KillerClass, EPCClass DeadManClass);
	
protected:
	virtual void CacheAssets() override;
	
protected:
	TWeakObjectPtr<UTextBlock> TB_Killer;
	TWeakObjectPtr<UImage> IMG_KillerFace;
	TWeakObjectPtr<UImage> IMG_KillerDeco;
	
	TWeakObjectPtr<UTextBlock> TB_DeadMan;
	TWeakObjectPtr<UImage> IMG_DeadFace;
	TWeakObjectPtr<UImage> IMG_DeadDeco;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlKill")
	float LifeTime;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> PortraitInfo;

	UPROPERTY(EditAnywhere)
	FSlateColor ColorRedTeamDeco;

	UPROPERTY(EditAnywhere)
	FSlateColor ColorRedTeamText;

	UPROPERTY(EditAnywhere)
	FSlateColor ColorBlueTeamDeco;

	UPROPERTY(EditAnywhere)
	FSlateColor ColorBlueTeamText;

private:
	float m_fStartTime;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "EventSubsystem.h"
#include "B2UIWealthPopupSlot.generated.h"

UCLASS()
class BLADEII_API UB2UIWealthPopupSlot : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CacheAssets() override;

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

public:
	void InitWealthPopupSlot(const EWEALTH_TYPE& eWealthType, const int32& MaxCount);
	void UpdateWealthPopupSlot();

private:
	void UpdateWealthText(const int32 ProgressCount, const int32 MaxCount);

private:
	int64 GetProgressCount(const EWEALTH_TYPE& eWealthType);

private:
	int64 GetProgressCount_Gem();
	int64 GetProgressCount_DarkCrystal();
	int64 GetProgressCount_Gold();
	int64 GetProgressCount_Social();
	int64 GetProgressCount_BladePoint();
	int64 GetProgressCount_RestTime();
	int64 GetProgressCount_StageBoostTicket();
	int64 GetProgressCount_Pvp1on1Ticket();
	int64 GetProgressCount_TeamMatchTicket();
	int64 GetProgressCount_HeroTowerTicket();
	int64 GetProgressCount_FamePoint(); 
	int64 GetProgressCount_StageAutoClearTicket();

public:
	void SetWealthImageFromMaterial(UMaterialInterface* pMaterial);

private:
	TWeakObjectPtr<UImage>		IMG_Wealth;
	TWeakObjectPtr<UTextBlock>	TB_Count;

private:
	EWEALTH_TYPE				CachedWealthType;
	int32						CachedMaxCount;

private:
	bool				IsSubscribe;
	TArray<issue_ptr>	Issues;
};

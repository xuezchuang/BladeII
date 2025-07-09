// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIGuildBattleHistory.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildBattleHistory : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void OpenSetting(const FB2ResponseGuildBattleHistoryPtr& HistoryData);
protected:
	UFUNCTION()
	void OnClickBTN_Close();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TSubclassOf<class UB2UIGuildBattleHistorySlot>		GuildRankSlot;

private:
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<class UB2UIGuildBattleHistoryRecord>		UIP_GuildWarSeasonRecord;
	TWeakObjectPtr<class UB2UIGuildBattleHistoryRecord>		UIP_GuildWarBestSeasonRecord;
	TWeakObjectPtr<UVerticalBox>							VB_List;
};

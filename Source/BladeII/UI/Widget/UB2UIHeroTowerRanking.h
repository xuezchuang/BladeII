// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "UB2UIHeroTowerRanking.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UUB2UIHeroTowerRanking : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	
	UFUNCTION()
	void OnClickBtnClose();

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetFriendList(const FB2ResponseGetFriendRankingsPtr& FriendRank);

	void UpdateList();

	virtual void OnClose(bool RightNow = false) override;
	virtual void UpdateStaticText() override;

	UFUNCTION()
	virtual void OnOpen(bool RightNow = false) override;

	UFUNCTION()
	void OnClick_AllRanking();
	UFUNCTION()
	void OnClick_FriendRanking();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RankerInfo)
	TSubclassOf<class UB2UIHeroTowerRankerInfo> RankerInfoClass;

protected:
	virtual void CloseWidgetDelegate() override;

	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UOverlay>		OV_AllRanking;
	TWeakObjectPtr<UOverlay>		OV_AllRanking2;
	TWeakObjectPtr<UOverlay>		OV_FriendRanking;
	TWeakObjectPtr<UOverlay>		OV_FriendRanking2;

	TWeakObjectPtr<UB2Button>		BTN_AllRank;
	TWeakObjectPtr<UB2Button>		BTN_FriendRank;

	TWeakObjectPtr<UVerticalBox>	VB_RankingList;

	TWeakObjectPtr<UCanvasPanel> CP_Ranking;

	TWeakObjectPtr<UTextBlock> STB_Title;
	TWeakObjectPtr<UTextBlock> STB_AllRank;
	TWeakObjectPtr<UTextBlock> STB_AllRank2;
	TWeakObjectPtr<UTextBlock> STB_FriendRank;
	TWeakObjectPtr<UTextBlock> STB_FriendRank2;

	TWeakObjectPtr<UTextBlock>	TB_HeroRanking;

private:
	TArray<TWeakObjectPtr<class UB2UIHeroTowerRankerInfo>> RankerList;
	TArray<FModRankerInfo> FriendRankingList;

	int32 DeliverytGetFriendRankingListTicket;

	bool bShowingAllRanker;
};

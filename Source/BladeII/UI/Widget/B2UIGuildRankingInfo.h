// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIGuildRankingInfo.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildRankingInfo : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetGuildRanking();

	void SetSuarchList(const FB2ResponseFindGuildPtr& GuildInfo);
	void SetTapGuildRanking();
	void SetTapRankingReward();

	void RankingListReceive(const FB2ResponseGetGuildRankingListPtr& RankingInfo);

public:
	virtual void OnClose(bool RightNow = false) override;

	UFUNCTION()
		virtual void OnOpen(bool RightNow = false) override;
	UFUNCTION()
		void OnClickGuildRanking();

	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnClickSuarch();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TSubclassOf<class UB2UIGuildInfoSlot>		GuildRankSlot;

protected:


	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UB2Button>		BTN_GuildRank;
	TWeakObjectPtr<UB2Button>		BTN_RankReward;
	TWeakObjectPtr<UB2Button>		BTN_Surch;
	TWeakObjectPtr<UEditableTextBox>	ETB_SearchGuild;


	TWeakObjectPtr<class UB2UIGuildInfoSlot>	UIP_MyGuildInfoSlotRank;


	TWeakObjectPtr<UVerticalBox>				VB_RankingList;
	TWeakObjectPtr<UVerticalBox>				VB_LeftReward;
	TWeakObjectPtr<UVerticalBox>				VB_RightReward;


	TWeakObjectPtr<UCanvasPanel> CP_GuildRanking;
	TWeakObjectPtr<UCanvasPanel> CP_RankingReward;

private:

	TArray<b2network::B2GuildRankingPtr> RankingArr;
	int32 DeliveryFindGuildTicket;
	int32 DeliverytGetGuildRankingListTicket;
};

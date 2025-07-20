#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIGuildSlotBar.generated.h"


DECLARE_DELEGATE_OneParam(FGuildDetail, int64)
DECLARE_DELEGATE_OneParam(FGuildSlotDelect, class UB2UIGuildInfoSlot*)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildInfoSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetGuildInfo(FB2ResponseGuildInfoPtr GuildInfo);
	void SetGuildRankingInfo(b2network::B2GuildRankingPtr RankingInfo);
	void SetGuildDelegate(FGuildDetail Guilddelegate);
	void SetGuildDelectDelegate(FGuildSlotDelect GuildDelegate);
	void SetRank(int32 Rank);
	void SetRemainTime(int64 InTime);

	virtual void UpdateStaticText() override;

	UFUNCTION()
		void OnClickGuildDetail();

	UFUNCTION()
		void OnClickGuildDeny();

protected:
	TWeakObjectPtr<class UB2UIGuildInfoPart1>		UIP_GuildInfoPart1;
	TWeakObjectPtr<class UB2UIGuildInfoPart2>		UIP_GuildInfoPart2;
	TWeakObjectPtr<class UB2UIGuildInfoPart3>		UIP_GuildInfoPart3;

	TWeakObjectPtr<UB2Button>						BTN_Join;
	TWeakObjectPtr<UB2Button>						BTN_Deny;
	TWeakObjectPtr<class UB2RichTextBlock>			RTB_JoinText;
	TWeakObjectPtr<class UB2RichTextBlock>			RTB_JoinTime;
	TWeakObjectPtr<UTextBlock>						TB_JoinTime;

	TWeakObjectPtr<class UB2RichTextBlock>			RTB_GuildRank;
	TWeakObjectPtr<UTextBlock>						TB_Reject;
	TWeakObjectPtr<UTextBlock>						TB_Join;


	TWeakObjectPtr<UOverlay>						OV_Rank1;
	TWeakObjectPtr<UOverlay>						OV_Rank2;
	TWeakObjectPtr<UOverlay>						OV_Rank3;

private:
	FGuildDetail					DelegateGuildDetailInfo;
	FGuildSlotDelect				DelegateGuildSlotDelect;
	int64							GuildID;

	int64							DeliveryRejectInviteGuildTicket;
};

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UICommonTab.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildJoinList.generated.h"


enum GuildJoinListType
{
	GUILD_Recommend,
	GUILD_Create,
	GUILD_Accept,
};

class UB2UIGuildCreate;

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildJoinList : public UB2UIWidget
{
	GENERATED_BODY()

public:
	~UB2UIGuildJoinList();
public:
	virtual void Init() override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void UpdateStaticText() override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	
private:
	int64 DeliveryReceiveGetRecommendGuildTicket;
	int64 DeliveryReceiveGetAcceptGuildTicket;
	int64 DeliveryFindGuildTicket;

public:
	void SetRecommendGuild(const TArray<FB2ResponseGuildInfoPtr>& RecommendGuildArr);
	void SetAcceptGuild(const TArray<b2network::B2GuildInvitePtr>& InvitePtr);
	void FindGuild(const FB2ResponseFindGuildPtr& GuildInfo);


	void SetRecommendTab();
	

	UFUNCTION()
		void OnClickRecommendGuildTap();

	UFUNCTION()
		void OnClickAcceptGuildTap();

	UFUNCTION()
		void OnClickCreateGuildTap();

	UFUNCTION()
		void EnterGuildDetailInfo(int64 GuildID);

	UFUNCTION()
		void DelectGuildSlot(class UB2UIGuildInfoSlot* AcceptSlot);

	UFUNCTION()
		void OnClickSearchGuild();

protected:

	TWeakObjectPtr<UB2UIGuildCreate>			UIP_GuildCreate;


	TWeakObjectPtr<UB2Button>			BTN_SearchGuild;
	TWeakObjectPtr<UEditableTextBox>	ETB_SearchGuild;

	TWeakObjectPtr<UVerticalBox>		VB_List;
	TWeakObjectPtr<UVerticalBox>		VB_AcceptList;

	TWeakObjectPtr<UCanvasPanel>		CP_Accept;
	TWeakObjectPtr<UB2RichTextBlock>	TB_AcceptMsg;
	TWeakObjectPtr<UB2RichTextBlock>	TB_AcceptNumber;
	TWeakObjectPtr<UTextBlock>			TB_GuildSearch;
	TWeakObjectPtr<UTextBlock>			TB_GuildJoinText;

	TWeakObjectPtr<class UB2UICommonTab>	UI_Common_Tap_AcceptGuild;
	TWeakObjectPtr<class UB2UICommonTab>	UI_Common_Tap_Recommend;
	TWeakObjectPtr<class UB2UICommonTab>	UI_Common_Tap_CreateGuild;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VectorParameterValue)
		TSubclassOf<class UB2UIGuildInfoSlot>		RecommendGuild;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VectorParameterValue)
		TSubclassOf<class  UB2UIGuildInfoSlot>		AcceptGuild;

public:
	GuildJoinListType							TapType;
};

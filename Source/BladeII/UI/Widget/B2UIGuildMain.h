#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIGuildMain.generated.h"

UENUM(BlueprintType)
enum class EGuildInfoType : uint8
{
	MyGuild,
	OtherGuild,
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildMain : public UB2UIWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnHistoryOpen(bool IsHistory) override;
	virtual void OnClose(bool RightNow = false) override;

	void GuildMainPageUpdate();

	void ReceivedGuildUpdate(const FB2ResponseUpdateGuildPtr& GuildInfo);

	void SetGuildMain(EGuildInfoType type, const FGuildInfo& GuildInfo);
	void SetGuildInfo(const FB2ResponseGuildInfoPtr GuildInfo);
	void SetGuildBattleCurrentState();
	void SetOtherGuildMemberList(const TArray<b2network::B2GuildMemberPtr>& RecommendGuildArr);
	void SetRewardComplete(bool IsReward);
	void SetGuildSchedule();

	void WriteNoticeState();
	void SaveNoticeState();
	void SetRewardBoxProgress();

	void NotApplyCationPopup();

	UFUNCTION()
		void OnClickGuildListInfo();
	UFUNCTION()
		void OnClickGuildDropOut();
	UFUNCTION()
		void OnClickJoinGuild();
	UFUNCTION()
		void OnClickGuildWarReady();
	UFUNCTION()
		void OnClickGuildWar();
	UFUNCTION()
		void OnClickSeasonInfo();
	UFUNCTION()
		void SendDropOut();
	UFUNCTION()
		void SendJoinGuild();
	UFUNCTION()
		void OnClickAni();
	UFUNCTION()
		void OnClickInvite();
	UFUNCTION()
		void OnClickGuildRanking();
	UFUNCTION()
		void OnClickGuildWarInfo();
	UFUNCTION()
		void OnNoticeTextChange(const FText& GuildNotice, ETextCommit::Type CommitMethod);
	UFUNCTION()
		void OnClickSaveNotice();
	UFUNCTION()
		void OnClickGuildEdit();
	UFUNCTION()
		void OnClickBTN_GuildBattleProgess();
	UFUNCTION()
		void OnClickBTN_Mercenary();

	UFUNCTION(BlueprintCallable, Category = BladeII)
		void SetHideAni(UWidgetAnimation* Ani);
	UFUNCTION(BlueprintImplementableEvent)
		void SetPlayAnimationEvent(int32 Aninumber);
	UFUNCTION(BlueprintImplementableEvent)
		void OpenPlayAnimationEvent();

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

protected:
	TWeakObjectPtr<UB2Button>					BTN_GuildListInfo;
	TWeakObjectPtr<UB2Button>					BTN_GuildRank;
	TWeakObjectPtr<UB2Button>					BTN_GuildWarInfo;
	TWeakObjectPtr<UB2Button>					BTN_GuildWarReady;
	TWeakObjectPtr<UB2Button>					BTN_GuildWar;
	TWeakObjectPtr<UB2Button>					BTN_GuildBuff;
	TWeakObjectPtr<UB2Button>					BTN_Mercenary;

	TWeakObjectPtr<class UB2UIGuildMark>		UIP_GuildMark;
	TWeakObjectPtr<class UB2RichTextBlock>		RTB_GuildName;
	TWeakObjectPtr<class UB2RichTextBlock>		RTB_GuildNumber;
	TWeakObjectPtr<class UB2RichTextBlock>		RTB_GuildScore;
	TWeakObjectPtr<class UB2RichTextBlock>		RTB_GuildRank;


	TWeakObjectPtr<UTextBlock>					TB_GuildLevel;
	TWeakObjectPtr<UProgressBar>				PB_GuildProgress;
	TWeakObjectPtr<UTextBlock>					TB_LevelPercent;

	TWeakObjectPtr<UImage>						IMG_LevelMax;
	
	TWeakObjectPtr<class UB2RichTextBlock>		RTB_GuildNotice;
	TWeakObjectPtr<UB2Button>					BTN_Invite;
	TWeakObjectPtr<UB2Button>					BTN_GuildEdit;
	TWeakObjectPtr<UB2Button>					BTM_DropOut;
	TWeakObjectPtr<UB2Button>					BTN_GuildJoin;
	TWeakObjectPtr<UB2Button>					BTN_GuildNotice;
	TWeakObjectPtr<UB2Button>					BTN_Ani;

	TWeakObjectPtr<UB2Button>					BTN_GuildBattleProgess;
	TWeakObjectPtr<UB2RichTextBlock>					TB_GuildBattleState;
	
	TWeakObjectPtr<UCanvasPanel>				CP_MyGuildMenu;
	TWeakObjectPtr<UCanvasPanel>				CP_OtherGuildMenu;
	TWeakObjectPtr<UCanvasPanel>				CP_MyGuildBtn;
	TWeakObjectPtr<UCanvasPanel>				CP_GuildJoin;

	TWeakObjectPtr<UPanelWidget>				OV_Schedule;

	TWeakObjectPtr<UVerticalBox>				VB_MemberList;

	TWeakObjectPtr<class UB2UIGuildSeasonReward>	UI_GuildSeasonReward;


	TWeakObjectPtr<class UB2UIGuildMatchPopup>	UI_GuildMatchPopup;

	TWeakObjectPtr<UTextBlock>					TB_GuildCombat;

	TWeakObjectPtr<UB2Button>					BTN_GuildNoticePanel;
	TWeakObjectPtr<UEditableTextBox>			ETB_GuildNotice;
	TWeakObjectPtr<UB2Button>					BTN_WriteNotice;
	TWeakObjectPtr<UB2Button>					BTN_SaveNotice;

	TWeakObjectPtr<UPanelWidget>				X_NoticePanel;

	TWeakObjectPtr<class UB2UIGuildRewardBoxProgress>	UIP_GuildRewardBoxProgress;
	TWeakObjectPtr<class UB2UIGuildSchedule>			UIP_GuildSchedule;


	TWeakObjectPtr<UPanelWidget>				OV_HistoryImage;
	TWeakObjectPtr<UPanelWidget>				OV_GuildRankingImage;
	TWeakObjectPtr<UPanelWidget>				OV_GuildMemberInfoImage;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_WinCount;

	//Static Text
	TWeakObjectPtr<UB2RichTextBlock>				RTB_GuildMemberInfo;
	TWeakObjectPtr<UTextBlock>						TB_GuildJoin;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_MainGuildInfo;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_MainGuildRank;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_MainWarInfo;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_MainGuildScore;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_MainGuildCombat;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_MainGuildNotice;

	TWeakObjectPtr<UTextBlock>						TB_Save;
	TWeakObjectPtr<UTextBlock>						TB_Write;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_GuildBuff;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_GuildWarReady;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_Leave;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_Edit;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_Invite;



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TSubclassOf<class UB2UIGuildMemberSlot>		UIP_GuildMemberInfoSlot;
private:
	int32 GuildBattleCurrentState;

	int64  GuildID;
	int64  DeliveryInviteTicket;
	int64  DeliveryGuildNoticeTicket;
	int64  DeliveryUpdateGuildTicket;
	int64 DeliveryGuildBattleTurnRewardTicket;
	bool	AniSlotHide;
};
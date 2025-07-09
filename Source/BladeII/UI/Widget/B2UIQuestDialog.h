#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "Quest.h"
#include "MissionManager.h"
#include "B2UIBackWidget.h"
#include "B2UIQuestDialog.generated.h"

DECLARE_DELEGATE(FOnQuestDialogClosed);

enum class EDialogType : uint8
{
	Quest,
	Mission,
	Max
};

enum class EQuestRewardButtonType : uint8
{
	Shortcut,
	GetReward,
	GetAchivement,
	GiveUp,
	Max
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIQuestDialog : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	UB2UIQuestDialog(const FObjectInitializer& ObjectInitializer);
	void OpenDialogBySlotIndex(int32 QuestSlot);
	void SetOpenQuestID(int32 id);
	void SetRewardQuestID(int32 id);
	void SetCloseQuestID(int32 id);
	void SetQuestTitleText(const FText& TitleText);
	void SetQuestNameText(const FText& TitleText);
	void SetQuestNameText(const FText& TitleText, const FString& Strnum);
	void SetDialogText(int32 index);
	void SetNpc(int32 index);
	void SetRewardItem();
	void SetQuestCompleteMessage();
	
	void UpdateRewardItem(EDialogType InDialogType);
	void FinalApplyDialogText(bool IsStringFilledUp = false);
	void SetDisplayServerReward(const TArray<b2network::B2RewardPtr>& ServerRewardInfo);
	void SetImmediateCompletedMode(bool Immediated);
	void UpdateDialog();
	void OpenDialog(bool CheckCompleteQuestNotice = true);
	void OpenRewardDialog(const class Quest& QuestRef, const TArray<b2network::B2RewardPtr>& ServerRewardInfo);
	void SetNextMainQuest(QuestPtr NextMainQuest);
	void SetDialogCloseDelegate(FOnQuestDialogClosed&& CloseDelegate);
	void OpenMissionCompleteDialog(int32 InMissionID);
	void SetMissionDialogText(int32 InMissionID);
	int32 GenerateGoalCount(MissionPtr Mission);
	void AddRewardIcon(ERewardType Type, int32 Count);
	void AddRewardIcon(const int InRewardId, int32 Count);
	void SetRewardButtonType(EQuestRewardButtonType RewardButtonType);
	void SetVisibilityRewardButtonType(EQuestRewardButtonType RewardButtonType);

	UFUNCTION()
	void OnClickDialogueNext();
	UFUNCTION()
	void OnClickConfirm();
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickSkip();
	UFUNCTION()
		void OnClickGiveUp();

	void OnClickMsgReward();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<TSoftObjectPtr<UTexture2D>>		NPCImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString					ProgressColorHex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface*		RewardPointMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float					ShowCompleteNoticeDelayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float					TotalFlowTime = 5;
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void CloseWidgetDelegate() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager);

private:
	void OpenAutoClearQuest();
	void SetToggleQuestTitleWidget(bool IsVisibleTitle);
	void SetPlaySound(bool IsStartMode);
	USoundCue* GetNpcSound(bool IsStartMode);
	void StopCurrentDialogSound();
	UTexture2D* GetNPCImageTexture(int32 InIndex);
	void UpdateManualTimeCounter();

	TArray<FString>			DialogArr;
	int32					QuestSlotIndex;
	int32					QuestID;
	int32					QuestType;
	int32					PageCount;
	uint8					DivisionTable;
	uint8					DivisionStat;
	TArray<int32>			RewardItemIDList;
	TArray<int32>			RewardItemCounts;
	float					ManualLastRealTime;
	float					ManualRealDeltaTime;
	float					MaxCharShowInterval;	
	bool					bShowServerRewardItem;
	bool					IsInStageMode;
	QuestPtr				NextMainQuestPtr;
	int32					NextMainQuestSlot;	
	FOnQuestDialogClosed	OnQuestDialogDelegate;
	EDialogType				DialogType;
	int32					CurrRewardIconIdx;
	int32					MissionID;
	EQuestRewardButtonType	CurRewardButtonType;
	TArray<b2network::B2RewardPtr>			RewardArr;
	TArray<b2network::B2RewardPtr>			RewardInfo;
	TWeakObjectPtr<class UB2RichTextBlock>			RTB_NpcName;
	TWeakObjectPtr<class UB2RichTextBlock>			RTB_QuestTitle;
	TWeakObjectPtr<class UB2RichTextBlock>			RTB_QuestName1;
	TWeakObjectPtr<class UB2RichTextBlock>			RTB_QuestName2;
	TWeakObjectPtr<class UB2RichTextBlock>			RTB_Dialog;
	TWeakObjectPtr<UHorizontalBox>					HB_QuestName;
	TWeakObjectPtr<UOverlay>						O_Dialog;
	TWeakObjectPtr<UOverlay>						O_Reward;
	TWeakObjectPtr<UImage>							IMG_Npc;
	TWeakObjectPtr<UButton>							BTN_Dialogue_Next;
	TWeakObjectPtr<UImage>							IMG_Dialogue_Next;
	TWeakObjectPtr<UButton>							BTN_Result;
	TWeakObjectPtr<UButton>							BTN_Shortcut;
	TWeakObjectPtr<UTextBlock>						TB_SuccessReward;
	TWeakObjectPtr<UTextBlock>						TB_ConfirmText;
	TWeakObjectPtr<UTextBlock>						TB_ConfirmText2;
	TWeakObjectPtr<UButton>							BTN_Close;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_RewardIcon1;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_RewardIcon2;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_RewardIcon3;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_RewardIcon4;
	TArray<TWeakObjectPtr<class UB2UIRewardIcon>>	RewardIcons;
	TWeakObjectPtr<class UB2UIRewardNotice>			QuestCompleteNotice;
	FProgressiveTextBuilder							ProgressiveDialogText;
	TWeakObjectPtr<UB2Button> BTN_GiveUp;
	TWeakObjectPtr<UTextBlock> TB_GiveUp;
	UPROPERTY(Transient)
		class UAudioComponent* CurrentPlayingDialogSound;
};
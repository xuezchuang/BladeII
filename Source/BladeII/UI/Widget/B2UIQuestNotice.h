#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "../B2RichTextBlock.h"
#include "B2UIQuestNotice.generated.h"

/*
* Lobby Main UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIQuestNotice : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	UFUNCTION()
	void OnClickBtnClose();

	UFUNCTION()
	void OnClickBtnSlot1();

	UFUNCTION()
	void OnClickBtnSlot2();

	UFUNCTION()
	void OnClickBtnSlot3();

	void InitQuestSlotNotice(const TArray<b2network::B2QuestInfoPtr>& QuestList);
	TWeakObjectPtr<class UB2UIQuestNoticeSlot> GetSuitableQuestSlot(int32& Outindex);

	void OpenQuestDialog(int32 SlotIndex);

protected:


	TArray< TWeakObjectPtr<UB2UIQuestNoticeSlot>> NoticeSlots;

	TWeakObjectPtr<UB2Button>	BTN_AreaSlot1;
	TWeakObjectPtr<UB2Button>	BTN_AreaSlot2;
	TWeakObjectPtr<UB2Button>	BTN_AreaSlot3;

	TWeakObjectPtr<UImage>		IMG_Character;
	TWeakObjectPtr<UButton>		BTN_Close;

	TWeakObjectPtr<UTextBlock>	TB_QuestNPC;
	TWeakObjectPtr<UTextBlock>	TB_QuestNotice;
	TWeakObjectPtr<UTextBlock>	TB_QuestNoticeMent;

	


};

USTRUCT()
struct FQuestNotifySlot
{
	GENERATED_USTRUCT_BODY()

	TWeakObjectPtr<UCanvasPanel>		CP_QuestSlot;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_QuestTitle;
	TWeakObjectPtr<UTextBlock>			TB_QuestComplete;
	TWeakObjectPtr<UImage>				IMG_QuestCompleteIcon;

	FQuestNotifySlot() :
		CP_QuestSlot(nullptr), RTB_QuestTitle(nullptr), TB_QuestComplete(nullptr), IMG_QuestCompleteIcon(nullptr)
	{}

	void SetQuestCompleteNotify(bool bVisible, int32 QuestSlot);
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIQuestCompleteNotify : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void CacheAssets() override;

	void ShowCompleteQuestNotify(int32 QuestSlot);
	void ActiveCloseTimer(float AutoCloseTime);

	UFUNCTION()
	void CloseCompleteNotify();
	UFUNCTION()
	void OpenNotify();

protected:
	TArray<FQuestNotifySlot>	QuestCompleteNotiSlots;

	FTimerHandle AutoClosetTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NotifyAutoCloseTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoGapTime;
private:
	FTimerHandle AutoGapTimerHandle;
	TQueue<int32>				ProgressQuestSlots;

	void VisibleNotify(int32 QuestSlot, bool IsVisible);
};
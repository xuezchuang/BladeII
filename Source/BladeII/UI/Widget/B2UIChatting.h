#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2Chatting.h"
//#include "B2UIChatMessageBase.h"//
#include "B2UIUserInfoActionPopup.h"
#include "B2UIBackWidget.h"
#include "CommonStruct.h"
#include "B2ScrollBox.h"
#include "B2UIChatting.generated.h"

enum class EB2UIChatCategory // Would be similar to EB2ChatType, but not probably be the same..
{
	UICHTY_All,
	UICHTY_Guild,
	UICHTY_Noti,
	
	UICHTY_End
};

namespace FUIChatMessageConfig
{
	const int32 InputGuardTime = 3;
	const int32 PaddingAreaSize = 5;
}

USTRUCT(BlueprintType)
struct FChatMessageContainer
{
	GENERATED_USTRUCT_BODY()
		FChatMessageContainer()
	{
	}

	TArray<TWeakObjectPtr<class UB2UIChatMessageBase>> Slots;
};

/*
 * Chatting main frame
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChatting : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

	EB2UIChatCategory SelectedCategory; // Selected by category tab button.

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIChatMessageBase> MessagePartClass_OtherUser;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIChatMessageBase> MessagePartClass_Myself;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIChatMessageBase> MessagePartClass_PublicNoti;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIChatMessageBase> MessagePartClass_LocalNoti;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIChatMessageBase> MessagePartClass_GuildSupport;
public:
	UB2UIChatting(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CloseWidgetDelegate() override;

public:
	void UpdateGuildBadge();
	void UpdateGuildBadge(class UB2UIDocBase* Sender, int32 Cur, int32 Prev);
	void UpdateChatMessageAll();
	void UpdateChatMessage(bool bAutoScrollDown); // To be called when we received a new message, or periodically, to refresh all displayed messages.
	void UpdateControlInfo();
	void UpdateChatScrollEnd(bool bForce = false);

	void SetUserBlockState(bool IsBlock, const FString& UserNickName = FString(""));

	EB2UIChatCategory GetCurChatCategory() { return SelectedCategory; }

protected:
	void DestroyChatMessages();
	void HideChatMessages();

	void SetChatCategoryTab(EB2UIChatCategory InUIChatCategory);

	bool GetIsAvailableForInput();
	void OnUpdateChatInput();

	void OnUpdateMessageTime();
	void OnUpdateScroll(float ScrolledDelta);
	void OnEndScroll();
	void UpdateScrollButton();
	void UpdateScrollButtonState(bool bUp, bool bDown);
	void UpdateScrollButtonText(const int32 UpIndex, const int32 BottomIndex);
	void CheckScrollButtons(const int32 TargetIndex);

	void OnCloseCallback();

	void UpdateSideButtons();

	int32 GetScrollNearTopIndex();
	int32 GetScrollNearBottomIndex();
	int32 GetTargetScrollIndex(const int32 TargetIndex);
	int32 GetTargetScrollNumber(const int32 From, const bool bIsMin);
	int32 GetTargetScrollCount(const int32 From, const bool bIsMin);
	int32 GetLastSupportSlotIndex();
	int32 GetFitstSupportSlotIndex();
	
	void UpdateNoticeContent();
	bool CheckAndExecuteConsoleCommandFromChatInput(const FString& ChatEditMsg);

	void SetInvisibleChatRows();

public:
	static TArray<EB2ChatType> GetAllDisplayTypes(EB2UIChatCategory InUIChatCategory);
	static EB2ChatType GetMessageInputType(EB2UIChatCategory InUIChatCategory);
	TSubclassOf<class UB2UIChatMessageBase> GetProperUIMessageClass(const int32 InSlotType);
	class UB2UIChatMessageBase* GetUIMessageSlot(const FB2ChatMessage& InMessageInfo);

	int32 GetUIMessageSlotType(const FB2ChatMessage& InMessageInfo);

protected:

	UFUNCTION()
	void OnClickBtnEnterChatMsg();
	
	UFUNCTION()
	void OnChatMsgEditChanged(const FText& InText);

	UFUNCTION()
	void OnClickBtnControl();
	void OpenChangeChannelPopup();
	void OpenRequestGuildItemPopup();

	UFUNCTION()
	void OnClickBtnChatTabAll();
	UFUNCTION()
	void OnClickBtnChatTabGuild();
	UFUNCTION()
	void OnClickBtnChatTabNoti();

	UFUNCTION()
	void OnClickBtnScrollDown();
	UFUNCTION()
	void OnClickBtnScrollUp();

	UFUNCTION()
	void OnClickBlockList();

	UFUNCTION()
	void OnChatMsgUserNameClicked(const int64 AccountID, const int64 GuildID, const FString& InUserName);

	UFUNCTION()
	void OnClickBtnClose();


	UFUNCTION()
	void OnChangedCurrentChannel(class UB2UIDocBase* Sender, const int32 CurrentChannelNum, const int32 PrevChannelNum);

private:
	void SubscribeEvent();
	void UnsubscribeEvent();

	void ReceiveGuildDetailInfo(const FB2ReceiveGuildInfo& GuildInfoData);
protected:

	TWeakObjectPtr<UTextBlock> TB_Chatting_Title;

	TWeakObjectPtr<UB2ScrollBox> SB_MessagePanel; // Where all message parts are created.

	TWeakObjectPtr<UEditableText> EDIT_ChatMsg;
	TWeakObjectPtr<UB2Button> BTN_EnterChatMsg;
	TWeakObjectPtr<UTextBlock> TB_EnterChatMsg;

	TWeakObjectPtr<UTextBlock> TB_EssentialNotiLabel;
	TWeakObjectPtr<UTextBlock> TB_EssentialNotiContent;

	TWeakObjectPtr<UB2Button> BTN_Control;
	TWeakObjectPtr<UTextBlock> TB_Control;

	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UB2Button> BTN_BlockList;
	TWeakObjectPtr<UTextBlock> TB_BlockList;

	TWeakObjectPtr<UCanvasPanel> CP_ChatTabSet_All;
	TWeakObjectPtr<UB2Button> BTN_ChatTab_All;
	TWeakObjectPtr<UImage> IMG_Selected_ChatTab_All;
	TWeakObjectPtr<UTextBlock> TB_ChatTab_All;

	TWeakObjectPtr<UCanvasPanel> CP_ChatTabSet_Guild;
	TWeakObjectPtr<UB2Button> BTN_ChatTab_Guild;
	TWeakObjectPtr<UImage> IMG_Selected_ChatTab_Guild;
	TWeakObjectPtr<UTextBlock> TB_ChatTab_Guild;

	TWeakObjectPtr<UCanvasPanel> CP_ChatTabSet_Noti;
	TWeakObjectPtr<UB2Button> BTN_ChatTab_Noti;
	TWeakObjectPtr<UImage> IMG_Selected_ChatTab_Noti;
	TWeakObjectPtr<UTextBlock> TB_ChatTab_Noti;
	
	TWeakObjectPtr<UPanelWidget> X_GuildBadge;
	TWeakObjectPtr<UTextBlock> TB_GuildNotiCount;

	TWeakObjectPtr<UPanelWidget> X_ChatInput;
	
	TWeakObjectPtr<UB2Button> BTN_ScrollDown;
	TWeakObjectPtr<UB2Button> BTN_ScrollUp;
	TWeakObjectPtr<UTextBlock> TB_ScrollUp;
	TWeakObjectPtr<UTextBlock> TB_ScrollDown;
	

	TWeakObjectPtr<UB2UIUserInfoActionPopup> UIP_UserInfoActionPopupSet;

	UPROPERTY(Transient)
	TArray<class UB2UIChatMessageBase*> CreatedMessages; // Dynamically created

	UPROPERTY(Transient)
	TArray<class UB2UIChatMessageBase*> CachedSupportMessages;

	UPROPERTY(Transient)
	TMap<int32, FChatMessageContainer> CachedSlotMap;

	FTimerHandle TimeToMessageUpdate;
	FTimerHandle TimeToInputGuard;
	FTimerHandle TimeToUpdateScroll;

	int32 TimerChatInputCount;

	// To handle max message length clamp.. 
	bool bClampedEditMsgPending;
	float LastScrolledOffect;
	bool bDirectScrolling;

	bool bBlockScrollBox;

	int32 ScrolledSupportIndex;
	FText PendingClampedEditMsg;

	int64 DeliveryGetGuildInfoTicket;
	int64 UpdateChatUITicket;

	const TCHAR* SYS_CHEAT_PREFIX; //For Cheat
	const TCHAR* SYS_SERVERCMD_PREFIX; //For Cheat

};
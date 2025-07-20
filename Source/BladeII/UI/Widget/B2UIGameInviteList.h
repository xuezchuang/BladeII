// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "B2ScrollBox.h"
#include "B2UIGameInviteList.generated.h"

DECLARE_DELEGATE_OneParam(FB2UIGameInviteCloseEvent, bool);

enum class EGameInviteTab : uint8
{
	Friend,
	Guild,
	Search
};

class UB2UIGameInviteSlot;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGameInviteList : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	const uint8 MAX_INVITE_SLOT = 3;
	
public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
		
protected:	
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	void SubscribeEvent();
	void UnSubscribeEvent();
	void StartRefreshTimer(EGameInviteTab InviteTap);
	void ProcRefreshTime();

	void StartInviteUser(const FText& SelectCharName, int32 CheckSlotIndex);
	void StopInviteTimer(const FText& CharName);
	void SetJoinEvent(const FText& CharName);

	void SetInviteUIByGameMode(EB2GameMode ModeType);

	bool StartInvite_Raid(const FString& NickName, int32 SlotIndex);
	bool StartInvite_FriendlyPVP(const FString& NickName, int32 SlotIndex);

private:
	void CloseInviteListUI();
	void SetUIState(EGameInviteTab InviteState);
	void ResetSlotList(bool IsDeleteSlot = true, int32 RemainSlotCountByDelete = 10);
	void DeleteSlotList(int32 RemainCount = 0);
	void SetDisibleSlotList(int32 RemainCount = 0);
	void SetDisibleSlot(const FText& CharName);
	void RemoveSlot(const FText& CharName);
	
	void SetRaidJoinSlotStateByClose();
	void RequestUserList(EGameInviteTab InviteTap);
	
	int32 CreateInviteSlotCommon(int32 SlotIndex, FB2CharacterSummaryPtr CharSummaryPtr, int64 InPenaltyTime);
	void CreateInviteSlots(TArray<FB2FriendPtr>& FriendList);
	void CreateInviteSlots(TArray<FB2GuildMemberPtr> CharList);

	TWeakObjectPtr<class UB2UIGameInviteSlot> CreateOrGetSlot(int32 SlotIndex);

	bool CanUserJoin(const FString& AccountName);

	void SetVisibleSearchTab(bool bVisible);
	void UpdateVisibleSearchWidget(bool bVisible);

	void RequestFindUser(const FString& NickName);

	// Events
public:
	void OnDeliveryGetFriendList(const FB2FriendList& FriendList);
	void OnDeliveryGetGuildMemberList(const FB2ResponseGuildMemberPtr& GuildMemberList);
	void OnDeliveryFindUser(const FB2FindUser& FindUser);
	// UFUNCTIONs
public:
	UFUNCTION()
	void OnClick_BTN_Close();
	UFUNCTION()
	void OnClick_BTN_CloseArea();

	UFUNCTION()
	void OnClick_BTN_UserSearch();

	UFUNCTION()
	void OnClick_BTN_Refresh();

	UFUNCTION(BlueprintImplementableEvent)
	void SetRefreshTimer_BP(bool IsEnableRefreshBtn);

	UFUNCTION(BlueprintImplementableEvent)
	void SetSlotIndex_BP(int32 SlotNum);

	UFUNCTION()
	void OnClickBtnFriend();
	UFUNCTION()
	void OnClickBtnGuild();
	UFUNCTION()
	void OnClickBtnUserSearchTab();

	// Event Tickets
private:
	int32	DeliveryGetFriendListTicket;
	int32	DeliveryGetGuildMemberTicket;
	int32	DeliveryFindUserTicket;
	// Widgets
protected:
	TWeakObjectPtr<UB2Button>				BTN_Close;
	TWeakObjectPtr<UB2Button>				BTN_CloseArea;
	TWeakObjectPtr<UB2ScrollBox>			SB_SlotList;
	TWeakObjectPtr<UB2Button>				BTN_Refresh;
	TWeakObjectPtr<UTextBlock>				TB_RefreshTime;
	
	// 八祸
	TWeakObjectPtr<UOverlay>				O_UserSearch;
	TWeakObjectPtr<UB2Button>				BTN_UserSearch;
	TWeakObjectPtr<UEditableTextBox>		TIB_SearchName;
	
	TWeakObjectPtr<UTextBlock>				STB_Title;

	TWeakObjectPtr<UB2Button>				BTN_Block;

	TArray<TWeakObjectPtr<class UB2UIGameInviteSlot>> InviteSlots;

	TWeakObjectPtr<class UB2UICommonTab> TAB_Friend;
	TWeakObjectPtr<class UB2UICommonTab> TAB_Guild;
	TWeakObjectPtr<class UB2UICommonTab> TAB_Search;

public:
	FB2UIGameInviteCloseEvent		FCloseEvent;

	// BluePrint Datas
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Invite")
	TSubclassOf<class UB2UIGameInviteSlot> GameInviteSlotClass;

	// member variables
private:
	EGameInviteTab			SelectedTabType;
	int64					SendPointTime;
	FTimerHandle			RefeshPointTimer;

	TMap<FName, int64>		InvitingSlotDatas;	// 檬措甫 柳青窍绊 乐绰 浇吩 府胶飘 (Key : FName, StartTime)

	EB2GameMode				GameModeType;
};

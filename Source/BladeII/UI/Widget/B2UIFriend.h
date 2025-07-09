#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIFriendRow.h"
#include "B2UIFriendSideMenu.h"
#include "B2UIFriend.generated.h"

/*
* Lobby Main UI
*/

#define GET_SLOT_FOR_FRIEND_TAB(TAB,NAME)											\
GET_SLOT_BYNAME(UB2Button, TAB->Button, BTN_##NAME);								\
GET_SLOT_BYNAME(UImage, TAB->SelectedImage, IMG_##NAME##_Selected);					\
GET_SLOT_BYNAME(UImage, TAB->UnselectedImage, IMG_##NAME##_Unselected);				\
GET_SLOT_BYNAME(UTextBlock, TAB->SelectedText, TB_##NAME##_Selected);				\
GET_SLOT_BYNAME(UTextBlock, TAB->UnselectedText, TB_##NAME##_Unselected);

enum class EFriendUIMainState : uint8
{
	EFUMS_None,
	EFUMS_MyFriend,
	EFUMS_FriendFind,
};

enum class EFriendUISubState : uint8
{
	EBNS_None,
	EFUSS_MF_MyGameFriendListView,
	EFUSS_FF_SendFriendListView,
	EFUSS_FF_ReceiveFriendListView,
	EFUSS_FF_RecommendFriendListView,
	EFUSS_FF_KakaoInviteListView,
};

enum class EFriendTabState : uint8
{
	EFTS_None,
	EFTS_Selected,
	EFTS_Unselected,
	EFTS_Hiddne,
};

struct FFriendTab
{
	TWeakObjectPtr<UB2Button>	Button;
	TWeakObjectPtr<UImage>		SelectedImage;
	TWeakObjectPtr<UImage>		UnselectedImage;
	TWeakObjectPtr<UTextBlock>	SelectedText;
	TWeakObjectPtr<UTextBlock>	UnselectedText;

	TArray<FFriendTab> SubTabs;
};

class UB2UIFriendSideMenu;
class UB2UIFriendKakaoInvite;

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIFriend : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetFriendUIState(UB2Button* SelectedSubTab, EFriendUISubState SubState);

	void SetStsticContents();
	void RefreshGameFriendCount(int32 nNewFriendCount);

	virtual void RegisterUIMarkForRedDot() final;

public:

	UFUNCTION()
	bool RedDotCondition_MyFriends();
	UFUNCTION()
	bool RedDotCondition_SendAbleFriendPoint();
	UFUNCTION()
	bool RedDotCondition_RequestFriends();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_stMainMyFriendList();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_stMainFindFriendMenu();

	UFUNCTION()
	void OnMyFriendListBtnClick();

	UFUNCTION()
	void OnFindFriendMenuBtnClick();

	UFUNCTION()
	void OnKakaoInviteMenuBtnClick();

	UFUNCTION()
	void OnRcvListBtnClick();

	UFUNCTION()
	void OnSndListBtnClick();

	UFUNCTION()
	void OnRcmdListBtnClick();

	UFUNCTION()
	void OnDelFriendModeBtnClick();

	UFUNCTION()
	void OnSendPointAllBtnClick();

	UFUNCTION()
	void OnFindBtnClick();

	UFUNCTION()
	void OnRefreshBtnClick();

	void SetSendFriendRowAccountId(int64 nAccountId);

	EFriendUISubState GetSubState();
	bool IsFriendDeleteMode();
	int32 GetFriendDeleteRemainCount();

private:
	void SubscribeEvent();
	void UnsubscribeEvent();

	void OnDeliveryAskFriend();
	void OnDeliveryAcceptAskFriend();
	void OnDeliveryRejectAskFriend();
	void OnDeliveryGetFriendList(const FB2FriendList& FriendList);
	void OnDeliveryCancelAskFriend();
	void OnDeliveryDeleteFriend(const FB2DeleteFriend&  tuDeleteFriend);
	void OnDeliveryGetAllSendAskFriend(const FBAllSendAskFriendList& FriendList);
	void OnDeliveryGetAllReceiveAskFriend(const FB2AllReceiveAskFriendList& FriendList);
	void OnDeliverySendSocialPoint(const FB2SendSocialPoint& SendSocialPoint);
	void OnDeliveryGetRecommendFriend(const FB2RecommendFriendList& FriendList);
	void OnDeliveryFindUser(const FB2FindUser& FindUser);
	void OnErrorHandle5513();

	class UB2UIFriendRow* GetFriendRowUI(int64 nAccountId);
	void DeleteSavedFriendRow();
	void SetFriendTabState(FFriendTab* pFriendTab, EFriendTabState TabState);
	void DeleteFriendRow(int32 nAccountId);

	template <typename T>
	void CreateFriendRows(TArray<T>& arFriendInfo);
	void ResetFriendRows();

	void RefreshReceiveFriendAskCount();
	void RefreshSendFriendAskCount();
	void CheckNoFriendListComment();

	void SetFriendDeleteMode(bool bNewDeleteMode);

	void SetTimerRefreshFriend();
	void SetTimerFindFriend();
	void UpdateRefreshFriendButtons(bool bTimeOver);
	void UpdateRefreshFriendTexts(const int32 nCount, const bool bTimeOver);
	void UpdateRefreshTimeCount();

	void UpdateFindFriendButtons(bool bTimeOver);
	void UpdateFindFriendTexts(const int32 nCount, const bool bTimeOver);
	void UpdateFindTimeCount();

	void ReleaseFindTimer();
	void ReleaseRefreshTimer();

	void SetTimerLastLogoutTimer();
	
	void UpdateLastLogoutTimes();

	void OnSideMenuBtnCallback(const int32 MenuIndex);

protected:

	TWeakObjectPtr<UPanelWidget> G_SubBtns_MyFriendList;
	TWeakObjectPtr<UPanelWidget> G_SubBtns_FindFriendMenu;
	TWeakObjectPtr<UPanelWidget> G_MyFriendListPart;
	TWeakObjectPtr<UPanelWidget> G_RcvListPart;
	TWeakObjectPtr<UPanelWidget> G_SndListPart;
	TWeakObjectPtr<UPanelWidget> G_RcmdListPart;
	TWeakObjectPtr<UPanelWidget> G_FriendPreviewPart;

	TWeakObjectPtr<UOverlay> G_NoFriend;

	TWeakObjectPtr<UB2Button> BTN_GameFriendList;
	TWeakObjectPtr<UB2Button> BTN_RcvList;
	TWeakObjectPtr<UB2Button> BTN_SndList;
	TWeakObjectPtr<UB2Button> BTN_RcmdList;

	TWeakObjectPtr<UB2Button> BTN_DelFriendMode;
	TWeakObjectPtr<UB2Button> BTN_SendPointAll;
	TWeakObjectPtr<UImage> IMG_SendPointAllBtn;
	TWeakObjectPtr<UImage> IMG_SendPointAllBtnDisable;

	TWeakObjectPtr<UB2Button> BTN_Find;
	TWeakObjectPtr<UB2Button> BTN_Refresh;
	TWeakObjectPtr<UTextBlock> TB_CurFriendCount;
	TWeakObjectPtr<UTextBlock> TB_MaxFriendCount;
	TWeakObjectPtr<UTextBlock> TB_NoFriendCommnet;
	TWeakObjectPtr<UB2RichTextBlock> TB_RcvFriendCount;
	TWeakObjectPtr<UB2RichTextBlock> TB_SndFriendCount;
	TWeakObjectPtr<UEditableTextBox> TIB_FindName;

	TWeakObjectPtr<UB2ScrollBox> SB_Main;

	TWeakObjectPtr<UTextBlock> STB_NoFriendName;
	TWeakObjectPtr<UTextBlock> STB_CurrentFriends;
	TWeakObjectPtr<UTextBlock> STB_Find;
	TWeakObjectPtr<UTextBlock> STB_Refresh;
	TWeakObjectPtr<UTextBlock> STB_AllSend;
	TWeakObjectPtr<UTextBlock> STB_FriendBreak;
	TWeakObjectPtr<UTextBlock> STB_RecieveFriends;
	TWeakObjectPtr<UTextBlock> STB_SendFriends;

	TWeakObjectPtr<UTextBlock>	TB_GameFriendList_Selected;
	TWeakObjectPtr<UTextBlock>	TB_GameFriendList_Unselected;

	TWeakObjectPtr<UTextBlock>	TB_RcmdList_Selected;
	TWeakObjectPtr<UTextBlock>	TB_RcmdList_Unselected;

	TWeakObjectPtr<UTextBlock>	TB_RcvList_Unselected;
	TWeakObjectPtr<UTextBlock>	TB_RcvList_Selected;

	TWeakObjectPtr<UTextBlock>	TB_SndList_Unselected;
	TWeakObjectPtr<UTextBlock>	TB_SndList_Selected;

	TWeakObjectPtr<UTextBlock>	TB_MyFriendInvite_Selected;
	TWeakObjectPtr<UTextBlock>	TB_MyFriendInvite_Unselected;

	TWeakObjectPtr<UB2UIFriendSideMenu> UIP_FriendSideMenu;
	TWeakObjectPtr<UB2UIFriendKakaoInvite> UIP_FriendKakaoInvite;

	TWeakObjectPtr<UB2UIWidgetBase> UI_CommonFrame;

	int32 DeliveryAskFriendTicket;
	int32 DeliveryAcceptAskFriendTicket;
	int32 DeliveryRejectAskFriendTicket;
	int32 DeliveryGetFriendListTicket;
	int32 DeliveryCancelAskFriendTicket;
	int32 DeliveryDeleteFriendTicket;
	int32 DeliveryGetAllSendAskFriendTicket;
	int32 DeliveryGetAllReceiveAskFriendTicket;
	int32 DeliverySendSocialPointTicket;
	int32 DeliveryGetRecommendFriendTicket;
	int32 DeliveryFindUserTicket;
	int32 HandleServerError5503Ticket;
	int32 HandleServerError5513Ticket;
	int32 OnFriendSideMenuSelectTicket;

	int32 m_nFriendDeleteRemainCount;
	int32 m_nFriendCount;

	int32 RefreshTimeCount;
	int32 FindTimeCount;
	FTimerHandle TimeToRefreshFriend;
	FTimerHandle TimeToFindFriend;
	FTimerHandle TimeToLastLogoutTime;

	EB2UIFriendSideMenu CurrentSideMenu;

	TWeakObjectPtr<class UB2UICommonTab>		UI_CommonFriendList;
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<UB2UIFriendRow> FriendContentClass;

private:
	TArray<FFriendTab> m_arMainTabs;
	UB2Button* m_pCurrentMainTab;

	TArray<UB2UIFriendRow*> m_arFriendRows;
	int64 m_nSendFriendRowAccountId;
	EFriendUISubState m_eSubState;
	bool m_bIsFriendDeleteMode;
};

template <typename T>
void UB2UIFriend::CreateFriendRows(TArray<T>& arFriendInfo)
{
	if (SB_Main.IsValid() && SB_Main->GetVisibility() != ESlateVisibility::Visible)
		SB_Main->SetVisibility(ESlateVisibility::Visible);

	for (auto ptrFriendInfo : arFriendInfo)
	{
		UB2UIFriendRow* CreatedWidget = CreateWidget<UB2UIFriendRow>(GetOwningPlayer(), FriendContentClass);

		if (CreatedWidget)
		{
			SB_Main->AddChild(CreatedWidget);
			CreatedWidget->Init();
			CreatedWidget->SetFriendContent(ptrFriendInfo);
			CreatedWidget->SetOwnerUI(this);

			m_arFriendRows.Add(CreatedWidget);
		}
	}
}
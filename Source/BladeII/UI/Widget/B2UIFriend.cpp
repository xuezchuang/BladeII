#include "B2UIFriend.h"
#include "Retailer.h"
#include "B2UIFriendRow.h"

#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2RedDotManager.h"
#include "B2UIDocHelper.h"
#include "B2UICommonTab.h"
#include "B2UIFriendSideMenu.h"
#include "B2UIFriendKakaoInvite.h"
#include "B2GameInstance.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2CodeTable.h"

void UB2UIFriend::Init()
{
	SubscribeEvent();

	Super::Init();

	m_pCurrentMainTab = NULL;
	// 檬扁 0疙栏肺 钎矫
	RefreshGameFriendCount(0);

	// 模备绝澜舅覆 见败初澜
	G_NoFriend->SetVisibility(ESlateVisibility::Hidden);

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_Friend"));

	SetStsticContents();

	//徘汲沥
	OnMyFriendListBtnClick();

	m_nSendFriendRowAccountId = -1;
	SetFriendDeleteMode(false);
	m_nFriendDeleteRemainCount = 0;
	RefreshTimeCount = -1;
}

void UB2UIFriend::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetTimerLastLogoutTimer();

	// UIHeader 客 鞍捞 Scene 栏肺 备己窍绰 版快 Init 矫痢俊 窍搁 Scene 俊 器窃等 header 啊 积己捞 救等 惑炔老 荐 乐澜.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_Friend"));
	DoMarkRedDot();
	if (UIP_FriendSideMenu.IsValid())
		UIP_FriendSideMenu->DoMarkRedDot();
}

void UB2UIFriend::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvent();

	if (UIP_FriendKakaoInvite.IsValid())
		UIP_FriendKakaoInvite->UnsubscribeEvent();

	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToLastLogoutTime);

	Super::DestroySelf(InUIManager);
}

void UB2UIFriend::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UPanelWidget, G_SubBtns_MyFriendList);
	GET_SLOT(UPanelWidget, G_SubBtns_FindFriendMenu);
	GET_SLOT(UPanelWidget, G_MyFriendListPart);
	GET_SLOT(UPanelWidget, G_RcvListPart);
	GET_SLOT(UPanelWidget, G_SndListPart);
	GET_SLOT(UPanelWidget, G_RcmdListPart);
	GET_SLOT(UPanelWidget, G_FriendPreviewPart);

	GET_SLOT(UOverlay, G_NoFriend);

	FFriendTab* TempFriendTab;

	m_arMainTabs.AddDefaulted(1);
	TempFriendTab = &m_arMainTabs[0];
	GET_SLOT_FOR_FRIEND_TAB(TempFriendTab, MyFriendList);
	//BTN_MyFriendList = TempFriendTab->Button;

	// 皋牢徘1 - 辑宏徘1
	m_arMainTabs[0].SubTabs.AddDefaulted(1);
	TempFriendTab = &m_arMainTabs[0].SubTabs[0];
	GET_SLOT_FOR_FRIEND_TAB(TempFriendTab, GameFriendList);
	//咯扁 辑宏徘 滚瓢篮 喘副老捞 绝澜 徘肮荐 眠啊登搁 函荐档 鞍捞 眠啊窍磊
	BTN_GameFriendList = TempFriendTab->Button;

	m_arMainTabs.AddDefaulted(1);
	TempFriendTab = &m_arMainTabs[1];
	GET_SLOT_FOR_FRIEND_TAB(TempFriendTab, FindFriendMenu);
	//BTN_FindFriendMenu = TempFriendTab->Button;

	// 皋牢徘2 - 辑宏徘1
	m_arMainTabs[1].SubTabs.AddDefaulted(1);
	TempFriendTab = &m_arMainTabs[1].SubTabs[0];
	GET_SLOT_FOR_FRIEND_TAB(TempFriendTab, RcvList);
	BTN_RcvList = TempFriendTab->Button;

	// 皋牢徘2 - 辑宏徘2
	m_arMainTabs[1].SubTabs.AddDefaulted(1);
	TempFriendTab = &m_arMainTabs[1].SubTabs[1];
	GET_SLOT_FOR_FRIEND_TAB(TempFriendTab, SndList);
	BTN_SndList = TempFriendTab->Button;

	// 皋牢徘2 - 辑宏徘3
	m_arMainTabs[1].SubTabs.AddDefaulted(1);
	TempFriendTab = &m_arMainTabs[1].SubTabs[2];
	GET_SLOT_FOR_FRIEND_TAB(TempFriendTab, RcmdList);
	BTN_RcmdList = TempFriendTab->Button;

	m_arMainTabs.AddDefaulted(1);
	TempFriendTab = &m_arMainTabs[2];
	GET_SLOT_FOR_FRIEND_TAB(TempFriendTab, KakaoFriendMenu);
	
	GET_SLOT(UB2Button, BTN_DelFriendMode);
	GET_SLOT(UB2Button, BTN_SendPointAll);

	GET_SLOT(UImage, IMG_SendPointAllBtn);
	GET_SLOT(UImage, IMG_SendPointAllBtnDisable);

	GET_SLOT(UB2Button, BTN_Find);
	GET_SLOT(UB2Button, BTN_Refresh);

	GET_SLOT(UTextBlock, TB_CurFriendCount);
	GET_SLOT(UTextBlock, TB_MaxFriendCount);
	GET_SLOT(UTextBlock, TB_NoFriendCommnet);

	GET_SLOT(UB2RichTextBlock, TB_RcvFriendCount);
	GET_SLOT(UB2RichTextBlock, TB_SndFriendCount);

	GET_SLOT(UEditableTextBox, TIB_FindName);

	GET_SLOT(UB2ScrollBox, SB_Main);

	GET_SLOT(UTextBlock, STB_NoFriendName);
	GET_SLOT(UTextBlock, STB_CurrentFriends);
	GET_SLOT(UTextBlock, STB_Find);
	GET_SLOT(UTextBlock, STB_AllSend);
	GET_SLOT(UTextBlock, STB_FriendBreak);
	GET_SLOT(UTextBlock, STB_RecieveFriends);
	GET_SLOT(UTextBlock, STB_SendFriends);
	GET_SLOT(UTextBlock, STB_Refresh);

	GET_SLOT(UTextBlock, TB_GameFriendList_Selected);
	GET_SLOT(UTextBlock, TB_GameFriendList_Unselected);
	
	GET_SLOT(UTextBlock, TB_MyFriendInvite_Selected);
	GET_SLOT(UTextBlock, TB_MyFriendInvite_Unselected);

	GET_SLOT(UTextBlock, TB_RcmdList_Unselected);
	GET_SLOT(UTextBlock, TB_RcmdList_Selected);

	GET_SLOT(UTextBlock, TB_RcvList_Unselected);
	GET_SLOT(UTextBlock, TB_RcvList_Selected);

	GET_SLOT(UTextBlock, TB_SndList_Unselected);
	GET_SLOT(UTextBlock, TB_SndList_Selected);

	GET_SLOT(UB2UICommonTab, UI_CommonFriendList);
	if (UI_CommonFriendList.IsValid())
		UI_CommonFriendList->Init();

	GET_SLOT(UB2UIFriendSideMenu, UIP_FriendSideMenu);
	if (UIP_FriendSideMenu.IsValid())
		UIP_FriendSideMenu->Init();

	GET_SLOT(UB2UIFriendKakaoInvite, UIP_FriendKakaoInvite);
	if (UIP_FriendKakaoInvite.IsValid())
		UIP_FriendKakaoInvite->Init();  

	GET_SLOT(UB2UIWidgetBase, UI_CommonFrame);

	UpdateRefreshTimeCount();
	UpdateFindTimeCount();

	//BTN_GameFriendList滚瓢篮 官牢爹 救茄促.(瘤陛 救鞘夸)
	//BIND_CLICK_FUNC_TO_BTN(BTN_FindFriendMenu, &UB2UIFriend::OnFindFriendMenuBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_RcvList, &UB2UIFriend::OnRcvListBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_SndList, &UB2UIFriend::OnSndListBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_RcmdList, &UB2UIFriend::OnRcmdListBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_DelFriendMode, &UB2UIFriend::OnDelFriendModeBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_SendPointAll, &UB2UIFriend::OnSendPointAllBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Find, &UB2UIFriend::OnFindBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Refresh, &UB2UIFriend::OnRefreshBtnClick);
}

void UB2UIFriend::UpdateStaticText()
{
	if (STB_Find.IsValid())
		STB_Find->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Search")));

	if (STB_NoFriendName.IsValid())
		STB_NoFriendName->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Manager")));

	if (TB_GameFriendList_Selected.IsValid())
		TB_GameFriendList_Selected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_MyList")));

	if (TB_GameFriendList_Unselected.IsValid())
		TB_GameFriendList_Unselected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_MyList")));

	if (TB_MyFriendInvite_Selected.IsValid())
		TB_MyFriendInvite_Selected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_MyList")));

	if (TB_MyFriendInvite_Unselected.IsValid())
		TB_MyFriendInvite_Unselected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_MyList")));

	if (TB_RcmdList_Selected.IsValid())
		TB_RcmdList_Selected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Recommend")));

	if (TB_RcmdList_Unselected.IsValid())
		TB_RcmdList_Unselected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Recommend")));

	if (TB_RcvList_Selected.IsValid())
		TB_RcvList_Selected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_ReceiveList2")));

	if (TB_RcvList_Unselected.IsValid())
		TB_RcvList_Unselected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_ReceiveList2")));

	if (TB_SndList_Selected.IsValid())
		TB_SndList_Selected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_SendList2")));

	if (TB_SndList_Unselected.IsValid())
		TB_SndList_Unselected->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_SendList2")));

	if (STB_SendFriends.IsValid())
		STB_SendFriends->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_SendList")));

	if (STB_RecieveFriends.IsValid())
		STB_RecieveFriends->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_ReceiveList")));

	if (STB_CurrentFriends.IsValid())
		STB_CurrentFriends->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_CurrentFirends")));

	if (STB_FriendBreak.IsValid())
		STB_FriendBreak->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Revocation")));

	if (STB_AllSend.IsValid())
		STB_AllSend->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_AllSend")));

	if(TIB_FindName.IsValid())
		TIB_FindName->SetHintText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_SearchNickname")));
}

void UB2UIFriend::BindDelegates()
{
}

void UB2UIFriend::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2UIFriend::OnSideMenuBtnCallback(const int32 MenuIndex)
{
	CurrentSideMenu = static_cast<EB2UIFriendSideMenu>(MenuIndex);
	switch (CurrentSideMenu)
	{
	case EB2UIFriendSideMenu::MyFriendList:
		OnMyFriendListBtnClick();
		break;
	case EB2UIFriendSideMenu::FindFriend:
		OnFindFriendMenuBtnClick();
		break;
	case EB2UIFriendSideMenu::KakaoInvite:
		OnKakaoInviteMenuBtnClick();
		break;
	default:
			break;
	}
}

void UB2UIFriend::SetFriendUIState(UB2Button* SelectedSubTab, EFriendUISubState SubState)
{
	int32 MenuIndex = static_cast<int32>(CurrentSideMenu);

	if (m_arMainTabs.IsValidIndex(MenuIndex))
	{
		for (FFriendTab SubTab : m_arMainTabs[MenuIndex].SubTabs)
		{
			if (SubTab.Button == SelectedSubTab)
				SetFriendTabState(&SubTab, EFriendTabState::EFTS_Selected);
			else
				SetFriendTabState(&SubTab, EFriendTabState::EFTS_Unselected);
		}
	}

	// 辑宏胶抛捞飘俊 蝶扼 UI弊缝喊 visivle悸泼, 利寸茄 模备府胶飘 辑滚俊 夸没
	// 弊缝阑 牡滚胶 菩澄肺且瘤 促弗吧肺 且瘤肋 葛福摆促. 沥窍绊 盲框
	// 弊缝篮 UPanelWidget 捞吧肺 某胶泼秦辑 柳青秦航

	if (!G_MyFriendListPart.IsValid() || !G_RcvListPart.IsValid() ||
		!G_SndListPart.IsValid() || !G_RcmdListPart.IsValid())
		return;

	// UI胶抛捞飘 函版矫 模备昏力 葛靛绰 秦力
	SetFriendDeleteMode(false);

	G_MyFriendListPart->SetVisibility(ESlateVisibility::Hidden);
	G_RcvListPart->SetVisibility(ESlateVisibility::Hidden);
	G_SndListPart->SetVisibility(ESlateVisibility::Hidden);
	G_RcmdListPart->SetVisibility(ESlateVisibility::Hidden);

 	if (UIP_FriendKakaoInvite.IsValid())
 		UIP_FriendKakaoInvite->SetVisibility(ESlateVisibility::Hidden);

	if (G_FriendPreviewPart.IsValid())
		G_FriendPreviewPart->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if(SB_Main.IsValid())
		SB_Main->SetVisibility(ESlateVisibility::Hidden);

	ResetFriendRows();
	G_NoFriend->SetVisibility(ESlateVisibility::Hidden);

	UI_CommonFrame->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_eSubState = SubState;

	switch (SubState)
	{
	case EFriendUISubState::EFUSS_MF_MyGameFriendListView:
		G_MyFriendListPart->SetVisibility(ESlateVisibility::Visible);
		data_trader::Retailer::GetInstance().RequestGetFriendList();
		break;
	case EFriendUISubState::EFUSS_FF_SendFriendListView:
		G_SndListPart->SetVisibility(ESlateVisibility::Visible);
		data_trader::Retailer::GetInstance().RequestGetAllSendAskFriend();
		// 焊郴搁辑 咆胶飘 0疙栏肺 檬扁拳
		RefreshSendFriendAskCount();
		break;
	case EFriendUISubState::EFUSS_FF_ReceiveFriendListView:
		G_RcvListPart->SetVisibility(ESlateVisibility::Visible);
		data_trader::Retailer::GetInstance().RequestGetAllReceiveAskFriend();
		// 焊郴搁辑 咆胶飘 0疙栏肺 檬扁拳
		RefreshReceiveFriendAskCount();
		break;
	case EFriendUISubState::EFUSS_FF_RecommendFriendListView:
		G_RcmdListPart->SetVisibility(ESlateVisibility::Visible);
		data_trader::Retailer::GetInstance().RequestGetRecommendFriend();
		break;
	case EFriendUISubState::EFUSS_FF_KakaoInviteListView:
		if (UIP_FriendKakaoInvite.IsValid())
		{
			UIP_FriendKakaoInvite->SetVisibility(ESlateVisibility::Visible);
			UIP_FriendKakaoInvite->OnRefresh();
		}
		if (G_FriendPreviewPart.IsValid())
			G_FriendPreviewPart->SetVisibility(ESlateVisibility::Hidden);
		UI_CommonFrame->SetVisibility(ESlateVisibility::Hidden);
		data_trader::Retailer::GetInstance().RequestPlatformFriendInviteInfo(b2network::B2PlatformCode::FOR_KAKAO);
		break;
	}
}

void UB2UIFriend::SetStsticContents()
{
	// (弥措 {0}疙)
	int32 nMaxFriendCount = BladeIIGameImpl::GetClientDataStore().GetMaxFriendCount();
	TB_MaxFriendCount->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("MaxManCountBracket")), FText::AsNumber(nMaxFriendCount)));
}

void UB2UIFriend::RefreshGameFriendCount(int32 nNewFriendCount)
{
	// 郴啊 荐遏沁阑锭 窍唱 棵妨辑 钎矫秦临妨绊 历厘秦仇(瘤奎阑订 郴妨林绊)
	// 惑措啊 荐遏窍芭唱 瘤款扒 府胶飘 货肺罐阑锭 盎脚秦滴 瞪淀
	m_nFriendCount = nNewFriendCount;
	TB_CurFriendCount->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("ManCount")), FText::AsNumber(nNewFriendCount)));
}

void UB2UIFriend::RegisterUIMarkForRedDot()
{
	if (UIP_FriendSideMenu.IsValid())
		UIP_FriendSideMenu->RegisterUIMarkForRedDot();
	// 快沥器牢飘 焊尘 荐 乐阑 锭
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("BTN_SendPointAll")), &UB2UIFriend::RedDotCondition_SendAbleFriendPoint);
	// 模备夸没捞 乐阑锭
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("BTN_RcvListSet")), &UB2UIFriend::RedDotCondition_RequestFriends);
}

bool UB2UIFriend::RedDotCondition_SendAbleFriendPoint()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
		return FriendDoc->GetAbleToSendFriendPoint();

	return false;
}

bool UB2UIFriend::RedDotCondition_MyFriends()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
		return FriendDoc->GetIsNewFriend() || FriendDoc->GetAbleToSendFriendPoint();
	
	return false;
}

bool UB2UIFriend::RedDotCondition_RequestFriends()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		if (BladeIIGameImpl::GetClientDataStore().GetMaxFriendCount() <= FriendDoc->GetFriendList().Num())
			return false;
		return FriendDoc->GetIsAskFriend();
	}

	return false;
}

void UB2UIFriend::OnMyFriendListBtnClick()
{
	if(UI_CommonFriendList.IsValid())
		UI_CommonFriendList->SetTabSelected(true);

	SetFriendUIState(BTN_GameFriendList.Get(), EFriendUISubState::EFUSS_MF_MyGameFriendListView);
	if (G_SubBtns_MyFriendList.IsValid())
		G_SubBtns_MyFriendList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (G_SubBtns_FindFriendMenu.IsValid())
		G_SubBtns_FindFriendMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIFriend::OnFindFriendMenuBtnClick()
{
	SetFriendUIState(BTN_RcvList.Get(), EFriendUISubState::EFUSS_FF_ReceiveFriendListView);
	if (G_SubBtns_MyFriendList.IsValid())
		G_SubBtns_MyFriendList->SetVisibility(ESlateVisibility::Collapsed);
	if (G_SubBtns_FindFriendMenu.IsValid())
		G_SubBtns_FindFriendMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIFriend::OnKakaoInviteMenuBtnClick()
{
	SetFriendUIState(BTN_RcvList.Get(), EFriendUISubState::EFUSS_FF_KakaoInviteListView);
	if (G_SubBtns_MyFriendList.IsValid())
		G_SubBtns_MyFriendList->SetVisibility(ESlateVisibility::Collapsed);
	if (G_SubBtns_FindFriendMenu.IsValid())
		G_SubBtns_FindFriendMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIFriend::OnRcvListBtnClick()
{
	SetFriendUIState(BTN_RcvList.Get(), EFriendUISubState::EFUSS_FF_ReceiveFriendListView);
}

void UB2UIFriend::OnSndListBtnClick()
{
	SetFriendUIState(BTN_SndList.Get(), EFriendUISubState::EFUSS_FF_SendFriendListView);
}

void UB2UIFriend::OnRcmdListBtnClick()
{
	SetFriendUIState(BTN_RcmdList.Get(), EFriendUISubState::EFUSS_FF_RecommendFriendListView);
}

void UB2UIFriend::OnDelFriendModeBtnClick()
{
	SetFriendDeleteMode(!m_bIsFriendDeleteMode);

	// Row甸 滚瓢惑怕 盎脚
	for (int nCnt = 0; nCnt < m_arFriendRows.Num(); ++nCnt)
	{
		if (m_arFriendRows[nCnt])
		{
			m_arFriendRows[nCnt]->CheckButtonState();
		}
	}
}

void UB2UIFriend::OnSendPointAllBtnClick()
{
	TArray<int64> arSendPointAccountId;

	for (UB2UIFriendRow* pRow : m_arFriendRows)
	{
		if (pRow->IsSocialPointSendable())
		{
			arSendPointAccountId.AddDefaulted(1);
			arSendPointAccountId[arSendPointAccountId.Num() - 1] = pRow->GetFriendAccountId();
		}
	}

	if (arSendPointAccountId.Num() == 0)
	{
		UB2UIManager* pUIManager = UB2UIManager::GetInstance();
		pUIManager->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("NoFriendToSendSocialPoint")),
			0,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);

		return;
	}

	data_trader::Retailer::GetInstance().RequestSendSocialPoint(arSendPointAccountId);
}

void UB2UIFriend::OnFindBtnClick()
{
	ResetFriendRows();

	FString strFindUserName = TIB_FindName->GetText().ToString();
	TIB_FindName->SetText(FText::GetEmpty());

	if (strFindUserName.Len() < 2 || strFindUserName.Len() > 8)
	{
		UB2UIManager* pUIManager = UB2UIManager::GetInstance();
		pUIManager->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_NickNameExplanation")),
			0,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);

		return;
	}

	SetTimerFindFriend();
	data_trader::Retailer::GetInstance().RequestFindUser(strFindUserName);
}

void UB2UIFriend::OnRefreshBtnClick()
{
	SetTimerRefreshFriend();
	SetFriendUIState(BTN_RcmdList.Get(), EFriendUISubState::EFUSS_FF_RecommendFriendListView);
}

void UB2UIFriend::SetSendFriendRowAccountId(int64 nAccountId)
{
	m_nSendFriendRowAccountId = nAccountId;
}

EFriendUISubState UB2UIFriend::GetSubState()
{
	return m_eSubState;
}

bool UB2UIFriend::IsFriendDeleteMode()
{
	return m_bIsFriendDeleteMode;
}

int32 UB2UIFriend::GetFriendDeleteRemainCount()
{
	return m_nFriendDeleteRemainCount;
}

void UB2UIFriend::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIFriend);

	DeliveryAskFriendTicket = DeliveryAskFriendClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryAskFriend)
		Capture->OnDeliveryAskFriend();
	END_CAPTURE_OBJECT()
		);

	DeliveryAcceptAskFriendTicket = DeliveryAcceptAskFriendClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryAcceptAskFriend)
		Capture->OnDeliveryAcceptAskFriend();
	END_CAPTURE_OBJECT()
		);

	DeliveryRejectAskFriendTicket = DeliveryRejectAskFriendClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryRejectAskFriend)
		Capture->OnDeliveryRejectAskFriend();
	END_CAPTURE_OBJECT()
		);

	DeliveryGetFriendListTicket = DeliveryGetFriendListClass<FB2FriendList>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetFriendList, const FB2FriendList& FriendList)
		Capture->OnDeliveryGetFriendList(FriendList);
	END_CAPTURE_OBJECT()
		);

	DeliveryCancelAskFriendTicket = DeliveryCancelAskFriendClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryCancelAskFriend)
		Capture->OnDeliveryCancelAskFriend();
	END_CAPTURE_OBJECT()
		);

	DeliveryDeleteFriendTicket = DeliveryDeleteFriendClass<FB2DeleteFriend>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryDeleteFriend, FB2DeleteFriend tuDeleteFriend)
		Capture->OnDeliveryDeleteFriend(tuDeleteFriend);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetAllSendAskFriendTicket = DeliveryGetAllSendAskFriendClass<FBAllSendAskFriendList>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetAllSendAskFriend, const FBAllSendAskFriendList& FriendList)
		Capture->OnDeliveryGetAllSendAskFriend(FriendList);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetAllReceiveAskFriendTicket = DeliveryGetAllReceiveAskFriendClass<FB2AllReceiveAskFriendList>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetAllReceiveAskFriend, const FB2AllReceiveAskFriendList& FriendList)
		Capture->OnDeliveryGetAllReceiveAskFriend(FriendList);
	END_CAPTURE_OBJECT()
		);

	DeliverySendSocialPointTicket = DeliverySendSocialPointClass<FB2SendSocialPoint>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliverySendSocialPoint, const FB2SendSocialPoint& SendSocialPoint)
		Capture->OnDeliverySendSocialPoint(SendSocialPoint);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetRecommendFriendTicket = DeliveryGetRecommendFriendClass<FB2RecommendFriendList>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetRecommendFriend, const FB2RecommendFriendList& FriendList)
		Capture->OnDeliveryGetRecommendFriend(FriendList);
	END_CAPTURE_OBJECT()
		);

	DeliveryFindUserTicket = DeliveryFindUserClass<FB2FindUser>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryFindUser, const FB2FindUser& FindUser)
		Capture->OnDeliveryFindUser(FindUser);
	END_CAPTURE_OBJECT()
		);

	OnFriendSideMenuSelectTicket = OnFriendSideMenuSelectClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(OnFriendSideMenuSelect, const int32 SelectTab)
		Capture->OnSideMenuBtnCallback(SelectTab);
	END_CAPTURE_OBJECT()
		);

	HandleServerError5503Ticket = HandleServerError5503Class<>::GetInstance().Subscribe([]() { DevShowError("5503"); });

	HandleServerError5513Ticket = HandleServerError5513Class<>::GetInstance().Subscribe([this]() { OnErrorHandle5513(); });
}

void UB2UIFriend::UnsubscribeEvent()
{
	DeliveryAskFriendClass<>::GetInstance().Unsubscribe(DeliveryAskFriendTicket);
	DeliveryAcceptAskFriendClass<>::GetInstance().Unsubscribe(DeliveryAcceptAskFriendTicket);
	DeliveryRejectAskFriendClass<>::GetInstance().Unsubscribe(DeliveryRejectAskFriendTicket);
	DeliveryGetFriendListClass<FB2FriendList>::GetInstance().Unsubscribe(DeliveryGetFriendListTicket);
	DeliveryCancelAskFriendClass<>::GetInstance().Unsubscribe(DeliveryCancelAskFriendTicket);
	DeliveryDeleteFriendClass<FB2DeleteFriend>::GetInstance().Unsubscribe(DeliveryDeleteFriendTicket);
	DeliveryGetAllSendAskFriendClass<FBAllSendAskFriendList>::GetInstance().Unsubscribe(DeliveryGetAllSendAskFriendTicket);
	DeliveryGetAllReceiveAskFriendClass<FB2AllReceiveAskFriendList>::GetInstance().Unsubscribe(DeliveryGetAllReceiveAskFriendTicket);
	DeliverySendSocialPointClass<FB2SendSocialPoint>::GetInstance().Unsubscribe(DeliverySendSocialPointTicket);
	DeliveryGetRecommendFriendClass<FB2RecommendFriendList>::GetInstance().Unsubscribe(DeliveryGetRecommendFriendTicket);
	DeliveryFindUserClass<FB2FindUser>::GetInstance().Unsubscribe(DeliveryFindUserTicket);
	HandleServerError5503Class<>::GetInstance().Unsubscribe(HandleServerError5503Ticket);
	HandleServerError5513Class<>::GetInstance().Unsubscribe(HandleServerError5513Ticket);
	OnFriendSideMenuSelectClass<int32>::GetInstance().Unsubscribe(OnFriendSideMenuSelectTicket);
}

void UB2UIFriend::OnDeliveryAskFriend()
{
	DeleteSavedFriendRow();
	DevShowNotReadyYetMessage("FriendAdd");
}

void UB2UIFriend::OnDeliveryAcceptAskFriend()
{
	RefreshGameFriendCount(m_nFriendCount + 1);

	// 模备府胶飘 绝阑锭 内膏飘眉农
	CheckNoFriendListComment();

	RefreshReceiveFriendAskCount();
	DevShowNotReadyYetMessage("FriendPermit");
	
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		FriendDoc->OnForcedAddFriendInfo(m_nSendFriendRowAccountId);
		FriendDoc->OnForcedDeleteAskFriend(m_nSendFriendRowAccountId);
	}
	DoMarkRedDot();
	if(UIP_FriendSideMenu.IsValid())
		UIP_FriendSideMenu->DoMarkRedDot();
	DeleteSavedFriendRow();
	
	//////////////////////////////////////////////////////////////////
	//咀记 瘤钎 肺弊 (模备 肝扁)
	B2ActionLogSender Sender;
	Sender.setCategory(FString("FRIEND"))
		.setAction(FString("ACCEPT"))
		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//////////////////////////////////////////////////////////////////
}

void UB2UIFriend::OnDeliveryRejectAskFriend()
{
	// 模备府胶飘 绝阑锭 内膏飘眉农
	CheckNoFriendListComment();

	RefreshReceiveFriendAskCount();

	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend()) FriendDoc->OnForcedDeleteAskFriend(m_nSendFriendRowAccountId);
	DoMarkRedDot();
	if (UIP_FriendSideMenu.IsValid())
		UIP_FriendSideMenu->DoMarkRedDot();
	DeleteSavedFriendRow();
}

void UB2UIFriend::OnDeliveryGetFriendList(const FB2FriendList& FriendList)
{
	//TArray<FB2FriendPtr> arFriendList = GET_TUPLE_DATA(FB2ResponseGetFriendList::friends_index, FriendList);
	//m_nFriendDeleteRemainCount = GET_TUPLE_DATA(FB2ResponseGetFriendList::left_delete_friend_count_index, FriendList);

	//CreateFriendRows<FB2FriendPtr>(arFriendList);

	//RefreshGameFriendCount(arFriendList.Num());

	//// 模备府胶飘 绝阑锭 内膏飘眉农
	//CheckNoFriendListComment();
	//DoMarkRedDot();
	//if (UIP_FriendSideMenu.IsValid())
	//	UIP_FriendSideMenu->DoMarkRedDot();
	//auto FriendDoc = UB2UIDocHelper::GetDocFriend();

	//TArray<int64> AllReadyNewCheck;
	//for (FB2FriendPtr TargetFriend : arFriendList)
	//{
	//	if (TargetFriend->is_new)
	//	{
	//		AllReadyNewCheck.Add(TargetFriend->user_index);
	//		if (FriendDoc) FriendDoc->OnForcedRemoveNewFriend(TargetFriend->user_index);
	//	}
	//}

	//if(AllReadyNewCheck.Num() > 0)
	//	data_trader::Retailer::GetInstance().RequestConfirmNewFriend(true, AllReadyNewCheck);
}

void UB2UIFriend::OnDeliveryCancelAskFriend()
{
	DeleteSavedFriendRow();

	// 模备府胶飘 绝阑锭 内膏飘眉农
	CheckNoFriendListComment();

	RefreshSendFriendAskCount();
}

void UB2UIFriend::OnDeliveryDeleteFriend(const FB2DeleteFriend& tuDeleteFriend)
{
	//m_nFriendDeleteRemainCount = GET_TUPLE_DATA(FB2ResponseDeleteFriend::left_delete_friend_count_index, tuDeleteFriend);

	//RefreshGameFriendCount(m_nFriendCount - 1);

	//// 模备府胶飘 绝阑锭 内膏飘眉农
	//CheckNoFriendListComment();

	//if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	//	FriendDoc->OnForcedDeleteFriendInfo(m_nSendFriendRowAccountId);

	//DeleteSavedFriendRow();
	//DoMarkRedDot();
	//if (UIP_FriendSideMenu.IsValid())
	//	UIP_FriendSideMenu->DoMarkRedDot();
}

void UB2UIFriend::OnDeliveryGetAllSendAskFriend(const FBAllSendAskFriendList& FriendList)
{
	//TArray<FB2UserPtr> arFriendList = GET_TUPLE_DATA(FB2ResponseGetAllSendAskFriend::acceptors_index, FriendList);

	//CreateFriendRows<FB2UserPtr>(arFriendList);

	//RefreshSendFriendAskCount();

	//// 模备府胶飘 绝阑锭 内膏飘眉农
	//CheckNoFriendListComment();
}

void UB2UIFriend::OnDeliveryGetAllReceiveAskFriend(const FB2AllReceiveAskFriendList& FriendList)
{
	//TArray<FB2UserPtr> arFriendList = GET_TUPLE_DATA(FB2ResponseGetAllReceiveAskFriend::requestors_index, FriendList);

	//CreateFriendRows<FB2UserPtr>(arFriendList);

	//RefreshReceiveFriendAskCount();

	//// 模备府胶飘 绝阑锭 内膏飘眉农
	//CheckNoFriendListComment();
	//DoMarkRedDot();
	//if (UIP_FriendSideMenu.IsValid())
	//	UIP_FriendSideMenu->DoMarkRedDot();
}

void UB2UIFriend::OnDeliverySendSocialPoint(const FB2SendSocialPoint& SendSocialPoint)
{
	//int32 nSocialPoint = GET_TUPLE_DATA(FB2ResponseSendSocialPoint::current_social_point_index, SendSocialPoint);
	//int32 nOldSocialPoint = BladeIIGameImpl::GetClientDataStore().GetSocialPoint();

	//BladeIIGameImpl::GetClientDataStore().SetSocialPoint(nSocialPoint);

	//TArray<int64> arSuccessIds = GET_TUPLE_DATA(FB2ResponseSendSocialPoint::success_account_ids_index, SendSocialPoint);
	//int64 nFailAccountId = GET_TUPLE_DATA(FB2ResponseSendSocialPoint::fail_account_id_index, SendSocialPoint);

	//if (nFailAccountId != 0 && arSuccessIds.Num() == 0)
	//{
	//	// 角菩茄局啊 乐澜
	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("FailToSendSocialPoint")),
	//		0,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm
	//	);
	//}
	//else
	//{
	//	int32 nEarnSocialPointWhenSend = BladeIIGameImpl::GetClientDataStore().GetAddSocialPointForSendToFriend();

	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("NotifySendSocialPoint")), FText::AsNumber(arSuccessIds.Num()), FText::AsNumber(arSuccessIds.Num() * nEarnSocialPointWhenSend)),
	//		0,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm
	//	);

	//	//////////////////////////////////////////////////////////////////
	//	//咀记 瘤钎 肺弊 (快沥 器牢飘 惯价)
	//	B2ActionLogSender Sender;
	//	Sender.setCategory(FString("FRIEND"))
	//		.setAction(FString("SOCIALPOINT_SEND"))
	//		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//	//////////////////////////////////////////////////////////////////
	//}

	//auto FriendDoc = UB2UIDocHelper::GetDocFriend();

	//// 滚瓢 惑怕 盎脚
	//for (UB2UIFriendRow* row : m_arFriendRows)
	//{
	//	for (int64 nAccountId : arSuccessIds)
	//	{
	//		if (row->GetFriendAccountId() == nAccountId)
	//		{
	//			int64 LastSendPointTimeDummy = UB2GameInstance::GetUTCNow().ToUnixTimestamp();
	//			row->SetSendPointTime(LastSendPointTimeDummy);
	//			row->CheckButtonState();
	//		
	//			if (FriendDoc) 
	//				FriendDoc->OnForcedFriendSocialPointInfo(nAccountId, LastSendPointTimeDummy);
	//		}
	//	}
	//}

	//DoMarkRedDot();
 //	if (UIP_FriendSideMenu.IsValid())
 //		UIP_FriendSideMenu->DoMarkRedDot();
}

void UB2UIFriend::OnDeliveryGetRecommendFriend(const FB2RecommendFriendList& FriendList)
{
	//TArray<FB2UserPtr> arFriendList = GET_TUPLE_DATA(FB2ResponseGetRecommendFriend::recommended_users_index, FriendList);

	//CreateFriendRows<FB2UserPtr>(arFriendList);
}

void UB2UIFriend::OnDeliveryFindUser(const FB2FindUser& FindUser)
{
	TArray<FB2UserPtr> arFriendList;
	//FB2UserPtr ptrUser = GET_TUPLE_DATA(FB2ResponseFindUser::user_index, FindUser);
	//arFriendList.Add(ptrUser);

	//CreateFriendRows<FB2UserPtr>(arFriendList);
}

void UB2UIFriend::OnErrorHandle5513()
{
	FMsgPopupOnClick	Delegate;
	Delegate.BindLambda([this]() {
		data_trader::Retailer::GetInstance().RequestRejectAskFriend(m_nSendFriendRowAccountId);
	});
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
		BladeIIGetLOCText(FString(B2LOC_CAT_ERROR_CODE), TEXT("5513")),
		0,
		true, 
		true,
		EUIMsgPopupButtonGroup::Confirm,
		Delegate);
}

UB2UIFriendRow* UB2UIFriend::GetFriendRowUI(int64 nAccountId)
{
	for (int nCnt = 0; nCnt < m_arFriendRows.Num(); ++nCnt)
	{
		if (m_arFriendRows[nCnt] && m_arFriendRows[nCnt]->GetFriendAccountId() == nAccountId)
			return m_arFriendRows[nCnt];
	}

	return NULL;
}

void UB2UIFriend::DeleteSavedFriendRow()
{
	if (m_nSendFriendRowAccountId == -1)
		return;

	// 瘤快磊
	UB2UIFriendRow* pUI = GetFriendRowUI(m_nSendFriendRowAccountId);
	m_arFriendRows.Remove(pUI);
	pUI->DestroySelf(NULL);

	m_nSendFriendRowAccountId = -1;
}

void UB2UIFriend::SetFriendTabState(FFriendTab* pFriendTab, EFriendTabState TabState)
{
	if (pFriendTab->Button == nullptr)
		return;
	switch (TabState)
	{
	case EFriendTabState::EFTS_Selected:
		pFriendTab->Button->SetVisibility(ESlateVisibility::Hidden);
		pFriendTab->SelectedImage->SetVisibility(ESlateVisibility::Visible);
		pFriendTab->UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
		pFriendTab->SelectedText->SetVisibility(ESlateVisibility::Visible);
		pFriendTab->UnselectedText->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EFriendTabState::EFTS_Unselected:
		pFriendTab->Button->SetVisibility(ESlateVisibility::Visible);
		pFriendTab->SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		pFriendTab->UnselectedImage->SetVisibility(ESlateVisibility::Visible);
		pFriendTab->SelectedText->SetVisibility(ESlateVisibility::Hidden);
		pFriendTab->UnselectedText->SetVisibility(ESlateVisibility::Visible);
		break;
	case EFriendTabState::EFTS_Hiddne:
		pFriendTab->Button->SetVisibility(ESlateVisibility::Hidden);
		pFriendTab->SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		pFriendTab->UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
		pFriendTab->SelectedText->SetVisibility(ESlateVisibility::Hidden);
		pFriendTab->UnselectedText->SetVisibility(ESlateVisibility::Hidden);
		break;
	}
}

void UB2UIFriend::ResetFriendRows()
{
	// 胶农费冠胶 厚况林磊
	for (int nCnt = 0; nCnt < m_arFriendRows.Num(); ++nCnt)
	{
		if (m_arFriendRows[nCnt])
		{
			// UI概聪廉甫 烹秦 父甸瘤 臼疽澜
			m_arFriendRows[nCnt]->DestroySelf(NULL);
		}
	}
	m_arFriendRows.Empty();

	SB_Main->SetAnimateScroll(false);
	SB_Main->SetScrollOffset(0.0f);
}

void UB2UIFriend::RefreshReceiveFriendAskCount()
{
	int32 nRcvCnt = m_arFriendRows.Num();
	int32 nMaxRcvCnt = BladeIIGameImpl::GetClientDataStore().GetMaxReceiveAskFriend();

	TB_RcvFriendCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Friend_ColoredManCountMaxManCount")), FText::AsNumber(nRcvCnt), FText::AsNumber(nMaxRcvCnt)));
}

void UB2UIFriend::RefreshSendFriendAskCount()
{
	int32 nSndCnt = m_arFriendRows.Num();
	int32 nMaxSndCnt = BladeIIGameImpl::GetClientDataStore().GetMaxSendAskFriend();

	TB_SndFriendCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Friend_ColoredManCountMaxManCount")), FText::AsNumber(nSndCnt), FText::AsNumber(nMaxSndCnt)));
}

void UB2UIFriend::CheckNoFriendListComment()
{
	if (m_arFriendRows.Num() == 0)
	{
		FString strTextKey;

		switch (m_eSubState)
		{
		case EFriendUISubState::EFUSS_MF_MyGameFriendListView:
			strTextKey = TEXT("NoGameFriendList");
			break;
		case EFriendUISubState::EFUSS_FF_ReceiveFriendListView:
			strTextKey = TEXT("NoReceiveFriendAskList");
			break;
		case EFriendUISubState::EFUSS_FF_SendFriendListView:
			strTextKey = TEXT("NoSendFriendAskList");
			break;
		default:
			strTextKey = TEXT("");
			break;
		}
		TB_NoFriendCommnet->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, strTextKey));
		G_NoFriend->SetVisibility(ESlateVisibility::Visible);
	}
	else
		G_NoFriend->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIFriend::SetFriendDeleteMode(bool bNewDeleteMode)
{
	m_bIsFriendDeleteMode = bNewDeleteMode;

	//厚劝己拳 绢骂窍瘤;
	if (bNewDeleteMode)
	{
		BTN_SendPointAll->SetIsEnabled(false);
		// 滚瓢苞 角力焊捞绰 捞固瘤啊 促抚. 厚劝己拳 捞固瘤肺 胶困莫
		IMG_SendPointAllBtn->SetVisibility(ESlateVisibility::Hidden);
		IMG_SendPointAllBtnDisable->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BTN_SendPointAll->SetIsEnabled(true);
		// 滚瓢苞 角力焊捞绰 捞固瘤啊 促抚. 劝己拳 捞固瘤肺 胶困莫
		IMG_SendPointAllBtn->SetVisibility(ESlateVisibility::Visible);
		IMG_SendPointAllBtnDisable->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIFriend::SetTimerRefreshFriend()
{
	RefreshTimeCount = 0;
	if (BTN_Refresh.IsValid())
		BTN_Refresh->SetIsEnabled(false);

	const int32 FreshTimeMax = 3;
	RefreshTimeCount = FreshTimeMax;

	UpdateRefreshFriendButtons(false);
	UpdateRefreshFriendTexts(RefreshTimeCount, false);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeToRefreshFriend, this, &UB2UIFriend::UpdateRefreshTimeCount, 1.0f, true);
}

void UB2UIFriend::SetTimerFindFriend()
{
	FindTimeCount = 0;
	if (BTN_Find.IsValid())
		BTN_Find->SetIsEnabled(false);

	const int32 FreshTimeMax = 3;
	FindTimeCount = FreshTimeMax;

	UpdateFindFriendButtons(false);
	UpdateFindFriendTexts(FindTimeCount, false);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeToFindFriend, this, &UB2UIFriend::UpdateFindTimeCount, 1.0f, true);

}

void UB2UIFriend::SetTimerLastLogoutTimer()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeToFindFriend, this, &UB2UIFriend::UpdateLastLogoutTimes, 5.0f, true);
}

void UB2UIFriend::UpdateRefreshFriendButtons(bool bTimeOver)
{
	if (bTimeOver)
	{
		if (BTN_Refresh.IsValid())
			BTN_Refresh->SetIsEnabled(true);

		ReleaseRefreshTimer();
	}
}

void UB2UIFriend::UpdateRefreshFriendTexts(const int32 nCount, const bool bTimeOver)
{
	if (STB_Refresh.IsValid())
		STB_Refresh->SetVisibility(bTimeOver ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	if (bTimeOver == false)
	{
		if (STB_Refresh.IsValid())
			STB_Refresh->SetText(FText::AsNumber(nCount));
	}
}

void UB2UIFriend::UpdateFindFriendButtons(bool bTimeOver)
{
	if (bTimeOver)
	{
		if (BTN_Find.IsValid())
			BTN_Find->SetIsEnabled(true);

		ReleaseFindTimer();
	}
}

void UB2UIFriend::UpdateFindFriendTexts(const int32 nCount, const bool bTimeOver)
{
	if (bTimeOver == false)
	{
		if (STB_Find.IsValid())
			STB_Find->SetText(FText::AsNumber(nCount));
	}
	else
	{
		if (STB_Find.IsValid())
			STB_Find->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Search")));
	}
}

void UB2UIFriend::UpdateRefreshTimeCount()
{
	--RefreshTimeCount;
	bool bTimeOver = RefreshTimeCount <= 0;

	UpdateRefreshFriendButtons(bTimeOver);
	UpdateRefreshFriendTexts(RefreshTimeCount, bTimeOver);
}

void UB2UIFriend::UpdateFindTimeCount()
{
	--FindTimeCount;
	bool bTimeOver = FindTimeCount <= 0;

	UpdateFindFriendButtons(bTimeOver);
	UpdateFindFriendTexts(FindTimeCount, bTimeOver);
}

void UB2UIFriend::ReleaseRefreshTimer()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToRefreshFriend);
}

void UB2UIFriend::UpdateLastLogoutTimes()
{
	for (int nCnt = 0; nCnt < m_arFriendRows.Num(); ++nCnt)
	{
		if (m_arFriendRows[nCnt])
		{
			m_arFriendRows[nCnt]->UpdateLastLogoutTime();
		}
	}
}

void UB2UIFriend::ReleaseFindTimer()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToFindFriend);
}
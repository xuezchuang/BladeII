// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFriendSideMenu.h"
#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIDocHelper.h"

#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2RedDotManager.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "Retailer.h"
#include "B2PlatformAPI.h"

void UB2UIFriendSideMenuRow::Init()
{
	Super::Init();

	bIsEnabledState = false;
}


void UB2UIFriendSideMenuRow::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_MyFriendList_Unselected);
	GET_SLOT(UImage, IMG_MyFriendList_Selected);

	GET_SLOT(UTextBlock, TB_MyFriendList_Unselected);
	GET_SLOT(UTextBlock, TB_MyFriendList_Selected);

	GET_SLOT(UB2Button, BTN_MyFriendList);

	GET_SLOT(UB2UnitedWidgetBase, UIP_Reddot_FriendList);

	SetButtonEnabled(false);
}

void UB2UIFriendSideMenuRow::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_MyFriendList, &UB2UIFriendSideMenuRow::OnButtonCallback);
}

void UB2UIFriendSideMenuRow::UpdateStaticText()
{
	Super::UpdateStaticText();

	FText MenuName;
	switch (SideMenuType)
	{
		case EB2UIFriendSideMenu::MyFriendList:
			{
				MenuName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Friend_List"));
			}
			break;
		case EB2UIFriendSideMenu::FindFriend:
			{
				MenuName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Friend_Sign"));
			}
			break;
		case EB2UIFriendSideMenu::MyKakaoFriendList:
			{
				MenuName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Friend_KakaoFriendList"));
			}
			break;
		case EB2UIFriendSideMenu::KakaoInvite:
			{
				MenuName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Friend_KakaoFriendInvite"));
			}
			break;
		default:
			{
				check(!TEXT("Check Side Menu Type"));
			}
			break;
	}


	if (TB_MyFriendList_Unselected.IsValid())
		TB_MyFriendList_Unselected->SetText(MenuName);
	if (TB_MyFriendList_Selected.IsValid())
		TB_MyFriendList_Selected->SetText(MenuName);
}

void UB2UIFriendSideMenuRow::SetButtonEnabled(bool InEnabled)
{
	bIsEnabledState = InEnabled;
	UpdateButtonState();
}

void UB2UIFriendSideMenuRow::UpdateButtonState()
{
	ESlateVisibility TBEnableState = bIsEnabledState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
	ESlateVisibility TBDisableState = bIsEnabledState ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible;

 	if (IMG_MyFriendList_Selected.IsValid())
 		IMG_MyFriendList_Selected->SetVisibility(TBEnableState);
	if (TB_MyFriendList_Selected.IsValid())
		TB_MyFriendList_Selected->SetVisibility(TBEnableState);

 	if (IMG_MyFriendList_Unselected.IsValid())
 		IMG_MyFriendList_Unselected->SetVisibility(TBDisableState);
	if (TB_MyFriendList_Unselected.IsValid())
		TB_MyFriendList_Unselected->SetVisibility(TBDisableState);
}

bool UB2UIFriendSideMenuRow::GetIsButtonEnabled()
{
	return bIsEnabledState;
}

void UB2UIFriendSideMenuRow::OnButtonCallback()
{
	if (ParentMenu.IsValid())
	{
		ParentMenu->OnMenuSelect(SideMenuType);
	}
}

void UB2UIFriendSideMenuRow::SetUIParentInfo(const UB2UIFriendSideMenu *Parent)
{
	//ParentMenu = Parent;
}


////////////////////////////////////////////////////////////////////////////
// UB2UIFriendSideMenu
////////////////////////////////////////////////////////////////////////////
void UB2UIFriendSideMenu::Init()
{
	Super::Init();
}

void UB2UIFriendSideMenu::CacheAssets()
{
	Super::CacheAssets();
}

void UB2UIFriendSideMenu::BindDelegates()
{
	Super::BindDelegates();

	MenuTabs.SetNum(static_cast<uint8>(EB2UIFriendSideMenu::End));
	GET_SLOT_BYNAME(UB2UIFriendSideMenuRow, MenuTabs[static_cast<uint8>(EB2UIFriendSideMenu::MyFriendList)], UIP_TabMyFriendList);
	GET_SLOT_BYNAME(UB2UIFriendSideMenuRow, MenuTabs[static_cast<uint8>(EB2UIFriendSideMenu::FindFriend)], UIP_TabFindFriend);
	GET_SLOT_BYNAME(UB2UIFriendSideMenuRow, MenuTabs[static_cast<uint8>(EB2UIFriendSideMenu::MyKakaoFriendList)], UIP_TabKakaoFriendList);
	GET_SLOT_BYNAME(UB2UIFriendSideMenuRow, MenuTabs[static_cast<uint8>(EB2UIFriendSideMenu::KakaoInvite)], UIP_TabKakaoInvite);

	for (const auto &Each : MenuTabs)
	{
		if (Each.Get())
		{
			Each->Init();
			Each->SetUIParentInfo(this);
		}
	}

 	int32 FirstIndex = static_cast<uint8>(EB2UIFriendSideMenu::MyFriendList);
 	if(MenuTabs.IsValidIndex(FirstIndex))
 		MenuTabs[static_cast<uint8>(EB2UIFriendSideMenu::MyFriendList)]->SetButtonEnabled(true);

#if PLATFORM_ANDROID || PLATFORM_IOS
	if (B2P_GetKakaoIDPCode() == EPlatformIDPCode::None || B2P_GetKakaoIDPCode() == EPlatformIDPCode::Guest)
	{
		if (MenuTabs[static_cast<uint8>(EB2UIFriendSideMenu::KakaoInvite)].IsValid())
			MenuTabs[static_cast<uint8>(EB2UIFriendSideMenu::KakaoInvite)]->SetVisibility(ESlateVisibility::Collapsed);
	}
#endif
}

void UB2UIFriendSideMenu::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UIFriendSideMenu::OnMenuSelect(EB2UIFriendSideMenu SelectMenu)
{
	for (const auto &Each : MenuTabs)
	{
		if (Each.Get())
		{
			Each->SetButtonEnabled(false);
		}
	}

	const int32 SelectIndex = static_cast<uint8>(SelectMenu);
	if (MenuTabs.IsValidIndex(SelectIndex))
	{
		MenuTabs[static_cast<uint8>(SelectMenu)]->SetButtonEnabled(true);
		OnFriendSideMenuSelectClass<int32>::GetInstance().Signal(SelectIndex);
	}
}

void UB2UIFriendSideMenu::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("MyFriend")), &UB2UIFriendSideMenu::RedDotCondition_MyFriends);

	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("FindFriend")), &UB2UIFriendSideMenu::RedDotCondition_RequestFriends);
}

bool UB2UIFriendSideMenu::RedDotCondition_SendAbleFriendPoint()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
		return FriendDoc->GetAbleToSendFriendPoint();

	return false;
}

bool UB2UIFriendSideMenu::RedDotCondition_MyFriends()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
		return FriendDoc->GetIsNewFriend() || FriendDoc->GetAbleToSendFriendPoint();

	return false;
}

bool UB2UIFriendSideMenu::RedDotCondition_RequestFriends()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		if (BladeIIGameImpl::GetClientDataStore().GetMaxFriendCount() <= FriendDoc->GetFriendList().Num())
			return false;
		return FriendDoc->GetIsAskFriend();
	}
	return false;
}
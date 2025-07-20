#include "B2UIFriendRow.h"
#include "Retailer.h"
#include "B2UIFriend.h"

#include "B2UIManager.h"
#include "BladeIIGameMode.h"
#include "B2PCClassInfoBox.h"
#include "B2InGameHUDInfo_Player.h"
#include "B2UIMsgPopupUserInfo.h"
#include "B2UIHeader.h"
#include "B2GameInstance.h"
#include "B2GameInstance.h"
#include "BladeIIGameImpl.h"

void UB2UIFriendRow::Init()
{
	Super::Init();

	m_nSendPointTime = 0;
}

// 模备沥焊 技泼 捞饶俊 龋免秦具窃
void UB2UIFriendRow::SetOwnerUI(UB2UIFriend* pOwner)
{
	m_pOwnerUI = pOwner;
	
	CheckButtonState();
}

void UB2UIFriendRow::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Image, IMG_Character);

	GET_SLOT(UTextBlock, TB_Guild);
	GET_SLOT(UTextBlock, TB_PlayerLevel);
	GET_SLOT(UTextBlock, TB_PlayerName);
	GET_SLOT(UTextBlock, TB_SendPointTimeBtn);
	GET_SLOT(UTextBlock, TB_LoginTime);

	GET_SLOT(UB2Button, BTN_WholeArea);
	GET_SLOT(UB2Button, BTN_Ask);
	GET_SLOT(UB2Button, BTN_SendPoint);
	GET_SLOT(UB2Button, BTN_SendPointTime);
	GET_SLOT(UB2Button, BTN_Delete);
	GET_SLOT(UB2Button, BTN_Cancel);
	GET_SLOT(UB2Button, BTN_Reject);
	GET_SLOT(UB2Button, BTN_Accept);
	GET_SLOT(UB2Button, BTN_KakaoInvite);

	GET_SLOT(UTextBlock, TB_Request);
	GET_SLOT(UTextBlock, TB_SendSocialPoint);
	GET_SLOT(UTextBlock, TB_Cancel);
	GET_SLOT(UTextBlock, TB_ConnectTime);
	GET_SLOT(UTextBlock, TB_Accept);
	GET_SLOT(UTextBlock, TB_Reject);
	GET_SLOT(UTextBlock, TB_Revocation);
	
	GET_SLOT(UB2UIWidgetBase, UIP_Reddot_NewFriend);
	BIND_CLICK_FUNC_TO_BTN(BTN_WholeArea, &UB2UIFriendRow::OnWholeAreaBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Ask, &UB2UIFriendRow::OnAskBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_SendPoint, &UB2UIFriendRow::OnSendPointBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Delete, &UB2UIFriendRow::OnDeleteBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UIFriendRow::OnCancelBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Reject, &UB2UIFriendRow::OnRejectBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Accept, &UB2UIFriendRow::OnAcceptBtnClick);
}

void UB2UIFriendRow::UpdateStaticText()
{
	if(TB_Request.IsValid())
		TB_Request->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Request")));

	if (TB_SendSocialPoint.IsValid())
		TB_SendSocialPoint->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_SendSocialPoint")));

	if (TB_Cancel.IsValid())
		TB_Cancel->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Cancel")));

	if (TB_ConnectTime.IsValid())
		TB_ConnectTime->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_ConnectTime")));

	if (TB_Accept.IsValid())
		TB_Accept->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("General_Accept")));

	if (TB_Reject.IsValid())
		TB_Reject->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("General_Reject")));

	if (TB_Revocation.IsValid())
		TB_Revocation->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_Revocation")));

}

void UB2UIFriendRow::BindDelegates()
{

}



void UB2UIFriendRow::CheckSocialPointSendTime()
{
	if (IsSocialPointSendable())
	{
		// 焊尘荐 乐澜
		BTN_SendPoint->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FDateTime timePointSend = FDateTime::FromUnixTimestamp(m_nSendPointTime);

		FDateTime ServerTimeNow = UB2GameInstance::GetUTCNow();

		FTimespan timeElapse = ServerTimeNow - timePointSend;

		int32 CooltimeForSendSocialPoint = BladeIIGameImpl::GetClientDataStore().GetCooltimeForSendSocialPoint();

		// 给焊晨 巢篮矫埃 钎扁
		BTN_SendPointTime->SetVisibility(ESlateVisibility::Visible);

		FTimespan timeRemain = FTimespan::FromHours(CooltimeForSendSocialPoint) - timeElapse;
				
		TB_SendPointTimeBtn->SetText(FText::FromString(timeRemain.ToString(TEXT("%h:%m:%s"))));

		GetWorld()->GetTimerManager().SetTimer(SocialPointTimer, this, &UB2UIFriendRow::CheckSocialPointSendTime, 1.0f, false);
	}
}

void UB2UIFriendRow::CheckButtonState()
{
	// 滚瓢 惑怕
	BTN_Ask->SetVisibility(ESlateVisibility::Hidden);
	BTN_SendPoint->SetVisibility(ESlateVisibility::Hidden);
	BTN_SendPointTime->SetVisibility(ESlateVisibility::Hidden);
	BTN_Delete->SetVisibility(ESlateVisibility::Hidden);
	BTN_Cancel->SetVisibility(ESlateVisibility::Hidden);
	BTN_Reject->SetVisibility(ESlateVisibility::Hidden);
	BTN_Accept->SetVisibility(ESlateVisibility::Hidden);

	if (!m_pOwnerUI)
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(SocialPointTimer);

	switch (m_pOwnerUI->GetSubState())
	{
	case EFriendUISubState::EFUSS_MF_MyGameFriendListView:
		// 昏力葛靛
		if (m_pOwnerUI->IsFriendDeleteMode())
			BTN_Delete->SetVisibility(ESlateVisibility::Visible);
		else
		{
			CheckSocialPointSendTime();
		}		
		break;
	case EFriendUISubState::EFUSS_FF_ReceiveFriendListView:
		BTN_Reject->SetVisibility(ESlateVisibility::Visible);
		BTN_Accept->SetVisibility(ESlateVisibility::Visible);
		break;
	case EFriendUISubState::EFUSS_FF_SendFriendListView:
		BTN_Cancel->SetVisibility(ESlateVisibility::Visible);
		break;
	case EFriendUISubState::EFUSS_FF_RecommendFriendListView:
		BTN_Ask->SetVisibility(ESlateVisibility::Visible);
		break;
	}
}

bool UB2UIFriendRow::IsSocialPointSendable()
{
	FDateTime timePointSend = FDateTime::FromUnixTimestamp(m_nSendPointTime);

	FDateTime ServerTimeNow = UB2GameInstance::GetUTCNow();

	FTimespan timeElapse = ServerTimeNow - timePointSend;

	int32 CooltimeForSendSocialPoint = BladeIIGameImpl::GetClientDataStore().GetCooltimeForSendSocialPoint();
	
	return timeElapse >= FTimespan::FromHours(CooltimeForSendSocialPoint);
}

void UB2UIFriendRow::SetSendPointTime(int64 nUnixTimeSec)
{
	m_nSendPointTime = nUnixTimeSec;
}

void UB2UIFriendRow::OnWholeAreaBtnClick()
{
	// 模备 规巩栏肺 楷搬
	/*UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
		pUIManager->OpenMsgPopup(
		EUIMsgPopup::Center, 
		FText::FromString(TEXT("Go to Visit Friend Scene")), 
		FText::FromString(TEXT("")), 
		1.0f);*/


	FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Signal(m_nAccountId, EPCClass::EPC_End, EPCClass::EPC_End, EPCClass::EPC_End);
}

void UB2UIFriendRow::SetFriendContent(FB2FriendPtr ptrFriend)
{
	if (ptrFriend.get() == nullptr)
		return;

	m_nSendPointTime = ptrFriend->last_social_point_send_time / 1000;
	
	SetFriendContent(ptrFriend->user);

	// 货模备
	if (UIP_Reddot_NewFriend.IsValid())
		UIP_Reddot_NewFriend->SetVisibility(ptrFriend->is_new ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UIFriendRow::SetFriendContent(FB2UserPtr ptrUser)
{
	if (ptrUser.get() == nullptr)
		return;

	// FB2UserPtr俊父 乐绰 沥焊 技泼
	FString szTest = ptrUser->guild_name;

	TB_Guild->SetText(FText::FromString(szTest));

	
	SetFriendContent(ptrUser->character_summary);
	SetLastLogOutTime(ptrUser->last_logout_time);
}

void UB2UIFriendRow::SetFriendContent(FB2CharacterSummaryPtr ptrCharacterSummary)
{
	if (ptrCharacterSummary.get() == nullptr)
		return;

	// FB2CharacterSummaryPtr俊 乐绰沥焊 技泼
	// 咯变 蝶肺 滑鞘夸 绝瘤父.. 趣矫葛福聪 盒府
	m_nAccountId = ptrCharacterSummary->account_id;

	//檬惑拳
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));

	if (B2GI)
	{		
		UB2InGameHUDInfo_Player* pHUDInfo_Player = B2GI->GetPlayerHUDInfo(SvrToCliPCClassType(ptrCharacterSummary->character_type));
		if (pHUDInfo_Player)
		{
			UMaterialInstance* pMI = Cast<UMaterialInstance>(pHUDInfo_Player->GetPlayerPortrait(this));
			if (pMI)
				IMG_Character->SetBrushFromMaterial(pMI);
		}
	}

	//饭骇
	FText txtLevel = FText::Format(FText::FromString(TEXT("LV.{0}")), FText::AsNumber(ptrCharacterSummary->character_level));
	TB_PlayerLevel->SetText(txtLevel);

	//捞抚
	TB_PlayerName->SetText(FText::FromString(ptrCharacterSummary->account_name));

	// 弥辆 立加矫埃 固备泅

	// 辨靛 固备泅
}

void UB2UIFriendRow::SetLastLogOutTime(int64 LogOutTime)
{
	m_nLastLogoutTime = LogOutTime;
	UpdateLastLogoutTime();
}

int64 UB2UIFriendRow::GetFriendAccountId()
{
	return m_nAccountId;
}

void UB2UIFriendRow::OnAskBtnClick()
{
	m_pOwnerUI->SetSendFriendRowAccountId(m_nAccountId);

	data_trader::Retailer::GetInstance().RequestAskFriend(m_nAccountId);
}

void UB2UIFriendRow::OnSendPointBtnClick()
{
	TArray<int64> ar_account_id;
	ar_account_id.AddDefaulted(1);
	ar_account_id[0] = m_nAccountId;

	// 窍唱焊郴扁 绢饭捞俊 夯牢波父 呈辑焊郴搁 达
	data_trader::Retailer::GetInstance().RequestSendSocialPoint(ar_account_id);
}

void UB2UIFriendRow::OnDeleteBtnClick()
{
	int32 nRemainDeleteCount = m_pOwnerUI->GetFriendDeleteRemainCount();

	if (nRemainDeleteCount == 0)
	{
		int32 nMaxDeleteCount = BladeIIGameImpl::GetClientDataStore().GetMaxDeleteFriendCountForDay();

		// 模备 秦瘤绰 窍风俊 10疙鳖瘤父 啊瓷钦聪促 坷疵巢篮模备秦瘤冉荐:0雀
		UB2UIManager* pUIManager = UB2UIManager::GetInstance();
		UB2UIMsgPopupSimpleAddComment* pPopupUI = pUIManager->OpenMsgPopup<UB2UIMsgPopupSimpleAddComment>(EUIMsgPopup::SimpleAddComment,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("NotifyFriendDeleteLimit")), FText::AsNumber(nMaxDeleteCount)));
		
		// 眠啊内膏飘
		pPopupUI->SetAddComment(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TodayFriendDeleteCountRedNum")), FText::AsNumber(nRemainDeleteCount)));

		return;
	}
	else
	{
		//模备甫 秦瘤窍矫摆嚼聪鳖? 秒家/犬牢
		//OnConfirmDelete 胆府霸捞飘肺 呈拎具窃
		//困连累诀捞 救登辑 缴敲肺 老窜
		UB2UIManager* pUIManager = UB2UIManager::GetInstance();
		UB2UIMsgPopupUserInfo* pPopupUI = pUIManager->OpenMsgPopup<UB2UIMsgPopupUserInfo>(EUIMsgPopup::UserInfo,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ConfirmDeleteFriend")),
			0,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([this](){

			this->OnConfirmDelete();

		})
			);

		// 蜡历沥焊 技泼
		pPopupUI->SetUserInfo(Cast<UMaterialInstance>(IMG_Character->GetDynamicMaterial()),
			TB_PlayerName->GetText(), TB_PlayerLevel->GetText(), TB_Guild->GetText());
		
		// 眠啊内膏飘
		pPopupUI->SetAddComment(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TodayFriendDeleteCountGreenNum")), FText::AsNumber(nRemainDeleteCount)));

		return;
	}
}

void UB2UIFriendRow::OnCancelBtnClick()
{
	m_pOwnerUI->SetSendFriendRowAccountId(m_nAccountId);

	data_trader::Retailer::GetInstance().RequestCancelAskFriend(m_nAccountId);
}

void UB2UIFriendRow::OnRejectBtnClick()
{
	m_pOwnerUI->SetSendFriendRowAccountId(m_nAccountId);

	data_trader::Retailer::GetInstance().RequestRejectAskFriend(m_nAccountId);
}

void UB2UIFriendRow::OnAcceptBtnClick()
{
	m_pOwnerUI->SetSendFriendRowAccountId(m_nAccountId);

	data_trader::Retailer::GetInstance().RequestAcceptAskFriend(m_nAccountId);
}

void UB2UIFriendRow::OnConfirmDelete()
{
	m_pOwnerUI->SetSendFriendRowAccountId(m_nAccountId);

	data_trader::Retailer::GetInstance().RequestDeleteFriend(m_nAccountId);
}

void UB2UIFriendRow::UpdateLastLogoutTime()
{
	if (TB_LoginTime.IsValid() == false)
		return;

	FString StrDay, StrHour, StrMin;

	FTimespan RemainTime = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(m_nLastLogoutTime / 1000);

	int32 RemainDay = RemainTime.GetDays();
	int32 RemainHour = RemainTime.GetHours();
	int32 RemainMin = RemainTime.GetMinutes();

	if (!RemainDay && !RemainHour && !RemainMin)
		RemainMin = 1;

	if (RemainDay != 0)
		StrDay = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Day")), FText::FromString(FString::FromInt(RemainDay))).ToString();

	if (RemainHour != 0)
		StrHour = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), FText::FromString(FString::FromInt(RemainHour))).ToString();

	if (RemainMin != 0)
		StrMin = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::FromString(FString::FromInt(RemainMin))).ToString();

	FString strTime;

	if (RemainDay == 0)
	{
		if (RemainHour == 0)
			strTime = FString::Printf(TEXT("%s"), *StrMin);
		else
			strTime = FString::Printf(TEXT("%s %s"), *StrHour, *StrMin);
	}
	else
	{
		if (RemainHour == 0)
			strTime = FString::Printf(TEXT("%s"), *StrDay);
		else
			strTime = FString::Printf(TEXT("%s %s"), *StrDay, *StrHour);
	}

	TB_LoginTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_ContactTime")), FText::FromString(strTime)));
}
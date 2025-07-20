// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlUserReportPopup.h"
#include "B2UIManager.h"

#include "B2ControlGameMode.h"
#include "BladeIINetPlayer.h"
#include "B2UIDocHelper.h"
#include "BladeIIGameImpl.h"
#include "B2UIUserReport.h"
#include "B2GameDefine.h"


const int32 MAX_CONTROL_TEAM_PER_COUNT = static_cast<int32>(CONTROL_GAME_MAX_PLAYERS / 2);

UB2UIControlUserReportPopup::UB2UIControlUserReportPopup(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	UseRepotUser.Empty();

}

void UB2UIControlUserReportPopup::Init()
{
	Super::Init();
	//SetControlUserReportPopup();

	//CAPTURE_UOBJECT(UB2UIControlUserReportPopup);

	//UserReportTicket = DeliveryReportUserClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(UserReport)
	//	Capture->UserReportComplete();
	//END_CAPTURE_OBJECT()
	//	);

}

void UB2UIControlUserReportPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	auto ControlDoc = UB2UIDocHelper::GetDocControl();

	if (ControlDoc)
		ControlDoc->SetUserReportUser(UseRepotUser);


	//DeliveryReportUserClass<>::GetInstance().Unsubscribe(UserReportTicket);
}

void UB2UIControlUserReportPopup::SetControlUserReportPopup()
{
	SlotSetting();
	EnAlbeSettting();
}

void UB2UIControlUserReportPopup::UserReportComplete()
{
	auto DocSome = UB2UIDocHelper::GetDocSome();

	UB2UIManager::GetInstance()->CloseUI(UIFName::UserReportPopup);

	if (DocSome)
	{
		int64 UserAccountID = DocSome->GetSendReportUserAccountID();

		if(!UseRepotUser.Contains(UserAccountID))
			UseRepotUser.Add(DocSome->GetSendReportUserAccountID(), DocSome->GetSendReportUserAccountID());
		EnAlbeSettting();
		DevShowNotReadyYetMessage(TEXT("General_ReportComplete"));
	}
}

void UB2UIControlUserReportPopup::SlotSetting()
{
	TArray<class ABladeIINetPlayer*> RedTeamArr;
	TArray<class ABladeIINetPlayer*> BlueTeamArr;

	auto ControlDoc = UB2UIDocHelper::GetDocControl();

	if (ControlDoc)
		UseRepotUser = ControlDoc->GetUserReportUser();


	AB2ControlGameMode* ControlGamemode = Cast<AB2ControlGameMode>(GetWorld()->GetAuthGameMode());

	if (ControlGamemode == nullptr)
		return;

	RedTeamArr = ControlGamemode->GetTeamPlayerArray(ETeamType::ETT_Red);
	BlueTeamArr = ControlGamemode->GetTeamPlayerArray(ETeamType::ETT_Blue);


	for (int32 i = 0; i < MAX_CONTROL_TEAM_PER_COUNT; ++i)
	{
		if (RedTeamArr.Num() > i)
		{
			RedTeam[i]->SetUserReportSlot(RedTeamArr[i]->GetCharacterDataStore()->GetMainPlayerClass(),
				RedTeamArr[i]->GetAccountId(), RedTeamArr[i]->LocalizedUserNickName);

		}
		else
			RedTeam[i]->SetVisibility(ESlateVisibility::Hidden);

		if (BlueTeamArr.Num() > i)
		{
			BlueTeam[i]->SetUserReportSlot(BlueTeamArr[i]->GetCharacterDataStore()->GetMainPlayerClass(),
				BlueTeamArr[i]->GetAccountId(), BlueTeamArr[i]->LocalizedUserNickName);
		}
		else
			BlueTeam[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	int64 MyAccount = BladeIIGameImpl::GetClientDataStore().GetAccountId();

	if (!UseRepotUser.Contains(MyAccount))
		UseRepotUser.Add(MyAccount);
	
}

void UB2UIControlUserReportPopup::EnAlbeSettting()
{
	for (int32 i = 0; i < MAX_CONTROL_TEAM_PER_COUNT; ++i)
	{
		BlueTeam[i]->SetIsEnable(true);
		RedTeam[i]->SetIsEnable(true);

		for (auto El : UseRepotUser)
		{

			if (RedTeam[i]->GetAccount() == El.Key)
			{
				RedTeam[i]->SetIsEnable(false);
			}
			if (BlueTeam[i]->GetAccount() == El.Key)
			{
				BlueTeam[i]->SetIsEnable(false);
			}
		}
	}
}

void UB2UIControlUserReportPopup::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ControlUserReportPopup);
}

void UB2UIControlUserReportPopup::CacheAssets()
{
	RedTeam.SetNum(MAX_CONTROL_TEAM_PER_COUNT);
	BlueTeam.SetNum(MAX_CONTROL_TEAM_PER_COUNT);

	GET_SLOT(UB2Button, BTN_Close);

	for (int32 i = 0; i < MAX_CONTROL_TEAM_PER_COUNT; i++)
	{
		RedTeam[i] = GetCachedWidget<UB2UIControlUserReportPopupSlot>(FName(*FString::Printf(TEXT("UIP_Red%d"), i + 1)));
		BlueTeam[i] = GetCachedWidget<UB2UIControlUserReportPopupSlot>(FName(*FString::Printf(TEXT("UIP_Blue%d"), i + 1)));
	}
	GET_SLOT(UTextBlock, TB_Title);

	for (int32 i = 0; i < MAX_CONTROL_TEAM_PER_COUNT; ++i)
	{
		if(RedTeam[i].IsValid())
			RedTeam[i]->Init();
		if(BlueTeam[i].IsValid())
			BlueTeam[i]->Init();
	}

}

void UB2UIControlUserReportPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIControlUserReportPopup::OnClickClose);
}

void UB2UIControlUserReportPopup::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Report")));
}

UB2UIControlUserReportPopupSlot::UB2UIControlUserReportPopupSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}
void UB2UIControlUserReportPopupSlot::Init()
{
	Super::Init();
}

void UB2UIControlUserReportPopupSlot::CacheAssets()
{

	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UTextBlock, TB_NickName);
	GET_SLOT(UImage, IMG_Character);
}

void UB2UIControlUserReportPopupSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIControlUserReportPopupSlot::OnClickSelect);
}

void UB2UIControlUserReportPopupSlot::UpdateStaticText()
{

}

void UB2UIControlUserReportPopupSlot::SetUserReportSlot(EPCClass InClass, int64 InAccountID, FText NickName)
{

	if(IMG_Character.IsValid())
		IMG_Character->SetBrushFromMaterial(CharterTexture[static_cast<int32>(InClass)]);

	if (TB_NickName.IsValid())
		TB_NickName->SetText(NickName);


	AccountID = InAccountID;

	if (BladeIIGameImpl::GetClientDataStore().GetAccountId() == AccountID)
	{
		SetIsEnable(false);
	}
}

void UB2UIControlUserReportPopupSlot::SetIsEnable(bool InEnable)
{
	if (BTN_Select.IsValid())
		BTN_Select->SetIsEnabled(InEnable);
}

void UB2UIControlUserReportPopupSlot::OnClickSelect()
{
	UB2UIUserReport* UserReportPopup = UB2UIManager::GetInstance()->OpenUI<UB2UIUserReport>(UIFName::UserReportPopup);

	if (UserReportPopup)
		UserReportPopup->UpdatePopup(b2network::B2ReportModeType::ASSAULT, AccountID, TB_NickName->GetText().BuildSourceString());
}

// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UICreateNickname.h"
#include "B2UIHeader.h"
#include "Retailer.h"
#include "B2PlatformAPI.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UICreateNickname::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UOverlay, O_Content);
	GET_SLOT(UOverlay, O_ContentInput);
	GET_SLOT(UOverlay, O_Btn_Create);
	GET_SLOT(UOverlay, O_Btn_Two);
	GET_SLOT(UOverlay, O_Btn_Confirm);
	GET_SLOT(UOverlay, O_Warning);
	GET_SLOT(UOverlay, O_Btn_Changes);

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_FirstContent);
	GET_SLOT(UTextBlock, TB_SecondContent);
	GET_SLOT(UTextBlock, TB_InputContent);
	GET_SLOT(UTextBlock, TB_CreateBtn);
	GET_SLOT(UTextBlock, TB_OKBtn);
	GET_SLOT(UTextBlock, TB_CancleBtn);
	GET_SLOT(UTextBlock, TB_ConfirmBtn);
	GET_SLOT(UTextBlock, TB_ChangeOkBtn);
	GET_SLOT(UTextBlock, TB_ChangeNoBtn);

	GET_SLOT(UB2Button, Btn_Create);
	GET_SLOT(UB2Button, Btn_Ok);
	GET_SLOT(UB2Button, Btn_Cancle);
	GET_SLOT(UB2Button, Btn_Confirm);

	GET_SLOT(UB2Button, Btn_Change_Ok);
	GET_SLOT(UB2Button, Btn_Change_No);

	GET_SLOT(UEditableText, ETB_Input);

	GET_SLOT(UB2RichTextBlock, RTB_Warning);
}

void UB2UICreateNickname::UpdateStaticText()
{
	if (TB_Title.IsValid()){
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_Title")));
	}
	if (TB_SecondContent.IsValid()){ 
		TB_SecondContent->SetText(FText::FromString(TEXT("")));
	}
	if (TB_InputContent.IsValid()){
		TB_InputContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_InputContent")));
	}
	if (TB_CreateBtn.IsValid()){ 
		TB_CreateBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_CreateBtn")));
	}
	if (TB_OKBtn.IsValid()){ 
		TB_OKBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_OKBtn")));
	}
	if (TB_CancleBtn.IsValid()){
		TB_CancleBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Cancel")));
	}
	if (TB_ConfirmBtn.IsValid()){
		TB_ConfirmBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	}
	if (TB_ChangeOkBtn.IsValid()) {
		TB_ChangeOkBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	}
	if (TB_ChangeNoBtn.IsValid()) {
		TB_ChangeNoBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Cancel")));
	}
	if (ETB_Input.IsValid()){
		ETB_Input->SetText(FText::FromString(TEXT("")));
	}
	if (ETB_Input.IsValid()){
		ETB_Input->SetHintText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_InputETB")));
	}
	
	if (RTB_Warning.IsValid())
	{
		RTB_Warning->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_Warning")));
	}
}

void UB2UICreateNickname::CloseWidgetDelegate()
{
	if (bIsChange)
	{
		switch (CurrentState)
		{
		case ECreateState::Change:
			OnClickBtn_ChangeNo();
			break;
		case ECreateState::Create:
		case ECreateState::Confirm:
		case ECreateState::WrongNick:
		case ECreateState::UseNick:
		case ECreateState::SameNick:
			OnClickBtn_Cancle();
			break;
		default:
			break;
		}
	}
	else
	{
		switch (CurrentState)
		{
		case ECreateState::Create:
			B2P_GameExitPopUp("byebye");
			break;
		case ECreateState::Confirm:
			OnClickBtn_Cancle();
			break;
		case ECreateState::WrongNick:
			B2P_GameExitPopUp("byebye");
			break;
		case ECreateState::UseNick:
			OnClickBtn_Confirm();
			break;
		default:
			break;
		}
	}
}

void UB2UICreateNickname::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(Btn_Create, &UB2UICreateNickname::OnClickBtn_Create);
	BIND_CLICK_FUNC_TO_BTN(Btn_Ok, &UB2UICreateNickname::OnClickBtn_Ok);
	BIND_CLICK_FUNC_TO_BTN(Btn_Cancle, &UB2UICreateNickname::OnClickBtn_Cancle);
	BIND_CLICK_FUNC_TO_BTN(Btn_Confirm, &UB2UICreateNickname::OnClickBtn_Confirm);
	BIND_CLICK_FUNC_TO_BTN(Btn_Change_Ok, &UB2UICreateNickname::OnClickBtn_ChangeOk);
	BIND_CLICK_FUNC_TO_BTN(Btn_Change_No, &UB2UICreateNickname::OnClickBtn_ChangeNo);
}

void UB2UICreateNickname::Init()
{
	Super::Init();

	NicknameChangeItemID = 0;
	bIsChange = false;
	SubscribeEvents();
	SetStateMode(ECreateState::Create);
}

void UB2UICreateNickname::DestroySelf(UB2UIManager* InUIManager)
{
	NicknameChangeItemID = 0;
	bIsChange = false;
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UICreateNickname::OnClickBtn_Create()
{
	const FText FtxtNick = ETB_Input.IsValid() ? ETB_Input->GetText() : FText::FromString(TEXT(""));

	if (!IsCheckWrong(FtxtNick))
	{
		SetStateMode(ECreateState::WrongNick);
	}
	else
	{
		if (TB_FirstContent.IsValid()){
			TB_FirstContent->SetText(FtxtNick);
			m_fstrNickName = FtxtNick.ToString();
		}

		SetStateMode(ECreateState::Confirm);
	}
}

void UB2UICreateNickname::OnClickBtn_Ok()
{
	if(bIsChange)
		ReQuestChangeNickName(ETB_Input->GetText().ToString());
	else
		ReQuestCreateNickName(ETB_Input->GetText().ToString());
}

void UB2UICreateNickname::OnClickBtn_Cancle()
{
	SetStateMode(bIsChange ? ECreateState::Change : ECreateState::Create);
}

void UB2UICreateNickname::OnClickBtn_Confirm()
{
	SetStateMode(bIsChange ? ECreateState::Change : ECreateState::Create);
}

void UB2UICreateNickname::OnClickBtn_ChangeOk()
{
	const FText FtxtNick = ETB_Input.IsValid() ? ETB_Input->GetText() : FText::FromString(TEXT(""));

	if (!IsCheckWrong(FtxtNick))
	{
		SetStateMode(ECreateState::WrongNick);
	}
	else
	{
		if (TB_FirstContent.IsValid()) {
			TB_FirstContent->SetText(FtxtNick);
			m_fstrNickName = FtxtNick.ToString();
		}

		if (IsCheckSame())
		{
			SetStateMode(ECreateState::SameNick);
			return;
		}
		SetStateMode(ECreateState::Confirm);
	}
}

void UB2UICreateNickname::OnClickBtn_ChangeNo()
{
	// 이름 변경권 사용하지 않음.
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("NameChange_Cancel"))),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
		
		UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
		if (HeaderUI)
			HeaderUI->HideHeader(false);

		UB2UIManager::GetInstance()->CloseUI(UIFName::CreateNickName);
	})
	);
}

void UB2UICreateNickname::SetStateMode(ECreateState eState)
{
	if (O_Content.IsValid()){
		O_Content->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (O_ContentInput.IsValid()){
		O_ContentInput->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (O_Btn_Create.IsValid()){
		O_Btn_Create->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (O_Btn_Two.IsValid()){
		O_Btn_Two->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (O_Btn_Confirm.IsValid()){
		O_Btn_Confirm->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (O_Btn_Changes.IsValid()) {
		O_Btn_Changes->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (O_Warning.IsValid())
	{
		O_Warning->SetVisibility(ESlateVisibility::Collapsed);
	}

	CurrentState = eState;

	switch (eState)
	{
	case ECreateState::Create:
		SettingCreate();
		break;
	case ECreateState::Change:
		SettingChange();
		break;
	case ECreateState::Confirm:
		SettingConfirm();
		break;
	case ECreateState::WrongNick:
		SettingWrong();
		break;
	case ECreateState::UseNick:
		SettingUse();
		break;
	case ECreateState::SameNick:
		SettingSame();
		break;
	default:
		break;
	}
}


void UB2UICreateNickname::SetInputText(const FText& fText)
{
	if (ETB_Input.IsValid())
		ETB_Input->SetText((fText));
}

FText UB2UICreateNickname::GetInputText()
{
	if (ETB_Input.IsValid())
		return ETB_Input->GetText();

	return FText::FromName(TEXT(""));
}


void UB2UICreateNickname::SubscribeEvents()
{
	DeliveryCreateNickNameTicket = DeliveryUserNickNameClass<>::GetInstance().Subscribe(
		[this](){this->ResponCraeteNickName(); });
	DeliveryChangeNickNameTicket = DeliveryChangeNickNameClass<FB2UpdateNicknamePtr>::GetInstance().Subscribe(
		[this](const FB2UpdateNicknamePtr& response) { this->ResponChangeNickName(response); });

	HandleErrorUseNickNameTicket = HandleServerError101Class<>::GetInstance().Subscribe(
		[this](){this->ErrorUseNickName(); });
	HandleErrorWorngNickNameTicket = HandleServerError113Class<>::GetInstance().Subscribe(
		[this](){this->ErrorWrongNickName(); });
}


void UB2UICreateNickname::UnsubscribeEvents()
{
	DeliveryUserNickNameClass<>::GetInstance().Unsubscribe(DeliveryCreateNickNameTicket);
	DeliveryChangeNickNameClass<FB2UpdateNicknamePtr>::GetInstance().Unsubscribe(DeliveryChangeNickNameTicket);

	HandleServerError101Class<>::GetInstance().Unsubscribe(HandleErrorUseNickNameTicket);
	HandleServerError113Class<>::GetInstance().Unsubscribe(HandleErrorWorngNickNameTicket);
}

void UB2UICreateNickname::SettingCreate()
{
	if (O_ContentInput.IsValid()){
		O_ContentInput->SetVisibility(ESlateVisibility::Visible);
	}
	if (O_Btn_Create.IsValid()){
		O_Btn_Create->SetVisibility(ESlateVisibility::Visible);
	}
	if (ETB_Input.IsValid()){
		ETB_Input->SetText(FText::FromString(TEXT("")));
	}
	if (ETB_Input.IsValid()){
		ETB_Input->SetHintText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_InputETB")));
	}
	m_fstrNickName = TEXT("");
}

void UB2UICreateNickname::SettingChange()
{
	FString UserName = BladeIIGameImpl::GetClientDataStore().GetUserNickName();

	if(O_Btn_Changes.IsValid()) {
		O_Btn_Changes->SetVisibility(ESlateVisibility::Visible);
	}

	if (O_ContentInput.IsValid()) {
		O_ContentInput->SetVisibility(ESlateVisibility::Visible);
	}
	if (ETB_Input.IsValid()) {
		ETB_Input->SetText(FText::FromString(TEXT("")));
	}
	if (ETB_Input.IsValid()) {
		ETB_Input->SetHintText(FText::FromString(UserName));
	}

	m_fstrNickName = TEXT("");
}

void UB2UICreateNickname::SettingConfirm()
{
	if (O_Content.IsValid()){
		O_Content->SetVisibility(ESlateVisibility::Visible);
	}
	if (O_Btn_Two.IsValid()){
		O_Btn_Two->SetVisibility(ESlateVisibility::Visible);
	}

	/*if (TB_FirstContent.IsValid()){
	TB_FirstContent->SetText(FText::FromString(TEXT("")));
	}*/

	if (O_Warning.IsValid())
	{
		O_Warning->SetVisibility(ESlateVisibility::Visible);
	}

	if (TB_SecondContent.IsValid()){
		TB_SecondContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_CreateSecondContent")));
	}


	if (bIsChange)
	{
		if (TB_FirstContent.IsValid()) {
			TB_FirstContent->SetText(FText::FromString(m_fstrNickName));
		}

		if (TB_OKBtn.IsValid()) {
			TB_OKBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Change")));
		}

		if (TB_SecondContent.IsValid()) {
			TB_SecondContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("NameChange_ChangeConfirm")));
		}
	}
}

void UB2UICreateNickname::SettingUse()
{
	if (O_Content.IsValid()){
		O_Content->SetVisibility(ESlateVisibility::Visible);
	}
	if (O_Btn_Confirm.IsValid()){
		O_Btn_Confirm->SetVisibility(ESlateVisibility::Visible);
	}
	if (TB_FirstContent.IsValid()){
		TB_FirstContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_SameFirstContent")));
	}
	if (TB_SecondContent.IsValid()){
		TB_SecondContent->SetText(FText::FromString(TEXT("")));
	}
}

void UB2UICreateNickname::SettingWrong()
{
	if (O_Content.IsValid()){
		O_Content->SetVisibility(ESlateVisibility::Visible);
	}
	if (O_Btn_Confirm.IsValid()){
		O_Btn_Confirm->SetVisibility(ESlateVisibility::Visible);
	}

	if (TB_FirstContent.IsValid()){
		TB_FirstContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_WrongFirstContent")));
	}
	if (TB_SecondContent.IsValid()){
		TB_SecondContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CreateNick_WrongSecondContent")));
	}
}

void UB2UICreateNickname::SettingSame()
{
	if (O_Content.IsValid()) {
		O_Content->SetVisibility(ESlateVisibility::Visible);
	}
	if (O_Btn_Confirm.IsValid()) {
		O_Btn_Confirm->SetVisibility(ESlateVisibility::Visible);
	}

	//4. 현재 사용 중인 이름과 동일합니다.

	if (TB_FirstContent.IsValid()) {
		TB_FirstContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("NameChange_Overlap")));
	}

	if (TB_SecondContent.IsValid()) {
		TB_SecondContent->SetText(FText::GetEmpty());
	}
}

bool UB2UICreateNickname::IsCheckWrong(const FText& fTxtNick)
{
	FString TextString = fTxtNick.ToString();
	if (TextString.Len() < B2_CHAR_NICKNAME_MIN_LENGTH || TextString.Len() > B2_CHAR_NICKNAME_MAX_LENGTH)
	{
		return false;
	}

	// 금칙어 추가하기.

	return true;
}

bool UB2UICreateNickname::IsCheckUse(const FText& fTxtNick)
{
	const FString InNickNameStr = fTxtNick.ToString();

	return true;
}

bool UB2UICreateNickname::IsCheckSame()
{
	if (m_fstrNickName.IsEmpty() == true)
		return false;	

	FString UserName = BladeIIGameImpl::GetClientDataStore().GetUserNickName();

	return m_fstrNickName.Equals(UserName);
}

void UB2UICreateNickname::ReQuestCreateNickName(FString fstr)
{
	BladeIIGameImpl::GetClientDataStore().RequestSetNickName(fstr);
}

void UB2UICreateNickname::ResponCraeteNickName()
{
	BladeIIGameImpl::GetClientDataStore().SetUserNickName(m_fstrNickName);
	DoneToGetAccountClass<>::GetInstance().Signal();

	UB2UIManager::GetInstance()->CloseUI(UIFName::CreateNickName);
}

void UB2UICreateNickname::ReQuestChangeNickName(FString fstr)
{
	data_trader::Retailer::GetInstance().RequestChangeNickname(fstr, NicknameChangeItemID);
}

void UB2UICreateNickname::ResponChangeNickName(const FB2UpdateNicknamePtr& response)
{
	BladeIIGameImpl::GetClientDataStore().SetUserNickName(response->new_nickname);
	BladeIIGameImpl::GetClientDataStore().UseNickNameChangeTicket(response);

	UB2UIManager::GetInstance()->CloseUI(UIFName::CreateNickName);

	// 이름 변경권이 성공적
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("NameChange_Success"))),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
	);
}

void UB2UICreateNickname::ErrorUseNickName()
{
	SetStateMode(ECreateState::UseNick);
}

void UB2UICreateNickname::ErrorWrongNickName()
{
	SetStateMode(ECreateState::WrongNick);
}

void UB2UICreateNickname::SetModeChangeNickName(int32 itemID)
{
	bIsChange = true;
	NicknameChangeItemID = itemID;
	SetStateMode(ECreateState::Change);

	UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	if (HeaderUI)
		HeaderUI->HideHeader(true);
}

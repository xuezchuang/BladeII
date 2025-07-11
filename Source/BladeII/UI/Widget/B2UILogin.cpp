#include "B2UILogin.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupInput.h"
#include "Retailer.h"


#include "FB2ErrorMessage.h"


void UB2UILogin::CacheAssets()
{
	GET_SLOT(UButton, BTN_SimpleRun)
	GET_SLOT(UButton, BTN_GameStart)
	GET_SLOT(UEditableTextBox, ETB_ID)
	GET_SLOT(UEditableTextBox, ETB_PW)

	//AccountTestCode
	GET_SLOT(UButton, BTN_CreateAccount)
	GET_SLOT(UEditableTextBox, ETB_NewAccountID)
	GET_SLOT(UEditableTextBox, ETB_NewAccountPW)
}

void UB2UILogin::UpdateStaticText()
{

}

void UB2UILogin::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_SimpleRun, &UB2UILogin::OnClickBtnSimpleRun)
	BIND_CLICK_FUNC_TO_BTN(BTN_GameStart, &UB2UILogin::OnClickBtnGameStart)

	//AccountTestCode
	BIND_CLICK_FUNC_TO_BTN(BTN_CreateAccount, &UB2UILogin::OnClickBtnCreateAccount)
}

//AccountTestCode
void UB2UILogin::PopupCreateAccountMenu()
{
	ShowTestAccountMenu(true);
}
void UB2UILogin::EndCreateAccountMenu()
{
	ShowTestAccountMenu(false);
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UILogin::OnClickBtnSimpleRun()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CharacterIntro);
	//UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple, FText::FromString(TEXT("Ÿ��Ʋ����")), FText::FromString(TEXT("������������~~\n�������ٿ�")));

	/*
	UB2UIMsgPopupInput* InputPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupInput>(EUIMsgPopup::Input, FText::FromString(TEXT("Title")), FText::FromString(TEXT("This is Content~~\nContent~~")), 0.f, true, true, EUIMsgPopupButtonGroup::YesOrNo);
	if (InputPopup)
	{
		InputPopup->AddHandler(EUIMsgPopupButton::Positive, 
			FMsgPopupOnClick::CreateLambda([=]()
			{
				auto TestText = InputPopup->GetInputText();
				UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple, FText::FromString(TEXT("Title")), TestText, 0.f, true, true, EUIMsgPopupButtonGroup::Yes);
			}
		));
	}
	*/

}
void UB2UILogin::OnClickBtnGameStart()
{
	// ���� �ڵ� �������� ���� ����
	if (ETB_ID.IsValid() && ETB_PW.IsValid())
		LobbyLoginAndStartGameClass<const FString&, const FString&>::GetInstance().Signal(ETB_ID->GetText().ToString(), ETB_PW->GetText().ToString());
}

//AccountTestCode
void UB2UILogin::OnClickBtnCreateAccount()
{
	if (ETB_NewAccountID.IsValid() && ETB_NewAccountPW.IsValid())
	{
		FString EditedNewAccountID = ETB_NewAccountID->GetText().ToString();

		if (EditedNewAccountID.Len() > 0)
		{
			data_trader::Retailer::GetInstance().RequestSetNickName(EditedNewAccountID);
			EndCreateAccountMenu(); // �� ���⼭ �ٷ� ����. ���� �ߺ� ID �˻� ���� �� �־�� ��. ������ �ӽ� ���.
		}
		else
		{
#if WITH_EDITOR && !PLATFORM_MAC
			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ID �� �Է��ϼ���.")));
#endif
		}
	}
}
#include "B2UICharacterObserve.h"
#include "B2UIManager.h"
#include "Event.h"
#include "B2UIDocHelper.h"
#include "B2UILobbyCharInfo.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"

extern void AndroidThunkCpp_SetOrientation(bool bSet);
#endif

void UB2UICharacterObserve::CacheAssets()
{
	GET_SLOT(UButton, BTN_Back)
	GET_SLOT(UButton, BTN_BackExtra)
	GET_SLOT(UB2Button, BTN_CharNickEdit)
	GET_SLOT(UTextBlock, TB_CharNickEdit)
	GET_SLOT(UB2UICharacterNickNameEdit, UIP_CharNickEditP)
	if (UIP_CharNickEditP.IsValid())
	{
		UIP_CharNickEditP->Init();
		UIP_CharNickEditP->SetVisibility(ESlateVisibility::Collapsed); // 일단 숨겨둔다.
	}

	GET_SLOT(UB2Button, BTN_ToggleCharInfo)
	GET_SLOT(UTextBlock, TB_ToggleInfo)
	GET_SLOT(UB2UILobbyCharInfo, UIP_CharSimpleInfo)
	if (UIP_CharSimpleInfo.IsValid())
	{
		UIP_CharSimpleInfo->Init();
		UIP_CharSimpleInfo->SetVisibility(ESlateVisibility::Collapsed);
	}

	GET_SLOT(UB2Button, BTN_Change)

	GET_SLOT(UOverlay, Overlay_36)
	GET_SLOT(UOverlay, OV_CharNickEditBtnSet_test)
	GET_SLOT(UOverlay, OV_ToggleCharInfo)
	GET_SLOT(UCanvasPanel, CP_BTN_BackSet)
	
	GET_SLOT(UTextBlock, TBS_Back)

	m_bChangeScreen = false;
}

void UB2UICharacterObserve::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Back, &UB2UICharacterObserve::OnClickBtnBack)
	BIND_CLICK_FUNC_TO_BTN(BTN_BackExtra, &UB2UICharacterObserve::OnClickBtnBack) /* 같은 콜백에 바인드 */
	BIND_CLICK_FUNC_TO_BTN(BTN_CharNickEdit, &UB2UICharacterObserve::OnClickBtnCharNickEdit)

	BIND_CLICK_FUNC_TO_BTN(BTN_ToggleCharInfo, &UB2UICharacterObserve::OnClickBtnToggleCharInfo)

	if (UIP_CharSimpleInfo.IsValid())
	{
		TWeakObjectPtr<UB2Button> BTN_CharInfoClose = UIP_CharSimpleInfo->GetBGButton();
		BIND_CLICK_FUNC_TO_BTN(BTN_CharInfoClose, &UB2UICharacterObserve::OnClickBtnToggleCharInfo)
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_Change, &UB2UICharacterObserve::OnClickBtnChangeScreen)
	UnsubscribeEvent(); // 혹시 몰라서 여기저기서..
	SubscribeEvent();
}

void UB2UICharacterObserve::UpdateStaticText()
{
	if (TB_CharNickEdit.IsValid())
	{
		TB_CharNickEdit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditBtn")));
	}

	if (TB_ToggleInfo.IsValid())
		TB_ToggleInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterSimpleInfo_Open")));

	if (TBS_Back.IsValid())
	{
		TBS_Back->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GoBack")));
	}
}

void UB2UICharacterObserve::OnClose(bool RightNow)
{
	Super::OnClose(RightNow);

	UnsubscribeEvent();
}

void UB2UICharacterObserve::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvent();
}

void UB2UICharacterObserve::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UICharacterObserve);

	LobbyCharObservePopupNickEditClass<EPCClass>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT(EPCClass ec)
			Capture->PopupCharNickEdit(ec);
		END_CAPTURE_OBJECT()
		);
	LobbyCharObserveCloseNickEditClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT()
			Capture->CloseCharNickEdit();
		END_CAPTURE_OBJECT()
		);
}

void UB2UICharacterObserve::UnsubscribeEvent()
{
	LobbyCharObservePopupNickEditClass<EPCClass>::GetInstance().UnsubscribeAll();
	LobbyCharObserveCloseNickEditClass<>::GetInstance().UnsubscribeAll();
}

void UB2UICharacterObserve::PopupCharNickEdit(EPCClass InEditClass)
{
	if (bSetToggleCharInfo)
		OnClickBtnToggleCharInfo();

	if (UIP_CharNickEditP.IsValid())
	{
		UIP_CharNickEditP->BeginEditPopup(InEditClass);
	}
}

void UB2UICharacterObserve::CloseCharNickEdit()
{
	if (UIP_CharNickEditP.IsValid())
	{
		UIP_CharNickEditP->CloseMe();
	}
}

void UB2UICharacterObserve::OpenCharacterSimpleInfoUI()
{
	if (UIP_CharSimpleInfo.IsValid())
	{
		if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		{ 
			UIP_CharSimpleInfo->OpenCharInfo(IntToPCClass(DocBattle->GetCurPCClass()));
		}	

		if (TB_ToggleInfo.IsValid())
			TB_ToggleInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterSimpleInfo_Close")));
	}		
}

void UB2UICharacterObserve::CloseCharacterSimpleInfoUI()
{
	if (UIP_CharSimpleInfo.IsValid())
		UIP_CharSimpleInfo->CloseCharInfo();

	if (TB_ToggleInfo.IsValid())
		TB_ToggleInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterSimpleInfo_Open")));
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UICharacterObserve::OnClickBtnBack()
{
	UB2UIManager::GetInstance()->ChangeUISceneBack();
}

void UB2UICharacterObserve::OnClickBtnCharNickEdit()
{
	EPCClass ObservingClass = EPCClass::EPC_End;

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{ // CharacterObserve UI 를 열기 전에 이게 세팅이 되어 있어야 함.
		ObservingClass = IntToPCClass(DocBattle->GetCurPCClass());
	}

	if (ObservingClass != EPCClass::EPC_End)
	{
		// 이게 처음에는 다른 UI 에서 호출하는 식이어서 굳이 Event signal 을 만들게 되었는데 사실 같은 UI page 에서 호출하는 이상은 필요가 없지..
		LobbyCharObservePopupNickEditClass<EPCClass>::GetInstance().Signal(ObservingClass);
	}
}

void UB2UICharacterObserve::OnClickBtnToggleCharInfo()
{
	if (UIP_CharNickEditP.IsValid() && UIP_CharNickEditP->IsVisible())
		return;

	bSetToggleCharInfo = !bSetToggleCharInfo;
	
	if (bSetToggleCharInfo)
		OpenCharacterSimpleInfoUI();
	else
		CloseCharacterSimpleInfoUI();		
}

void UB2UICharacterObserve::OnClickBtnChangeScreen()
{
	if (m_bChangeScreen)
	{
#if PLATFORM_ANDROID
		AndroidThunkCpp_SetOrientation(false);
#endif
		m_bChangeScreen = false;
		Overlay_36->SetVisibility(ESlateVisibility::Visible);
		CP_BTN_BackSet->SetVisibility(ESlateVisibility::Visible);
		OV_CharNickEditBtnSet_test->SetVisibility(ESlateVisibility::Visible);
		OV_ToggleCharInfo->SetVisibility(ESlateVisibility::Visible);		
	}
	else
	{
#if PLATFORM_ANDROID
		AndroidThunkCpp_SetOrientation(true);
#endif
		m_bChangeScreen = true;
		Overlay_36->SetVisibility(ESlateVisibility::Hidden);
		CP_BTN_BackSet->SetVisibility(ESlateVisibility::Hidden);
		OV_CharNickEditBtnSet_test->SetVisibility(ESlateVisibility::Hidden);
		OV_ToggleCharInfo->SetVisibility(ESlateVisibility::Hidden);
	}

	LobbyCharObserveChangeScreenClass<bool>::GetInstance().Signal(m_bChangeScreen);

	CloseCharacterSimpleInfoUI();
	bSetToggleCharInfo = false;
	
}
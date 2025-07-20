// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UISecession.h"
#include "B2UIManager.h"



#include "Global.h"
#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

void UB2UISecession::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, B2Button_4);
	GET_SLOT(UB2Image, B2Image_3);
	GET_SLOT(UB2Image, B2Image_20);
	GET_SLOT(UImage, TitleBGImage);
	GET_SLOT(UTextBlock, TXT_SettingTitle);
	GET_SLOT(UB2Button, BTN_Exit);
	GET_SLOT(UOverlay, Overlay_0);
	GET_SLOT(UTextBlock, TXT_Body);
	GET_SLOT(UB2Image, B2Image_2);
	GET_SLOT(UTextBlock, TXT_NickName);
	GET_SLOT(UTextBlock, TXT_03);
	GET_SLOT(UB2Button, B2Button_0);
	GET_SLOT(UTextBlock, TXT_OK1);
	GET_SLOT(UOverlay, Overlay_1);
	GET_SLOT(UTextBlock, TXT_Body2);
	GET_SLOT(UB2Button, BTN_Secession);
	GET_SLOT(UTextBlock, TXT_OK2);
}

void UB2UISecession::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(B2Button_4, &UB2UISecession::OnClickB2Button_4);
	BIND_CLICK_FUNC_TO_BTN(BTN_Exit, &UB2UISecession::OnClickBTN_Exit);
	BIND_CLICK_FUNC_TO_BTN(B2Button_0, &UB2UISecession::OnClickB2Button_0);
	BIND_CLICK_FUNC_TO_BTN(BTN_Secession, &UB2UISecession::OnClickBTN_Secession);
}

void UB2UISecession::Init()
{
	Super::Init();
}

void UB2UISecession::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}

void UB2UISecession::OnClickB2Button_4()
{

}

void UB2UISecession::OnClickBTN_Exit()
{
	//DestroySelf(UB2UIManager::GetInstance());
	SetVisibility(ESlateVisibility::Hidden);
}

void UB2UISecession::OnClickB2Button_0()
{

}

void UB2UISecession::OnClickBTN_Secession()
{
	SetVisibility(ESlateVisibility::Hidden);

	// 关俊 TODO
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			FText::FromString(TEXT("[Temporary] Deleting account...")), // 购啊 版绊 巩磊啊 甸绢啊摆瘤..
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([]()
		{
			//
			// TODO 拌沥 昏力
			//

			ClearLocalIniSavedState(); // 咯变 寸楷洒 肺拿 历厘 惑怕甫 朝覆.
#if WITH_EDITOR && !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("俺惯滚傈 皋矫瘤: 肺拿 霸烙 惑怕 历厘 单捞磐绰 朝酒艾澜.")));
#endif
		}
			)
			);
	}
}



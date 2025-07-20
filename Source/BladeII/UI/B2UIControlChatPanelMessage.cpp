// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UIControlChatPanelMessage.h"
#include "B2ControlGameMode.h"


UB2UIControlChatPanelMessage::UB2UIControlChatPanelMessage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIControlChatPanelMessage::Init()
{
	Super::Init();
}

void UB2UIControlChatPanelMessage::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, Btn_Send);
	//GET_SLOT(UB2Image, IMG_Icon);
	GET_SLOT(UTextBlock, TB_Message);

	//GET_SLOT(UB2Image, IMG_BG_Dark);
	//GET_SLOT(UB2Image, IMG_BG_Bright);
}

void UB2UIControlChatPanelMessage::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(Btn_Send, &UB2UIControlChatPanelMessage::OnClickBtnSend);
}

void UB2UIControlChatPanelMessage::OnClickBtnSend()
{
	// 霸烙葛靛烹秦辑 傈价. ControlBattleSub俊绰 霸烙葛靛俊辑 畴萍.
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		pGM->NetSendControlChat(ControlMessageType, ControlMessageIndex);
}

void UB2UIControlChatPanelMessage::AddControlChatPanelMessageItem(EControlChatType Type, int32 Index, FString TextKey, UMaterialInterface* MI)
{
	//ControlMessageType = Type;
	//ControlMessageIndex = Index;

	//if (IMG_Icon.IsValid())
	//{
	//	if (!MI)
	//	{
	//		// 酒捞能 见扁绊
	//		IMG_Icon->SetVisibility(ESlateVisibility::Hidden);

	//		// 归魄 鞘夸窍搁 官层淋(扁裙辑浚 乐绢辑)
	//	}
	//	else
	//		IMG_Icon->SetBrushFromMaterial(MI);
	//}

	//// 硅搬汲沥.
	//if (MI)
	//{
	//	if (IMG_BG_Bright.IsValid())
	//		IMG_BG_Bright->SetVisibility(ESlateVisibility::HitTestInvisible);
	//}
	//else
	//{
	//	if (IMG_BG_Dark.IsValid())
	//		IMG_BG_Dark->SetVisibility(ESlateVisibility::HitTestInvisible);
	//}

	//if (TB_Message.IsValid())
	//{
	//	TB_Message->SetText(BladeIIGetLOCText(B2LOC_CAT_CONTROLCHAT, TextKey));
	//}
}

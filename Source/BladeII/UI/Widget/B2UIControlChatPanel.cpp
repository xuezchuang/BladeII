// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlChatPanel.h"
#include "B2UIControlChatPanelMessage.h"




UB2UIControlChatPanel::UB2UIControlChatPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GridPanelColumnCount = 4;
}

void UB2UIControlChatPanel::Init()
{
	Super::Init();

	// 菩澄 郴侩拱 厚框
	ResetPanelContents();
}

void UB2UIControlChatPanel::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UOverlay, O_BTN_Msg_DeAct);
	GET_SLOT(UOverlay, O_BTN_Msg_Act);
	GET_SLOT(UOverlay, O_Contents_Msg);

	GET_SLOT(UOverlay, O_BTN_Emotion_DeAct);
	GET_SLOT(UOverlay, O_BTN_Emotion_Act);
	GET_SLOT(UOverlay, O_Contents_Emotion);

	GET_SLOT(UVerticalBox, VB_ChatText);
	GET_SLOT(UUniformGridPanel, GP_ChatEmoticon);

	GET_SLOT(UB2Button, BTN_Msg_Tab);
	GET_SLOT(UB2Button, BTN_Emotion_Tab);	
}

void UB2UIControlChatPanel::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Msg_Tab, &UB2UIControlChatPanel::OnClickBtnMsgTab);
	BIND_CLICK_FUNC_TO_BTN(BTN_Emotion_Tab, &UB2UIControlChatPanel::OnClickBtnEmotionTab);
}

void UB2UIControlChatPanel::OnClickBtnMsgTab()
{
	SetChatTab(EControlChatType::Message);
}

void UB2UIControlChatPanel::OnClickBtnEmotionTab()
{
	SetChatTab(EControlChatType::Emoticon);
}

void UB2UIControlChatPanel::ResetPanelContents()
{
	// 阿 菩澄救狼 郴侩拱阑 瘤矿聪促.

	if (VB_ChatText.IsValid())
	{
		TArray<UPanelSlot*> Slots = VB_ChatText->GetSlots();

		for (UPanelSlot* pSlot : Slots)
		{
			if (pSlot)
				VB_ChatText->RemoveChild(pSlot->Content);
		}
	}

	if (GP_ChatEmoticon.IsValid())
	{
		TArray<UPanelSlot*> Slots = GP_ChatEmoticon->GetSlots();

		for (UPanelSlot* pSlot : Slots)
		{
			if (pSlot)
				GP_ChatEmoticon->RemoveChild(pSlot->Content);
		}
	}
}

void UB2UIControlChatPanel::SetChatTab(EControlChatType TabType)
{
	CurrentTab = TabType;

	if (!O_BTN_Msg_DeAct.IsValid()
		|| !O_BTN_Msg_Act.IsValid()
		|| !O_Contents_Msg.IsValid()
		|| !O_BTN_Emotion_DeAct.IsValid()
		|| !O_BTN_Emotion_Act.IsValid()
		|| !O_Contents_Emotion.IsValid())
		return;

	// 徘俊 蝶弗 厚瘤喉.
	O_BTN_Msg_DeAct->SetVisibility(ESlateVisibility::Hidden);
	O_BTN_Msg_Act->SetVisibility(ESlateVisibility::Hidden);
	O_Contents_Msg->SetVisibility(ESlateVisibility::Hidden);

	O_BTN_Emotion_DeAct->SetVisibility(ESlateVisibility::Hidden);
	O_BTN_Emotion_Act->SetVisibility(ESlateVisibility::Hidden);
	O_Contents_Emotion->SetVisibility(ESlateVisibility::Hidden);

	switch (CurrentTab)
	{
	case EControlChatType::Message:
		O_BTN_Msg_Act->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		O_BTN_Emotion_DeAct->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		O_Contents_Msg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;
	case EControlChatType::Emoticon:
		O_BTN_Msg_DeAct->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		O_BTN_Emotion_Act->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		O_Contents_Emotion->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;
	}

}

void UB2UIControlChatPanel::AddControlChatPanelMessageItem(EControlChatType Type, int32 Index, FString TextKey, UMaterialInterface* MI)
{
	UB2UIControlChatPanelMessage* pUI;

	switch (Type)
	{
	case EControlChatType::Message:
		pUI = CreateWidget<UB2UIControlChatPanelMessage>(GetWorld(), MessageItemClass);
		break;
	case EControlChatType::Emoticon:
		pUI = CreateWidget<UB2UIControlChatPanelMessage>(GetWorld(), EmoticonItemClass);
		break;
	default:
		return;
		break;
	}

	if (!pUI)
		return;

	pUI->Init();
	pUI->AddControlChatPanelMessageItem(Type, Index, TextKey, MI);

	switch (Type)
	{
	case EControlChatType::Message:
		if (VB_ChatText.IsValid())
		{
			VB_ChatText->AddChild(pUI);
		}
		break;
	case EControlChatType::Emoticon:
		if (GP_ChatEmoticon.IsValid())
		{
			GP_ChatEmoticon->AddChild(pUI);
						
			UUniformGridSlot* GridPanelSlot = Cast<UUniformGridSlot>(pUI->Slot);
			if (GridPanelSlot)
			{
				// 皋技瘤 牢郸胶绰 1何磐. 弊府靛菩澄篮 0何磐矫累
				GridPanelSlot->SetColumn((Index - 1) % GridPanelColumnCount);
				GridPanelSlot->SetRow((Index - 1) / GridPanelColumnCount);
			}			
		}
		break;
	default:
		return;
		break;
	}


}

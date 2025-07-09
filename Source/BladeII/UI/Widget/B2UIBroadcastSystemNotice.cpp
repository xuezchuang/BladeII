// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIBroadcastSystemNotice.h"

UB2UIBroadcastSystemNotice::UB2UIBroadcastSystemNotice(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIBroadcastSystemNotice::Init()
{
	Super::Init();
	IsPlayBroadcast = false;
}

void UB2UIBroadcastSystemNotice::CacheAssets()
{
	GET_SLOT(UB2RichTextBlock, TB_Message);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UPanelWidget, OV_Content);
}

void UB2UIBroadcastSystemNotice::BindDelegates()
{

}

void UB2UIBroadcastSystemNotice::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

  	bool CanScrolling = IsEableScrolling && OV_Content.IsValid();
  	if (CanScrolling)
  	{
  		FVector2D Position = OV_Content->GetRenderTransform().Translation;
  		Position.X -= InDeltaTime * ScrollSpeed;
  		OV_Content->SetRenderTranslation(Position);

 		float Endline = -(WidgetWidth + VisibleWidgetWidth);
 		if (Endline > Position.X)
 		{
 			IsEableScrolling = false;
 			OnFinishedBroadcast();
 		}
  	}
}

void UB2UIBroadcastSystemNotice::DestroySelf(UB2UIManager* InUIManager)
{
	IsEableScrolling = false;
	Super::DestroySelf(InUIManager);
//	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToBroadcast);
}

void UB2UIBroadcastSystemNotice::OnStartBroadcastWidgetInfo(float InVisibleWidgetWidth, float InMessageWidth, int32 InTextLength)
{
	VisibleWidgetWidth = InVisibleWidgetWidth;
	WidgetWidth = InMessageWidth;
	MessageLength = InTextLength;
	IsEableScrolling = true;

	if (OV_Content.IsValid())
	{
		FVector2D Position = OV_Content->GetRenderTransform().Translation;
		Position.X = 0;
		OV_Content->SetRenderTranslation(Position);
	}
}

void UB2UIBroadcastSystemNotice::SetMessageInfo(const FString &InMessage, const int32 InDisplayTime)
{
	if (IsEableScrolling == true || IsPlayBroadcast == true)
	{
		FB2BroadcastMessage NewMessage;
		NewMessage.DisplayTime = InDisplayTime;
		NewMessage.Message = InMessage;
		ChatStore.Add(NewMessage);
		return;
	}
	if (TB_Message.IsValid())
	{
		TB_Message->SetText(FText::FromString(InMessage));	
	}

	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	}
	DisplayTime = static_cast<float>(InDisplayTime);
}

void UB2UIBroadcastSystemNotice::StartBroadcast()
{
	if (IsPlayBroadcast == true)
		return;
	if (IsEableScrolling == true)
		return;

	IsPlayBroadcast = true;

	SetVisibility(ESlateVisibility::HitTestInvisible);
	StartBroadcast_BP();

	//UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeToBroadcast, this, &UB2UIBroadcastSystemNotice::OnFinishedBroadcast, DisplayTime, false);
}

void UB2UIBroadcastSystemNotice::OnFinishedBroadcast()
{
	IsEableScrolling = false;
	IsPlayBroadcast = false;
	if (ChatStore.Num() > 0)
	{
		SetMessageInfo(ChatStore[0].Message, ChatStore[0].DisplayTime);
		StartBroadcast_BP();
		ChatStore.RemoveAt(0);
		return;
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
	UB2UIManager::GetInstance()->CloseUI(UIFName::BroadcastSystemNotice);
}



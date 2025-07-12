// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIBattleNotice.h"
#include "BladeIILocText.h"

UB2UIBattleNotice::UB2UIBattleNotice(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}


void UB2UIBattleNotice::CacheAssets()
{
	GET_SLOT(UB2ScrollBox, SB_NoticeSlot);

	NoticeSlots.SetNum(MaxNoticeCount);
	GET_SLOT_BYNAME(UB2UIBattleNoticeSlot, NoticeSlots[0], NoticeSlot1);
	GET_SLOT_BYNAME(UB2UIBattleNoticeSlot, NoticeSlots[1], NoticeSlot2);
	GET_SLOT_BYNAME(UB2UIBattleNoticeSlot, NoticeSlots[2], NoticeSlot3);
	GET_SLOT_BYNAME(UB2UIBattleNoticeSlot, NoticeSlots[3], NoticeSlot4);

	for (TWeakObjectPtr<UB2UIBattleNoticeSlot> NoticeSlot : NoticeSlots)
	{
		if (NoticeSlot.IsValid())
		{
			NoticeSlot->Init();
			NoticeSlot->Empty();
		}
	}
}

void UB2UIBattleNotice::Subscribes()
{
	//CAPTURE_UOBJECT(UB2UIBattleNotice);

	//OnEtherActivatedTicket = OnEtherActivatedClass<int32, ABladeIIPlayer*, ABladeIICharacter*>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(OnEtherActivated, int32 EtherSetID, ABladeIIPlayer* Owner, ABladeIICharacter* Causer)
	//	Capture->OnEtherActivated(EtherSetID, Owner, Causer);
	//	END_CAPTURE_OBJECT()
	//);

}

void UB2UIBattleNotice::Unsubscribes()
{
	//OnEtherActivatedClass<int32, ABladeIIPlayer*, ABladeIICharacter*>::GetInstance().Unsubscribe(OnEtherActivatedTicket);
}

void UB2UIBattleNotice::Init()
{
	Super::Init();

	Subscribes();
}

void UB2UIBattleNotice::DestroySelf(UB2UIManager* InUIManager)
{
	Unsubscribes();

	for (auto NoticeSlot : NoticeSlots)
	{
		if (NoticeSlot.IsValid())
			NoticeSlot->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIBattleNotice::AddSingleEmptySlot()
{
	if (UB2UIBattleNoticeSlot* LastestSlot = GetSwapSlotToAdd())
	{
		LastestSlot->Empty();
	}
}

void UB2UIBattleNotice::UpdateNoticeText(const FText& Text)
{
	if (UB2UIBattleNoticeSlot* LastestSlot = GetSwapSlotToAdd())
	{
		LastestSlot->SetText(Text);
		LastestSlot->Show();
		OnUpdatedNotice();
	}
}

void UB2UIBattleNotice::OnUpdatedNotice()
{
	if (APlayerController* OwingPlyaer = GetOwningPlayer())
	{
		FTimerManager& TimerManager = OwingPlyaer->GetWorldTimerManager();

		if (TimerManager.IsTimerActive(NoticeHideHandler))
			TimerManager.ClearTimer(NoticeHideHandler);

		TimerManager.SetTimer(NoticeHideHandler, this, &UB2UIBattleNotice::ActivatedTimer_NoticeHide, NoticeSeconds, false);
	}
}

void UB2UIBattleNotice::ActivatedTimer_NoticeHide()
{
	for (TWeakObjectPtr<UB2UIBattleNoticeSlot> NoticeSlot : NoticeSlots)
	{
		if (NoticeSlot.IsValid() && NoticeSlot->IsValidSlot())
			NoticeSlot->OnNoticeHide();
	}
}

UB2UIBattleNoticeSlot* UB2UIBattleNotice::GetSwapSlotToAdd()
{
	if (SB_NoticeSlot.IsValid())
	{
		UB2UIBattleNoticeSlot* TopSlot = Cast<UB2UIBattleNoticeSlot>(SB_NoticeSlot->GetChildAt(0));
		SB_NoticeSlot->RemoveChildAt(0);

		SB_NoticeSlot->AddChild(TopSlot);

		return TopSlot;
	}

	return nullptr;
}

int32 UB2UIBattleNotice::GetValidSlotCount()
{
	int32 ValidCount = 0;
	for (TWeakObjectPtr<UB2UIBattleNoticeSlot> NoticeSlot : NoticeSlots)
	{
		if (NoticeSlot.IsValid() && NoticeSlot->IsValidSlot())
			ValidCount++;
	}

	return ValidCount;
}

void UB2UIBattleNotice::OnEtherActivated(int32 EtherSetID, ABladeIIPlayer* Owner, ABladeIICharacter* Causer)
{
//	if (Owner && Owner->IsLocalPlayer())
//	{
//		FText EtherNoticeFormat = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Ether_ActivateNotice"));
//		FText EtherName = BladeIIGetLOCText(B2LOC_CAT_ETHER, FString::FromInt(EtherSetID));
//
//		UpdateNoticeText(FText::Format(EtherNoticeFormat, EtherName));
//	}
}





UB2UIBattleNoticeSlot::UB2UIBattleNoticeSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UB2UIBattleNoticeSlot::CacheAssets()
{
	//GET_SLOT(UB2RichTextBlock, RTB_BattleText);
}

void UB2UIBattleNoticeSlot::AnimateSlot(ESlotAnimation Animation)
{

}

void UB2UIBattleNoticeSlot::Show()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OnNoticeShow();

	bValidSlot = true;
}

void UB2UIBattleNoticeSlot::Empty()
{
	SetVisibility(ESlateVisibility::Hidden);
	bValidSlot = false;
}

void UB2UIBattleNoticeSlot::SetText(const FText& SlotText)
{
	//if (RTB_BattleText.IsValid())
	//{
	//	RTB_BattleText->SetText(SlotText);
	//}
}

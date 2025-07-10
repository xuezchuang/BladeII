// Fill out your copyright notice in the Description page of Project Settings.


#include "B2ScrollBox.h"


UB2ScrollBox::UB2ScrollBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ScrollAdjustType = EScrollAdjustType::ESAT_NONE;

	bUseScrollDeacceleration = true;
}

void UB2ScrollBox::CreateMyScrollBox()
{
	MyScrollBox = SNew(SScrollBox);
		//.Style(&WidgetStyle)
		//.ScrollBarStyle(&WidgetBarStyle)
		//.Orientation(Orientation)
		//.ConsumeMouseWheel(ConsumeMouseWheel)
		//.AllowOverscroll(AllowOverscroll ? EAllowOverscroll::Yes : EAllowOverscroll::No)
		//.OnUserScrolled(FOnUserScrolled::CreateUObject(this, &UB2ScrollBox::OnUserScrolled))
		//.OnEndUserScrolling(BIND_UOBJECT_DELEGATE(FSimpleDelegate, OnUserScrolledEnd))
		//.NatualMove(bNatualmove);
		
}

void UB2ScrollBox::EndUserScrolling()
{
	//if (!bUseScrollDeacceleration && MyScrollBox.IsValid())
	//	MyScrollBox->ClearScrollVelocity();

	//CheckScrollAdjustType();
}

// ScrollAdjustType과 스트롤Orientation에 따라서
// 스크롤 아이템을 설정한 기준점에 맞춰줌
void UB2ScrollBox::CheckScrollAdjustType()
{
	//if (ScrollAdjustType == EScrollAdjustType::ESAT_NONE || IsRightClickScrolling() || !MyScrollBox.IsValid())
	//	return;

	//UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);

	//if (!CanvasPanelSlot)
	//	return;

	//// 이 기능 사용할때는 무조건 관성스크롤 작동 안시킴
	//// 추후 필요하면 일정 이하속도일때만 발동하게끔 수정한다.
	//MyScrollBox->ClearScrollVelocity();

	//// Check Start
	//TArray<UPanelSlot*> ScrollSlots = GetSlots();

	//float fTotalSlotsSize = 0.f;
	//float ScrollBoxSizeToCheck = 0.f;

	//switch (Orientation)
	//{
	//case EOrientation::Orient_Horizontal:
	//	ScrollBoxSizeToCheck = Cast<UCanvasPanelSlot>(Slot)->GetSize().X;
	//	break;
	//case EOrientation::Orient_Vertical:
	//	ScrollBoxSizeToCheck = Cast<UCanvasPanelSlot>(Slot)->GetSize().Y;
	//	break;
	//default:
	//	break;
	//}

	//switch (ScrollAdjustType)
	//{
	//case EScrollAdjustType::ESAT_MID:
	//	ScrollBoxSizeToCheck /= 2; 
	//	break;
	//case EScrollAdjustType::ESAT_START:
	//	ScrollBoxSizeToCheck = 0.0f;
	//	break;
	//case EScrollAdjustType::ESAT_END:
	//	break;
	//default:
	//	break;
	//}

	//for (int nCnt = 0; nCnt < ScrollSlots.Num(); ++nCnt)
	//{
	//	LastAdjustItemIndex = nCnt;

	//	float SlotSize = 0.f;

	//	switch (Orientation)
	//	{
	//	case EOrientation::Orient_Horizontal:
	//		SlotSize = ScrollSlots[nCnt]->Content->GetDesiredSize().X + Cast<UScrollBoxSlot>(ScrollSlots[nCnt])->Padding.Left + Cast<UScrollBoxSlot>(ScrollSlots[nCnt])->Padding.Right;
	//		break;
	//	case EOrientation::Orient_Vertical:
	//		SlotSize = ScrollSlots[nCnt]->Content->GetDesiredSize().Y + Cast<UScrollBoxSlot>(ScrollSlots[nCnt])->Padding.Top + Cast<UScrollBoxSlot>(ScrollSlots[nCnt])->Padding.Bottom;
	//		break;
	//	default:
	//		break;
	//	}

	//	fTotalSlotsSize += SlotSize;

	//	float fAdjustAmountForCurSlot = 0.f;
	//	float fTestVar = 0.f;

	//	switch (ScrollAdjustType)
	//	{
	//	case EScrollAdjustType::ESAT_MID:
	//		fAdjustAmountForCurSlot = SlotSize / 2;
	//		break;
	//	case EScrollAdjustType::ESAT_START:
	//		fAdjustAmountForCurSlot = SlotSize;
	//		fTestVar = SlotSize / 2;
	//		break;
	//	case EScrollAdjustType::ESAT_END:
	//		fAdjustAmountForCurSlot = SlotSize;
	//		fTestVar = SlotSize / 2;
	//		break;
	//	default:
	//		break;
	//	}

	//	if (fTotalSlotsSize - GetScrollOffset() > ScrollBoxSizeToCheck && fTotalSlotsSize - GetScrollOffset() > ScrollBoxSizeToCheck + fTestVar)
	//	{
	//		bNatualmove ? MyScrollBox->SetNatualScrollOffSet(fTotalSlotsSize - ScrollBoxSizeToCheck - fAdjustAmountForCurSlot)
	//			:MyScrollBox->SetScrollOffset(fTotalSlotsSize - ScrollBoxSizeToCheck - fAdjustAmountForCurSlot);
	//		return;
	//	}

	//	if (nCnt == ScrollSlots.Num() - 1)
	//	{
	//		bNatualmove ? MyScrollBox->SetNatualScrollOffSet(fTotalSlotsSize - ScrollBoxSizeToCheck)
	//		: MyScrollBox->SetScrollOffset(fTotalSlotsSize - ScrollBoxSizeToCheck);
	//		return;
	//	}
	//}
}

void UB2ScrollBox::SetAdjustFromSlotIndex(int32 SlotIndex)
{
	////if (ScrollAdjustType == EScrollAdjustType::ESAT_NONE || IsRightClickScrolling() || !MyScrollBox.IsValid())
	////	return;
	//
	//UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);

	//if (!CanvasPanelSlot)
	//	return;

	//// Check Start
	//TArray<UPanelSlot*> ScrollSlots = GetSlots();

	//if (SlotIndex >= ScrollSlots.Num())
	//	return;

	//float fTotalSlotsSize = 0.f;
	//float ScrollBoxSizeToCheck = 0.f;

	//switch (Orientation)
	//{
	//case EOrientation::Orient_Horizontal:
	//	ScrollBoxSizeToCheck = Cast<UCanvasPanelSlot>(Slot)->GetSize().X;
	//	break;
	//case EOrientation::Orient_Vertical:
	//	ScrollBoxSizeToCheck = Cast<UCanvasPanelSlot>(Slot)->GetSize().Y;
	//	break;
	//default:
	//	break;
	//}

	//switch (ScrollAdjustType)
	//{
	//case EScrollAdjustType::ESAT_MID:
	//	ScrollBoxSizeToCheck /= 2;
	//	break;
	//case EScrollAdjustType::ESAT_START:
	//	ScrollBoxSizeToCheck = 0.0f;
	//	break;
	//case EScrollAdjustType::ESAT_END:
	//	break;
	//default:
	//	break;
	//}

	//for (int nCnt = 0; nCnt < SlotIndex + 1; ++nCnt)
	//{
	//	float SlotSize = 0.f;

	//	switch (Orientation)
	//	{
	//	case EOrientation::Orient_Horizontal:
	//		ScrollSlots[nCnt]->Content->ForceLayoutPrepass();
	//		SlotSize = ScrollSlots[nCnt]->Content->GetDesiredSize().X + Cast<UScrollBoxSlot>(ScrollSlots[nCnt])->Padding.Left + Cast<UScrollBoxSlot>(ScrollSlots[nCnt])->Padding.Right;
	//		break;
	//	case EOrientation::Orient_Vertical:
	//		ScrollSlots[nCnt]->Content->ForceLayoutPrepass();
	//		SlotSize = ScrollSlots[nCnt]->Content->GetDesiredSize().Y + Cast<UScrollBoxSlot>(ScrollSlots[nCnt])->Padding.Top + Cast<UScrollBoxSlot>(ScrollSlots[nCnt])->Padding.Bottom;
	//		break;
	//	default:
	//		break;
	//	}

	//	fTotalSlotsSize += SlotSize;

	//	float fAdjustAmountForCurSlot = 0.f;
	//	float fTestVar = 0.f;

	//	switch (ScrollAdjustType)
	//	{
	//	case EScrollAdjustType::ESAT_MID:
	//		fAdjustAmountForCurSlot = SlotSize / 2;
	//		break;
	//	case EScrollAdjustType::ESAT_START:
	//		fAdjustAmountForCurSlot = SlotSize;
	//		fTestVar = SlotSize / 2;
	//		break;
	//	case EScrollAdjustType::ESAT_END:
	//		fAdjustAmountForCurSlot = SlotSize;
	//		fTestVar = SlotSize / 2;
	//		break;
	//	default:
	//		break;
	//	}

	//	if (nCnt ==  SlotIndex)
	//	{
	//		MyScrollBox->SetScrollOffset(fTotalSlotsSize - ScrollBoxSizeToCheck - fAdjustAmountForCurSlot);
	//	}

	//	if (nCnt == ScrollSlots.Num() - 1)
	//	{
	//		MyScrollBox->SetScrollOffset(fTotalSlotsSize - ScrollBoxSizeToCheck);
	//	}
	//}

	//CheckScrollAdjustType();
}

void UB2ScrollBox::SetAnimateScroll(bool bNewAnimateScroll)
{
	//if (MyScrollBox.IsValid())
		//MyScrollBox->SetAnimateScroll(bNewAnimateScroll);	
}

float UB2ScrollBox::GetScrollVelocity()
{
	//if (MyScrollBox.IsValid())
	//	return MyScrollBox->GetScrollVelocity();

	return 0;
}

bool UB2ScrollBox::IsScrolling()
{
	//if (MyScrollBox.IsValid())
	//	return MyScrollBox->IsScrolling();

	return 0;
}

float UB2ScrollBox::GetScrollPhysicalOffset()
{
	//if (MyScrollBox.IsValid())
	//	return MyScrollBox->GetScrollPhysicalOffset();

	return 0;
}

FReply UB2ScrollBox::OnSBMouseButtonDownFromSender(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//if (MyScrollBox.IsValid())
	//	return MyScrollBox->OnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Unhandled();
}

FReply UB2ScrollBox::OnSBMouseButtonUpFromSender(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//if (MyScrollBox.IsValid())
	//	return MyScrollBox->OnMouseButtonUp(InGeometry, InMouseEvent);

	return FReply::Unhandled();
}

FReply UB2ScrollBox::OnSBMouseMoveFromSender(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//if (MyScrollBox.IsValid())
	//	return MyScrollBox->OnMouseMove(InGeometry, InMouseEvent);

	return FReply::Unhandled();
}

void UB2ScrollBox::OnSBMouseLeaveFromSender(const FPointerEvent& InMouseEvent)
{
	//if (MyScrollBox.IsValid())
	//	MyScrollBox->OnMouseLeave(InMouseEvent);
}

FReply UB2ScrollBox::OnSBTouchStartedFromSender(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	//if (MyScrollBox.IsValid())
	//	return MyScrollBox->OnTouchStarted(InGeometry, InGestureEvent);

	return FReply::Unhandled();
}

FReply UB2ScrollBox::OnSBTouchMovedFromSender(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	//if (MyScrollBox.IsValid())
	//	return MyScrollBox->OnTouchMoved(InGeometry, InGestureEvent);

	return FReply::Unhandled();
}

FReply UB2ScrollBox::OnSBTouchEndedFromSender(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	//if (MyScrollBox.IsValid())
		//return MyScrollBox->OnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Unhandled();
}

void UB2ScrollBox::OnUserScrolled(float ScrolledDelta)
{
	OnScrolled.ExecuteIfBound(ScrolledDelta);
}

void UB2ScrollBox::SetOnUserScrolled(const FOnUserScrolled& InHandler)
{
	OnScrolled = InHandler;
}

void UB2ScrollBox::OnUserScrolledEnd()
{
	EndUserScrolling();
	OnScrolledEnd.ExecuteIfBound();
}

void UB2ScrollBox::SetOnUserScrolledEnd(const FSimpleDelegate& InHandler)
{
	OnScrolledEnd = InHandler;
}

float UB2ScrollBox::GetScrollOffsetMax()
{
	//if(MyScrollBox.IsValid())
	//	return MyScrollBox->GetOrientation() == EOrientation::Orient_Vertical ? MyScrollBox->GetDesiredSize().Y : MyScrollBox->GetDesiredSize().X;

	return 0;
}

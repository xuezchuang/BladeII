// Fill out your copyright notice in the Description page of Project Settings.

#include "B2Slider.h"
#include "B2UIManager.h"


TSharedRef<SWidget> UB2Slider::RebuildWidget()
{
	MySlider = SNew(SSlider)
		.Style(&GetWidgetStyle())
		.IsFocusable(IsFocusable)
		.OnMouseCaptureBegin(BIND_UOBJECT_DELEGATE(FSimpleDelegate, PreHandleOnMouseCaptureBegin))
		.OnMouseCaptureEnd(BIND_UOBJECT_DELEGATE(FSimpleDelegate, PreHandleOnMouseCaptureEnd))
		.OnControllerCaptureBegin(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnControllerCaptureBegin))
		.OnControllerCaptureEnd(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnControllerCaptureEnd))
		.OnValueChanged(BIND_UOBJECT_DELEGATE(FOnFloatValueChanged, HandleOnValueChanged));

	return MySlider.ToSharedRef();
}

void UB2Slider::PreHandleOnMouseCaptureBegin()
{
	OneTimeBlockClickEvent = BlockClickEventInCaptured;
	if (OneTimeBlockClickEvent)
		UB2UIManager::BlockUIClickEvent(true);

	HandleOnMouseCaptureBegin();
}
void UB2Slider::PreHandleOnMouseCaptureEnd()
{
	if (OneTimeBlockClickEvent)
	{
		UB2UIManager::BlockUIClickEvent(false);
		OneTimeBlockClickEvent = false;
	}

	HandleOnMouseCaptureEnd();
}

void UB2Slider::BeginDestroy()
{
	if (OneTimeBlockClickEvent)
		UB2UIManager::BlockUIClickEvent(false);

	Super::BeginDestroy();
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIBackWidget.h"


UB2UIBackWidget::UB2UIBackWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void IB2UIBackWidget::CloseWidget()
{
	if (IsClose)
	{
		CloseWidgetDelegate();
	}
}

void IB2UIBackWidget::SetClose(bool Value)
{
	IsClose = Value;
}

void IB2UIBackWidget::OpenBattleMainPause()
{
	CloseWidgetDelegate();
}

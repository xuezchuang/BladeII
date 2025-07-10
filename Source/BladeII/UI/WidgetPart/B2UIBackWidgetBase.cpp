// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIBackWidgetBase.h"
//#include "BladeII.h"

#include "B2AndroidBackManager.h"

void UB2UIBackWidgetBase::OpenPopup()
{
	if (this->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
}

void UB2UIBackWidgetBase::ClosePopup()
{
	if (this->GetVisibility() != ESlateVisibility::Collapsed)
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIBackWidgetBase::CloseWidgetDelegate()
{
	ClosePopup();
}
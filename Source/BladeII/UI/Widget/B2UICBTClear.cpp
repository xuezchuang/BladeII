// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UICBTClear.h"


void UB2UICBTClear::Init()
{
	Super::Init();
}
void UB2UICBTClear::CacheAssets()
{
	GET_SLOT(UB2Button, B2Button_0);
}
void UB2UICBTClear::UpdateStaticText()
{

}
void UB2UICBTClear::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(B2Button_0, &UB2UICBTClear::OnClickClose)
}

void UB2UICBTClear::SetLevelUpState(bool InbMain, bool InbSub)
{
	MainLevelUp = InbMain;
	SubLevelUp = InbSub;
}

void UB2UICBTClear::OnClickClose()
{
	DelegateCBTFinish.ExecuteIfBound(MainLevelUp, SubLevelUp);

	OnClose(); // 일반 UIWidget 이라면 UIManager 에 의해서 불리는 부분

	DelegateCBTFinish.Unbind();
}
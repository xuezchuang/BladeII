// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICheckPermission.h"
#include "B2UIManager.h"

#include "B2UIPatch.h"
#include "B2UIActionLogo.h"
#include "B2DLCFrontGameMode.h"


UB2UICheckPermission::UB2UICheckPermission(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UICheckPermission::CacheAssets()
{
	
	GET_SLOT(UB2Button, B2Button_0)
}

void UB2UICheckPermission::BindDelegates()
{
	Super::BindDelegates();

	//BIND_CLICK_FUNC_TO_BTN(B2Button_0, &UB2UICheckPermission::GotoPermission)
	
}

void UB2UICheckPermission::Init()
{
	Super::Init();
}

void  UB2UICheckPermission::SetOwnerUI(UB2UIActionLogo* owner)
{
	owner_ = owner;
}

void UB2UICheckPermission::GotoPermissionSelect()
{
	owner_->OpenTitleMain();
}





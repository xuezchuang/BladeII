// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2DynItemIcon_EnhanceIngred.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "Event.h"

UB2DynItemIcon_EnhanceIngred::UB2DynItemIcon_EnhanceIngred(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2DynItemIcon_EnhanceIngred::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Area)
	GET_SLOT(UB2DynItemIcon_LobbyInven, DynItemIcon)

	if (BTN_Area.IsValid())
	{
		BTN_Area->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	this->SetVisibility(ESlateVisibility::Visible);
}

void UB2DynItemIcon_EnhanceIngred::UpdateItemData(const FB2Item& InItem)
{
	NativeItemData = InItem;

	if (DynItemIcon.IsValid())
	{
		DynItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget);
		DynItemIcon->UpdateItemData(InItem);
	}
}

void UB2DynItemIcon_EnhanceIngred::OnClickedBTNArea()
{
	UnSelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
}

void UB2DynItemIcon_EnhanceIngred::OnClickedSenderBTNArea()
{ // ManualScrollBox 기능의 일부로 불리는 인터페이스
	Super::OnClickedSenderBTNArea();

	OnClickedBTNArea(); // 실제 클릭 핸들링
}
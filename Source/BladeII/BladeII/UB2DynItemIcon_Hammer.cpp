//// Fill out your copyright notice in the Description page of Project Settings.
//
#include "UB2DynItemIcon_Hammer.h"
//#include "B2DynItemIcon_LobbyInven.h"
//
//#include "Event.h"
//
//UB2DynItemIcon_Hammer::UB2DynItemIcon_Hammer(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//
//}
//
//void UB2DynItemIcon_Hammer::CacheAssets()
//{
//	Super::CacheAssets();
//
//	GET_SLOT(UB2Button, BTN_Area)
//		GET_SLOT(UB2DynItemIcon_LobbyInven, DynItemIcon)
//
//		if (BTN_Area.IsValid())
//		{
//			BTN_Area->SetVisibility(ESlateVisibility::HitTestInvisible);
//		}
//	this->SetVisibility(ESlateVisibility::Visible);
//}
//
//void UB2DynItemIcon_Hammer::UpdateItemData(const FB2Item& InItem)
//{
//	NativeItemData = InItem;
//
//	if (DynItemIcon.IsValid())
//	{
//		DynItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget);
//		DynItemIcon->UpdateItemData(InItem);
//	}
//}
//
//void UB2DynItemIcon_Hammer::OnClickedBTNArea()
//{
//	UnSelectForItemHammerClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
//}
//
//void UB2DynItemIcon_Hammer::OnClickedSenderBTNArea()
//{ // ManualScrollBox 기능의 일부로 불리는 인터페이스
//	Super::OnClickedSenderBTNArea();
//
//	OnClickedBTNArea(); // 실제 클릭 핸들링
//}
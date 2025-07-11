//// Fill out your copyright notice in the Description page of Project Settings.
//
//
#include "B2UIMailRewardPopUp.h"
//#include "B2UIManager.h"
//#include "B2RedDotManager.h"
//
//
//void UB2UIMailRewardPopUp::CacheAssets()
//{
//	Super::CacheAssets();
//
//	GET_SLOT(UButton, BTN_Confirm);
//	GET_SLOT(UTextBlock, TB_Reward);
//
//	Anim_MailPopUp = GetAnimation(this, "Anim_MailPopUp");
//}
//
//void UB2UIMailRewardPopUp::UpdateStaticText()
//{
//	if(TB_Reward.IsValid())
//		TB_Reward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetMailReward")));
//}
//
//void UB2UIMailRewardPopUp::BindDelegates()
//{
//	//BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIMailRewardPopUp::OnClickConfirm);
//}
//
//void UB2UIMailRewardPopUp::OnClickConfirm()
//{
//	if (ConfirmDelegate.ExecuteIfBound())
//		ConfirmDelegate.Unbind();
//
//	if (auto* UIManager = UB2UIManager::GetInstance())
//	{
//		UIManager->CloseUI(UIFName::RewardMailPopup);
//	}
//}
//
//void UB2UIMailRewardPopUp::OnOpen(bool RightNow)
//{
//	Super::OnOpen(RightNow);
//
//	//보상 팝업 열릴때 매일 쪽 갱신
//	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_MAIL });
//}
//
//void UB2UIMailRewardPopUp::Init()
//{
//	Super::Init();
//
//	CacheAssets();
//}
//
//void UB2UIMailRewardPopUp::DestroySelf(class UB2UIManager* InUIManager)
//{
//	Super::DestroySelf(InUIManager);
//
//	if (ConfirmDelegate.ExecuteIfBound())
//		ConfirmDelegate.Unbind();
//}
//
//void UB2UIMailRewardPopUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//}
//
//void UB2UIMailRewardPopUp::SetConfirmCallback(const FMsgPopupOnClick& Handler)
//{
//	ConfirmDelegate = Handler;
//}
//
//void UB2UIMailRewardPopUp::FinishAnimation()
//{
//	DestroySelf(UB2UIManager::GetInstance());
//}

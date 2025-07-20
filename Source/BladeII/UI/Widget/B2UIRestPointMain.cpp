#include "B2UIRestPointMain.h"
#include "B2UIWidget.h"
#include "B2UIHeader.h"
#include "B2UIManager.h"

#include "B2UIMsgPopupUseResources.h"
#include "B2UIRewardEventSet.h"
#include "B2UIMailRewardPopUp.h"
#include "B2UIShortagePopupManager.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "Retailer.h"

void UB2UIRestPointMain::Init()
{
	bCachedRewardRequestState = ERestPointRewardRequestState::ERPRR_None;

	Super::Init();

	SubscribeEvents();
}
void UB2UIRestPointMain::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 客 鞍捞 Scene 栏肺 备己窍绰 版快 Init 矫痢俊 窍搁 Scene 俊 器窃等 header 啊 积己捞 救等 惑炔老 荐 乐澜.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("BreakTime_Reward"));

	SetMyRestInfo();
	SetRewardList();

	if (RewardItemArray.Num() > 0)
	{
		RewardItemArray[0]->SetSelected(true);

		OnSelectedReward(RewardItemArray[0]->GetRewardInfo());
	}
}

void UB2UIRestPointMain::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_DubbleReward.IsValid())
		STB_DubbleReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BreakTime_GetReward2")));
	if (STB_NormalReward.IsValid())
		STB_NormalReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BreakTime_GetReward")));
	if (STB_RestTitle.IsValid())
		STB_RestTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BreakTime_Point")));
}

void UB2UIRestPointMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UProgressBar, PB_RestPoint);

	GET_SLOT(UB2Button, BTN_NormalReward);
	GET_SLOT(UB2Button, BTN_DubbleReward);

	GET_SLOT(UTextBlock, STB_RestTitle);
	GET_SLOT(UTextBlock, TB_RewardPoint);
	GET_SLOT(UTextBlock, TB_NeedRestPoint);
	GET_SLOT(UTextBlock, TB_NeedRestPoint2);
	GET_SLOT(UTextBlock, TB_NeedGem);

	GET_SLOT(UTextBlock, STB_DubbleReward);
	GET_SLOT(UTextBlock, STB_NormalReward);

	GET_SLOT(UHorizontalBox, List_Rest);
}

void UB2UIRestPointMain::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_NormalReward, &UB2UIRestPointMain::OnClicked_NormalReward);
	BIND_CLICK_FUNC_TO_BTN(BTN_DubbleReward, &UB2UIRestPointMain::OnClicked_DubbleReward);
}

void UB2UIRestPointMain::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UIRestPointMain::OnResponseReward(const FB2ResponseRestRewardPtr& msgPtr)
{
	if (msgPtr)
	{
		if (msgPtr->reward)
		{
			UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);
			BladeIIGameImpl::GetClientDataStore().SetRestPoint(0);
		}

		if (msgPtr->used_cash > 0)
			BladeIIGameImpl::GetClientDataStore().AddUserDataWithDoc(EDocUserDataType::Gem, -msgPtr->used_cash);
	}

	SetMyRestInfo();
	UpdateRewardList();
}

void UB2UIRestPointMain::UpdateRewardList()
{
	if (List_Rest.IsValid())
	{
		for (int32 i = 0; i < RewardItemArray.Num(); i++)
		{
			FRestRewardInfo RewardInfo = RewardItemArray[i]->GetRewardInfo();
			if (RewardInfo.Count_Per_Point < 1)
				RewardInfo.Count = 1;
			else
				RewardInfo.Count = RewardInfo.Count_Per_Point*BladeIIGameImpl::GetClientDataStore().GetRestPoint();

			RewardItemArray[i]->SetRewardItemInfo(RewardInfo);

			if (RewardItemArray[i]->GetSelected())
			{
				OnSelectedReward(RewardItemArray[i]->GetRewardInfo());
			}
		}
	}
}

void UB2UIRestPointMain::SetRewardList()
{
	if (List_Rest.IsValid())
	{
		List_Rest->ClearChildren();
		RewardItemArray.Empty();
		int32 nCount = 0;

		for (auto RewardItem : BladeIIGameImpl::GetClientDataStore().GetRestRewardData())
		{
			nCount++;
			UB2UIRestPointReward* RewardWidget = CreateWidget<UB2UIRestPointReward>(GetWorld(), RestRewardClass);
			if (RewardWidget)
			{
				RewardWidget->Init();
				RewardWidget->ButtonDelegate.BindUObject(this, &UB2UIRestPointMain::OnSelectedReward);

				FRestRewardInfo RewardInfo;
				RewardInfo.Index = RewardItem->id;
				RewardInfo.RewardItem_Id = RewardItem->reward_id;
				RewardInfo.Count_Per_Point = RewardItem->reward_count_per_point;

				if (RewardItem->reward_count_per_point < 1)
					RewardInfo.Count = 1;
				else
					RewardInfo.Count = BladeIIGameImpl::GetClientDataStore().GetRestPoint();

				RewardWidget->SetRewardItemInfo(RewardInfo);

				List_Rest->AddChild(RewardWidget);

				UHorizontalBoxSlot* SlotItem = Cast<UHorizontalBoxSlot>(RewardWidget->Slot);
				if (SlotItem)
				{
					FSlateChildSize TempSizeOption;
					TempSizeOption.SizeRule = ESlateSizeRule::Fill;

					SlotItem->SetSize(TempSizeOption);
				}

				RewardItemArray.Add(RewardWidget);
			}
		}

	}
}

void UB2UIRestPointMain::SetMyRestInfo()
{
	float nRestPoint = float(BladeIIGameImpl::GetClientDataStore().GetRestPoint()) / 100.0;

	if (PB_RestPoint.IsValid())
		PB_RestPoint->SetPercent(nRestPoint);
	if (TB_RewardPoint.IsValid())
		TB_RewardPoint->SetText(FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetRestPoint()));
}

void UB2UIRestPointMain::ShowEnoughPointPopup()
{
	UB2UIMsgPopupUseResources* MsgUINoGem = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupUseResources>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BreakTime_NotEnoughPoint")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm);
}

void UB2UIRestPointMain::SubscribeEvents()
{
	// 酒付档.. 霸烙阑 舶促啊 促矫 难具 罐阑 荐 乐绰 巴老 抛聪 Unsubscribe 甫 救 秦档 寸厘 巩力啊 绝菌阑 淀?
	DeliveryRestRewardTicket = DeliveryRestRewardClass<FB2ResponseRestRewardPtr>::GetInstance().Subscribe([this](const FB2ResponseRestRewardPtr& msgPtr)
	{
		this->OnResponseReward(msgPtr);
	});
}
void UB2UIRestPointMain::UnsubscribeEvents()
{
	DeliveryRestRewardClass<FB2ResponseRestRewardPtr>::GetInstance().Unsubscribe(DeliveryRestRewardTicket);
}

void UB2UIRestPointMain::OnClicked_NormalReward()
{
	if (BladeIIGameImpl::GetClientDataStore().GetRestPoint() == 0)
	{
		ShowEnoughPointPopup();
		return;
	}

	FRestRewardInfo selectedInfo;
	for (int32 i = 0; i < RewardItemArray.Num(); i++)
	{
		if (RewardItemArray[i]->GetSelected())
			selectedInfo = RewardItemArray[i]->GetRewardInfo();

	}

	if (selectedInfo.Count_Per_Point < 1 && selectedInfo.Count_Per_Point*BladeIIGameImpl::GetClientDataStore().GetRestPoint() < 1)
	{
		//器牢飘何练 舅覆扑诀
		ShowEnoughPointPopup();
	}
	else
	{
		data_trader::Retailer::GetInstance().RequestReceiveRestReward(selectedInfo.Index, false);
		bCachedRewardRequestState = ERestPointRewardRequestState::ERPRR_Normal; // 老馆 焊惑 夸没沁澜 付农. 扼固郡捞 荐荐茄 啦悼阑 窍霸 瞪 巴.
	}
}

void UB2UIRestPointMain::OnClicked_DubbleReward()
{
	if (BladeIIGameImpl::GetClientDataStore().GetRestPoint() == 0)
	{
		ShowEnoughPointPopup();
		return;
	}

	FRestRewardInfo selectedInfo;
	for (int32 i = 0; i < RewardItemArray.Num(); i++)
	{
		if (RewardItemArray[i]->GetSelected())
			selectedInfo = RewardItemArray[i]->GetRewardInfo();

	}

	if (selectedInfo.Count_Per_Point < 1)
	{
		if (selectedInfo.Count_Per_Point*BladeIIGameImpl::GetClientDataStore().GetRestPoint() < 1)
		{
			//器牢飘何练 舅覆扑诀
			ShowEnoughPointPopup();
			return;
		}

		if (BladeIIGameImpl::GetClientDataStore().GetGemAmount() < 100) // 离何练
		{
			//离何练 舅覆扑诀
			ShortageMGR->PopupGoToShopForGemMessage();
			return;
		}
	}
	else
	{
		if (BladeIIGameImpl::GetClientDataStore().GetGemAmount() < selectedInfo.Count) // 离何练
		{
			//离何练 舅覆扑诀
			ShortageMGR->PopupGoToShopForGemMessage();
			return;
		}
	}

	data_trader::Retailer::GetInstance().RequestReceiveRestReward(selectedInfo.Index, true);
	bCachedRewardRequestState = ERestPointRewardRequestState::ERPRR_Double; // 蝶喉 焊惑 夸没沁澜 付农. 扼固郡捞 拜茄 错胶甫 眠霸 瞪 巴.
}

void UB2UIRestPointMain::OnSelectedReward(FRestRewardInfo RewardInfo)
{
	for (int32 i = 0; i < RewardItemArray.Num(); i++)
	{
		if (RewardItemArray[i]->GetRewardInfo().Index == RewardInfo.Index)
		{
			RewardItemArray[i]->SetSelected(true);
		}
		else
		{
			RewardItemArray[i]->SetSelected(false);
		}
	}

	bool bIsFixPoint = RewardInfo.Count_Per_Point < 1;
	int32 nRestPoint = bIsFixPoint ? 100 : BladeIIGameImpl::GetClientDataStore().GetRestPoint();
	if (TB_NeedRestPoint.IsValid())
		TB_NeedRestPoint->SetText(FText::AsNumber(nRestPoint));

	if (TB_NeedRestPoint2.IsValid())
		TB_NeedRestPoint2->SetText(FText::AsNumber(nRestPoint));

	if (TB_NeedGem.IsValid())
	{
		TB_NeedGem->SetText(FText::AsNumber(nRestPoint));

		bool bNotEnoughGem = false;
		if (bIsFixPoint)
			bNotEnoughGem = (BladeIIGameImpl::GetClientDataStore().GetGemAmount() < 100);
		else
			bNotEnoughGem = (BladeIIGameImpl::GetClientDataStore().GetGemAmount() < BladeIIGameImpl::GetClientDataStore().GetRestPoint());
		
		TB_NeedGem->SetColorAndOpacity(bNotEnoughGem ? UB2UIManager::GetInstance()->TextColor_Decrease : FSlateColor(FColor::White));
	}
}

///////////
void UB2UIRestPointReward::Init()
{
	Super::Init();
}

void UB2UIRestPointReward::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UTextBlock, TB_RewardName);
	GET_SLOT(UTextBlock, TB_RewardCount);
	GET_SLOT(UPanelWidget, P_Check);
	GET_SLOT(UImage, IMG_NormalBG);
	GET_SLOT(UImage, IMG_CheckBG);
	GET_SLOT(UBorder, P_Border);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);

	if (UIP_RewardIcon.IsValid())
		UIP_RewardIcon->Init();
}

void UB2UIRestPointReward::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIRestPointReward::OnClicked_RewardItem);
}

void UB2UIRestPointReward::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIRestPointReward::SetRewardItemInfo(FRestRewardInfo RewardInfo)
{
	CurrentRewardInfo = RewardInfo;

	if (TB_RewardCount.IsValid())
	{
		TB_RewardCount->SetText(FText::AsNumber(CurrentRewardInfo.Count*CurrentRewardInfo.Count_Per_Point));
	}

	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->UpdateRewardIcon(CurrentRewardInfo.RewardItem_Id, FText::GetEmpty());
		
		if (TB_RewardName.IsValid())
			TB_RewardName->SetText(UIP_RewardIcon->GetRewardText(CurrentRewardInfo.RewardItem_Id));
	
	}

	if (P_Border.IsValid())
	{
		int32 CurrentCount = BladeIIGameImpl::GetClientDataStore().GetRestPoint()*RewardInfo.Count_Per_Point;
		if(RewardInfo.Count_Per_Point < 1 && CurrentCount < 1)
			P_Border->SetContentColorAndOpacity(FLinearColor(0.3, 0.3, 0.3, 1));
		else
			P_Border->SetContentColorAndOpacity(FLinearColor(1,1,1,1));
	}
}

void UB2UIRestPointReward::OnClicked_RewardItem()
{
	ButtonDelegate.ExecuteIfBound(CurrentRewardInfo);
}

void UB2UIRestPointReward::SetSelected(bool bSelected)
{
	bIsSelected = bSelected;

	if (P_Check.IsValid())
		P_Check->SetVisibility(bIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (IMG_NormalBG.IsValid())
		IMG_NormalBG->SetVisibility(!bIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (IMG_CheckBG.IsValid())
		IMG_CheckBG->SetVisibility(bIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

bool UB2UIRestPointReward::GetSelected()
{
	return bIsSelected;
}
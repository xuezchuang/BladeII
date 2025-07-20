// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildBuffWidget.h"
#include "B2UIManager.h"

#include "B2UIGuildSkillSlot.h"
#include "B2UISlotItem.h"
#include "B2GuildBuffInfo.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2CombatStatEvaluator.h"
#include "B2UIDocHelper.h"


void UB2UIGuildBuffWidget::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close);

	GuildSkillSlotArr.SetNum(6);
	GET_SLOT_BYNAME(UB2UIGuildSkillSlot, GuildSkillSlotArr[0], UIP_GuildSkillSlot1_1);
	GET_SLOT_BYNAME(UB2UIGuildSkillSlot, GuildSkillSlotArr[1], UIP_GuildSkillSlot1_2);
	GET_SLOT_BYNAME(UB2UIGuildSkillSlot, GuildSkillSlotArr[2], UIP_GuildSkillSlot1_3);
	GET_SLOT_BYNAME(UB2UIGuildSkillSlot, GuildSkillSlotArr[3], UIP_GuildSkillSlot2_1);
	GET_SLOT_BYNAME(UB2UIGuildSkillSlot, GuildSkillSlotArr[4], UIP_GuildSkillSlot2_2);
	GET_SLOT_BYNAME(UB2UIGuildSkillSlot, GuildSkillSlotArr[5], UIP_GuildSkillSlot2_3);

	for (int32 i = 0; i < GuildSkillSlotArr.Num(); ++i)
	{
		GuildSkillSlotArr[i]->Init();
		GuildSkillSlotArr[i]->SetGuildSkillInfoDelegate(FGuildSkillInfoDelegate::CreateLambda([this](int32 SlotNumber) {this->UpdateGuildSkillInfo(SlotNumber); }));
		GuildSkillSlotArr[i]->SetGuildBuffWidgetUpdateDelegate(FGuildBuffWidgetUpdateDelegate::CreateLambda([this](int32 SkillId, int32 SkillLv) {this->UpdateGuildSkillInfo(SkillId, SkillLv); }));
	}

	GET_SLOT(UImage, IMG_Buff);
	GET_SLOT(UTextBlock, TB_BuffText);
	GET_SLOT(UTextBlock, TB_BuffLv);

	GET_SLOT(UB2RichTextBlock, TB_CurrentEffect);
	GET_SLOT(UB2RichTextBlock, TB_NextEffect);

	GET_SLOT(UB2Button, BTN_BuffLevelUp);
	GET_SLOT(UB2Button, BTN_BuffBuy);
	GET_SLOT(UB2Button, BTN_OneBTNBuy);

	GET_SLOT(UTextBlock, TB_LvelUpCost);
	GET_SLOT(UTextBlock, TB_BuffBuyCost);

	GET_SLOT(UPanelWidget, CP_BuffBuy);
	GET_SLOT(UPanelWidget, CP_OneMore);
	GET_SLOT(UPanelWidget, CP_OneBTNBuy);

	for (int32 i = 0; i < GuildSkillSlotArr.Num(); ++i)
	{
		GuildSkillSlotArr[i]->SetSlotInfo(BladeIIGameImpl::GetClientDataStore().GetGuildCurrentSkillInfo(i + 1), i + 1);

	}

	//Static Text
	GET_SLOT(UTextBlock, TB_GuildBuffNotice);
	GET_SLOT(UTextBlock, TB_BuffActive);
	GET_SLOT(UTextBlock, TB_GuildBuff);

	GET_SLOT(UTextBlock, TB_BuffEffect);
	GET_SLOT(UTextBlock, TB_CurrentOption);
	GET_SLOT(UTextBlock, TB_NextOption);
	GET_SLOT(UTextBlock, TB_LevelUp);
	GET_SLOT(UTextBlock, TB_Buy);
	GET_SLOT(UTextBlock, TB_OneBTNBuy);
	GET_SLOT(UTextBlock, TB_OneBTNCost);
	GET_SLOT(UTextBlock, TB_LevelRequire);
	
	GET_SLOT(UScaleBox, SB_OneBTNBuy);

	SB_OneBTNBuy->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIGuildBuffWidget::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildBuffWidget::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_BuffLevelUp, &UB2UIGuildBuffWidget::OnClickBTN_BuffLevelUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_BuffBuy, &UB2UIGuildBuffWidget::OnClickBTN_BuffBuy);
	BIND_CLICK_FUNC_TO_BTN(BTN_OneBTNBuy, &UB2UIGuildBuffWidget::OnClickBTN_OneBTNBuy);
}


void UB2UIGuildBuffWidget::UpdateStaticText()
{
	if (TB_GuildBuffNotice.IsValid())
		TB_GuildBuffNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff")));

	if (TB_BuffActive.IsValid())
		TB_BuffActive->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BuffActive")));

	if (TB_GuildBuff.IsValid())
		TB_GuildBuff->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildWarBuff")));

	if (TB_BuffEffect.IsValid())
		TB_BuffEffect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BuffEffect")));

	if (TB_CurrentOption.IsValid())
		TB_CurrentOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_CurrentOptionTitle")));

	if (TB_NextOption.IsValid())
		TB_NextOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_NextOptionTitle")));

	if (TB_LevelUp.IsValid())
		TB_LevelUp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_LevelUp")));

	if (TB_Buy.IsValid())
		TB_Buy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_Purchase")));

	if (TB_OneBTNBuy.IsValid())
		TB_OneBTNBuy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_PurchaseAll")));	//GeneralText累诀 鞘夸
}

void UB2UIGuildBuffWidget::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);
}

void UB2UIGuildBuffWidget::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);
}

void UB2UIGuildBuffWidget::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UIGuildBuffWidget::Init()
{
	Super::Init();

	SubscribeEvents();

	if (GuildSkillSlotArr.IsValidIndex(0))
		GuildSkillSlotArr[0]->OnClickSelect();
}

void UB2UIGuildBuffWidget::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UIGuildBuffWidget::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliverytLevelUpGuildSkillClass<FB2ResponseLevelUpGuildSkillPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseLevelUpGuildSkillPtr SkillPtr)
	{
		this->ResponseUpdate(SkillPtr->skill, SkillPtr->current_gold);
		this->GuildSkillSlotArr[SelectSkillID - 1]->PlayLvUpAni();
		this->LevelUpPlaySoundEvent();
	}
	));

	Issues.Add(DeliverytBuyGuildSkillClass<FB2ResponseBuyGuildSkillPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseBuyGuildSkillPtr BuyGuildSkill)
	{
		this->ResponseUpdate(BuyGuildSkill->skill, BuyGuildSkill->current_gold);
		this->GuildSkillSlotArr[SelectSkillID - 1]->PlayShotAni();
		this->BuyPlaySoundEvent();
	}
	));
}

void UB2UIGuildBuffWidget::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIGuildBuffWidget::ResponseUpdate(b2network::B2GuildSkillPtr InSkill, int32  InGold)
{
	//BladeIIGameImpl::GetClientDataStore().SetGuildSkillInfo(InSkill);
	//BladeIIGameImpl::GetClientDataStore().SetGoldAmount(InGold);

	//UpdateHeroCombatPower();

	//GuildSkillSlotArr[SelectSkillID - 1]->SetSlotInfo(BladeIIGameImpl::GetClientDataStore().GetGuildCurrentSkillInfo(SelectSkillID), SelectSkillID);
	//GuildSkillSlotArr[SelectSkillID - 1]->OnClickSelect();

	//for(int i=0;i<purchasableBuffCount;i++)
	//	GuildSkillSlotArr[i]->SetSlotInfo(BladeIIGameImpl::GetClientDataStore().GetGuildCurrentSkillInfo(i + 1), i + 1);

	//if (purchasedBuffCount != -1)
	//{
	//	GuildSkillSlotArr[purchasedBuffCount]->OnClickSelect();

	//	b2network::B2mdGuildSkillPtr SkillMasterData;

	//	SkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillSlotArr[purchasedBuffCount]->GetSkillID(), GuildSkillSlotArr[purchasedBuffCount]->GetSkillLv());

	//	int64 Secs_ = GuildSkillSlotArr[purchasedBuffCount]->GetRemainTime() -2;
	//	int32 Hr_ = FPlatformMath::FloorToInt(Secs_ / 3600);							//矫
	//	int32 Min_ = FPlatformMath::FloorToInt((Secs_ - (Hr_ * 3600)) / 60);			//盒
	//	int32 Sec_ = FPlatformMath::FloorToInt(Secs_ - (Hr_ * 3600) - (Min_ * 60));	//檬

	//	if (Hr_ < oneBTNBuyNumber * countAbleHour)
	//	{
	//		if (Hr_ == oneBTNBuyNumber * countAbleHour -1)
	//		{
	//			if (Min_ == 59 && Sec_ > 50)
	//			{
	//				purchasedBuffCount++;
	//				if (purchasedBuffCount < purchasableBuffCount)
	//					BuyOneBTNBuff();
	//			}
	//			else
	//			{
	//				BuyOneBTNBuff();
	//			}
	//		}
	//		else
	//		{
	//			BuyOneBTNBuff();
	//		}
	//	}
	//	else if (Hr_ == oneBTNBuyNumber * countAbleHour)
	//	{
	//		purchasedBuffCount++;
	//		if (purchasedBuffCount < purchasableBuffCount)
	//			BuyOneBTNBuff();
	//	}
	//	else
	//	{
	//		BuyOneBTNBuff();
	//	}
	//}
}

void UB2UIGuildBuffWidget::OnClickBTN_Close()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildSkill);
}

void UB2UIGuildBuffWidget::OnClickBTN_BuffLevelUp()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UIGuildBuffWidget::OnDelayBuffLevelUpCallback, 0.1f, false);
}

void UB2UIGuildBuffWidget::OnClickBTN_BuffBuy()
{
	purchasedBuffCount = -1;
	data_trader::Retailer::GetInstance().RequestBuyGuildSkill(SelectSkillID,1);
}

void UB2UIGuildBuffWidget::OnClickBTN_OneBTNBuy()
{
	///*
	//if (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= GetOneBTNPrice())
	//{
	//	purchasedBuffCount = 0;
	//	oneBTNBuyCount = 1;
	//	BuyOneBTNBuff();
	//}
	//else
	//{
	//	//榜靛啊 何练秦档 喘范阑 锭 碍力肺 版绊芒阑 剁快档废 窃
	//	HandleServerError911Class<>::GetInstance().Signal();
	//}
	//*/

	//	purchasedBuffCount = 0;

	//	b2network::B2mdGuildSkillPtr SkillMasterData;

	//	SkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillSlotArr[purchasedBuffCount]->GetSkillID(), GuildSkillSlotArr[purchasedBuffCount]->GetSkillLv());

	//	int64 Secs = GuildSkillSlotArr[purchasedBuffCount]->GetRemainTime();
	//	int32 Hr = FPlatformMath::FloorToInt(Secs / 3600);							//矫
	//	int32 Min = FPlatformMath::FloorToInt((Secs - (Hr * 3600)) / 60);			//盒
	//	int32 Sec = FPlatformMath::FloorToInt(Secs - (Hr * 3600) - (Min * 60));	//檬

	//	for (int j = 1; j <= oneBTNBuyNumber; j++)
	//	{
	//		if (Hr < countAbleHour * j)
	//		{
	//			oneBTNBuyCount = j;
	//			break;
	//		}
	//	}
	BuyOneBTNBuff();
	
}

void UB2UIGuildBuffWidget::OnDelayBuffLevelUpCallback()
{
	purchasedBuffCount = -1;
	data_trader::Retailer::GetInstance().RequestLevelUpGuildSkill(SelectSkillID);
}

void UB2UIGuildBuffWidget::BuyOneBTNBuff()
{
	if (GuildSkillSlotArr[purchasedBuffCount]->GetSkillState() > 1)
	{
		data_trader::Retailer::GetInstance().RequestBuyGuildSkill(GuildSkillSlotArr[purchasedBuffCount]->GetSkillID(), 1);
	}
}

void UB2UIGuildBuffWidget::UpdateGuildSkillInfo(int32 SlotNumber)
{
	for (int32 i = 0; i < GuildSkillSlotArr.Num(); ++i)
	{
		i + 1 == SlotNumber ? GuildSkillSlotArr[i]->OnSelectedSlot(true) : GuildSkillSlotArr[i]->OnSelectedSlot(false);
	}
}

void UB2UIGuildBuffWidget::UpdateGuildSkillInfo(int32 InSkillId, int32 InLv)
{

	SelectSkillID = InSkillId;
	b2network::B2mdGuildSkillPtr SkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(InSkillId, InLv);
	b2network::B2mdGuildSkillPtr NextSkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(InSkillId, InLv+1);

	UB2GuildSkillInfo* SkillAsset = StaticFindGuildSkillInfo();

	if (!SkillAsset || !SkillMasterData)
		return;

	const FSingleGuildSkillInfo* SkillAssetData = SkillAsset->GetSingleInfoOfID(InSkillId);

	if (!SkillAssetData)
		return;

	if (IMG_Buff.IsValid())
		IMG_Buff->SetBrushFromMaterial(SkillAssetData->ButtonIcon_Normal.Get());

	if (TB_BuffLv.IsValid())
		TB_BuffLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(InLv)));

	if (TB_LvelUpCost.IsValid())
	{
		TB_LvelUpCost->SetText(FText::AsNumber(SkillMasterData->upgrade_cost_gold));
		TB_LvelUpCost->SetColorAndOpacity(FSlateColor(BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= SkillMasterData->upgrade_cost_gold ? FLinearColor::White : FLinearColor::Red));
	}

	if (CP_BuffBuy.IsValid())
		CP_BuffBuy->SetVisibility(SkillMasterData->buy_cost_gold ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (TB_BuffBuyCost.IsValid())
	{
		TB_BuffBuyCost->SetText(FText::AsNumber(SkillMasterData->buy_cost_gold));
		TB_BuffBuyCost->SetColorAndOpacity(FSlateColor(BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= SkillMasterData->buy_cost_gold ? FLinearColor::White : FLinearColor::Red));
	}

	SetGuildSkillBuffText(SvrToCliOptionType(SkillMasterData->buff_option_type), SkillMasterData->buff_option_value, NextSkillMasterData ? NextSkillMasterData->buff_option_value : 0, SkillMasterData ? SkillMasterData->upgrade_req_guild_level : 0);

	if (SkillMasterData != NULL)
	{
		if (BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->level < SkillMasterData->upgrade_req_guild_level)
		{
			BTN_BuffLevelUp->SetIsEnabled(false);
		}
		else
		{
			BTN_BuffLevelUp->SetIsEnabled(true);
		}
	}
	else
	{
		BTN_BuffLevelUp->SetIsEnabled(false);
	}

	if (TB_OneBTNCost.IsValid())
	{
		TB_OneBTNCost->SetText(FText::AsNumber(GetOneBTNPrice()));
		TB_OneBTNCost->SetColorAndOpacity(FSlateColor(BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= GetOneBTNPrice() ? FLinearColor::White : FLinearColor::Red));
	}

	if (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= GetOneBTNPrice())
		BTN_OneBTNBuy->SetIsEnabled(true);
	else
		BTN_OneBTNBuy->SetIsEnabled(false);
}

int32 UB2UIGuildBuffWidget::GetOneBTNPrice()
{
	int32 totalPrice = 0;

	//b2network::B2mdGuildSkillPtr SkillMasterData;

	//for (int i = 0; i < purchasableBuffCount; i++)
	//{
	//	/*
	//	if (GuildSkillSlotArr[i]->GetSkillState() > 1)
	//	{
	//		SkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillSlotArr[i]->GetSkillID(), GuildSkillSlotArr[i]->GetSkillLv());
	//		totalPrice += SkillMasterData->buy_cost_gold * oneBTNBuyNumber;
	//	}
	//	*/

	//	if (GuildSkillSlotArr[i]->GetSkillState() == 2)
	//	{
	//		SkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillSlotArr[i]->GetSkillID(), GuildSkillSlotArr[i]->GetSkillLv());
	//		totalPrice += SkillMasterData->buy_cost_gold * oneBTNBuyNumber;
	//	}
	//	else if (GuildSkillSlotArr[i]->GetSkillState() == 3)
	//	{
	//		SkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillSlotArr[i]->GetSkillID(), GuildSkillSlotArr[i]->GetSkillLv());

	//		int64 Secs = GuildSkillSlotArr[i]->GetRemainTime();
	//		int32 Hr = FPlatformMath::FloorToInt(Secs / 3600);							//矫
	//		int32 Min = FPlatformMath::FloorToInt((Secs - (Hr * 3600)) / 60);			//盒
	//		int32 Sec = FPlatformMath::FloorToInt(Secs - (Hr * 3600) - (Min * 60));	//檬

	//		if (Hr == 0)
	//		{
	//			totalPrice += SkillMasterData->buy_cost_gold * oneBTNBuyNumber;
	//			continue;
	//		}

	//		for (int j = 1; j <= oneBTNBuyNumber; j++)
	//		{
	//			if (Hr < countAbleHour * j)
	//			{
	//				totalPrice += SkillMasterData->buy_cost_gold * (oneBTNBuyNumber - j + 1);
	//				break;
	//			}
	//		}
	//	}
	//}

	return totalPrice;
}

void UB2UIGuildBuffWidget::SetGuildSkillBuffText(EItemOption BuffOption, float Param1, float Param2, float Param3)
{

	if (!TB_CurrentEffect.IsValid() || !TB_NextEffect.IsValid() || !TB_BuffText.IsValid())
		return;

	int32 Param1Int = Param1;
	int32 Param2Int = Param2;
	int32 Param3Int = Param3;


	FString BuffText;
	switch (BuffOption)
	{
	case EItemOption::EIO_Offense_IncAttack:
		TB_BuffText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_ActiveBuff_AttackLevelUp")));
		BuffText = TEXT("Guild_Buff_ApplyInfo_AttackLevelUp");
		break;
	case EItemOption::EIO_Guild_Offense_IncAttack:
		TB_BuffText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_ActiveBuff_AttackLevelUp")));
		BuffText = TEXT("Guild_Buff_ApplyInfo_AttackLevelUp2");
		break;
	case EItemOption::EIO_Defense_IncDefense:
		TB_BuffText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_ActiveBuff_DefenseLevelUp")));
		BuffText = TEXT("Guild_Buff_ApplyInfo_DefenseLevelUp");
		break;
	case EItemOption::EIO_Guild_Defense_IncDefense:
		TB_BuffText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_ActiveBuff_DefenseLevelUp")));
		BuffText = TEXT("Guild_Buff_ApplyInfo_DefenseLevelUp2");
		break;
	case EItemOption::EIO_Health_IncHealth:
		TB_BuffText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_ActiveBuff_HealthLevelUp")));
		BuffText = TEXT("Guild_Buff_ApplyInfo_HealthLevelUp");
		break;
	case EItemOption::EIO_Guild_Health_IncHealth:
		TB_BuffText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_ActiveBuff_HealthLevelUp")));
		BuffText = TEXT("Guild_Buff_ApplyInfo_HealthLevelUp2");
		break;
	}

	TB_CurrentEffect->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BuffText), FText::AsNumber(Param1Int)));

	if (Param2Int > 0.f)
	{
		CP_OneMore->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TB_NextEffect->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BuffText), FText::AsNumber(Param2Int)));
	}
	else
	{
		CP_OneMore->SetVisibility(ESlateVisibility::Hidden);
		TB_NextEffect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_MaxEnhanceLevel")));
	}

	if (Param3Int > 0.f)
	{
		TB_LevelRequire->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TB_LevelRequire->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, "Guild_Buff_NextLevel"), FText::AsNumber(Param3Int)));
	}
	else
	{
		TB_LevelRequire->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIGuildBuffWidget::UpdateHeroCombatPower()
{
	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		if (auto* DocHero = UB2UIDocHelper::GetDocHero(ClassCnt))
		{
			DocHero->SetAttackPower(CombatStatEval::GetPCAttack(IntToPCClass(ClassCnt)));
			DocHero->SetDefensePower(CombatStatEval::GetPCDefense(IntToPCClass(ClassCnt)));
			DocHero->SetHealthPoint(CombatStatEval::GetPCHealth(IntToPCClass(ClassCnt)));

			DocHero->SetCombatPower(CombatStatEval::GetPCCombatPower(IntToPCClass(ClassCnt)));
			DocHero->SetChangedCombatPower(true);
		}
	}
}

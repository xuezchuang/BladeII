// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildSkillSlot.h"
#include "B2UIManager.h"

#include "B2GuildBuffInfo.h"
#include "B2UIDocHelper.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildSkillSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_OnBuffIcon);
	GET_SLOT(UTextBlock, TB_OnBuffName);
	GET_SLOT(UTextBlock, TB_OnBuffLv);
	GET_SLOT(UTextBlock, TB_OnRemainTime);

	GET_SLOT(UPanelWidget, OV_OffBuff);

	GET_SLOT(UImage, IMG_OffBuffIcon);
	GET_SLOT(UTextBlock, TB_OffBuffName);
	GET_SLOT(UTextBlock, TB_OffBuffLv);

	GET_SLOT(UPanelWidget, OV_OnBuff);
	GET_SLOT(UTextBlock, TB_BuffTime);

	GET_SLOT(UB2Button, BTN_Select);

	GET_SLOT(UPanelWidget, OV_Select);

	GET_SLOT(UPanelWidget, OV_UseOnBuff);
	GET_SLOT(UImage, IMG_UseOnBuffIcon);
	GET_SLOT(UTextBlock, TB_UseOnBuffName);
	GET_SLOT(UTextBlock, TB_UseOnBuffLv);
	GET_SLOT(UTextBlock, TB_AvailableMaxTime);
}

void UB2UIGuildSkillSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIGuildSkillSlot::OnClickSelect);
}

void UB2UIGuildSkillSlot::Init()
{
	Super::Init();
}

void UB2UIGuildSkillSlot::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}


void UB2UIGuildSkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RemainTime(BuffTime);
}

void UB2UIGuildSkillSlot::SetGuildSkillInfoDelegate(FGuildSkillInfoDelegate InGuildSkilldelegate)
{
	GuildSkillInfoDelegate = InGuildSkilldelegate;
}


void UB2UIGuildSkillSlot::SetGuildBuffWidgetUpdateDelegate(FGuildBuffWidgetUpdateDelegate InGuildBuffdelegate)
{
	GuildBuffWidgetUpdateDelegate = InGuildBuffdelegate;
}

void UB2UIGuildSkillSlot::OnSelectedSlot(bool bSelected)
{
	if (OV_Select.IsValid())
	{
		OV_Select->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}


void UB2UIGuildSkillSlot::SetSlotInfo(const b2network::B2GuildSkillPtr& SkillInfo, int32 InSkillID)
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return;


	SkillID = SkillInfo ? SkillInfo->id : InSkillID;
	SkillLv = SkillInfo ? SkillInfo->level : 0;


	b2network::B2mdGuildSkillPtr SkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(SkillID, SkillLv);

	UB2GuildSkillInfo* SkillAsset =  StaticFindGuildSkillInfo();

	BuffTime = SkillInfo ? (SkillInfo->buff_end_time ? SkillInfo->buff_end_time : INDEX_NONE) : INDEX_NONE;

	if (BuffTime == INDEX_NONE)
	{
		TB_OnRemainTime->SetVisibility(ESlateVisibility::Visible);
	}

	if (!SkillAsset || !SkillMasterData)
		return;

	const FSingleGuildSkillInfo* SkillAssetData =  SkillAsset->GetSingleInfoOfID(SkillID);

	if (!SkillAssetData)
		return;

	IMG_OnBuffIcon->SetBrushFromMaterial(SkillAssetData->ButtonIcon_Normal.Get());
	IMG_OffBuffIcon->SetBrushFromMaterial(SkillAssetData->ButtonIcon_Disabled.Get());
	IMG_UseOnBuffIcon->SetBrushFromMaterial(SkillAssetData->ButtonIcon_Normal.Get());

	int32 GuildLimitLv = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(SkillID, SkillLv - 1)->upgrade_req_guild_level;
	int32 GuildLv = GuildDoc->GetGuildLv();

	(GuildLimitLv > GuildLv) ?
		SetGuildLvText(SkillMasterData->upgrade_req_guild_level, SkillInfo ? SkillInfo->buff_end_time : 0) : SetBuffLvText(SkillLv , SkillInfo ? SkillInfo->buff_end_time : 0);

	if(GuildLimitLv <= GuildLv)
		SetUseOnBuff(SkillMasterData->buff_time_sec, SkillLv);

	SetBuffText(SvrToCliOptionType(SkillMasterData->buff_option_type));

	if (SkillMasterData->buff_time_sec == 0)
	{
		TB_OnRemainTime->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_ApplyForever")));
		OV_OnBuff->SetVisibility(ESlateVisibility::Hidden);
		if(GuildLimitLv <= GuildLv)
			PlayLoopAni();
	}
	else
	{
		RemainTime(BuffTime);
	}

	if (TB_AvailableMaxTime.IsValid())
		TB_AvailableMaxTime->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_MaxTime")));
}


void UB2UIGuildSkillSlot::SetBuffText(EItemOption BuffOption)
{
	FString BuffText;
	switch (BuffOption)
	{
	case EItemOption::EIO_Offense_IncAttack:
	case EItemOption::EIO_Guild_Offense_IncAttack:
		BuffText = TEXT("Guild_Buff_ActiveBuff_AttackLevelUp");
		break;
	case EItemOption::EIO_Defense_IncDefense:
	case EItemOption::EIO_Guild_Defense_IncDefense:
		BuffText = TEXT("Guild_Buff_ActiveBuff_DefenseLevelUp");
		break;
	case EItemOption::EIO_Health_IncHealth:
	case EItemOption::EIO_Guild_Health_IncHealth:
		BuffText = TEXT("Guild_Buff_ActiveBuff_HealthLevelUp");
		break;
	}

	if (TB_OnBuffName.IsValid())
		TB_OnBuffName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BuffText));

	if (TB_OffBuffName.IsValid())
		TB_OffBuffName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BuffText));

	if(TB_UseOnBuffName.IsValid())
		TB_UseOnBuffName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BuffText));
}


void UB2UIGuildSkillSlot::SetGuildLvText(int32 Lv, int64 BuffEndTime)
{
	if (TB_OffBuffLv.IsValid())
		TB_OffBuffLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_GuildLevel")), FText::AsNumber(Lv)));

	OV_UseOnBuff->SetVisibility(ESlateVisibility::Hidden);
	OV_OnBuff->SetVisibility(ESlateVisibility::Hidden);
	OV_OffBuff->SetVisibility(ESlateVisibility::Visible);

	BTN_Select->SetVisibility(ESlateVisibility::Hidden);

}

void UB2UIGuildSkillSlot::SetBuffLvText(int32 Lv, int64 BuffEndTime)
{
	if (TB_OnBuffLv.IsValid())
		TB_OnBuffLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(Lv)));

	if (TB_UseOnBuffLv.IsValid())
		TB_UseOnBuffLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(Lv)));

	OV_UseOnBuff->SetVisibility(BuffEndTime ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	OV_OnBuff->SetVisibility(BuffEndTime ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	OV_OffBuff->SetVisibility(ESlateVisibility::Hidden);
}


void UB2UIGuildSkillSlot::SetUseOnBuff(int32 InBuffTime, int32 InSkillLv)
{
	if (OV_UseOnBuff.IsValid())
	{
		OV_UseOnBuff->SetVisibility(!InBuffTime && InSkillLv ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

}

void UB2UIGuildSkillSlot::RemainTime(int64 timeTick)
{
	if (timeTick == INDEX_NONE)
	{
		//TB_OnRemainTime->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FDateTime Time = FDateTime::UtcNow();

	int64 RemainTime = timeTick - (Time.ToUnixTimestamp() * 1000);

	if (RemainTime <= 0)
	{
		OV_UseOnBuff->SetVisibility(ESlateVisibility::Hidden);
		OV_OnBuff->SetVisibility(ESlateVisibility::Visible);
		OV_OffBuff->SetVisibility(ESlateVisibility::Hidden);
		BuffTime = INDEX_NONE;
		return;
	}
	else
	{
		OV_UseOnBuff->SetVisibility(ESlateVisibility::Visible);
		OV_OnBuff->SetVisibility(ESlateVisibility::Hidden);
		OV_OffBuff->SetVisibility(ESlateVisibility::Hidden);
		PlayLoopAni();
	}

	FTimespan RemainTime2 = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(timeTick / 1000);

	int32 RemainDay = RemainTime2.GetDays();
	int32 RemainHour = RemainTime2.GetHours();
	int32 RemainMin = RemainTime2.GetMinutes();

	/*if (RemainTime <= 0)
	{
		RemainTime = 0;
		timeTick = INDEX_NONE;

		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), FText::Format(
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleEnd")), FText::FromString(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->guild_name)), 0, true, true, EUIMsgPopupButtonGroup::Confirm
			, FMsgPopupOnClick::CreateLambda([this]() {
			LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildMain); }
		));

	}*/

	TB_OnRemainTime->SetText(FText::Format(FText::FromString(TEXT("{0} {1}")),BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff_Apply")) ,FText::FromString(UB2UIManager::FormatSecondsAsTime(RemainTime / 1000, TEXT("hh:mm:ss")))));
	EndTime = RemainTime / 1000;
}


void UB2UIGuildSkillSlot::OnClickSelect()
{
	GuildSkillInfoDelegate.ExecuteIfBound(SlotNumber);
	GuildBuffWidgetUpdateDelegate.ExecuteIfBound(SkillID, SkillLv);
}

int32 UB2UIGuildSkillSlot::GetSkillState()
{
	//if(OV_UseOnBuff->Visibility== ESlateVisibility::Visible)
	//	return 3;	//activated

	//else if (OV_OnBuff->Visibility == ESlateVisibility::Visible)
	//	return 2;		//available

	//else if (OV_OffBuff->Visibility == ESlateVisibility::Visible)
	//	return 1;		//unavailable

	//else 
	return -1;
}
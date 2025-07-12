// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UIWealthPopupSlot.h"

#include "BladeIIGameImpl.h"
#include "B2UIDocHelper.h"

void UB2UIWealthPopupSlot::Init()
{
	Super::Init();

	SubscribeEvents();
}

void UB2UIWealthPopupSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UIWealthPopupSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Wealth);
	GET_SLOT(UTextBlock, TB_Count);
}

void UB2UIWealthPopupSlot::SubscribeEvents()
{
	//if (IsSubscribe == false)
	//{
	//	UnsubscribeEvents();

	//	Issues.Add(ChangeStaminaChargeTimeClass<>::GetInstance().Subscribe2(
	//		[this]()
	//	{
	//		this->UpdateWealthPopupSlot();
	//	}
	//	));

	//	Issues.Add(ChangePvPMatchPointChargeTimeClass<>::GetInstance().Subscribe2(
	//		[this]()
	//	{
	//		this->UpdateWealthPopupSlot();
	//	}
	//	));

	//	Issues.Add(ChangeTeamMatchPointChargeTimeClass<>::GetInstance().Subscribe2(
	//		[this]()
	//	{
	//		this->UpdateWealthPopupSlot();
	//	}
	//	));

	//	IsSubscribe = true;
	//}
}

void UB2UIWealthPopupSlot::UnsubscribeEvents()
{
	IsSubscribe = false;

	Issues.Empty();
}

void UB2UIWealthPopupSlot::InitWealthPopupSlot(const EWEALTH_TYPE& eWealthType, const int32& MaxCount)
{
	this->Init();

	CachedWealthType = eWealthType;

	CachedMaxCount = MaxCount;

	UpdateWealthPopupSlot();
}

void UB2UIWealthPopupSlot::UpdateWealthPopupSlot()
{
	UpdateWealthText(GetProgressCount(CachedWealthType), CachedMaxCount);
}

int64 UB2UIWealthPopupSlot::GetProgressCount(const EWEALTH_TYPE& eWealthType)
{
	switch (eWealthType)
	{
	case EWEALTH_TYPE::GEM:					return GetProgressCount_Gem();
	case EWEALTH_TYPE::DARK_CRYSTAL:		return GetProgressCount_DarkCrystal();
	case EWEALTH_TYPE::GOLD:				return GetProgressCount_Gold();
	case EWEALTH_TYPE::SOCIAL:				return GetProgressCount_Social();
	case EWEALTH_TYPE::BLADE_POINT:			return GetProgressCount_BladePoint();
	case EWEALTH_TYPE::REST_TIME:			return GetProgressCount_RestTime();		
	case EWEALTH_TYPE::STAGE_BOOST_TICKET:	return GetProgressCount_StageBoostTicket();
	case EWEALTH_TYPE::PVP1ON1_TICKET:		return GetProgressCount_Pvp1on1Ticket();
	case EWEALTH_TYPE::TEAM_MATCH_TICKET:	return GetProgressCount_TeamMatchTicket();
	case EWEALTH_TYPE::HERO_TOWER_TICKET:	return GetProgressCount_HeroTowerTicket();
	case EWEALTH_TYPE::FAME_POINT:			return GetProgressCount_FamePoint();
	case EWEALTH_TYPE::STAGE_AUTO_CLEAR_TICKET:			return GetProgressCount_StageAutoClearTicket();
	default:								return 0;
	}

	return 0;
}

void UB2UIWealthPopupSlot::UpdateWealthText(const int32 ProgressCount, const int32 MaxCount)
{
	if (TB_Count.IsValid() == false)
	{
		return;
	}

	FText CountText;

	if(MaxCount)
	{
		CountText = FText::FromString(FString::Printf(TEXT("%d/%d"), ProgressCount, MaxCount));
	}
	else
	{
		CountText = FText::FromString(FString::Printf(TEXT("%d"), ProgressCount));
	}
		
	TB_Count->SetText(CountText);
}

int64 UB2UIWealthPopupSlot::GetProgressCount_Gem()
{
	return BladeIIGameImpl::GetClientDataStore().GetGemAmount();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_DarkCrystal()
{
	return BladeIIGameImpl::GetClientDataStore().GetDarkCrystal();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_Gold()
{
	return BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_Social()
{
	return BladeIIGameImpl::GetClientDataStore().GetSocialPoint();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_BladePoint()
{
	return BladeIIGameImpl::GetClientDataStore().GetBladeCurPoint();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_RestTime()
{
	return BladeIIGameImpl::GetClientDataStore().GetRestPoint();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_StageBoostTicket()
{
	return BladeIIGameImpl::GetClientDataStore().GetStageBoostTicket();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_Pvp1on1Ticket()
{
	int64 MatchPoint = 0;

	if (auto* DocPvpMatch = UB2UIDocHelper::GetDocPVP1on1Rival())
	{
		MatchPoint = DocPvpMatch->GetMatchPoint();
	}

	return MatchPoint;
}

int64 UB2UIWealthPopupSlot::GetProgressCount_TeamMatchTicket()
{
	int64 TeamMatchPoint = 0;

	if (auto* DocTeamMatch = UB2UIDocHelper::GetDocTeamMatch())
	{
		TeamMatchPoint = DocTeamMatch->GetTeamMatchPoint();
	}
	
	return TeamMatchPoint;
}

int64 UB2UIWealthPopupSlot::GetProgressCount_HeroTowerTicket()
{
	return BladeIIGameImpl::GetClientDataStore().GetCurrentHeroTowerTicketCount();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_FamePoint()
{
	return BladeIIGameImpl::GetClientDataStore().GetFamePoint();
}

int64 UB2UIWealthPopupSlot::GetProgressCount_StageAutoClearTicket()
{
	return BladeIIGameImpl::GetClientDataStore().GetStageAutoClearTicket();
}

void UB2UIWealthPopupSlot::SetWealthImageFromMaterial(UMaterialInterface* pMaterial)
{
	if (pMaterial && IMG_Wealth.IsValid())
	{
		IMG_Wealth->SetBrushFromMaterial(pMaterial);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIHallOfFamePlayerInfo.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "InternalEvents.h"
#include "BladeIIGameImpl.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIHallOfFame.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"

void UB2UIHallOfFamePlayerInfo::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_UserName);
	GET_SLOT(UTextBlock, TB_PraiseCount);
	GET_SLOT(UTextBlock, TB_AttackPower);
	GET_SLOT(UTextBlock, TB_MVPNUM);

	GET_SLOT(UPanelWidget, OV_Ranking1);
	GET_SLOT(UPanelWidget, OV_Ranking2);
	GET_SLOT(UPanelWidget, OV_Ranking3);
	GET_SLOT(UPanelWidget, OV_MVP);

	GET_SLOT(UImage, IMG_ConfirmPraiseEffect);
	GET_SLOT(UImage, IMG_ConfirmPraise);

	GET_SLOT(UButton, BTN_Praise);
	GET_SLOT(UB2Button, BTN_OpenRankerInfo);

	GET_SLOT(UB2UIGradeRankAndStar, UIP_PVPGrade_Middle);
	GET_SLOT(UWidgetAnimation, ANIM_Open);
	GET_SLOT(UWidgetAnimation, ANIM_Like);
}

void UB2UIHallOfFamePlayerInfo::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Praise, &UB2UIHallOfFamePlayerInfo::OnClick_Praise);
	BIND_CLICK_FUNC_TO_BTN(BTN_OpenRankerInfo, &UB2UIHallOfFamePlayerInfo::OnClick_RankerInfo);
}

void UB2UIHallOfFamePlayerInfo::OnClick_Praise()
{
	data_trader::RequestPraiseTargetRankerClass<int32, int64, int32>::GetInstance().Signal(MatchMode, AccountID, Ranking);
	
	SetVisiblePraise(false, true);

	if (IMG_ConfirmPraise.IsValid())
	{
		IMG_ConfirmPraise->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (IMG_ConfirmPraiseEffect.IsValid())
	{
		IMG_ConfirmPraiseEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
	{
		UIHallOfFame->SetVisiblePraise(false, Ranking);
	}

	if (ANIM_Like.IsValid())
	{
		PlayAnimation(ANIM_Like.Get());
	}
}

void UB2UIHallOfFamePlayerInfo::OnClick_RankerInfo()
{
	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	{
		DocSome->SetCurrentBestCharacter(CharacterClass);
	}

	if (AccountID != 0 && CharacterClass != EPCClass::EPC_End)
	{
		//data_trader::Retailer::GetInstance().RequestFindAccount(MemberID);
	//	RequestFindAccountClass<const TArray<b2network::B2FindAccountQueryPtr>&>::GetInstance().Signal(FindAccountInfoArr);
		//data_trader::Retailer::GetInstance().RequestFindAccount(AccountID);
		//FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Signal(AccountID, EPCClass::EPC_Gladiator, EPCClass::EPC_Assassin, EPCClass::EPC_Fighter);



		if (auto SomeDoc = UB2UIDocHelper::GetDocSome())
		{
			SomeDoc->SetFindUserAccountID(AccountID);
			SomeDoc->SetOnClickUserToHallOf(true);
			data_trader::Retailer::GetInstance().RequestFindAccount(AccountID, EPCClass::EPC_Gladiator, EPCClass::EPC_Assassin, EPCClass::EPC_Fighter, EPCClass::EPC_Wizard, 0, 0);
		}
	}
}

void UB2UIHallOfFamePlayerInfo::Init()
{
	Super::Init();

	if (UIP_PVPGrade_Middle.IsValid())
	{
		UIP_PVPGrade_Middle->Init();
	}

	if (OV_Ranking1.IsValid())
	{
		OV_Ranking1->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_Ranking2.IsValid())
	{
		OV_Ranking2->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_Ranking3.IsValid())
	{
		OV_Ranking3->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_MVP.IsValid())
	{
		OV_MVP->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IMG_ConfirmPraise.IsValid())
	{
		IMG_ConfirmPraise->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IMG_ConfirmPraiseEffect.IsValid())
	{
		IMG_ConfirmPraiseEffect->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIHallOfFamePlayerInfo::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

EHallOfFameClass UB2UIHallOfFamePlayerInfo::GetHallOfFameClass(EPCClass InPcClass, int32 InRanking)
{
	if (InRanking == 1)
	{
		switch (InPcClass)
		{
		case EPCClass::EPC_Gladiator:
			return EHallOfFameClass::FIRST_Gladiator;
		case EPCClass::EPC_Fighter:
			return EHallOfFameClass::FIRST_Fighter;
		case EPCClass::EPC_Assassin:
			return EHallOfFameClass::FIRST_Assassin;
		case EPCClass::EPC_Wizard:
			return EHallOfFameClass::FIRST_Wizard;
		default:
			break;
		}
	}
	else if (InRanking == 2)
	{
		return EHallOfFameClass::SECOND;
	}
	else if (InRanking == 3)
	{
		return EHallOfFameClass::THIRD;
	}

	return EHallOfFameClass::END;
}

void UB2UIHallOfFamePlayerInfo::SetVisible(bool InVisible)
{
	this->SetVisibility(InVisible? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	
	if (InVisible)
	{
		//UpdatePosition(true);
		SetAdjustPosition();
		PlayAnimation(ANIM_Open.Get());
	}
}

void UB2UIHallOfFamePlayerInfo::SetVisiblePraise(bool InPraiseButton, bool InPraiseCompleteWidget)
{
	bool IsRanker = BladeIIGameImpl::GetClientDataStore().GetAccountId() == AccountID;

	if (BTN_Praise.IsValid())
	{
		BTN_Praise->SetVisibility(InPraiseButton && !IsRanker ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (IMG_ConfirmPraise.IsValid())
	{
		IMG_ConfirmPraise->SetVisibility(InPraiseCompleteWidget && !IsRanker ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IMG_ConfirmPraiseEffect.IsValid())
	{
		IMG_ConfirmPraiseEffect->SetVisibility(InPraiseCompleteWidget && !IsRanker ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UIHallOfFamePlayerInfo::SetUserInfo(const FRankerInfo& RankerInfo)
{
	Ranking = RankerInfo.Ranking;
	MatchMode = static_cast<int32>(RankerInfo.Mode);
	AccountID = RankerInfo.AccountID;
	CharacterClass = RankerInfo.CharacterClass;
	SetAdjustPosition();

	if (TB_UserName.IsValid())
	{
		TB_UserName->SetText(FText::FromString(RankerInfo.UserName));
	}

	if (TB_AttackPower.IsValid())
	{
		TB_AttackPower->SetText(FText::AsNumber(RankerInfo.AttackPower));
	}

	if (TB_PraiseCount.IsValid())
	{
		TB_PraiseCount->SetText(FText::AsNumber(RankerInfo.PraiseCount));
	}

	if (Ranking == 1)
	{
		if (OV_Ranking1.IsValid())
		{
			OV_Ranking1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else if (Ranking == 2)
	{
		if (OV_Ranking2.IsValid())
		{
			OV_Ranking2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else if (Ranking == 3)
	{
		if (OV_Ranking3.IsValid())
		{
			OV_Ranking3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	bool IsControlMode = static_cast<EHallOfFameMode>(MatchMode) == EHallOfFameMode::CONTROL;

	if (OV_MVP.IsValid())
	{
		OV_MVP->SetVisibility(IsControlMode ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IsControlMode && TB_MVPNUM.IsValid())
	{
		TB_MVPNUM->SetText(FText::AsNumber(RankerInfo.MvpCount));
	}

	if (UIP_PVPGrade_Middle.IsValid())
	{
		UIP_PVPGrade_Middle->SetGradeRank(RankerInfo.MatchGrade, RankerInfo.MatchStarCount);
	}
}

void UB2UIHallOfFamePlayerInfo::SetAdjustPosition()
{
	EHallOfFameClass HallOfFameClass = GetHallOfFameClass(CharacterClass, Ranking);

	if (EHallOfFameClass::END != HallOfFameClass && AdjustPositions.Find(HallOfFameClass))
	{
		AdjustPosition = AdjustPositions[HallOfFameClass];
	}
}

void UB2UIHallOfFamePlayerInfo::UpdatePosition(bool InForce)
{
	if (!InForce && this->IsPlayingAnimation())
	{
		return;
	}

	float fDPIScale = UWidgetLayoutLibrary::GetViewportScale(this);

	APlayerController* OwningPC = GetOwningPlayer();
	FVector2D vProjectedLocation;
	FVector TargetPos = TargetActor->GetActorLocation();

	if (Ranking == 1)
	{
		TargetPos.Z = 350;
	}
	else
	{
		TargetPos.Z = 320;
	}

	if (OwningPC->ProjectWorldLocationToScreen(TargetPos, vProjectedLocation))
	{
		this->ForceLayoutPrepass();

		vProjectedLocation.X -= (this->GetDesiredSize().X*fDPIScale) * 0.5f;
		vProjectedLocation.Y -= (this->GetDesiredSize().Y*fDPIScale) * 0.5f;

		vProjectedLocation.X += AdjustPosition.X * fDPIScale;
		vProjectedLocation.Y += AdjustPosition.Y * fDPIScale;

		SetPositionInViewport(vProjectedLocation);
	}
}

void UB2UIHallOfFamePlayerInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 위치 업데이트

	//APlayerController* PC = UGameplayStatics::GetLocalPlayerController(this);

	//FVector2D ProjectedLocation;
	//if (TargetActor && PC && PC->ProjectWorldLocationToScreen(TargetActor->GetActorLocation(), ProjectedLocation))
	//{
	//	ForceLayoutPrepass();

	//	FVector2D vUIPos = ProjectedLocation;

	//	vUIPos.X -= (GetDesiredSize().X * UWidgetLayoutLibrary::GetViewportScale(this)) / 2;
	//	vUIPos.Y -= GetDesiredSize().Y * UWidgetLayoutLibrary::GetViewportScale(this);

	//	// BP에서 입력하는 보정값
	//	vUIPos.X += AdjustPosition.X * UWidgetLayoutLibrary::GetViewportScale(this);
	//	vUIPos.Y += AdjustPosition.Y * UWidgetLayoutLibrary::GetViewportScale(this);

	//	SetPositionInViewport(vUIPos);
	//}
}



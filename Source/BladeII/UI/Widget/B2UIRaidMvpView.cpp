// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIRaidMvpView.h"
#include "B2RaidGameMode.h"
#include "B2UIControlMvpViewPlayerInfo.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2UIMain.h"


#include "BladeIIGameImpl.h"

UB2UIRaidMvpView::UB2UIRaidMvpView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_arPlayerInfoUI.Empty();

	AdjustPosition = FVector2D(0.0f, 0.0f);

	m_bSendResult = false;
}

void UB2UIRaidMvpView::Init()
{
	Super::Init();

	SetResultHudInfo();
	PlayAnimationEvent();

	if (AB2RaidGameMode* pGM = Cast<AB2RaidGameMode>(UGameplayStatics::GetGameMode(this)))
		pGM->SetAddFriendButtonForRaidResult();

	StopDefeatMenuClass<>::GetInstance().Signal();
	StopPauseMenuClass<>::GetInstance().Signal();
}

void UB2UIRaidMvpView::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (UB2UIControlMvpViewPlayerInfo* pUI : m_arPlayerInfoUI)
	{
		pUI->DestroySelf(InUIManager);
	}

	m_arPlayerInfoUI.Empty();
}

void UB2UIRaidMvpView::AddPlayer(AActor* TargetActor, class ABladeIIPlayer* pPlayer, bool bMvp, int32 InContribution)
{
	UB2UIControlMvpViewPlayerInfo* pUI = CreateWidget<UB2UIControlMvpViewPlayerInfo>(GetWorld(), MvpViewItemClass);

	if (pUI)
	{
		pUI->Init();
		pUI->SetHiddenKillCount();
		pUI->SetVisibleContribution();
		pUI->SetContribution(InContribution);
		
		pUI->Init();

		pUI->AddToViewport(BII_WIDGET_ZORDER_NEW_UI_MAIN);

		pUI->SetTargetActor(TargetActor);
		pUI->SetPlayer(pPlayer);

		m_arPlayerInfoUI.Add(pUI);

		if (bMvp)
			pUI->SetMvp();
				
		m_arPlayerInfoUI.Add(pUI);

		int64 PlayerAccountId = pPlayer->GetAccountId();
				
		if (pPlayer->IsLocalPlayer() || !UB2UIDocHelper::GetDocFriend()->IsMyFriend(PlayerAccountId))
			pUI->SetAskFriendBtnHidden();
	}
}

void UB2UIRaidMvpView::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Next);

	GET_SLOT(UTextBlock, RTB_RaidName);
	GET_SLOT(UB2RichTextBlock, RTB_ClearDifficulty);
	GET_SLOT(UTextBlock, RTB_PlayTime);
	GET_SLOT(UTextBlock, STB_GotoResult);
	GET_SLOT(UImage, IMG_Boss);
}

void UB2UIRaidMvpView::UpdateStaticText()
{
	if(STB_GotoResult.IsValid())
		STB_GotoResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("General_Result"))));
}

void UB2UIRaidMvpView::CloseWidgetDelegate()
{
	OnClickBTN_Next();
}

void UB2UIRaidMvpView::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Next, &UB2UIRaidMvpView::OnClickBTN_Next);
}

void UB2UIRaidMvpView::OnClickBTN_Next()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::RaidResult);

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIRaidMvpView::SetResultHudInfo()
{
	//auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	//if (RaidDoc == nullptr)
	//	return;

	//bool isResultWin = false;
	//if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
	//	isResultWin = (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin);

	//if (IMG_Boss.IsValid())
	//{
	//	int32 BossType = RaidDoc->GetLastRaidType() - 1;
	//	if (BossImages.IsValidIndex(BossType))
	//	{
	//		IMG_Boss->SetBrushFromTexture(BossImages[BossType]);
	//	}
	//}

	//const TArray<FRaidMasterData>* RaidMasterData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(RaidDoc->GetLastRaidType());
	//int32 DifficultyIdx = FMath::Max((int32)RaidDoc->GetLastRaidStep() - 1, 0);
	//if (RaidMasterData && RaidMasterData->IsValidIndex(DifficultyIdx) == true)
	//{
	//	FRaidMasterData RewardInfo = (*RaidMasterData)[DifficultyIdx];

	//	if (RTB_RaidName.IsValid())
	//	{
	//		FText DificultyText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, isResultWin ? TEXT("Raid_ResultDifficulty") : TEXT("Raid_ResultDifficulty_Fail")), FText::AsNumber(RaidDoc->GetLastRaidStep()));
	//		RTB_RaidName->SetText(DificultyText);
	//	}

	//	if (RTB_ClearDifficulty.IsValid())
	//	{
	//		FText ResultText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, isResultWin ? TEXT("Raid_ResultScuccess") : TEXT("Raid_ResultFailed"));
	//		RTB_ClearDifficulty->SetText(ResultText);
	//	}

	//}

	//if (RTB_PlayTime.IsValid())
	//{
	//	int32 Secs = RaidDoc->GetRaidEndBattleTime();
	//	int32 Hr = FPlatformMath::FloorToInt(Secs / 3600);
	//	int32 Min = FPlatformMath::FloorToInt((Secs - (Hr * 3600)) / 60);
	//	int32 Sec = FPlatformMath::FloorToInt(Secs - (Hr * 3600) - (Min * 60));

	//	if (Secs == 0)
	//		RTB_PlayTime->SetText(FText::FromString(TEXT("-")));
	//	else
	//	{
	//		RTB_PlayTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Time_NM_NS")), FText::AsNumber(Min), FText::AsNumber(Sec)));
	//	}
	//}
}

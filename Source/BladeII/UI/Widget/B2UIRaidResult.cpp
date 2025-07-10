// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRaidResult.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocRaid.h"

#include "B2UIRaidResultSlot.h"
#include "B2UIRaidRoulette.h"
#include "B2RaidGameMode.h"
#include "B2UIRewardEventSet.h"
#include "B2ItemInfo.h"
#include "B2UIStageResultLevelUp.h"


#include "BladeIIGameImpl.h"

void UB2RaidResult::Init()
{
	Super::Init();

	if (auto* RaidDoc = UB2UIDocHelper::GetDocRaid())
	{
		if (IMG_Boss.IsValid())
		{
			int32 BossType = RaidDoc->GetLastRaidType() - 1;
			if (BossImages.IsValidIndex(BossType))
			{
				IMG_Boss->SetBrushFromTexture(BossImages[BossType]);
			}
		}
	}

	if (BTN_SkipRoulette.IsValid())
		BTN_SkipRoulette->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2RaidResult::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);
}

void UB2RaidResult::CacheAssets()
{
	GET_SLOT(UHorizontalBox, X_BottomButton);
	GET_SLOT(UB2Button, BTN_Bonus);
	GET_SLOT(UB2Button, BTN_Lobby);
	GET_SLOT(UB2Button, BTN_Raid);
	GET_SLOT(UB2Button, BTN_Retry);
	GET_SLOT(UB2Button, BTN_PartyRetry);
	GET_SLOT(UB2Button, BTN_SkipRoulette);

	GET_SLOT(UB2RichTextBlock, RTB_Close);
	GET_SLOT(UCanvasPanel, CP_Bottom);

	GET_SLOT(UTextBlock, RTB_RaidName);
	GET_SLOT(UB2RichTextBlock, RTB_ClearDifficulty);
	GET_SLOT(UCanvasPanel, CP_LeftStatePanel);

	GET_SLOT(UTextBlock, RTB_PlayTime);
	GET_SLOT(UCanvasPanel, CP_PlayTime);

	GET_SLOT(UTextBlock, RTB_GuideContribution);
	GET_SLOT(UOverlay, CP_Contribution);

	GET_SLOT(UB2UIRewardIcon, BonusItem);
	if (BonusItem.IsValid())
		BonusItem->Init();

	GET_SLOT(UPanelWidget, CP_Roulette);
	GET_SLOT(UB2RichTextBlock, RTB_GuideBonus);
	GET_SLOT(UB2UIRaidRoulette, Roulette);
	if (Roulette.IsValid())
		Roulette->Init();

	RankSlots.SetNum(static_cast<uint8>(ERaidRankSlot::Max));
	GET_SLOT_BYNAME(UB2UIRaidResultSlot, RankSlots[static_cast<uint8>(ERaidRankSlot::Slot_0)], Slot0);
	GET_SLOT_BYNAME(UB2UIRaidResultSlot, RankSlots[static_cast<uint8>(ERaidRankSlot::Slot_1)], Slot1);
	GET_SLOT_BYNAME(UB2UIRaidResultSlot, RankSlots[static_cast<uint8>(ERaidRankSlot::Slot_2)], Slot2);
	GET_SLOT_BYNAME(UB2UIRaidResultSlot, RankSlots[static_cast<uint8>(ERaidRankSlot::Slot_3)], Slot3);

	for (int32 i = 0; i < RankSlots.Num(); ++i)
	{
		auto ThisSlot = RankSlots[i];
		if (ThisSlot.IsValid())
		{
			ThisSlot->Init();
		}
	}

	GET_SLOT(UB2UIStageResultLevelUp, UIP_LevelUp);
	if (UIP_LevelUp.IsValid())
		UIP_LevelUp->Init();

	GET_SLOT(UTextBlock, STB_GotoLobby);
	GET_SLOT(UTextBlock, STB_GotoRaid);
	GET_SLOT(UTextBlock, STB_ReTry);
	GET_SLOT(UTextBlock, STB_ViewBonus);
	GET_SLOT(UTextBlock, STB_PartyRetry);
	GET_SLOT(UImage, IMG_Boss);
}

void UB2RaidResult::NativeConstruct()
{
	Super::NativeConstruct();

	ShowResult();
}

void UB2RaidResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2RaidResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Bonus, &UB2RaidResult::OnClicked_Bonus);
	BIND_CLICK_FUNC_TO_BTN(BTN_Lobby, &UB2RaidResult::OnClicked_Lobby);
	BIND_CLICK_FUNC_TO_BTN(BTN_Raid, &UB2RaidResult::OnClicked_Raid);
	BIND_CLICK_FUNC_TO_BTN(BTN_Retry, &UB2RaidResult::OnClicked_Retry);
	BIND_CLICK_FUNC_TO_BTN(BTN_PartyRetry, &UB2RaidResult::OnClicked_PartyRetry);
	BIND_CLICK_FUNC_TO_BTN(BTN_SkipRoulette, &UB2RaidResult::OnClicked_SkipRoulette);
}

void UB2RaidResult::UpdateStaticText()
{
	if (RTB_Close.IsValid())
		RTB_Close->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Quit")));

	if (RTB_GuideContribution.IsValid())
		RTB_GuideContribution->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResultContribution")));

	if (RTB_GuideBonus.IsValid())
		RTB_GuideBonus->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResultBonus")));
	
	if (STB_GotoLobby.IsValid())
		STB_GotoLobby->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Lobby")));
	if (STB_GotoRaid.IsValid())
		STB_GotoRaid->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Raid")));
	if (STB_ReTry.IsValid())
		STB_ReTry->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_Restart")));
	if (STB_PartyRetry.IsValid())
		STB_PartyRetry->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_PartyRetry")));
	if (STB_ViewBonus.IsValid())
		STB_ViewBonus->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_BonusReward")));
}

void UB2RaidResult::CloseWidgetDelegate()
{
	if (BTN_Bonus.IsValid() && BTN_Bonus->GetVisibility() == ESlateVisibility::Visible)
	{
		OnClicked_Bonus();
	}
	else if (X_BottomButton.IsValid() && X_BottomButton->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		OnClicked_Raid();
	}
}

void UB2RaidResult::OnClicked_Bonus()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_Bonus"));

	PlayBonusItemAnim(0);

	if (BTN_Bonus.IsValid())
		BTN_Bonus->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2RaidResult::OnClicked_Lobby()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_Lobby"));

	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (RaidDoc)
		RaidDoc->SetReturnToPageType(ERaidGotoWhere::Lobby);

	ReturnToRaidMainClass<>::GetInstance().Signal();
}

void UB2RaidResult::OnClicked_Raid()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_Raid"));
	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (RaidDoc)
		RaidDoc->SetReturnToPageType(ERaidGotoWhere::RaidMain);

	ReturnToRaidMainClass<>::GetInstance().Signal();
}

void UB2RaidResult::OnClicked_Retry()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_Retry"));
	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (RaidDoc)
		RaidDoc->SetReturnToPageType(ERaidGotoWhere::Rematch);

	ReturnToRaidMainClass<>::GetInstance().Signal();
}

void UB2RaidResult::OnClicked_PartyRetry()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_PartyRetry"));
	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (RaidDoc)
		RaidDoc->SetReturnToPageType(ERaidGotoWhere::PartyRematch);

	ReturnToRaidMainClass<>::GetInstance().Signal();
}

void UB2RaidResult::OnClicked_SkipRoulette()
{
	if (Roulette.IsValid())
		Roulette->SkipRoulette();

	if (BTN_SkipRoulette.IsValid())
		BTN_SkipRoulette->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2RaidResult::FinishBonusItemAnim()
{
	CheckAndOpenLevelUpPart();
}

void UB2RaidResult::OnReadyToRoulette()
{
	if (Roulette.IsValid())
		Roulette->StartRoulette();

	if (BTN_SkipRoulette.IsValid())
		BTN_SkipRoulette->SetVisibility(ESlateVisibility::Visible);
}

void UB2RaidResult::OnFinishedSpinRoulette()
{
	if (BTN_SkipRoulette.IsValid())
		BTN_SkipRoulette->SetVisibility(ESlateVisibility::Collapsed);

	if (Roulette.IsValid())
		PlayBonusItemAnim(Roulette->GetCurrentTargetIndex() +1);

	SetVisibleBonusButton(false);
}

void UB2RaidResult::SetCloseTimeON()
{
	DeltaTimeCount = 0.0f;
	const int32 StartTimeSec = 5;
	CloseTimeCount = StartTimeSec;

	IsStartCloseTimer = true;
}

void UB2RaidResult::ShowResult()
{
	SetResultHudInfo();

	TArray<FRaidResultRankingInfo> RankArray = GetRankArrayInfo();
	SetResultSlotInfo(RankArray);
	SetRouletteInfo(RankArray);
}

void UB2RaidResult::SetResultHudInfo()
{
	//auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	//if (RaidDoc == nullptr)
	//	return;

	//bool isResultWin = false;
	//if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
	//	isResultWin = (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin);

	//const TArray<FRaidMasterData>* RaidMasterData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(RaidDoc->GetLastRaidType());
	//int32 DifficultyIdx = FMath::Max(RaidDoc->GetLastRaidStep() - 1, 0);
	//if (RaidMasterData && RaidMasterData->IsValidIndex(DifficultyIdx) == true)
	//{
	//	FRaidMasterData RewardInfo = (*RaidMasterData)[DifficultyIdx];
	//	
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

	//if (CP_Roulette.IsValid())
	//	CP_Roulette->SetVisibility(isResultWin ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

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

	//if (auto* B2RaidGM = Cast<AB2RaidGameMode>(UGameplayStatics::GetGameMode(this)))
	//{
	//	const bool IsAlone = B2RaidGM->IsStandaloneGame();
	//	if (BTN_PartyRetry.IsValid())
	//		BTN_PartyRetry->SetIsEnabled(!IsAlone);
	//}
	
}

void UB2RaidResult::SetResultSlotInfo(const TArray<FRaidResultRankingInfo> &RankArray)
{
	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (RaidDoc == nullptr)
		return;
	
	bool isResultWin = false;
	if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
	{
		isResultWin = (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin);
	}

	////////////////////////////////////////
	//          StepReward 
	///////////////////////////////////////
	int32 StepRewardID = 0;
	int32 StepRewardGrade = 0;
	int32 StepRewardCount = 0;

	const TArray<FRaidMasterData>* RaidData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(RaidDoc->GetLastRaidType());

	// 기여도가 부족한 사람은 보상 정보가 없으므로, rewardptr 값을 참조할 수 없음.
	// 그래서 id 와 grade 값은 마스터 데이터에서 찾아오도록함.
	if (RaidData && RaidData->IsValidIndex(RaidDoc->GetLastRaidStep() - 1))
	{
		FRaidMasterData RewardInfo = (*RaidData)[RaidDoc->GetLastRaidStep() - 1];
		StepRewardID = RewardInfo.StepRewardID;
		StepRewardGrade = (int32)((RewardInfo.StepRewardID % 1000) / 100);
	
	}
	
	const FB2ResponseRaidResultPtr& Rewards = RaidDoc->GetRaidResultReward();
	if (Rewards && Rewards->step_clear_reward && Rewards->step_clear_reward->raw_reward)
	{
		StepRewardCount = Rewards->step_clear_reward->raw_reward->count;
	}

	////////////////////////////////////////
	//          GuildCount
	///////////////////////////////////////
	int32 MyGuildCount = 0;
	for (int32 i = 0; i < RankSlots.Num(); ++i)
	{
		if (RaidDoc->GetIsMyGuildMemberByIndex(i))
			MyGuildCount++;
	}

	///////////////////////////////////////////////
	//      MainReward & EachSlot RewardInfo Set
	///////////////////////////////////////////////
	for (int32 i = 0; i < RankSlots.Num(); ++i)
	{
		auto ThisSlot = RankSlots[i];
		if (ThisSlot.IsValid())
		{
			const FRaidResultRankingInfo &Each = RankArray[i];

			FRaidResultSlotInfo SlotInfo;
			
			int32 RewardItemId = 0;
			int32 RewardItemCount = 0;
			bool bMainUsedRewardID = false;
			RaidDoc->GetRaidRewardInfo(i + 1, Each.PCClass,  RewardItemId, RewardItemCount, bMainUsedRewardID);

			SlotInfo.Number = i + 1;
			SlotInfo.Level = Each.Level;
			SlotInfo.PCIndex = Each.PCClass;
			SlotInfo.RewardID = RewardItemId;
			SlotInfo.RewardGrade = (int32)((RewardItemId%1000)/100);
			SlotInfo.RewardCount = RewardItemCount;
			SlotInfo.ContributionPercent = Each.Contribution; 
			SlotInfo.Name = Each.CharName;
			SlotInfo.AccountID = Each.AccountID;

			SlotInfo.StepRewardID = StepRewardID;
			SlotInfo.StepRewardGrade = StepRewardGrade;
			SlotInfo.StepRewardCount = StepRewardCount;
			SlotInfo.IsMyGuildMan = Each.IsMyGuildMan;
			SlotInfo.IsGuildParty = (MyGuildCount > 1);
			SlotInfo.IsMainUsedRewardID = bMainUsedRewardID;

			SlotInfo.IsMy = false;
			AB2RaidGameMode* B2RaidGM = Cast<AB2RaidGameMode>(UGameplayStatics::GetGameMode(this));
			if (B2RaidGM)
			{
				if (B2RaidGM->GetMyNetId() == Each.NetID)
				{
					SlotInfo.IsMy = true;
					SlotInfo.Level = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(SlotInfo.PCIndex);
				}
			}

			if (Each.IsExitGame)
				SlotInfo.SlotState = ERaidResultSlotState::Giveup;
			else
				SlotInfo.SlotState = isResultWin ? ERaidResultSlotState::Win : ERaidResultSlotState::Lose;

			if (SlotInfo.SlotState != ERaidResultSlotState::Giveup)
				SlotInfo.GetExp = BladeIIGameImpl::GetClientDataStore().GetExpectedExpOfRaidGame(RaidDoc->GetLastRaidType(), RaidDoc->GetLastRaidStep());

			ThisSlot->UpdateSlot(SlotInfo);
		}
	}
}

void UB2RaidResult::SetVisibleBonusButton(bool bIsvisible)
{
	if (bIsvisible)
	{
		if (X_BottomButton.IsValid())
			X_BottomButton->SetVisibility(ESlateVisibility::Collapsed);
		if (BTN_Bonus.IsValid())
			BTN_Bonus->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (X_BottomButton.IsValid())
			X_BottomButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (BTN_Bonus.IsValid())
			BTN_Bonus->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2RaidResult::SetRouletteInfo(const TArray<FRaidResultRankingInfo> &IndexArray)
{
	SetVisibleBonusButton(false);

	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (RaidDoc == nullptr)
		return;
	
	bool isResultWin = false;
	if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
		isResultWin = (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin);

	if (isResultWin == false)
	{
		if (CP_Roulette.IsValid())
			CP_Roulette->SetVisibility(ESlateVisibility::Hidden);

		return;
	}

	if (BonusItem.IsValid())
	{
		int32 BonusItemId = 0;
		int32 BonusItemCount = 0;
		bool BonusUseRewardID = false;
		int32 BonusUserRank = 0;
		EPCClass BonusUserPCClass = EPCClass::EPC_End;

		for (int32 i = 0; i < IndexArray.Num(); ++i)
		{
			if (IndexArray.IsValidIndex(i))
			{
				if (IndexArray[i].NetID == RaidDoc->GetRaidBonusPlayerNetID())
				{
					BonusUserRank = i + 1;
					BonusUserPCClass = IndexArray[i].PCClass;
					break;
				}
			} 
		}

		RaidDoc->GetRaidBonusRewardInfo(BonusUserRank,
										BonusUserPCClass,
										BonusItemId,
										BonusItemCount,
										BonusUseRewardID);
		
		int32 bonusGrade = (int32)((BonusItemId % 1000) / 100);
		FText BonusItemCountText = BonusItemCount > 0 ? FText::AsNumber(BonusItemCount) : FText::GetEmpty();

		if(BonusUseRewardID)
			BonusItem->UpdateRewardIcon(BonusItemId, BonusItemCountText);
		else
			BonusItem->UpdateRewardIconByItemIndex(BonusItemId, BonusItemCountText);

		BonusItem->SetGrade(bonusGrade);
	}

	if (Roulette.IsValid())
	{
		Roulette->RouletteSpinDelegate.BindUObject(this, &UB2RaidResult::OnFinishedSpinRoulette);

		TArray<FRaidRouletteInfo> InfoArray;
		int32 TargetIndex = 0;
		bool bIsMyBonus = false;
		for (int32 i = 0; i < IndexArray.Num(); ++i)
		{
			const FRaidResultRankingInfo &Each = IndexArray[i];

			FRaidRouletteInfo Info;
			if (Each.IsExitGame)
				Info.State = FRaidRouletteInfo::EState::Giveup;
			else
				Info.State = isResultWin ? FRaidRouletteInfo::EState::Win : FRaidRouletteInfo::EState::Lose;

			Info.PCIndex = 0;
			Info.Contribution = Each.Contribution;
			Info.PieceColor = RouletteColors.IsValidIndex(i) ? RouletteColors[i] : FLinearColor(FColor(255, 255, 255, 255));
			InfoArray.Add(Info);

			if (Each.NetID > -1 && RaidDoc->GetRaidBonusPlayerNetID() == Each.NetID)
			{
				TargetIndex = i;
				BonusItemNumber = i;
			}

			AB2RaidGameMode* B2RaidGM = Cast<AB2RaidGameMode>(UGameplayStatics::GetGameMode(this));
			if (B2RaidGM)
			{
				if (B2RaidGM->GetMyNetId() == Each.NetID && Info.Contribution < 1 && isResultWin)
				{
					CheckAndOpenLevelUpPart();
					return;
				}
			}
		}

		SetVisibleBonusButton(true);
		Roulette->SetRoulette(InfoArray, TargetIndex);
	}
}

TArray<FRaidResultRankingInfo> UB2RaidResult::GetRankArrayInfo()
{
	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (RaidDoc == nullptr)
		return TArray<FRaidResultRankingInfo>();

	TArray<FRaidResultRankingInfo> RankArray;
	for (int32 i = 0; i < RankSlots.Num(); ++i)
	{
		FRaidResultRankingInfo Rank;
		Rank.NetID = RaidDoc->GetNetIdByIndex(i);
		Rank.Contribution = RaidDoc->GetRaidContributionByIndex(i);
		Rank.Level = RaidDoc->GetPCLevelByIndex(i);
		Rank.PCClass = RaidDoc->GetPCClassByIndex(i);
		Rank.IsExitGame = RaidDoc->GetPCExitMainGameByIndex(i);
		Rank.CharName = RaidDoc->GetCharNameByIndex(i);
		Rank.AccountID = RaidDoc->GetAccountIdByIndex(i);
		Rank.IsMyGuildMan = RaidDoc->GetIsMyGuildMemberByIndex(i);
		RankArray.Add(Rank);
	}

	// 레벨 높은사람이 우선.
	struct FRaidRaking_LevelSorter
	{
		bool operator()(const FRaidResultRankingInfo& A, const FRaidResultRankingInfo& B) const
		{
			return (A.Level > B.Level) ? true : false;
		}
	};
	RankArray.StableSort(FRaidRaking_LevelSorter());

	// 공헌도 높은사람이 우선
	struct FRaidRaking_ContributionSorter
	{
		bool operator()(const FRaidResultRankingInfo& A, const FRaidResultRankingInfo& B) const
		{
			return (A.Contribution > B.Contribution) ? true : false;
		}
	};
	RankArray.StableSort(FRaidRaking_ContributionSorter());

	// 그다음 나간 사람이 뒤로 
	struct FRaidRaking_ExitUserSorter
	{
		bool operator()(const FRaidResultRankingInfo& A, const FRaidResultRankingInfo& B) const
		{
			return (!A.IsExitGame && B.IsExitGame) ? true : false;
		}
	};
	RankArray.StableSort(FRaidRaking_ExitUserSorter());

	// 더미아이디는 제일 뒤로
	struct FRaidRaking_DummyIDSorter
	{
		bool operator()(const FRaidResultRankingInfo& A, const FRaidResultRankingInfo& B) const
		{
			return (A.NetID > 0 && B.NetID < 0) ? true : false;
		}
	};
	RankArray.StableSort(FRaidRaking_DummyIDSorter());


	return RankArray;
}

void UB2RaidResult::CheckAndOpenLevelUpPart()
{
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	TArray<EPCClass> ThisPCClass;

	ThisPCClass.Add(CharacterDataStore.GetMainPlayerClass());

	for (auto pcclass : ThisPCClass)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
		//UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(pcclass));
		//bool bCharLevelUp = (FMath::Max(DocHero->GetBasicUpgradeLevel(), DocHero->GetExtraUpgradeLevel()) > DocHero->GetCurrentLevel());
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(pcclass);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(pcclass);
		if (ClearAfterLevel > ClearBeforeLevel)
		{
			if (UIP_LevelUp.IsValid())
			{
				UIP_LevelUp->StartMe(ThisPCClass);
				UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnLevelUpFXFinish(); });
				return;
			}
		}
	}

	OnLevelUpFXFinish();
}

void UB2RaidResult::OnLevelUpFXFinish()
{
	SetVisibleBonusButton(false);
}
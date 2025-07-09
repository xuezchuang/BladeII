#include "BladeII.h"
#include "B2UICounterAttackSelectCharacter.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIPVPEntryHero.h"
#include "B2UIDocHelper.h"
#include "B2UICharacterSelect.h"
#include "B2UICounterAttackSelectCharacterSlot.h"
#include "B2StageManager.h"
#include "B2ItemInfo.h"
#include "B2UIMsgPopupUseResources.h"
#include "Retailer.h"
#include "B2UIStartButton.h"
#include "B2UIRewardEventSet.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIRecommandPower.h"
#include "TutorialManager.h"
#include "BladeIIGameImpl.h"
#include "B2CombatStatEvaluator.h"
#include "B2UIMsgPopupHeroTowerSweep.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIFairyLinkButton.h"

UB2UICounterAttackSelectCharacter::~UB2UICounterAttackSelectCharacter()
{
	DeliveryBuyTicketCounterDungeonClass<FB2CounterDungeonBuyTicket>::GetInstance().Unsubscribe(DeliveryBuyTicket);
}

void UB2UICounterAttackSelectCharacter::Init()
{
	Super::Init();

	SetMaxDifficulty();
	SubscribeEvent();
}

void UB2UICounterAttackSelectCharacter::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("CounterAttack_CharacterSelect"));
}

void UB2UICounterAttackSelectCharacter::CacheAssets()
{
	UIP_EntryArr.SetNum(4);

	GET_SLOT(UB2UIPVPEntryHero, UIP_Entry);
	if (UIP_Entry.IsValid())
		UIP_Entry->Init();

	GET_SLOT_BYNAME(UB2UIPVPEntryHero, UIP_EntryArr[0], UIP_Entry1);
	GET_SLOT_BYNAME(UB2UIPVPEntryHero, UIP_EntryArr[1], UIP_Entry2);
	GET_SLOT_BYNAME(UB2UIPVPEntryHero, UIP_EntryArr[2], UIP_Entry3);
	GET_SLOT_BYNAME(UB2UIPVPEntryHero, UIP_EntryArr[3], UIP_Entry4);

	for (int32 i = 0; i < UIP_EntryArr.Num(); ++i)
	{
		UIP_EntryArr[i]->Init();
		UIP_EntryArr[i]->FOnClickBtnSelect.BindUObject(this, &UB2UICounterAttackSelectCharacter::OnClickEntry);
	}

	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, TB_RecommandPower);
	GET_SLOT(UTextBlock, STB_RecommandPower);

	GET_SLOT(UButton, BTN_GoToHeroMgmt);


	GET_SLOT(UButton, BTN_Sweep);
	GET_SLOT(UTextBlock, TB_SweepButton);

	GET_SLOT(UTextBlock, TB_DailyEnter);

	GET_SLOT(UB2UIStartButton, UIP_StartBT);
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->Init();
		UIP_StartBT->StartButtonDelegateBindLambda([this]() {StartBattle(); });
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_BattleStart")));
	}

	GET_SLOT(UB2UIDungeonDifficulty, UIP_DungeonLevelofDifficulty);
	if (UIP_DungeonLevelofDifficulty.IsValid())
	{
		CAPTURE_UOBJECT(UB2UICounterAttackSelectCharacter);
		UIP_DungeonLevelofDifficulty->Init();
		UIP_DungeonLevelofDifficulty->SetDifficultyDelegate(
			USE_CAPTURE_OBJECT(int32 InDifficulty)
			Capture->ChangeDifficulty(InDifficulty);
		END_CAPTURE_OBJECT()
			);
	}

	//StaticText
	GET_SLOT(UTextBlock, STB_DungeonLv);
	GET_SLOT(UTextBlock, STB_EquipManagement);
	GET_SLOT(UTextBlock, STB_RecommandPower);
	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, STB_EntenHero);
	GET_SLOT(UTextBlock, STB_RewardInfo);
	GET_SLOT(UTextBlock, STB_DailyEnter);
	GET_SLOT(UB2RichTextBlock, STB_JoinExplanation);

	GET_SLOT(UHorizontalBox, List_Reward);

	GET_SLOT(UB2UIRecommandPower, UIP_StageRecommendPower);
	if (UIP_StageRecommendPower.IsValid())
	{
		UIP_StageRecommendPower->Init();
	}

	GET_SLOT(UB2UIFairyLinkButton, UIP_FairyLinkButton);
	if (UIP_FairyLinkButton.IsValid())
		UIP_FairyLinkButton->Init();

	GET_SLOT(UTextBlock, TB_MonsterName);
	GET_SLOT(UTextBlock, TB_MonsterDesc);
	GET_SLOT(UTextBlock, TB_BestScoreDesc);
}

void UB2UICounterAttackSelectCharacter::UpdateStaticText()
{
	if (STB_DungeonLv.IsValid())
		STB_DungeonLv->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_DungeonLvChoice")));

	if (STB_EquipManagement.IsValid())
		STB_EquipManagement->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_EquipMgmtButton")));

	if (STB_RecommandPower.IsValid())
		STB_RecommandPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_RecomCombatPower")));

	if (STB_CombatPower.IsValid())
		STB_CombatPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_CurrentCombatpower")));

	if (STB_EntenHero.IsValid())
		STB_EntenHero->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_EnterHero")));

	if (STB_RewardInfo.IsValid())
		STB_RewardInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RewardInfo")));

	if (STB_DailyEnter.IsValid())
		STB_DailyEnter->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_DailyEnter")));

	if (STB_JoinExplanation.IsValid())
		STB_JoinExplanation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_JoinCounterExplanation")));

	if (TB_MonsterName.IsValid())
		TB_MonsterName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_BossName")));

	if (TB_MonsterDesc.IsValid())
		TB_MonsterDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_BossExplanation")));

	if (TB_BestScoreDesc.IsValid())
		TB_BestScoreDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_HighestRecord2")));
}

void UB2UICounterAttackSelectCharacter::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_GoToHeroMgmt, &UB2UICounterAttackSelectCharacter::OnClickBtnGoToHeroMgmt);
	BIND_CLICK_FUNC_TO_BTN(BTN_Sweep, &UB2UICounterAttackSelectCharacter::OnClickSweepBattle);
}

void UB2UICounterAttackSelectCharacter::BindDocAuto()
{
	SetSelectCharacter();

	auto CounterDungeonDoc = UB2UIDocHelper::GetDocCounterAttack();

	if (!CounterDungeonDoc)
		return;

	CounterDungeonDoc->OnHighestLevelChanged.AddUObject(this, &UB2UICounterAttackSelectCharacter::OnHighestUpdate);
}

void UB2UICounterAttackSelectCharacter::UnbindDoc()
{

}

void UB2UICounterAttackSelectCharacter::DestroySelf(UB2UIManager* InUIManager)
{
	if (UIP_StageRecommendPower.IsValid())
	{
		UIP_StageRecommendPower->DestroySelf(InUIManager);
	}

	UnsubscribeEvent();

	Super::DestroySelf(InUIManager);
}

void UB2UICounterAttackSelectCharacter::SetSelectCharacter()
{
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	for (int32 i = 0; i < PCClassToInt(EPCClass::EPC_End); ++i)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
		//auto* DocHero = UB2UIDocHelper::GetDocHero(i);
		if (UIP_EntryArr.IsValidIndex(i) && UIP_EntryArr[i].IsValid())
		{
			//UIP_EntryArr[i]->SetEntryInfo(IntToPCClass(i), EEntryType::EntryType_None, DocHero->GetCurrentLevel());
			const int32 SelectCharLevel = CharacterDataStore.GetCharacterLevel(IntToPCClass(i));
			UIP_EntryArr[i]->SetEntryInfo(IntToPCClass(i), EEntryType::EntryType_None, SelectCharLevel);
		}
	}
}

const FString UB2UICounterAttackSelectCharacter::DayDivision(EUICounterDayType Day)
{
	switch (Day)
	{
	case EUICounterDayType::Monday:
		return  FString::Printf(TEXT("CounterAttack_Monday"));
	case EUICounterDayType::Tuesday:
		return  FString::Printf(TEXT("CounterAttack_Tuesday"));
	case EUICounterDayType::Wednesday:
		return  FString::Printf(TEXT("CounterAttack_Wedneday"));
	case EUICounterDayType::Thursday:
		return  FString::Printf(TEXT("CounterAttack_Thursday"));
	case EUICounterDayType::Friday:
		return  FString::Printf(TEXT("CounterAttack_Friday"));
	case EUICounterDayType::Saturday:
		return  FString::Printf(TEXT("CounterAttack_Saturday"));
	case EUICounterDayType::Sunday:
		return  FString::Printf(TEXT("CounterAttack_Sunday"));
	default:
		return FString::Printf(TEXT(""));
	}
}

const FString UB2UICounterAttackSelectCharacter::DungeonTypeDivision(int32 DungeonType)
{
	switch (DungeonType)
	{
	case b2network::B2CounterDungeon::TYPE::ARMOR:
		return  FString::Printf(TEXT("CounterAttack_DungeonName1"));
	case b2network::B2CounterDungeon::TYPE::WEAPON:
		return  FString::Printf(TEXT("CounterAttack_DungeonName2"));
	case b2network::B2CounterDungeon::TYPE::ACCESSORY:
		return  FString::Printf(TEXT("CounterAttack_DungeonName3"));
	case b2network::B2CounterDungeon::TYPE::GOLD:
		return  FString::Printf(TEXT("CounterAttack_DungeonName4"));
	default:
		return FString::Printf(TEXT(""));
	}
}

void UB2UICounterAttackSelectCharacter::SetEnterCount(int32 Count, int32 MaxCount)
{
	if (TB_DailyEnter.IsValid())
	{
		TB_DailyEnter->SetText(FText::FromString(FString::FromInt(Count) + "/" + FString::FromInt(MaxCount)));
		TB_DailyEnter->SetColorAndOpacity(Count == 0 ? UB2UIManager::GetInstance()->TextColor_Decrease : UB2UIManager::GetInstance()->TextColor_Increase);
	}
}

void UB2UICounterAttackSelectCharacter::SetMaxDifficulty()
{
	auto* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();
	int32 MaxDifficultyNumber = CounterDoc ? BladeIIGameImpl::GetClientDataStore().GetCounterDungeonTypeNumber(CounterDoc->GetOpenedDungeonSlot()) : 1;
	UIP_DungeonLevelofDifficulty->SetMaxDifficulty(MaxDifficultyNumber);
}

void UB2UICounterAttackSelectCharacter::OnChangedCurPCClass(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass)
{
	SetSelectCharacter();
}

void UB2UICounterAttackSelectCharacter::OnChangedTagPCClass(class UB2UIDocBase* Sender, int32 TagPCClass, int32 PrevTagPCClass)
{
	SetSelectCharacter();
}

void UB2UICounterAttackSelectCharacter::OnHighestUpdate(class UB2UIDocBase* Sender, int32 CurLv, int32 PreLv)
{
	auto& CounterData = BladeIIGameImpl::GetClientDataStore().GetCounterDungeonData();
	auto CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	for (auto& Ele : CounterData)
	{
		if (Ele.DungeonType == CounterDoc->GetOpenedDungeonSlot() &&
			Ele.DifficultyLevel == CurLv)
		{
			BestLevel = Ele.DifficultyLevel;
			BestLevelServerStageID = Ele.ServerStageId;
			break;
		}
	}

	auto* DocBattle = UB2UIDocHelper::GetDocBattle();

	if (!DocBattle)
		return;

	SweepButtonState(CurLv);
	SetSelectCharacter();
	OnClickEntry(IntToPCClass(DocBattle->GetCurPCClass()));
}

void UB2UICounterAttackSelectCharacter::ChangeDifficulty(int32 InDifficulty)
{
	auto CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	if (!CounterDoc)
		return;

	CounterDoc->SetNewDifficulty(InDifficulty);

	if (TB_BestScoreDesc.IsValid())
		TB_BestScoreDesc->SetVisibility(CounterDoc->GetHighestLevel() == InDifficulty ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	SetPreRewardList();

	auto& CounterData = BladeIIGameImpl::GetClientDataStore().GetCounterDungeonData();

	for (auto& Ele : CounterData)
	{
		if (Ele.DungeonType == CounterDoc->GetOpenedDungeonSlot())
		{
			if (Ele.DifficultyLevel == CounterDoc->GetHighestLevel())
			{
				BestLevel = Ele.DifficultyLevel;
				BestLevelServerStageID = Ele.ServerStageId;
			}

			if (Ele.DifficultyLevel == InDifficulty)
			{
				RefreshRecommendStats(Ele.RecommendCombat, Ele.RecommendAttack, Ele.RecommendDefense);
			}
		}
	}

	SweepButtonState(CounterDoc->GetHighestLevel());
}

void UB2UICounterAttackSelectCharacter::RefreshRecommendStats(int32 RecommandPower, int32 RecommendAttack, int32 RecommendDefense)
{
	if (TB_CombatPower.IsValid() && TB_RecommandPower.IsValid())
	{
		if (nCurrentCombatPower >= RecommandPower)
			TB_CombatPower->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Increase);
		else
			TB_CombatPower->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);

		TB_RecommandPower->SetText(FText::AsNumber(RecommandPower));
	}

	if (UIP_StageRecommendPower.IsValid())
	{
		UIP_StageRecommendPower->SetInfo(CurrentAttack, CurrentDefense, RecommendAttack, RecommendDefense);
	}
}

void UB2UICounterAttackSelectCharacter::OnClickEntry(EPCClass InEntry)
{
	if (InEntry == EPCClass::EPC_End)
		InEntry = EPCClass::EPC_Gladiator;

	auto* DocCounter = UB2UIDocHelper::GetDocCounterAttack();
	check(DocCounter);

	int32 Difficulty = DocCounter->GetNewDifficulty();
	if (Difficulty == 0)	// 0이면 1단계로 변경
	{
		if (DocCounter->GetHighestLevel() <= 1)
			Difficulty = 1;
		else
			Difficulty = DocCounter->GetHighestLevel();

		DocCounter->SetNewDifficulty(Difficulty);
	}

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		int32 ChangeMainClass = DocBattle->GetCurPCClass();
		int32 ChangeSubClass = DocBattle->GetTagPCClass();

		if (ChangeSubClass == PCClassToInt(InEntry))
		{
			DocBattle->SetTagPCClass(ChangeMainClass);
		}

		DocBattle->SetCurPCClass(PCClassToInt(InEntry));
	}

	if (auto* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(InEntry)))
	{
		nCurrentCombatPower = DocHero->GetCombatPower();
	}

	CurrentAttack = CombatStatEval::GetPCAttack(InEntry);
	CurrentDefense = CombatStatEval::GetPCDefense(InEntry);

	if (TB_CombatPower.IsValid())
	{
		TB_CombatPower->SetText(FText::AsNumber(nCurrentCombatPower));
	}

	if (UIP_DungeonLevelofDifficulty.IsValid())
		UIP_DungeonLevelofDifficulty->SetCurrentDifficulty(Difficulty);

	for (int32 i = 0; i < PCClassToInt(EPCClass::EPC_End); ++i)
	{
		UIP_EntryArr[i]->SetVisibilityWidgets(IntToPCClass(i) == InEntry ? EEntryType::EntryType_MainChar : EEntryType::EntryType_None);
	}
}

void UB2UICounterAttackSelectCharacter::SweepButtonState(int32 InHighest)
{
	if (BTN_Sweep.IsValid() && TB_SweepButton.IsValid())
	{
		BTN_Sweep->SetIsEnabled(InHighest ? true : false);

		FText ButtonText = InHighest ?
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_AutoClear_Button")), FText::AsNumber(InHighest)) :
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_AutoClear_Title"));

		TB_SweepButton->SetText(ButtonText);
	}
}

bool UB2UICounterAttackSelectCharacter::CheckBuyTicket()
{
	// true면  티켓이 있다 false면 없다요
	auto* DocCounterAttack = UB2UIDocHelper::GetDocCounterAttack();
	check(DocCounterAttack);
	if (DocCounterAttack->GetLeftTicket() == 0)
	{
		if (DocCounterAttack->GetNextBuyTicketGemCost() == INDEX_NONE)
		{
			// 더이상 코스트로 살 수 없을경우
			UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_CompleteNotice")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm);

			return false;
		}

		UB2UIMsgPopupUseResources* MsgUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupUseResources>(EUIMsgPopup::UseGem,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_BuyTicketTitleMsg")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([this]() {
			SendBuyTicket();
		})
			);

		if (MsgUI)
		{
			MsgUI->SetCost(DocCounterAttack->GetNextBuyTicketGemCost());
			MsgUI->SetSubContent(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_BuyTicketSubMsg")));
		}
		return false;
	}

	return true;
}

void UB2UICounterAttackSelectCharacter::SendBuyTicket()
{
	auto* DocCounterAttack = UB2UIDocHelper::GetDocCounterAttack();
	auto* DocUser = UB2UIDocHelper::GetDocUser();

	if (!DocCounterAttack)
		return;
	if (!DocUser)
		return;

	if (DocCounterAttack->GetNextBuyTicketGemCost() > DocUser->GetGem())
	{
		ShortageMGR->PopupGoToShopForGemMessage();
	}
	else
	{
		data_trader::Retailer::GetInstance().RequestBuyTicketCounterDungeon();
	}
}

void UB2UICounterAttackSelectCharacter::OnOpen(bool RightNow /*= false*/)
{
	//Super::OnOpen(RightNow);

	//auto* DocCounterAttack = UB2UIDocHelper::GetDocCounterAttack();
	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();

	//if (!DocCounterAttack)
	//	return;
	//if (!DocBattle)
	//	return;

	//DeliveryBuyTicket = DeliveryBuyTicketCounterDungeonClass<FB2CounterDungeonBuyTicket>::GetInstance().Subscribe([this](const FB2CounterDungeonBuyTicket& TicketInfo) {
	//	auto* DDoc = UB2UIDocHelper::GetDocCounterAttack();

	//	int32 currentgem = GET_TUPLE_DATA(FB2ResponseCounterDungeonBuyTicket::current_gem_index, TicketInfo);
	//	BladeIIGameImpl::GetClientDataStore().SetGemAmount(currentgem);

	//	int32 LeftTicket = GET_TUPLE_DATA(FB2ResponseCounterDungeonBuyTicket::left_ticket_index, TicketInfo);

	//	DDoc->SetLeftTicket(LeftTicket);
	//	SetEnterCount(DDoc->GetLeftTicket(), BladeIIGameImpl::GetClientDataStore().GetMaxCounterDungeonTicketCount());
	//});

	//OnClickEntry(IntToPCClass(DocBattle->GetCurPCClass()));
	//SetEnterCount(DocCounterAttack->GetLeftTicket(), BladeIIGameImpl::GetClientDataStore().GetMaxCounterDungeonTicketCount());
}

void UB2UICounterAttackSelectCharacter::OnClickBtnGoToHeroMgmt()
{
	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(IntToPCClass(DocBattle->GetCurPCClass()));

	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UICounterAttackSelectCharacter::OnClickSweepBattle()
{
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_CounterDungeon) && CheckContentsModeState(b2network::B2ContentsMode::COUNTER))
		return;

	if (!SetArinCheck())
		return;

	UB2UIMsgPopupHeroTowerSweep* pSweepPopup = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupHeroTowerSweep>(UIFName::CounterDungeonSweepPopup);
	if (!pSweepPopup)
		return;

	int32 nClearFloor = UB2UIDocHelper::GetDocHeroTower()->GetMaxClearFloor();

	// GetSweepItems() 가 서버로 부터 받는 아이템과 총합을 구해줌 2개 이상의 보상일경우 아래 SetAddComment를 추가로 구현
	pSweepPopup->SetAddCommentForHeroTower(GetSweepItems());

	pSweepPopup->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sweep")));
	pSweepPopup->SetContent(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTowerDoYouSweepNFloor?")), FText::AsNumber(nClearFloor)));
	pSweepPopup->SetTipText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_AutoClearDescription")));
	pSweepPopup->SetCounterDungeonTip(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_Guide_3")), FText::AsNumber(BestLevel)));
	pSweepPopup->SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("HeroTower_AutoClear"))));

	pSweepPopup->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this]() { this->OnCounterDungeonSweep(); UB2UIManager::GetInstance()->CloseUI(UIFName::CounterDungeonSweepPopup); }));
	pSweepPopup->AddHandler(EUIMsgPopupButton::Negative, FMsgPopupOnClick::CreateLambda([this]() { UB2UIManager::GetInstance()->CloseUI(UIFName::CounterDungeonSweepPopup);   }));
}

#include "B2UIArinConsult_PreCombat.h"
bool UB2UICounterAttackSelectCharacter::CheckAndInvokeArinConsulting()
{
	// 스테이지 시작 버튼 눌렀을 때 불리는 거 가정.

	// 일정 횟수 이전에 무시했던 경우. 이걸 기본적인 Arin 의 조언 상태 체크 이전에 실행해서 스킵 횟수 차감이 매번 실행되도록 함.
	// Arin 의 조언이 발동하는 상황에서만 이걸 검사할 경우 자칫 너무 오래 Arin 의 조언을 못 보게 될 수도 있다.
	UB2UIDocUICondition* DocCondition = UB2UIDocHelper::GetDocUICondition();
	if (DocCondition && DocCondition->ShouldIgnorePreCombatArinConsulting())
	{ // 하나 차감하고 이번엔 스킵
		DocCondition->SetPreCombatArinConsultIgnoreCount(DocCondition->GetPreCombatArinConsultIgnoreCount() - 1);
		return false;
	}

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	// ArinConsultingSubject 는 이 페이지 들어올때 내지는 영웅관리나 캐릭터 선택하면서 세팅되어야 함.
	EArinConsultPreCombat_MainSubject ArinConsultSubject = DocBS ? DocBS->GetArinConsultingSubject() : EArinConsultPreCombat_MainSubject::ARCPCMS_None;

	if (ArinConsultSubject != EArinConsultPreCombat_MainSubject::ARCPCMS_None)
	{
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			auto* ArinUI = UIMgrInst->OpenUI<UB2UIArinConsult_PreCombat>(UIFName::ArinConsult_PreCombat);
			if (ArinUI)
				ArinUI->SetFromCounterDungeon();
			return true;
		}
	}
	return false;
}

bool UB2UICounterAttackSelectCharacter::SetArinCheck()
{
	if (!CheckBuyTicket())
		return false;

	auto* DocCounterDG = UB2UIDocHelper::GetDocCounterAttack();
	if (DocCounterDG == nullptr)
		return false;

	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	auto& CounterDGDataArray = ClientDataStore.GetCounterDungeonData();

	for (auto& CounterDGData : CounterDGDataArray)
	{
		if (DocCounterDG->GetOpenedDungeonType().Num() > 0 &&
			CounterDGData.DifficultyLevel == DocCounterDG->GetNewDifficulty())
		{
			for (auto& Ele : DocCounterDG->GetOpenedDungeonType())
			{
				if (Ele == DocCounterDG->GetOpenedDungeonSlot() &&
					CounterDGData.DungeonType == DocCounterDG->GetOpenedDungeonSlot())
				{
					UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
					if (DocBS)
					{//Arin 조언을 위한 데이터 세팅
						DocBS->SetClientStageId(StageDataStore.GetClientStageId(CounterDGData.ServerStageId));
						DocBS->SetStageDifficulty(StageDataStore.GetStageDifficulty(CounterDGData.ServerStageId));
						DocBS->SetServerStageId(CounterDGData.ServerStageId);
					}
				}
			}
		}
	}
	return true;
}

void UB2UICounterAttackSelectCharacter::StartBattle()
{
	// 점검 정산 체크
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_CounterDungeon) && CheckContentsModeState(b2network::B2ContentsMode::COUNTER))
		return;

	if (!SetArinCheck())
		return;

	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();

	if (!DocBattle)
		return;
	if (!DocBattleStage)
		return;

	auto& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
	StageData.RequestStartCounterDungeon(DocBattleStage->GetServerStageId(), IntToPCClass(DocBattle->GetCurPCClass()), IntToPCClass(DocBattle->GetTagPCClass()));
}

void UB2UICounterAttackSelectCharacter::OnCounterDungeonSweep()
{
	auto* DocBattle = UB2UIDocHelper::GetDocBattle();

	if (!DocBattle)
		return;
	int32 MainSvrClass = CliToSvrPCClassType(IntToPCClass(DocBattle->GetCurPCClass()));

	data_trader::Retailer::GetInstance().RequestSweepCounterDungeon(BestLevelServerStageID, MainSvrClass);
}

const TMap<int, FRewardItem>& UB2UICounterAttackSelectCharacter::GetSweepItems()
{
	int32 ClearFloor = UB2UIDocHelper::GetDocHeroTower()->GetMaxClearFloor();
	SweepItems.Empty();

	auto* DocCounter = UB2UIDocHelper::GetDocCounterAttack();

	auto& ArrData = BladeIIGameImpl::GetClientDataStore().GetCounterDungeonData();

	for (auto& Ele : ArrData)		//3중 포문 ㅠㅠ 마스터데이터 던전정보 찾고 내가갈수있는던전 정보 찾고 비교... 그러고 보상정보 for문.. 
	{
		if (Ele.DifficultyLevel == BestLevel &&
			DocCounter->GetOpenedDungeonType().Num() > 0)
		{
			for (auto& Ele2 : DocCounter->GetOpenedDungeonType())
			{
				if (Ele2 == DocCounter->GetOpenedDungeonSlot() &&
					Ele.DungeonType == Ele2)
				{
					for (auto rewarditem : Ele.RewardId)
					{
						SweepItems.Add(rewarditem.Id, rewarditem);
						SweepItems[rewarditem.Id].Count = rewarditem.Count;
						SweepItems[rewarditem.Id].MaxCount = rewarditem.MaxCount;
						//AddPreRewardListItem(rewarditem.Id, rewarditem.Count, rewarditem.MaxCount);
					}
				}
			}
		}
	}

	return SweepItems;
}

void UB2UICounterAttackSelectCharacter::AckCounterrSweep(const FB2SweepCounterDungeonInfo& MsgPtr)
{
	//OpenSweepResultPopup(MsgPtr);

	//auto CountDoc = UB2UIDocHelper::GetDocCounterAttack();

	//if (!CountDoc)
	//	return;

	//int32 LeftTiket = GET_TUPLE_DATA(FB2ResponseSweepCounterDungeon::left_ticket_index, MsgPtr);
	//int32 NextGemcost = GET_TUPLE_DATA(FB2ResponseSweepCounterDungeon::next_buy_ticket_gem_cost_index, MsgPtr);

	//CountDoc->SetNextBuyTicketGemCost(NextGemcost);
	//CountDoc->SetLeftTicket(LeftTiket);

	//SetEnterCount(LeftTiket, BladeIIGameImpl::GetClientDataStore().GetMaxCounterDungeonTicketCount());
}

void UB2UICounterAttackSelectCharacter::OpenSweepResultPopup(const FB2SweepCounterDungeonInfo& MsgPtr)
{
	//const TArray<b2network::B2RewardPtr>& RewardInfo = GET_TUPLE_DATA(FB2ResponseSweepCounterDungeon::received_rewards_index, MsgPtr);

	//UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	//if (RewardPopup != nullptr)
	//{
	//	FText TitleText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("heroTower_AutoClearSuccess"));
	//	RewardPopup->InitRewardMsgPopupText(TitleText, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Common_RewardedDesc")));
	//	RewardPopup->UpdateDescriptionText(FText::GetEmpty());

	//	for (auto& RewardInfoSlot : RewardInfo)
	//	{

	//		int32 itemid = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(RewardInfoSlot->raw_reward->id);
	//		RewardPopup->AddSingleRewardItem(itemid, RewardInfoSlot->raw_reward->count);

	//		BladeIIGameImpl::GetClientDataStore().ApplyRewardInfo(RewardInfoSlot);
	//	}

	//	RewardPopup->ShowRewardPopup(true);
	//}

	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();

	//if (!DocBattle)
	//	return;
	//SetSelectCharacter();
	//OnClickEntry(IntToPCClass(DocBattle->GetCurPCClass()));
	////m_nTempHeroPieceAmount = -1;
}

void UB2UICounterAttackSelectCharacter::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UICounterAttackSelectCharacter);

	DeliverySweepCounterTicket = DeliverySweepCounterDungeonClass<FB2SweepCounterDungeonInfo>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliverySweepCounter, const FB2SweepCounterDungeonInfo& MsgPtr)
		Capture->AckCounterrSweep(MsgPtr);
	END_CAPTURE_OBJECT()
		);
}

void UB2UICounterAttackSelectCharacter::UnsubscribeEvent()
{
	DeliverySweepCounterDungeonClass<FB2SweepCounterDungeonInfo>::GetInstance().Unsubscribe(DeliverySweepCounterTicket);
}

void UB2UICounterAttackSelectCharacter::SetPreRewardList()
{
	auto* DocCounter = UB2UIDocHelper::GetDocCounterAttack();
	if (!DocCounter)
		return;
	if (!List_Reward.IsValid())
		return;

	List_Reward->ClearChildren();

	auto& ArrData = BladeIIGameImpl::GetClientDataStore().GetCounterDungeonData();

	for (auto& Ele : ArrData)		//3중 포문 ㅠㅠ 마스터데이터 던전정보 찾고 내가갈수있는던전 정보 찾고 비교... 그러고 보상정보 for문.. 
	{
		if (Ele.DifficultyLevel == DocCounter->GetNewDifficulty() &&
			DocCounter->GetOpenedDungeonType().Num() > 0)
		{
			for (auto& Ele2 : DocCounter->GetOpenedDungeonType())
			{
				if (Ele2 == DocCounter->GetOpenedDungeonSlot() &&
					Ele.DungeonType == Ele2)
				{
					for (auto rewarditem : Ele.RewardId)
					{
						AddPreRewardListItem(rewarditem.Id, rewarditem.Count, rewarditem.MaxCount);
					}
				}
			}
		}
	}
}

void UB2UICounterAttackSelectCharacter::AddPreRewardListItem(int32 rewardid, int32 rewardcount, int32 rewardcountMax)
{
	auto* CreatedWidget = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardInfoClass);
	if (CreatedWidget != NULL)
	{
		FText IconText;

		if (rewardcount != rewardcountMax)
			IconText = FText::FromString(FString::Printf(TEXT("%d ~ %d"), rewardcount, rewardcountMax));
		else
			IconText = FText::FromString(FString::Printf(TEXT("%d"), rewardcountMax));

		CreatedWidget->Init();
		CreatedWidget->UpdateRewardIcon(rewardid, IconText);
		List_Reward->AddChild(CreatedWidget);
	}
}

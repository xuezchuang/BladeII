#include "B2UITeamMatchCharSelect.h"
#include "B2UITeamEntrySelectSlot.h"
#include "B2UIWidget.h"
#include "B2UIHeader.h"
#include "B2UIManager.h"
#include "B2UITMFormation.h"
#include "B2UIDocHelper.h"
#include "B2UICharacterSelectItem.h"

#include "B2UISlotItem.h"
#include "B2ItemInfo.h"

#include "Retailer.h"
#include "B2UIFormationSelectButton.h"
#include "B2UIMsgPopupUseResources.h"
#include "B2UIShortagePopupManager.h"

#include "B2UIFormationPopup.h"
#include "TutorialManager.h"

#include "BladeIIGameImpl.h"

void UB2UITeamMatchCharSelect::Init()
{
	Super::Init();
	
	UB2UIDocTeamMatch* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (TeamDoc)
	{
		OldFormation.FormationType = TeamDoc->GetTMFormation();
		OldFormation.Slot1Char = TeamDoc->GetCharacterClass(true, 0);
		OldFormation.Slot2Char = TeamDoc->GetCharacterClass(true, 1);
		OldFormation.Slot3Char = TeamDoc->GetCharacterClass(true, 2);
	}	

	IsHeroSelect = false;

	UpdateFormationInfo(TeamDoc->GetTMFormation());
	UpdateFormationCharacterSlot();
	CharacterSlotBaseSetting();
	FormationInfo->SetStartPos(TeamDoc->GetTMFormation());

	TeamDoc->OnFormationAttLevelChanged.AddUObject(this, &UB2UITeamMatchCharSelect::OnChangedATKLevel);
	TeamDoc->OnFormationDefLevelChanged.AddUObject(this, &UB2UITeamMatchCharSelect::OnChangedDEFLevel);
	TeamDoc->OnFormationNormalLevelChanged.AddUObject(this, &UB2UITeamMatchCharSelect::OnChangedNORLevel);
}

void UB2UITeamMatchCharSelect::BindDocAuto()
{
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_TeamMatch_DuelReady"));
	
	/*auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (TeamMatchDoc)
	{
		TeamMatchDoc->OnTMFormationChanged.AddUObject(this, &UB2UITeamMatchCharSelect::OnChangedFormationType);
	}	*/	
}

void UB2UITeamMatchCharSelect::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_TeamMatch_DuelReady"));
}

void UB2UITeamMatchCharSelect::UnbindDoc()
{
	auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (TeamMatchDoc)
	{
		TeamMatchDoc->OnTMFormationChanged.RemoveAll(this);
	}

	TeamMatchDoc->OnFormationAttLevelChanged.RemoveAll(this);
	TeamMatchDoc->OnFormationDefLevelChanged.RemoveAll(this);
	TeamMatchDoc->OnFormationNormalLevelChanged.RemoveAll(this);
}

void UB2UITeamMatchCharSelect::UpdateStaticText()
{
	Super::UpdateStaticText();
	
	if (TB_TMFormation.IsValid())
		TB_TMFormation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_Formation")));

	if (TB_BattleStart.IsValid())
		TB_BattleStart->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_BattleStart")));

	if (TB_FormationManager.IsValid())
		TB_FormationManager->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationManager")));


}

void UB2UITeamMatchCharSelect::UpdateFormationCharacterSlot()
{
	FormationInfo->UIDocSetCharacter();
}

void UB2UITeamMatchCharSelect::OnClickBtnHeroSlot(class UB2UICharacterSelectItem* SelectHero)
{
	if (!IsHeroSelect)
		return;

	if (SelectHero->GetSelecteMode() != EUIHeroSelectModeType::SelectTargetMain)
		FormationInfo->SetSlotCharacter(NewFormationClass);
	else
	{
		UB2UIDocTeamMatch* TeamUIDoc = UB2UIDocHelper::GetDocTeamMatch();

		if (TeamUIDoc == nullptr)
			return;
		
		////다른슬롯과 교환될때 검사
		if (TeamUIDoc->GetPCCharacterClass1() == SelectHero->GetCharacterClass())
		{
			TeamUIDoc->SetPCCharacterClass1(NewFormationClass);
		}
		else if (TeamUIDoc->GetPCCharacterClass2() == SelectHero->GetCharacterClass())
		{
			TeamUIDoc->SetPCCharacterClass2(NewFormationClass);
		}
		else if (TeamUIDoc->GetPCCharacterClass3() == SelectHero->GetCharacterClass())
		{
			TeamUIDoc->SetPCCharacterClass3(NewFormationClass);
		}

		if (TeamUIDoc->GetPCCharacterClass1() == EPCClass::EPC_End)
		{
			TeamUIDoc->SetPCCharacterClass1(SelectHero->GetCharacterClass());
		}
		else if (TeamUIDoc->GetPCCharacterClass2() == EPCClass::EPC_End)
		{
			TeamUIDoc->SetPCCharacterClass2(SelectHero->GetCharacterClass());
		}
		else if (TeamUIDoc->GetPCCharacterClass3() == EPCClass::EPC_End)
		{
			TeamUIDoc->SetPCCharacterClass3(SelectHero->GetCharacterClass());
		}

		PlayHeroSlotClickSound();
		DevShowNotReadyYetMessage(FString(TEXT("ChangeCharacter")));

	}

	CharacterSlotModSetting();
	FormationInfo->UIDocSetCharacter();

	IsHeroSelect = false;
}

void UB2UITeamMatchCharSelect::OnClickBtnFormaitionSlot(EPCClass PCClass)
{
	if (IsHeroSelect)
	{
		FormationInfo->SetSlotCharacter(NewFormationClass);
		CharacterSlotModSetting();
		IsHeroSelect = false;
	}
	else
	{
		for (int32 CntClass = 0; CntClass < GetMaxPCClassNum(); ++CntClass)
		{
			CntClass == PCClassToInt(PCClass) ? CharacterSlotArr[CntClass]->SetSelectMode(EUIHeroSelectModeType::SelectOwnerMain) : CharacterSlotArr[CntClass]->SetSelectMode(EUIHeroSelectModeType::SelectTargetMain);
		}

		IsHeroSelect = true;

		PlayFormationSlotClickSound();
	}

	NewFormationClass = PCClass;
}

void UB2UITeamMatchCharSelect::FormationPlayAnimation(ETMFormation Formation)
{
	UB2UIDocTeamMatch* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (!TeamDoc)
		return;

	if (Formation == TeamDoc->GetTMFormation())
		return;

	if (FormationInfo->IsChangeAnimation())
		return;

	DevShowNotReadyYetMessage(FString(TEXT("ChangeFormation")));

	ETMFormation PreFormation = TeamDoc->GetTMFormation();
	TeamDoc->SetTMFormation(Formation);
	FormationInfo->SetPlayAnimation(PreFormation, Formation);
	UpdateFormationInfo(TeamDoc->GetTMFormation());

	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass1())]->SetFormationNum(TeamDoc->GetFirstCharcterForamtionNUM());
	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass2())]->SetFormationNum(TeamDoc->GetSecondCharcterForamtionNUM());
	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass3())]->SetFormationNum(TeamDoc->GetThirdCharcterForamtionNUM());
}

void UB2UITeamMatchCharSelect::DelegateEnhanceButtonClick(ETMFormation Formation, int32 Level)
{
	UB2UIManager* UIManager = UB2UIManager::GetInstance();
	if (UIManager != NULL)
	{
		UB2UIFormationPopup* EnhacePopup = UIManager->OpenUI<UB2UIFormationPopup>(UIFName::FormationPopup);
		if (EnhacePopup)
		{
			EnhacePopup->SetData(Formation, Level);
		}
	}
}

void UB2UITeamMatchCharSelect::OnChangedATKLevel(UB2UIDocBase* Doc, int32 CurrLv, int32 PreLv)
{
	TArray<float> AttackBonus;
	TArray<float> DefenseBonus;
	
	GetAbility(ETMFormation::ETM_Attack, CurrLv, AttackBonus, DefenseBonus);

	SelectFormationButtonArr[static_cast<int32>(ETMFormation::ETM_Attack)]->SetFormation(ETMFormation::ETM_Attack, CurrLv, AttackBonus, DefenseBonus);
}

void UB2UITeamMatchCharSelect::OnChangedDEFLevel(UB2UIDocBase* Doc, int32 CurrLv, int32 PreLv)
{
	TArray<float> AttackBonus;
	TArray<float> DefenseBonus;

	GetAbility(ETMFormation::ETM_Defense, CurrLv, AttackBonus, DefenseBonus);

	SelectFormationButtonArr[static_cast<int32>(ETMFormation::ETM_Defense)]->SetFormation(ETMFormation::ETM_Defense, CurrLv, AttackBonus, DefenseBonus);
}

void UB2UITeamMatchCharSelect::OnChangedNORLevel(UB2UIDocBase* Doc, int32 CurrLv, int32 PreLv)
{
	TArray<float> AttackBonus;
	TArray<float> DefenseBonus;

	GetAbility(ETMFormation::ETM_Normal, CurrLv, AttackBonus, DefenseBonus);

	SelectFormationButtonArr[static_cast<int32>(ETMFormation::ETM_Normal)]->SetFormation(ETMFormation::ETM_Normal, CurrLv, AttackBonus, DefenseBonus);
}

void UB2UITeamMatchCharSelect::SetTotalHerostat(UB2UIDocTeamMatch* TeamDoc)
{
	int32 AttPoint = 0;
	int32 DefPoint = 0;
	int32 CombatPower = 0;

	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		auto* HeroDoc = UB2UIDocHelper::GetDocHero(i);

		if (!HeroDoc)
			break;

		if (i == PCClassToInt(TeamDoc->GetPCCharacterClass1()) ||
			i == PCClassToInt(TeamDoc->GetPCCharacterClass2()) ||
			i == PCClassToInt(TeamDoc->GetPCCharacterClass3()))
		{
			CombatPower += HeroDoc->GetCombatPower();
		}
	}

	if (TB_CombatPower.IsValid())
	{
		TB_CombatPower->SetText(FText::AsNumber(CombatPower));
	}
}


void UB2UITeamMatchCharSelect::OnClickTeamMatchStart()
{
	// 점검 정산 체크
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPTeam) && CheckContentsModeState(b2network::B2ContentsMode::PVP_TEAM))
		return;

	if (!CheckBuyTicket()) return;
	OnStartGame(false);
}

void UB2UITeamMatchCharSelect::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_TMFormation);
	GET_SLOT(UTextBlock, TB_FormationManager);

	GET_SLOT(UB2UITMCharacterSelectItem, GladiatorSlot);
	GET_SLOT(UB2UITMCharacterSelectItem, AssassinSlot);
	GET_SLOT(UB2UITMCharacterSelectItem, MageSlot);
	GET_SLOT(UB2UITMCharacterSelectItem, FighterSlot);

	GET_SLOT(UTextBlock, TB_BattleStart);
	GET_SLOT(UTextBlock, TB_CombatPower);

	GET_SLOT(UB2UITMFormation, FormationInfo);

	SelectFormationButtonArr.SetNum(3);

	GET_SLOT_BYNAME(UB2UITMFormationSelectButton, SelectFormationButtonArr[static_cast<int32>(ETMFormation::ETM_Normal)], UIP_TMFormationSelectButton_0);
	GET_SLOT_BYNAME(UB2UITMFormationSelectButton, SelectFormationButtonArr[static_cast<int32>(ETMFormation::ETM_Attack)], UIP_TMFormationSelectButton_1);
	GET_SLOT_BYNAME(UB2UITMFormationSelectButton, SelectFormationButtonArr[static_cast<int32>(ETMFormation::ETM_Defense)], UIP_TMFormationSelectButton_2);

	SetFormationButtonSlot();

	GET_SLOT(UB2Button, BTN_Upgrade);
}

void UB2UITeamMatchCharSelect::BindDelegates()
{
	if (GladiatorSlot.IsValid())
	{
		GladiatorSlot->Init();
		GladiatorSlot->OnCharacterSelectItemClickedDelgate.BindDynamic(this, &UB2UITeamMatchCharSelect::OnClickBtnHeroSlot);
	}


	if (AssassinSlot.IsValid())
	{
		AssassinSlot->Init();
		AssassinSlot->OnCharacterSelectItemClickedDelgate.BindDynamic(this, &UB2UITeamMatchCharSelect::OnClickBtnHeroSlot);
	}
	
	if (MageSlot.IsValid())
	{
		MageSlot->Init();
		MageSlot->OnCharacterSelectItemClickedDelgate.BindDynamic(this, &UB2UITeamMatchCharSelect::OnClickBtnHeroSlot);
	}



	if (FighterSlot.IsValid())
	{
		FighterSlot->Init();
		FighterSlot->OnCharacterSelectItemClickedDelgate.BindDynamic(this, &UB2UITeamMatchCharSelect::OnClickBtnHeroSlot);
	}


	if (FormationInfo.IsValid())
	{
		FormationInfo->Init();
		FormationInfo->CharacterSlotUpdateDelegate.BindUObject(this, &UB2UITeamMatchCharSelect::OnClickBtnFormaitionSlot);
	}


}

void UB2UITeamMatchCharSelect::OnClose(bool RightNow)
{
	if (IsHeroSelect)
		FormationInfo->CancelFormation();

	CharacterSlotArr[PCClassToInt(EPCClass::EPC_Gladiator)] = GladiatorSlot.Get();
	CharacterSlotArr[PCClassToInt(EPCClass::EPC_Assassin)] = AssassinSlot.Get();
	CharacterSlotArr[PCClassToInt(EPCClass::EPC_Wizard)] = MageSlot.Get();
	CharacterSlotArr[PCClassToInt(EPCClass::EPC_Fighter)] = FighterSlot.Get();

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		CharacterSlotArr[i]->BindDoc(UB2UIDocHelper::GetDocHero(i));
		CharacterSlotArr[i]->SetSelectedType(EUIHeroSelectType::None);
	}
	
	DeliveryEnhanceTeamFormationClass<FB2EnhanceTeamFormation>::GetInstance().Unsubscribe(DeliveryEnhanceFormationTicket);

	Super::OnClose(RightNow);
}

void UB2UITeamMatchCharSelect::SetFormationButtonSlot()
{
	UB2UIDocTeamMatch* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (!TeamDoc)
		return;

	auto& DataStore = BladeIIGameImpl::GetClientDataStore();

	int32 Lv = 0; 
	FFormationAnimationDelegate AniDelegate;
	AniDelegate.BindUObject(this, &UB2UITeamMatchCharSelect::FormationPlayAnimation);
	
	FFormationClickDelegate ClickDelegate;
	ClickDelegate.BindUObject(this, &UB2UITeamMatchCharSelect::DelegateEnhanceButtonClick);

	for (int32 i = 0; i < static_cast<int32>(ETMFormation::ETM_End); ++i)
	{
		if (static_cast<ETMFormation>(i) == ETMFormation::ETM_Normal)
			Lv = TeamDoc->GetFormationNormalLevel();

		if (static_cast<ETMFormation>(i) == ETMFormation::ETM_Attack)
			Lv = TeamDoc->GetFormationAttLevel();

		if (static_cast<ETMFormation>(i) == ETMFormation::ETM_Defense)
			Lv = TeamDoc->GetFormationDefLevel();

		TArray<float> AttackBonus;
		TArray<float> DefenseBonus;

		GetAbility(static_cast<ETMFormation>(i), Lv, AttackBonus, DefenseBonus);

		if(SelectFormationButtonArr[i].IsValid())
		{
			SelectFormationButtonArr[i]->Init();
			SelectFormationButtonArr[i]->SetFormation(static_cast<ETMFormation>(i), Lv, AttackBonus, DefenseBonus);
			SelectFormationButtonArr[i]->SetDelegate(AniDelegate, ClickDelegate);
		}
	}
}

void UB2UITeamMatchCharSelect::CharacterSlotBaseSetting()
{
	CharacterSlotArr[PCClassToInt(EPCClass::EPC_Gladiator)] = GladiatorSlot.Get();
	CharacterSlotArr[PCClassToInt(EPCClass::EPC_Assassin)] = AssassinSlot.Get();
	CharacterSlotArr[PCClassToInt(EPCClass::EPC_Wizard)] = MageSlot.Get();
	CharacterSlotArr[PCClassToInt(EPCClass::EPC_Fighter)] = FighterSlot.Get();

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		CharacterSlotArr[i]->BindDoc(UB2UIDocHelper::GetDocHero(i));
		CharacterSlotArr[i]->SetSelectedType(EUIHeroSelectType::None);
	}

	CharacterSlotModSetting();
}


void UB2UITeamMatchCharSelect::CharacterSlotModSetting()
{
	for (int32 CntClass = 0; CntClass < GetMaxPCClassNum(); ++CntClass)
	{
		CharacterSlotArr[CntClass]->SetSelectMode(EUIHeroSelectModeType::None);
		CharacterSlotArr[CntClass]->SetSelectedType(EUIHeroSelectType::None);
	}

	UB2UIDocTeamMatch* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();
	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass1())]->SetSelectedType(EUIHeroSelectType::Mod);
	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass2())]->SetSelectedType(EUIHeroSelectType::Mod);
	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass3())]->SetSelectedType(EUIHeroSelectType::Mod);
	
	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass1())]->SetFormationNum(TeamDoc->GetFirstCharcterForamtionNUM());
	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass2())]->SetFormationNum(TeamDoc->GetSecondCharcterForamtionNUM());
	CharacterSlotArr[PCClassToInt(TeamDoc->GetPCCharacterClass3())]->SetFormationNum(TeamDoc->GetThirdCharcterForamtionNUM());

	SetTotalHerostat(TeamDoc);
}

void UB2UITeamMatchCharSelect::UpdateFormationInfo(ETMFormation TMFormaiton)
{
	for (int32 i = 0; i < static_cast<int32>(ETMFormation::ETM_End); ++i)
	{
		if (TMFormaiton == static_cast<ETMFormation>(i))
			SelectFormationButtonArr[i]->SetOnSlotState();
		else
			SelectFormationButtonArr[i]->SetOffSlotState();
	}
}

bool UB2UITeamMatchCharSelect::CheckBuyTicket()
{
	//true면  티켓이 있다 false면 없다요

	auto* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	check(TeamDoc)

	if (TeamDoc->GetTeamMatchPoint() == 0)
	{
		UB2UIMsgPopupUseResources* MsgUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupUseResources>(EUIMsgPopup::UseGem,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TeamMatch_BuyTicketTitleMsg")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([this]() { CheckGemForBuyTicket(); }));

		if (MsgUI)
		{
			MsgUI->SetCost(BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(TeamDoc->GetTeamAdditionalMatchCount()));
			MsgUI->SetSubContent(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_BuyTicketSubMsg")));
		}
		return false;
	}

	return true;
}

void UB2UITeamMatchCharSelect::CheckGemForBuyTicket()
{
	auto* TeamBattleDoc = UB2UIDocHelper::GetDocTeamMatch();
	int32 AdditonalCount = TeamBattleDoc->GetTeamAdditionalMatchCount();

	if (BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(AdditonalCount) > BladeIIGameImpl::GetClientDataStore().GetGemAmount()) // 잼부족
		ShortageMGR->PopupGoToShopForGemMessage();
	else
		OnStartGame(true);
}

void UB2UITeamMatchCharSelect::OnStartGame(bool bUsedAdditional)
{
	if (IsHeroSelect)
		FormationInfo->CancelFormation();

	bool bDirty = false;

	UB2UIDocTeamMatch* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (TeamDoc)
	{
		bDirty |= OldFormation.FormationType != TeamDoc->GetTMFormation();
		bDirty |= OldFormation.Slot1Char != TeamDoc->GetCharacterClass(true, 0);
		bDirty |= OldFormation.Slot2Char != TeamDoc->GetCharacterClass(true, 1);
		bDirty |= OldFormation.Slot3Char != TeamDoc->GetCharacterClass(true, 2);
		TeamDoc->SetUsedAdditionalPoint(bUsedAdditional);
	}
	else
		bDirty = true;

	if (bDirty)
	{
		UB2UIDocTeamMatch* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
		if (TeamMatchDoc)
		{
			data_trader::Retailer::GetInstance().RequestUpdateTeamBattleEntry(
				TeamMatchDoc->GetTMFormation(),
				TeamMatchDoc->GetCharacterClass(true, 0),
				TeamMatchDoc->GetCharacterClass(true, 1),
				TeamMatchDoc->GetCharacterClass(true, 2)
			);

			TeamMatchDoc->SetUsedAdditionalPoint(bUsedAdditional);
		}
	}

	StartBladeIITeamMatchGame(Cast<AGameMode>(GetWorld()->GetAuthGameMode()));
}

void UB2UITeamMatchCharSelect::GetAbility(ETMFormation Formation, int32 Level, TArray<float>& ATKBonus, TArray<float>& DEFBonus)
{
	auto& DataStore = BladeIIGameImpl::GetClientDataStore();
	for (int idx = 0; idx < 3; idx++)
	{
		ATKBonus.Add(DataStore.GetAttackDamageBonusByTeamFormation(Formation, Level, idx + 1));
		DEFBonus.Add(DataStore.GetReduceDamageBonusByTeamFormation(Formation, Level, idx + 1));
	}
}


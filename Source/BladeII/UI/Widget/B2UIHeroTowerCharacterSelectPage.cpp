// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIHeroTowerCharacterSelectPage.h"
#include "B2UIStartButton.h"
#include "B2UIManager.h"
#include "B2LobbyGameMode.h"
#include "B2UIMsgPopupUseResources.h"
#include "B2UIMsgPopupSimpleAddComment.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHeroTower.h"
#include "B2StageManager.h"
#include "B2UIDocHero.h"
#include "B2UIRecommandPower.h"
#include "B2UICharacterSelectPage.h"

#include "TutorialManager.h"
#include "B2CombatStatEvaluator.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2UIFairyLinkButton.h"

void UB2UIHeroTowerCharacterSelectPage::CacheAssets()
{
	Super::CacheAssets();

	HeroArr.SetNum(GetMaxPCClassNum());

	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, HeroArr[PCClassToInt(EPCClass::EPC_Gladiator)], UIP_Gladiator);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, HeroArr[PCClassToInt(EPCClass::EPC_Assassin)], UIP_Assassin);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, HeroArr[PCClassToInt(EPCClass::EPC_Wizard)], UIP_Wizard);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, HeroArr[PCClassToInt(EPCClass::EPC_Fighter)], UIP_Fighter);
	for (auto El : HeroArr)
	{
		El->Init();
	}

	GET_SLOT(UTextBlock, STB_MainChar);
	GET_SLOT(UTextBlock, STB_SubChar);
	GET_SLOT(UTextBlock, STB_Title);
	GET_SLOT(UTextBlock, STB_Equip);
	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, STB_RecomPower);

	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, TB_RecomPower);

	GET_SLOT(UB2Image, IMG_EmptyMain);
	GET_SLOT(UB2Image, IMG_EmptySub);
	GET_SLOT(UVerticalBox, VB_MainCharacter);
	GET_SLOT(UVerticalBox, VB_SubCharacter);

	GET_SLOT(UB2Button, BTN_CharacterChange);
	GET_SLOT(UB2Button, BTN_MainHero);
	GET_SLOT(UB2Button, BTN_SubHero);

	GET_SLOT(UB2Button, BTN_EquipManagement);
	GET_SLOT(UB2UIRecommandPower, UIP_RecommendPower);
	if (UIP_RecommendPower.IsValid())
	{
		UIP_RecommendPower->Init();
	}
	GET_SLOT(UB2UIStartButton, UIP_StartBT);
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->Init();
		UIP_StartBT->ClickDirectDelegateBindLambda([this]() { PageBlock(true); });
		UIP_StartBT->StartButtonDelegateBindLambda([this]() { BattleStart(); });
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_BattleStart")));
	}

	GET_SLOT(UB2UIFairyLinkButton, UIP_FairyLinkButton);
	if (UIP_FairyLinkButton.IsValid())
		UIP_FairyLinkButton->Init();
}

void UB2UIHeroTowerCharacterSelectPage::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_EquipManagement, &UB2UIHeroTowerCharacterSelectPage::OnClickBtnEquipManagement);
	BIND_CLICK_FUNC_TO_BTN(BTN_MainHero, &UB2UIHeroTowerCharacterSelectPage::OnClickMainButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_SubHero, &UB2UIHeroTowerCharacterSelectPage::OnClickSubButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_CharacterChange, &UB2UIHeroTowerCharacterSelectPage::OnClickBtnChangeMainSub);
}

void UB2UIHeroTowerCharacterSelectPage::Init()
{
	Super::Init();

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_HeroTop"));

	// 게임시작 요청여부 초기화
	UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

	auto DocBattle = UB2UIDocHelper::GetDocBattle();

	if (DocBattle)
	{
		ChangeMainClass = DocBattle->GetCurPCClass();
		ChangeSubClass = DocBattle->GetTagPCClass();
	}

	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		const EPCClass HeroClassEnum = IntToPCClass(ClassCnt);
		if (auto* DocHero = UB2UIDocHelper::GetDocHero(ClassCnt))
		{
			const int32 HeroLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(HeroClassEnum);
			HeroArr[ClassCnt]->SetHeroInfo(HeroClassEnum, HeroLevel, DocHero->GetCombatPower());
			HeroArr[ClassCnt]->SetSlotDelegate(FCharacterSlot::CreateLambda([this](EPCClass InValue) {SlotDelegateFunction(InValue); }));
		}
	}

	UpDateHero();
}

void UB2UIHeroTowerCharacterSelectPage::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_HeroTop"));
}

void UB2UIHeroTowerCharacterSelectPage::DestroySelf(UB2UIManager* InUIManager)
{
	if (UIP_RecommendPower.IsValid())
	{
		UIP_RecommendPower->DestroySelf(InUIManager);
	}
	Super::DestroySelf(InUIManager);
}

void UB2UIHeroTowerCharacterSelectPage::UpdateStaticText()
{
	if (TB_RecomPower.IsValid())
	{
		UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

		if (pDoc)
		{
			int32 nFloor = pDoc->GetStartFloor();

			nRecommandCombatPower = BladeIIGameImpl::GetClientDataStore().GetHeroTowerRecommendedCombat(nFloor);
			BladeIIGameImpl::GetClientDataStore().GetHeroTowerRecommendAbility(nFloor, RecommendAttack, RecommendDefense);
			TB_RecomPower->SetText(FText::AsNumber(nRecommandCombatPower));
		}
	}

	STB_MainChar->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CharacterSelect_MainPlayer")));
	STB_SubChar->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CharacterSelect_TagPlayer")));
	STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CharacterSelect_Title")));
	STB_Equip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_EquipMgmtButton")));
	STB_CombatPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_CurrentCombatpower")));
	STB_RecomPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_RecomCombatPower")));
}

void UB2UIHeroTowerCharacterSelectPage::RequestStartHeroTower(bool bUseGem)
{
	if (auto* pDoc = UB2UIDocHelper::GetDocHeroTower())
	{
		if (auto* pGameMode = Cast<AGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
			FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();

			StageData.SetHeroTowerFloor(pDoc->GetStartFloor());
			StageData.RequestStartHeroTower(pGameMode, pDoc->GetStartFloor(), CharDataStore.GetMainPlayerClass(), CharDataStore.GetSubPlayerClass());
		}
	}
}

void UB2UIHeroTowerCharacterSelectPage::PageBlock(bool bBlock)
{
	for (int32 SlotIdx = 0; SlotIdx < HeroArr.Num(); SlotIdx++)
	{
		auto& SlotWidget = HeroArr[SlotIdx];
		if (SlotWidget.IsValid())
			SlotWidget->SetSlotSelectable(!bBlock);
	}
}

void UB2UIHeroTowerCharacterSelectPage::BattleStart()
{
	if (CanStartBattle())
	{
		RequestStartHeroTower();
	}
}

bool UB2UIHeroTowerCharacterSelectPage::CanStartBattle()
{
	// 점검 정산 체크
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_HeroTower) && CheckContentsModeState(b2network::B2ContentsMode::HERO_TOWER))
		return false;

	auto* DocHeroTower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHeroTower == nullptr || DocHeroTower->GetDailyLeftFreeTryCount() == 0)
		return false;

	FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();
	const bool bIsAllSelected = (CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End && CharDataStore.GetSubPlayerClass() != EPCClass::EPC_End);
	if (bIsAllSelected == false)
		return false;

	return true;
}

void UB2UIHeroTowerCharacterSelectPage::OnClickBtnEquipManagement()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UIHeroTowerCharacterSelectPage::SetChangeCombatPower(int32 InValue, int32 CurAttack, int32 CurDefense)
{
	nCurrentCombatPower = InValue;

	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(InValue));

	if (TB_CombatPower.IsValid())
	{
		TB_CombatPower->SetColorAndOpacity(GetPowerColor(nCurrentCombatPower, nRecommandCombatPower));
	}

	if (UIP_RecommendPower.IsValid())
	{
		UIP_RecommendPower->SetInfo(CurAttack, CurDefense, RecommendAttack, RecommendDefense);
	}

	CheckStartButton();
}

void UB2UIHeroTowerCharacterSelectPage::CheckStartButton()
{
	FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();
	bool bIsAllSelected = (CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End && CharDataStore.GetSubPlayerClass() != EPCClass::EPC_End);
	
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->SetButtonEnabled(bIsAllSelected);
	}
}

void UB2UIHeroTowerCharacterSelectPage::OnClickBtnChangeMainSub()							//Main, Sub 교체 살려두겠습니다...
{
	// UB2UICharacterSelect 에서 하는 것과 중복되긴 하는데 따로 빼기도 뭐하군..

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{// Main/Sub 바꿔치기
		LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(DocBattle->GetTagPCClass()), IntToPCClass(DocBattle->GetCurPCClass()));
		Swap(ChangeMainClass, ChangeSubClass);
		// 선택 클래스가 변경됨에 따라 아린의 조언도 체크. 전투력은 둘이 합치니 달라질 게 없겠지만 스킬포인트 같은 거에 따라 이동 시 누구 화면으로 가는지 정도 달라지겠군 ㅋ
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Signal(DocBS ? DocBS->GetServerStageId() : FServerStageID());
		PlayChangeChacterAnimBP();

		UpDateHero();
	}
}

void UB2UIHeroTowerCharacterSelectPage::UpDateHero()
{
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	bool bCharacterEmptyState = false;

	if (DocBattle)
	{
		if (IMG_EmptyMain.IsValid())
			IMG_EmptyMain->SetVisibility(IntToPCClass(ChangeMainClass) == EPCClass::EPC_End ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (IMG_EmptySub.IsValid())
			IMG_EmptySub->SetVisibility(IntToPCClass(ChangeSubClass) == EPCClass::EPC_End ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (VB_MainCharacter.IsValid())
			VB_MainCharacter->SetVisibility(IntToPCClass(ChangeMainClass) == EPCClass::EPC_End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		if (VB_SubCharacter.IsValid())
			VB_SubCharacter->SetVisibility(IntToPCClass(ChangeSubClass) == EPCClass::EPC_End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		bCharacterEmptyState = (IntToPCClass(ChangeSubClass) == EPCClass::EPC_End || IntToPCClass(ChangeMainClass) == EPCClass::EPC_End) ? true : false;
	}
	
	int32 CombatPower = 0;
	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		auto DocHero = UB2UIDocHelper::GetDocHero(ClassCnt);

		if (DocHero && DocBattle)
		{
			if (ClassCnt == ChangeMainClass)
			{
				HeroArr[ClassCnt]->SetMainHero(true);
			}
			else if (ClassCnt == ChangeSubClass)
			{
				HeroArr[ClassCnt]->SetMainHero(false);
			}
			else
				HeroArr[ClassCnt]->SlotSelect(false);

			HeroArr[ClassCnt]->SetSelectArrow(bCharacterEmptyState);
		}

		if (ClassCnt == ChangeMainClass || (ClassCnt == ChangeSubClass))
		{
			CombatPower += DocHero->GetCombatPower();
		}
	}

	// 날개 갱신이 필요한 케이스가 있다. 메쉬조합체크
	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(ChangeMainClass));
	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(ChangeSubClass));

	LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(ChangeMainClass), IntToPCClass(ChangeSubClass));

	EPCClass CurPCClass = IntToPCClass(ChangeMainClass);
	EPCClass TagPCClass = IntToPCClass(ChangeSubClass);

	int32 MainCharAttack = CombatStatEval::GetPCAttack(CurPCClass);
	int32 MainCharDefense = CombatStatEval::GetPCDefense(CurPCClass);
	int32 TagCharAttack = CombatStatEval::GetPCAttack(TagPCClass);
	int32 TagCharDefense = CombatStatEval::GetPCDefense(TagPCClass);

	int32 CurAttack = MainCharAttack + TagCharAttack;
	int32 CurDefense = MainCharDefense + TagCharDefense;

	SetChangeCombatPower(CombatPower, CurAttack, CurDefense);
}

void UB2UIHeroTowerCharacterSelectPage::OnClickMainButton()
{
	if (ChangeMainClass == GetMaxPCClassNum())
		return;

	ChangeMainClass = GetMaxPCClassNum();
	UpDateHero();
}

void UB2UIHeroTowerCharacterSelectPage::OnClickSubButton()
{
	if (ChangeSubClass == GetMaxPCClassNum())
		return;

	ChangeSubClass = GetMaxPCClassNum();
	UpDateHero();
}

void UB2UIHeroTowerCharacterSelectPage::SlotDelegateFunction(EPCClass InValue)
{
	int32 EPCEND = GetMaxPCClassNum();
	int32 EPCValue = PCClassToInt(InValue);

	if (ChangeMainClass == EPCValue)
		ChangeMainClass = EPCEND;
	else if (ChangeSubClass == EPCValue)
		ChangeSubClass = EPCEND;
	else if (ChangeMainClass == EPCEND)
	{
		if (ChangeSubClass != EPCValue)
			ChangeMainClass = EPCValue;
	}
	else if (ChangeSubClass == EPCEND)
	{
		if (ChangeMainClass != EPCValue)
			ChangeSubClass = EPCValue;
	}
	else
		return;

	UpDateHero();
}
// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupUseResources.h"
#include "B2UIPVPCharacterSelectPage.h"
#include "B2UIHeader.h"
#include "B2UIShortagePopupManager.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"
#include "B2UIStartButton.h"
#include "B2UICharacterSelectPage.h"
#include "TutorialManager.h"
#include "B2NetGameMode.h"
#include "B2UIGameInviteList.h"
#include "B2UIFairyLinkButton.h"
#include "BladeIIGameImpl.h"

void UB2UIPVPCharacterSelectPage::CacheAssets()
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

	GET_SLOT(UB2Image, IMG_EmptyMain);
	GET_SLOT(UB2Image, IMG_EmptySub);

	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, TB_CombatPower);

	GET_SLOT(UVerticalBox, VB_MainCharacter);
	GET_SLOT(UVerticalBox, VB_SubCharacter);

	GET_SLOT(UB2Button, BTN_EquipManagement);
	GET_SLOT(UB2Button, BTN_CharacterChange);
	GET_SLOT(UB2Button, BTN_MainHero);
	GET_SLOT(UB2Button, BTN_SubHero);

	GET_SLOT(UB2UIStartButton, UIP_StartBT);

	GET_SLOT(UPanelWidget, P_AutoStart);
	GET_SLOT(UTextBlock, TB_AutoStartNotice);

	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->Init();
		UIP_StartBT->ClickDirectDelegateBindLambda([this]() { PageBlock(true); });
		UIP_StartBT->StartButtonDelegateBindLambda([this]() 
		{ 
			if (this->bFriendshipMode)
			{
				if (CanStartFriendshipGame())
					StartFriendshipGame();
				else
					ShowUserInviteUI();
			}
			else
			{
				BattleStart();
			}
		});
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_BattleStart")));
		UIP_StartBT->SetGoodsNumber(1);
	}

	GET_SLOT(UB2UIFairyLinkButton, UIP_FairyLinkButton);
	if (UIP_FairyLinkButton.IsValid())
		UIP_FairyLinkButton->Init();
}

void UB2UIPVPCharacterSelectPage::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_EquipManagement, &UB2UIPVPCharacterSelectPage::OnClickBtnEquipManagement);
	BIND_CLICK_FUNC_TO_BTN(BTN_MainHero, &UB2UIPVPCharacterSelectPage::OnClickMainButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_SubHero, &UB2UIPVPCharacterSelectPage::OnClickSubButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_CharacterChange, &UB2UIPVPCharacterSelectPage::OnClickBtnChangeMainSub);
}

void UB2UIPVPCharacterSelectPage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bFriendshipMode && bInInvitationSelect)
	{
		ElapsedSeconds += InDeltaTime;
		AutoStartSeconds -= InDeltaTime;
		if (ElapsedSeconds >= 1.f)
		{
			TimerTick_StartFriendshipGame(AutoStartSeconds);
			ElapsedSeconds = 0.f;
		}
	}
}

void UB2UIPVPCharacterSelectPage::Init()
{
	Super::Init();

	if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
	{
		bFriendshipMode = DocPVP->GetIsFriendshipGame();
		IsInvitee = DocPVP->GetIsInvitee();
	}

	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattle && DocBattleStage)
	{
		ChangeMainClass = DocBattle->GetCurPCClass();
		ChangeSubClass = DocBattle->GetTagPCClass();
	}

	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		if (auto* DocHero = UB2UIDocHelper::GetDocHero(ClassCnt))
		{
			const EPCClass HeroClassEnum = IntToPCClass(ClassCnt);
			const int32 HeroLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(HeroClassEnum);
			HeroArr[ClassCnt]->SetHeroInfo(HeroClassEnum, HeroLevel, DocHero->GetCombatPower());
			HeroArr[ClassCnt]->SetSlotDelegate(FCharacterSlot::CreateLambda([this](EPCClass InValue) {SlotDelegateFunction(InValue); }));
		}
	}

	UpDateHero();

	if (P_AutoStart.IsValid())
		P_AutoStart->SetVisibility(ESlateVisibility::Hidden);

	const FString HeaderKey = GetHeaderTextKey(bFriendshipMode);
	SetLobbyUIHeaderTitleByGeneralTextTableKey(HeaderKey);
}

void UB2UIPVPCharacterSelectPage::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	
	const FString HeaderKey = GetHeaderTextKey(bFriendshipMode);
	SetLobbyUIHeaderTitleByGeneralTextTableKey(HeaderKey);

	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->SetGoodsNumber(1);

		if (bFriendshipMode)
		{
			UIP_StartBT->HideAllIcon();
			UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_InviteFriendshipGame")));
		}
	}
}

void UB2UIPVPCharacterSelectPage::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);
}

void UB2UIPVPCharacterSelectPage::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
	{
		const bool IsInvalidFriendshipGame = DocPVP->GetIsFriendshipGame() && DocPVP->GetIsFrienshipGameValidSet() == false;
		if (IsInvalidFriendshipGame)
		{
			if (auto* MatchGameMode = Cast<AB2MatchGameModeBase>(GetWorld()->GetAuthGameMode()))
			{
				if(IsInvitee)
					MatchGameMode->RequestDeclineInvitation(EB2GameMode::PVP_Tag);

				MatchGameMode->ClearInvitationGameInfo(EB2GameMode::PVP_Tag);
			}
			// 상대방에 친선전 취소 날려야 함
		}
	}
}

void UB2UIPVPCharacterSelectPage::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_CombatPower.IsValid())
	{
		TB_CombatPower->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}

	if (STB_CombatPower.IsValid())
	{
		STB_CombatPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_CurrentCombatpower")));
	}
}

void UB2UIPVPCharacterSelectPage::BattleStart()
{
	if (CanStartBattle())
	{
		RequestStartPvPMatch(false);
	}
}

bool UB2UIPVPCharacterSelectPage::CheckBuyTicket()
{
	//true면  티켓이 있다 false면 없다요

	auto* PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	check(PvPDoc)
		if (PvPDoc->GetMatchPoint() == 0)
		{
			UB2UIMsgPopupUseResources* MsgUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupUseResources>(EUIMsgPopup::UseGem,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPMatch_BuyTicketTitleMsg")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::ConfirmOrCancel,
				FMsgPopupOnClick::CreateLambda([this]() { CheckGemForBuyTicket(); }));

			if (MsgUI)
			{
				MsgUI->SetCost(BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(PvPDoc->GetAdditionalMatchCount()));
				MsgUI->SetSubContent(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_BuyTicketSubMsg")));
			}
			return false;
		}

	return true;
}

void UB2UIPVPCharacterSelectPage::CheckGemForBuyTicket()
{
	auto* PvPBattleDoc = UB2UIDocHelper::GetDocPVP1on1Rival();
	int32 AdditonalCount = PvPBattleDoc->GetAdditionalMatchCount();

	if (BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(AdditonalCount) > BladeIIGameImpl::GetClientDataStore().GetGemAmount()) // 잼부족
		ShortageMGR->PopupGoToShopForGemMessage();
	else
		RequestStartPvPMatch(true);
}

void UB2UIPVPCharacterSelectPage::ShowFriendshipGameTimer()
{
	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{
		const float TotalInvitaitonTime = DocLobbyMatch->GetInviterTimeout();
		const float ElapsedTime = GWorld->GetTimeSeconds() - DocLobbyMatch->GetInvitationBeginTime();

		AutoStartSeconds = TotalInvitaitonTime - ElapsedTime;
		if (AutoStartSeconds > 0.f)
			bInInvitationSelect = true;

		if (P_AutoStart.IsValid())
			P_AutoStart->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		TimerTick_StartFriendshipGame(AutoStartSeconds);
	}
}

void UB2UIPVPCharacterSelectPage::SetGameStartSecondText(float Second)
{
	if (TB_AutoStartNotice.IsValid())
	{
		const int32 DisplaySeconds = FMath::CeilToFloat(Second);
		FText Message = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_GameAutoStartNotice")), FText::AsNumber(DisplaySeconds));
		TB_AutoStartNotice->SetText(Message);
	}
}

void UB2UIPVPCharacterSelectPage::StartFriendshipGame()
{
	if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
		DocPVP->SetIsFrienshipGameValidSet(true);

	if (auto* MatchGameMode = Cast<AB2MatchGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MatchGameMode->RequestAcceptInvitation(EB2GameMode::PVP_Tag);
		StartBladeIIPVPGame(MatchGameMode);
	}
}

void UB2UIPVPCharacterSelectPage::TimerTick_StartFriendshipGame(float LeftSeconds)
{
	if (LeftSeconds <= 0.f)
	{
		if (CanStartFriendshipGame())
			StartFriendshipGame();

		else 
		{
			if(auto* MatchGameMode = Cast<AB2MatchGameModeBase>(GetWorld()->GetAuthGameMode()))
				MatchGameMode->RequestDeclineInvitation(EB2GameMode::PVP_Tag);
		}

		if (P_AutoStart.IsValid())
			P_AutoStart->SetVisibility(ESlateVisibility::Hidden);

		bInInvitationSelect = false;
	}

	SetGameStartSecondText(LeftSeconds);
}

bool UB2UIPVPCharacterSelectPage::CanStartFriendshipGame()
{
	auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (bFriendshipMode && DocPVP)
	{
		FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();
		const bool bIsAllSelected = (CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End && CharDataStore.GetSubPlayerClass() != EPCClass::EPC_End);

		if (DocPVP->GetIsInvitee()) // 초대 수신자
		{
			return DocPVP->GetIsInviterReady() && bIsAllSelected;
		}
		else // 초대 발신자
		{
			return DocPVP->GetIsInviteeReady() && !DocPVP->GetInviterForceCancel() && bIsAllSelected;
		}
	}

	return false;
}

FString UB2UIPVPCharacterSelectPage::GetHeaderTextKey(bool bFriendshipGame)
{
	if (bFriendshipGame)
		return TEXT("PVP1on1_FriendshipGameStart");
		
	return TEXT("PVP1on1_TitleName");
}

void UB2UIPVPCharacterSelectPage::RequestStartPvPMatch(bool bUsedAdditional)
{
	auto* PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	if (PvPDoc)
		PvPDoc->SetUsedAdditionalPoint(bUsedAdditional);

	StartBladeIIPVPGame(Cast<AGameMode>(GetWorld()->GetAuthGameMode()));
}

void UB2UIPVPCharacterSelectPage::SetChangeCombatPower(int32 InValue)
{
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(InValue));

	nCurrentCombatPower = InValue;

	CheckStartButton();
}

void UB2UIPVPCharacterSelectPage::OnClickBtnEquipManagement()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UIPVPCharacterSelectPage::OnClickBtnChangeMainSub()							//Main, Sub 교체 살려두겠습니다...
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

void UB2UIPVPCharacterSelectPage::UpDateHero()
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
			CombatPower += DocHero->GetCombatPower();
	}

	// 날개 갱신이 필요한 케이스가 있다. 메쉬조합체크
	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(ChangeMainClass));
	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(ChangeSubClass));

	LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(ChangeMainClass), IntToPCClass(ChangeSubClass));

	SetChangeCombatPower(CombatPower);

	CheckStartButton();
}

void UB2UIPVPCharacterSelectPage::CheckStartButton()
{
	FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();
	const bool bIsAllSelected = (CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End && CharDataStore.GetSubPlayerClass() != EPCClass::EPC_End);

	if (UIP_StartBT.IsValid())
		UIP_StartBT->SetButtonEnabled(bIsAllSelected);
}

void UB2UIPVPCharacterSelectPage::PageBlock(bool bBlock)
{
	for (int32 SlotIdx = 0; SlotIdx < HeroArr.Num(); SlotIdx++)
	{
		auto& SlotWidget = HeroArr[SlotIdx];
		if (SlotWidget.IsValid())
			SlotWidget->SetSlotSelectable(!bBlock);
	}
}

bool UB2UIPVPCharacterSelectPage::CanStartBattle()
{
	FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();
	const bool bIsAllSelected = (CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End && CharDataStore.GetSubPlayerClass() != EPCClass::EPC_End);
	if (bIsAllSelected == false)
		return false;

	// 점검 정산 체크
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPOneAndOne) && CheckContentsModeState(b2network::B2ContentsMode::PVP_DUEL))
		return false;

	if (CheckBuyTicket() == false) 
		return false;

	return true;
}

void UB2UIPVPCharacterSelectPage::OnClickMainButton()
{
	if (ChangeMainClass == GetMaxPCClassNum())
		return;

	ChangeMainClass = GetMaxPCClassNum();
	UpDateHero();
}

void UB2UIPVPCharacterSelectPage::OnClickSubButton()
{
	if (ChangeSubClass == GetMaxPCClassNum())
		return;

	ChangeSubClass = GetMaxPCClassNum();
	UpDateHero();
}

void UB2UIPVPCharacterSelectPage::SlotDelegateFunction(EPCClass InValue)
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

void UB2UIPVPCharacterSelectPage::OnPrevAcceptInvitation()
{
	if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
	{
		bFriendshipMode = DocPVP->GetIsFriendshipGame();
		IsInvitee = DocPVP->GetIsInvitee();
	}

	if (bFriendshipMode && IsInvitee)
	{
		ShowFriendshipGameTimer();
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_StartFriendshipGame")));
		UIP_StartBT->HideAllIcon();

		const FString HeaderKey = GetHeaderTextKey(bFriendshipMode);
		SetLobbyUIHeaderTitleByGeneralTextTableKey(HeaderKey);
	}
}

void UB2UIPVPCharacterSelectPage::OnInvitationDeclineByInvitee()
{
	if (P_AutoStart.IsValid())
		P_AutoStart->SetVisibility(ESlateVisibility::Hidden);

	if(UIP_StartBT.IsValid())
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_InviteFriendshipGame")));

	bInInvitationSelect = false;
}

void UB2UIPVPCharacterSelectPage::OnInvitationSuccess()
{
	auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (bFriendshipMode && DocPVP)
	{
		if (CanStartFriendshipGame())
			StartFriendshipGame();
	}
}

void UB2UIPVPCharacterSelectPage::ShowUserInviteUI()
{
	UB2UIManager* UIManager = UB2UIManager::GetInstance();
	if(auto* InviteUI = UIManager->OpenUI<UB2UIGameInviteList>(UIFName::RaidInviteList))
	{
		PageBlock(true);

		InviteUI->SetInviteUIByGameMode(EB2GameMode::PVP_Tag);

		InviteUI->FCloseEvent.Unbind();
		InviteUI->FCloseEvent.BindUObject(this, &UB2UIPVPCharacterSelectPage::PageBlock);
	}
}

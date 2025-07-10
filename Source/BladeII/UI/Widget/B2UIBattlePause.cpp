#include "B2UIBattlePause.h"
#include "EventSubsystem.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"

UB2UIBattlePause::UB2UIBattlePause(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIBattlePause::Init()
{
	Super::Init();

	SubscribeEvents();
}

void UB2UIBattlePause::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvents();
}

void UB2UIBattlePause::CacheAssets()
{
	GET_SLOT(UButton, BTN_Continue)
	GET_SLOT(UButton, BTN_Configuration)
	GET_SLOT(UButton, BTN_Village)
	GET_SLOT(UButton, BTN_WorldMap)
	GET_SLOT(UButton, BTN_TagMatch)
	GET_SLOT(UButton, BTN_TeamMatch)
	GET_SLOT(UButton, BTN_CounterAttack)
	GET_SLOT(UButton, BTN_HeroTower)
	GET_SLOT(UButton, BTN_Raid)
	GET_SLOT(UButton, BTN_Guild)
	GET_SLOT(UButton, BTN_Dimension)

	GET_SLOT(UPanelWidget, WorldMapButtonSet);
	GET_SLOT(UPanelWidget, TagMatchButtonSet);
	GET_SLOT(UPanelWidget, TeamMatchButtonSet);
	GET_SLOT(UPanelWidget, CounterAttackButtonSet);
	GET_SLOT(UPanelWidget, HeroTowerButtonSet);
	GET_SLOT(UPanelWidget, RaidButtonSet);
	GET_SLOT(UPanelWidget, GuildButtonSet);
	GET_SLOT(UPanelWidget, DimensionButtonSet);
}

void UB2UIBattlePause::UpdateStaticText()
{
	//귀찮은건 BP에서 처리
}

void UB2UIBattlePause::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Continue, &UB2UIBattlePause::OnClickBtnContinue)
	BIND_CLICK_FUNC_TO_BTN(BTN_Configuration, &UB2UIBattlePause::OnClickBtnConfiguration)
	BIND_CLICK_FUNC_TO_BTN(BTN_Village, &UB2UIBattlePause::OnClickBtnVillage)
	BIND_CLICK_FUNC_TO_BTN(BTN_WorldMap, &UB2UIBattlePause::OnClickBtnWorldMap)
	BIND_CLICK_FUNC_TO_BTN(BTN_TagMatch, &UB2UIBattlePause::OnClickBtnTagMatch)
	BIND_CLICK_FUNC_TO_BTN(BTN_TeamMatch, &UB2UIBattlePause::OnClickBtnTeamMatch)
	BIND_CLICK_FUNC_TO_BTN(BTN_CounterAttack, &UB2UIBattlePause::OnClickBtnCounterAttack)
	BIND_CLICK_FUNC_TO_BTN(BTN_HeroTower, &UB2UIBattlePause::OnClickBtnHeroTower)
	BIND_CLICK_FUNC_TO_BTN(BTN_Raid, &UB2UIBattlePause::OnClickBtnRaid)
	BIND_CLICK_FUNC_TO_BTN(BTN_Guild, &UB2UIBattlePause::OnClickBtnGuild)
	BIND_CLICK_FUNC_TO_BTN(BTN_Dimension, &UB2UIBattlePause::OnClickBtnDimension)
}

void UB2UIBattlePause::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryGetHeroTowerClass<FB2ResponseGetHeroTowerPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseGetHeroTowerPtr& msgPtr)
	{
		if (UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower())
			pDoc->SetGetHeroTowerPtr(msgPtr);

		ReturnToHeroTowerMainMenuClass<>::GetInstance().Signal();
		UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
	}
	));
}

void UB2UIBattlePause::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIBattlePause::SetMod(EB2GameMode Mod)
{
	if (WorldMapButtonSet.IsValid())
		WorldMapButtonSet->SetVisibility(Mod == EB2GameMode::Scenario ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (TagMatchButtonSet.IsValid())
		TagMatchButtonSet->SetVisibility(Mod == EB2GameMode::PVP_Tag ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (TeamMatchButtonSet.IsValid())
		TeamMatchButtonSet->SetVisibility(Mod == EB2GameMode::PVP_Team ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (CounterAttackButtonSet.IsValid())
		CounterAttackButtonSet->SetVisibility(Mod == EB2GameMode::CounterDungeon ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (HeroTowerButtonSet.IsValid())
		HeroTowerButtonSet->SetVisibility(Mod == EB2GameMode::HeroTower ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (RaidButtonSet.IsValid())
		RaidButtonSet->SetVisibility(Mod == EB2GameMode::Raid ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (GuildButtonSet.IsValid())
		GuildButtonSet->SetVisibility(Mod == EB2GameMode::Guild ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (DimensionButtonSet.IsValid())
		DimensionButtonSet->SetVisibility(Mod == EB2GameMode::Dimension ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UIBattlePause::CloseWidgetDelegate()
{
	OnClickBtnContinue();
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIBattlePause::OnClickBtnContinue()
{
	//UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
	//UGameplayStatics::SetGamePaused(this->GetOwningPlayer(), false); // 무조건 pause 에서 복귀.

	//// 이 BattlePause UI 는 인게임 전투 다른 부분에서는 안 쓰는 텍스쳐를 사용하므로 나가면서 GC 를 한번 돌려준다. 예를 들어 복귀 직후 보스를 죽이면서 승리 연출 같은 거 나오면 미처 GC 돌기 전에 메모리가 더 올라갈 수 있으므로
	//UWorld* TheWorld = GetWorld();
	//if (TheWorld)
	//{
	//	TheWorld->ForceGarbageCollection();
	//}

	//// 이후 처리는 ABladeIIGameMode::ClearPause 에서
	//
	//// pvp에서만 사용
	//if (GetB2GameModeType(this) == EB2GameMode::PVP_Tag)
	//{
	//	StopPauseMenuClass<>::GetInstance().Signal();
	//}
}

void UB2UIBattlePause::OnClickBtnConfiguration()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->OpenUI(UIFName::SemiSetting);
	}
}

void UB2UIBattlePause::OnClickBtnRaid()
{
	FText msg = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_PenaltyNotice2")); 
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		if (DocRaid->GetAlreadyExitUser())
			msg = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_NonPenaltyNotice")); // 이미 탈주한 사람있음.

		DocRaid->SetReturnToPageType(ERaidGotoWhere::RaidMain);
	}

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		msg,
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([](){
		UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
		ReturnToRaidMainClass<>::GetInstance().Signal();
	})
	);
}

void UB2UIBattlePause::OnClickBtnGuild()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		(GetB2GameModeType(this) == EB2GameMode::PVP_Tag || GetB2GameModeType(this) == EB2GameMode::PVP_Team) ?
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit_PVP")) :
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([]() {
		UB2UIManager::GetInstance()->CloseUI(UIFName::GuildMatchBattle);
		ReturnToGuildMainClass<>::GetInstance().Signal();
	})
	);
}

void UB2UIBattlePause::OnClickBtnVillage()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			(GetB2GameModeType(this) == EB2GameMode::PVP_Tag || GetB2GameModeType(this) == EB2GameMode::PVP_Team) ?
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit_PVP")) :
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([](){
				CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // 반복전투 도중이었다면 Pause 메뉴 통해 로비로 나갈 때 해제.
				UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
				GoToVillageClass<>::GetInstance().Signal();
			})
	);
}

void UB2UIBattlePause::OnClickBtnWorldMap()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([](){
				GiveUpGameClass<>::GetInstance().Signal();
				CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // 반복전투 도중이었다면 Pause 메뉴 통해 로비로 나갈 때 해제.
				ChangeStaminaChargeTimeClass<>::GetInstance().Signal();
				UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
				GoToMapClass<>::GetInstance().Signal();
		})
	);
}

void UB2UIBattlePause::OnClickBtnTagMatch()
{
	bool IsFriendshipGame = false;
	if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
		IsFriendshipGame = DocPVP->GetIsFriendshipGame();

	FString SummaryText = IsFriendshipGame ? TEXT("SensitiveNoti_WarnForQuit_FriendshipGame") : TEXT("SensitiveNoti_WarnForQuit_PVP");

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, SummaryText),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([](){
			UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
			ReturnToPVPMainMenuClass<bool>::GetInstance().Signal(true);
		})
	);
}

void UB2UIBattlePause::OnClickBtnTeamMatch()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit_PVP")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([](){
			UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
			ReturnToPVPMainMenuClass<bool>::GetInstance().Signal(false);
		})
	);
}

void UB2UIBattlePause::OnClickBtnCounterAttack()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit_Counter")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([](){
			UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
			GoToMapClass<>::GetInstance().Signal();
		})
	);
}

void UB2UIBattlePause::OnClickBtnHeroTower()
{
	

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit_HeroTower")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([](){
		GiveUpGameClass<>::GetInstance().Signal();
		data_trader::Retailer::GetInstance().RequestGetHeroTower();
	})
		);
}

void UB2UIBattlePause::OnClickBtnDimension()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_WarnForQuit_Dimension")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([]() {
		UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
		GiveUpGameClass<>::GetInstance().Signal();
	})
	);
}

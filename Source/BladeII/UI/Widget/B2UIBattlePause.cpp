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
	//�������� BP���� ó��
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
	//UGameplayStatics::SetGamePaused(this->GetOwningPlayer(), false); // ������ pause ���� ����.

	//// �� BattlePause UI �� �ΰ��� ���� �ٸ� �κп����� �� ���� �ؽ��ĸ� ����ϹǷ� �����鼭 GC �� �ѹ� �����ش�. ���� ��� ���� ���� ������ ���̸鼭 �¸� ���� ���� �� ������ ��ó GC ���� ���� �޸𸮰� �� �ö� �� �����Ƿ�
	//UWorld* TheWorld = GetWorld();
	//if (TheWorld)
	//{
	//	TheWorld->ForceGarbageCollection();
	//}

	//// ���� ó���� ABladeIIGameMode::ClearPause ����
	//
	//// pvp������ ���
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
			msg = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_NonPenaltyNotice")); // �̹� Ż���� �������.

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
				CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // �ݺ����� �����̾��ٸ� Pause �޴� ���� �κ�� ���� �� ����.
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
				CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // �ݺ����� �����̾��ٸ� Pause �޴� ���� �κ�� ���� �� ����.
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

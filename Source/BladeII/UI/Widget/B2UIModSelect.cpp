#include "BladeII.h"
#include "B2Button.h"
#include "B2UIModSelect.h"
#include "B2UIChallengePopUp.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "Retailer.h"

UB2UIModSelect::UB2UIModSelect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bExternalDynamicSetupReady = false; // �ñ������δ� true �� �Ǿ��.
}

void UB2UIModSelect::Init()
{
	Super::Init();

	auto* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	if (HeaderUI)
		HeaderUI->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Challenge")));
}

void UB2UIModSelect::CacheAssets()
{
	GET_SLOT(UImage, IMG_BG);
	if (bExternalDynamicSetupReady)
	{
		IMG_BG->SetVisibility(ESlateVisibility::Collapsed); // �ñ������� �̰� ���Ű� �Ǿ�� ��.
	}

	//
	// Locked ���δ� ���� �ý��������� ���� ��.
	//

	GET_SLOT(UB2UISingleModSelect, UIP_ModSelect_PVPP);
	if (UIP_ModSelect_PVPP.IsValid()){
		UIP_ModSelect_PVPP->Init();
		UIP_ModSelect_PVPP->SetMyModInfo(ELobbyModSceneElem::LMODIDX_PVP, false);
	}
	GET_SLOT(UB2UISingleModSelect, UIP_ModSelect_DayDungeonP);
	if (UIP_ModSelect_DayDungeonP.IsValid()){
		UIP_ModSelect_DayDungeonP->Init();
		UIP_ModSelect_DayDungeonP->SetMyModInfo(ELobbyModSceneElem::LMODIDX_CounterDungeon, false);
	}
	GET_SLOT(UB2UISingleModSelect, UIP_ModSelect_HeroTopP);
	if (UIP_ModSelect_HeroTopP.IsValid()){
		UIP_ModSelect_HeroTopP->Init();
		UIP_ModSelect_HeroTopP->SetMyModInfo(ELobbyModSceneElem::LMODIDX_HeroTop, false);
	}
	GET_SLOT(UB2UISingleModSelect, UIP_ModSelect_TeamMatchP);
	if (UIP_ModSelect_TeamMatchP.IsValid()){
		UIP_ModSelect_TeamMatchP->Init();
		UIP_ModSelect_TeamMatchP->SetMyModInfo(ELobbyModSceneElem::LMODIDX_TeamMatch, false);
	}
	GET_SLOT(UB2UISingleModSelect, UIP_ModSelect_RaidP);
	if (UIP_ModSelect_RaidP.IsValid()){
		UIP_ModSelect_RaidP->Init();
		UIP_ModSelect_RaidP->SetMyModInfo(ELobbyModSceneElem::LMODIDX_Raid, false);
	}
	GET_SLOT(UB2UISingleModSelect, UIP_ModSelect_GuildMatchP);
	if (UIP_ModSelect_GuildMatchP.IsValid()){
		UIP_ModSelect_GuildMatchP->Init();
		UIP_ModSelect_GuildMatchP->SetMyModInfo(ELobbyModSceneElem::LMODIDX_GuildMatch, true);
	}
	GET_SLOT(UB2UISingleModSelect, UIP_ModSelect_ControlP);
	if (UIP_ModSelect_ControlP.IsValid()){
		UIP_ModSelect_ControlP->Init();
		UIP_ModSelect_ControlP->SetMyModInfo(ELobbyModSceneElem::LMODIDX_Control, false);
	}
}

void UB2UIModSelect::BindDelegates()
{
	if (UIP_ModSelect_PVPP.IsValid()){
		UIP_ModSelect_PVPP->OnBtnAreaClickDelegate.BindDynamic(this, &UB2UIModSelect::OnClickBtnTagMatch);
	}
	if (UIP_ModSelect_DayDungeonP.IsValid()){
		UIP_ModSelect_DayDungeonP->OnBtnAreaClickDelegate.BindDynamic(this, &UB2UIModSelect::OnClickBtnDayDungeon);
	}
	if (UIP_ModSelect_HeroTopP.IsValid()){
		UIP_ModSelect_HeroTopP->OnBtnAreaClickDelegate.BindDynamic(this, &UB2UIModSelect::OnClickBtnHeroTop);
	}
	if (UIP_ModSelect_TeamMatchP.IsValid()){
		UIP_ModSelect_TeamMatchP->OnBtnAreaClickDelegate.BindDynamic(this, &UB2UIModSelect::OnClickBtnTeamMatch);
	}
	if (UIP_ModSelect_RaidP.IsValid()){
		UIP_ModSelect_RaidP->OnBtnAreaClickDelegate.BindDynamic(this, &UB2UIModSelect::OnClickBtnRaid);
	}
	if (UIP_ModSelect_GuildMatchP.IsValid()){
		UIP_ModSelect_GuildMatchP->OnBtnAreaClickDelegate.BindDynamic(this, &UB2UIModSelect::OnClickBtnGuildMatch);
	}
	if (UIP_ModSelect_ControlP.IsValid()){
		UIP_ModSelect_ControlP->OnBtnAreaClickDelegate.BindDynamic(this, &UB2UIModSelect::OnClickBtnControl);
	}
}

void UB2UIModSelect::DestroySelf(UB2UIManager* InUIManager)
{
	if (UIP_ModSelect_PVPP.IsValid()){
		UIP_ModSelect_PVPP->OnBtnAreaClickDelegate.Unbind();
	}
	if (UIP_ModSelect_DayDungeonP.IsValid()){
		UIP_ModSelect_DayDungeonP->OnBtnAreaClickDelegate.Unbind();
	}
	if (UIP_ModSelect_HeroTopP.IsValid()){
		UIP_ModSelect_HeroTopP->OnBtnAreaClickDelegate.Unbind();
	}
	if (UIP_ModSelect_TeamMatchP.IsValid()){
		UIP_ModSelect_TeamMatchP->OnBtnAreaClickDelegate.Unbind();
	}
	if (UIP_ModSelect_RaidP.IsValid()){
		UIP_ModSelect_RaidP->OnBtnAreaClickDelegate.Unbind();
	}
	if (UIP_ModSelect_GuildMatchP.IsValid()){
		UIP_ModSelect_GuildMatchP->OnBtnAreaClickDelegate.Unbind();
	}
	if (UIP_ModSelect_ControlP.IsValid()){
		UIP_ModSelect_ControlP->OnBtnAreaClickDelegate.Unbind();
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIModSelect::SelectModOpenPopUp(ELobbyModSceneElem InModEnum, bool bLock)
{
	UB2UIChallengePopUp* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIChallengePopUp>(UIFName::ChallengePopUp);
	pUI->Init();
	pUI->SetPopupInfo(InModEnum, bLock);
	pUI->OnClickDelegate.BindUObject(this, &UB2UIModSelect::OnClickPopUpProcess);
}


void UB2UIModSelect::OnClickPopUpProcess(ELobbyModSceneElem InModEnum)
{
	switch (InModEnum)
	{
	case ELobbyModSceneElem::LMODIDX_PVP:
		data_trader::Retailer::GetInstance().RequestGetMatchStatus();
		break;
	case ELobbyModSceneElem::LMODIDX_TeamMatch:
		data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
		break;
	case ELobbyModSceneElem::LMODIDX_HeroTop:
		DevShowNotReadyYetMessage();
		//data_trader::Retailer::GetInstance().RequestGetHeroTower();
		break;
	case ELobbyModSceneElem::LMODIDX_GuildMatch:
		DevShowNotReadyYetMessage();
		break;
	case ELobbyModSceneElem::LMODIDX_CounterDungeon:
		DevShowNotReadyYetMessage();
		//data_trader::Retailer::GetInstance().RequestGetCounterDungeon();
		break;
	case ELobbyModSceneElem::LMODIDX_Raid:
		DevShowNotReadyYetMessage();
		break;
	case ELobbyModSceneElem::LMODIDX_Control:
		data_trader::Retailer::GetInstance().RequestAssaultBattleStatus();
	default:
		break;
	}
}


void UB2UIModSelect::OnClickBtnTagMatch()
{
	//data_trader::Retailer::GetInstance().RequestGetMatchStatus();	
	SelectModOpenPopUp(ELobbyModSceneElem::LMODIDX_PVP);
}

void UB2UIModSelect::OnClickBtnDayDungeon()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::CounterAttackDGMain);
	
	//data_trader::Retailer::GetInstance().RequestGetCounterDungeon();
	//DevShowNotReadyYetMessage();

	SelectModOpenPopUp(ELobbyModSceneElem::LMODIDX_CounterDungeon);
}

void UB2UIModSelect::OnClickBtnHeroTop()
{
	// �ٷο���. �������� ����� ��û�Ҽ���..
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::HeroTowerMain);

	//data_trader::Retailer::GetInstance().RequestGetHeroTower();

	SelectModOpenPopUp(ELobbyModSceneElem::LMODIDX_HeroTop, ShouldShowGDCVersionContent()); //[GDC]
}

void UB2UIModSelect::OnClickBtnTeamMatch()
{
	//data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();	
	SelectModOpenPopUp(ELobbyModSceneElem::LMODIDX_TeamMatch);
}

void UB2UIModSelect::OnClickBtnRaid()
{
	//LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_RaidMain);

	SelectModOpenPopUp(ELobbyModSceneElem::LMODIDX_Raid, ShouldShowGDCVersionContent()); //[GDC]
}

void UB2UIModSelect::OnClickBtnGuildMatch()
{
	//DevShowNotReadyYetMessage();
	SelectModOpenPopUp(ELobbyModSceneElem::LMODIDX_GuildMatch);
}

void UB2UIModSelect::OnClickBtnControl()
{
	SelectModOpenPopUp(ELobbyModSceneElem::LMODIDX_Control, ShouldShowGDCVersionContent());//[GDC]
}

void UB2UIModSelect::UpdateModUIPosition(const TArray<FVector2D>& Positions2D)
{
	if (!bExternalDynamicSetupReady){
		return;
	}

	// Position2D �迭 �ε����� ELobbyModSceneElem ������ �µ���.

	checkSlow(Positions2D.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);

	for (int32 PosIdx = 0; PosIdx < Positions2D.Num(); ++PosIdx)
	{
		ELobbyModSceneElem ThisModEnum = static_cast<ELobbyModSceneElem>(PosIdx);
		UB2UISingleModSelect* ThisModWidget = GetModSelectWidget(ThisModEnum);
		// ��ü ȭ�� ũ���� CanvasPanel �ȿ� ��ġ�� �Ǿ� �־�� ��.
		UCanvasPanelSlot* ThisModWidgetSlot = ThisModWidget ? Cast<UCanvasPanelSlot>(ThisModWidget->Slot) : NULL;
		if (ThisModWidgetSlot)
		{
			const FVector2D& ThisPos = Positions2D[PosIdx];
			// ���� ��ǥ�� ����ϵ��� offset ����
			FVector2D PlaceOffset = -1.0f * ThisModWidgetSlot->GetSize();
			PlaceOffset *= 0.5f;
			ThisModWidgetSlot->SetPosition(ThisPos + PlaceOffset);
		}
	}
}

UB2UISingleModSelect* UB2UIModSelect::GetModSelectWidget(ELobbyModSceneElem InModeScene)
{ // ���� �̰� Button �� �ƴ� WidgetPart �� �����ϵ��� �ٲ� ��.
	switch (InModeScene)
	{
	case ELobbyModSceneElem::LMODIDX_PVP: return UIP_ModSelect_PVPP.Get();
	case ELobbyModSceneElem::LMODIDX_TeamMatch: return UIP_ModSelect_TeamMatchP.Get();
	case ELobbyModSceneElem::LMODIDX_HeroTop: return UIP_ModSelect_HeroTopP.Get();
	case ELobbyModSceneElem::LMODIDX_GuildMatch: return UIP_ModSelect_GuildMatchP.Get();
	case ELobbyModSceneElem::LMODIDX_CounterDungeon: return UIP_ModSelect_DayDungeonP.Get();
	case ELobbyModSceneElem::LMODIDX_Raid: return UIP_ModSelect_RaidP.Get();
	case ELobbyModSceneElem::LMODIDX_Control: return UIP_ModSelect_ControlP.Get();
	}
	return NULL;
}
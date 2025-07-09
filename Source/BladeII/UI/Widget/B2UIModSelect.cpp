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
	bExternalDynamicSetupReady = false; // 궁극적으로는 true 로 되어야.
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
		IMG_BG->SetVisibility(ESlateVisibility::Collapsed); // 궁극적으로 이건 제거가 되어야 함.
	}

	//
	// Locked 여부는 추후 시스템적으로 들어가야 함.
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
	// 바로연다. 프로토콜 생기면 요청할수도..
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

	// Position2D 배열 인덱스는 ELobbyModSceneElem 순서에 맞도록.

	checkSlow(Positions2D.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);

	for (int32 PosIdx = 0; PosIdx < Positions2D.Num(); ++PosIdx)
	{
		ELobbyModSceneElem ThisModEnum = static_cast<ELobbyModSceneElem>(PosIdx);
		UB2UISingleModSelect* ThisModWidget = GetModSelectWidget(ThisModEnum);
		// 전체 화면 크기의 CanvasPanel 안에 배치가 되어 있어야 함.
		UCanvasPanelSlot* ThisModWidgetSlot = ThisModWidget ? Cast<UCanvasPanelSlot>(ThisModWidget->Slot) : NULL;
		if (ThisModWidgetSlot)
		{
			const FVector2D& ThisPos = Positions2D[PosIdx];
			// 센터 좌표를 사용하도록 offset 적용
			FVector2D PlaceOffset = -1.0f * ThisModWidgetSlot->GetSize();
			PlaceOffset *= 0.5f;
			ThisModWidgetSlot->SetPosition(ThisPos + PlaceOffset);
		}
	}
}

UB2UISingleModSelect* UB2UIModSelect::GetModSelectWidget(ELobbyModSceneElem InModeScene)
{ // 추후 이건 Button 이 아닌 WidgetPart 를 리턴하도록 바뀔 것.
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
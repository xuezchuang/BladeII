#include "B2UIModSelect.h"
#include "B2Button.h"

#include "B2UIChallengePopUp.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "Retailer.h"

UB2UIModSelect::UB2UIModSelect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bExternalDynamicSetupReady = false; // 泵必利栏肺绰 true 肺 登绢具.
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
		IMG_BG->SetVisibility(ESlateVisibility::Collapsed); // 泵必利栏肺 捞扒 力芭啊 登绢具 窃.
	}

	//
	// Locked 咯何绰 眠饶 矫胶袍利栏肺 甸绢啊具 窃.
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
	// 官肺楷促. 橇肺配妮 积扁搁 夸没且荐档..
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

	// Position2D 硅凯 牢郸胶绰 ELobbyModSceneElem 鉴辑俊 嘎档废.

	checkSlow(Positions2D.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);

	for (int32 PosIdx = 0; PosIdx < Positions2D.Num(); ++PosIdx)
	{
		ELobbyModSceneElem ThisModEnum = static_cast<ELobbyModSceneElem>(PosIdx);
		UB2UISingleModSelect* ThisModWidget = GetModSelectWidget(ThisModEnum);
		// 傈眉 拳搁 农扁狼 CanvasPanel 救俊 硅摹啊 登绢 乐绢具 窃.
		UCanvasPanelSlot* ThisModWidgetSlot = ThisModWidget ? Cast<UCanvasPanelSlot>(ThisModWidget->Slot) : NULL;
		if (ThisModWidgetSlot)
		{
			const FVector2D& ThisPos = Positions2D[PosIdx];
			// 季磐 谅钎甫 荤侩窍档废 offset 利侩
			FVector2D PlaceOffset = -1.0f * ThisModWidgetSlot->GetSize();
			PlaceOffset *= 0.5f;
			ThisModWidgetSlot->SetPosition(ThisPos + PlaceOffset);
		}
	}
}

UB2UISingleModSelect* UB2UIModSelect::GetModSelectWidget(ELobbyModSceneElem InModeScene)
{ // 眠饶 捞扒 Button 捞 酒囱 WidgetPart 甫 府畔窍档废 官拆 巴.
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
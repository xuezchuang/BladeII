#include "bladeII.h"
#include "B2UIContentsSubMain.h"
#include "B2UIEnum.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2LobbyUI_HeroMgmtCommon.h"
#include "B2Airport.h"
#include "B2LobbyInventory.h"
#include "Retailer.h"
#include "B2EtherManager.h"


#include "BladeIIGameImpl.h"

void UB2UIContensSubMain::Init()
{
	Super::Init();

	bool bGDC = ShouldShowGDCVersionContent(); // [GDC] Feb 2017

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyHeroMgmt_Title"));

	SetUseLock_BP(bGDC);
}

void UB2UIContensSubMain::CacheAssets()
{
	GET_SLOT(UUserWidget, UIP_ContentSubInventory);
	GET_SLOT(UUserWidget, UIP_ContentSubSkill);
	GET_SLOT(UUserWidget, UIP_ContentSubBrevet);
	GET_SLOT(UUserWidget, UIP_ContentSubWing);
	GET_SLOT(UUserWidget, UIP_ContentSubRelic);
	GET_SLOT(UUserWidget, UIP_ContentSubEther);
}


void UB2UIContensSubMain::BindDelegates()
{
}

void UB2UIContensSubMain::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIContensSubMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UB2UIContensSubMain::OnSceneOpen(EUIScene InOpenedScene)
{
	DoMarkRedDot();
}

//====================================================================================
//									Click Handler
//====================================================================================

void UB2UIContensSubMain::GotoInvenScene()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory); // GameMode 쪽 신호
}

void UB2UIContensSubMain::GotoSkillScene()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Skill); // GameMode 쪽 신호
}

void UB2UIContensSubMain::GotoBrevetScene()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Brevet); // GameMode 쪽 신호
}

void UB2UIContensSubMain::GotoWingScene()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Wing); // GameMode 쪽 신호
}

void UB2UIContensSubMain::GotoConstantheroScene()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Relic); // GameMode 쪽 신호
	//DevShowNotReadyYetMessage();
}

void UB2UIContensSubMain::GotoEtherSecne()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EtherMain);
}

void UB2UIContensSubMain::GotoNotYetSecne()
{
	DevShowNotReadyYetMessage();
}

bool UB2UIContensSubMain::HasRedDot_Inventory()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Inventory();
}

bool UB2UIContensSubMain::HasRedDot_Skill()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Skill();
}

bool UB2UIContensSubMain::HasRedDot_Brevet()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Brevet();
}

bool UB2UIContensSubMain::HasRedDot_Wing()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Wing();
}

bool UB2UIContensSubMain::HasRedDot_Relic()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Relic();
}

bool UB2UIContensSubMain::HasRedDot_Ether()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Ether();
}

//====================================================================================
//									Mark For New
//====================================================================================

void UB2UIContensSubMain::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_ContentSubInventory.Get()), &UB2UIContensSubMain::HasRedDot_Inventory);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_ContentSubSkill.Get()), &UB2UIContensSubMain::HasRedDot_Skill);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_ContentSubBrevet.Get()), &UB2UIContensSubMain::HasRedDot_Brevet);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_ContentSubWing.Get()), &UB2UIContensSubMain::HasRedDot_Wing);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_ContentSubRelic.Get()), &UB2UIContensSubMain::HasRedDot_Relic);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_ContentSubEther.Get()), &UB2UIContensSubMain::HasRedDot_Ether);
}

#include "BladeII.h"
#include "B2UIGuildJoinAndCreate.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIGuildCreate.h"
#include "Retailer.h"


void UB2UIGuildJoinAndCreate::Init()
{
	Super::Init();
}

void UB2UIGuildJoinAndCreate::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_GuildCreate);
	GET_SLOT(UB2Button, BTN_GuildJoin);
}

void UB2UIGuildJoinAndCreate::UpdateStaticText()
{
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("Guild_GuildJoin"));
}

void UB2UIGuildJoinAndCreate::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("Guild_GuildJoin"));
}

void UB2UIGuildJoinAndCreate::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildCreate, &UB2UIGuildJoinAndCreate::OnClickGuildCreate)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildJoin, &UB2UIGuildJoinAndCreate::OnClickGuildJoin)
}

void UB2UIGuildJoinAndCreate::OnClickGuildCreate()
{
	UB2UIGuildCreate* JoinCreate = UB2UIManager::GetInstance()->OpenUI<UB2UIGuildCreate>(UIFName::GuildCreate);
	UB2GuildMarkInfo* GuildInfo = StaticFindGuildMarkInfo();
}

void UB2UIGuildJoinAndCreate::OnClickGuildJoin()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::RecommendGuildInfo);
	//data_trader::Retailer::GetInstance().RequestGuildAskJoinGuild(7);
}


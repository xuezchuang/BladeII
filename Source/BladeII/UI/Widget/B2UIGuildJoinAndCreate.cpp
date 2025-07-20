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

	// UIHeader 客 鞍捞 Scene 栏肺 备己窍绰 版快 Init 矫痢俊 窍搁 Scene 俊 器窃等 header 啊 积己捞 救等 惑炔老 荐 乐澜.
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


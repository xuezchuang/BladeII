#include "BladeII.h"
#include "B2UIGuildJoinList.h"
#include "B2UIGuildSlotBar.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIGuildCreate.h"
#include "B2UIDocHelper.h"
#include "B2UICommonTab.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2UIGuildJoinList::~UB2UIGuildJoinList()
{
	DeliveryReceiveGetRecommendGuildClass<FB2ReciveRecommenGuild>::GetInstance().Unsubscribe(DeliveryReceiveGetRecommendGuildTicket);
}

void UB2UIGuildJoinList::Init()
{
	Super::Init();
	UIP_GuildCreate->Init();

}

void UB2UIGuildJoinList::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);


	DeliveryReceiveGetRecommendGuildTicket = DeliveryReceiveGetRecommendGuildClass<FB2ReciveRecommenGuild>::GetInstance().Subscribe([this](const FB2ReciveRecommenGuild& GuildInfo)
	{
		if (this->TapType != GUILD_Recommend)
			return;
		//auto OtherGuildInfo = GET_TUPLE_DATA(FB2ResponseeRecommenGuild::guilds_index, GuildInfo);
		//this->SetRecommendGuild(OtherGuildInfo);

	});

	DeliveryReceiveGetAcceptGuildTicket = DeliverGetGuildInviteListClass<FB2ResponseGetGuildInviteListPtr>::GetInstance().Subscribe([this](const FB2ResponseGetGuildInviteListPtr& InviteInfo)
	{
		if (this->TapType != GUILD_Accept)
			return;
		this->SetAcceptGuild(InviteInfo->invites);
	});

	DeliveryFindGuildTicket = DeliveryFindGuildClass<FB2ResponseFindGuildPtr>::GetInstance().Subscribe([this](const FB2ResponseFindGuildPtr& GuildInfo)
	{
		this->FindGuild(GuildInfo);
	});

	SetOpenState(EUIOpenState::Opened);
	if(UI_Common_Tap_Recommend.IsValid())
		UI_Common_Tap_Recommend->ForcedClickTab();
}

void UB2UIGuildJoinList::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);

	DeliveryReceiveGetRecommendGuildClass<FB2ReciveRecommenGuild>::GetInstance().Unsubscribe(DeliveryReceiveGetRecommendGuildTicket);
	DeliverGetGuildInviteListClass<FB2ResponseGetGuildInviteListPtr>::GetInstance().Unsubscribe(DeliveryReceiveGetAcceptGuildTicket);
	DeliveryFindGuildClass<FB2ResponseFindGuildPtr>::GetInstance().Unsubscribe(DeliveryFindGuildTicket);
}

void UB2UIGuildJoinList::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("Guild_GuildJoin"));
}

void UB2UIGuildJoinList::CacheAssets()
{

	GET_SLOT(UB2Button, BTN_SearchGuild)
	GET_SLOT(UTextBlock, TB_GuildSearch)
	
	GET_SLOT(UEditableTextBox, ETB_SearchGuild)
	GET_SLOT(UB2UIGuildCreate, UIP_GuildCreate)
	GET_SLOT(UVerticalBox, VB_List)
	GET_SLOT(UVerticalBox, VB_AcceptList)

	GET_SLOT(UCanvasPanel, CP_Accept)
	GET_SLOT(UB2RichTextBlock, TB_AcceptMsg)
	GET_SLOT(UB2RichTextBlock, TB_AcceptNumber)
	GET_SLOT(UTextBlock, TB_GuildJoinText)

	GET_SLOT(UB2UICommonTab , UI_Common_Tap_AcceptGuild)
	GET_SLOT(UB2UICommonTab, UI_Common_Tap_Recommend)
	GET_SLOT(UB2UICommonTab, UI_Common_Tap_CreateGuild)
}

void UB2UIGuildJoinList::BindDelegates()
{

	BIND_CLICK_FUNC_TO_BTN(BTN_SearchGuild, &UB2UIGuildJoinList::OnClickSearchGuild);

	REGISTER_UITAB_BINDING(UI_Common_Tap_AcceptGuild.Get(), &UB2UIGuildJoinList::OnClickAcceptGuildTap);
	REGISTER_UITAB_BINDING(UI_Common_Tap_Recommend.Get(), &UB2UIGuildJoinList::OnClickRecommendGuildTap);
	REGISTER_UITAB_BINDING(UI_Common_Tap_CreateGuild.Get(), &UB2UIGuildJoinList::OnClickCreateGuildTap);
}

void UB2UIGuildJoinList::UpdateStaticText()
{
	if (ETB_SearchGuild.IsValid())
		ETB_SearchGuild->SetHintText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildName_Serch")));

	if(TB_GuildSearch.IsValid())
		TB_GuildSearch->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Search")));

	if(TB_AcceptMsg.IsValid())
		TB_AcceptMsg->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_RequestDesc")));

	if (TB_GuildJoinText.IsValid())
		TB_GuildJoinText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_RequestGuild")));
}

void UB2UIGuildJoinList::SetRecommendGuild(const TArray<FB2ResponseGuildInfoPtr>& RecommendGuildArr)
{
	SetRecommendTab();
	for (auto Ele : RecommendGuildArr)
	{
		auto GuildSlot = CreateWidget<UB2UIGuildInfoSlot>(GetOwningPlayer(), RecommendGuild);

		check(GuildSlot)
		GuildSlot->Init();
		GuildSlot->SetGuildInfo(Ele);
		VB_List->AddChild(GuildSlot);

		FGuildDetail	DetailInfo;
		DetailInfo.BindUObject(this, &UB2UIGuildJoinList::EnterGuildDetailInfo);
		GuildSlot->SetGuildDelegate(DetailInfo);
	}
	VB_AcceptList->ClearChildren();
}

void UB2UIGuildJoinList::SetAcceptGuild(const TArray<b2network::B2GuildInvitePtr>& InvitePtr)
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	check(GuildDoc)

	GuildDoc->SetGuildAcceptState(true);
	UIP_GuildCreate->SetVisibility(ESlateVisibility::Hidden);

	CP_Accept->SetVisibility(ESlateVisibility::Visible);
	VB_List->ClearChildren();
	VB_AcceptList->ClearChildren();

	for (auto& El : InvitePtr)
	{
		auto SlotWidget = CreateWidget<UB2UIGuildInfoSlot>(GetWorld(), AcceptGuild);
		check(SlotWidget)

		VB_AcceptList->AddChild(SlotWidget);
		SlotWidget->Init();
		SlotWidget->SetGuildInfo(El->guild);
		FGuildDetail	DetailInfo;
		DetailInfo.BindUObject(this, &UB2UIGuildJoinList::EnterGuildDetailInfo);
		SlotWidget->SetGuildDelegate(DetailInfo);

		FGuildSlotDelect DelectInfo;
		DelectInfo.BindUObject(this, &UB2UIGuildJoinList::DelectGuildSlot);
		SlotWidget->SetGuildDelectDelegate(DelectInfo);

		SlotWidget->SetRemainTime(El->expire_time);
	}

	TB_AcceptNumber->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_InvitemCount")), FText::AsNumber(InvitePtr.Num()), FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetGuildMaxInviteCount())));
	
}

void UB2UIGuildJoinList::FindGuild(const FB2ResponseFindGuildPtr& GuildInfo)
{
	SetRecommendTab();

	auto GuildSlot = CreateWidget<UB2UIGuildInfoSlot>(GetOwningPlayer(), RecommendGuild);

	check(GuildSlot)
	GuildSlot->Init();
	GuildSlot->SetGuildInfo(GuildInfo->guild);
	VB_List->AddChild(GuildSlot);

	FGuildDetail	DetailInfo;
	DetailInfo.BindUObject(this, &UB2UIGuildJoinList::EnterGuildDetailInfo);
	GuildSlot->SetGuildDelegate(DetailInfo);
}

void UB2UIGuildJoinList::SetRecommendTab()
{
	UIP_GuildCreate->OnClose();
	UIP_GuildCreate->SetVisibility(ESlateVisibility::Hidden);
	VB_List->ClearChildren();

	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	check(GuildDoc)

		GuildDoc->SetGuildAcceptState(false);


	CP_Accept->SetVisibility(ESlateVisibility::Hidden);

}

void UB2UIGuildJoinList::OnClickRecommendGuildTap()
{
	data_trader::Retailer::GetInstance().RequestGetRecommendGuild();

	TapType = GUILD_Recommend;
}

void UB2UIGuildJoinList::OnClickAcceptGuildTap()
{
	data_trader::Retailer::GetInstance().RequestGetGuildInviteList();
	TapType = GUILD_Accept;
}

void UB2UIGuildJoinList::OnClickCreateGuildTap()
{
	TapType = GUILD_Create;
	CP_Accept->SetVisibility(ESlateVisibility::Hidden);

	VB_List->ClearChildren();
	UB2GuildMarkInfo* GuildInfo = StaticFindGuildMarkInfo();
	UIP_GuildCreate->SetVisibility(ESlateVisibility::Visible);
	UIP_GuildCreate->OnOpen();
}

void UB2UIGuildJoinList::EnterGuildDetailInfo(int64 GuildID)
{
	data_trader::Retailer::GetInstance().RequestGuildDetailInfo(GuildID);
}

void UB2UIGuildJoinList::DelectGuildSlot(UB2UIGuildInfoSlot* AcceptSlot)
{
	if (VB_AcceptList.IsValid())
		VB_AcceptList->RemoveChild(AcceptSlot);
}

void UB2UIGuildJoinList::OnClickSearchGuild()
{
	data_trader::Retailer::GetInstance().RequestFindGuild(ETB_SearchGuild->GetText().BuildSourceString());
}

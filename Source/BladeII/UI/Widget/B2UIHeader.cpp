#include "B2UIHeader.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUser.h"
#include "B2UIDocPVP1on1Rival.h"
#include "B2UIDocTeamMatch.h"
#include "B2UIDocUICondition.h"
#include "Event.h"
#include "B2UIHeaderResource.h"
#include "B2RedDotManager.h"
#include "B2UIGuildMark.h"
#include "B2UILinkManager.h"
#include "B2LobbyInventory.h"
#include "B2UILobbyMain.h"
#include "B2UILobbyNoticeSlot.h"
#include "B2UIMsgPopupCenter.h"
#include "Retailer.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"
#include "BladeIIUtil.h"
#include "B2PlatformAPI.h"
#include "B2UIHotTimeIcon.h"
#include "B2UIHotTimeBuffPopup.h"
#include "HotTimeBuffManager.h"
#include "B2LobbyGameMode.h"
#include "B2UIDailyNoticePopup.h"

#include "MoviePlayer.h"


#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"

extern void AndroidThunkCpp_SetOrientation(bool bSet);
#endif

#if PLATFORM_IOS
#include "IOSAppDelegate.h"
#include "KakaoTalkPlugin.h"
#endif


UB2UIHeader::UB2UIHeader(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsInDJLegacyHeroMgmt = false;
	DJLegacyHeroMgmtSubMode = EHeroMgmtSubMode::EHMSM_End;

	bScreenVertical = false;

	m_bSubscribed = false;

	m_bModeOpenAnimPlayCheck = false;

	IsAddRestoreRotate = false;

	m_numNicknameClicked = 0;

	m_bSenser = false;
	m_bRotate = false;
	m_bPortRate = false;

	m_bPopupCnt = 0;

	CachedDoneLoadingScreen = false;
	m_LoadMovie = false;
}

void UB2UIHeader::SetIsInDJLegacyHeroMgmt(bool bInDJLegacy, EHeroMgmtSubMode InSubMode)
{
	bIsInDJLegacyHeroMgmt = bInDJLegacy;

	if (bIsInDJLegacyHeroMgmt)
	{
		DJLegacyHeroMgmtSubMode = InSubMode;
		if (DJLegacyHeroMgmtSubMode == EHeroMgmtSubMode::EHMSM_End) {
			DJLegacyHeroMgmtSubMode = EHeroMgmtSubMode::EHMSM_Inventory;
		}

		ChangeLayoutByUIScene(DJLegacyHeroMgmtPageToUIScene(DJLegacyHeroMgmtSubMode)); // UIManager 시스템의 더미 맵핑 씬 이름으로 넣어줌

		SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Title"))); // 하위 모드별로 세팅이 또 될 것임.
	}
	else
	{
		DJLegacyHeroMgmtSubMode = EHeroMgmtSubMode::EHMSM_End;
	}
}

void UB2UIHeader::CacheAssets()
{
	GET_SLOT(UPanelWidget, P_Header);
	GET_SLOT(UPanelWidget, P_User);

	GET_SLOT(UButton, BTN_Back);
	GET_SLOT(UButton, BTN_Back_Text);
	GET_SLOT(UButton, BTN_Home);
	GET_SLOT(UButton, BTN_Mail);
	GET_SLOT(UButton, BTN_Setting);
	GET_SLOT(UButton, BTN_Chat);
	GET_SLOT(UButton, BTN_ScreenRotate);
	GET_SLOT(UButton, BTN_Notice);
	GET_SLOT(UButton, X_BTN_Help);

	GET_SLOT(UButton, BTN_HotTime);
	GET_SLOT(UB2UIHotTimeIcon, UIP_Hottime);
	if (UIP_Hottime.IsValid())
		UIP_Hottime->Init();

	GET_SLOT(UButton, BTN_DailyNotice);
	GET_SLOT(UB2UIHotTimeIcon, UIP_DailyNotice);
	if (UIP_DailyNotice.IsValid())
		UIP_DailyNotice->Init();

	GET_SLOT(UButton, BTN_RestoreRotate);

	GET_SLOT(UB2RichTextBlock, TB_Title);

	GET_SLOT(UButton, BTN_User);
	GET_SLOT(UTextBlock, TB_UserName);
	GET_SLOT(UTextBlock, TB_UserNickName);
	GET_SLOT(UButton, BTN_UserNickname);
	GET_SLOT(UTextBlock, TB_UserGuildName);

	GET_SLOT(UTextBlock, TB_UserLevel);
	GET_SLOT(UTextBlock, TB_UserExpCur);
	GET_SLOT(UTextBlock, TB_UserExpMax);
	GET_SLOT(UTextBlock, TB_UserVipLevel);
	GET_SLOT(UImage, IMG_UserPortrait);

	GET_SLOT(UB2UIGuildMark, UIP_GuildMark);
	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->Init();
	GET_SLOT(UImage, IMG_DefaultMark);

	GET_SLOT(UHorizontalBox, HB_ResourceBox);
	ModeOpenNoticeSlots.AddUninitialized(m_iMaxCountModeOpenNoticeSlot);
	GET_SLOT_BYNAME(UB2UILobbyNoticeSlot, ModeOpenNoticeSlots[0], UIP_ModOpenNotice_0);
	GET_SLOT_BYNAME(UB2UILobbyNoticeSlot, ModeOpenNoticeSlots[1], UIP_ModOpenNotice_1);
	GET_SLOT_BYNAME(UB2UILobbyNoticeSlot, ModeOpenNoticeSlots[2], UIP_ModOpenNotice_2);

	for (auto ModeOpenNoticeSlot : ModeOpenNoticeSlots)
	{
		if (ModeOpenNoticeSlot.IsValid())
		{
			ModeOpenNoticeSlot->Init();
			ModeOpenNoticeSlot->SetVisibility(ESlateVisibility::Collapsed); // 처음은 안보이게 설정
		}
	}
	GET_SLOT(UOverlay, Overlay_ModOpenNotice);
	Overlay_ModOpenNotice->SetVisibility(ESlateVisibility::Collapsed); // 처음은 안보이게 설정

	GET_SLOT(UBorder, X_ForcedBlockHeader);

	SubscribeEvent();
	StartPollingModeOpenNotice_BP();
}

void UB2UIHeader::UpdateStaticText()
{

}

void UB2UIHeader::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Back, &UB2UIHeader::OnClickBtnBack);
	BIND_CLICK_FUNC_TO_BTN(BTN_Back_Text, &UB2UIHeader::OnClickBtnBack);
	BIND_CLICK_FUNC_TO_BTN(BTN_Home, &UB2UIHeader::OnClickBtnHome);
	BIND_CLICK_FUNC_TO_BTN(BTN_Mail, &UB2UIHeader::OnClickBtnMail);
	BIND_CLICK_FUNC_TO_BTN(BTN_Setting, &UB2UIHeader::OnClickBtnSettings);
	BIND_CLICK_FUNC_TO_BTN(BTN_Chat, &UB2UIHeader::OnClickBtnChat);
	BIND_CLICK_FUNC_TO_BTN(BTN_User, &UB2UIHeader::OnClickBtnUser);
	BIND_CLICK_FUNC_TO_BTN(BTN_UserNickname, &UB2UIHeader::OnClickBtnUserNickname);
	BIND_CLICK_FUNC_TO_BTN(BTN_ScreenRotate, &UB2UIHeader::OnClickBtnScreenRotate);
	BIND_CLICK_FUNC_TO_BTN(BTN_RestoreRotate, &UB2UIHeader::OnClickBtnRestoreRotate);
	BIND_CLICK_FUNC_TO_BTN(BTN_Notice, &UB2UIHeader::OnClickBtnNotice);
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Help, &UB2UIHeader::OnClickBtnHelp);
	BIND_CLICK_FUNC_TO_BTN(BTN_HotTime, &UB2UIHeader::OnClickHotTime);
	BIND_CLICK_FUNC_TO_BTN(BTN_DailyNotice, &UB2UIHeader::OnClickDailyNotice);
}

void UB2UIHeader::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Mail.Get()), &UB2UIHeader::RedDotCondition_Mail);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Setting.Get()), &UB2UIHeader::RedDotCondition_Setting);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Chat.Get()), &UB2UIHeader::RedDotCondition_Chat);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Notice.Get()), &UB2UIHeader::RedDotCondition_ModeOpenNotice);
}

void UB2UIHeader::BindDocAuto()
{
	UnbindDoc();

	auto DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{
		DocUser->OnSocialPointChanged.AddUObject(this, &UB2UIHeader::OnChangedSocialPoint);
		DocUser->OnGoldChanged.AddUObject(this, &UB2UIHeader::OnChangedGold);
		DocUser->OnGemChanged.AddUObject(this, &UB2UIHeader::OnChangedGem);
		DocUser->OnFamePointChanged.AddUObject(this, &UB2UIHeader::OnChangedFamePoint);
		DocUser->OnStaminaCurChanged.AddUObject(this, &UB2UIHeader::OnChangedStaminaCur);
		DocUser->OnStaminaMaxChanged.AddUObject(this, &UB2UIHeader::OnChangedStaminaMax);
		DocUser->OnNextTimeStaminaGetChanged.AddUObject(this, &UB2UIHeader::OnChangedNextTimeStaminaGet);
		DocUser->OnUserLevelChanged.AddUObject(this, &UB2UIHeader::OnChangedUserLevel);
		DocUser->OnUserExpCurChanged.AddUObject(this, &UB2UIHeader::OnChangedUserExpCur);
		DocUser->OnUserExpMaxChanged.AddUObject(this, &UB2UIHeader::OnChangedUserExpMax);
		DocUser->OnUserVipLevelChanged.AddUObject(this, &UB2UIHeader::OnChangedUserVipLevel);
		DocUser->OnUserNickNameChanged.AddUObject(this, &UB2UIHeader::OnChangedUserNickName);
		DocUser->OnUserPortraitChanged.AddUObject(this, &UB2UIHeader::OnChangedUserPortrait);
		DocUser->OnUserGuildNameChanged.AddUObject(this, &UB2UIHeader::OnChangedGuildName);
		DocUser->OnUserGuildPortraitChanged.AddUObject(this, &UB2UIHeader::OnChangedUserGuildPortrait);
		DocUser->OnRankPromotionPointChanged.AddUObject(this, &UB2UIHeader::OnChangedHeaderRankPromotionPoint);
		DocUser->OnAdvancedRankPromotionPointChanged.AddUObject(this, &UB2UIHeader::OnChangedHeaderAdvancedRankPromotionPoint);
		DocUser->OnDarkCrystalChanged.AddUObject(this, &UB2UIHeader::OnChangedHeaderDarkCrystal);
		DocUser->OnHeroPieceChanged.AddUObject(this, &UB2UIHeader::OnChangedHeaderHeroPiece);
		DocUser->OnEtherPieceChanged.AddUObject(this, &UB2UIHeader::OnChangedHeaderEtherPiece);
		DocUser->OnShowVersionChanged.AddUObject(this, &UB2UIHeader::OnChangedShowVersion);
		DocUser->OnDimensionPieceChanged.AddUObject(this, &UB2UIHeader::OnChangedDimensionPiece);
	}

	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
	{
		//if (bIsInDJLegacyHeroMgmt)
		//{
		//	EHeroMgmtSubMode SubModeForLayout = (DJLegacyHeroMgmtSubMode != EHeroMgmtSubMode::EHMSM_End) ? DJLegacyHeroMgmtSubMode : EHeroMgmtSubMode::EHMSM_Inventory;
		//	ChangeLayoutByUIScene(DJLegacyHeroMgmtPageToUIScene(SubModeForLayout)); // UIManager 시스템의 더미 맵핑 씬 이름으로 넣어줌
		//}
		//else
		//{ // DJLegacyHeroMgmt 에서도 이게 원하는 값으로 나올 수도 있음.
		//	ChangeLayoutByUIScene(DocUICondition->GetCurrUIScene());
		//}

		SetTitle(DocUICondition->GetHeaderTitle());

		DocUICondition->OnCurrUISceneChanged.AddUObject(this, &UB2UIHeader::OnChangedCurrUIScene);
		DocUICondition->OnHeaderTitleChanged.AddUObject(this, &UB2UIHeader::OnChangedHeaderTitle);
		DocUICondition->OnHideAllLobbyMainUIsChanged.AddUObject(this, &UB2UIHeader::OnChangedHideAllLobbyMainUIs);
	}

	auto DocPvp = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (DocPvp)
	{
		DocPvp->OnMatchPointChanged.AddUObject(this, &UB2UIHeader::OnChangedTagMatchTicketCur);
		DocPvp->OnNextTimeMatchPointGetChanged.AddUObject(this, &UB2UIHeader::OnChangedNextTimeTagMatchTicketGet);
	}

	auto DocTeam = UB2UIDocHelper::GetDocTeamMatch();
	if (DocTeam)
	{
		DocTeam->OnTeamMatchPointChanged.AddUObject(this, &UB2UIHeader::OnChangedTeamMatchTicketCur);
		DocTeam->OnNextTimeTeamMatchPointGetChanged.AddUObject(this, &UB2UIHeader::OnChangedNextTimeTeamMatchTicketGet);
	}
	UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	if (DocGuild)
	{
		DocGuild->OnGuildChatBadgeChanged.AddUObject(this, &UB2UIHeader::OnChangeHeaderChat);
	}
	RefreshUIInfo();

	/*UB2GameInstance* B2GameInstance = GetWorld()->GetGameInstance<UB2GameInstance>();
	B2GameInstance->ResumeDelegate.AddUniqueDynamic(this, &UB2UIHeader::OnPlatformResume);*/
}

void UB2UIHeader::Init()
{
	Super::Init();

	//핫타임 1초에 한번 업데이트
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
		TheWorld->GetTimerManager().SetTimer(UpdateHotTimeHandle, this, &UB2UIHeader::SetHotTime, 1.0f, true);
}

void UB2UIHeader::UnbindDoc()
{
	auto DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{
		DocUser->OnGoldChanged.RemoveAll(this);
		DocUser->OnGemChanged.RemoveAll(this);
		DocUser->OnStaminaCurChanged.RemoveAll(this);
		DocUser->OnStaminaMaxChanged.RemoveAll(this);
		DocUser->OnNextTimeStaminaGetChanged.RemoveAll(this);
		DocUser->OnUserLevelChanged.RemoveAll(this);
		DocUser->OnUserExpCurChanged.RemoveAll(this);
		DocUser->OnUserExpMaxChanged.RemoveAll(this);
		DocUser->OnUserVipLevelChanged.RemoveAll(this);
		DocUser->OnUserNickNameChanged.RemoveAll(this);
		DocUser->OnUserPortraitChanged.RemoveAll(this);
		DocUser->OnUserGuildNameChanged.RemoveAll(this);
		DocUser->OnUserGuildPortraitChanged.RemoveAll(this);
		DocUser->OnRankPromotionPointChanged.RemoveAll(this);
		DocUser->OnAdvancedRankPromotionPointChanged.RemoveAll(this);
		DocUser->OnDarkCrystalChanged.RemoveAll(this);
		DocUser->OnEtherPieceChanged.RemoveAll(this);
		DocUser->OnShowVersionChanged.RemoveAll(this);
	}

	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
	{
		DocUICondition->OnCurrUISceneChanged.RemoveAll(this);
		DocUICondition->OnHeaderTitleChanged.RemoveAll(this);
		DocUICondition->OnHideAllLobbyMainUIsChanged.RemoveAll(this);
	}

	auto DocPvp = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (DocPvp)
	{
		DocPvp->OnMatchPointChanged.RemoveAll(this);
		DocPvp->OnNextTimeMatchPointGetChanged.RemoveAll(this);
	}

	auto DocTeam = UB2UIDocHelper::GetDocTeamMatch();
	if (DocTeam)
	{
		DocTeam->OnTeamMatchPointChanged.RemoveAll(this);
		DocTeam->OnNextTimeTeamMatchPointGetChanged.RemoveAll(this);
	}

	UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	if (DocGuild)
	{
		DocGuild->OnGuildChatBadgeChanged.RemoveAll(this);
	}

	/*UB2GameInstance* B2GameInstance = GetWorld()->GetGameInstance<UB2GameInstance>();
	B2GameInstance->ResumeDelegate.RemoveDynamic(this, &UB2UIHeader::OnPlatformResume);*/
}

void UB2UIHeader::DestroySelf(class UB2UIManager* InUIManager)
{
	UnSubscribeEvent();

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
		TheWorld->GetTimerManager().ClearTimer(UpdateHotTimeHandle);

	Super::DestroySelf(InUIManager);
}

void UB2UIHeader::ShowUserInfo(bool bShow)
{
	ShowUserInfo_BP(bShow);
}

void UB2UIHeader::ShowDailyNotice(bool bShow)
{
	ShowDailyNotice_BP(bShow);
}

void UB2UIHeader::ShowBackBtn(bool bShow)
{
	bIgnoreBackBTN = !bShow;

	ShowBackBtn_BP(bShow);

	if (BTN_Home.IsValid())
	{
		BTN_Home->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (!bShow)
			BTN_Home->RemoveFromParent();
	}
}

void UB2UIHeader::ShowTitle(bool bShow)
{
	ShowTitle_BP(bShow);
}

void UB2UIHeader::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2UIHeader::SetTitle(const FText& InText)
{
	if (TB_Title.IsValid())
		TB_Title->SetText(InText);
}

void UB2UIHeader::SetSocialPoint(int32 InValue, int32 InValueMax, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::SocialPoint);

	if (pUI)
		pUI->SetAmount(InValue, InValueMax, bFromInit);
}

void UB2UIHeader::SetGold(int32 InValue, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::Gold);

	if (pUI)
		pUI->SetAmount(InValue, -1, bFromInit);
}

void UB2UIHeader::SetGem(int32 InValue, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::Gem);

	if (pUI)
		pUI->SetAmount(InValue, -1, bFromInit);
}

void UB2UIHeader::SetStamina(int32 InValueCur, int32 InValueMax, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::BladePoint);

	if (pUI)
		pUI->SetAmount(InValueCur, InValueMax, bFromInit);
}

void UB2UIHeader::SetNextTimeStaminaGet(int32 InValue)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::BladePoint);

	if (pUI)
		pUI->SetRemainTime(FText::FromString(UB2UIManager::FormatSecondsAsTime(InValue, TEXT("mm:ss"))));
}

void UB2UIHeader::SetUserName(const FText& InText)
{
	if (TB_UserName.IsValid())
		TB_UserName->SetText(InText);
}

void UB2UIHeader::SetUserNickName()
{
	if (TB_UserNickName.IsValid())
		TB_UserNickName->SetText(FText::FromString(BladeIIGameImpl::GetClientDataStore().GetUserNickName()));
}

void UB2UIHeader::SetHotTime()
{
	if (UIP_Hottime.IsValid())
		UIP_Hottime->ActiveEffect_BP(HotTimeBuffManager::GetInstance().IsActiveHotTimeBuff(EHotTimeEventType::All));
}

void UB2UIHeader::SetDailyNotice()
{
	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		if (UIP_DailyNotice.IsValid())
			UIP_DailyNotice->ActiveEffect_BP(DocStore->HasReceivedPeriodicPackage());
	}
}

void UB2UIHeader::SetUserGuild()
{
	const FUserGuildInfo& GuildInfo = BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo();

	if (BladeIIGameImpl::GetClientDataStore().GetUserGuildID())
	{
		if (UIP_GuildMark.IsValid()) {
			UIP_GuildMark->SetGuildMark(GuildInfo.Mark_Index, GuildInfo.MarkColor_Index, GuildInfo.MarkBG_Index, GuildInfo.MarkBGColor_Index);
			UIP_GuildMark->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		if (TB_UserGuildName.IsValid()) {
			TB_UserGuildName->SetText(FText::FromString(GuildInfo.GuildName));
		}

		if (IMG_DefaultMark.IsValid()) {
			IMG_DefaultMark->SetVisibility(ESlateVisibility::Hidden); // GuildMark 랑 같은 위치.
		}
	}
	else
	{
		if (IMG_DefaultMark.IsValid()) {
			IMG_DefaultMark->SetVisibility(ESlateVisibility::HitTestInvisible); // GuildMark 랑 같은 위치.
		}
		if (UIP_GuildMark.IsValid()) {
			UIP_GuildMark->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TB_UserGuildName.IsValid()) {
			TB_UserGuildName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_NoGuild")));
		}
	}
}

void UB2UIHeader::SetUserLevel(int32 InValue)
{
	if (TB_UserLevel.IsValid())
		TB_UserLevel->SetText(FText::AsNumber(InValue));
}

void UB2UIHeader::SetUserExp(int32 InValueCur, int32 InValueMax)
{
	if (TB_UserExpCur.IsValid())
		TB_UserExpCur->SetText(FText::AsNumber(InValueCur));

	if (TB_UserExpMax.IsValid())
		TB_UserExpMax->SetText(FText::AsNumber(InValueMax));
}

void UB2UIHeader::SetUserVipLevel(int32 InValue)
{
	if (TB_UserVipLevel.IsValid())
		TB_UserVipLevel->SetText(FText::AsNumber(InValue));
}

void UB2UIHeader::SetUserPortrait(UMaterialInterface* InMaterial)
{
	if (IMG_UserPortrait.IsValid())
		IMG_UserPortrait->SetBrushFromMaterial(InMaterial);
}

void UB2UIHeader::SetHeroTowerTicket(int32 InValue, int32 InValueMax, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::HeroTowerTicket);

	if (pUI)
		pUI->SetAmount(InValue, InValueMax, bFromInit);
}

void UB2UIHeader::SetHeroPiece(int32 InValue, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::HeroPiece);

	if (pUI)
		pUI->SetAmount(InValue, -1, bFromInit);
}

void UB2UIHeader::SetUserBrevetPoint(int32 InValue, bool bFromInit)
{
	if (UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::Brevet))
		pUI->SetAmount(InValue, -1, bFromInit);
}
void UB2UIHeader::SetUserAdvancedBrevetPoint(int32 InValue, bool bFromInit)
{
	if (UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::AdvancedBrevet))
		pUI->SetAmount(InValue, -1, bFromInit);
}

void UB2UIHeader::SetTagMatchTicket(int32 InValueCur, int32 InValueMax, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::TagMatchTicket);

	if (pUI)
		pUI->SetAmount(InValueCur, InValueMax, bFromInit);
}

void UB2UIHeader::SetNextTimeTagMatchTicketGet(int32 InValue)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::TagMatchTicket);

	if (pUI)
		pUI->SetRemainTime(FText::FromString(UB2UIManager::FormatSecondsAsTime(InValue, TEXT("mm:ss"))));
}

void UB2UIHeader::SetTeamMatchTicket(int32 InValueCur, int32 InValueMax, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::TeamMatchTicket);

	if (pUI)
		pUI->SetAmount(InValueCur, InValueMax, bFromInit);
}

void UB2UIHeader::SetNextTimeTeamMatchTicketGet(int32 InValue)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::TeamMatchTicket);

	if (pUI)
		pUI->SetRemainTime(FText::FromString(UB2UIManager::FormatSecondsAsTime(InValue, TEXT("mm:ss"))));
}

void UB2UIHeader::SetControlMatchTicket(int32 InValue, int32 InValueMax, bool bFromInit)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::ControlMatchTicket);

	if (pUI)
		pUI->SetAmount(InValue, InValueMax, bFromInit);
}

void UB2UIHeader::SetUserDarkCrystal(int32 InValue, bool bFromInit /*=false*/)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::DarkCrystal);

	if (pUI)
		pUI->SetAmount(InValue, -1, bFromInit);
}

void UB2UIHeader::SetEtherPiece(int32 InValue, bool bFromInit /*= false*/)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::EtherPiece);

	if (pUI)
		pUI->SetAmount(InValue, -1, bFromInit);
}

void UB2UIHeader::SetFamePoint(int32 InValue, bool bFromInit /*= false*/)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::FamePoint);

	if (pUI)
		pUI->SetAmount(InValue, -1, bFromInit);
}

void UB2UIHeader::SetDimensionPiece(int32 InValue, bool bFromInit /*= false*/)
{
	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::DimensionPiece);

	if (pUI)
		pUI->SetAmount(InValue, -1, bFromInit);
}

void UB2UIHeader::RefreshUIInfo()
{
	// 씬전환될때 정보갱신해준다. 골드보석등은 틱에서 해주니깐 안해줘도 되는데. 그냥 한번더 하자
	if (auto* DocUser = UB2UIDocHelper::GetDocUser())
	{
		SetSocialPoint(DocUser->GetSocialPoint(), DocUser->GetSocialPointMax(), true);
		SetGold(DocUser->GetGold(), true);
		SetGem(DocUser->GetGem(), true);
		SetStamina(DocUser->GetStaminaCur(), DocUser->GetStaminaMax(), true);
		SetFamePoint(DocUser->GetFamePoint(), true);
		SetNextTimeStaminaGet(DocUser->GetNextTimeStaminaGet());
		SetUserLevel(DocUser->GetUserLevel());
		SetUserExp(DocUser->GetUserExpCur(), DocUser->GetUserExpMax());
		SetUserVipLevel(DocUser->GetUserVipLevel());
		SetUserNickName();
		SetHotTime();
		SetDailyNotice();
		SetUserPortrait(DocUser->GetUserPortrait());
		SetUserGuild();
		SetUserBrevetPoint(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE));
		SetUserAdvancedBrevetPoint(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE));
		SetUserDarkCrystal(DocUser->GetDarkCrystal(), true);
		SetHeroPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE), true);
		SetEtherPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ETHER_PIECE), true);
		SetDimensionPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE), true);
	}

	if (UB2UIDocHeroTower* pDocHeroTower = UB2UIDocHelper::GetDocHeroTower())
	{
		SetHeroTowerTicket(pDocHeroTower->GetDailyLeftFreeTryCount(), BladeIIGameImpl::GetClientDataStore().GetMaxHeroTowerTicketCount(), true);
	}

	// 일대일, 팀매치, 점령전 티켓정보 추가되면 작성

	if (UB2UIDocPVP1on1Rival* pDocTagMatch = UB2UIDocHelper::GetDocPVP1on1Rival())
	{
		SetTagMatchTicket(pDocTagMatch->GetMatchPoint(), BladeIIGameImpl::GetClientDataStore().GetMaxPvpMatchPoint(), true);
	}

	if (UB2UIDocTeamMatch* pDocTeamMatch = UB2UIDocHelper::GetDocTeamMatch())
	{
		SetTeamMatchTicket(pDocTeamMatch->GetTeamMatchPoint(), BladeIIGameImpl::GetClientDataStore().GetMaxTeamMatchPoint(), true);
	}

	UpdateUIVisibleVerticalMode(bScreenVertical);

	if (auto* DocChapter = UB2UIDocHelper::GetDocUICondition())
	{
		EUIScene CurrUIScene = UB2UIManager::GetInstance()->GetCurrUIScene();
		const bool bCombineMenuOpen = DocChapter->GetIsCombineMenu();

		if (CurrUIScene == EUIScene::LobbyMain)
			OnUpdateCombineMenu(bCombineMenuOpen);
	}

	UpdateHelpButton();
}

void UB2UIHeader::ApplyAddAmountAction(ERewardType rewardType)
{
	EUIHeaderResourceType TargetUIType = EUIHeaderResourceType::None;

	switch (rewardType)
	{
	case ERewardType::Gold:
		TargetUIType = EUIHeaderResourceType::Gold;
		break;
	case ERewardType::Gem:
		TargetUIType = EUIHeaderResourceType::Gem;
		break;
	case ERewardType::BladePoint:
		TargetUIType = EUIHeaderResourceType::BladePoint;
		break;
	case ERewardType::Hero_Piece:
		TargetUIType = EUIHeaderResourceType::HeroPiece;
		break;
	case ERewardType::Rank_Promotion_Point:
		TargetUIType = EUIHeaderResourceType::Brevet;
		break;
 	case ERewardType::Advanced_Rank_Promotion_Point:
 		TargetUIType = EUIHeaderResourceType::AdvancedBrevet;
 		break;
	case ERewardType::Fame_Point:
		TargetUIType = EUIHeaderResourceType::FamePoint;
		break;
	}

	if (TargetUIType != EUIHeaderResourceType::None)
	{
		UB2UIHeaderResource* pUI = GetHeaderResourceUI(TargetUIType);

		if (pUI)
			pUI->PlayAnimationEvent_AddAmount();
	}
}

void UB2UIHeader::SetForceVisibileMailRedDot(bool bVisible)
{
	ForceSetVisibleRedDot(BTN_Mail.Get(), bVisible);
}

void UB2UIHeader::UpdateUIVisibleVerticalMode(bool bVerticalMode)
{
	if (P_Header.IsValid())
		P_Header->SetVisibility(bVerticalMode ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (P_User.IsValid())
	{
		if (bVerticalMode == false)	// 보여야할때
		{
			if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
			{
				if (EUIScene::LobbyMain == UIManager->GetCurrUIScene())
					P_User->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
		else
		{
			P_User->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	//if (BTN_RestoreRotate.IsValid())
	//{
	if (bVerticalMode)
	{
		B2AndroidBackManager::GetInstance()->AddBackWidget(this, FName(TEXT("HEADER")));
		IsAddRestoreRotate = true;
		//BTN_RestoreRotate->SetVisibility(ESlateVisibility::Visible);
		BTN_RestoreRotate->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (IsAddRestoreRotate)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(this, FName(TEXT("HEADER")));
			IsAddRestoreRotate = false;
		}
		//BTN_RestoreRotate->SetVisibility(ESlateVisibility::Collapsed);
		BTN_RestoreRotate->SetVisibility(ESlateVisibility::Hidden);
	}
	//}

}

void UB2UIHeader::OnReceiveCheckModeOpen(const FB2ResponseCheckModeOpenPtr& fb2r_CheckModeOpen)
{
	std::vector<EModeOpenType> vecModeOpenType;

	TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;

	bool bAssultIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(fb2r_CheckModeOpen->assault_open_period_infos, OpenTimes);
	if (bAssultIsOpen)
	{
		vecModeOpenType.push_back(EModeOpenType::EModeOpenType_Occupy);
	}

	bool bRaidIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(fb2r_CheckModeOpen->raid_open_period_infos, OpenTimes);
	if (bRaidIsOpen)
	{
		vecModeOpenType.push_back(EModeOpenType::EModeOpenType_Raid);
	}

	int64 iUserGuildID = BladeIIGameImpl::GetClientDataStore().GetUserGuildID();
	if (iUserGuildID)
	{
		if (fb2r_CheckModeOpen->guild_battle_state == b2network::B2GuildBattleStateType::BATTLE)
		{
			vecModeOpenType.push_back(EModeOpenType::EModeOpenType_Guild_Enter);
		}
		else if (fb2r_CheckModeOpen->guild_battle_state == b2network::B2GuildBattleStateType::READY)
		{
			vecModeOpenType.push_back(EModeOpenType::EModeOpenType_Guild_Request);
		}
	}

	int32 vecModeOpenTypeSize = vecModeOpenType.size();

	if (vecModeOpenTypeSize == 0)
	{
		SetModeOpenNoticeRedDot(false);
		DoMarkRedDot();
		return;
	}

	int32 iIndex = 0;

	for (auto ModeOpenNoticeSlot : ModeOpenNoticeSlots)
	{
		if (ModeOpenNoticeSlot.IsValid())
		{
			if (iIndex < vecModeOpenTypeSize)
			{
				ModeOpenNoticeSlot->SetModeOpenType((int)vecModeOpenType[iIndex]);
			}
			else
			{
				ModeOpenNoticeSlot->EnableLobbyNoticeSlot_BP(false);
			}
		}

		iIndex++;
	}

	if (m_bModeOpenAnimPlayCheck == true)
	{
		if (Overlay_ModOpenNotice->GetVisibility() == ESlateVisibility::Collapsed)
		{
			AnimModeOpenNoticeSlots_BP(true);
		}

		m_bModeOpenAnimPlayCheck = false;
	}
	else
	{
		SetModeOpenNoticeRedDot(true);
	}

	DoMarkRedDot();
}

bool UB2UIHeader::GetModeOpenNoticeRedDot()
{
	auto* DocSome = UB2UIDocHelper::GetDocSome();

	if (!DocSome) { return false; }

	return DocSome->GetbModeOpenNoticeRedDotCheck();
}

void UB2UIHeader::SetModeOpenNoticeRedDot(bool bEnable)
{
	auto* DocSome = UB2UIDocHelper::GetDocSome();

	if (!DocSome) { return; }

	DocSome->SetbModeOpenNoticeRedDotCheck(bEnable);
}

void UB2UIHeader::SetForcedBlockHeader(bool isBlock)
{
	if (X_ForcedBlockHeader.IsValid())
	{
		X_ForcedBlockHeader->SetVisibility(isBlock ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2UIHeader::SetForcedBlockHeader(bool isBlock, FLinearColor NewColor)
{
	if (X_ForcedBlockHeader.IsValid())
	{
		X_ForcedBlockHeader->SetVisibility(isBlock ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		X_ForcedBlockHeader->SetBrushColor(NewColor);
	}
}

void UB2UIHeader::PollingModeOpenNotice_BP()
{
	m_bModeOpenAnimPlayCheck = false;

	data_trader::Retailer::GetInstance().RequestCheckModeOpen();
}

void UB2UIHeader::ChangeLayoutByUIScene(EUIScene InScene)
{
	m_CurrentSceneAOS = InScene;

	switch (InScene)
	{
	case EUIScene::LobbyMain:
		ShowUserInfo(true);
		ShowDailyNotice(true);
		ShowBackBtn(false);
		ShowTitle(false);
		m_bModeOpenAnimPlayCheck = false;
		SetModeOpenNoticeRedDot(true);
		data_trader::Retailer::GetInstance().RequestCheckModeOpen();
		break;

	case EUIScene::Chapter:
	case EUIScene::BattleStageInfo:
		ShowUserInfo(false);
		ShowDailyNotice(false);
		ShowBackBtn(true);
		ShowTitle(true);
		break;

	default:
		ShowUserInfo(false);
		ShowDailyNotice(false);
		ShowBackBtn(true);
		ShowTitle(true);
		break;
	}

	// 씬바뀔땐 숨긴거 해제(숨기는순간 씬전환일어나면 헤더가 사라짐ㅋ)
	if (m_bIsHideHeader)
	{
		HideHeader(false);
		m_bIsHideHeader = false;
	}

	// 씬별로 보여줄 재화UI 체크
	CheckResourceParts(InScene);
	RefreshUIInfo();
}

void UB2UIHeader::CheckResourceParts(EUIScene InScene)
{
	for (FApplyheaderResourceInfo& ResourceInfo : HeaderResourceSelectInfo)
	{
		for (EUIScene CheckScene : ResourceInfo.ApplyScenes)
		{
			if (InScene == CheckScene)
			{
				// 리소스인포에 있는것들로 만듬
				CreateResourcePartsUI(ResourceInfo.ToCreateResources);
				return;
			}
		}
	}

	// 못찾았으면 베이스인포에있는걸로 만듬
	CreateResourcePartsUI(BaseResources);
}

void UB2UIHeader::CreateResourcePartsUI(TArray<TSubclassOf<class UB2UIHeaderResource>> ResourcePartsUIClasses)
{
	// 만들기전에 있던거 제거
	for (UB2UIHeaderResource* pDeleteUI : m_arResourcePartsUI)
	{
		if (pDeleteUI)
			pDeleteUI->DestroySelf(NULL);
	}

	m_arResourcePartsUI.Empty();

	//bool bFirstItem = true;
	for (TSubclassOf<class UB2UIHeaderResource>& ResourcePartsUIClass : ResourcePartsUIClasses)
	{
		ensure(ResourcePartsUIClass);

		if (!ResourcePartsUIClass)
			continue;

		UB2UIHeaderResource* pUIP = CreateWidget<UB2UIHeaderResource>(GetOwningPlayer(), ResourcePartsUIClass);
		HB_ResourceBox->AddChild(pUIP);
		if (pUIP)
			pUIP->Init();

		// 멤버 어레이어 저장
		m_arResourcePartsUI.Add(pUIP);
	}
}

void UB2UIHeader::RotateScreen(bool bRotateToDefault)
{
	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome && DocSome->GetIsCharacterObserveScene() == true)
	{
		if (bRotateToDefault)
		{
			/*
			#if PLATFORM_ANDROID
			AndroidThunkCpp_SetOrientation(false);
			#elif PLATFORM_IOS

			[[IOSAppDelegate GetDelegate] performSelectorOnMainThread:@selector(RotateOrientationLandscape) withObject:nil waitUntilDone : NO];
			#endif*/
			bScreenVertical = false;
		}
		else
		{
			/*
			#if PLATFORM_ANDROID
			AndroidThunkCpp_SetOrientation(true);
			#elif PLATFORM_IOS
			[[IOSAppDelegate GetDelegate] performSelectorOnMainThread:@selector(RotateOrientationPortrait) withObject:nil waitUntilDone : NO];
			#endif*/
			bScreenVertical = true;
		}

		UpdateUIVisibleVerticalMode(bScreenVertical);
		LobbyCharObserveChangeScreenClass<bool>::GetInstance().Signal(bScreenVertical);
	}
}

void UB2UIHeader::SubscribeEvent()
{
	if (m_bSubscribed == false)
	{
		CAPTURE_UOBJECT(UB2UIHeader);

		DeliveryCheckModeOpenTicket = DeliveryCheckModeOpenClass<FB2ResponseCheckModeOpenPtr>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(DeliveryCheckModeOpen, FB2ResponseCheckModeOpenPtr fb2r_CheckModeOpen)
			Capture->OnReceiveCheckModeOpen(fb2r_CheckModeOpen);
		END_CAPTURE_OBJECT()
			);

		OnUpdateCombineMenuTicket = OnUpdateCombineMenuClass<bool>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(OnUpdateCombineMenu, bool bVisible)
			Capture->OnUpdateCombineMenu(bVisible);
		END_CAPTURE_OBJECT()
			);

		m_bSubscribed = true;
	}
}

void UB2UIHeader::OnUpdateCombineMenu(bool bVisible)
{
	if (BTN_ScreenRotate.IsValid())
	{
		BTN_ScreenRotate->SetIsEnabled(!bVisible);
	}
}

void UB2UIHeader::UpdateHelpButton()
{
	if (X_BTN_Help.IsValid())
	{
		EUIScene CurUIScene = EUIScene::None;
		if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
		{
			CurUIScene = UIManager->GetCurrUIScene();
		}

		if (
			CurUIScene == EUIScene::Inventory ||
			CurUIScene == EUIScene::Chapter ||
			CurUIScene == EUIScene::PVPOneOnOne ||
			CurUIScene == EUIScene::TeamMatchUI ||
			CurUIScene == EUIScene::ControlMain ||
			CurUIScene == EUIScene::Skill ||
			CurUIScene == EUIScene::Brevet ||
			CurUIScene == EUIScene::Wing ||
			CurUIScene == EUIScene::Relic ||
			CurUIScene == EUIScene::EtherMain ||
			CurUIScene == EUIScene::CounterAttackDGMain ||
			CurUIScene == EUIScene::HeroTowerReady ||
			CurUIScene == EUIScene::GuildMain ||
			CurUIScene == EUIScene::SummonItemStore ||
			CurUIScene == EUIScene::Store ||
			CurUIScene == EUIScene::Mission ||
			CurUIScene == EUIScene::Friend ||
			CurUIScene == EUIScene::RestPointMain ||
			CurUIScene == EUIScene::CollectBookSelectClass ||
			CurUIScene == EUIScene::MagicStore ||
			CurUIScene == EUIScene::HallOfFame||
			CurUIScene == EUIScene::DonationEvent ||
			CurUIScene == EUIScene::DimensionMain ||
			CurUIScene == EUIScene::Totem ||
			CurUIScene == EUIScene::Costume
			)
		{
			X_BTN_Help->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			X_BTN_Help->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIHeader::OnClickBtnHelp()
{
	FString HelpURL = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("GuideURL_Menu")).ToString();

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		EUIScene CurUIScene = UIManager->GetCurrUIScene();
		HelpURL = BladeIIGameImpl::GetClientDataStore().GetWebURL(CurUIScene);
	}

	B2P_ShowOpenURL(HelpURL);
}

void UB2UIHeader::OnClickHotTime()
{
	UB2UIManager::GetInstance()->OpenUI<UB2UIHotTimeBuffPopup>(UIFName::HotTimePopup);
}

void UB2UIHeader::OnClickDailyNotice()
{
	UB2UIManager::GetInstance()->OpenUI<UB2UIDailyNoticePopup>(UIFName::DailyNoticePopup);
}

bool UB2UIHeader::IsAnimPlayCheck(const FName& animName)
{
	if (UWidgetAnimation* Anim = GetAnimation(animName))
	{
		return IsAnimationPlaying(Anim) ? true : false;
	}

	return false;
}

void UB2UIHeader::UnSubscribeEvent()
{
	DeliveryCheckModeOpenClass<FB2ResponseCheckModeOpenPtr>::GetInstance().Unsubscribe(DeliveryCheckModeOpenTicket);
	OnUpdateCombineMenuClass<bool>::GetInstance().Unsubscribe(OnUpdateCombineMenuTicket);
	DeliveryCheckModeOpenTicket = 0;
	OnUpdateCombineMenuTicket = 0;

	m_bSubscribed = false;
}

void UB2UIHeader::CloseWidgetDelegate()
{
	if (IsAddRestoreRotate)
	{
		OnClickBtnRestoreRotate();
	}
	else
	{
		OnClickBtnBack();
	}
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIHeader::OnClickBtnBack()
{
	if (bIgnoreBackBTN)
		//버튼 연타시에 버튼이 없는 Scene임에도 백히스토리로 가는 것을 방지
		return;

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		EUIScene PrevUIScene = UIManager->GetCurrUIScene();

		PlaySoundOnBackBtn(PrevUIScene);

		//UIScene History에 의해 자동으로 Back된다. DJLegacy HeroMgmt 모드인 경우도 처리될 것.
		UIManager->ChangeUISceneBack();
	}
}

void UB2UIHeader::OnClickBtnHome()
{
	if (bIgnoreBackBTN)
		//버튼 연타시에 버튼이 없는 Scene임에도 백히스토리로 가는 것을 방지
		return;

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		if (auto* DocChapter = UB2UIDocHelper::GetDocUICondition())
		{
			DocChapter->SetIsCombineMenu(false);
		}

		UIManager->ClearUISceneHistory();
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
		UIManager->ChangeUIScene(EUIScene::LobbyMain);
	}
}

//#include "Retailer.h"
void UB2UIHeader::OnClickBtnMail()
{
	FB2UILinkManager::LinkUIScene(EUIScene::Mail);
	//data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(MailTabIndex::EMailTab_Gift));
}

void UB2UIHeader::OnClickBtnSettings()
{
	if (UB2UIManager* UIMgrInst = UB2UIManager::GetInstance())
		UIMgrInst->OpenUI(UIFName::Setting);
}

void UB2UIHeader::DelayFunc_OpenUI(FName UIName, float Delay)
{
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([UIName]
	{
		if (UB2UIManager* UIMgrInst = UB2UIManager::GetInstance())
			UIMgrInst->OpenUI(UIName);
	});

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, Delay, false);
}

void UB2UIHeader::OnClickBtnChat()
{
	DelayFunc_OpenUI(UIFName::Chatting, 0.3f);
}

void UB2UIHeader::OnClickBtnUser()
{
	UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	if (LobbyMainUIPage)
	{
		LobbyMainUIPage->ForcedChangeFocusCharacter();
	}
}

void UB2UIHeader::OnClickBtnUserNickname()
{
	auto* doc = UB2UIDocHelper::GetDocUser();
	if (doc)
	{
		auto flag = !doc->GetShowVersion();

		if (flag)
		{
			if (m_numNicknameClicked++ > 5)
			{
				doc->SetShowVersion(true);

				m_numNicknameClicked = 0;
			}
		}
		else
		{
			doc->SetShowVersion(false);
		}
	}
}

void UB2UIHeader::OnClickBtnScreenRotate()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UIHeader::OnClickBtnScreenRotate"));

	//if (bScreenVertical == false)
	//	RotateScreen(false);

	//B2P_ScreenCapture();
}

void UB2UIHeader::OnClickBtnRestoreRotate()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UIHeader::OnClickBtnRestoreRotate"));

	//if (bScreenVertical)
	//	RotateScreen(true);

	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient != nullptr)
	{
		FOutputDevice& Ar = (FOutputDevice&)(*GLog);
		ViewportClient->HandleScreenshotCommand(TEXT("shot"), Ar);

		//B2P_ScreenCapture();
	}
}

void UB2UIHeader::OnClickBtnNotice()
{
	if (IsAnimPlayCheck(TEXT("ANIM_ModeNotice_Open")) || IsAnimPlayCheck(TEXT("ANIM_ModeNotice_Close")))
		return;

	if (Overlay_ModOpenNotice->GetVisibility() == ESlateVisibility::Collapsed)
	{
		SetModeOpenNoticeRedDot(false);
		m_bModeOpenAnimPlayCheck = true;
		data_trader::Retailer::GetInstance().RequestCheckModeOpen();
	}
	else if (Overlay_ModOpenNotice->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		AnimModeOpenNoticeSlots_BP(false);
	}
}

//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIHeader::OnChangedSocialPoint(class UB2UIDocBase* Sender, int32 Point, int32 PrevPoint)
{
	if (PrevPoint < Point)
		ApplyAddAmountAction(ERewardType::Social_Point);
	if (auto* DocUser = UB2UIDocHelper::GetDocUser())
		SetSocialPoint(Point, DocUser->GetSocialPointMax());
}

void UB2UIHeader::OnChangedGold(class UB2UIDocBase* Sender, int32 Gold, int32 PrevGold)
{
	if (PrevGold < Gold)
		ApplyAddAmountAction(ERewardType::Gold);
	SetGold(Gold);
}
void UB2UIHeader::OnChangedGem(class UB2UIDocBase* Sender, int32 Gem, int32 PrevGem)
{
	if (PrevGem < Gem)
		ApplyAddAmountAction(ERewardType::Gem);
	SetGem(Gem);
}

void UB2UIHeader::OnChangedFamePoint(class UB2UIDocBase* Sender, int32 FamePoint, int32 PrevFamePoint)
{
	if (PrevFamePoint < FamePoint)
		ApplyAddAmountAction(ERewardType::Fame_Point);
	SetFamePoint(FamePoint);
}

void UB2UIHeader::OnChangedStaminaCur(class UB2UIDocBase* Sender, int32 StaminaCur, int32 PrevStaminaCur)
{
	// 블레이드 포인트는 시간마다 추가 될 때는 애니메이션을 출력하지 않아야 되므로, 
	// UIheader에서 처리하지않음.
	if (auto* DocUser = UB2UIDocHelper::GetDocUser())
		SetStamina(StaminaCur, DocUser->GetStaminaMax());
}

void UB2UIHeader::OnChangedStaminaMax(class UB2UIDocBase* Sender, int32 StaminaMax, int32 PrevStaminaMax)
{
	if (auto* DocUser = UB2UIDocHelper::GetDocUser())
		SetStamina(DocUser->GetStaminaCur(), StaminaMax);
}

void UB2UIHeader::OnChangedNextTimeStaminaGet(class UB2UIDocBase* Sender, int32 NextTimeStaminaGet, int32 PrevNextTimeStaminaGet)
{
	SetNextTimeStaminaGet(NextTimeStaminaGet);
}

void UB2UIHeader::OnChangedTagMatchTicketCur(class UB2UIDocBase* Sender, int32 TagTicketCur, int32 PrevTagTicketCur)
{
	if (auto* DocPvpMatch = UB2UIDocHelper::GetDocPVP1on1Rival())
		SetTagMatchTicket(TagTicketCur, BladeIIGameImpl::GetClientDataStore().GetMaxPvpMatchPoint());
}

void UB2UIHeader::OnChangedNextTimeTagMatchTicketGet(class UB2UIDocBase* Sender, int32 NextTimeTagTicketGet, int32 PrevNextTimeTagTicketGet)
{
	SetNextTimeTagMatchTicketGet(NextTimeTagTicketGet);
}

void UB2UIHeader::OnChangedTeamMatchTicketCur(class UB2UIDocBase* Sender, int32 TeamTicketCur, int32 PrevTeamTicketur)
{
	if (auto* DocTeamMatch = UB2UIDocHelper::GetDocTeamMatch())
		SetTeamMatchTicket(TeamTicketCur, BladeIIGameImpl::GetClientDataStore().GetMaxTeamMatchPoint());
}

void UB2UIHeader::OnChangedNextTimeTeamMatchTicketGet(class UB2UIDocBase* Sender, int32 NextTimeTeamTicketGet, int32 PrevNextTimeTeamTicketGet)
{
	SetNextTimeTeamMatchTicketGet(NextTimeTeamTicketGet);
}

void UB2UIHeader::OnChangedUserLevel(class UB2UIDocBase* Sender, int32 UserLevel, int32 PrevUserLevel)
{
	SetUserLevel(UserLevel);
}

void UB2UIHeader::OnChangedUserExpCur(class UB2UIDocBase* Sender, int32 UserExpCur, int32 PrevUserExpCur)
{
	if (auto* DocUser = UB2UIDocHelper::GetDocUser())
		SetUserExp(UserExpCur, DocUser->GetUserExpMax());
}

void UB2UIHeader::OnChangedUserExpMax(class UB2UIDocBase* Sender, int32 UserExpMax, int32 PrevUserExpMax)
{
	if (auto* DocUser = UB2UIDocHelper::GetDocUser())
		SetUserExp(DocUser->GetUserExpCur(), UserExpMax);
}

void UB2UIHeader::OnChangedUserVipLevel(class UB2UIDocBase* Sender, int32 UserVipLevel, int32 PrevUserVipLevel)
{
	SetUserVipLevel(UserVipLevel);
}

void UB2UIHeader::OnChangedUserNickName(class UB2UIDocBase* Sender, FText UserNickName, FText PrevUserNickName)
{
	SetUserNickName();
}

void UB2UIHeader::OnChangedUserPortrait(class UB2UIDocBase* Sender, UMaterialInterface* UserPortrait, UMaterialInterface* PrevUserPortrait)
{
	SetUserPortrait(UserPortrait);
}

void UB2UIHeader::OnChangedCurrUIScene(class UB2UIDocBase* Sender, EUIScene CurrUIScene, EUIScene PrevCurrUIScene)
{
	ChangeLayoutByUIScene(CurrUIScene);
}

void UB2UIHeader::OnChangedHeaderTitle(class UB2UIDocBase* Sender, FText HeaderTitle, FText PrevHeaderTitle)
{
	SetTitle(HeaderTitle);
}

void UB2UIHeader::OnChangedHideAllLobbyMainUIs(class UB2UIDocBase* Sender, bool HideAllLobbyMainUIs, bool PrevHideAllLobbyMainUIs)
{
	//로비에서 10초간, 아무 행동도 하지 않으면 모든 UI를 숨기라고 한다.
	HideHeader(HideAllLobbyMainUIs);

	m_bIsHideHeader = HideAllLobbyMainUIs;
}

bool UB2UIHeader::RedDotCondition_Mail()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mail();
}

bool UB2UIHeader::RedDotCondition_Chat()
{

	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Chat();
}

bool UB2UIHeader::RedDotCondition_Setting()
{

	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Setting();
}

bool UB2UIHeader::RedDotCondition_ModeOpenNotice()
{
	auto* DocSome = UB2UIDocHelper::GetDocSome();

	if (!DocSome) { return false; }

	return DocSome->GetbModeOpenNoticeRedDotCheck();
}

UB2UIHeaderResource* UB2UIHeader::GetHeaderResourceUI(EUIHeaderResourceType UItype)
{
	for (UB2UIHeaderResource* pUI : m_arResourcePartsUI)
	{
		if (pUI == nullptr)
			continue;
		if (pUI->ResourceUIType == UItype)
			return pUI;
	}

	return NULL;
}

void UB2UIHeader::OnChangedGuildName(class UB2UIDocBase* Sender, FText GuildName, FText PrevGuildName)
{
}

void UB2UIHeader::OnChangedUserGuildPortrait(class UB2UIDocBase* Sender, UMaterialInterface* UserGuildPortrait, UMaterialInterface* PrevUserGuildPortrait)
{
}

void UB2UIHeader::OnChangedHeaderHeroPiece(class UB2UIDocBase* Sender, int32 Cur, int32 Prev)
{
	if (Prev < Cur)
		ApplyAddAmountAction(ERewardType::Hero_Piece);
	SetHeroPiece(Cur);
}

void UB2UIHeader::OnChangedHeaderRankPromotionPoint(class UB2UIDocBase* Sender, int32 Cur, int32 Prev)
{
	if (Prev < Cur)
		ApplyAddAmountAction(ERewardType::Rank_Promotion_Point);
	SetUserBrevetPoint(Cur);
}
void UB2UIHeader::OnChangedHeaderAdvancedRankPromotionPoint(class UB2UIDocBase* Sender, int32 Cur, int32 Prev)
{
 	if (Prev < Cur)
 		ApplyAddAmountAction(ERewardType::Advanced_Rank_Promotion_Point);
 	SetUserAdvancedBrevetPoint(Cur);
}

void UB2UIHeader::OnChangedHeaderDarkCrystal(class UB2UIDocBase* Sender, int32 Cur, int32 Prev)
{
	if (Prev < Cur)
		ApplyAddAmountAction(ERewardType::Dark_Crystal);
	SetUserDarkCrystal(Cur);
}

void UB2UIHeader::OnChangedHeaderEtherPiece(class UB2UIDocBase* Sender, int32 Cur, int32 Prev)
{
	SetEtherPiece(Cur);
}

void UB2UIHeader::OnChangedDimensionPiece(class UB2UIDocBase* Sender, int32 Cur, int32 Prev)
{
	SetDimensionPiece(Cur);

	UB2UIHeaderResource* pUI = GetHeaderResourceUI(EUIHeaderResourceType::DimensionPiece);

	if (pUI)
		pUI->PlayAnimationEvent_AddAmount();
}

void UB2UIHeader::OnChangedShowVersion(class UB2UIDocBase* Sender, bool bCur, bool Prev)
{
	if (UB2UIManager* UIMgrInst = UB2UIManager::GetInstance())
	{
		UIMgrInst->ShowMainWidgetVersion();
	}
}

void UB2UIHeader::OnChangeHeaderChat(class UB2UIDocBase* Sender, int32 Cur, int32 Prev)
{
	UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::Header);
}

void UB2UIHeader::OnPlatformResume()
{

}

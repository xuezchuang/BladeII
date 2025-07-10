#include "B2UIPVPModFindingMatch.h"
#include "B2UIPVP1on1MatchInfo.h"
#include "B2Button.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocLobbyMatch.h"
#include "B2UIMsgPopupUseResources.h"
#include "B2CombatStatEvaluator.h"
#include "BladeIIGameImpl.h"
#include "B2PVPGameMode.h"

UB2UIPVPModFindingMatch::UB2UIPVPModFindingMatch(const FObjectInitializer& ObjectInitializer)
	:	Super(ObjectInitializer)
{
	CachedCountDownNumber = 0;

	OpendCancleMsgPopup = nullptr;
}

void UB2UIPVPModFindingMatch::Init()
{
	Super::Init();

	InitSetLocalUserData_UsePvpOnly();
	
	FindMatch(false);
}

void UB2UIPVPModFindingMatch::FindMatch(bool bSet)
{
	bFindMatch = bSet;
	
	if (UIP_RightMatchInfo.IsValid())
		UIP_RightMatchInfo->SetVisibility(bFindMatch ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (TB_Wait.IsValid())
		TB_Wait->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, bFindMatch ? TEXT("PVP1on1_WaitingMatch") : TEXT("PVP1on1_FindingPlayer")));

	if (!bFindMatch && TB_Count.IsValid())
		TB_Count->SetVisibility(ESlateVisibility::Hidden);

	if (!bFindMatch)
		Finding_BP();
}

void UB2UIPVPModFindingMatch::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Wait);
	GET_SLOT(UTextBlock, TB_Count);
	GET_SLOT(UImage, IMG_WheelFinding);
	GET_SLOT(UImage, IMG_Versus);
	GET_SLOT(UB2UIPVP1on1MatchInfo, UIP_LeftMatchInfo);
	if (UIP_LeftMatchInfo.IsValid())
	{
		UIP_LeftMatchInfo->Init();
		UIP_LeftMatchInfo->SetLeft(true);
	}		

	GET_SLOT(UB2UIPVP1on1MatchInfo, UIP_RightMatchInfo);
	if (UIP_RightMatchInfo.IsValid())
	{
		UIP_RightMatchInfo->Init();
		UIP_RightMatchInfo->SetLeft(false);
	}		

	GET_SLOT(UButton, BTN_Cancel);
}

void UB2UIPVPModFindingMatch::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UIPVPModFindingMatch::OnClickedCancelBtn);
}

void UB2UIPVPModFindingMatch::SetCount(int32 Count)
{
	if (TB_Count.IsValid())
	{
		TB_Count->SetText(FText::AsNumber(Count));
	}

	if (CachedCountDownNumber != Count)
	{
		OnCountChangedBP(Count);
		CachedCountDownNumber = Count;
	}
}

void UB2UIPVPModFindingMatch::SetReady(int32 Count)
{
	SetReady_BP();

	SetCount(Count);
}

void UB2UIPVPModFindingMatch::SetUserData(bool bLeft, const FString& UserNick, int32 Level, int32 Grade, int32 Star, int32 WinStreak, int32 CombatValue,
	const FString& GuildName, int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor, int32 InAttackPower, int32 InDefense)
{
	TWeakObjectPtr<UB2UIPVP1on1MatchInfo>& MatchInfo = bLeft ? UIP_LeftMatchInfo : UIP_RightMatchInfo;
	if (MatchInfo.IsValid())
	{
		MatchInfo->SetUserName(UserNick);
		MatchInfo->SetChracterLevel(Level);

		MatchInfo->SetGradeAndStar(Grade, Star);

		MatchInfo->SetWinningStreak(WinStreak);

		MatchInfo->SetCombatPower(CombatValue);

		MatchInfo->SetMyBadgeImage(bLeft);

		MatchInfo->SetAttackPower(InAttackPower);
		MatchInfo->SetDefense(InDefense);

		//일단 길드시스템 완료되기전까지 비워놓기.
		MatchInfo->SetGuildNameAndMark(GuildName, Pattern, PatternColor, BG, BGColor);

		bool bShowStarGrade = true;
		if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
			bShowStarGrade = !DocPVP->GetIsFriendshipGame();

		MatchInfo->SetVisibilityStarGrade(bShowStarGrade);
	}
}

void UB2UIPVPModFindingMatch::CloseWidgetDelegate()
{
	OnClickedCancelBtn();
}

void UB2UIPVPModFindingMatch::InitSetLocalUserData_UsePvpOnly()
{
	AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(UGameplayStatics::GetGameMode(this));
	if (PVPGM == nullptr)
	{
		return;
	}

	ICharacterDataStore* pCharacterDataStore = &(BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData());
	if (pCharacterDataStore)
	{
		const EPCClass MainClass = pCharacterDataStore->GetMainPlayerClass();
		const EPCClass SubClass = pCharacterDataStore->GetSubPlayerClass();

		int32 PowerValue = CombatStatEval::GetPCCombatPower(MainClass, pCharacterDataStore);
		PowerValue += CombatStatEval::GetPCCombatPower(SubClass, pCharacterDataStore);

		int32 AttackPower = CombatStatEval::GetPCAttack(MainClass, pCharacterDataStore);
		AttackPower += CombatStatEval::GetPCAttack(SubClass, pCharacterDataStore);

		int32 Defense = CombatStatEval::GetPCDefense(MainClass, pCharacterDataStore);
		Defense += CombatStatEval::GetPCDefense(SubClass, pCharacterDataStore);

		int32 iTagMatchGrade = 10;
		int32 iTagMatchGradeStar = 2;
		int32 iTagMatchCurrContinuousWins = 4;

		if (UB2UIDocBattle* pDocBattle = UB2UIDocHelper::GetDocBattle())
		{
			iTagMatchGrade = pDocBattle->GetTagMatchGrade();
			iTagMatchGradeStar = pDocBattle->GetTagMatchGradeStar();
			iTagMatchCurrContinuousWins = pDocBattle->GetTagMatchCurrContinuousWins();
		}

		const bool bIsLocalPlayer = true;
		this->SetUserData(
			bIsLocalPlayer,
			pCharacterDataStore->GetUserNickName(),
			pCharacterDataStore->GetUserLevel(),
			iTagMatchGrade,
			iTagMatchGradeStar,
			iTagMatchCurrContinuousWins,
			PowerValue,
			TEXT(""), 0, 0, 0, 0, AttackPower, Defense);
	}
}

void UB2UIPVPModFindingMatch::OnClickedCancelBtn()
{
	UB2UIMsgPopupSimple* MsgUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Cancel_MatchingMsg")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::MatchingContinue,
		FMsgPopupOnClick::CreateLambda([this]() {OpendCancleMsgPopup = nullptr; }),
		FMsgPopupOnClick::CreateLambda([this]() {OpendCancleMsgPopup = nullptr; this->OnCancelMatch(); })
		);

	OpendCancleMsgPopup = MsgUI;
}

void UB2UIPVPModFindingMatch::SetEnableCancel(bool bToSet)
{
	//if (BTN_Cancel.IsValid())
	//{
	//	BTN_Cancel->SetVisibility(bToSet ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	//}

	//SetClose(bToSet);

	//if (!bToSet && OpendCancleMsgPopup)
	//{
	//	UB2UIManager::GetInstance()->CloseMsgPopup(OpendCancleMsgPopup);
	//	OpendCancleMsgPopup = nullptr;
	//}
}

void UB2UIPVPModFindingMatch::OnCancelMatch()
{
	auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
	bool IsInviteGame = DocLobbyMatch ? DocLobbyMatch->HasValidInvitation() : false;

	if (IsInviteGame)
	{
		ReturnToPVPMainMenuClass<bool>::GetInstance().Signal(false);
	}
	else
	{
		data_trader::Retailer::GetInstance().RequestCancelMatchmaking();
	}
}
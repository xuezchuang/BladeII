#include "B2UIRankChanged.h"


#include "BladeIIUtil.h"

UB2UIRankChanged::UB2UIRankChanged(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UsingGameModeType = EB2GameMode::End;
}

void UB2UIRankChanged::Init()
{
	Super::Init();
}

void UB2UIRankChanged::CacheAssets()
{
	GET_SLOT(UCanvasPanel, CP_MyRanking);
	GET_SLOT(UCanvasPanel, CP_OtherRanking);

	GET_SLOT(UB2UIRankerInfo, UIP_MyRankingInfo_TagMatch);
	if (UIP_MyRankingInfo_TagMatch.IsValid())
		UIP_MyRankingInfo_TagMatch->Init();
	GET_SLOT(UB2UITeamRankerInfo, UIP_MyRankingInfo_TeamMatch);
	if (UIP_MyRankingInfo_TeamMatch.IsValid())
		UIP_MyRankingInfo_TeamMatch->Init();
	GET_SLOT(UB2UIRankerInfo, UIP_MyRankingInfo_ControlMatch);
	if (UIP_MyRankingInfo_ControlMatch.IsValid())
		UIP_MyRankingInfo_ControlMatch->Init();
	GET_SLOT(UB2UIRankerInfo, UIP_OtherRankingInfo_TagMatch);
	if (UIP_OtherRankingInfo_TagMatch.IsValid())
		UIP_OtherRankingInfo_TagMatch->Init();
	GET_SLOT(UB2UITeamRankerInfo, UIP_OtherRankingInfo_TeamMatch);
	if (UIP_OtherRankingInfo_TeamMatch.IsValid())
		UIP_OtherRankingInfo_TeamMatch->Init();
	GET_SLOT(UB2UIRankerInfo, UIP_OtherRankingInfo_ControlMatch);
	if (UIP_OtherRankingInfo_ControlMatch.IsValid())
		UIP_OtherRankingInfo_ControlMatch->Init();

	GET_SLOT(UB2RichTextBlock, RTB_RankChanged);

	GET_SLOT(UB2Button, BTN_Confirm);
}

void UB2UIRankChanged::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIRankChanged::OnClickedConfirmBTN);
}

void UB2UIRankChanged::OnClickedConfirmBTN()
{
	SetVisibility(ESlateVisibility::Collapsed);
	ButtonDelegate.ExecuteIfBound();
}

void UB2UIRankChanged::SetUsingGameMode(EB2GameMode ModeType)
{
	UsingGameModeType = ModeType;

	bool bTagMatch = UsingGameModeType == EB2GameMode::PVP_Tag;
	bool bTeamMatch = UsingGameModeType == EB2GameMode::PVP_Team;
	bool bControlMatch = UsingGameModeType == EB2GameMode::Control;

	if (UIP_MyRankingInfo_TagMatch.IsValid())
		UIP_MyRankingInfo_TagMatch->SetVisibility(bTagMatch ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (UIP_MyRankingInfo_TeamMatch.IsValid())
		UIP_MyRankingInfo_TeamMatch->SetVisibility(bTeamMatch ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (UIP_MyRankingInfo_ControlMatch.IsValid())
		UIP_MyRankingInfo_ControlMatch->SetVisibility(bControlMatch ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	//서버 프로토콜 변경시까지 계속 숨김
	if (UIP_OtherRankingInfo_TagMatch.IsValid())
		UIP_OtherRankingInfo_TagMatch->SetVisibility(bTagMatch ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (UIP_OtherRankingInfo_TeamMatch.IsValid())
		UIP_OtherRankingInfo_TeamMatch->SetVisibility(bTeamMatch ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (UIP_OtherRankingInfo_ControlMatch.IsValid())
		UIP_OtherRankingInfo_ControlMatch->SetVisibility(bControlMatch ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UIRankChanged::SetMyRankingData(const FModRankerInfo& RankerInfo)
{
	if (UsingGameModeType == EB2GameMode::PVP_Tag && UIP_MyRankingInfo_TagMatch.IsValid())
		UIP_MyRankingInfo_TagMatch->SetRankerInfo(RankerInfo);

	if (UsingGameModeType == EB2GameMode::PVP_Team && UIP_MyRankingInfo_TeamMatch.IsValid())
		UIP_MyRankingInfo_TeamMatch->SetRankerInfo(RankerInfo);

	if (UsingGameModeType == EB2GameMode::Control && UIP_MyRankingInfo_ControlMatch.IsValid())
		UIP_MyRankingInfo_ControlMatch->SetRankerInfo(RankerInfo);
}

void UB2UIRankChanged::SetOtherRankingData(const FModRankerInfo& RankerInfo)
{
	if (UsingGameModeType == EB2GameMode::PVP_Tag && UIP_OtherRankingInfo_TagMatch.IsValid())
		UIP_OtherRankingInfo_TagMatch->SetRankerInfo(RankerInfo);

	if (UsingGameModeType == EB2GameMode::PVP_Team && UIP_OtherRankingInfo_TeamMatch.IsValid())
		UIP_OtherRankingInfo_TeamMatch->SetRankerInfo(RankerInfo);

	if (UsingGameModeType == EB2GameMode::Control && UIP_OtherRankingInfo_ControlMatch.IsValid())
		UIP_OtherRankingInfo_ControlMatch->SetRankerInfo(RankerInfo);
}

void UB2UIRankChanged::SetChangedRanking(int32 DiffRanking)
{
	if (RTB_RankChanged.IsValid())
		RTB_RankChanged->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_RankingChanged")), FText::AsNumber(DiffRanking)));

	OnRankChange();
}
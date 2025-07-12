// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildBattle.h"
#include "B2UIManager.h"
#include "B2UIGuildMatchBattleSlot.h"

#include "B2UIDocHelper.h"
#include "B2UIDocGuild.h"
#include "B2GuildGameMode.h"
#include "B2UIGuildMark.h"
#include "B2UIGuildTeam.h"
#include "BladeIIGMAIPlayer.h"
#include "BladeIIMercenaryPlayer.h"
#include "B2UIGuildNPCSkill.h"
#include "B2UIGuildRound.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2UIGuildBattle::UB2UIGuildBattle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetClose(false);
}
void UB2UIGuildBattle::CacheAssets()
{
	Super::CacheAssets();

	LeftSlots.AddDefaulted(4);
	GET_SLOT_BYNAME(UB2UIGuildMatchBattleSlot, LeftSlots[0], LeftSlot1);
	GET_SLOT_BYNAME(UB2UIGuildMatchBattleSlot, LeftSlots[1], LeftSlot2);
	GET_SLOT_BYNAME(UB2UIGuildMatchBattleSlot, LeftSlots[2], LeftSlot3);
	GET_SLOT_BYNAME(UB2UIGuildMatchBattleSlot, LeftSlots[3], LeftSlot4);
	for (auto LeftSlot : LeftSlots)
	{
		LeftSlot->Init();
	}

	RightSlots.AddDefaulted(4);
	GET_SLOT_BYNAME(UB2UIGuildMatchBattleSlot, RightSlots[0], RightSlot1);
	GET_SLOT_BYNAME(UB2UIGuildMatchBattleSlot, RightSlots[1], RightSlot2);
	GET_SLOT_BYNAME(UB2UIGuildMatchBattleSlot, RightSlots[2], RightSlot3);
	GET_SLOT_BYNAME(UB2UIGuildMatchBattleSlot, RightSlots[3], RightSlot4);
	for (auto RightSlot : RightSlots)
	{
		RightSlot->Init();
	}

	LeftGuildTeams.AddDefaulted(2);
	GET_SLOT_BYNAME(UB2UIGuildTeam, LeftGuildTeams[0], LeftGuildTeam1);
	GET_SLOT_BYNAME(UB2UIGuildTeam, LeftGuildTeams[1], LeftGuildTeam2);

	RightGuildTeams.AddDefaulted(2);
	GET_SLOT_BYNAME(UB2UIGuildTeam, RightGuildTeams[0], RightGuildTeam1);
	GET_SLOT_BYNAME(UB2UIGuildTeam, RightGuildTeams[1], RightGuildTeam2);

	for (int32 Index = 0 ; Index < LeftGuildTeams.Num(); Index++)
	{
		LeftGuildTeams[Index]->Init();
		LeftGuildTeams[Index]->SetData(Index);
	}
	for (int32 Index = 0; Index < RightGuildTeams.Num(); Index++)
	{
		RightGuildTeams[Index]->Init();
		RightGuildTeams[Index]->SetData(Index);
	}

	LeftSlotCurrentTeamNum  = INDEX_NONE;
	RightSlotCurrentTeamNum = INDEX_NONE;
	
	GET_SLOT(UB2Button, BTN_ChangeCam);
	GET_SLOT(UB2Button, BTN_Pause);

	GET_SLOT(UTextBlock, TB_BattleTime)
	GET_SLOT(UB2UIGuildMark, UIP_MyGuildMark)
	if (UIP_MyGuildMark.IsValid())
		UIP_MyGuildMark->Init();

	GET_SLOT(UB2UIGuildMark, UIP_OtherGuildMark)
	if (UIP_OtherGuildMark.IsValid())
		UIP_OtherGuildMark->Init();

	GET_SLOT(UB2RichTextBlock, TB_MyGuildName)
	GET_SLOT(UB2RichTextBlock, TB_MyNickName)
	GET_SLOT(UB2RichTextBlock, TB_OtherGuildName)
	GET_SLOT(UB2RichTextBlock, TB_OtherNickName)

	for (uint8 Cnt = 0; Cnt < 2; Cnt++)
	{
		FString SkillAssetName = TEXT("NPCSkill") + FString::FromInt(Cnt);
		auto SkillAsset = GetCachedWidget<UB2UIGuildNPCSkill>(FName(*SkillAssetName));
		if (SkillAsset)
		{
			UIP_GuildNPCSkills.Add(SkillAsset);
			SkillAsset->Init();
		}
	}
}

void UB2UIGuildBattle::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_ChangeCam, &UB2UIGuildBattle::OnClickBTN_ChangeCam);
	BIND_CLICK_FUNC_TO_BTN(BTN_Pause , &UB2UIGuildBattle::OnClickPauseButton)
}

void UB2UIGuildBattle::CloseWidgetDelegate()
{
	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		if (DocBattle->GetIsCenematicMode() == false)
		{
			OnClickPauseButton();
		}
	}
}

void UB2UIGuildBattle::Init()
{
	Super::Init();
}

void UB2UIGuildBattle::DestroySelf(UB2UIManager* InUIManager)
{
	for (auto LeftSlot : LeftSlots)
	{
		LeftSlot->DestroySelf(InUIManager);
	}

	for (auto RightSlot : RightSlots)
	{
		RightSlot->DestroySelf(InUIManager);
	}
	for (auto LeftGuildTeam : LeftGuildTeams)
	{
		LeftGuildTeam->DestroySelf(InUIManager);
	}
	for (auto RightGuildTeam : RightGuildTeams)
	{
		RightGuildTeam->DestroySelf(InUIManager);
	}
	Super::DestroySelf(InUIManager);
}

void UB2UIGuildBattle::BindDocAuto()
{
	UnbindDoc();

	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	BII_CHECK(GuildDoc);
	if (!GuildDoc)
		return;

	SetDoc(GuildDoc);
	GuildDoc->OnChangeGuildTeam.AddUObject(this, &UB2UIGuildBattle::OnChangeGuildTeam);
	GuildDoc->OnBattleTimeChanged.AddUObject(this, &UB2UIGuildBattle::OnChangeBattleTime);
	GuildDoc->OnAllyDisplayTeamNumChanged.AddUObject(this, &UB2UIGuildBattle::OnChangePlayerGuildTeamDisplay);
	GuildDoc->OnOtherDisplayTeamNumChanged.AddUObject(this, &UB2UIGuildBattle::OnChangeEnemyGuildTeamDisplay);
	
}

void UB2UIGuildBattle::UnbindDoc()
{
	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	if (GuildDoc)
	{
		GuildDoc->OnChangeGuildTeam.RemoveAll(this);
		GuildDoc->OnBattleTimeChanged.RemoveAll(this);
		GuildDoc->OnAllyDisplayTeamNumChanged.RemoveAll(this);
		GuildDoc->OnOtherDisplayTeamNumChanged.RemoveAll(this);
	}
		
	SetDoc(nullptr);
}

void UB2UIGuildBattle::OnClickBTN_ChangeCam()
{
	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	BII_CHECK(GuildDoc);
	if (!GuildDoc)
		return;

	//GuildGameChangeCameraClass<bool, ABladeIIPlayer*>::GetInstance().Signal(!GuildDoc->GetShortCamera(), nullptr);
}

void UB2UIGuildBattle::OnClickPauseButton()
{
	//StartPauseMenuClass<EB2GameMode>::GetInstance().Signal(GetB2GameModeType(this));

	//SetPendingPause();
}

void UB2UIGuildBattle::SetTitleSetting()
{
	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	if (!GuildDoc)
		return;

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	auto MyGuildInfo = ClientData.GetMyGuildInfo().GuildInfo;
	auto OtherGuildInfo = ClientData.GetOtherGuildBattleInfo();
	
	if (MyGuildInfo)
	{
		UIP_MyGuildMark->SetGuildMark(MyGuildInfo->mark_index, MyGuildInfo->mark_color, MyGuildInfo->mark_bg_index, MyGuildInfo->mark_bg_color);
		TB_MyGuildName->SetText(FText::FromString(MyGuildInfo->guild_name));
		TB_MyNickName->SetText(FText::FromString(ClientData.GetUserNickName()));
	}

	if (OtherGuildInfo)
	{
		UIP_OtherGuildMark->SetGuildMark(OtherGuildInfo->mark_index, OtherGuildInfo->mark_color, OtherGuildInfo->mark_bg_index, OtherGuildInfo->mark_bg_color);
		TB_OtherGuildName->SetText(FText::FromString(OtherGuildInfo->guild_name));
		TB_OtherNickName->SetText(FText::FromString(GuildDoc->GetOtherGuildMemeberCharInfo()->nickname));
	}
}

void UB2UIGuildBattle::SetBattleTime(int64 BattleTime)
{
	if (TB_BattleTime.IsValid())
	{
		TB_BattleTime->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(BattleTime, TEXT("m:ss"))));
	}
}

void UB2UIGuildBattle::RefreshGuildTeam(ETeamIndex InTeamIndex, uint8 InTeamNum)
{
	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	if (!GuildDoc)
		return;

	FGuildTeamModel * ViewModel = GuildDoc->FindGuildTeamViewModel(InTeamIndex, InTeamNum);
	if (!ViewModel)
		return;

	bool bMyTeam = InTeamIndex == ETeamIndex::Player;

	TArray<TWeakObjectPtr<UB2UIGuildTeam>>& CurrentTeam = bMyTeam ? LeftGuildTeams : RightGuildTeams;

	for(int32 Index = 0; Index < CurrentTeam.Num(); Index++)
	{
		if (Index < InTeamNum)
			CurrentTeam[Index]->SetBattleState(EGuildTeamState::End);
		else if (Index == InTeamNum)
			CurrentTeam[Index]->SetBattleState(EGuildTeamState::Start);
		else
			CurrentTeam[Index]->SetBattleState(EGuildTeamState::Waiting);
	}

	int8& CurrentTeamNum = InTeamIndex == ETeamIndex::Player ? LeftSlotCurrentTeamNum : RightSlotCurrentTeamNum;

	if (ViewModel->Characters.Num() > 0 && InTeamNum != CurrentTeamNum)
	{
		TArray<TWeakObjectPtr<UB2UIGuildMatchBattleSlot>>& CurrentSlot = bMyTeam ? LeftSlots : RightSlots;
		
		int NPCSkillIndex = 0;
		for (ABladeIICharacter* Character : ViewModel->Characters)
		{
			ABladeIIGMAIPlayer* GMAIPlayer = Cast<ABladeIIGMAIPlayer>(Character);
			if (GMAIPlayer)
			{
				const FGuildPlayerInfo & PlayerInfo = GMAIPlayer->GetGuildPlayerInfo();
				int32 Index = PlayerInfo.DocCharIndex;
				if (CurrentSlot.IsValidIndex(Index))
				{
					CurrentSlot[Index]->SetData(PlayerInfo.CharClass, GMAIPlayer->GetPlayerLevel(), bMyTeam, Index, PlayerInfo.FormationNumber);
					CurrentSlot[Index]->ConnectPawn(GMAIPlayer);
				}
			}
			ABladeIIMercenaryPlayer* Mercenary = Cast<ABladeIIMercenaryPlayer>(Character);
			if (Mercenary)
			{
				const FGuildMercenaryInfo & MercenaryInfo = Mercenary->GetMercenaryInfo();
				int32 Index = MercenaryInfo.DocCharIndex;
				if (CurrentSlot.IsValidIndex(Index))
				{
					CurrentSlot[Index]->SeMercenarytData(MercenaryInfo, bMyTeam);
					CurrentSlot[Index]->ConnectPawn(Mercenary);
				}
				if (bMyTeam && UIP_GuildNPCSkills.IsValidIndex(NPCSkillIndex) && UIP_GuildNPCSkills[NPCSkillIndex].IsValid())
				{
					UIP_GuildNPCSkills[NPCSkillIndex]->SetNPCSkill(Mercenary);
					++NPCSkillIndex;
				}
			}
		}
	}
}

void UB2UIGuildBattle::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	SetTitleSetting();
	RefreshGuildTeam(ETeamIndex::Player, 0);
	RefreshGuildTeam(ETeamIndex::Enemy, 0);
	SetClose(true);
}

void UB2UIGuildBattle::OnChangeGuildTeam(ETeamIndex InTeamIndex, uint8 InTeamNum)
{
	RefreshGuildTeam(InTeamIndex, InTeamNum);
}

void UB2UIGuildBattle::OnChangePlayerGuildTeamDisplay(UB2UIDocBase * Sender, uint8 NewValue, uint8 PreValue)
{
	RefreshGuildTeam(ETeamIndex::Player, NewValue);
}

void UB2UIGuildBattle::OnChangeEnemyGuildTeamDisplay(UB2UIDocBase * Sender, uint8 NewValue, uint8 PreValue)
{
	RefreshGuildTeam(ETeamIndex::Enemy, NewValue);
}

void UB2UIGuildBattle::OnChangeBattleTime(UB2UIDocBase * Sender, int32 NewValue, int32 PreValue)
{
	SetBattleTime(NewValue);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIGameInviteSlot.h"
#include "B2UIDocHelper.h"
#include "B2UIDocRaid.h"



#include "BladeIIUtil.h"

void UB2UIGameInviteSlot::Init()
{
	Super::Init();
	SlotState = EGameInviteSlotIndex::Invitable;
	CurCountDownNum = 0;
	SlotIndex = 0;

	if (auto* World = GetWorld())
		World->GetTimerManager().ClearTimer(InviteTimerHandle);
}

void UB2UIGameInviteSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Character);
	GET_SLOT(UTextBlock, TB_CharLevel);
	GET_SLOT(UTextBlock, TB_CharName);
	GET_SLOT(UB2Button, BTN_Invite);
	GET_SLOT(UTextBlock, TB_CombatPower);

	GET_SLOT(UTextBlock, STB_Invite);
	GET_SLOT(UTextBlock, STB_Invite2);
	GET_SLOT(UTextBlock, STB_InviteSuccess);
	GET_SLOT(UTextBlock, STB_CannotInvite);
	GET_SLOT(UTextBlock, STB_OffLine);
	GET_SLOT(UTextBlock, STB_RaidBreaker);
}

void UB2UIGameInviteSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Invite, &UB2UIGameInviteSlot::OnClick_BTNInvite);
}

void UB2UIGameInviteSlot::UpdateStaticText()
{
	if (STB_Invite.IsValid())
		STB_Invite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Guild_Invitable"))));
	if (STB_Invite2.IsValid())
		STB_Invite2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Guild_Invitable"))));
	if (STB_InviteSuccess.IsValid())
		STB_InviteSuccess->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_InviteComplete"))));
	if (STB_CannotInvite.IsValid())
		STB_CannotInvite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_UnableRequest"))));
	if (STB_OffLine.IsValid())
		STB_OffLine->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_InviteOffLine"))));
	if (STB_RaidBreaker.IsValid())
		STB_RaidBreaker->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_Penalty"))));
}

void UB2UIGameInviteSlot::OnClick_BTNInvite()
{
	FClickBTNInvite.ExecuteIfBound(GetNickName(), SlotIndex);
}

void UB2UIGameInviteSlot::SetSlotEntryData(FB2CharacterSummaryPtr CharSummaryPtr, int32 RemainInvitingTime /*= -1*/, bool bIsPenalty /*= false*/)
{
	if (CharSummaryPtr.get() == nullptr)
		return;

	SetSlotEntryData(FText::FromString(CharSummaryPtr->account_name),
		CharSummaryPtr->character_level,
		SvrToCliPCClassType(CharSummaryPtr->character_type),
		CharSummaryPtr->power);		// jwyim power

	bool IsOfflineUser = !CharSummaryPtr->is_online;
	UpdateSlotState(FText::FromString(CharSummaryPtr->account_name), IsOfflineUser, RemainInvitingTime, bIsPenalty);
}

void UB2UIGameInviteSlot::SetSlotEntryData(const FText& CharName, int32 CharLevel, EPCClass PCType, int32 CombatPoint)
{
	if (PCType == EPCClass::EPC_End)
		return;

	int32 PcIndex = PCClassToInt(PCType);

	if (CharterTexture.IsValidIndex(PcIndex) && IMG_Character.IsValid())
		IMG_Character->SetBrushFromMaterial(CharterTexture[PcIndex]);

	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(CombatPoint));

	if (TB_CharName.IsValid())
		TB_CharName->SetText(CharName);

	if (TB_CharLevel.IsValid())
		TB_CharLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(CharLevel))));
}

void UB2UIGameInviteSlot::SetSlotState(EGameInviteSlotIndex State, int32 RemainInvitingTime)
{
	SlotState = State;

	SetSlotState_BP((int32)State);

	if (SlotState == EGameInviteSlotIndex::Inviting)
		StartJoinTimer(RemainInvitingTime);
}

void UB2UIGameInviteSlot::UpdateSlotState(const FText& CharName, bool IsOffline, int32 RemainInvitingTime, bool bIsPenalty)
{
	EGameInviteSlotIndex LocalSlotState = EGameInviteSlotIndex::Invitable;
	
	// 货肺绊魔 穿福扁傈俊 檬措窍绊乐带 惑炔捞搁 促矫 檬措 惑怕肺 父甸绢淋
	if (RemainInvitingTime != -1)
		LocalSlotState = EGameInviteSlotIndex::Inviting;

	if (IsAlreadyInviteUser(CharName))
		LocalSlotState = EGameInviteSlotIndex::AlreadyInvite;

	if (bIsPenalty)
		LocalSlotState = EGameInviteSlotIndex::Penalty;

	if (IsOffline)
		LocalSlotState = EGameInviteSlotIndex::Offline;

	SetSlotState(LocalSlotState, RemainInvitingTime);
}

void UB2UIGameInviteSlot::StartJoinTimer(int32 RemainInvitingTime)
{
	CurCountDownNum = (RemainInvitingTime != -1) ? RemainInvitingTime : UB2UIGameInviteSlot::JOINCOUNTDOWN_SECOND;

	SetInviteTimerCount_BP(CurCountDownNum);
	// 概 鸥捞赣绰 1檬 窜困烙.
	GetWorld()->GetTimerManager().SetTimer(InviteTimerHandle, this, &UB2UIGameInviteSlot::UpdateJoinTime, 1.0f, false);
}

void UB2UIGameInviteSlot::SetSlotIndex(int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;
}

const FText UB2UIGameInviteSlot::GetNickName()
{
	if (TB_CharName.IsValid())
		return TB_CharName->GetText();

	return FText();
}

bool UB2UIGameInviteSlot::IsAlreadyInviteUser(const FText& CharName)
{
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		for (int32 i = 0; i < 4; ++i)
		{
			if (DocRaid->GetJoinSlotStateByIndex(i) != ERaidJoinSlotState::JoinCharacter)
				continue;
			
			if (CharName.EqualTo(DocRaid->GetJoinSlotCharNameByIndex(i)))
				return true;
		}
	}

	return false;
}

void UB2UIGameInviteSlot::UpdateJoinTime()
{
	if (UWorld* TheWorld = GetWorld())
	{
		TheWorld->GetTimerManager().ClearTimer(InviteTimerHandle);

		--CurCountDownNum;

		SetInviteTimerCount_BP(CurCountDownNum);

		if (CurCountDownNum > 0)
		{
			// 犁蓖利 龋免
			TheWorld->GetTimerManager().SetTimer(InviteTimerHandle, this, &UB2UIGameInviteSlot::UpdateJoinTime, 1.0f, false);
		}
		else
		{
			FStopInviteTimer.ExecuteIfBound(GetNickName());

			if (SlotState == EGameInviteSlotIndex::Inviting)
				SetSlotState(EGameInviteSlotIndex::Invitable);
		}
	}
}


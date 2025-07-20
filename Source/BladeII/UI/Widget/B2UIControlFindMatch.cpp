// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlFindMatch.h"
#include "B2ControlGameMode.h"
#include "B2UIDocHelper.h"


#include "BladeIIGameImpl.h"
#include "../../BladeII/BladeIILocText.h"


void UB2UIControlFindMatch::NativeConstruct()
{
	// 唱吝俊 咆胶飘抛捞喉肺 捞悼!
	// %s(%d困)
	FString strNameRank = FString::Printf(TEXT("%s(%dRank)"), *BladeIIGameImpl::GetLocalCharacterData().GetUserNickName(), 100);

	if (TB_NameRank.IsValid())
		TB_NameRank->SetText(FText::FromString(strNameRank));

	UB2UIDocBattle* pDocBattle = UB2UIDocHelper::GetDocBattle();
	UB2UIDocHero* pDocHero = UB2UIDocHelper::GetDocHero(pDocBattle->GetCurPCClass());

	if (TB_ATK_Value.IsValid())
		TB_ATK_Value->SetText(FText::AsNumber(pDocHero->GetAttackPower()));

	if (TB_DEF_Value.IsValid())
		TB_DEF_Value->SetText(FText::AsNumber(pDocHero->GetDefensePower()));
}

void UB2UIControlFindMatch::SetAllyPlayerCount(int32 nPlayerCount)
{
	UE_LOG(LogBladeII, Log, TEXT("nPlayerCount = %d"), nPlayerCount);
	// 评盔 肮荐UI 阂难林绰芭 咯扁辑
}

void UB2UIControlFindMatch::SetFindingEnd()
{
	if (TB_MatchingNotice.IsValid())
		TB_MatchingNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlWaitMoveToBattleGround")));

	PlayAnimationEvent_Anim_FindingEnd();
}

void UB2UIControlFindMatch::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_NameRank);
	GET_SLOT(UTextBlock, TB_ATK_Value);
	GET_SLOT(UTextBlock, TB_DEF_Value);
	GET_SLOT(UTextBlock, TB_MatchingNotice);

	GET_SLOT(UButton, BTN_Cancle);

	BIND_CLICK_FUNC_TO_BTN(BTN_Cancle, &UB2UIControlFindMatch::OnCancleBtnClick);
}

void UB2UIControlFindMatch::OnCancleBtnClick()
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
	{
		if (!pGM->GetRoomId().IsEmpty())
			pGM->LeaveRoomIfJoined();
		else
			pGM->ReturnToControlMainMenu();
	}
}

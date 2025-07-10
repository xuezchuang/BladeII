// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlTeamPlayerInfo.h"
#include "BladeIINetPlayer.h"
#include "B2ControlGameMode.h"
#include "B2InGameHUDInfo_Player.h"
#include "B2PCClassInfoBox.h"
#include "B2GameInstance.h"



UB2UIControlTeamPlayerInfo::UB2UIControlTeamPlayerInfo(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CurrentHpBar = NULL;

	m_fRemainResurrectTime = 0;
}

void UB2UIControlTeamPlayerInfo::Init()
{
	Super::Init();
	TargetPlayer.Reset();

	IsPlayerAlive = true;
}

void UB2UIControlTeamPlayerInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetTargetPlayer() && CurrentHpBar)
	{
		CurrentHpBar->SetPercent(TargetPlayer->GetHealth() / TargetPlayer->GetMaxHealth());
	}

	if (TargetPlayer.IsValid() && TargetPlayer->IsAlive() != IsPlayerAlive)
	{
		IsPlayerAlive = TargetPlayer->IsAlive();

		if (IsPlayerAlive)
			PlayAnimationEvent_NormalState();
		else
			PlayAnimationEvent_DeadState();
	}

	UpdateRemainResurrectTime(InDeltaTime);
}

void UB2UIControlTeamPlayerInfo::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_LvName);
	GET_SLOT(UTextBlock, TB_KillInfo);
	GET_SLOT(UImage, IMG_CharacterImage);
	GET_SLOT(UTextBlock, TB_RemainResurrectTime);
	GET_SLOT(UProgressBar, PB_HP_R);
	GET_SLOT(UProgressBar, PB_HP_B);
}

void UB2UIControlTeamPlayerInfo::UpdateRemainResurrectTime(float InDeltaTime)
{
	m_fRemainResurrectTime -= InDeltaTime;

	m_fRemainResurrectTime = FMath::Max(m_fRemainResurrectTime, 0.0f);

	if (TB_RemainResurrectTime.IsValid())
	{
		TB_RemainResurrectTime->SetText(FText::AsNumber(int32(m_fRemainResurrectTime)));

		if (m_fRemainResurrectTime <= 0.0f)
			TB_RemainResurrectTime->SetVisibility(ESlateVisibility::Hidden);
		else
			TB_RemainResurrectTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UIControlTeamPlayerInfo::SetPlayer(class ABladeIINetPlayer* pPlayer)
{
	TargetPlayer = pPlayer;
	TargetPlayerNetId = TargetPlayer->GetNetId();

	// 정보 셋팅
	if (TB_LvName.IsValid())
		TB_LvName->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharOnHeadLevelAndName")), FText::AsNumber(pPlayer->GetPlayerLevel()), FText::FromString(pPlayer->GetCharacterDataStore()->GetUserNickName())));

	//초상화
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));

	if (B2GI)
	{
		UB2InGameHUDInfo_Player* pHUDInfo_Player = B2GI->GetPlayerHUDInfo(pPlayer->GetCurrentPlayerClass());
		if (pHUDInfo_Player)
		{
			UMaterialInstance* pMI = Cast<UMaterialInstance>(pHUDInfo_Player->GetPlayerPortrait(this));
			if (pMI)
				IMG_CharacterImage->SetBrushFromMaterial(pMI);
		}
	}

	SetKillCount(0);


	if (pPlayer && PB_HP_R.IsValid() && PB_HP_B.IsValid())
	{
		PB_HP_R->SetVisibility(ESlateVisibility::Hidden);
		PB_HP_B->SetVisibility(ESlateVisibility::Hidden);

		switch (ETeamType(pPlayer->GetTeamNum()))
		{
		case ETeamType::ETT_Red:
			CurrentHpBar = PB_HP_R.Get();
			break;
		case ETeamType::ETT_Blue:
			CurrentHpBar = PB_HP_B.Get();
			break;
		}

		if (CurrentHpBar)
			CurrentHpBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UIControlTeamPlayerInfo::SetKillCount(int32 nCount)
{
	if (TB_KillInfo.IsValid())
		TB_KillInfo->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlPlayerKillPoint")), FText::AsNumber(nCount)));
}

void UB2UIControlTeamPlayerInfo::SetRemainResurrectTime(float fRemainTime)
{
	m_fRemainResurrectTime = fRemainTime;
}

ABladeIINetPlayer* UB2UIControlTeamPlayerInfo::GetTargetPlayer()
{
	if (TargetPlayer.IsValid())
		return TargetPlayer.Get();

	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	
	if (pGM)
	{
		TargetPlayer = Cast<ABladeIINetPlayer>(pGM->FindPawnByNetId(TargetPlayerNetId));

		if (TargetPlayer.IsValid())
			return TargetPlayer.Get();
	}

	return NULL;
}

uint32 UB2UIControlTeamPlayerInfo::GetTargetPlayerNetId()
{
	return TargetPlayerNetId;
}

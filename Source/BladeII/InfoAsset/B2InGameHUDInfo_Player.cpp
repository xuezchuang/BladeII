// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square


#include "B2InGameHUDInfo_Player.h"
#include "B2LobbyGameMode.h"
//#include "B2PVPGameMode.h"
//#include "B2RaidGameMode.h"
//#include "B2CounterAttackGameMode.h"

UB2InGameHUDInfo_Player::UB2InGameHUDInfo_Player(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bOverrideAttackButtonStyle = false;
}

UMaterialInterface* UB2InGameHUDInfo_Player::GetPlayerPortrait(UObject* WorldContextObject)
{
	// �Ȱ��� ���ܸ��� �ʻ�ȭ���� ���� �ٸ� ��Ʋ�󽺿� ���� �־ �κ����� ������������ ���� �ٸ��� ���. Ư���� ���̽���. 
	// �� �ʻ�ȭ ������ ��Ʋ�󽺿� �� �ְ� ������ ���� �̷� �ʿ� ���� �� ������..
	AB2LobbyGameMode* B2LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	TSoftObjectPtr<UMaterialInterface>& TAssetToLoad = B2LobbyGM ? PlayerPortrait_Lobby : PlayerPortrait_InGameBattle;
	return TAssetToLoad.LoadSynchronous();
}

UMaterialInterface* UB2InGameHUDInfo_Player::GetPlayerDeathPortrait(UObject* WorldContextObject)
{
	//�׾������� Battle �ܿ��� ���� �� ���Ƽ� GetPlayerPortraitó�� üũ ����
	TSoftObjectPtr<UMaterialInterface>& TAssetToLoad = PlayerDeathPortrait_InGameBattle;
	return TAssetToLoad.LoadSynchronous();
}

UTexture2D* UB2InGameHUDInfo_Player::GetPlayerLargePortrait()
{ // �̰� �ƽθ� ���� �ؽ�����. �κ� ���� ���� �� ������ �ֽô��� ����. ���� �ؽ��ķ� ���� ���� ��Ƽ���� �ʿ��� UI ������ �ʿ��� �׳� �ؽ��ķ� �־.
	return PlayerLargePortrait.LoadSynchronous();
}

void ApplyAttackBtnMarginCommon(FButtonStyle& InOutButtonStyleToModify, FMargin& InMarginToSet)
{
	InOutButtonStyleToModify.Normal.Margin = InMarginToSet;
	InOutButtonStyleToModify.Hovered.Margin = InMarginToSet;
	InOutButtonStyleToModify.Pressed.Margin = InMarginToSet;
	InOutButtonStyleToModify.Disabled.Margin = InMarginToSet;
}

void UB2InGameHUDInfo_Player::ApplyAttackBtnMargin(FButtonStyle& InOutButtonStyleToModify, UObject* WorldContextObject)
{
	//AGameModeBase* CurGM = UGameplayStatics::GetGameMode(WorldContextObject);

	//AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(CurGM);
	//AB2RaidGameMode* RaidGM = Cast<AB2RaidGameMode>(CurGM);
	//AB2CounterAttackGameMode* CounterGM = Cast<AB2CounterAttackGameMode>(CurGM);

	//if (PVPGM) {
	//	ApplyAttackBtnMarginCommon(InOutButtonStyleToModify, AttackButtonStyle_Margin_PVP);
	//}
	//else if (RaidGM){
	//	ApplyAttackBtnMarginCommon(InOutButtonStyleToModify, AttackButtonStyle_Margin_Raid);
	//}
	//else if (CounterGM) {
	//	ApplyAttackBtnMarginCommon(InOutButtonStyleToModify, AttackButtonStyle_Margin_Counter);
	//}
	//else{
	//	ApplyAttackBtnMarginCommon(InOutButtonStyleToModify, AttackButtonStyle_Margin);
	//}
}
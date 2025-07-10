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
	// 똑같이 생겨먹은 초상화지만 서로 다른 아틀라스에 속해 있어서 로비인지 전투맵인지에 따라 다르게 사용. 특수한 케이스임. 
	// 걍 초상화 정도는 아틀라스에 안 넣고 별도로 쓰면 이럴 필요 없을 거 같은데..
	AB2LobbyGameMode* B2LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	TSoftObjectPtr<UMaterialInterface>& TAssetToLoad = B2LobbyGM ? PlayerPortrait_Lobby : PlayerPortrait_InGameBattle;
	return TAssetToLoad.LoadSynchronous();
}

UMaterialInterface* UB2InGameHUDInfo_Player::GetPlayerDeathPortrait(UObject* WorldContextObject)
{
	//죽었을때는 Battle 외에는 없을 것 같아서 GetPlayerPortrait처럼 체크 안함
	TSoftObjectPtr<UMaterialInterface>& TAssetToLoad = PlayerDeathPortrait_InGameBattle;
	return TAssetToLoad.LoadSynchronous();
}

UTexture2D* UB2InGameHUDInfo_Player::GetPlayerLargePortrait()
{ // 이건 아싸리 별도 텍스쳐임. 로비 전투 따로 쓸 이유가 애시당초 없음. 별도 텍스쳐로 쓰다 보니 머티리얼도 필요없어서 UI 디자인 쪽에서 그냥 텍스쳐로 넣어돔.
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
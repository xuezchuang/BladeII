// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square


#include "B2InGameHUDInfo_Player.h"
//#include "B2LobbyGameMode.h"
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
	//// 度鞍捞 积败冈篮 檬惑拳瘤父 辑肺 促弗 酒撇扼胶俊 加秦 乐绢辑 肺厚牢瘤 傈捧甘牢瘤俊 蝶扼 促福霸 荤侩. 漂荐茄 纳捞胶烙. 
	//// 傲 檬惑拳 沥档绰 酒撇扼胶俊 救 持绊 喊档肺 静搁 捞凡 鞘夸 绝阑 芭 鞍篮单..
	//AB2LobbyGameMode* B2LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	//TSoftObjectPtr<UMaterialInterface>& TAssetToLoad = B2LobbyGM ? PlayerPortrait_Lobby : PlayerPortrait_InGameBattle;
	//return TAssetToLoad.LoadSynchronous();
	return NULL;
}

UMaterialInterface* UB2InGameHUDInfo_Player::GetPlayerDeathPortrait(UObject* WorldContextObject)
{
	//磷菌阑锭绰 Battle 寇俊绰 绝阑 巴 鞍酒辑 GetPlayerPortrait贸烦 眉农 救窃
	TSoftObjectPtr<UMaterialInterface>& TAssetToLoad = PlayerDeathPortrait_InGameBattle;
	return TAssetToLoad.LoadSynchronous();
}

UTexture2D* UB2InGameHUDInfo_Player::GetPlayerLargePortrait()
{ // 捞扒 酒轿府 喊档 咆胶媚烙. 肺厚 傈捧 蝶肺 镜 捞蜡啊 局矫寸檬 绝澜. 喊档 咆胶媚肺 静促 焊聪 赣萍府倔档 鞘夸绝绢辑 UI 叼磊牢 率俊辑 弊成 咆胶媚肺 持绢导.
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
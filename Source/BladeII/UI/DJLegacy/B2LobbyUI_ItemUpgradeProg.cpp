// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemUpgradeProg.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "B2LobbyUI_ItemDetailPostOp.h"

UB2LobbyUI_ItemUpgradeProg::UB2LobbyUI_ItemUpgradeProg(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BeginUpgradePS = NULL;
	BeginUpgradeFxScale = FVector(1.0f, 1.0f, 1.0f);
	CreatedBeginUpgradeFx = NULL;

	CompleteUpgradePS = NULL;
	CompleteUpgradeFxScale = FVector(1.0f, 1.0f, 1.0f);
	CreatedCompleteUpgradeFx = NULL;
}

void UB2LobbyUI_ItemUpgradeProg::CacheAssets()
{
	Super::CacheAssets();

}

void UB2LobbyUI_ItemUpgradeProg::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// 酒流 铰鞭 楷免俊辑绰 铰鞭 傈狼 酒捞袍 钎矫啊 绝栏骨肺 见败初绰促.
	if (CreatedItemIcon_Target)
	{
		CreatedItemIcon_Target->SetVisibility(ESlateVisibility::Collapsed);
	}

	StartUpgradeSoundfx();
}

void UB2LobbyUI_ItemUpgradeProg::DestroySelf()
{
	Super::DestroySelf();
}

void UB2LobbyUI_ItemUpgradeProg::UpdateStaticWidgets()
{
	Super::UpdateStaticWidgets();
}

// 酒贰 捞抚甸篮 漂沥 Particle System 府家胶狼 Named Material Slots 俊 沥狼登哥 肚茄 Required 葛碘狼 Named Material Override 俊辑 荤侩.
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_TargetBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_TargetItemIcon(TEXT("MAT_ItemIcon"));
//const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_IngredBGPanel(TEXT("MAT_ItemBGPanel_Right")); 捞扒 钎矫甫 窍霸 瞪荐档 救 窍霸 瞪 荐档..
//const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_IngredItemIcon(TEXT("MAT_ItemIcon_Right"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_UpgradeBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_UpgradeItemIcon(TEXT("MAT_ItemIcon"));

void UB2LobbyUI_ItemUpgradeProg::StartBeginUpgradeScene()
{
	DestroyAllFx(); // 父老 捞傈巴捞 巢酒乐绢具 茄促搁 官曹 巴.

	if (BeginUpgradePS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedBeginUpgradeFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeginUpgradePS, FXCreateTransform);
		if (CreatedBeginUpgradeFx)
		{
			CreatedBeginUpgradeFx->SetWorldScale3D(BeginUpgradeFxScale);

			SetupNamedMIDForFxComp(CreatedBeginUpgradeFx); // 鞘夸茄 MIC 甸俊辑 MID 甫 父甸绢辑 荤侩 啊瓷窍档废 技泼.

			// Fx 率俊 MID 鳖瘤 霖厚啊 登菌促搁 颇飘喊肺 颇扼固磐 蔼阑 啊廉棵 UI 率狼 MIC 甫 啊廉客辑 MID 俊 颇扼固磐甫 技泼.


			// Fx 率俊 MID 鳖瘤 霖厚啊 登菌促搁 颇飘喊肺 颇扼固磐 蔼阑 啊廉棵 UI 率狼 MIC 甫 啊廉客辑 MID 俊 颇扼固磐甫 技泼.
			{
				UMaterialInstanceDynamic* TargetBGPanelMID = GetFxNamedMID(CreatedBeginUpgradeFx, Name_FxMID_TargetBGPanel);
				UMaterialInstanceConstant* TargetBGPanelMIC_Ref = CreatedItemIcon_Target ? CreatedItemIcon_Target->GetBGPanelMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(TargetBGPanelMID, TargetBGPanelMIC_Ref);
			}{
				UMaterialInstanceDynamic* TargetItemIconMID = GetFxNamedMID(CreatedBeginUpgradeFx, Name_FxMID_TargetItemIcon);
				UMaterialInstanceConstant* TargetItemIconMIC_Ref = CreatedItemIcon_Target ? CreatedItemIcon_Target->GetMainIconMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(TargetItemIconMID, TargetItemIconMIC_Ref);
			}
		}
	}

	PlayResultSound(true);
}

void UB2LobbyUI_ItemUpgradeProg::StartCompleteUpgradeScene()
{
	DestroyAllFx(); // 父老 巢酒乐绢具 且 捞傈 巴捞 乐促搁 官曹 巴.

	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // 搬苞 酒捞袍 单捞磐甫 促矫 茄锅 啊廉咳.

	SetItemData_Result(NativeItemData_Result); // 搬苞 酒捞袍 酒捞能阑 泅犁 焊捞扒 救 焊捞扒 老窜 积己阑 秦具 窃. 弊贰具 官肺 酒贰俊辑 UI material 啊廉客辑 Fx 率俊辑 颇扼固磐甫 技泼茄促.
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Hidden); // 绢恫 版快电 老窜 见败初霸 瞪 巴捞促. 瘤陛篮 3D 瓤苞啊 唱客具 且 矫痢.
	}
	
	if (CompleteUpgradePS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedCompleteUpgradeFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CompleteUpgradePS, FXCreateTransform);
		if (CreatedCompleteUpgradeFx)
		{
			CreatedCompleteUpgradeFx->SetWorldScale3D(CompleteUpgradeFxScale);

			SetupNamedMIDForFxComp(CreatedCompleteUpgradeFx); // 鞘夸茄 MIC 甸俊辑 MID 甫 父甸绢辑 荤侩 啊瓷窍档废 技泼.

			// Fx 率俊 MID 鳖瘤 霖厚啊 登菌促搁 颇飘喊肺 颇扼固磐 蔼阑 啊廉棵 UI 率狼 MIC 甫 啊廉客辑 MID 俊 颇扼固磐甫 技泼.
			{
				UMaterialInstanceDynamic* UpgradeBGPanelMID = GetFxNamedMID(CreatedCompleteUpgradeFx, Name_FxMID_UpgradeBGPanel);
				UMaterialInstanceConstant* UpgradeBGPanelMIC_Ref = CreatedItemIcon_Result ? CreatedItemIcon_Result->GetBGPanelMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(UpgradeBGPanelMID, UpgradeBGPanelMIC_Ref);
			}{
				UMaterialInstanceDynamic* UpgradeItemIconMID = GetFxNamedMID(CreatedCompleteUpgradeFx, Name_FxMID_UpgradeItemIcon);
				UMaterialInstanceConstant* UpgradeItemIconMIC_Ref = CreatedItemIcon_Result ? CreatedItemIcon_Result->GetMainIconMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(UpgradeItemIconMID, UpgradeItemIconMIC_Ref);
			}
		}
	}
}

void UB2LobbyUI_ItemUpgradeProg::DestroyAllFx()
{
	Super::DestroyAllFx();

	DestroyBeginUpgradeFx();
	DestroyCompleteUpgradeFx();
}

void UB2LobbyUI_ItemUpgradeProg::DestroyBeginUpgradeFx()
{
	if (CreatedBeginUpgradeFx){
		CreatedBeginUpgradeFx->ConditionalBeginDestroy();
		CreatedBeginUpgradeFx = NULL;
	}
}

void UB2LobbyUI_ItemUpgradeProg::DestroyCompleteUpgradeFx()
{
	if (CreatedCompleteUpgradeFx){
		CreatedCompleteUpgradeFx->ConditionalBeginDestroy();
		CreatedCompleteUpgradeFx = NULL;
	}
}

void UB2LobbyUI_ItemUpgradeProg::ShowBattleScorePopup()
{// 局聪皋捞记 楷免 辆丰矫 龋免登绰 傈捧仿 包访 扑诀 
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(IntToPCClass(NativeItemData_Target.AllowedPCClass));
}

void UB2LobbyUI_ItemUpgradeProg::ShowCloseButton()
{
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->ShowBackButton();
	}
}
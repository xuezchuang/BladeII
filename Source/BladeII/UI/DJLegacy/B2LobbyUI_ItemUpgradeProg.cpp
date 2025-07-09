// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
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

	// 아직 승급 연출에서는 승급 전의 아이템 표시가 없으므로 숨겨놓는다.
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

// 아래 이름들은 특정 Particle System 리소스의 Named Material Slots 에 정의되며 또한 Required 모듈의 Named Material Override 에서 사용.
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_TargetBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_TargetItemIcon(TEXT("MAT_ItemIcon"));
//const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_IngredBGPanel(TEXT("MAT_ItemBGPanel_Right")); 이건 표시를 하게 될수도 안 하게 될 수도..
//const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_IngredItemIcon(TEXT("MAT_ItemIcon_Right"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_UpgradeBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_UpgradeItemIcon(TEXT("MAT_ItemIcon"));

void UB2LobbyUI_ItemUpgradeProg::StartBeginUpgradeScene()
{
	DestroyAllFx(); // 만일 이전것이 남아있어야 한다면 바꿀 것.

	if (BeginUpgradePS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedBeginUpgradeFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeginUpgradePS, FXCreateTransform);
		if (CreatedBeginUpgradeFx)
		{
			CreatedBeginUpgradeFx->SetWorldScale3D(BeginUpgradeFxScale);

			SetupNamedMIDForFxComp(CreatedBeginUpgradeFx); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.

			// Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.


			// Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
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
	DestroyAllFx(); // 만일 남아있어야 할 이전 것이 있다면 바꿀 것.

	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // 결과 아이템 데이터를 다시 한번 가져옴.

	SetItemData_Result(NativeItemData_Result); // 결과 아이템 아이콘을 현재 보이건 안 보이건 일단 생성을 해야 함. 그래야 바로 아래에서 UI material 가져와서 Fx 쪽에서 파라미터를 세팅한다.
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Hidden); // 어떤 경우든 일단 숨겨놓게 될 것이다. 지금은 3D 효과가 나와야 할 시점.
	}
	
	if (CompleteUpgradePS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedCompleteUpgradeFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CompleteUpgradePS, FXCreateTransform);
		if (CreatedCompleteUpgradeFx)
		{
			CreatedCompleteUpgradeFx->SetWorldScale3D(CompleteUpgradeFxScale);

			SetupNamedMIDForFxComp(CreatedCompleteUpgradeFx); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.

			// Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
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
{// 애니메이션 연출 종료시 호출되는 전투력 관련 팝업 
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(IntToPCClass(NativeItemData_Target.AllowedPCClass));
}

void UB2LobbyUI_ItemUpgradeProg::ShowCloseButton()
{
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->ShowBackButton();
	}
}
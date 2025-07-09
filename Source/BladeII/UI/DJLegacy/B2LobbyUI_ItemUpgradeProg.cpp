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

	// ���� �±� ���⿡���� �±� ���� ������ ǥ�ð� �����Ƿ� ���ܳ��´�.
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

// �Ʒ� �̸����� Ư�� Particle System ���ҽ��� Named Material Slots �� ���ǵǸ� ���� Required ����� Named Material Override ���� ���.
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_TargetBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_TargetItemIcon(TEXT("MAT_ItemIcon"));
//const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_IngredBGPanel(TEXT("MAT_ItemBGPanel_Right")); �̰� ǥ�ø� �ϰ� �ɼ��� �� �ϰ� �� ����..
//const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_IngredItemIcon(TEXT("MAT_ItemIcon_Right"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_UpgradeBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemUpgradeProg::Name_FxMID_UpgradeItemIcon(TEXT("MAT_ItemIcon"));

void UB2LobbyUI_ItemUpgradeProg::StartBeginUpgradeScene()
{
	DestroyAllFx(); // ���� �������� �����־�� �Ѵٸ� �ٲ� ��.

	if (BeginUpgradePS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedBeginUpgradeFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeginUpgradePS, FXCreateTransform);
		if (CreatedBeginUpgradeFx)
		{
			CreatedBeginUpgradeFx->SetWorldScale3D(BeginUpgradeFxScale);

			SetupNamedMIDForFxComp(CreatedBeginUpgradeFx); // �ʿ��� MIC �鿡�� MID �� ���� ��� �����ϵ��� ����.

			// Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.


			// Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.
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
	DestroyAllFx(); // ���� �����־�� �� ���� ���� �ִٸ� �ٲ� ��.

	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // ��� ������ �����͸� �ٽ� �ѹ� ������.

	SetItemData_Result(NativeItemData_Result); // ��� ������ �������� ���� ���̰� �� ���̰� �ϴ� ������ �ؾ� ��. �׷��� �ٷ� �Ʒ����� UI material �����ͼ� Fx �ʿ��� �Ķ���͸� �����Ѵ�.
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Hidden); // � ���� �ϴ� ���ܳ��� �� ���̴�. ������ 3D ȿ���� ���;� �� ����.
	}
	
	if (CompleteUpgradePS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedCompleteUpgradeFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CompleteUpgradePS, FXCreateTransform);
		if (CreatedCompleteUpgradeFx)
		{
			CreatedCompleteUpgradeFx->SetWorldScale3D(CompleteUpgradeFxScale);

			SetupNamedMIDForFxComp(CreatedCompleteUpgradeFx); // �ʿ��� MIC �鿡�� MID �� ���� ��� �����ϵ��� ����.

			// Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.
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
{// �ִϸ��̼� ���� ����� ȣ��Ǵ� ������ ���� �˾� 
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(IntToPCClass(NativeItemData_Target.AllowedPCClass));
}

void UB2LobbyUI_ItemUpgradeProg::ShowCloseButton()
{
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->ShowBackButton();
	}
}
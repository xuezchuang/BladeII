// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemEnhanceProg.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "TutorialManager.h"

// �Ʒ� �̸����� Ư�� Particle System ���ҽ��� Named Material Slots �� ���ǵǸ� ���� Required ����� Named Material Override ���� ���.
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_TargetBGPanel(TEXT("MAT_ItemBGPanel_Left"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_TargetItemIcon(TEXT("MAT_ItemIcon_Left"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_IngredBGPanel(TEXT("MAT_ItemBGPanel_Right"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_IngredItemIcon(TEXT("MAT_ItemIcon_Right"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_UnitingBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_UnitingItemIcon(TEXT("MAT_ItemIcon"));

UB2LobbyUI_ItemEnhanceProg::UB2LobbyUI_ItemEnhanceProg(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;
	CreatedItemIcon_Ingred = NULL;
	GettingClosePS = NULL;
	GettingCloseFxScale = FVector(1.0f, 1.0f, 1.0f);
	CreatedGettingCloseFx = NULL;
	EnhanceSuccessUnitingPS = NULL;
	EnhanceSuccessUnitingFxScale = FVector(1.0f, 1.0f, 1.0f);
	CreatedUnitingFx = NULL;
}

void UB2LobbyUI_ItemEnhanceProg::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_ItemIconSet_Ingred);
}

void UB2LobbyUI_ItemEnhanceProg::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	SetItemData_Target(NativeItemData_Target);

	TArray<FB2Item> AllIngreds; // ���� �ϳ����� ��.
	CachedInventory->GetAllGhostIngreds(AllIngreds);
	if (AllIngreds.Num() > 0)
		SetItemData_Ingred(AllIngreds[0]);

	// ���⿡�� �� �̰��� Ÿ�� ������ �������� ��� ȭ�鿡���� �����ؼ� ������ ����. ��� �������� �ִ� �� ������.
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->SetShowMainItemIcon(false);
	}

	PlayResultSound(CachedInventory->IsSuccessEnhanceItemForResultPresent());


	TUTORIAL // Tutorial Hardcoding
	if (TutorialManager::GetInstance().IsCurrentTutorial(TutorialID_ItemEnhance, 7))
	{
		TutorialManager::GetInstance().OnItemEnhancedClicked();
	}
}

void UB2LobbyUI_ItemEnhanceProg::DestroySelf()
{
	Super::DestroySelf();

	DestroyItemIcon_Ingred();
}

void UB2LobbyUI_ItemEnhanceProg::UpdateStaticWidgets()
{
	Super::UpdateStaticWidgets();
}

void UB2LobbyUI_ItemEnhanceProg::StartGettingCloseScene()
{
	DestroyAllFx(); // ���� �������� �����־�� �Ѵٸ� �ٲ� ��.

	if (!GettingClosePS) {
		return;
	}

	FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
	CreatedGettingCloseFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GettingClosePS, FXCreateTransform);
	if (CreatedGettingCloseFx)
	{
		CreatedGettingCloseFx->SetWorldScale3D(GettingCloseFxScale);

		// ���� RefID �� ���� ������ ��Ƽ������ ����Ǵ� UI �����ܰ� �������� �ܰ��� �������� ��.

		SetupNamedMIDForFxComp(CreatedGettingCloseFx); // �ʿ��� MIC �鿡�� MID �� ���� ��� �����ϵ��� ����.
													   // Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.
		{
			UMaterialInstanceDynamic* TargetBGPanelMID = GetFxNamedMID(CreatedGettingCloseFx, Name_FxMID_TargetBGPanel);
			UMaterialInstanceConstant* TargetBGPanelMIC_Ref = CreatedItemIcon_Target ? CreatedItemIcon_Target->GetBGPanelMIC() : NULL;
			SetupMIDNonAtlasParamFromUIMIC(TargetBGPanelMID, TargetBGPanelMIC_Ref);
		} {
			UMaterialInstanceDynamic* TargetItemIconMID = GetFxNamedMID(CreatedGettingCloseFx, Name_FxMID_TargetItemIcon);
			UMaterialInstanceConstant* TargetItemIconMIC_Ref = CreatedItemIcon_Target ? CreatedItemIcon_Target->GetMainIconMIC() : NULL;
			SetupMIDNonAtlasParamFromUIMIC(TargetItemIconMID, TargetItemIconMIC_Ref);
		} {
			UMaterialInstanceDynamic* IngredBGPanelMID = GetFxNamedMID(CreatedGettingCloseFx, Name_FxMID_IngredBGPanel);
			UMaterialInstanceConstant* IngredBGPanelMIC_Ref = CreatedItemIcon_Ingred ? CreatedItemIcon_Ingred->GetBGPanelMIC() : NULL;
			SetupMIDNonAtlasParamFromUIMIC(IngredBGPanelMID, IngredBGPanelMIC_Ref);
		} {
			UMaterialInstanceDynamic* IngredItemIconMID = GetFxNamedMID(CreatedGettingCloseFx, Name_FxMID_IngredItemIcon);
			UMaterialInstanceConstant* IngredItemIconMIC_Ref = CreatedItemIcon_Ingred ? CreatedItemIcon_Ingred->GetMainIconMIC() : NULL;
			SetupMIDNonAtlasParamFromUIMIC(IngredItemIconMID, IngredItemIconMIC_Ref);
		}
	}
}

void UB2LobbyUI_ItemEnhanceProg::StartUnitingScene()
{
	DestroyAllFx(); // ���� �������� �����־�� �Ѵٸ� �ٲ� ��.

	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // ��� ������ �����͸� �ٽ� �ѹ� ������.

	SetItemData_Result(NativeItemData_Result); // ��� ������ �������� ���� ���̰� �� ���̰� �ϴ� ������ �ؾ� ��. �׷��� �ٷ� �Ʒ����� UI material �����ͼ� Fx �ʿ��� �Ķ���͸� �����Ѵ�.
	if (CreatedItemIcon_Result)
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Hidden); // � ���� �ϴ� ���ܳ��� �� ���̴�. ������ 3D ȿ���� ���;� �� ����.
	
	UParticleSystem* CurUnitingPS = CachedInventory->IsSuccessEnhanceItemForResultPresent() ? EnhanceSuccessUnitingPS : EnhanceFailUnitingPS;
	if (CurUnitingPS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedUnitingFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurUnitingPS, FXCreateTransform);
		if (CreatedUnitingFx)
		{
			CreatedUnitingFx->SetWorldScale3D(EnhanceSuccessUnitingFxScale);

			SetupNamedMIDForFxComp(CreatedUnitingFx); // �ʿ��� MIC �鿡�� MID �� ���� ��� �����ϵ��� ����.

			// Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.
			{
				UMaterialInstanceDynamic* UnitingPSBGPanelMID = GetFxNamedMID(CreatedUnitingFx, Name_FxMID_UnitingBGPanel);
				UMaterialInstanceConstant* UnitingPSBGPanelMIC_Ref = CreatedItemIcon_Result ? CreatedItemIcon_Result->GetBGPanelMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(UnitingPSBGPanelMID, UnitingPSBGPanelMIC_Ref);
			} {
				UMaterialInstanceDynamic* UnitingPSItemIconMID = GetFxNamedMID(CreatedUnitingFx, Name_FxMID_UnitingItemIcon);
				UMaterialInstanceConstant* UnitingPSItemIconMIC_Ref = CreatedItemIcon_Result ? CreatedItemIcon_Result->GetMainIconMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(UnitingPSItemIconMID, UnitingPSItemIconMIC_Ref);
			}
		}
	}
}

void UB2LobbyUI_ItemEnhanceProg::SetItemData_Ingred(FB2Item& InItem)
{
	DestroyItemIcon_Ingred();

	NativeItemData_Ingred = InItem;
	CreatedItemIcon_Ingred = CreateItemIconCommon(X_CP_ItemIconSet_Ingred.Get(), NativeItemData_Ingred, ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);
}

void UB2LobbyUI_ItemEnhanceProg::DestroyItemIcon_Ingred()
{
	if (CreatedItemIcon_Ingred) {
		CreatedItemIcon_Ingred->ConditionalBeginDestroy();
		CreatedItemIcon_Ingred = NULL;
	}
}

void UB2LobbyUI_ItemEnhanceProg::DestroyAllFx()
{
	Super::DestroyAllFx();
	DestroyGettingCloseFx();
	DestroyUnitingFx();
}

void UB2LobbyUI_ItemEnhanceProg::DestroyGettingCloseFx()
{
	if (CreatedGettingCloseFx) {
		CreatedGettingCloseFx->ConditionalBeginDestroy();
		CreatedGettingCloseFx = NULL;
	}
}

void UB2LobbyUI_ItemEnhanceProg::DestroyUnitingFx()
{
	if (CreatedUnitingFx) {
		CreatedUnitingFx->ConditionalBeginDestroy();
		CreatedUnitingFx = NULL;
	}
}

void UB2LobbyUI_ItemEnhanceProg::ShowBattleScorePopup()
{// �ִϸ��̼� ���� ����� ȣ��Ǵ� ������ ���� �˾� 
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(IntToPCClass(NativeItemData_Target.AllowedPCClass));
}

void UB2LobbyUI_ItemEnhanceProg::ShowCloseButton()
{
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->ShowBackButton();
	}
}

void UB2LobbyUI_ItemEnhanceProg::StartShowResultItemIcon()
{
	Super::StartShowResultItemIcon();

	// Note : ��� ��ȭ�� ������� ����Ǵ°��� �ƴϹǷ� ����� ������ �ƴ� �׳� UI������ ǥ���ϵ��� ����.
	if (CreatedItemIcon_Result)	
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(false, true);
}

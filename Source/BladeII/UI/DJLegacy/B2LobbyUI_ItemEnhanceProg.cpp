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

// 아래 이름들은 특정 Particle System 리소스의 Named Material Slots 에 정의되며 또한 Required 모듈의 Named Material Override 에서 사용.
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

	TArray<FB2Item> AllIngreds; // 재료는 하나여야 함.
	CachedInventory->GetAllGhostIngreds(AllIngreds);
	if (AllIngreds.Num() > 0)
		SetItemData_Ingred(AllIngreds[0]);

	// 연출에서 쓴 이곳의 타겟 아이템 아이콘을 결과 화면에서도 연장해서 보여줄 것임. 결과 페이지에 있는 건 무시함.
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
	DestroyAllFx(); // 만일 이전것이 남아있어야 한다면 바꿀 것.

	if (!GettingClosePS) {
		return;
	}

	FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
	CreatedGettingCloseFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GettingClosePS, FXCreateTransform);
	if (CreatedGettingCloseFx)
	{
		CreatedGettingCloseFx->SetWorldScale3D(GettingCloseFxScale);

		// 이하 RefID 에 따라 적절한 머티리얼이 적용되는 UI 아이콘과 마찬가지 외관이 나오도록 함.

		SetupNamedMIDForFxComp(CreatedGettingCloseFx); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.
													   // Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
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
	DestroyAllFx(); // 만일 이전것이 남아있어야 한다면 바꿀 것.

	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // 결과 아이템 데이터를 다시 한번 가져옴.

	SetItemData_Result(NativeItemData_Result); // 결과 아이템 아이콘을 현재 보이건 안 보이건 일단 생성을 해야 함. 그래야 바로 아래에서 UI material 가져와서 Fx 쪽에서 파라미터를 세팅한다.
	if (CreatedItemIcon_Result)
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Hidden); // 어떤 경우든 일단 숨겨놓게 될 것이다. 지금은 3D 효과가 나와야 할 시점.
	
	UParticleSystem* CurUnitingPS = CachedInventory->IsSuccessEnhanceItemForResultPresent() ? EnhanceSuccessUnitingPS : EnhanceFailUnitingPS;
	if (CurUnitingPS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedUnitingFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurUnitingPS, FXCreateTransform);
		if (CreatedUnitingFx)
		{
			CreatedUnitingFx->SetWorldScale3D(EnhanceSuccessUnitingFxScale);

			SetupNamedMIDForFxComp(CreatedUnitingFx); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.

			// Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
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
{// 애니메이션 연출 종료시 호출되는 전투력 관련 팝업 
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

	// Note : 장비 강화는 별등급이 변경되는것은 아니므로 등급은 연출이 아닌 그냥 UI상으로 표시하도록 하자.
	if (CreatedItemIcon_Result)	
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(false, true);
}

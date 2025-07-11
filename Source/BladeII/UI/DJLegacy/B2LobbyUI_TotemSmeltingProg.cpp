// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_TotemSmeltingProg.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Event.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"

const FName UB2LobbyUI_TotemSmeltingProg::Name_FxMID_SmeltingCompleteBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_TotemSmeltingProg::Name_FxMID_SmeltingCompleteItemIcon(TEXT("MAT_ItemIcon"));

UB2LobbyUI_TotemSmeltingProg::UB2LobbyUI_TotemSmeltingProg(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;

	SmeltingCompletePS = NULL;
	SmeltingCompleteFxScale = FVector(1.0f, 1.0f, 1.0f);
	CreatedSmeltingCompleteFx = NULL;
}

void UB2LobbyUI_TotemSmeltingProg::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock,		TB_SmeltingComplete);
	GET_SLOT(UHorizontalBox,	HB_SmeltingComplete);
	if (HB_SmeltingComplete.IsValid())
	{
		HB_SmeltingComplete->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2LobbyUI_TotemSmeltingProg::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	StartBGSoundfx();
}

void UB2LobbyUI_TotemSmeltingProg::DestroySelf()
{
	Super::DestroySelf();
}

void UB2LobbyUI_TotemSmeltingProg::UpdateStaticWidgets()
{
	Super::UpdateStaticWidgets();

	if (TB_SmeltingComplete.IsValid())
	{
		TB_SmeltingComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_smelt_complete")));
	}
}

void UB2LobbyUI_TotemSmeltingProg::SetTotemData_Result(FB2Totem& InTotem)
{
	DestroyItemIcon_Result();

	InTotem.GetItemFormat(NativeItemData_Result);
	CreatedItemIcon_Result = CreateTotemIcon(X_CP_ItemIconSet_Result.Get(), InTotem, ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);
}

UB2DynItemIcon_ItemOpScene* UB2LobbyUI_TotemSmeltingProg::CreateTotemIcon(UCanvasPanel* ParentCP, const FB2Totem& InTotemData, ELobbyInvenItemIconUsage IconUsage)
{
	if (!ItemIconWidgetClass || !ParentCP || InTotemData.ID <= 0 || InTotemData.RefID <= 0) {
		return NULL;
	}

	// Anchor ������ ���� GetSize �� ���ϴ� ���� �� ���� ���̹Ƿ� ����.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(ParentCP->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());
	UB2DynItemIcon_ItemOpScene* CreatedItemIcon = Cast<UB2DynItemIcon_ItemOpScene>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, ParentCP, DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
	));

	if (CreatedItemIcon)
	{
		CreatedItemIcon->SetIconUsageType(IconUsage);
		CreatedItemIcon->UpdateTotemData(InTotemData);
		return CreatedItemIcon;
	}
	return NULL;
}

void UB2LobbyUI_TotemSmeltingProg::StartShowResultItemIcon()
{
	Super::StartShowResultItemIcon();

	if (HB_SmeltingComplete.IsValid())
	{
		HB_SmeltingComplete->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2LobbyUI_TotemSmeltingProg::StartSmeltingCompleteFx()
{
	DestroySmeltingCompleteFx();

	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // ��� ������ �����͸� �ٽ� �ѹ� ������.

	FB2Totem ResultTotem;
	TMap<int64, FB2Totem> LocalAlltotemData;
	
	BladeIIGameImpl::GetLocalCharacterData().GetAllTotems(LocalAlltotemData);

	if (LocalAlltotemData.Contains(NativeItemData_Result.InstanceUID))
	{
		ResultTotem = LocalAlltotemData[NativeItemData_Result.InstanceUID];
	}

	SetTotemData_Result(ResultTotem);// ��� ������ �������� ���� ���̰� �� ���̰� �ϴ� ������ �ؾ� ��. �׷��� �ٷ� �Ʒ����� UI material �����ͼ� Fx �ʿ��� �Ķ���͸� �����Ѵ�.
	
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Hidden); // � ���� �ϴ� ���ܳ��� �� ���̴�. ������ 3D ȿ���� ���;� �� ����.
	}

	if (SmeltingCompletePS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedSmeltingCompleteFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmeltingCompletePS, FXCreateTransform);
		if (CreatedSmeltingCompleteFx)
		{
			CreatedSmeltingCompleteFx->SetWorldScale3D(SmeltingCompleteFxScale);

			SetupNamedMIDForFxComp(CreatedSmeltingCompleteFx); // �ʿ��� MIC �鿡�� MID �� ���� ��� �����ϵ��� ����.

			// Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.
			{
				UMaterialInstanceDynamic* SmeltingCompletePSBGPanelMID = GetFxNamedMID(CreatedSmeltingCompleteFx, Name_FxMID_SmeltingCompleteBGPanel);
				UMaterialInstanceConstant* SmeltingCompletePSBGPanelMIC_Ref = CreatedItemIcon_Result ? CreatedItemIcon_Result->GetBGPanelMIC_Totem() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(SmeltingCompletePSBGPanelMID, SmeltingCompletePSBGPanelMIC_Ref);
			} {
				UMaterialInstanceDynamic* SmeltingCompletePSItemIconMID = GetFxNamedMID(CreatedSmeltingCompleteFx, Name_FxMID_SmeltingCompleteItemIcon);
				UMaterialInstanceConstant* SmeltingCompletePSItemIconMIC_Ref = CreatedItemIcon_Result ? CreatedItemIcon_Result->GetMainIconMIC_Totem() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(SmeltingCompletePSItemIconMID, SmeltingCompletePSItemIconMIC_Ref);
			}
		}
	}
}

void UB2LobbyUI_TotemSmeltingProg::FinishSmeltingCompleteFx()
{
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UGameplayStatics::SetGlobalTimeDilation(LobbyGM, 1.0f); // ���⿡�� OverallPlayRate �� ���� �����ߴ� �� ����.
	QuitItemOpModeClass<bool>::GetInstance().Signal(true);

	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_TotemSmeltingProg::DestroySmeltingCompleteFx()
{
	if (CreatedSmeltingCompleteFx) {
		CreatedSmeltingCompleteFx->ConditionalBeginDestroy();
		CreatedSmeltingCompleteFx = NULL;
	}
}

void UB2LobbyUI_TotemSmeltingProg::DestroyAllFx()
{
	Super::DestroyAllFx();
	DestroySmeltingCompleteFx();
}
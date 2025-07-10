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

	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
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
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // 결과 아이템 데이터를 다시 한번 가져옴.

	FB2Totem ResultTotem;
	TMap<int64, FB2Totem> LocalAlltotemData;
	
	BladeIIGameImpl::GetLocalCharacterData().GetAllTotems(LocalAlltotemData);

	if (LocalAlltotemData.Contains(NativeItemData_Result.InstanceUID))
	{
		ResultTotem = LocalAlltotemData[NativeItemData_Result.InstanceUID];
	}

	SetTotemData_Result(ResultTotem);// 결과 아이템 아이콘을 현재 보이건 안 보이건 일단 생성을 해야 함. 그래야 바로 아래에서 UI material 가져와서 Fx 쪽에서 파라미터를 세팅한다.
	
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Hidden); // 어떤 경우든 일단 숨겨놓게 될 것이다. 지금은 3D 효과가 나와야 할 시점.
	}

	if (SmeltingCompletePS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedSmeltingCompleteFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmeltingCompletePS, FXCreateTransform);
		if (CreatedSmeltingCompleteFx)
		{
			CreatedSmeltingCompleteFx->SetWorldScale3D(SmeltingCompleteFxScale);

			SetupNamedMIDForFxComp(CreatedSmeltingCompleteFx); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.

			// Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
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
	UGameplayStatics::SetGlobalTimeDilation(LobbyGM, 1.0f); // 연출에서 OverallPlayRate 에 따라 변경했던 거 복구.
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
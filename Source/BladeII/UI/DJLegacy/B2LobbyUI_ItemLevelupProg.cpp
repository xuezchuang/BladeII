// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemLevelupProg.h"
#include "B2LobbyUI_ItemLevelupMain.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2UIManager_Lobby.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "Particles/ParticleSystemComponent.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "TutorialManager.h"

UB2LobbyUI_ItemLevelupProg::UB2LobbyUI_ItemLevelupProg(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;
	ItemIconWidgetClass = NULL;
	CreatedTargetItemIcon = NULL;
	TargetItemIconScreenPos = FVector2D(0.0f, 0.0f);
	IngredientIconPlacementMargin = 10.0f;
	IngredientIconNextRowOffset = FVector2D(0.0f, 20.0f);
	IngredientIconMaxSize = FVector2D(176.0f, 220.0f);

	OverallPlayRate = 1.0f;
	ItemSuckingTrailDepth = 5.0f; // �̰� ũ�� �� ���. 2D �� 3D �� ���ΰ� ������ �Ǵٺ���.
	ItemSuckingStartTime = 0.5f;
	ItemSuckingInterval = 0.2f;
	ItemSuckingAccAcc = 100.0f;
	ItemSuckingBaseAcc = 200.0f;
	EffectiveItemSuckingAcc = 0.0f;
	ItemSuckingMaxSpeed = 400.0f;
	ItemSuckingTrailPS = NULL;
	ItemSuckingTrailScale = FVector(1.0f, 1.0f, 1.0f);
	ItemSuckingTrailLifeTime = 0.0f;
	bAllIngredSuckingFxCreated = false;
	
	OverallBGFxSetup.CreateDepth = 100.0f;
	CreatedOverallBGFx = NULL;
	EnhanceBlowupFxSetup.CreateDepth = 50.0f;
	CreatedEnhanceBlowupFx = NULL;
	TargetItemBGFxSetup.CreateDepth = 10.0f;
	CreatedTargetItemBGFx = NULL;
	FinalEnhanceSuccessFxSetup.CreateDepth = 5.0f;
	CreatedFinalEnhanceSuccessFx = NULL;

	FinalResultPageNRef = NULL;

	bUseCameraAdaptiveFOV = false;
	AdaptiveFOVScale = 0.6f;
}

void UB2LobbyUI_ItemLevelupProg::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_TargetItemIconSet);
	GET_SLOT(UCanvasPanel, X_CP_EnhanceIngredIconPanel);
	//GET_SLOT(UWidgetAnimation, FinalResultAnimation);
}

void UB2LobbyUI_ItemLevelupProg::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// �ܼ��� ������ ������ �������� Inventory �ν��Ͻ� ���� static ���� ���� �� ������ ��ȭ ����� �κ��丮�� �� �ִ� ���¿��� ����.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);
	
	EffectiveItemSuckingAcc = ItemSuckingBaseAcc; // ��ӵ�(ItemSuckingAccAcc 0)������ �ʱⰪ�� ������

	// ������Ʈ �Ǳ� ��/�� Ÿ�� ������ �����Ͱ� �ʿ� (UB2LobbyInventory::OnReqForItemOpCommon ���� ���) 
	bool bFailedToFindResultData = !CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_Before, true);
	if (bFailedToFindResultData || !IsSupportedOpMode(CachedInventory->GetItemOpMode()))
	{
		FinishAndProceedToResult(); // ���� �ϳ� �����Ѵٸ�.. ��¿ �� ����.
		return;
	}

	// �� ������ ��� �����͸� �������� ���� ���� ����. �׽�Ʈ Ŀ�ǵ� ����̰ų� ���� ���� ������ �� �ٸ��� �Ǿ��ٰų�..
	if (!CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_After, false))
		NativeTargetLevelupItem_After = NativeTargetLevelupItem_Before; // ������ ��� �ϴ��� ���� �����͸� �־��ش�.

	if (FinalResultPageNRef) // �̰� ���� ������ �ƴϴ�. ��, ����� ���� �� �־��.
	{
		FinalResultPageNRef->StartFromLobby(InUIManager, InGM);
		FinalResultPageNRef->UpdateItemData(NativeTargetLevelupItem_Before, NativeTargetLevelupItem_After);
		FinalResultPageNRef->ClosePopup(); // ó������ ���ܳ���. ���� ���� �ܰ�μ� ��������.
	}
		
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, OverallPlayRate);

	ApplyCameraAdaptiveFOV();

	SetTargetItemIconData(NativeTargetLevelupItem_Before); // Before �� ����.

	DestroyItemLevelupIngredientItemIcon();
	DestroySuckingFXComps();
	OnFinalIngredJustSucked();
}

void UB2LobbyUI_ItemLevelupProg::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// Sucking Fx ���� ������ ������Ʈ.
	bool bAtLeastOneSuckingFxPresent = false;
	for (int32 FI = 0; FI < CreatedSuckingFxSet.Num(); ++FI)
	{
		if (CreatedSuckingFxSet[FI].FXComp){ // ���� �Ŀ��� FXComp �� ���ŵǰ� NULL �� �� ��.
			bAtLeastOneSuckingFxPresent = true;
			UpdateSingleSuckingFxComp(FI, InDeltaTime);
		}
	}
	if (bAtLeastOneSuckingFxPresent)
	{
		EffectiveItemSuckingAcc += (ItemSuckingAccAcc * InDeltaTime); // �ϳ��� �־�� ���ӵ� ����.
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroySelf()
{
	Super::DestroySelf();

	if (FinalResultPageNRef)
	{
		FinalResultPageNRef->DestroySelf();
	}

	DestroyTargetItemIcon();
	DestroyItemLevelupIngredientItemIcon();
	DestroySuckingFXComps();
	DestroyOverallBGFx();
	DestroyEnhanceBlowupFx();
	DestroyTargetItemBGFx();
	DestroyFinalEnhanceSuccessFxComp();
}

const FName UB2LobbyUI_ItemLevelupProg::Name_FxMID_ItemBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemLevelupProg::Name_FxMID_ItemIcon(TEXT("MAT_ItemIcon"));

UMaterialInstanceDynamic* UB2LobbyUI_ItemLevelupProg::GetFxMID_ItemBGPanel(class UParticleSystemComponent* InPSComp)
{
	return GetFxNamedMID(InPSComp, Name_FxMID_ItemBGPanel);
}

UMaterialInstanceDynamic* UB2LobbyUI_ItemLevelupProg::GetFxMID_ItemIcon(class UParticleSystemComponent* InPSComp)
{
	return GetFxNamedMID(InPSComp, Name_FxMID_ItemIcon);
}

void UB2LobbyUI_ItemLevelupProg::ApplyCameraAdaptiveFOV()
{
	checkSlow(CachedLobbyGM && CachedLobbyGM->IsInItemOpDirectingView());

	APlayerController* OwningPC = GetOwningPlayer();
	// �� ��� �¾����� ���� ī�޶� ���Ͱ� ViewTarget ���� ���õǾ� �־�� ��.
	ACameraActor* ViewTargetCam = OwningPC ? Cast<ACameraActor>(OwningPC->GetViewTarget()) : nullptr;
	checkSlow(ViewTargetCam);
	if (ViewTargetCam && ViewTargetCam->GetCameraComponent())
	{ // AspectRatioAdaptiveFOV �� ����ϸ� ���� �ػ� �������� ū ȭ�� �������� ����� �߸��� �� ���� �� ������
		// ��� �о��� �þ߷� ���� ���߾��� �� ����Ǵ� ���� �߻��� �� �ִ�. ��Ȳ�� ���� �����ϰ�..
		// ���� 2D-3D ��Ұ� ������ ��߳��� �� ���� �ִ�. �̰� Ư�� �پ��� �ػ� �������� �׽�Ʈ �ʿ�.
		//ViewTargetCam->GetCameraComponent()->SetAspectRatioAdaptiveFOV(bUseCameraAdaptiveFOV);
		//ViewTargetCam->GetCameraComponent()->SetAdaptiveFOVScale(AdaptiveFOVScale);
	}
}

void UB2LobbyUI_ItemLevelupProg::SetTargetItemIconData(FB2Item& InItem)
{
	//DestroyTargetItemIcon();

	if (ItemIconWidgetClass == NULL || !X_CP_TargetItemIconSet.IsValid() || InItem.InstanceUID <= 0 || InItem.ItemRefID <= 0){
		return;
	}

	if (CreatedTargetItemIcon == nullptr)
	{
		// Anchor ������ ���� GetSize �� ���ϴ� ���� �� ���� ���̹Ƿ� ����.
		UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(X_CP_TargetItemIconSet->Slot);
		FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// ItemIconPanelNRef ���� �������� ������ ������ widget ����
		CreatedTargetItemIcon = Cast<UB2DynItemIcon_ItemOpScene>(DynCreateInCanvasPanel(
			ItemIconWidgetClass,
			this,
			X_CP_TargetItemIconSet.Get(),
			DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f),
			AllowedIconSize,
			FVector2D(0.0f, 0.0f),
			0,
			false));
	}

	if (CreatedTargetItemIcon)
	{
		// ������ �߽� ���� ��ũ�� ���� ��ǥ�� ���´�.
		// X_CP_TargetItemIconSet �� ��ġ ������ HACKGetWidgetScreenCoord �� ����		
		HACKGetWidgetScreenCoord(X_CP_TargetItemIconSet.Get(), GetOwningPlayer(), TargetItemIconScreenPos, true);
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);
		CreatedTargetItemIcon->UpdateItemData(InItem);
	}
}

void UB2LobbyUI_ItemLevelupProg::SetIngredItemData(const TArray<FB2Item>& AllIngreds)
{
	DestroyItemLevelupIngredientItemIcon();
	DestroySuckingFXComps();
	
	if (ItemIconWidgetClass == NULL || !X_CP_EnhanceIngredIconPanel.IsValid() || AllIngreds.Num() == 0){
		return;
	}

	const FVector2D AllowedSingleSlotSize = GetAllowedSingleIngredientIconSize(); // Main panel ������ ���� ��� ������
	const FVector2D CenterPos = GetIngredIconPanelCenterPos();
		
	const int32 MaxRowIndex = (AllIngreds.Num() - 1) / LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;

	for (int32 II = 0; II < AllIngreds.Num(); ++II)
	{
		const FB2Item& ThisIngred = AllIngreds[II];

		////////////////////////////// ��ǥ ��� BEGIN
		const int32 RowIndex = II / LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;
		// �̹� �ٿ��� ���� ����. ù ���� �� ������� ���� ���� �߰��� ������ ��ġ
		const int32 HPlacementIndex = II % LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;

		float PosX = CenterPos.X;
		float PosY = CenterPos.Y;

		if (RowIndex == 0) // ù��° ���� ���Ϳ��� �����ؼ� Ȧ/¦ ���� � ���� ��ǥ ���ؼ� �¿�� ��������.
		{
			// �̹� row �� ù��° ��� �ε���
			const int32 MyRowStartIndex = RowIndex * LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;
			// �̰� �̹� ���� �� ���� �ʴ´ٸ� �迭 ũ�Ⱑ ��.
			const int32 NextRowStartIndex = FMath::Min(AllIngreds.Num(), MyRowStartIndex + LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW);
			// �̹� �ٿ� ä���� ��ü ������ ����
			const int32 TotalNumOfThisRow = NextRowStartIndex - MyRowStartIndex;
			const bool bRowHasEvenElem = (TotalNumOfThisRow % 2 == 0); // Ȧ��/¦�� ������ ���� ��ġ ��ǥ�� �ٲ�.
			const bool bAmIEvenIndex = (HPlacementIndex % 2 == 0);
			const bool bIsAtLeft = (bRowHasEvenElem == bAmIEvenIndex); // Ȧ���϶� 0��° ��Ҵ� �����δ� �߾�

			if (bRowHasEvenElem)
			{
				float AbsDisplacement = ((float)((HPlacementIndex + 2) / 2) * (IngredientIconPlacementMargin + AllowedSingleSlotSize.X)) - (AllowedSingleSlotSize.X * 0.5f);
				PosX += (bAmIEvenIndex ? -1.0f : 1.0f) * AbsDisplacement;
			}
			else
			{
				if (HPlacementIndex != 0) // Ȧ���� ��ġ���� ó�� �ε����� ���Ϳ� �ٷ�
				{
					float AbsDisplacement = ((float)((HPlacementIndex + 1) / 2) * (IngredientIconPlacementMargin + AllowedSingleSlotSize.X));
					PosX += (bAmIEvenIndex ? 1.0f : -1.0f) * AbsDisplacement;
				}
			}
		}
		else
		{ // �̹� ���� Ȧ¦���� ���ο� ������� ù��° �ٿ� ��ġ�� ���� ���󰡾� �� ��. CreatedIngredIconsSet ���ĵ� ù°�� �������
			if (HPlacementIndex < CreatedIngredIconsSet.Num())
			{
				PosX = CreatedIngredIconsSet[HPlacementIndex].RelativeCoord.X;
				PosY = CreatedIngredIconsSet[HPlacementIndex].RelativeCoord.Y;
			}

			PosX += (float)RowIndex * IngredientIconNextRowOffset.X; // �ٸ��� offset
			PosY += (float)RowIndex * IngredientIconNextRowOffset.Y;
		}

		FItemLevelupProgIngredItemIconSet NewIconSet;
		NewIconSet.RelativeCoord = FVector2D(PosX, PosY); // �»�� �������� ���� �����Ǳ� ���� ���� ������ ����. ��ƼŬ �����ÿ� ���ɼ���.

		PosX -= (AllowedSingleSlotSize.X * 0.5f); // ���������� �»�� ���� ��ǥ��.
		PosY -= (AllowedSingleSlotSize.Y * 0.5f);
		////////////////////////////// ��ǥ ��� END

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// MainPanelNativeRef ���� �������� ������ ������ widget ����. RowIndex ���� �����ϴ� Z-order �� �ش�. �տ������� ���󰡸鼭 ȿ���� ���̵��� �ϱ� ����.
		UB2DynItemIcon_ItemOpScene* NewItemIcon = Cast<UB2DynItemIcon_ItemOpScene>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, X_CP_EnhanceIngredIconPanel.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedSingleSlotSize,
			FVector2D(PosX, PosY), MaxRowIndex - RowIndex, false
			));
		if (NewItemIcon)
		{
			NewItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemLevelupMenuIngredient);
			NewItemIcon->UpdateItemData(ThisIngred); // ���� ������ ������ �Ѱ��ش�.
						
			NewIconSet.DynIcon = NewItemIcon;
			CreatedIngredIconsSet.Add(NewIconSet);
			
			if (RowIndex == 0)
			{
				// ù �ٿ� ���ؼ� ����� ��ǥ �迭�� RowIndex �� �ƴ� X ��ǥ ũ���� �����Ѵ�. ��° �� ���ĺ��ʹ� �� ������� ���� �� ��. (������ ��° �� ���ĵ� ������ �� ����)
				struct FFirstRowIngredIconPosSorter
				{
					bool operator()(const FItemLevelupProgIngredItemIconSet& A, const FItemLevelupProgIngredItemIconSet& B) const { return A.RelativeCoord.X < B.RelativeCoord.X; }
				};
				CreatedIngredIconsSet.Sort(FFirstRowIngredIconPosSorter());
			}
		}
	}

	// �� ������ CreatedTargetItemIcon �� �����Ǿ� �־�� ��.
	BII_CHECK(CreatedTargetItemIcon);
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetTotalEnhanceIngredNum(AllIngreds.Num());
	}

	if (GetOwningPlayer() && CreatedIngredIconsSet.Num() > 0)
	{
		// ������� sucking ȿ�� ����..
		if (ItemSuckingStartTime > 0.0f)
		{
			GetOwningPlayer()->GetWorldTimerManager().SetTimer(ItemSuckingIntervalTimer, FTimerDelegate::CreateUObject(this, &UB2LobbyUI_ItemLevelupProg::BeginIngredItemSucking, 0), ItemSuckingStartTime, false);
		}
		else
		{
			BeginIngredItemSucking(0);
		}
	}
}

int32 UB2LobbyUI_ItemLevelupProg::GetAllIngredNum()
{
	return CreatedIngredIconsSet.Num();
}

void UB2LobbyUI_ItemLevelupProg::UpdateStaticWidgets()
{
}

void UB2LobbyUI_ItemLevelupProg::DestroyTargetItemIcon()
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->DestroySelf();
		CreatedTargetItemIcon = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyItemLevelupIngredientItemIcon()
{
	for (FItemLevelupProgIngredItemIconSet& CurrElem : CreatedIngredIconsSet)
	{
		if (CurrElem.DynIcon){
			CurrElem.DynIcon->DestroySelf();
		}
	}
	CreatedIngredIconsSet.Empty();
}

void UB2LobbyUI_ItemLevelupProg::DestroySuckingFXComps()
{
	for (FItemLevelupProgIngredSuckingFxSet& CurrFxSet : CreatedSuckingFxSet)
	{
		if (CurrFxSet.FXComp){
			CurrFxSet.FXComp->ConditionalBeginDestroy();
		}
	}
	CreatedSuckingFxSet.Empty();

	APlayerController* LocalOwningPlayer = GetOwningPlayer();
	//if (LocalOwningPlayer && LocalOwningPlayer->IsValidObj())
	{
		LocalOwningPlayer->GetWorldTimerManager().ClearTimer(ItemSuckingIntervalTimer);
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyOverallBGFx()
{
	if (CreatedOverallBGFx)
	{
		CreatedOverallBGFx->ConditionalBeginDestroy();
		CreatedOverallBGFx = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyEnhanceBlowupFx()
{
	if (CreatedEnhanceBlowupFx)
	{
		CreatedEnhanceBlowupFx->ConditionalBeginDestroy();
		CreatedEnhanceBlowupFx = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyTargetItemBGFx()
{
	if (CreatedTargetItemBGFx)
	{
		CreatedTargetItemBGFx->ConditionalBeginDestroy();
		CreatedTargetItemBGFx = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyFinalEnhanceSuccessFxComp()
{
	if (CreatedFinalEnhanceSuccessFx)
	{
		CreatedFinalEnhanceSuccessFx->ConditionalBeginDestroy();
		CreatedFinalEnhanceSuccessFx = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::BeginIngredItemSucking(int32 IngredIndex)
{
	if (CreatedIngredIconsSet.IsValidIndex(IngredIndex) && ItemSuckingTrailPS)
	{
		FVector FXCreateWorldPos = GetIngredIconDesiredWorldPos(IngredIndex); // �� ������ ��ũ�� ��ǥ�� �ش��ϴ� ���� ��ǥ ����. (FxCreatingDepth ��ŭ �Ʒ�)

		FTransform FXCreateTransform(FXCreateWorldPos);
		UParticleSystemComponent* CreatedSuckingFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemSuckingTrailPS, FXCreateTransform);

		if (CreatedSuckingFX)
		{
			CreatedSuckingFX->SetWorldScale3D(ItemSuckingTrailScale);

			FItemLevelupProgIngredSuckingFxSet NewFxSet;
			NewFxSet.FXComp = CreatedSuckingFX;

			// Ÿ�ٱ��� �̵��� ���� ��ǥ �Ÿ��� �־����. �� �̻� �̵��ϸ� ���� �ɷ� �ؼ� �ӵ��� �����ٰ� ��ĥ �� ������.
			NewFxSet.CurrentSpeed = 0.0f;
			NewFxSet.FlightDistSoFar = 0.0f;
			NewFxSet.FlightTimeSoFar = 0.0f;
			NewFxSet.TotalDistance = (GetTargetIconDesiredWorldPos(ItemSuckingTrailDepth) - CreatedSuckingFX->GetComponentLocation()).Size();

			UB2DynItemIcon_ItemOpScene* ThisIngredIcon = CreatedIngredIconsSet[IngredIndex].DynIcon;
			SetupNamedMIDForFxComp(NewFxSet.FXComp); // �ʿ��� MIC �鿡�� MID �� ���� ��� �����ϵ��� ����.
			// Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.
			{
				UMaterialInstanceDynamic* ItemBGPanelMID = GetFxMID_ItemBGPanel(NewFxSet.FXComp);
				UMaterialInstanceConstant* ItemBGPanelMIC_Ref = ThisIngredIcon ? ThisIngredIcon->GetBGPanelMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);
			}{
				UMaterialInstanceDynamic* ItemIconMID = GetFxMID_ItemIcon(NewFxSet.FXComp);
				UMaterialInstanceConstant* ItemIconMIC_Ref = ThisIngredIcon ? ThisIngredIcon->GetMainIconMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
			}
			
			OnSingleIngredBeginSucking(IngredIndex);

			CreatedSuckingFxSet.Add(NewFxSet);
		}

		if (IngredIndex == 0)
		{ // ù��° ��� ���ۿ� ���� �̺�Ʈ.
			StartSfx_Phase2_FirstIngredient();
			OnFirstIngredBeginSucking_BP();
		}
	}	

	// ���� ���� �����ϱ� ���� Ÿ�̸�
	APlayerController* LocalOwningPlayer = GetOwningPlayer();
	//if (LocalOwningPlayer && LocalOwningPlayer->IsValidObj())
	{
		LocalOwningPlayer->GetWorldTimerManager().ClearTimer(ItemSuckingIntervalTimer);
		if (CreatedIngredIconsSet.IsValidIndex(IngredIndex + 1))
		{
			LocalOwningPlayer->GetWorldTimerManager().SetTimer(ItemSuckingIntervalTimer, FTimerDelegate::CreateUObject(this, &UB2LobbyUI_ItemLevelupProg::BeginIngredItemSucking, IngredIndex + 1), FMath::Max(ItemSuckingInterval, 0.01f), false);
		}
		else
		{ // ��� ������ ��. ���� ���� üũ�� �����ϵ��� ǥ��
			bAllIngredSuckingFxCreated = true;
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::UpdateSingleSuckingFxComp(int32 IngredIndex, float InDeltaSec)
{
	if (CreatedSuckingFxSet.IsValidIndex(IngredIndex))
	{
		// �ܼ��� �̷� ���ӵ�, �ӵ� �������� ���� �ȵǸ� projectile �̶�.

		FItemLevelupProgIngredSuckingFxSet& CurrFxSet = CreatedSuckingFxSet[IngredIndex];
		if (CurrFxSet.FXComp)
		{
			FVector FXCompCurrPos = CurrFxSet.FXComp->GetComponentLocation();
			FVector ToTargetDir = (GetTargetIconDesiredWorldPos(ItemSuckingTrailDepth) - FXCompCurrPos);
			ToTargetDir.Normalize();

			// ���� dV = At, dS = Vt
			CurrFxSet.CurrentSpeed += (EffectiveItemSuckingAcc * InDeltaSec);
			CurrFxSet.CurrentSpeed = FMath::Min(ItemSuckingMaxSpeed, CurrFxSet.CurrentSpeed);
			FVector ThisMoveAmount = ToTargetDir * ((CurrFxSet.CurrentSpeed) * InDeltaSec);

			CurrFxSet.FXComp->SetWorldLocation(FXCompCurrPos + ThisMoveAmount);

			// �󸶳� ����������� üũ�ϴ� ��� �̵� �Ÿ��� ���ؼ� ó���� ������ �Ÿ��� �Ѿ����� ��. �ʹ� ���� �̵��ص� �� ��ħ.
			CurrFxSet.FlightDistSoFar += ThisMoveAmount.Size();
			CurrFxSet.FlightTimeSoFar += InDeltaSec;

			const bool bDestroyThisOne = (
				(ItemSuckingTrailLifeTime > 0.0f && CurrFxSet.FlightTimeSoFar >= ItemSuckingTrailLifeTime) ||
				(CurrFxSet.FlightDistSoFar >= CurrFxSet.TotalDistance) // �ð��� �������� �ʾҴٸ� ���������� �ı�.
				); 

			if (bDestroyThisOne)
			{
				OnSingleIngredEndSucking(IngredIndex); // �ϳ� ���Ƶ��� �Ϳ� ���� �̺�Ʈ ��

				CurrFxSet.FXComp->ConditionalBeginDestroy(); // ��ǥ ���� ���� �� FXComp ����. CurrFxSet ��ü�� CreatedSuckingFxSet ���� ���������� �ʴ´�. üũ�� �� �ʿ��ؼ�.
				CurrFxSet.FXComp = NULL;

				CheckForAllFxSucked(); // ��� ���Ƶ鿴���� �˻�.
			}
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::CheckForAllFxSucked()
{
	// ��� ��ŷ �Ǿ����� �˻��ؼ� �̺�Ʈ�� �����µ� 
	// �ܼ��� IngredIndex == GetAllIngredNum() - 1 �̷� ������ ���ؼ� ������ ��ŷ���� �˻��ص� ū ���� ���� �� ������ ���� �ϳ� �� �ε��� ���� �ڴʰ� �����ϴ� ���°� ������ ���� ����.
	if (bAllIngredSuckingFxCreated)
	{
		bool bAllSucked = true;
		for (FItemLevelupProgIngredSuckingFxSet& EndCheckFxSet : CreatedSuckingFxSet)
		{
			if (EndCheckFxSet.FXComp){
				bAllSucked = false;
				break;
			}
		}
		if (bAllSucked){
			OnFinalIngredJustSucked();
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::OnFinalIngredJustSucked()
{
	// ������ ��ᰡ ���Ե� �������ʹ� ������ Ÿ���̹Ƿ� ���� ȿ�� Ÿ�ֿ̹� ���� ��.
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->OnTargetItemLastSucking();
	}
	StartSfx_Phase3_FinalSucked(CachedInventory->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume);
	OnFinalIngredJustSucked_BP();
}

void UB2LobbyUI_ItemLevelupProg::UpdateTargetItemToNewData()
{
	FB2Item PreviewItem = NativeTargetLevelupItem_After;

	if (CachedInventory)
	{ 
		// �ռ� �������� ���� �����Ƿ� ������Ʈ �� ������ �ٽ� ����. ������ ��츸 �ٽ� ������ �ұ�..
		if (FinalResultPageNRef && CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_After, false))
			FinalResultPageNRef->UpdateItemData(NativeTargetLevelupItem_Before, NativeTargetLevelupItem_After);

		// �����ΰ�� ��밪���� ���� �����ְ� ���Ŀ� ���� �����ͷ� �����ؼ� ������
		if (CachedInventory->IsItemLevelupJackpot())
		{
			int32 OutLevel = 0;
			float OutExp = 0;
			TArray<FB2Item> CachedIngredItems;
			CachedInventory->GetAllGhostIngreds(CachedIngredItems);
			if (CachedIngredItems.Num() > 0)
				OutLevel = GetLevel_PreviewItemLevelup(OutExp, NativeTargetLevelupItem_Before, CachedIngredItems, true);

			PreviewItem.Level = OutLevel;
			PreviewItem.Exp = OutExp;
		}
	}
	
	SetTargetItemIconData(PreviewItem); // �̸� ������ ������Ʈ �� ������ �����ͷ� ����.
}

void UB2LobbyUI_ItemLevelupProg::ShowOverallBGFx()
{
	DestroyOverallBGFx();
	int32 ViewSizeX = (int32)DesignedRefViewSize.X;
	int32 ViewSizeY = (int32)DesignedRefViewSize.Y;
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY); // ȭ�� �߾ӿ� ����
		CreatedOverallBGFx = OverallBGFxSetup.CreateUnderScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)));
	}	
}

void UB2LobbyUI_ItemLevelupProg::ShowEnhanceBlowupFx()
{
	DestroyEnhanceBlowupFx();
	int32 ViewSizeX = (int32)DesignedRefViewSize.X;
	int32 ViewSizeY = (int32)DesignedRefViewSize.Y;
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY); // ȭ�� �߾ӿ� ����
		CreatedEnhanceBlowupFx = EnhanceBlowupFxSetup.CreateUnderScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)));

		if (CreatedEnhanceBlowupFx && CreatedTargetItemIcon)
		{
			SetupNamedMIDForFxComp(CreatedEnhanceBlowupFx); // �ʿ��� MIC �鿡�� MID �� ���� ��� �����ϵ��� ����.
			// Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.
			{
				UMaterialInstanceDynamic* ItemBGPanelMID = GetFxMID_ItemBGPanel(CreatedEnhanceBlowupFx);
				UMaterialInstanceConstant* ItemBGPanelMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetBGPanelMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);
			} {
				UMaterialInstanceDynamic* ItemIconMID = GetFxMID_ItemIcon(CreatedEnhanceBlowupFx);
				UMaterialInstanceConstant* ItemIconMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetMainIconMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
			}
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::ShowTargetItemBGFx()
{
	DestroyTargetItemBGFx();
	CreatedTargetItemBGFx = TargetItemBGFxSetup.CreateUnderScreen(GetOwningPlayer(), TargetItemIconScreenPos);

	SetupNamedMIDForFxComp(CreatedTargetItemBGFx); // �ʿ��� MIC �鿡�� MID �� ���� ��� �����ϵ��� ����.
	// Fx �ʿ� MID ���� �غ� �Ǿ��ٸ� ��Ʈ���� �Ķ���� ���� ������ UI ���� MIC �� �����ͼ� MID �� �Ķ���͸� ����.
	{
		UMaterialInstanceDynamic* ItemBGPanelMID = GetFxMID_ItemBGPanel(CreatedTargetItemBGFx);
		UMaterialInstanceConstant* ItemBGPanelMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetBGPanelMIC() : NULL;
		SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);
	}{
		UMaterialInstanceDynamic* ItemIconMID = GetFxMID_ItemIcon(CreatedTargetItemBGFx);
		UMaterialInstanceConstant* ItemIconMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetMainIconMIC() : NULL;
		SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
	}
}

void UB2LobbyUI_ItemLevelupProg::ShowFinalEnhanceSuccessFx()
{
	DestroyFinalEnhanceSuccessFxComp();
	CreatedFinalEnhanceSuccessFx = FinalEnhanceSuccessFxSetup.CreateUnderScreen(GetOwningPlayer(), TargetItemIconScreenPos);
}

void UB2LobbyUI_ItemLevelupProg::SetTargetItemIconVisible(bool bVisible)
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
void UB2LobbyUI_ItemLevelupProg::SetTargetItemIconOverlayOnly(bool bOverlayOnly)
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetShowOverlayInfoOnly(!bOverlayOnly, true);
	}
}

void UB2LobbyUI_ItemLevelupProg::ShowFinalResultPage()
{
	if (FinalResultPageNRef)
	{
		FinalResultPageNRef->SetVisibility(ESlateVisibility::Visible);
		FinalResultPageNRef->OnVisibleUpdate();

		SetTargetItemIconVisible(false);
		FinalResultPageNRef->SetShowMainItemIcon(true);
	}
}

void UB2LobbyUI_ItemLevelupProg::ShowBattleScorePopup()
{  // �ִϸ��̼� ���� ����� ȣ��Ǵ� ������ ���� �˾� 
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(IntToPCClass(NativeTargetLevelupItem_After.AllowedPCClass));
}

void UB2LobbyUI_ItemLevelupProg::ShowCloseButton()
{
	if (FinalResultPageNRef)
	{
		FinalResultPageNRef->ShowBackButton();
	}
}

void UB2LobbyUI_ItemLevelupProg::OnSingleIngredBeginSucking(int32 IngredIndex)
{
	if (CreatedIngredIconsSet.IsValidIndex(IngredIndex))
	{
		FItemLevelupProgIngredItemIconSet& ThisSet = CreatedIngredIconsSet[IngredIndex];
		if (ThisSet.DynIcon)
		{
			ThisSet.DynIcon->OnIngredItemBeginSucking(IngredIndex); // ��� ������ �����ܿ� ������ �̺�Ʈ
		}
	}
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->OnTargetItemBeginSucking(IngredIndex); // Ÿ�� ������ �����ܿ� ������ �̺�Ʈ
	}

	StartSfx_Phase2N_EachIngredient();
}

void UB2LobbyUI_ItemLevelupProg::OnSingleIngredEndSucking(int32 IngredIndex)
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->OnTargetItemSuckedIngred(IngredIndex); // Ÿ�� ������ �����ܿ� ������ �̺�Ʈ.
	}
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetAllowedSingleIngredientIconSize()
{
	FVector2D ReturnSize(0.0f, 0.0f);

	// Anchor ������ ���� GetSize �� ���ϴ� ���� �� ���� ���̹Ƿ� ����.
	UCanvasPanelSlot* MainPanelSlot = X_CP_EnhanceIngredIconPanel.IsValid() ? Cast<UCanvasPanelSlot>(X_CP_EnhanceIngredIconPanel->Slot) : NULL;
	if (MainPanelSlot)
	{
		FVector2D MainPanelSize = MainPanelSlot->GetSize();

		// MainPanelSize �� Margin �� ����..
		ReturnSize.X = ((MainPanelSize.X / (float)LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW) - IngredientIconPlacementMargin);
		ReturnSize.Y = MainPanelSize.Y;

		ReturnSize.X = FMath::Min(ReturnSize.X, IngredientIconMaxSize.X);
		ReturnSize.Y = FMath::Min(ReturnSize.Y, IngredientIconMaxSize.Y);
	}

	return ReturnSize;
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetIngredIconPanelCenterPos()
{
	// Anchor ������ ���� GetSize �� ���ϴ� ���� �� ���� ���̹Ƿ� ����.
	UCanvasPanelSlot* MainPanelSlot = X_CP_EnhanceIngredIconPanel.IsValid() ? Cast<UCanvasPanelSlot>(X_CP_EnhanceIngredIconPanel->Slot) : NULL;
	return MainPanelSlot ? (MainPanelSlot->GetSize() * FVector2D(0.5f, 0.5f)) : FVector2D(0.0f, 0.0f);
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetIngredIconScreenPos(int32 IconIndex)
{
	if (CreatedIngredIconsSet.IsValidIndex(IconIndex))
	{
		FVector2D IngredPanelScreenCoord;
		
		// X_CP_EnhanceIngredIconPanel �� ��ġ ������ HACKGetWidgetScreenCoord �� ����
		if (HACKGetWidgetScreenCoord(X_CP_EnhanceIngredIconPanel.Get(), GetOwningPlayer(), IngredPanelScreenCoord, false, true))
		{			
			return ( // �г� ��ǥ�� Ingredient icon ���� ������ ��� ��ǥ�� ����. 
				IngredPanelScreenCoord +
				(CreatedIngredIconsSet[IconIndex].RelativeCoord * GetDesignToCurrentViewScale(GetOwningPlayer(), true)) 
				);
		}
	}
	return FVector2D(0.0f, 0.0f);
}

FVector UB2LobbyUI_ItemLevelupProg::GetIngredIconDesiredWorldPos(int32 IconIndex)
{
	FVector2D ScreenPos = GetIngredIconScreenPos(IconIndex);
	return GetWorldPosBeneathScreen(GetOwningPlayer(), ScreenPos, ItemSuckingTrailDepth);
}

FVector UB2LobbyUI_ItemLevelupProg::GetTargetIconDesiredWorldPos(float Depth)
{
	return GetWorldPosBeneathScreen(GetOwningPlayer(), TargetItemIconScreenPos, Depth);
}

void UB2LobbyUI_ItemLevelupProg::FinishAndProceedToResult()
{
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, 1.0f); // OverallPlayRate �� ���� �����ߴ� �� ����.

	QuitItemOpModeClass<bool>::GetInstance().Signal(true); // �����δ� ���â�� ���;� ��. �ű⼭ Quit �� �ϰ� �� ��.
}
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
	ItemSuckingTrailDepth = 5.0f; // 이게 크면 좀 곤란. 2D 랑 3D 의 맵핑과 관련이 되다보니.
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

	// 단순히 아이템 정보만 얻어오려면 Inventory 인스턴스 없이 static 으로 얻어올 수 있으나 강화 기능은 인벤토리가 떠 있는 상태에서 동작.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);
	
	EffectiveItemSuckingAcc = ItemSuckingBaseAcc; // 등가속도(ItemSuckingAccAcc 0)에서는 초기값이 유지됨

	// 업데이트 되기 전/후 타겟 아이템 데이터가 필요 (UB2LobbyInventory::OnReqForItemOpCommon 에서 백업) 
	bool bFailedToFindResultData = !CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_Before, true);
	if (bFailedToFindResultData || !IsSupportedOpMode(CachedInventory->GetItemOpMode()))
	{
		FinishAndProceedToResult(); // 만에 하나 실패한다면.. 어쩔 수 없지.
		return;
	}

	// 이 시점에 결과 데이터를 가져오지 못할 수도 있음. 테스트 커맨드 사용이거나 연출 시작 시점이 좀 다르게 되었다거나..
	if (!CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_After, false))
		NativeTargetLevelupItem_After = NativeTargetLevelupItem_Before; // 실패한 경우 일단은 이전 데이터를 넣어준다.

	if (FinalResultPageNRef) // 이건 동적 생성은 아니다. 단, 몇가지는 직접 해 주어야.
	{
		FinalResultPageNRef->StartFromLobby(InUIManager, InGM);
		FinalResultPageNRef->UpdateItemData(NativeTargetLevelupItem_Before, NativeTargetLevelupItem_After);
		FinalResultPageNRef->ClosePopup(); // 처음에는 숨겨놓음. 연출 최종 단계로서 보여진다.
	}
		
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, OverallPlayRate);

	ApplyCameraAdaptiveFOV();

	SetTargetItemIconData(NativeTargetLevelupItem_Before); // Before 로 시작.

	DestroyItemLevelupIngredientItemIcon();
	DestroySuckingFXComps();
	OnFinalIngredJustSucked();
}

void UB2LobbyUI_ItemLevelupProg::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// Sucking Fx 들의 움직임 업데이트.
	bool bAtLeastOneSuckingFxPresent = false;
	for (int32 FI = 0; FI < CreatedSuckingFxSet.Num(); ++FI)
	{
		if (CreatedSuckingFxSet[FI].FXComp){ // 도착 후에는 FXComp 가 제거되고 NULL 이 될 것.
			bAtLeastOneSuckingFxPresent = true;
			UpdateSingleSuckingFxComp(FI, InDeltaTime);
		}
	}
	if (bAtLeastOneSuckingFxPresent)
	{
		EffectiveItemSuckingAcc += (ItemSuckingAccAcc * InDeltaTime); // 하나라도 있어야 가속도 증가.
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
	// 이 장면 셋업에서 전용 카메라 액터가 ViewTarget 으로 세팅되어 있어야 함.
	ACameraActor* ViewTargetCam = OwningPC ? Cast<ACameraActor>(OwningPC->GetViewTarget()) : nullptr;
	checkSlow(ViewTargetCam);
	if (ViewTargetCam && ViewTargetCam->GetCameraComponent())
	{ // AspectRatioAdaptiveFOV 를 사용하면 기준 해상도 비율보다 큰 화면 비율에서 장면이 잘리는 걸 피할 수 있지만
		// 대신 넓어진 시야로 인해 감추었던 게 노출되는 일이 발생할 수 있다. 상황에 따라 적절하게..
		// 또한 2D-3D 요소간 정렬이 어긋나게 될 수도 있다. 이건 특히 다양한 해상도 비율에서 테스트 필요.
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
		// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
		UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(X_CP_TargetItemIconSet->Slot);
		FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// ItemIconPanelNRef 위에 최종적인 아이템 아이콘 widget 생성
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
		// 아이콘 중심 기준 스크린 공간 좌표를 얻어온다.
		// X_CP_TargetItemIconSet 의 배치 조건은 HACKGetWidgetScreenCoord 를 참고		
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

	const FVector2D AllowedSingleSlotSize = GetAllowedSingleIngredientIconSize(); // Main panel 구성에 따른 허용 사이즈
	const FVector2D CenterPos = GetIngredIconPanelCenterPos();
		
	const int32 MaxRowIndex = (AllIngreds.Num() - 1) / LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;

	for (int32 II = 0; II < AllIngreds.Num(); ++II)
	{
		const FB2Item& ThisIngred = AllIngreds[II];

		////////////////////////////// 좌표 계산 BEGIN
		const int32 RowIndex = II / LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;
		// 이번 줄에서 나의 순서. 첫 줄은 이 순서대로 가로 방향 중간에 가깝게 배치
		const int32 HPlacementIndex = II % LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;

		float PosX = CenterPos.X;
		float PosY = CenterPos.Y;

		if (RowIndex == 0) // 첫번째 줄은 센터에서 시작해서 홀/짝 여부 등에 따라 좌표 정해서 좌우로 퍼져나감.
		{
			// 이번 row 의 첫번째 요소 인덱스
			const int32 MyRowStartIndex = RowIndex * LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;
			// 이건 이번 줄이 다 차지 않는다면 배열 크기가 됨.
			const int32 NextRowStartIndex = FMath::Min(AllIngreds.Num(), MyRowStartIndex + LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW);
			// 이번 줄에 채워질 전체 아이콘 개수
			const int32 TotalNumOfThisRow = NextRowStartIndex - MyRowStartIndex;
			const bool bRowHasEvenElem = (TotalNumOfThisRow % 2 == 0); // 홀수/짝수 인지에 따라 배치 좌표가 바뀜.
			const bool bAmIEvenIndex = (HPlacementIndex % 2 == 0);
			const bool bIsAtLeft = (bRowHasEvenElem == bAmIEvenIndex); // 홀수일때 0번째 요소는 실제로는 중앙

			if (bRowHasEvenElem)
			{
				float AbsDisplacement = ((float)((HPlacementIndex + 2) / 2) * (IngredientIconPlacementMargin + AllowedSingleSlotSize.X)) - (AllowedSingleSlotSize.X * 0.5f);
				PosX += (bAmIEvenIndex ? -1.0f : 1.0f) * AbsDisplacement;
			}
			else
			{
				if (HPlacementIndex != 0) // 홀수개 배치에서 처음 인덱스는 센터에 바로
				{
					float AbsDisplacement = ((float)((HPlacementIndex + 1) / 2) * (IngredientIconPlacementMargin + AllowedSingleSlotSize.X));
					PosX += (bAmIEvenIndex ? 1.0f : -1.0f) * AbsDisplacement;
				}
			}
		}
		else
		{ // 이번 줄이 홀짝인지 여부에 상관없이 첫번째 줄에 배치한 것을 따라가야 할 것. CreatedIngredIconsSet 정렬된 첫째줄 순서대로
			if (HPlacementIndex < CreatedIngredIconsSet.Num())
			{
				PosX = CreatedIngredIconsSet[HPlacementIndex].RelativeCoord.X;
				PosY = CreatedIngredIconsSet[HPlacementIndex].RelativeCoord.Y;
			}

			PosX += (float)RowIndex * IngredientIconNextRowOffset.X; // 줄마다 offset
			PosY += (float)RowIndex * IngredientIconNextRowOffset.Y;
		}

		FItemLevelupProgIngredItemIconSet NewIconSet;
		NewIconSet.RelativeCoord = FVector2D(PosX, PosY); // 좌상단 기준으로 최종 조절되기 전에 따로 저장해 놓음. 파티클 생성시에 사용될수도.

		PosX -= (AllowedSingleSlotSize.X * 0.5f); // 최종적으로 좌상단 기준 좌표로.
		PosY -= (AllowedSingleSlotSize.Y * 0.5f);
		////////////////////////////// 좌표 계산 END

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// MainPanelNativeRef 위에 최종적인 아이템 아이콘 widget 생성. RowIndex 마다 감소하는 Z-order 를 준다. 앞에꺼부터 날라가면서 효과가 보이도록 하기 위함.
		UB2DynItemIcon_ItemOpScene* NewItemIcon = Cast<UB2DynItemIcon_ItemOpScene>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, X_CP_EnhanceIngredIconPanel.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedSingleSlotSize,
			FVector2D(PosX, PosY), MaxRowIndex - RowIndex, false
			));
		if (NewItemIcon)
		{
			NewItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemLevelupMenuIngredient);
			NewItemIcon->UpdateItemData(ThisIngred); // 개별 아이템 정보를 넘겨준다.
						
			NewIconSet.DynIcon = NewItemIcon;
			CreatedIngredIconsSet.Add(NewIconSet);
			
			if (RowIndex == 0)
			{
				// 첫 줄에 한해서 저장된 좌표 배열은 RowIndex 가 아닌 X 좌표 크기대로 정렬한다. 둘째 줄 이후부터는 이 순서대로 차게 될 것. (내지는 둘째 줄 이후도 정렬을 할 수도)
				struct FFirstRowIngredIconPosSorter
				{
					bool operator()(const FItemLevelupProgIngredItemIconSet& A, const FItemLevelupProgIngredItemIconSet& B) const { return A.RelativeCoord.X < B.RelativeCoord.X; }
				};
				CreatedIngredIconsSet.Sort(FFirstRowIngredIconPosSorter());
			}
		}
	}

	// 이 시점에 CreatedTargetItemIcon 이 생성되어 있어야 함.
	BII_CHECK(CreatedTargetItemIcon);
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetTotalEnhanceIngredNum(AllIngreds.Num());
	}

	if (GetOwningPlayer() && CreatedIngredIconsSet.Num() > 0)
	{
		// 순서대로 sucking 효과 시작..
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
		FVector FXCreateWorldPos = GetIngredIconDesiredWorldPos(IngredIndex); // 각 아이콘 스크린 좌표에 해당하는 월드 좌표 얻어옴. (FxCreatingDepth 만큼 아래)

		FTransform FXCreateTransform(FXCreateWorldPos);
		UParticleSystemComponent* CreatedSuckingFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemSuckingTrailPS, FXCreateTransform);

		if (CreatedSuckingFX)
		{
			CreatedSuckingFX->SetWorldScale3D(ItemSuckingTrailScale);

			FItemLevelupProgIngredSuckingFxSet NewFxSet;
			NewFxSet.FXComp = CreatedSuckingFX;

			// 타겟까지 이동을 위해 목표 거리를 넣어놓음. 이 이상 이동하면 끝인 걸로 해서 속도가 빠르다고 놓칠 일 없도록.
			NewFxSet.CurrentSpeed = 0.0f;
			NewFxSet.FlightDistSoFar = 0.0f;
			NewFxSet.FlightTimeSoFar = 0.0f;
			NewFxSet.TotalDistance = (GetTargetIconDesiredWorldPos(ItemSuckingTrailDepth) - CreatedSuckingFX->GetComponentLocation()).Size();

			UB2DynItemIcon_ItemOpScene* ThisIngredIcon = CreatedIngredIconsSet[IngredIndex].DynIcon;
			SetupNamedMIDForFxComp(NewFxSet.FXComp); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.
			// Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
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
		{ // 첫번째 재료 시작에 대한 이벤트.
			StartSfx_Phase2_FirstIngredient();
			OnFirstIngredBeginSucking_BP();
		}
	}	

	// 다음 것을 생성하기 위한 타이머
	APlayerController* LocalOwningPlayer = GetOwningPlayer();
	//if (LocalOwningPlayer && LocalOwningPlayer->IsValidObj())
	{
		LocalOwningPlayer->GetWorldTimerManager().ClearTimer(ItemSuckingIntervalTimer);
		if (CreatedIngredIconsSet.IsValidIndex(IngredIndex + 1))
		{
			LocalOwningPlayer->GetWorldTimerManager().SetTimer(ItemSuckingIntervalTimer, FTimerDelegate::CreateUObject(this, &UB2LobbyUI_ItemLevelupProg::BeginIngredItemSucking, IngredIndex + 1), FMath::Max(ItemSuckingInterval, 0.01f), false);
		}
		else
		{ // 모두 생성한 거. 이제 종료 체크가 가능하도록 표시
			bAllIngredSuckingFxCreated = true;
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::UpdateSingleSuckingFxComp(int32 IngredIndex, float InDeltaSec)
{
	if (CreatedSuckingFxSet.IsValidIndex(IngredIndex))
	{
		// 단순한 이런 가속도, 속도 적용으로 만족 안되면 projectile 이라도.

		FItemLevelupProgIngredSuckingFxSet& CurrFxSet = CreatedSuckingFxSet[IngredIndex];
		if (CurrFxSet.FXComp)
		{
			FVector FXCompCurrPos = CurrFxSet.FXComp->GetComponentLocation();
			FVector ToTargetDir = (GetTargetIconDesiredWorldPos(ItemSuckingTrailDepth) - FXCompCurrPos);
			ToTargetDir.Normalize();

			// 흔한 dV = At, dS = Vt
			CurrFxSet.CurrentSpeed += (EffectiveItemSuckingAcc * InDeltaSec);
			CurrFxSet.CurrentSpeed = FMath::Min(ItemSuckingMaxSpeed, CurrFxSet.CurrentSpeed);
			FVector ThisMoveAmount = ToTargetDir * ((CurrFxSet.CurrentSpeed) * InDeltaSec);

			CurrFxSet.FXComp->SetWorldLocation(FXCompCurrPos + ThisMoveAmount);

			// 얼마나 가까워졌는지 체크하는 대신 이동 거리를 더해서 처음에 산정한 거리를 넘었는지 비교. 너무 빨리 이동해도 안 놓침.
			CurrFxSet.FlightDistSoFar += ThisMoveAmount.Size();
			CurrFxSet.FlightTimeSoFar += InDeltaSec;

			const bool bDestroyThisOne = (
				(ItemSuckingTrailLifeTime > 0.0f && CurrFxSet.FlightTimeSoFar >= ItemSuckingTrailLifeTime) ||
				(CurrFxSet.FlightDistSoFar >= CurrFxSet.TotalDistance) // 시간을 지정하지 않았다면 도착했을때 파괴.
				); 

			if (bDestroyThisOne)
			{
				OnSingleIngredEndSucking(IngredIndex); // 하나 빨아들인 것에 대한 이벤트 ㅋ

				CurrFxSet.FXComp->ConditionalBeginDestroy(); // 목표 지점 도착 시 FXComp 제거. CurrFxSet 전체를 CreatedSuckingFxSet 에서 제거하지는 않는다. 체크가 좀 필요해서.
				CurrFxSet.FXComp = NULL;

				CheckForAllFxSucked(); // 모두 빨아들였는지 검사.
			}
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::CheckForAllFxSucked()
{
	// 모두 써킹 되었는지 검사해서 이벤트를 날리는데 
	// 단순히 IngredIndex == GetAllIngredNum() - 1 이런 식으로 비교해서 마지막 써킹인지 검사해도 큰 문제 없을 거 같지만 만에 하나 앞 인덱스 것이 뒤늦게 도착하는 사태가 벌어질 수도 있음.
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
	// 마지막 재료가 흡입된 순간부터는 고정된 타임이므로 전역 효과 타이밍에 좋을 것.
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
		// 앞서 실패했을 수도 있으므로 업데이트 된 데이터 다시 얻어옴. 실패한 경우만 다시 얻어오게 할까..
		if (FinalResultPageNRef && CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_After, false))
			FinalResultPageNRef->UpdateItemData(NativeTargetLevelupItem_Before, NativeTargetLevelupItem_After);

		// 잭팟인경우 기대값으로 먼저 보여주고 이후에 실제 데이터로 변경해서 보여줌
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
	
	SetTargetItemIconData(PreviewItem); // 미리 얻어놓은 업데이트 된 아이템 데이터로 세팅.
}

void UB2LobbyUI_ItemLevelupProg::ShowOverallBGFx()
{
	DestroyOverallBGFx();
	int32 ViewSizeX = (int32)DesignedRefViewSize.X;
	int32 ViewSizeY = (int32)DesignedRefViewSize.Y;
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY); // 화면 중앙에 생성
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
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY); // 화면 중앙에 생성
		CreatedEnhanceBlowupFx = EnhanceBlowupFxSetup.CreateUnderScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)));

		if (CreatedEnhanceBlowupFx && CreatedTargetItemIcon)
		{
			SetupNamedMIDForFxComp(CreatedEnhanceBlowupFx); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.
			// Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
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

	SetupNamedMIDForFxComp(CreatedTargetItemBGFx); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.
	// Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
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
{  // 애니메이션 연출 종료시 호출되는 전투력 관련 팝업 
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
			ThisSet.DynIcon->OnIngredItemBeginSucking(IngredIndex); // 재료 아이템 아이콘에 보내는 이벤트
		}
	}
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->OnTargetItemBeginSucking(IngredIndex); // 타겟 아이템 아이콘에 보내는 이벤트
	}

	StartSfx_Phase2N_EachIngredient();
}

void UB2LobbyUI_ItemLevelupProg::OnSingleIngredEndSucking(int32 IngredIndex)
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->OnTargetItemSuckedIngred(IngredIndex); // 타겟 아이템 아이콘에 보내는 이벤트.
	}
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetAllowedSingleIngredientIconSize()
{
	FVector2D ReturnSize(0.0f, 0.0f);

	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
	UCanvasPanelSlot* MainPanelSlot = X_CP_EnhanceIngredIconPanel.IsValid() ? Cast<UCanvasPanelSlot>(X_CP_EnhanceIngredIconPanel->Slot) : NULL;
	if (MainPanelSlot)
	{
		FVector2D MainPanelSize = MainPanelSlot->GetSize();

		// MainPanelSize 와 Margin 에 따라..
		ReturnSize.X = ((MainPanelSize.X / (float)LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW) - IngredientIconPlacementMargin);
		ReturnSize.Y = MainPanelSize.Y;

		ReturnSize.X = FMath::Min(ReturnSize.X, IngredientIconMaxSize.X);
		ReturnSize.Y = FMath::Min(ReturnSize.Y, IngredientIconMaxSize.Y);
	}

	return ReturnSize;
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetIngredIconPanelCenterPos()
{
	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
	UCanvasPanelSlot* MainPanelSlot = X_CP_EnhanceIngredIconPanel.IsValid() ? Cast<UCanvasPanelSlot>(X_CP_EnhanceIngredIconPanel->Slot) : NULL;
	return MainPanelSlot ? (MainPanelSlot->GetSize() * FVector2D(0.5f, 0.5f)) : FVector2D(0.0f, 0.0f);
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetIngredIconScreenPos(int32 IconIndex)
{
	if (CreatedIngredIconsSet.IsValidIndex(IconIndex))
	{
		FVector2D IngredPanelScreenCoord;
		
		// X_CP_EnhanceIngredIconPanel 의 배치 조건은 HACKGetWidgetScreenCoord 를 참고
		if (HACKGetWidgetScreenCoord(X_CP_EnhanceIngredIconPanel.Get(), GetOwningPlayer(), IngredPanelScreenCoord, false, true))
		{			
			return ( // 패널 좌표에 Ingredient icon 들이 생성된 상대 좌표를 더함. 
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
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, 1.0f); // OverallPlayRate 에 따라 변경했던 거 복구.

	QuitItemOpModeClass<bool>::GetInstance().Signal(true); // 실제로는 결과창이 나와야 함. 거기서 Quit 을 하게 될 것.
}
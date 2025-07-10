// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemCusProgBase.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "B2LobbyUI_ItemDetailPostOp.h"

float UB2LobbyUI_ItemCusProgBase::GetOverallPlayRateForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->OverallPlayRate;
	}

	return 0.0f;
}

float UB2LobbyUI_ItemCusProgBase::GetCreatingDepthForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->FxCreatingDepth;
	}

	return 0.0f;
}

float UB2LobbyUI_ItemCusProgBase::GetResultListingOffsetForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->ResultGradeStarListingOffset;
	}

	return 0.0f;
}

FVector UB2LobbyUI_ItemCusProgBase::GetStarFxScaleForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->ResultGradeStarFxScale;
	}

	return FVector(0.0f, 0.0f, 0.0f);
}

FVector2D UB2LobbyUI_ItemCusProgBase::GetResultStarCenterPosForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->ResultGradeStarCenterPos;
	}

	return FVector2D();
}

UParticleSystem* UB2LobbyUI_ItemCusProgBase::GetResultGradeStarPS(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->ResultGradeStarPS;
	}

	return nullptr;
}

FStarEffectInfo* UB2LobbyUI_ItemCusProgBase::GetStarEffectInfo(const FB2Item& InItem)
{
	return GetStarEffectInfo(InItem.StarGrade);
}

FStarEffectInfo* UB2LobbyUI_ItemCusProgBase::GetStarEffectInfo(const int32 InGrade)
{
	if (InGrade > 0 && InGrade <= FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE
		&& StarsInfo.IsValidIndex(0))
	{
		return &StarsInfo[0];
	}
	else if (InGrade > FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE && InGrade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE)
	{
		// 7성이면 1 8성이면2 9성이면 3...
		int32 StarContainerIndex = InGrade - FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE;
		if (StarsInfo.IsValidIndex(StarContainerIndex))
		{
			return &StarsInfo[StarContainerIndex];
		}
	}

	return nullptr;
}

UB2LobbyUI_ItemCusProgBase::UB2LobbyUI_ItemCusProgBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;
	
	ItemIconWidgetClass = NULL;
	CreatedItemIcon_Target = NULL;
	CreatedItemIcon_Result = NULL;

	bUseCameraAdaptiveFOV = false;
	AdaptiveFOVScale = 0.6f;
}

void UB2LobbyUI_ItemCusProgBase::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_ItemIconSet_Target);
	GET_SLOT(UCanvasPanel, X_CP_ItemIconSet_Result);
	GET_SLOT(UB2LobbyUI_ItemDetailPostOp, UIP_FinalResultPageP)
}

void UB2LobbyUI_ItemCusProgBase::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	TargetOpMode = CachedInventory->GetItemOpMode();

	// 업데이트 되기 전 타겟 아이템 데이터가 필요 (UB2LobbyInventory::OnReqForItemOpCommon 에서 백업) 
	if (!CachedInventory->GetItemOPTargetItemData(NativeItemData_Target, true) || // Result 데이터는 여기서 가져오지 않는다. 아마 response 가 온 상황일 거지만 혹 모르니..
		!IsSupportedOpMode(TargetOpMode))
	{
		FinishAndProceedToResult(); // 만에 하나 실패한다면.. 어쩔 수 없지.
		return;
	}

	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, GetOverallPlayRateForStar(NativeItemData_Target));

	ApplyCameraAdaptiveFOV();

	SetItemData_Target(NativeItemData_Target);

	// 업데이트 된 결과 아이템 데이터를 가져오기 위함. 아직 아이콘 세팅은 하지 않는다.
	// 아직 response 가 오지 않은 타이밍이라면 필요할 때 다시 가져올 것.
	if (!CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false)){
		// 어떤 이유로든 결과를 가져오지 못한다면 request 이전의 캐싱된 타겟 데이터를 넣는다.
		NativeItemData_Result = NativeItemData_Target;
	}

	InitFinalResultPage(InUIManager, InGM);

	UpdateStaticWidgets();
}

void UB2LobbyUI_ItemCusProgBase::DestroySelf()
{
	Super::DestroySelf();

	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->DestroySelf();
	}

	DestroyItemIcon_Target();
	DestroyItemIcon_Result();

	DestroyAllFx();
}

void UB2LobbyUI_ItemCusProgBase::UpdateStaticWidgets()
{
}

void UB2LobbyUI_ItemCusProgBase::StartResultGradeStarFx()
{
	DestroyResultGradeStarFx(); // 여기서 DestroyAllFx 하면 안되지롱.

	if (!GetResultGradeStarPS(NativeItemData_Result)){
		return;
	}
	
	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // 굳이 필요할까 싶긴 하지만.
	
	auto* StarEffectInfo = GetStarEffectInfo(NativeItemData_Result);
	auto* OwningPC = GetOwningPlayer();

	FVector FxCreateRefPos(0.0f,0.0f,0.0f); // 맨 처음 별을 생성하는 젤 좌측 좌표.
	FVector FxCreatePosOffsetDir(1.0f, 0.0f, 0.0f); // 월드 공간에서의 화면 가로 방향. 별들을 생성할 때 이 방향으로 옵셋을 줄 것.

	if (OwningPC && StarEffectInfo)
	{
		StarEffectInfo->ResultGradeStarCenterPos.X = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.X, 0.0f, 0.9f); // 이거보다 약간 오른쪽에서도 위치를 구할 거라 X 좌표 제한이 좀 있음. 대체로 화면 중앙이겠지만.

		// ResultGradeStarCenterPos 는 0 ~ 1 사이 normalized 된 값으로 하고 스크린 사이즈로 스케일 해서 해당 스크린 위치 아래 월드 좌표를 구한다.

		int32 CurrViewSizeX, CurrViewSizeY;
		OwningPC->GetViewportSize(CurrViewSizeX, CurrViewSizeY);

		// Y 위치 선정에 레퍼런스 해상도 비율 대비 현재 해상도 비율을 고려할 필요..
		int32 RefViewSizeX = (int32)UB2UnitedWidgetBase::DesignedRefViewSize.X;
		int32 RefViewSizeY = (int32)UB2UnitedWidgetBase::DesignedRefViewSize.Y;
		// 기준 해상도 비율(아마도 16:9) 대비 얼마나 종횡비가 큰지..
		const float RelativeAspectRatio =
			(CurrViewSizeX > 0 && CurrViewSizeY > 0 && RefViewSizeX > 0 && RefViewSizeY > 0) ?
			(((float)CurrViewSizeX / (float)CurrViewSizeY) / ((float)RefViewSizeX / (float)RefViewSizeY)) : 1.0f;

		float FinalGradeStarCenterPosX = StarEffectInfo->ResultGradeStarCenterPos.X;
		// 화면 중앙 기준으로 변환 -> 레퍼런스 해상도와 현재 해상도 비율만큼 조정 -> 다시 화면 상단 기준으로
		float FinalGradeStarCenterPosY = ( (StarEffectInfo->ResultGradeStarCenterPos.Y - 0.5f) * RelativeAspectRatio ) + 0.5f;

		// 이 시점에서는 별 6개 만땅 찼을 때의 중심 좌표이다.
		FxCreateRefPos = GetWorldPosBeneathScreen(OwningPC,
			FVector2D((float)CurrViewSizeX * FinalGradeStarCenterPosX, (float)CurrViewSizeY * FinalGradeStarCenterPosY),
			StarEffectInfo->FxCreatingDepth, true);

		// 약간 오른쪽에 생성해서 스크린 X 축의 월드 방향을 알아내는데에 사용.
		FVector FxCreateOffsetPos = GetWorldPosBeneathScreen(OwningPC,
			FVector2D((float)CurrViewSizeX * FMath::Min(1.0f, FinalGradeStarCenterPosX + 0.1f), (float)CurrViewSizeY * FinalGradeStarCenterPosY), StarEffectInfo->FxCreatingDepth, true);

		FxCreatePosOffsetDir = (FxCreateOffsetPos - FxCreateRefPos);
		FxCreatePosOffsetDir.Normalize();
	}

	if (NativeItemData_Result.StarGrade > FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE)
	{
		// 젤 좌측의 첫 별을 생성할 위치 되겠다.
		FxCreateRefPos = FxCreateRefPos - (FxCreatePosOffsetDir * StarEffectInfo->ResultGradeStarListingOffset * 0.5f);
		FxCreateRefPos.Y += 1.0f; // 위치 보정값

		FTransform FXCreateTransform(FxCreateRefPos + FxCreatePosOffsetDir);
		UParticleSystemComponent* ThisFxComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetResultGradeStarPS(NativeItemData_Result), FXCreateTransform);
		if (ThisFxComp)
		{
			ThisFxComp->SetWorldScale3D(GetStarFxScaleForStar(NativeItemData_Result));
			CreatedResultGradeStarFx.Add(ThisFxComp);
		}
	}
	else
	{
		// 젤 좌측의 첫 별을 생성할 위치 되겠다.
		FxCreateRefPos = FxCreateRefPos - (FxCreatePosOffsetDir * StarEffectInfo->ResultGradeStarListingOffset * (float)(/*FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE*/ NativeItemData_Result.StarGrade - 1) * 0.5f);

		// 별등급 개수만큼 좌측부터 해서 생성.
		for (int32 SI = 0; SI < NativeItemData_Result.StarGrade; ++SI)
		{
			FTransform FXCreateTransform(FxCreateRefPos + (FxCreatePosOffsetDir * GetResultListingOffsetForStar(NativeItemData_Result) * (float)SI));
			UParticleSystemComponent* ThisFxComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetResultGradeStarPS(NativeItemData_Result), FXCreateTransform);
			if (ThisFxComp)
			{
				ThisFxComp->SetWorldScale3D(GetStarFxScaleForStar(NativeItemData_Result));
				CreatedResultGradeStarFx.Add(ThisFxComp);
			}
		}
	}
}

void UB2LobbyUI_ItemCusProgBase::StartShowResultItemIcon()
{
	if (!CreatedItemIcon_Result)
	{
		CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false);
		SetItemData_Result(NativeItemData_Result);
	}
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Visible);
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(true, true); // 하지만 실제로는 이름과 레벨 정도만 UI 단에서 보이게 하는 걸로. 나머지는 Fx 3D 메쉬로.
		CreatedItemIcon_Result->ForceLayoutPrepass();
	}
}

void UB2LobbyUI_ItemCusProgBase::ShowFinalResultPage()
{
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->SetVisibility(ESlateVisibility::Visible);
	}

	// 이게 최종 결과 화면에도 사용되는 경우가 있는데 그럴 땐 뒷부분 3D 이펙트 없이 보여져야 해서 Overlay 만 보이게 한 거 취소함.
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Visible);
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(true, true);
	}
}

void UB2LobbyUI_ItemCusProgBase::InitFinalResultPage(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	if (UIP_FinalResultPageP.IsValid()) // 이건 동적 생성은 아니다. 단, 몇가지는 직접 해 주어야.
	{
		UIP_FinalResultPageP->StartFromLobby(InUIManager, InGM);
		UIP_FinalResultPageP->UpdateItemData(NativeItemData_Target, NativeItemData_Result);	// 처음에는 리절트가 없음.
		UIP_FinalResultPageP->ClosePopup(); // 처음에는 숨겨놓음. 연출 최종 단계로서 보여진다.
	}
}

void UB2LobbyUI_ItemCusProgBase::ApplyCameraAdaptiveFOV()
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

FVector UB2LobbyUI_ItemCusProgBase::GetDesiredScreenCenterWorldPos()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		int32 ViewSizeX, ViewSizeY;
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY);
		return GetWorldPosBeneathScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)), GetCreatingDepthForStar(NativeItemData_Result)); // NativeItemData_Result,NativeItemData_Target
	}
	return FVector(0.0f, 0.0f, 0.0f);
}

void UB2LobbyUI_ItemCusProgBase::SetItemData_Target(FB2Item& InItem)
{
	DestroyItemIcon_Target();

	NativeItemData_Target = InItem;
	CreatedItemIcon_Target = CreateItemIconCommon(X_CP_ItemIconSet_Target.Get(), NativeItemData_Target, ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);
}

void UB2LobbyUI_ItemCusProgBase::SetItemData_Result(FB2Item& InItem)
{
	DestroyItemIcon_Result();

	NativeItemData_Result = InItem;
	CreatedItemIcon_Result = CreateItemIconCommon(X_CP_ItemIconSet_Result.Get(), NativeItemData_Result, ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);

	if (UIP_FinalResultPageP.IsValid())
		UIP_FinalResultPageP->UpdateItemData(NativeItemData_Target, NativeItemData_Result);
}

UB2DynItemIcon_ItemOpScene* UB2LobbyUI_ItemCusProgBase::CreateItemIconCommon(UCanvasPanel* ParentCP, const FB2Item& InItemData, ELobbyInvenItemIconUsage IconUsage)
{
	if (!ItemIconWidgetClass || !ParentCP || InItemData.InstanceUID <= 0 || InItemData.ItemRefID <= 0){
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
		CreatedItemIcon->UpdateItemData(InItemData);
		return CreatedItemIcon;
	}
	return NULL;
}

void UB2LobbyUI_ItemCusProgBase::DestroyItemIcon_Target()
{
	if (CreatedItemIcon_Target){
		CreatedItemIcon_Target->ConditionalBeginDestroy();
		CreatedItemIcon_Target = NULL;
	}
}

void UB2LobbyUI_ItemCusProgBase::DestroyItemIcon_Result()
{
	if (CreatedItemIcon_Result){
		CreatedItemIcon_Result->ConditionalBeginDestroy();
		CreatedItemIcon_Result = NULL;
	}
}

void UB2LobbyUI_ItemCusProgBase::DestroyAllFx()
{	
	DestroyResultGradeStarFx();
}

void UB2LobbyUI_ItemCusProgBase::DestroyResultGradeStarFx()
{
	for (UParticleSystemComponent* CurrComp : CreatedResultGradeStarFx)
	{
		if (CurrComp){
			CurrComp->ConditionalBeginDestroy();
		}
	}
	CreatedResultGradeStarFx.Empty();
}

void UB2LobbyUI_ItemCusProgBase::FinishAndProceedToResult()
{
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, 1.0f); // OverallPlayRate 에 따라 변경했던 거 복구.
	QuitItemOpModeClass<bool>::GetInstance().Signal(true); // 실제로는 결과창이 나와야 함. 거기서 Quit 을 하게 될 것.
}

#if WITH_EDITOR
void UB2LobbyUI_ItemCusProgBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_ResultGradeStarCenterPos = GET_MEMBER_NAME_CHECKED(UB2LobbyUI_ItemCusProgBase, GetResultStarCenterPosForStar(NativeItemData_Result));
	
	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	//if (PropertyName == Name_ResultGradeStarCenterPos) // 이 이름으로 안 들어오고 X, Y 로 들어옴 ㅋㅋ 걍 언제나 체크
	{ // StartResultGradeStarFx 에서 ResultGradeStarCenterPos 약간 오른쪽 위치를 사용하는 게 있어서 X 좌표는 1.0 이 아닌 0.9까지만 허용.

		auto* StarEffectInfo = GetStarEffectInfo(NativeItemData_Result);

		StarEffectInfo->ResultGradeStarCenterPos.X = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.X, 0.0f, 0.9f);
		StarEffectInfo->ResultGradeStarCenterPos.Y = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.Y, 0.0f, 1.0f);
	}
}
#endif
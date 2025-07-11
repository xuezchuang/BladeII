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
		// 7���̸� 1 8���̸�2 9���̸� 3...
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

	// ������Ʈ �Ǳ� �� Ÿ�� ������ �����Ͱ� �ʿ� (UB2LobbyInventory::OnReqForItemOpCommon ���� ���) 
	if (!CachedInventory->GetItemOPTargetItemData(NativeItemData_Target, true) || // Result �����ʹ� ���⼭ �������� �ʴ´�. �Ƹ� response �� �� ��Ȳ�� ������ Ȥ �𸣴�..
		!IsSupportedOpMode(TargetOpMode))
	{
		FinishAndProceedToResult(); // ���� �ϳ� �����Ѵٸ�.. ��¿ �� ����.
		return;
	}

	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, GetOverallPlayRateForStar(NativeItemData_Target));

	ApplyCameraAdaptiveFOV();

	SetItemData_Target(NativeItemData_Target);

	// ������Ʈ �� ��� ������ �����͸� �������� ����. ���� ������ ������ ���� �ʴ´�.
	// ���� response �� ���� ���� Ÿ�̹��̶�� �ʿ��� �� �ٽ� ������ ��.
	if (!CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false)){
		// � �����ε� ����� �������� ���Ѵٸ� request ������ ĳ�̵� Ÿ�� �����͸� �ִ´�.
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
	DestroyResultGradeStarFx(); // ���⼭ DestroyAllFx �ϸ� �ȵ�����.

	if (!GetResultGradeStarPS(NativeItemData_Result)){
		return;
	}
	
	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // ���� �ʿ��ұ� �ͱ� ������.
	
	auto* StarEffectInfo = GetStarEffectInfo(NativeItemData_Result);
	auto* OwningPC = GetOwningPlayer();

	FVector FxCreateRefPos(0.0f,0.0f,0.0f); // �� ó�� ���� �����ϴ� �� ���� ��ǥ.
	FVector FxCreatePosOffsetDir(1.0f, 0.0f, 0.0f); // ���� ���������� ȭ�� ���� ����. ������ ������ �� �� �������� �ɼ��� �� ��.

	if (OwningPC && StarEffectInfo)
	{
		StarEffectInfo->ResultGradeStarCenterPos.X = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.X, 0.0f, 0.9f); // �̰ź��� �ణ �����ʿ����� ��ġ�� ���� �Ŷ� X ��ǥ ������ �� ����. ��ü�� ȭ�� �߾��̰�����.

		// ResultGradeStarCenterPos �� 0 ~ 1 ���� normalized �� ������ �ϰ� ��ũ�� ������� ������ �ؼ� �ش� ��ũ�� ��ġ �Ʒ� ���� ��ǥ�� ���Ѵ�.

		int32 CurrViewSizeX, CurrViewSizeY;
		OwningPC->GetViewportSize(CurrViewSizeX, CurrViewSizeY);

		// Y ��ġ ������ ���۷��� �ػ� ���� ��� ���� �ػ� ������ ����� �ʿ�..
		int32 RefViewSizeX = (int32)UB2UnitedWidgetBase::DesignedRefViewSize.X;
		int32 RefViewSizeY = (int32)UB2UnitedWidgetBase::DesignedRefViewSize.Y;
		// ���� �ػ� ����(�Ƹ��� 16:9) ��� �󸶳� ��Ⱦ�� ū��..
		const float RelativeAspectRatio =
			(CurrViewSizeX > 0 && CurrViewSizeY > 0 && RefViewSizeX > 0 && RefViewSizeY > 0) ?
			(((float)CurrViewSizeX / (float)CurrViewSizeY) / ((float)RefViewSizeX / (float)RefViewSizeY)) : 1.0f;

		float FinalGradeStarCenterPosX = StarEffectInfo->ResultGradeStarCenterPos.X;
		// ȭ�� �߾� �������� ��ȯ -> ���۷��� �ػ󵵿� ���� �ػ� ������ŭ ���� -> �ٽ� ȭ�� ��� ��������
		float FinalGradeStarCenterPosY = ( (StarEffectInfo->ResultGradeStarCenterPos.Y - 0.5f) * RelativeAspectRatio ) + 0.5f;

		// �� ���������� �� 6�� ���� á�� ���� �߽� ��ǥ�̴�.
		FxCreateRefPos = GetWorldPosBeneathScreen(OwningPC,
			FVector2D((float)CurrViewSizeX * FinalGradeStarCenterPosX, (float)CurrViewSizeY * FinalGradeStarCenterPosY),
			StarEffectInfo->FxCreatingDepth, true);

		// �ణ �����ʿ� �����ؼ� ��ũ�� X ���� ���� ������ �˾Ƴ��µ��� ���.
		FVector FxCreateOffsetPos = GetWorldPosBeneathScreen(OwningPC,
			FVector2D((float)CurrViewSizeX * FMath::Min(1.0f, FinalGradeStarCenterPosX + 0.1f), (float)CurrViewSizeY * FinalGradeStarCenterPosY), StarEffectInfo->FxCreatingDepth, true);

		FxCreatePosOffsetDir = (FxCreateOffsetPos - FxCreateRefPos);
		FxCreatePosOffsetDir.Normalize();
	}

	if (NativeItemData_Result.StarGrade > FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE)
	{
		// �� ������ ù ���� ������ ��ġ �ǰڴ�.
		FxCreateRefPos = FxCreateRefPos - (FxCreatePosOffsetDir * StarEffectInfo->ResultGradeStarListingOffset * 0.5f);
		FxCreateRefPos.Y += 1.0f; // ��ġ ������

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
		// �� ������ ù ���� ������ ��ġ �ǰڴ�.
		FxCreateRefPos = FxCreateRefPos - (FxCreatePosOffsetDir * StarEffectInfo->ResultGradeStarListingOffset * (float)(/*FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE*/ NativeItemData_Result.StarGrade - 1) * 0.5f);

		// ����� ������ŭ �������� �ؼ� ����.
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
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(true, true); // ������ �����δ� �̸��� ���� ������ UI �ܿ��� ���̰� �ϴ� �ɷ�. �������� Fx 3D �޽���.
		CreatedItemIcon_Result->ForceLayoutPrepass();
	}
}

void UB2LobbyUI_ItemCusProgBase::ShowFinalResultPage()
{
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->SetVisibility(ESlateVisibility::Visible);
	}

	// �̰� ���� ��� ȭ�鿡�� ���Ǵ� ��찡 �ִµ� �׷� �� �޺κ� 3D ����Ʈ ���� �������� �ؼ� Overlay �� ���̰� �� �� �����.
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Visible);
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(true, true);
	}
}

void UB2LobbyUI_ItemCusProgBase::InitFinalResultPage(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	if (UIP_FinalResultPageP.IsValid()) // �̰� ���� ������ �ƴϴ�. ��, ����� ���� �� �־��.
	{
		UIP_FinalResultPageP->StartFromLobby(InUIManager, InGM);
		UIP_FinalResultPageP->UpdateItemData(NativeItemData_Target, NativeItemData_Result);	// ó������ ����Ʈ�� ����.
		UIP_FinalResultPageP->ClosePopup(); // ó������ ���ܳ���. ���� ���� �ܰ�μ� ��������.
	}
}

void UB2LobbyUI_ItemCusProgBase::ApplyCameraAdaptiveFOV()
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
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, 1.0f); // OverallPlayRate �� ���� �����ߴ� �� ����.
	QuitItemOpModeClass<bool>::GetInstance().Signal(true); // �����δ� ���â�� ���;� ��. �ű⼭ Quit �� �ϰ� �� ��.
}

#if WITH_EDITOR
void UB2LobbyUI_ItemCusProgBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_ResultGradeStarCenterPos = GET_MEMBER_NAME_CHECKED(UB2LobbyUI_ItemCusProgBase, GetResultStarCenterPosForStar(NativeItemData_Result));
	
	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	//if (PropertyName == Name_ResultGradeStarCenterPos) // �� �̸����� �� ������ X, Y �� ���� ���� �� ������ üũ
	{ // StartResultGradeStarFx ���� ResultGradeStarCenterPos �ణ ������ ��ġ�� ����ϴ� �� �־ X ��ǥ�� 1.0 �� �ƴ� 0.9������ ���.

		auto* StarEffectInfo = GetStarEffectInfo(NativeItemData_Result);

		StarEffectInfo->ResultGradeStarCenterPos.X = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.X, 0.0f, 0.9f);
		StarEffectInfo->ResultGradeStarCenterPos.Y = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.Y, 0.0f, 1.0f);
	}
}
#endif
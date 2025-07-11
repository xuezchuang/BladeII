// Fill out your copyright notice in the Description page of Project Settings.


#include "B2SealCardFx.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UISlotItem.h"
#include "B2GameDefine.h"



void FCardFXSetupLoadedRefHolder::CacheMeIfYouCan(class UParticleSystem* InLoadedPS)
{
	if (InLoadedPS)
	{
		LoadedPSPtr.AddUnique(InLoadedPS);
	}
}

void FCardFXSetup::PreloadForItems(TArray<FB2Item>& InItems, FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder)
{
	// ���� �ε� ���ŷ�� �� �־ �̸� �ε��� �ʿ䰡 �ֱ�..
	for (FB2Item& ThisItem : InItems)
	{
		GetPSForItem(ThisItem, InTAssetLoadManager, InLoadedRefHolder);
	}
}

UParticleSystem* FCardFXSetup::GetPSForItem(const FB2Item& InItem, FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder) const
{
	UParticleSystem* LoadedPS = NULL;
	/*
	if (IsEquipmentItem(InItem) || IsEssenceItem(InItem))
	{
		const int32 PSIndex = InItem.StarGrade - 1;
		if (PSPerItemGrade.IsValidIndex(PSIndex))
		{
			const TSoftObjectPtr<UParticleSystem>& TAssetToLoad = PSPerItemGrade[PSIndex];
			if (!TAssetToLoad.IsValid())
			{
				InTAssetLoadManager.LoadSynchronous(TAssetToLoad.GetSoftObjectPath());
			}
			if (TAssetToLoad.IsValid())
			{
				LoadedPS = TAssetToLoad.Get();
			}
		}
	}
	else if (IsEtherItem(InItem))
	{
		if (InItem.StarGrade >= ETHER_MAX_STAR)
		{
			const int32 PSIndex = InItem.Level - 1;
			if (PSPerEtherTierExtend.IsValidIndex(PSIndex))
			{
				const TSoftObjectPtr<UParticleSystem>& TAssetToLoad = PSPerEtherTierExtend[PSIndex];
				if (!TAssetToLoad.IsValid())
				{
					InTAssetLoadManager.LoadSynchronous(TAssetToLoad.GetSoftObjectPath());
				}
				if (TAssetToLoad.IsValid())
				{
					LoadedPS = TAssetToLoad.Get();
				}
			}
		}
		else
		{
			const int32 PSIndex = InItem.Level - 1;
			if (PSPerEtherTier.IsValidIndex(PSIndex))
			{
				const TSoftObjectPtr<UParticleSystem>& TAssetToLoad = PSPerEtherTier[PSIndex];
				if (!TAssetToLoad.IsValid())
				{
					InTAssetLoadManager.LoadSynchronous(TAssetToLoad.GetSoftObjectPath());
				}
				if (TAssetToLoad.IsValid())
				{
					LoadedPS = TAssetToLoad.Get();
				}
			}
		}
	}
	else if (IsGoldItem(InItem))
	{
		if (!PSForGold.IsValid())
		{
			InTAssetLoadManager.LoadSynchronous(PSForGold.GetSoftObjectPath());
		}
		if (PSForGold.IsValid())
		{
			LoadedPS = PSForGold.Get();
		}
	}
	else if (IsMaterialItem(InItem))
	{
		if (InItem.bMaterialFromAtlas)
		{
			if (!PSForAtlasMaterial.IsValid())
			{
				InTAssetLoadManager.LoadSynchronous(PSForAtlasMaterial.GetSoftObjectPath());
			}
			if (PSForAtlasMaterial.IsValid())
			{
				LoadedPS = PSForAtlasMaterial.Get();
			}
		}
		else
		{
			if (!PSForNonAtlasMaterial.IsValid())
			{
				InTAssetLoadManager.LoadSynchronous(PSForNonAtlasMaterial.GetSoftObjectPath());
			}
			if (PSForNonAtlasMaterial.IsValid())
			{
				LoadedPS = PSForNonAtlasMaterial.Get();
			}
		}
	}
	*/

	const int32 PSIndex = InItem.StarGrade - 1;
	if (ChainedFX.IsValidIndex(0))	//����Ʈ ���
	{
		const TSoftObjectPtr<UParticleSystem>& TAssetToLoad = ChainedFX[0];	//����Ʈ ���
		if (!TAssetToLoad.IsValid())
		{
			InTAssetLoadManager.LoadSynchronous(TAssetToLoad.ToSoftObjectPath());
		}
		if (TAssetToLoad.IsValid())
		{
			LoadedPS = TAssetToLoad.Get();
		}
	}

	if (LoadedPS)
	{
		InLoadedRefHolder.CacheMeIfYouCan(LoadedPS);
		return LoadedPS;
	}

	return NULL;
}

void FCardFXSetup::UnloadTAssets(FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder)
{
#if WITH_EDITOR
	if (GIsEditor) {
		return; // �����Ϳ��� ��ε� ���� �� ����.
	}
#endif

	for (TSoftObjectPtr<UParticleSystem>& ThisOne : ChainedFX)
	{
		if (ThisOne.IsValid())
		{
			InTAssetLoadManager.Unload(ThisOne.ToSoftObjectPath());
		}
	}
	for (TSoftObjectPtr<UParticleSystem>& ThisOne : UnchainFX)
	{
		if (ThisOne.IsValid())
		{
			InTAssetLoadManager.Unload(ThisOne.ToSoftObjectPath());
		}
	}
	for (TSoftObjectPtr<UParticleSystem>& ThisOne : FlipCardFX)
	{
		if (ThisOne.IsValid())
		{
			InTAssetLoadManager.Unload(ThisOne.ToSoftObjectPath());
		}
	}

	InLoadedRefHolder.LoadedPSPtr.Empty(); // ��� �� �Ѳ�����
}

#if WITH_EDITOR
void FCardFXSetup::EditorLoadAll()
{
	if (!GIsEditor) {
		return;
	}
	// ���� ĳ�̰��� �� ����.
	for (TSoftObjectPtr<UParticleSystem>& ThisOne : ChainedFX)
	{
		ThisOne.LoadSynchronous();
	}
	for (TSoftObjectPtr<UParticleSystem>& ThisOne : UnchainFX)
	{
		ThisOne.LoadSynchronous();
	}
	for (TSoftObjectPtr<UParticleSystem>& ThisOne : FlipCardFX)
	{
		ThisOne.LoadSynchronous();
	}
}
#endif

const FName UB2SealCardFx::Name_FxMID_ItemBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2SealCardFx::Name_FxMID_EtherPanel(TEXT("MAT_EtherPanel"));
const FName UB2SealCardFx::Name_FxMID_ItemIcon(TEXT("MAT_ItemIcon"));
const FName UB2SealCardFx::Name_FxMID_StaIcon(TEXT("MAT_StarIcon"));
const FName UB2SealCardFx::Name_FxMID_Param_StarTile(TEXT("Star_Tile"));
const FName UB2SealCardFx::Name_FxMID_Param_StarSize(TEXT("Star_Size"));
const FName UB2SealCardFx::Name_FxMID_StarNumer(TEXT("MAT_Number"));

UB2SealCardFx::UB2SealCardFx(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemNumber = 0;
	FxDestMarkerPrefix = TEXT("RewardDest");
	FxCreateScale = 1.0f;
	FxMoveBaseAcceleration = 10.0f;
	FxMoveMinSpeed = 10.0f;
	FxMoveMaxSpeed = 100.0f;

	FxMoveAccAdjustConst = 1.0f;
	FxMoveFinalAcc = 10.0f;

	FxStarSize = 36;
}

void UB2SealCardFx::Init()
{
	Super::Init();

	FxMoveFinalAcc = FxMoveBaseAcceleration;
}

void UB2SealCardFx::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateFxMove(InDeltaTime);
}

void UB2SealCardFx::DestroySelf(class UB2UIManager* InUIManager)
{
	CleanupScene(); // �� ��Ʈ�� owner �ʿ��� DestroySelf �� ���� call �ϵ���, �ƴϸ� CleanupScene �� ���� call �ϵ���.
	Super::DestroySelf(InUIManager);
}

void UB2SealCardFx::CacheAssets()
{
	for (int32 MI = 1; MI <= ItemNumber; ++MI)
	{
		FString ThisMarkerName = FString::Printf(TEXT("%s_%d"), *FxDestMarkerPrefix, MI);
		UB2UISlotItem* ThisMarker = GetSlot<UB2UISlotItem>(FName(*ThisMarkerName));
		if (ThisMarker)
		{
			ThisMarker->Init();
			ThisMarker->SetVisibility(ESlateVisibility::Hidden); // �ϴ� �����.. ���߿� Overlay Info �� ���̵��� �� ��.
			AllFxDestMarkers.Add(ThisMarker);
		}
	}
	checkSlow(ItemNumber == AllFxDestMarkers.Num());
}

void UB2SealCardFx::BeginScene(const TArray<FB2Item>& InRewardItems, const FCardFXSetup& InPSSetup, FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder)
{
	checkSlow(InRewardItems.Num() == ItemNumber);

	for (int32 RI = 0; RI < InRewardItems.Num(); ++RI)
	{
		APlayerController* OwningPC = GetOwningPlayer();
		UB2UISlotItem* ThisDestMarker = AllFxDestMarkers.IsValidIndex(RI) ? AllFxDestMarkers[RI] : NULL;
		FVector2D MarkerScreenPos;
		// Marker �� ��ġ�� ���� HACKGetWidgetScreenCoord �� ��ũ����ǥ�� ���� ���� �� �ִ�.
		if (ThisDestMarker && OwningPC && HACKGetWidgetScreenCoord(ThisDestMarker, OwningPC, MarkerScreenPos, true))
		{
			if (InRewardItems[RI].ItemClass == EItemClass::EIC_Ether)
			{
				ThisDestMarker->BindEther(InRewardItems[RI]);
			}
			else
			{
				ThisDestMarker->BindDoc(InRewardItems[RI]); // 2D Slot Item Widget �� ������ ����.
			}

			UParticleSystemComponent* NewFx = CreateSingleRewardFx(InRewardItems[RI], ThisDestMarker, InPSSetup, InTAssetLoadManager, InLoadedRefHolder);
			if (NewFx)
			{
				FCardFXSet NewSet;

				NewSet.FXComp = NewFx;
				NewSet.DestMarkerWidget = ThisDestMarker;
				NewSet.DestPos = GetWorldPosBeneathScreen(OwningPC, MarkerScreenPos, InPSSetup.FxCreateDepth, false);
				NewSet.TotalDistance = (NewSet.DestPos - NewSet.FXComp->GetComponentLocation()).Size();
				NewSet.CurrentSpeed = FxMoveMinSpeed;
				NewSet.FlightDistSoFar = 0.0f;
				NewSet.FlightTimeSoFar = 0.0f;
				NewSet.MinOverlayHiddenTime = InPSSetup.MinOverlayHiddenTime;

				CreatedRewardFxSet.Add(NewSet);
			}
		}
	}

	OnBeginScene_BP();
}

void UB2SealCardFx::CleanupScene()
{
	for (UB2UISlotItem* ThisMarker : AllFxDestMarkers)
	{
		if (ThisMarker)
		{
			ThisMarker->UnbindDoc();
		}
	}
	// AllFxDestMarkers �� ���� �� ���ƿ� ���� ���� ��. �� ����� �������� ��

	UWorld* TheWorld = GetWorld();
	for (FCardFXSet& ThisSet : CreatedRewardFxSet)
	{
		if (ThisSet.FXComp)
		{
			ThisSet.FXComp->ConditionalBeginDestroy();
			ThisSet.FXComp = NULL;
		}

		if (TheWorld)
		{
			TheWorld->GetTimerManager().ClearTimer(ThisSet.ItemOverlayInfoShowTH);
		}
	}
	CreatedRewardFxSet.Empty();

	// FStageClearRewardPSSetup Unload �� ���� owner �ʼ�.
}

UParticleSystemComponent* UB2SealCardFx::CreateSingleRewardFx(const FB2Item& InRewardItem, UB2UISlotItem* InMarkerWidget, const FCardFXSetup& InPSSetup, FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder)
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC && InMarkerWidget)
	{
		int32 ViewSizeX, ViewSizeY;
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY);
		FVector2D FinalCreateScreenPos = InPSSetup.FxCreateNormalizedScreenPos * FVector2D((float)ViewSizeX, (float)ViewSizeY);
		FVector CreateWorldPos = GetWorldPosBeneathScreen(OwningPC, FinalCreateScreenPos, InPSSetup.FxCreateDepth, false);

		UParticleSystemComponent* CreatedFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InPSSetup.GetPSForItem(InRewardItem, InTAssetLoadManager, InLoadedRefHolder), FTransform(CreateWorldPos));
		if (CreatedFX)
		{
			// Scale �� �������� �ٸ� �� �־ ������ ���⿡ �ְ� Rotation �� PS ���º��� �� ���� ����.
			CreatedFX->SetWorldScale3D(FVector(FxCreateScale, FxCreateScale, FxCreateScale));
			CreatedFX->SetWorldRotation(InPSSetup.FxCreateRot);
			// Ȥ�ö� �ʿ��ϸ� ItemInfo �����µ� �ϴ��� InMarkerWidget �� Ȱ��
			//UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
			//FSingleItemInfoData* ThisItemInfoData = ItemInfoTable ? ItemInfoTable->GetInfoData(InRewardItem.ItemRefID) : NULL;

			// ������ material �� �¾��Ѵ�
			SetupNamedMIDForFxComp(CreatedFX);
			{
				// ParticleSystem �¾��� Grade ���� ���� ���ٰ� �ϴ� �� Background panel �¾��� �ʿ����� ���� ����.
				UMaterialInstanceDynamic* ItemBGPanelMID = GetFxNamedMID(CreatedFX, Name_FxMID_ItemBGPanel);
				UMaterialInstanceConstant* ItemBGPanelMIC_Ref = Cast<UMaterialInstanceConstant>(InMarkerWidget->GetActiveItemBGPanelMaterial());
				SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);
			} {
				UMaterialInstanceDynamic* EtherBGPanelMID = GetFxNamedMID(CreatedFX, Name_FxMID_EtherPanel);
				UMaterialInstanceConstant* EtherBGPanelMIC_Ref = Cast<UMaterialInstanceConstant>(InMarkerWidget->GetActiveEtherItemBGPanelMaterial());
				SetupMIDNonAtlasParamFromUIMIC(EtherBGPanelMID, EtherBGPanelMIC_Ref);
			} {
				UMaterialInstanceDynamic* ItemIconMID = GetFxNamedMID(CreatedFX, Name_FxMID_ItemIcon);
				UMaterialInstanceConstant* ItemIconMIC_Ref = Cast<UMaterialInstanceConstant>(InMarkerWidget->GetIconMaterial());
				SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
			} {
				// Atlas Material For Material Item
				UMaterialInstanceDynamic* Atlas_MaterialIconMID = GetFxNamedMID(CreatedFX, Name_FxMID_ItemIcon);
				UMaterialInstanceConstant* Atlas_MaterialIconMIC_Ref = Cast<UMaterialInstanceConstant>(InMarkerWidget->GetIconMaterial());
				SetupMIDAtlasParamFromUIMIC(Atlas_MaterialIconMID, Atlas_MaterialIconMIC_Ref);
			} {
				// NonAtlas Material For Material Item
				UMaterialInstanceDynamic* NonAtlas_MaterialIconMID = GetFxNamedMID(CreatedFX, Name_FxMID_ItemIcon);
				UMaterialInstanceConstant* NonAtlas_MaterialIconMIC_Ref = Cast<UMaterialInstanceConstant>(InMarkerWidget->GetIconMaterial());
				SetupMIDNonAtlasParamFromUIMIC(NonAtlas_MaterialIconMID, NonAtlas_MaterialIconMIC_Ref);
			} {
				// Ether Star Icon
				UMaterialInstanceDynamic* ItemStarIconMID = GetFxNamedMID(CreatedFX, Name_FxMID_StaIcon);
				SetMIDScalarParam(ItemStarIconMID, Name_FxMID_Param_StarTile, (float)InRewardItem.StarGrade);
				SetMIDScalarParam(ItemStarIconMID, Name_FxMID_Param_StarSize, (float)((InRewardItem.StarGrade - 1) * FxStarSize));
			} {
				// Ether Star Number ������ 7�� �̻��϶�
				UMaterialInstanceDynamic* ItemStarIconMID = GetFxNamedMID(CreatedFX, Name_FxMID_StarNumer);
				SetStageResultStar(ItemStarIconMID, InRewardItem.StarGrade);
			}

			return CreatedFX;
		}
	}
	return NULL;
}

void UB2SealCardFx::UpdateFxMove(float DeltaSecond)
{
	for (int32 RI = 0; RI < CreatedRewardFxSet.Num(); ++RI)
	{
		FCardFXSet& ThisSet = CreatedRewardFxSet[RI];
		if (ThisSet.FXComp && !ThisSet.bArrivedAtDest)
		{
			FVector FxCompPos = ThisSet.FXComp->GetComponentLocation();
			FVector ToTargetDir = ThisSet.DestPos - FxCompPos;
			ToTargetDir.Normalize();

			const float RemainingDist = ThisSet.TotalDistance - ThisSet.FlightDistSoFar;
			// ��ǥ�������� �Ÿ��� �ٸ� �����۵� ���� �������� �� ���߱� ���� ������ spring �𵨿� ���� scale �� �����Ͽ����� FxMoveBaseAcceleration ���� ������ �ʰ�. �ֳĸ� �츮�� �ڷ��� �߱��ϹǷ�
			FxMoveFinalAcc = FxMoveBaseAcceleration + (RemainingDist * FxMoveAccAdjustConst);

			ThisSet.CurrentSpeed = FMath::Min(ThisSet.CurrentSpeed + FxMoveFinalAcc * DeltaSecond, FxMoveMaxSpeed);
			FVector ThisMoveAmount = ToTargetDir * (ThisSet.CurrentSpeed * DeltaSecond);

			ThisSet.FXComp->SetWorldLocation(FxCompPos + ThisMoveAmount);

			ThisSet.FlightTimeSoFar += DeltaSecond;
			// ó�� ������ �Ÿ����� �ָ� ���ٸ� ������ �ɷ� ġ�� Ÿ�� ��ġ�� ����.
			ThisSet.FlightDistSoFar += ThisMoveAmount.Size();
			if (ThisSet.FlightDistSoFar >= ThisSet.TotalDistance)
			{
				ThisSet.bArrivedAtDest = true;
				ThisSet.FXComp->SetWorldLocation(ThisSet.DestPos);
				OnSingleRewardArriveAtDest(RI);
			}
		}
	}
}

void UB2SealCardFx::OnSingleRewardArriveAtDest(int32 RewardIndex)
{
	UWorld* TheWorld = GetWorld();
	if (CreatedRewardFxSet.IsValidIndex(RewardIndex))
	{
		FCardFXSet& ThisSet = CreatedRewardFxSet[RewardIndex];

		const float HowEarly = ThisSet.MinOverlayHiddenTime - ThisSet.FlightTimeSoFar;
		if (HowEarly > 0.0f && TheWorld)
		{ // ������ overlay �� ������ �� �ð����� ���� ���������Ƿ� Ÿ�̸� �¾�.
			TheWorld->GetTimerManager().SetTimer(ThisSet.ItemOverlayInfoShowTH, FTimerDelegate::CreateUObject(this, &UB2SealCardFx::ShowRewardOverlayInfo, RewardIndex), HowEarly, false);
		}
		else
		{ // �ٷ� ������ overlay ������ ������ ��.
			ShowRewardOverlayInfo(RewardIndex);
		}
	}

	OnSingleRewardArriveAtDest_BP(RewardIndex);
}

void UB2SealCardFx::ShowRewardOverlayInfo(int32 RewardIndex)
{
	if (CreatedRewardFxSet.IsValidIndex(RewardIndex))
	{
		FCardFXSet& ThisSet = CreatedRewardFxSet[RewardIndex];
		if (ThisSet.DestMarkerWidget)
		{
			ThisSet.DestMarkerWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			ThisSet.DestMarkerWidget->SetShowOverlayInfoOnly(true);
		}

		UWorld* TheWorld = GetWorld();
		if (TheWorld)
			TheWorld->GetTimerManager().ClearTimer(ThisSet.ItemOverlayInfoShowTH);
	}
}

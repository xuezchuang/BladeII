#include "BladeII.h"
#include "B2UIStageClearRewardFx.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UISlotItem.h"
#include "B2GameDefine.h"



void FStageClearRewardPSSetupLoadedRefHolder::CacheMeIfYouCan(class UParticleSystem* InLoadedPS)
{
	if (InLoadedPS)
	{
		LoadedPSPtr.AddUnique(InLoadedPS);
	}
}

void FStageClearRewardPSSetup::PreloadForItems(TArray<FB2Item>& InItems, FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder)
{
	// 은근 로딩 블로킹이 좀 있어서 미리 로딩할 필요가 있군..
	for (FB2Item& ThisItem : InItems)
	{
		GetPSForItem(ThisItem, InTAssetLoadManager, InLoadedRefHolder);
	}
}

UParticleSystem* FStageClearRewardPSSetup::GetPSForItem(const FB2Item& InItem, FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder) const
{
	UParticleSystem* LoadedPS = NULL;
	if (IsEquipmentItem(InItem) || IsEssenceItem(InItem))
	{
		const int32 PSIndex = InItem.StarGrade - 1;
		if (PSPerItemGrade.IsValidIndex(PSIndex))
		{
			const TSoftObjectPtr<UParticleSystem>& TAssetToLoad = PSPerItemGrade[PSIndex];
			if (!TAssetToLoad.IsValid())
			{
				InTAssetLoadManager.LoadSynchronous(TAssetToLoad.ToSoftObjectPath());
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
					InTAssetLoadManager.LoadSynchronous(TAssetToLoad.ToSoftObjectPath());
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
					InTAssetLoadManager.LoadSynchronous(TAssetToLoad.ToSoftObjectPath());
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
			InTAssetLoadManager.LoadSynchronous(PSForGold.ToSoftObjectPath());
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
				InTAssetLoadManager.LoadSynchronous(PSForAtlasMaterial.ToSoftObjectPath());
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
				InTAssetLoadManager.LoadSynchronous(PSForNonAtlasMaterial.ToSoftObjectPath());
			}
			if (PSForNonAtlasMaterial.IsValid())
			{
				LoadedPS = PSForNonAtlasMaterial.Get();
			}
		}
	}

	if (LoadedPS)
	{
		InLoadedRefHolder.CacheMeIfYouCan(LoadedPS);
		return LoadedPS;
	}

	return NULL;
}

void FStageClearRewardPSSetup::UnloadTAssets(FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder)
{
#if WITH_EDITOR
	if (GIsEditor) {
		return; // 에디터에선 언로딩 같은 거 안함.
	}
#endif

	for (TSoftObjectPtr<UParticleSystem>& ThisOne : PSPerItemGrade)
	{
		if (ThisOne.IsValid())
		{
			InTAssetLoadManager.Unload(ThisOne.ToSoftObjectPath());
		}
	}
	for (TSoftObjectPtr<UParticleSystem>& ThisOne : PSPerEtherTier)
	{
		if (ThisOne.IsValid())
		{
			InTAssetLoadManager.Unload(ThisOne.ToSoftObjectPath());
		}
	}
	if (PSForGold.IsValid())
	{
		InTAssetLoadManager.Unload(PSForGold.ToSoftObjectPath());
	}
	if (PSForAtlasMaterial.IsValid())
	{
		InTAssetLoadManager.Unload(PSForAtlasMaterial.ToSoftObjectPath());
	}
	if (PSForNonAtlasMaterial.IsValid())
	{
		InTAssetLoadManager.Unload(PSForNonAtlasMaterial.ToSoftObjectPath());
	}

	InLoadedRefHolder.LoadedPSPtr.Empty(); // 비울 땐 한꺼번에
}

#if WITH_EDITOR
void FStageClearRewardPSSetup::EditorLoadAll()
{
	if (!GIsEditor) {
		return;
	}
	// 굳이 캐싱같은 거 안함.
	for (TSoftObjectPtr<UParticleSystem>& ThisOne : PSPerItemGrade)
	{
		ThisOne.LoadSynchronous();
	}
	for (TSoftObjectPtr<UParticleSystem>& ThisOne : PSPerEtherTier)
	{
		ThisOne.LoadSynchronous();
	}
	PSForGold.LoadSynchronous();
	PSForAtlasMaterial.LoadSynchronous();
	PSForNonAtlasMaterial.LoadSynchronous();
}
#endif

const FName UB2UIStageClearRewardFx::Name_FxMID_ItemBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2UIStageClearRewardFx::Name_FxMID_EtherPanel(TEXT("MAT_EtherPanel"));
const FName UB2UIStageClearRewardFx::Name_FxMID_ItemIcon(TEXT("MAT_ItemIcon"));
const FName UB2UIStageClearRewardFx::Name_FxMID_StaIcon(TEXT("MAT_StarIcon"));
const FName UB2UIStageClearRewardFx::Name_FxMID_Param_StarTile(TEXT("Star_Tile"));
const FName UB2UIStageClearRewardFx::Name_FxMID_Param_StarSize(TEXT("Star_Size"));
const FName UB2UIStageClearRewardFx::Name_FxMID_StarNumer(TEXT("MAT_Number"));

UB2UIStageClearRewardFx::UB2UIStageClearRewardFx(const FObjectInitializer& ObjectInitializer)
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

void UB2UIStageClearRewardFx::Init()
{
	Super::Init();

	FxMoveFinalAcc = FxMoveBaseAcceleration;
}

void UB2UIStageClearRewardFx::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateFxMove(InDeltaTime);
}

void UB2UIStageClearRewardFx::DestroySelf(class UB2UIManager* InUIManager)
{
	CleanupScene(); // 이 파트의 owner 쪽에서 DestroySelf 를 직접 call 하든지, 아니면 CleanupScene 을 직접 call 하든지.
	Super::DestroySelf(InUIManager);
}

void UB2UIStageClearRewardFx::CacheAssets()
{
	for (int32 MI = 1; MI <= ItemNumber; ++MI)
	{
		FString ThisMarkerName = FString::Printf(TEXT("%s_%d"), *FxDestMarkerPrefix, MI);
		UB2UISlotItem* ThisMarker = GetSlot<UB2UISlotItem>(FName(*ThisMarkerName));
		if (ThisMarker)
		{
			ThisMarker->Init();
			ThisMarker->SetVisibility(ESlateVisibility::Hidden); // 일단 숨기고.. 나중에 Overlay Info 만 보이도록 할 거.
			AllFxDestMarkers.Add(ThisMarker);
		}
	}
	checkSlow(ItemNumber == AllFxDestMarkers.Num());
}

void UB2UIStageClearRewardFx::BeginScene(const TArray<FB2Item>& InRewardItems, const FStageClearRewardPSSetup& InPSSetup, FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder)
{
	checkSlow(InRewardItems.Num() == ItemNumber);

	for (int32 RI = 0; RI < InRewardItems.Num(); ++RI)
	{
		APlayerController* OwningPC = GetOwningPlayer();
		UB2UISlotItem* ThisDestMarker = AllFxDestMarkers.IsValidIndex(RI) ? AllFxDestMarkers[RI] : NULL;
		FVector2D MarkerScreenPos;
		// Marker 의 배치에 따라 HACKGetWidgetScreenCoord 로 스크린좌표를 얻지 못할 수 있다.
		if (ThisDestMarker && OwningPC && HACKGetWidgetScreenCoord(ThisDestMarker, OwningPC, MarkerScreenPos, true))
		{
			if (InRewardItems[RI].ItemClass == EItemClass::EIC_Ether)
			{
				ThisDestMarker->BindEther(InRewardItems[RI]);
			}
			else
			{
				ThisDestMarker->BindDoc(InRewardItems[RI]); // 2D Slot Item Widget 에 아이템 정보.
			}

			UParticleSystemComponent* NewFx = CreateSingleRewardFx(InRewardItems[RI], ThisDestMarker, InPSSetup, InTAssetLoadManager, InLoadedRefHolder);
			if (NewFx)
			{
				FStageClearRewardFxSet NewSet;
				
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

void UB2UIStageClearRewardFx::CleanupScene()
{
	for (UB2UISlotItem* ThisMarker : AllFxDestMarkers)
	{
		if (ThisMarker)
		{
			ThisMarker->UnbindDoc();
		}
	}
	// AllFxDestMarkers 를 비우는 건 문맥에 맞지 않을 듯. 별 상관도 없겠지만 ㅋ

	UWorld* TheWorld = GetWorld();
	for (FStageClearRewardFxSet& ThisSet : CreatedRewardFxSet)
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

	// FStageClearRewardPSSetup Unload 는 따로 owner 쪽서.
}

UParticleSystemComponent* UB2UIStageClearRewardFx::CreateSingleRewardFx(const FB2Item& InRewardItem, UB2UISlotItem* InMarkerWidget, const FStageClearRewardPSSetup& InPSSetup, FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder)
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
			// Scale 은 개수별로 다를 수 있어서 세팅이 여기에 있고 Rotation 은 PS 에셋별로 들어갈 만한 설정.
			CreatedFX->SetWorldScale3D(FVector(FxCreateScale, FxCreateScale, FxCreateScale));
			CreatedFX->SetWorldRotation(InPSSetup.FxCreateRot);
			// 혹시라도 필요하면 ItemInfo 빼오는데 일단은 InMarkerWidget 을 활용
			//UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
			//FSingleItemInfoData* ThisItemInfoData = ItemInfoTable ? ItemInfoTable->GetInfoData(InRewardItem.ItemRefID) : NULL;

			// 아이템 material 을 셋업한다
			SetupNamedMIDForFxComp(CreatedFX);
			{
				// ParticleSystem 셋업이 Grade 별로 따로 들어간다고 하니 이 Background panel 셋업은 필요하지 않을 수도.
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
				// Ether Star Number 별갯수 7개 이상일때
				UMaterialInstanceDynamic* ItemStarIconMID = GetFxNamedMID(CreatedFX, Name_FxMID_StarNumer);
				SetStageResultStar(ItemStarIconMID, InRewardItem.StarGrade);
			}

			return CreatedFX;
		}
	}
	return NULL;
}

void UB2UIStageClearRewardFx::UpdateFxMove(float DeltaSecond)
{
	for (int32 RI = 0; RI < CreatedRewardFxSet.Num(); ++RI)
	{
		FStageClearRewardFxSet& ThisSet = CreatedRewardFxSet[RI];
		if (ThisSet.FXComp && !ThisSet.bArrivedAtDest)
		{
			FVector FxCompPos = ThisSet.FXComp->GetComponentLocation();
			FVector ToTargetDir = ThisSet.DestPos - FxCompPos;
			ToTargetDir.Normalize();

			const float RemainingDist = ThisSet.TotalDistance - ThisSet.FlightDistSoFar;
			// 목표지점까지 거리가 다른 아이템들 간에 움직임을 좀 맞추기 위해 일종의 spring 모델에 따른 scale 을 도입하였지만 FxMoveBaseAcceleration 보다 작지는 않게. 왜냐면 우리는 박력을 추구하므로
			FxMoveFinalAcc = FxMoveBaseAcceleration + (RemainingDist * FxMoveAccAdjustConst);

			ThisSet.CurrentSpeed = FMath::Min(ThisSet.CurrentSpeed + FxMoveFinalAcc * DeltaSecond, FxMoveMaxSpeed);
			FVector ThisMoveAmount = ToTargetDir * (ThisSet.CurrentSpeed * DeltaSecond);

			ThisSet.FXComp->SetWorldLocation(FxCompPos + ThisMoveAmount);

			ThisSet.FlightTimeSoFar += DeltaSecond;
			// 처음 산정한 거리보다 멀리 갔다면 도착한 걸로 치고 타겟 위치에 고정.
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

void UB2UIStageClearRewardFx::OnSingleRewardArriveAtDest(int32 RewardIndex)
{
	UWorld* TheWorld = GetWorld();
	if (CreatedRewardFxSet.IsValidIndex(RewardIndex))
	{
		FStageClearRewardFxSet& ThisSet = CreatedRewardFxSet[RewardIndex];
		
		const float HowEarly = ThisSet.MinOverlayHiddenTime - ThisSet.FlightTimeSoFar;
		if (HowEarly > 0.0f && TheWorld)
		{ // 아이템 overlay 를 보여도 될 시간보다 일찍 도착했으므로 타이머 셋업.
			TheWorld->GetTimerManager().SetTimer(ThisSet.ItemOverlayInfoShowTH, FTimerDelegate::CreateUObject(this, &UB2UIStageClearRewardFx::ShowRewardOverlayInfo, RewardIndex), HowEarly, false);
		}
		else
		{ // 바로 아이템 overlay 정보를 보여도 됨.
			ShowRewardOverlayInfo(RewardIndex);
		}
	}

	OnSingleRewardArriveAtDest_BP(RewardIndex);
}

void UB2UIStageClearRewardFx::ShowRewardOverlayInfo(int32 RewardIndex)
{
	if (CreatedRewardFxSet.IsValidIndex(RewardIndex))
	{
		FStageClearRewardFxSet& ThisSet = CreatedRewardFxSet[RewardIndex];
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

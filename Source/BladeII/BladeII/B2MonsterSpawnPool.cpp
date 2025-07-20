//Fill out your copyright notice in the Description page of Project Settings.
//Project BladeII, Action Square
#include "B2MonsterSpawnPool.h"

#include "Engine.h"
#if WITH_EDITOR
#include "FileHelpers.h"
#include "Editor.h"
#endif
#include "Components/BillboardComponent.h"
#include "Materials/Material.h"
#include "BladeIIGameMode.h"
#include "B2StageGameMode.h"
#include "B2RaidGameMode.h"
#include "B2TutorialGameMode.h"

#include "B2StageManager.h"
#include "BladeIIPlayer.h"
#include "B2AutoWayPoint.h"
#include "BladeIIWorldSettings.h"
#include "Event.h"
#include "B2UIDocHelper.h"
#include "B2UIDocRaid.h"
#include "B2StageEventDirector.h"
#include "B2GameLoadingProgress.h"

#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"
#include "B2AssetLoader.h"
#include "Event.h"
#include "NavigationSystem.h"

void UB2SummonSettingAsset::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	CheckDataIntegrity();
#endif
}

const TArray<FMonsterSpawnPoolSummonSettings>* UB2SummonSettingAsset::GetInfoArrayForDifficulty(EStageDifficulty InDifficulty) const
{
	if (PerDifficultyInfoMap.Num() > 0)
	{
		int32 CastedDifficultyInt = static_cast<int32>(InDifficulty);
		const FWrappedSummonSettingsArray* FoundSetting = PerDifficultyInfoMap.Find(CastedDifficultyInt);
		if (!FoundSetting && InDifficulty != EStageDifficulty::ESD_Normal)
		{ // Normal 난이도로 폴백 시도
			FoundSetting = PerDifficultyInfoMap.Find(static_cast<int32>(EStageDifficulty::ESD_Normal));
		}
		if (FoundSetting)
		{
			return &(FoundSetting->DataArray);
		}
	}
	return NULL;
}

#if WITH_EDITOR
void UB2SummonSettingAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CheckDataIntegrity();
}
void UB2SummonSettingAsset::CheckDataIntegrity()
{
	if (InfoArray_DEPRECATED.Num() > 0 && PerDifficultyInfoMap.Num() == 0)
	{ // 새로 추가한 데이터 없을 때 기본 설정했던 걸로 모든 난이도 데이터 채워넣기
		for (int32 SDI = static_cast<int32>(EStageDifficulty::ESD_Normal); SDI < static_cast<int32>(EStageDifficulty::ESD_End); ++SDI)
		{
			FWrappedSummonSettingsArray NewSettingArray;
			NewSettingArray.DataArray = InfoArray_DEPRECATED;
			PerDifficultyInfoMap.Add(SDI, NewSettingArray);
		}
		this->MarkPackageDirty();
	}
	else
	{
#if !PLATFORM_MAC
		FString WrongKeyWarnStr;
		for (TMap<int32, FWrappedSummonSettingsArray>::TIterator InfoArrayIt(PerDifficultyInfoMap); InfoArrayIt; ++InfoArrayIt)
		{
			EStageDifficulty CastedDifficultyKey = static_cast<EStageDifficulty>(InfoArrayIt.Key());
			if (CastedDifficultyKey <= EStageDifficulty::ESD_None || CastedDifficultyKey >= EStageDifficulty::ESD_End)
			{
				WrongKeyWarnStr += FString::Printf(TEXT("%d, "), InfoArrayIt.Key());
			}
		}
		if (WrongKeyWarnStr.Len() > 0)
		{
			WrongKeyWarnStr = FString::Printf(TEXT("SummonSettingAsset %s 에 존재하지 않는 난이도 숫자를 키 값으로 갖는 element 가 있음\r\n해당 키 값: "), *this->GetName())
				+ WrongKeyWarnStr;
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WrongKeyWarnStr));
		}
#endif
	}
}
#endif

AB2MonsterSpawnPool::AB2MonsterSpawnPool(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
		// Settings almost from AInfo.
		PrimaryActorTick.bCanEverTick = false;
		bReplicates = false;
		NetUpdateFrequency = 10.0f;
		SetReplicateMovement(false);
		SetCanBeDamaged(false);
	
		bCheckedForBeginWave = false;
	
		DefaultCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DefaultCollisionComponent"));
		SetRootComponent(DefaultCollisionComponent);
		DefaultCollisionComponent->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
		DefaultCollisionComponent->SetCollisionProfileName(CollisionProfileName_Trigger);
		
	#if WITH_EDITOR && WITH_EDITORONLY_DATA
		EditorSpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("MonsterSpawnPoolSprite"));
		if (!IsRunningCommandlet() && (EditorSpriteComponent != nullptr))
		{
			// Structure to hold one-time initialization
			struct FConstructorStatics
			{
				ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
				FName ID_B2SpawnPool;
				FText NAME_B2SpawnPool;
				FConstructorStatics()
					: SpriteTexture(TEXT("/Engine/EditorResources/S_Actor"))
					, ID_B2SpawnPool(TEXT("B2SpawnPool"))
					, NAME_B2SpawnPool(FText::FromString(TEXT("B2SpawnPoolEditorSprite")))
				{
				}
			};
			static FConstructorStatics ConstructorStatics;
	
			EditorSpriteComponent->Sprite = ConstructorStatics.SpriteTexture.Get();
			EditorSpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_B2SpawnPool;
			EditorSpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_B2SpawnPool;
			EditorSpriteComponent->AttachToComponent(DefaultCollisionComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			EditorSpriteComponent->bIsScreenSizeScaled = true;
			EditorSpriteComponent->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f)); // Make it a little bigger.
		}
	#endif
	
		CachedStageManager = NULL;
	
		bUseOwnTypeMapInfo = false;
		TypeMapClassAsset = NULL;
		
		SummonSettingAsset = NULL;
	
		bDisableAllSpawn = false;
	
		CurrentClientStageId = 1;
		CurrentDifficultyLevel = (int32)EStageDifficulty::ESD_Normal;
		
		bForceClearing = false;
		bAllWaveDoneForCurrentStage = false;
		CurrentGameMode	= nullptr;
}

UBoxComponent* AB2MonsterSpawnPool::GetDefaultCollisionComponent() const { return DefaultCollisionComponent; }

class UB2NPCClassInfoBox* AB2MonsterSpawnPool::GetMobClassInfoBox()
{
	// 옛날 옛적에 InfoAsset 접근 코드들이 정리 안되어 있던 시절에 코드가 좀 더 길었어서 이렇게 따로 빠져 있는 거 ㅡㅡ;
	UB2NPCClassInfoBox* ClassInfoBox = StaticFindMobClassInfoBox(this);
	BII_CHECK(ClassInfoBox);

	return ClassInfoBox;
}

void AB2MonsterSpawnPool::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	// Not possible at PostLoad or PostRegisterAllComponents
	// OnConstruction gets called at loading time and PostEditChange event
	if (GIsEditor)
	{
		RegenerateEditingActors();
		EnsureHaveFixedSpawnTransforms();
		RegenerateComponentNameDisplay();
	}
#endif
}

void AB2MonsterSpawnPool::PostLoad()
{
	Super::PostLoad();
	#if WITH_EDITOR
		// 혹시라도 레벨 이름 바꾸면 redirector 처리가 안되므로 여기서도 검사.
		UWorld* TheWorld = GetWorld();
		for (FMonsterSpawnPoolPerStageSettings& ThisPerStageSet : PerStageSettings)
		{
			for (FMonsterSpawnPoolWaveLevelStreamingSettings& ThisLevelStreamingSet : ThisPerStageSet.WaveLevelStreamingSettings)
			{
				ThisLevelStreamingSet.StreamingSetting.EditorCheckForProperSetting(TheWorld);
			}
		}
	#endif
}

void AB2MonsterSpawnPool::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
}

const FMonsterSpawnPoolPerStageSettings* AB2MonsterSpawnPool::GetDesiredStageSetting(int32 InClientStageId, EStageDifficulty DifficultyLevel, bool bFallBackToLowerDifficulty)
{
	// GetDesiredSpawnPoolOfSettingFromList 에서도 비슷한 기능을 함. AB2MonsterSpawnPool 액터를 대상으로

	TArray<FMonsterSpawnPoolPerStageSettings*> FoundSettingOfClientStageId; // StageNum 만 맞는 거 먼저 걸러냄.
	for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
	{
		FMonsterSpawnPoolPerStageSettings& ThisSetting = PerStageSettings[SI];
		if (ThisSetting.TargetStageNum == InClientStageId)
		{
			FoundSettingOfClientStageId.Add(&ThisSetting);
		}
	}
	if (FoundSettingOfClientStageId.Num() > 0)
	{
		// 높은 난이도부터 iteration 하도록 정렬
		FoundSettingOfClientStageId.Sort([](const FMonsterSpawnPoolPerStageSettings& A, const FMonsterSpawnPoolPerStageSettings& B) { return A.TargetDifficultyLevel > B.TargetDifficultyLevel; });

		for (FMonsterSpawnPoolPerStageSettings* ThisSetting : FoundSettingOfClientStageId)
		{
			if (
				((EStageDifficulty)ThisSetting->TargetDifficultyLevel == DifficultyLevel) ||
				(bFallBackToLowerDifficulty && (EStageDifficulty)ThisSetting->TargetDifficultyLevel < DifficultyLevel) // 정확히 똑같은 난이도 세팅이 없을 때 가장 가까운 보다 낮은 난이도로 폴백.
				)
			{
				return ThisSetting;
			}
		}
		// 아직도 리턴을 못했다면 더 높은 난이도 세팅만 있던 것인데 그걸로라도 가장 가까운 걸로 폴백 시도.. 하는 건 일단 보류하자.. 
		// Lower 에만 적용. 다른 곳에도 lower 로만 fallback 하는 코드가 있기도 하고.
		//if (bFallBackToLowerDifficulty)
		//{
		//	return FoundSettingOfClientStageId[FoundSettingOfClientStageId.Num() - 1];
		//}
	}

	return NULL;
}

int32 AB2MonsterSpawnPool::GetStageWaveNum(int32 InClientStageId, EStageDifficulty DifficultyLevel)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(InClientStageId, DifficultyLevel);
	if (StageSetting)
		return StageSetting->WaveSettings.Num();

	return 0;
}

const FMonsterSpawnPoolWaveSettings* AB2MonsterSpawnPool::GetStageWaveSetting(int32 InClientStageId, EStageDifficulty DifficultyLevel, int32 WaveNum)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(InClientStageId, DifficultyLevel);
	if (StageSetting && WaveNum >= 0 && WaveNum < StageSetting->WaveSettings.Num())
	{
		return &(StageSetting->WaveSettings[WaveNum]);
	}

	return NULL;
}

void AB2MonsterSpawnPool::GetAllStageSummonSetting(TArray<FMonsterSpawnPoolSummonSettings>& OutMatchingSettings, int32 InClientStageId, EStageDifficulty DifficultyLevel, int32 SummonIndex)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(InClientStageId, DifficultyLevel);
	// 이번 스테이지에서 특별히 오버라이딩한 세팅 먼저
	if (StageSetting)
	{
		for (int32 SI = 0; SI < StageSetting->SummonSettings.Num(); ++SI)
		{
			if (StageSetting->SummonSettings[SI].SummonIndex == SummonIndex)
			{
				FMonsterSpawnPoolSummonSettings NewSetting = StageSetting->SummonSettings[SI];
				NewSetting.TransientMySettingIndex = SI; // 소환 몹 수 제한에 좀 사용할 일이 있어서 세팅 인덱스를 잠깐 저장해 둔다..
				OutMatchingSettings.Add(NewSetting);
			}
		}
	}

	// 만일 스테이지 세팅에서 찾은 게 없다면 공통 에셋 세팅에서 찾는다.
	if (OutMatchingSettings.Num() == 0)
	{
		const UB2SummonSettingAsset* SettingAssetObj = SummonSettingAsset ? Cast<UB2SummonSettingAsset>(SummonSettingAsset->GetDefaultObject()) : NULL;
		const TArray<FMonsterSpawnPoolSummonSettings>* FinalSettingArrayFromAsset = SettingAssetObj ? SettingAssetObj->GetInfoArrayForDifficulty(DifficultyLevel) : NULL;
		if (FinalSettingArrayFromAsset)
		{
			for (int32 SI = 0; SI < FinalSettingArrayFromAsset->Num(); ++SI)
			{
				if ((*FinalSettingArrayFromAsset)[SI].SummonIndex == SummonIndex)
				{
					FMonsterSpawnPoolSummonSettings NewSetting = (*FinalSettingArrayFromAsset)[SI];
					NewSetting.TransientMySettingIndex = SI; // 소환 몹 수 제한에 좀 사용할 일이 있어서 세팅 인덱스를 잠깐 저장해 둔다..
					OutMatchingSettings.Add(NewSetting);
				}
			}
		}
	}
}

int32 AB2MonsterSpawnPool::GetStageWaveNumBySettingIndex(int32 SettingIndex)
{
	// 걍 인덱스 맞춤
	if (SettingIndex >= 0 && SettingIndex < PerStageSettings.Num())
	{
		return PerStageSettings[SettingIndex].WaveSettings.Num();
	}
	return 0;
}

int32 AB2MonsterSpawnPool::GetTargetStageIdBySettingIndex(int32 SettingIndex)
{
	if (SettingIndex >= 0 && SettingIndex < PerStageSettings.Num())
	{
		return PerStageSettings[SettingIndex].TargetStageNum;
	}
	return 0;
}
int32 AB2MonsterSpawnPool::GetTargetDifficultyLevelBySettingIndex(int32 SettingIndex)
{
	if (SettingIndex >= 0 && SettingIndex < PerStageSettings.Num())
	{
		return PerStageSettings[SettingIndex].TargetDifficultyLevel;
	}
	return 0;
}

bool AB2MonsterSpawnPool::HasSetupFor(int32 InTestClientStageId, EStageDifficulty InTestDifficulty) const
{
	// 정확히 InTestStageNum 과 InTestDifficulty 에 해당하는 세팅이 있는지 검사
	for (const FMonsterSpawnPoolPerStageSettings& ThisStageSetting : PerStageSettings)
	{
		if (ThisStageSetting.TargetStageNum == InTestClientStageId && ThisStageSetting.TargetDifficultyLevel == static_cast<int32>(InTestDifficulty))
			return true;
	}
	return false;
}

void AB2MonsterSpawnPool::BeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::BeginPlay"));

	Super::BeginPlay();

	BeginPlaySetup();

	// 여기 처음 spawn 하면서 Async 로딩 완료되는 것도 있고 할 것.
	if (AmIActiveForCurrentWorld())
	{
		B2GMLoadingProgCollector::StepCustom(0.5f);
	}
}

void AB2MonsterSpawnPool::BeginPlaySetup()
{
		CurrentClientStageId = GetCurrentClientStageId();
		CurrentDifficultyLevel = GetCurrentStageDifficulty();
	
	#if UE_BUILD_SHIPPING
		bDisableAllSpawn = false;
	#endif
	
		const bool bAmIActiveForThisStage = AmIActiveForCurrentWorld();
		
		// Set the collision profile for any component which will be used as a trigger.
		// We might need some different usage later though..
		TArray<UActorComponent*> AllComps;
		GetComponents(AllComps);
		if (bAmIActiveForThisStage)
		{
			for (int32 CI = 0; CI < AllComps.Num(); ++CI)
			{
				UPrimitiveComponent* PossibleTrigger = Cast<UPrimitiveComponent>(AllComps[CI]);
				if (IsComponentForTriggerPurpose(PossibleTrigger) || IsComponentForTriggerToOpenDialogPurpose(PossibleTrigger))
				{
					PossibleTrigger->SetCollisionProfileName(CollisionProfileName_Trigger);
				}
			}
			// Then, disable it if it requires enabling later.
			for (int32 CI = 0; CI < AllComps.Num(); ++CI)
			{
				UPrimitiveComponent* PossibleLaterEnable = Cast<UPrimitiveComponent>(AllComps[CI]);
				if (IsComponentEnabledLater(PossibleLaterEnable))
				{
					PossibleLaterEnable->SetVisibility(false);
					PossibleLaterEnable->SetCollisionProfileName(CollisionProfileName_NoCollision);
				}
			}
		}
		else
		{// 이번 스테이지에 쓰일 거 아니면 모두 비활성화.
			for (int32 CI = 0; CI < AllComps.Num(); ++CI)
			{
				UPrimitiveComponent* ThisComp = Cast<UPrimitiveComponent>(AllComps[CI]);
				if (ThisComp)
				{
					ThisComp->SetVisibility(false);
					ThisComp->SetCollisionProfileName(CollisionProfileName_NoCollision);
				}
			}
		}
	
		// If we have any old state left, destroy..
		DestroyAllActiveWaves();
	
		if (bAmIActiveForThisStage)
		{
			// Add the same number of elements to RuntimeWaveStates
			const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
			for (int32 WI = 0; WI < StageWaveNum; ++WI)
			{
				RuntimeWaveStates.Add(FMonsterSpawnPoolRuntimeWaveState()); //RuntimeWaveStates.Num()이 GetStageWaveNum()이랑 다를 수 없음. DestroyAllActiveWaves()에서 RuntimeWaveStates array를 초기화.
	
				//SpawnWave() 에서 FinalSpawnNum값을 결정하던 것을 이쪽에서 미리 구해 놓는다.
				//Wave에 등장할 몹의 수를 편하게 구하기 위함.
				const FMonsterSpawnPoolWaveSettings* WaveSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WI);
				if (WaveSetting)
				{
					RuntimeWaveStates[WI].FinalSpawnNum = WaveSetting->SpawnNum;
					if (WaveSetting->SpawnNumRandomSlack > 0)
					{
						RuntimeWaveStates[WI].FinalSpawnNum -= (FMath::RandRange(0, FMath::Min(WaveSetting->SpawnNum, WaveSetting->SpawnNumRandomSlack)));
					}
				}
			}
	
			bForceClearing = false;
			bAllWaveDoneForCurrentStage = false;
	
			RegisterComponentOverlapCallback();
			
			ConditionalBeginWaveCheck(); // Beginning 조건의 wave 시작. 이걸 별도로 call 하게 되는 경우 BeginPlay 는 지난 후에 해야.
		}
}

void AB2MonsterSpawnPool::ConditionalBeginWaveCheck()
{// 일반적으로는 기본적인 BeginPlay 직후에 첫 wave 실행을 검사하게 된다. 어쩌다 필요한 적이 있어서 중복 진입할 수 있게 해 둠.
	if (AmIActiveForCurrentWorld() && !bCheckedForBeginWave)
	{
		bCheckedForBeginWave = true;
		CheckNewWaveSpawnBeginning();
	}
}

bool AB2MonsterSpawnPool::AmIActiveForCurrentWorld()
{
	// PreBeginPlay 이후 시점에 사용 가능.
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GM && B2GM->GetActiveSpawnPool() == this)
	{
		return true;
	}
	return false;
}

void AB2MonsterSpawnPool::BeginDestroy()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::BeginDestroy"));

	UWorld* MyWorld = GetWorld();
	if (MyWorld)
	{
		MyWorld->GetTimerManager().ClearAllTimersForObject(this);
	}

	CurrentGameMode = nullptr;

	Super::BeginDestroy();
}

void AB2MonsterSpawnPool::Destroyed()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::Destroyed"));

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	if (WaveEditingActors.Num())
	{
		// Can get to this point when it is initially placed in editor, without some other destruction handling being called
		// To prevent duplicated EditingActor creation, manually destroy it here.
		DestroyAllEditingActors();
	}
#endif
	// Not sure if we need to do this, but let's kill them all who spawned from here. That looks safer.
	DestroyAllActiveWaves();

	RemoveComponentOverlapCallback();

	UWorld* MyWorld = GetWorld();
	if (MyWorld)
	{
		MyWorld->GetTimerManager().ClearAllTimersForObject(this);
	}

	CurrentGameMode = nullptr;

	Super::Destroyed();
}

void AB2MonsterSpawnPool::DestroyAllActiveWaves() // ForceClearWaves 와는 다른 상태 Reset 목적으로 사용하는 것.
{
	FMobSpawnedBirthplaceInfo NullBirthplaceInfo;

	for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI)
	{
		FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];
		for (int32 FI = 0; FI < CurrState.FellowsInThisWave.Num(); ++FI)
		{
			ABladeIICharacter* ThisFellow = CurrState.FellowsInThisWave[FI];
			if (ThisFellow)
			{ // 이쪽으로 신호 안 오도록 한 후에 죽임.
				ThisFellow->SetBirthPlace(NullBirthplaceInfo);
				ThisFellow->Suicide();
			}
		}
		CurrState.FellowsInThisWave.Empty();
	}

	// We don't need to keep it now.
	RuntimeWaveStates.Empty();

	for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
	{
		FMonsterSpawnPoolSummonedFellowState& ThisSummon = SummonedFellows[SI];
		if (ThisSummon.Summonee)
		{
			ThisSummon.Summonee->SetBirthPlace(NullBirthplaceInfo);
			ThisSummon.Summonee->Suicide();
		}
	}
	SummonedFellows.Empty();
}

void AB2MonsterSpawnPool::GetAllExpectedNPCIDs(TArray<FCombinedNPCClassID>& OutExpectedIDs)
{
	GetEssentialPreloadNPCIDs(OutExpectedIDs);

	const EStageDifficulty Difficulty = static_cast<EStageDifficulty>(CurrentDifficultyLevel);
	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, Difficulty);

	for (int32 Wave = 0; Wave < StageWaveNum; Wave++)
		GetWaveNPCIDs(Wave, OutExpectedIDs);
}

void AB2MonsterSpawnPool::GetWaveNPCIDs(const int32 WaveIndex, TArray<FCombinedNPCClassID>& OutNPCIDs)
{
	const EStageDifficulty Difficulty = static_cast<EStageDifficulty>(CurrentDifficultyLevel);
	if (const auto* WaveSetting = GetStageWaveSetting(CurrentClientStageId, Difficulty, WaveIndex))
	{
		TArray<ECharacterType> AllUsedTypes;
		GatherAllClassAndTypesFromSetting(OutNPCIDs, AllUsedTypes, WaveSetting, nullptr);
	}
}

void AB2MonsterSpawnPool::GetSpawnNPCAssets(const int32 WaveIndex, TArray<FB2AsyncRequestInfo>& OutRequestAssets)
{
	TArray<FCombinedNPCClassID> NPCClassIDs;

	// GetAsyncRequestInfoFromNPCIds 에서는 AddUnique 를 하지 않는다. 
	// 만일 이걸 반복 사용하면서 RequestAsset 들을 모으려는 의도이면 이걸 사용하는 쪽에서 중복되지 않도록 걸러내도록

	GetWaveNPCIDs(WaveIndex, NPCClassIDs);
	if (UB2NPCClassInfoBox* NPCInfoBox = GetMobClassInfoBox())
	{
		NPCInfoBox->GetAsyncRequestInfoFromNPCIds(NPCClassIDs, OutRequestAssets);
	}
}

/** 레벨 시작 이후에 Async 로딩을 해도 되는 것 이외의.. 처음부터 로딩해야 하는 거 모음. */
void AB2MonsterSpawnPool::GetEssentialPreloadNPCIDs(TArray<FCombinedNPCClassID>& OutNPCIDs)
{
	const EStageDifficulty Difficulty = static_cast<EStageDifficulty>(CurrentDifficultyLevel);
	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, Difficulty);

	TArray<ECharacterType> AllUsedTypes; // Class 직접 지정 이외의 것들.

	//
	// 각 Wave 별로 Class 를 직접 명시한 것들은 게임 시작 이후의 Async 로딩을 위해 GetWaveNPCIDs 로 옮겨간 상태.
	// TypeMap 정도는 특정 Wave 이후의 것을 모을 수도 있을 듯.
	// SummonSetting 은 처음 한큐에 로딩해야 함.
	//

	// 소환 세팅은 Wave 구분 없음. 모두 첫방에 로딩해야
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, Difficulty);
	if (StageSetting)
	{
		for (int32 SI = 0; SI < StageSetting->SummonSettings.Num(); ++SI)
		{
			GatherAllClassAndTypesFromSetting(OutNPCIDs, AllUsedTypes, NULL, &(StageSetting->SummonSettings[SI]));
		}
	}
	const auto* SettingAssetObj = SummonSettingAsset ? Cast<UB2SummonSettingAsset>(SummonSettingAsset->GetDefaultObject()) : NULL;
	const TArray<FMonsterSpawnPoolSummonSettings>* FinalSettingArrayFromAsset = SettingAssetObj ? SettingAssetObj->GetInfoArrayForDifficulty(Difficulty) : NULL;
	if (FinalSettingArrayFromAsset)
	{
		for (int32 SI = 0; SI < FinalSettingArrayFromAsset->Num(); ++SI)
		{
			GatherAllClassAndTypesFromSetting(OutNPCIDs, AllUsedTypes, NULL, &((*FinalSettingArrayFromAsset)[SI]));
		}
	}


	// WaveSettings 에서 Type 만 모음. 실제 Spawn 세팅에 사용한 Type 들을 긁어오기 위해. Type 에 맵핑되는 클래스는 많아도 Type 자체는 많지 않다.
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		TArray<FCombinedNPCClassID> LocalTempIDs;
		GatherAllClassAndTypesFromSetting(LocalTempIDs, AllUsedTypes, GetStageWaveSetting(CurrentClientStageId, Difficulty, WI), NULL);
	}

	// ClassEnum 외에 Type 으로 지정한 것들에 대해 필요한 것들 찾아내기.
	UB2MonsterClassTypeMap* TypeMapAsset = TypeMapClassAsset ? Cast<UB2MonsterClassTypeMap>(TypeMapClassAsset->GetDefaultObject()) : NULL;
	if (bUseOwnTypeMapInfo || TypeMapAsset)
	{
		TArray<FMonsterTypeMapSingleInfo>& FinalTypeMapInfoArray = bUseOwnTypeMapInfo ? OwnTypeMapInfo : TypeMapAsset->InfoArray;

		for (int32 TMI = 0; TMI < FinalTypeMapInfoArray.Num(); ++TMI)
		{
			FMonsterTypeMapSingleInfo& CurrTypeMapInfo = FinalTypeMapInfoArray[TMI];

			// TypeMap 에 등록된 것들 중 실제로 spawn pool 에 세팅힌 type 인 경우만. TypeMap 에 등록된 전체를 가져오는 거나 대체로는 차이 없겠지만.
			for (int32 UTI = 0; UTI < AllUsedTypes.Num(); ++UTI)
			{
				if (CurrTypeMapInfo.CharacterType == AllUsedTypes[UTI])
				{
					OutNPCIDs.AddUnique(FCombinedNPCClassID(CurrTypeMapInfo.CharacterClassEnum, CurrTypeMapInfo.ClassVariation));
					break;
				}
			}
		}
	}
}

/** MonsterSpawnPool 을 통한 NPCClass 의 Preload 시점 이후 Async 로딩 개수 제한 설정.
 * 이 설정을 DeviceProfile 에 넣을 수 있음. */
static TAutoConsoleVariable<int32> CVarPostGameLoadNPCAsyncLoadLimit(
	TEXT("r.PostGameLoadNPCAsyncLoadLimit"),
	//////////
	3, // 설정에 대한 테스트 결과 http://172.168.2.105:8090/pages/viewpage.action?pageId=4822058
	//////////
	TEXT("Limit the maximum number of NPCClass data that can be loaded after world BeginPlay, in asynchronous manner."),
	ECVF_Default);

void AB2MonsterSpawnPool::PreloadPoolMonsters()
{
	// 맵 로딩 시점에 미리 로딩해 놓을 것들.
	// 여기서 로딩 안 한 것들은 게임 시작 이후 백그라운드 로딩을 하고자 하는 의도.

	TArray<FCombinedNPCClassID> PreloadNPCIDs;

	GetEssentialPreloadNPCIDs(PreloadNPCIDs);

	// 전역 설정으로 Async 로딩을 할 최소 wave index 이전까지 긁어오고.. 0 은 첨부터 필요할 테니 기본 포함
	const int32 BasicAsyncBeginWave = FMath::Max(GetBasicAsyncLoadBeginWave(), 1);
	for (int32 Wave = 0; Wave < BasicAsyncBeginWave; Wave++)
		GetWaveNPCIDs(Wave, PreloadNPCIDs);

	// 여기까지가 기본적으로 긁어모은 Preload 클래스들
	const TArray<FCombinedNPCClassID> BasicAsummedPreloadIDs = PreloadNPCIDs;

	//
	// 추가 기준을 통해 Async 백그라운드 로딩을 해야 하는 게 기준 이상으로 많으면 그 이전까지는 Preload 에 포함시킨다.
	//

	// 일단 끝까지 필요한 게 얼마나 되는지 긁어옴. 후반 wave 에 필요한 것일수록 긁어온 리스트의 뒤에 위치.
	const int32 MaxWaveNum = GetStageWaveNum(CurrentClientStageId, static_cast<EStageDifficulty>(CurrentDifficultyLevel));
	for (int32 Wave = BasicAsyncBeginWave; Wave < MaxWaveNum; Wave++)
		GetWaveNPCIDs(Wave, PreloadNPCIDs);

	IConsoleVariable* CVarAsyncLoadLimit = IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostGameLoadNPCAsyncLoadLimit"));
	const int32 AsyncLoadLimit = CVarAsyncLoadLimit ? FMath::Max(CVarAsyncLoadLimit->GetInt(), 0) : 0;

	// CVar 제한 이전의 async 대상 개수
	const int32 BasicAssumedAsyncNum = PreloadNPCIDs.Num() - BasicAsummedPreloadIDs.Num();
	// CVar 제한이 걸린 async 대상 개수
	int32 FinalAsyncReserveNum = FMath::Min(BasicAssumedAsyncNum, AsyncLoadLimit);

	// 일단 레이드 진입이 안되는 문제 때문에 임시 주석처리.
	// 꼭 물어봐야함.
	//check(PreloadNPCIDs.Num() > FinalAsyncReserveNum);

	ReservedNPCClassIDsForAsyncLoad.Empty();
	// 허가된 갯수만큼 뒤에서부터 차례대로 Reserved 리스트에 넣는데 첫 연출 돌아가는 와중에 완료를 어렵게 하는 (무거운) Blacklist 는 제외시킨다.
	int32 ReservedCount = 0;
	for (int32 ChkIdx = PreloadNPCIDs.Num() - 1; ChkIdx >= BasicAsummedPreloadIDs.Num(); --ChkIdx)
	{
		FCombinedNPCClassID& ThisID = PreloadNPCIDs[ChkIdx];
		if (ReservedCount < FinalAsyncReserveNum && !UB2NPCClassInfoBox::IsInSpawnPoolAsyncBlacklist(ThisID.GetNPCClassEnum()))
		{
			ReservedNPCClassIDsForAsyncLoad.Add(ThisID);
			PreloadNPCIDs.RemoveAt(ChkIdx);
			++ReservedCount;
			if (ReservedCount >= FinalAsyncReserveNum)
			{
				break;
			}
		}
	}
	check(PreloadNPCIDs.Num() >= BasicAsummedPreloadIDs.Num());

	// 최종적으로 취합된 Preload 리스트로 로딩 시작.
	TArray<FB2AsyncRequestInfo> PreloadAssets;
	if (UB2NPCClassInfoBox* NPCInfoBox = GetMobClassInfoBox())
	{
		NPCInfoBox->GetAsyncRequestInfoFromNPCIds(PreloadNPCIDs, PreloadAssets);
		// TryAsyncLoad 를 하지만 이걸 사용하는 게 맵 로딩 시점이라 결국 Flush 내지는 비스무리하게 되어 시작 전에 로딩될 것.
		NPCInfoBox->TryAsyncLoad(TEXT("MonsterPool Preload"), PreloadAssets);
	}
}

void AB2MonsterSpawnPool::TryAsyncLoadReservedMonsters()
{
	// PreloadPoolMonsters 에서 로딩 못한 NPCClass 들을 게임 시작 이후에 Async 요청할 목적
	UB2NPCClassInfoBox* NPCInfoBox = GetMobClassInfoBox();
	if (NPCInfoBox && ReservedNPCClassIDsForAsyncLoad.Num() > 0)
	{
		TArray<FB2AsyncRequestInfo> AsyncLoadAssets;
		NPCInfoBox->GetAsyncRequestInfoFromNPCIds(ReservedNPCClassIDsForAsyncLoad, AsyncLoadAssets);
		const FString RequestName = FString::Printf(TEXT("MonsterPool ReservedAsyncReq %s"), *GetName());
		NPCInfoBox->TryAsyncLoad(RequestName, AsyncLoadAssets);
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
		NPCInfoBox->DevTestMarkLastAsyncReqTime(RequestName); // 다수의 요청에 대해 제각각 시간 측정을 하지 못하므로 실제로 시간 측정을 하고자 하는 스팟에서만 사용
#endif
		ReservedNPCClassIDsForAsyncLoad.Empty();
	}
}

void AB2MonsterSpawnPool::TryAsyncLoadMonsters(int32 BeginWave, int32 EndWave /* = INDEX_NONE (== MaxWave) */)
{
	// PostGameLoadNPCAsyncLoadLimit 도입 이전의 코드인데, 혹시라도 Wave 범위를 콕 찍어서 async 로딩을 할 필요가 생기면..

	TArray<FSoftObjectPath> MyPoolMonsterAssets;
	
	const int32 Begin = FMath::Max<int32>(0, BeginWave);
	
	const EStageDifficulty Difficulty = static_cast<EStageDifficulty>(CurrentDifficultyLevel);
	const int32 WaveMax = GetStageWaveNum(CurrentClientStageId, Difficulty);
	const int32 End = (EndWave == INDEX_NONE || EndWave > WaveMax) ? WaveMax : EndWave;

	TArray<FB2AsyncRequestInfo> AsyncLoadInfoList;
	for (int32 Wave = Begin; Wave <= End; Wave++)
	{
		TArray<FB2AsyncRequestInfo> LocalAssetList;
		GetSpawnNPCAssets(Wave, LocalAssetList);
		// GetSpawnNPCAssets 성격상 그쪽서 AddUnique 하는 건 아닌 거 같아 여기서 매 Wave 마다 AddUnique.
		for (FB2AsyncRequestInfo& ThisOneToAdd : LocalAssetList)
		{
			AsyncLoadInfoList.AddUnique(ThisOneToAdd);
		}
	}

	if (UB2NPCClassInfoBox* NPCInfoBox = GetMobClassInfoBox())
		NPCInfoBox->TryAsyncLoad(GetName(), AsyncLoadInfoList);
}

void AB2MonsterSpawnPool::SetForcedPerStageSettings(int32 InClientStageNum, int32 InTargetDifficulty)
{
	PerStageSettings[0].TargetStageNum = InClientStageNum;
	PerStageSettings[0].TargetDifficultyLevel = InTargetDifficulty;
}

const int32 AB2MonsterSpawnPool::GetBasicAsyncLoadBeginWave()
{
	// 기본적으로 NPCClass 데이터의 Async 로딩을 적용하고자 하는 최초 wave 넘버. 이 이전 wave 랑 typemap, summon 등은 일차적인 Preload 대상이 된다.
	// 이것과 이후 wave 가 모두 async 로딩이 되는 건 아니고 PostGameLoadNPCAsyncLoadLimit 에 의해 추가로 Preload 를 하고 남은 나머지가 async 대상이 된다.
	const int32 AsyncLoadBeginWave = 1;

	return AsyncLoadBeginWave;
}

void AB2MonsterSpawnPool::GatherAllClassAndTypesFromSetting(TArray<FCombinedNPCClassID>& OutCombinedIDs, TArray<ECharacterType>& OutTypes,
	const FMonsterSpawnPoolWaveSettings* WaveSetting, const FMonsterSpawnPoolSummonSettings* SummonSetting)
{
	if (WaveSetting == NULL && SummonSetting == NULL)
	{
		return;
	}

	// WaveSetting 혹은 SummonSetting 둘 중 하나에서 최종 설정들을 가져온다.
	const TArray<FMonsterSpawnClassInfo>& SpawnClasses = WaveSetting ? WaveSetting->SpawnClasses : SummonSetting->SpawnClasses;
	const TArray<FMonsterSpawnTypeSpecific>& SpawnTypes = WaveSetting ? WaveSetting->SpawnCharacterTypes : SummonSetting->SpawnCharacterTypes;

	for (int32 SI = 0; SI < SpawnClasses.Num(); ++SI)
	{
		const FMonsterSpawnClassInfo& ThisSpawnClassInfo = SpawnClasses[SI];

		if (ThisSpawnClassInfo.CharacterClassEnum != ENPCClass::ENC_End)
		{ // CharacterClassEnum 설정된 것은 바로 리턴값에 추가.
			OutCombinedIDs.AddUnique(FCombinedNPCClassID(ThisSpawnClassInfo.CharacterClassEnum, ThisSpawnClassInfo.ClassVariation));
		}
	}

	if (SpawnClasses.Num() == 0 && SpawnTypes.Num() > 0)
	{
		// 이 경우 TypeMap 을 쓰게 되는데 사용하는 타입을 모두 모아놓는다.
		for (int32 TI = 0; TI < SpawnTypes.Num(); ++TI)
		{
			OutTypes.AddUnique(SpawnTypes[TI].CharacterType);
		}
	}
}

void AB2MonsterSpawnPool::GetAllExpectedDialogCodeNames(TArray<FName>& OutAllDlgCode)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (StageSetting)
	{
		for (int32 EI = 0; EI < StageSetting->WaveDialogEvents.Num(); ++EI)
		{
			const FMonsterSpawnPoolWaveDialogSettings& CurrSetting = StageSetting->WaveDialogEvents[EI];

			if (AB2StageEventDirector::CheckSkipForManageMode(this, CurrSetting.ManageMode, false) == EStageEventSkipType::SEST_Play)
			{
				// 사용하는 상황에서 다른 데에서 긁어온 것들을 한데 합칠 수 있으므로 AddUnique 함.
				OutAllDlgCode.AddUnique(CurrSetting.DialogCodeName);
			}
		}
	}
}

bool AB2MonsterSpawnPool::CustomSpawnWave(int32 WaveNum)
{
	// SpawnWave already handles a lot..
	return SpawnWave(WaveNum);
}

// 월드 재로딩 없이 SpawnPool 을 처음부터 다시 사용 가능하도록. 단, 이것만 한다고 해서 레벨 전체가 리셋되는 건 아님. 다른 것들도 처리해야 할 것.
void AB2MonsterSpawnPool::ResetAssumeNoWorldLoading()
{
#if !UE_BUILD_SHIPPING
	CheckResetAssumeNoWorldLoadingCondition(this);
#endif

	RemoveComponentOverlapCallback();

	bCheckedForBeginWave = false;
	BeginPlaySetup();
}

bool AB2MonsterSpawnPool::SummonMobs(class ABladeIICharacter* Summoner, int32 SummonIndex, float MaxDistance)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::SummonMobs"));

	TArray<FMonsterSpawnPoolSummonSettings> MatchingSettings; // SummonIndex 에 대한 모든 세팅 리스트
	GetAllStageSummonSetting(MatchingSettings, CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, SummonIndex);

	if (MatchingSettings.Num() == 0 || Summoner == NULL || bAllWaveDoneForCurrentStage)
	{
		return false;
	}

	for (int32 SI = 0; SI < MatchingSettings.Num(); ++SI)
	{
		FMonsterSpawnPoolSummonSettings& CurrSetting = MatchingSettings[SI];
		// TransientMySettingIndex 에 원래 SummonSettings 내에서의 인덱스가 들어가 있음.
		check(CurrSetting.TransientMySettingIndex >= 0)

		for (int32 MI = 0; MI < CurrSetting.SpawnNum; ++MI) // RandomSlack 같은 게 들어갈 수도..
		{			
			if (CountSummonedMobsBySetting(CurrSetting.TransientMySettingIndex) >= CurrSetting.MaxObjectByThisSetting){
				continue; // 현 세팅에서의 최대 summon 한도에 도달한 경우.
			}

			ABladeIICharacter* NewSummoned = SummonSingleMob(Summoner, CurrSetting, MI, MaxDistance);
			if (NewSummoned == NULL){
				continue;
			}

			// (아마도 StageManager 쪽으로) 통지. 소환된 경우는 WaveNum 같은 게 의미없고 bIsSummoned 정도만. 물론 boss 일리도 없겠지. ㅋ
			SpawnPoolMobSpawnClass<ABladeIICharacter*, const FMobSpawnedBirthplaceInfo&>::GetInstance().Signal(NewSummoned, FMobSpawnedBirthplaceInfo(this, -1, -1, false, true));

			FMonsterSpawnPoolSummonedFellowState NewState;
			NewState.SummonSettingIndex = CurrSetting.TransientMySettingIndex; // 몇번 세팅에 의해 spawn 되었는지 기록해서 추후 summon 제한 시 사용.
			NewState.Summoner = Summoner;
			NewState.Summonee = NewSummoned;
			SummonedFellows.Add(NewState);
			
			if (!NewSummoned->IsAlive())
			{ // SpawningNoDamageGuard 를 함에도 불구하고 출생등록 이전에 죽는 경우가 발생하는 거 같음. 이 시점에 죽어 있으면 강제 제거.
				OnSpawnedMonsterDead(NewSummoned, -1, -1, true);
			}
		}
	}

	return true;
}

ABladeIICharacter* AB2MonsterSpawnPool::SummonSingleMob(class ABladeIICharacter* Summoner, FMonsterSpawnPoolSummonSettings& InSetting, int32 SummonObjIndex, float MaxDistance)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::SummonSingleMob"));

	UWorld* TheWorld = GetWorld();
	BII_CHECK(TheWorld);

	if (TheWorld == NULL || Summoner == NULL)
	{
		return NULL;
	}

	FSpawnClassFromSetting_OutParams SpawnClassParam;
	GetSpawnClassFromSetting(SpawnClassParam, NULL, &InSetting, SummonObjIndex);

	// SummonerLocation 주변으로 무작위 방향에, MaxDistance 거리 이내로 Spawn location 을 정함.
	FVector2D RandSpawnDir(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));
	if (FMath::Abs(RandSpawnDir.X) <= KINDA_SMALL_NUMBER && FMath::Abs(RandSpawnDir.Y) <= KINDA_SMALL_NUMBER)
	{
		// 이런 경우가 얼마나 자주 나올지 모르겠지만 흔할 거 같진 않으니 적당히 예외처리.
		RandSpawnDir.X = FMath::FRandRange(0.1f, 1.0f);
	}
	RandSpawnDir.Normalize();
	RandSpawnDir *= FMath::RandRange(MaxDistance * 0.1f, MaxDistance);

	FTransform SpawnTransform(Summoner->GetActorLocation() + FVector(RandSpawnDir.X, RandSpawnDir.Y, 100.0f)); // 혹시라도 끼일 소지를 줄이기 위해 약간 높게 spawn 시킨 후 땅에 snap.

	ABladeIICharacter* NewSummoned = SpawnSingleMonsterCommon(SpawnClassParam, SpawnTransform);
	if (NewSummoned != nullptr)
		Summoner->AddMinion(NewSummoned);

	return NewSummoned;
}

int AB2MonsterSpawnPool::CountSummonedMobsBySetting(int32 SummonSettingIndex)
{
	int32 LocalCounted = 0;
	for (FMonsterSpawnPoolSummonedFellowState& CurrSummonState : SummonedFellows)
	{
		// 겸사겸사 체크좀..
		checkSlow(CurrSummonState.Summonee && CurrSummonState.Summonee->IsValidObj() && CurrSummonState.Summoner && CurrSummonState.Summoner->IsValidObj());

		if (CurrSummonState.SummonSettingIndex == SummonSettingIndex){
			LocalCounted++;
		}
	}
	return LocalCounted;
}

bool AB2MonsterSpawnPool::SpawnWave(int32 WaveNum, bool bDeferWaveStartCallBack /*= false*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::SpawnWave"));

		checkSlow(GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel) == RuntimeWaveStates.Num());
	
		UWorld* TheWorld = GetWorld();
		BII_CHECK(TheWorld);
	
		const FMonsterSpawnPoolWaveSettings* WaveSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WaveNum);
	
		if (TheWorld && WaveSetting)
		{
			FMonsterSpawnPoolRuntimeWaveState& CurrRuntimeState = RuntimeWaveStates[WaveNum];
	
			if (CurrRuntimeState.bWaveStarted == false)
			{
	#if !UE_BUILD_SHIPPING
				if (bDisableAllSpawn)
				{
					CurrRuntimeState.FinalSpawnNum = 0; // Let OnSingleWaveStarted to be called without any actual spawn.
				}
	#endif
	
				ABladeIIWorldSettings* B2WS = Cast<ABladeIIWorldSettings>(GetWorldSettings());
				const bool bDynamicShadowDisallowed = B2WS ? B2WS->IsDynamicShadowDisallowedWave(GetCurrentClientStageId(), GetCurrentStageDifficulty(), WaveNum) : false;
	
				for (int32 SI = 0; SI < CurrRuntimeState.FinalSpawnNum; ++SI)
				{
					FSpawnClassFromSetting_OutParams SpawnClassParam;
					GetSpawnClassFromSetting(SpawnClassParam, WaveSetting, NULL, SI);
	
					FTransform SpawnTransform = GetSpawnTransform(WaveSetting, SI);
	
					ABladeIICharacter* NewMonster = SpawnSingleMonsterCommon(SpawnClassParam, SpawnTransform);
	
					if (NewMonster)
					{
						if (bDynamicShadowDisallowed)
						{
							// 레벨 환경에 따라 도저히 (modulated) dynamic shadow 를 못 쓸 환경이면 꺼 준다.. 아쉽지만.
							TurnOffMeshComponentsDyamicShadowForModulated(NewMonster);
						}
	
						// (아마도 StageManager 쪽으로) 통지.
						// 에디터에서 본 오브젝트 번호를 인게임용 설정에서 사용하는 상황으로 인해 에디터용 SetRepresentingInfo 호출시와 같은 순서의 spawn object 인덱스를 넣어줌.
						SpawnPoolMobSpawnClass<ABladeIICharacter*, const FMobSpawnedBirthplaceInfo&>::GetInstance().Signal(NewMonster, FMobSpawnedBirthplaceInfo(this, WaveNum, SI, (bool)WaveSetting->bBossWave, false));
						
						// In the case of random location, do the final check for navigation.
						bool bInvalidNavLocation = false;
						UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(TheWorld);
						if (TheWorld->GetNavigationSystem() && NewMonster->GetMovementComponent()->bAutoActivate)
						{
							FNavLocation NavProjectedLocation;
							// ActorLocation could be different from the location from GetSpawnTransform, due to AdjustIfPossibleButAlwaysSpawn option.
							// That's why we do this check after spawn has succeeded.
							if (NavSys && NavSys->ProjectPointToNavigation(NewMonster->GetActorLocation(), NavProjectedLocation) == false)
							{
								if(WaveSetting->bRandomSpawnLocation)
									bInvalidNavLocation = true;
								else
								{
									if (NavSys && NavSys->ProjectPointToNavigation(NewMonster->GetActorLocation(), NavProjectedLocation, FVector(1000,1000,500)) == false)
									{
										//wrong position
									}
									else
									{
										NavProjectedLocation.Location.Z += FMath::Max(100.0f ,NewMonster->GetCapsuleComponent()->GetScaledCapsuleRadius());
										NewMonster->SetActorLocation(NavProjectedLocation.Location);
									}
								}
							}
						}
	
						if (bInvalidNavLocation == true && !WaveSetting->bBossWave &&
							// NavMesh 가 제대로 빌드되었더라도 몹이 너무 크거나 하면 여하간 실패하기 좋다. 중요할 가능성이 있는 설정이라면 spawn 을 계속.
							WaveSetting->WaveCompl_TerminateAll == false && WaveSetting->SpawnNum > 1)
						{
							UE_LOG(LogBladeII, Log, TEXT("[MonsterSpawnPool] Cancel spawning %s due to invalid navigation"), *NewMonster->GetName());
							TheWorld->DestroyActor(NewMonster);
						}
						else
						{
							// Spawn succeeded. Finally add to the entry.
							CurrRuntimeState.FellowsInThisWave.Add(NewMonster);
							
							if (!NewMonster->IsAlive())
							{ // SpawningNoDamageGuard 를 함에도 불구하고 출생등록 이전에 죽는 경우가 발생하는 거 같음. 이 시점에 죽어 있으면 강제 제거.
								OnSpawnedMonsterDead(NewMonster, WaveNum, SI, false);
							}
						}
						
						NewMonster->SetWeightRewardGold(SpawnClassParam.OutWeightRewardGold);
						NewMonster->SetCanDropItem(SpawnClassParam.OutCanDropItem);
					}
				}
	
				// Set the runtime state too.
				// At this point, we might still have no monsters actually spawned due to various reasons, but set WaveStarted true anyway, to let stage proceed.
				CurrRuntimeState.bWaveStarted = true;
				CurrRuntimeState.bWaveCompleted = false;
	
				if (bDeferWaveStartCallBack == false)
				{
					// 여기서 하지 않으면 밖에서 따로 호출해야 함
					OnSingleWaveStarted(WaveNum);
				}
	
				return true;
			}
		}

	return false;
}

ABladeIICharacter* AB2MonsterSpawnPool::SpawnSingleMonsterCommon(FSpawnClassFromSetting_OutParams& InClassParam, FTransform& InSpawnTransform)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::SpawnSingleMonsterCommon"));

	UWorld* TheWorld = GetWorld();
	BII_CHECK(TheWorld);

	if (TheWorld == NULL)
	{
	return NULL;
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.bDeferConstruction = true; // Need deferred construction and manually call FinishSpawning. AutoPossessAI 설정 및 NPCClassInfo 적용을 위해 DeferConstruction 이 필요.
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.ObjectFlags |= RF_Transient;	// We do not want to save them into the map.

	ABladeIICharacter* NewMonster = NULL;

	if (InClassParam.OutClassEnum != ENPCClass::ENC_End) // NPCClass enum has priority..
	{
		UB2NPCClassInfoBox* ClassInfoBox = GetMobClassInfoBox();
		if (ClassInfoBox)
		{
			NewMonster = CurrentGameMode->SpawnCharacter(ClassInfoBox, InClassParam.OutClassEnum, InClassParam.OutClassVariation, InSpawnTransform, SpawnInfo);
		}
	}
	else if (InClassParam.OutClassBP)
	{
		NewMonster = Cast<ABladeIICharacter>(TheWorld->SpawnActor(InClassParam.OutClassBP, &InSpawnTransform, SpawnInfo));
	}

	// Now, you must know NewMoster is going to be able to be nullptr and it doesn't mean that you're in trouble.
	if (NewMonster)
	{
		NewMonster->BeginSpawningNoDamageGuard(); // 프로젝타일이 그 자리에 있다거나 하면 FinishSpawning 도중에 맞아서 죽을 수 있고 그렇게 되면 death 시 처리가 안되므로 막는다.
		if (InClassParam.bOutBossMob)
		{
			NewMonster->CharacterType = ECharacterType::ECT_BossMob; // 딱히 의미가 있으려나 모르겠지만 ㅋ
		}
		else if (InClassParam.OutCharacterType != ECharacterType::ECT_End)
		{
			// NPCClassInfo 를 통해 세팅한 기본 CharacterType 과 MonsterClassTypeMap 을 통한 맵핑은 다를 수 있으므로 최종적으로 spawn 에 사용한 MonsterClassTypeMap 맵핑값을 할당.
			NewMonster->CharacterType = InClassParam.OutCharacterType;
		}
		if (InClassParam.bOutFixedLocation)
		{
			UCharacterMovementComponent* CharMoveComp = NewMonster->GetCharacterMovement();
			if (CharMoveComp)
			{
				// 실 コ蕨譚왠齡포왠齡，랍꼇角盧땡렘蕨
				CharMoveComp->bOrientRotationToMovement = false;
				NewMonster->bUseControllerRotationYaw = true;
			}
		}

		// NPCClassInfo 에서도 하는 건데 여기서도 material override.
		OverrideMaterialForNewSpawned(NewMonster, InClassParam.MaterialOverrides);

		// Need to set this to let SpawnDefaultController be called. 추가로 이렇게 FinishSpawning 을 delay 시킴으로서 NPCClassInfo 세팅 후 BeginPlay 가 불리는 효과가 있음.
		NewMonster->AutoPossessAI = EAutoPossessAI::Spawned;
		NewMonster->FinishSpawning(InSpawnTransform);

		if (InClassParam.bOutBossMob && InClassParam.OutClassEnum != ENPCClass::ENC_End)
		{
			// BossMob 세팅인 경우는 UI Doc 에도 업데이트 해 줄 것이 좀 있음.
			UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
			if (DocBattle)
			{
				// SetbCurrentlyBossAppeared 를 하기 전에 MobClass 와 Variation 을 먼저 세팅해야 엉뚱한 NPCClassInfo 로딩을 하지 않는다.
				DocBattle->SetBossMobClass(InClassParam.OutClassEnum);
				DocBattle->SetBossMobClassVariation(InClassParam.OutClassVariation);
				DocBattle->SetbCurrentlyBossAppeared(true); // 죽을 때 다시 리셋해야 함.
				DocBattle->SetBossHPMax(NewMonster->GetMaxHealth());
				DocBattle->SetBossHPCurrent(NewMonster->GetHealth());

				DocBattle->SetbBossQTEBreakable(NewMonster->IsQTEBreakable());
				DocBattle->SetBossQTEBreakPointMax(NewMonster->GetMaxQTEBreakPoint());
				DocBattle->SetBossQTEBreakPointCurrent(NewMonster->GetCurrentQTEBreakPoint());
			}
		}

		// Snap to ground might become optional later..
		SnapBladeIICharacterToGround(NewMonster);

		NewMonster->EndSpawningNoDamageGuard();
	}

	return NewMonster;
}

void AB2MonsterSpawnPool::OverrideMaterialForNewSpawned(class ABladeIICharacter* NewSpawned, const TArray<class UMaterialInterface*>& MaterialOverrides
#if WITH_EDITOR
	, USkeletalMeshComponent* InEditorMeshComp // If specified as valid value, NewSpawned will be ignored.
#endif
)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::OverrideMaterialForNewSpawned"));

#if WITH_EDITOR
	if (!GIsEditor)
	{
		check(!InEditorMeshComp); // 오용 방지.
	}
#endif

	USkeletalMeshComponent* MeshComp =
#if WITH_EDITOR
		InEditorMeshComp ? InEditorMeshComp : // 에디터 프리뷰 목적으로 Component 를 직접 전달하는 경우 그걸 사용.
#endif
		(NewSpawned ? NewSpawned->GetBaseMesh() : NULL); // Mob 은 특수한 attachment 외에 그 기본이 multi-component 구성이 아닌 한은 BaseMesh 만 사용.

	if (!MeshComp || MaterialOverrides.Num() == 0)
		return;

	int32 BaseMeshMaterialNum = MeshComp->GetNumMaterials();

	int32 ApplyMIIndex = 0;
	for (; ApplyMIIndex < FMath::Min(MaterialOverrides.Num(), BaseMeshMaterialNum); ++ApplyMIIndex)
	{
		if (MaterialOverrides[ApplyMIIndex]) // Null 세팅을 통해 특정 섹션을 뛰어넘을 수 있음.
		{
			MeshComp->SetMaterial(ApplyMIIndex, MaterialOverrides[ApplyMIIndex]);
		}
	}

	// 기본 element 개수보다 MaterialOverrides 를 많이 넣었다면 순서대로 attach 된 컴포넌트에 적용.
	if (MaterialOverrides.Num() > ApplyMIIndex)
	{
		TArray<USceneComponent*> AllAttached;
		MeshComp->GetChildrenComponents(false, AllAttached);
		for (USceneComponent* ThisChild : AllAttached)
		{
			UMeshComponent* ThisChildMesh = Cast<UMeshComponent>(ThisChild);
			if (ThisChildMesh)
			{
				if (MaterialOverrides[ApplyMIIndex])
				{ // 역시나 NULL 로 두면 특정 attach 는 뛰어넘을 수 있음.
// Attach 된 것은 굳이 element 인덱스 까지 가지 않는다. 그것까지 필요하다면 지나친 요구일 듯.
					ThisChildMesh->SetMaterial(0, MaterialOverrides[ApplyMIIndex]);
				}
				++ApplyMIIndex;
				if (MaterialOverrides.Num() <= ApplyMIIndex)
				{
					break;
				}
			}
		}
	}

}

bool AB2MonsterSpawnPool::GetSpawnClassFromSetting(FSpawnClassFromSetting_OutParams& OutParams, const FMonsterSpawnPoolWaveSettings* InWaveSetting, const FMonsterSpawnPoolSummonSettings* InSummonSetting, int32 SpawnObjIndex)
{
	OutParams.OutCharacterType = ECharacterType::ECT_End; // CharacterType 으로 spawn 하도록 한 경우에만 값이 세팅될 것.

	if (InWaveSetting == NULL && InSummonSetting == NULL)
	{
		return false;
	}

	// WaveSetting 혹은 SummonSetting 에서 설정을 빼 옴. WaveSetting 이 우선.
	const TArray<FMonsterSpawnClassInfo>& SpawnClassSetting = InWaveSetting ? InWaveSetting->SpawnClasses : InSummonSetting->SpawnClasses;
	const TArray<FMonsterSpawnTypeSpecific>& SpawnTypeSetting = InWaveSetting ? InWaveSetting->SpawnCharacterTypes : InSummonSetting->SpawnCharacterTypes;

	// 재사용 가능한 wave 인 경우, 골드를 드랍하지 않게한다. 소환인 경우도 마찬가지.
	const bool bNonGoldDropSetting = (InWaveSetting && InWaveSetting->bCanReuseWave) || (InWaveSetting == NULL && InSummonSetting != NULL);

	if (SpawnClassSetting.Num()) // Class 직접 지정이 우선.
	{
		const bool bOnlyOneSpawnClassElem = (SpawnClassSetting.Num() == 1);
		// Filtering by class and index range.
		TArray<FMonsterSpawnClassInfo> ClassFilteredInfo = SpawnClassSetting.FilterByPredicate(
			[&SpawnObjIndex, &bOnlyOneSpawnClassElem](FMonsterSpawnClassInfo InClassInfo)
			{
				return (
					(InClassInfo.CharacterClassEnum != ENPCClass::ENC_End || InClassInfo.CharacterClassBP != NULL) // See if class setting itself is valid.
					&&
					(bOnlyOneSpawnClassElem || InClassInfo.MaxSpawnObjIndex < 0 || // Well, if we got just one element, don't have to consider the index range. Same for negative MaxSpawnObjIndex setting.
						(InClassInfo.MinSpawnObjIndex <= SpawnObjIndex && SpawnObjIndex <= InClassInfo.MaxSpawnObjIndex))
					);
			}
		);

		if (ClassFilteredInfo.Num() > 0)
		{
			// Now, randomly select spawn class among specified-and-filtered classes.
			FMonsterSpawnClassInfo& FinalInfo = ClassFilteredInfo[FMath::RandRange(0, ClassFilteredInfo.Num() - 1)];

			// Either one of them should be valid..
			OutParams.OutClassEnum = FinalInfo.CharacterClassEnum;
			OutParams.OutClassVariation = FinalInfo.ClassVariation;
			OutParams.OutClassBP = FinalInfo.CharacterClassBP;
			//재사용 가능한 스폰풀인 경우, 골드를 드랍하지 않게한다. 소환인 경우도 마찬가지.
			OutParams.OutWeightRewardGold = bNonGoldDropSetting ? 0 : FinalInfo.RewardGoldWeight;
			OutParams.OutCanDropItem = FinalInfo.bCanDropItem;
			OutParams.OutNetPlay = FinalInfo.bNetPlay;
			OutParams.MaterialOverrides = FinalInfo.MaterialOverrides;
			OutParams.bOutFixedLocation = FinalInfo.bFixedLocation;

			// 이건 클래스 세팅에 들어간 건 아닌데 여하간 여기에 넣어두면 써먹기 좋아서. 근데 bBossWave 자체가 WaveSetting 보다는 클래스 세팅에 있어야 어울릴 만한 성격이기도 하다.
			OutParams.bOutBossMob = InWaveSetting ? InWaveSetting->bBossWave : false;
			return true;
		}
	}
	else if (SpawnTypeSetting.Num())
	{
		// Select proper element from SpawnCharacterTypes array. If we got duplicated setting, it will do random pickup.
		const bool bOnlyOneTypesElem = (SpawnTypeSetting.Num() == 1);
		TArray<FMonsterSpawnTypeSpecific> FilteredTypesArray = SpawnTypeSetting.FilterByPredicate(
			[&SpawnObjIndex, &bOnlyOneTypesElem](FMonsterSpawnTypeSpecific InTypeInfo)
			{
				return (bOnlyOneTypesElem || InTypeInfo.MaxSpawnObjIndex < 0 || // Well, if we got just one element, don't have to consider the index range. Same for negative MaxSpawnObjIndex setting.			
					InTypeInfo.MinSpawnObjIndex <= SpawnObjIndex && SpawnObjIndex <= InTypeInfo.MaxSpawnObjIndex);
			}
		);

		if (FilteredTypesArray.Num() > 0)
		{
			const FMonsterSpawnTypeSpecific* SpawnCharacterTypeForThis = &(FilteredTypesArray[FMath::RandRange(0, FilteredTypesArray.Num() - 1)]);

			// Use the CDO of BP class.
			UB2MonsterClassTypeMap* TypeMapAsset = TypeMapClassAsset ? Cast<UB2MonsterClassTypeMap>(TypeMapClassAsset->GetDefaultObject()) : NULL;

			if (
				SpawnCharacterTypeForThis &&
				((bUseOwnTypeMapInfo == false && TypeMapAsset != NULL) || (bUseOwnTypeMapInfo == true && OwnTypeMapInfo.Num() > 0))
				)
			{
				// Look up the monster info and randomly select from among matching category.
				// Either from TypeMapClassAsset or its own OwnTypeMapInfo.
				TArray<FMonsterTypeMapSingleInfo>& InfoArray = bUseOwnTypeMapInfo ? OwnTypeMapInfo : TypeMapAsset->InfoArray;

				// Get all the valid monster classes having matching type among given info array.
				TArray<FMonsterTypeMapSingleInfo> ClassFilteredInfo = InfoArray.FilterByPredicate(
					[&SpawnCharacterTypeForThis](FMonsterTypeMapSingleInfo InTypeInfo)
					{
						return (InTypeInfo.CharacterClassEnum != ENPCClass::ENC_End &&
							InTypeInfo.CharacterType == SpawnCharacterTypeForThis->CharacterType);
					}
				);

				if (ClassFilteredInfo.Num() > 0)
				{
					FMonsterTypeMapSingleInfo& FinalInfo = ClassFilteredInfo[FMath::RandRange(0, ClassFilteredInfo.Num() - 1)];
					// In this case, only class enum is available.
					OutParams.OutClassEnum = FinalInfo.CharacterClassEnum;
					OutParams.OutClassVariation = FinalInfo.ClassVariation;
					OutParams.OutCharacterType = FinalInfo.CharacterType; // Spawn 후 CharacterType 값을 덮어쓰도록 결과 파라미터로 전달.
					//재사용 가능한 스폰풀인 경우, 골드를 드랍하지 않게한다.
					OutParams.OutWeightRewardGold = bNonGoldDropSetting ? 0 : SpawnCharacterTypeForThis->RewardGoldWeight;
					OutParams.OutCanDropItem = SpawnCharacterTypeForThis->bCanDropItem;
					OutParams.MaterialOverrides = FinalInfo.MaterialOverrides;
					OutParams.bOutBossMob = InWaveSetting ? InWaveSetting->bBossWave : false; // 이건 클래스 세팅에 들어간 건 아닌데 여하간 여기에 넣어두면 써먹기 좋아서
					return true;
				}
			}
		}
	}

	return false;
}

FTransform AB2MonsterSpawnPool::GetSpawnTransform(const FMonsterSpawnPoolWaveSettings* InWaveSetting, int32 SpawnObjIndex)
{
	FVector SpawnLocation = this->GetActorLocation(); // Default spawn location if nothing can be really found.
	FRotator SpawnRotation = this->GetActorRotation();

	if (InWaveSetting->bRandomSpawnLocation == false && SpawnObjIndex >= 0 && SpawnObjIndex < InWaveSetting->FixedSpawnTransforms.Num())
	{
		SpawnLocation = InWaveSetting->FixedSpawnTransforms[SpawnObjIndex].GetLocation(); // In this case, simply get it.
		SpawnRotation = InWaveSetting->FixedSpawnTransforms[SpawnObjIndex].GetRotation().Rotator();
	}
	else // Random calculation
	{
		// Find specified component for spawn location, if not specified use the root component (default collision)
		// SpawnArea 는 ShapeComponent 여야 함.
		UShapeComponent* SpawnAreaComponent = Cast<UShapeComponent>(FindComponentByName(InWaveSetting->SpawnAreaComponentName));
		if (SpawnAreaComponent == NULL)
		{
			SpawnAreaComponent = Cast<UShapeComponent>(GetRootComponent());
		}

		if (SpawnAreaComponent)
		{
			SpawnLocation = SpawnAreaComponent->GetComponentLocation();
			SpawnRotation = SpawnAreaComponent->GetComponentRotation();

			if (UBoxComponent* SpawnAreaBox = Cast<UBoxComponent>(SpawnAreaComponent))
			{
				FVector AreaBoxExtent = SpawnAreaBox->Bounds.BoxExtent;
				float ThisRandX = FMath::FRandRange(-1.0f * AreaBoxExtent.X, AreaBoxExtent.X);
				float ThisRandY = FMath::FRandRange(-1.0f * AreaBoxExtent.Y, AreaBoxExtent.Y);

				// Give some random shuffling on spawn location.
				SpawnLocation = SpawnLocation + FVector(ThisRandX, ThisRandY, 0.0f);
			}
			else if (USphereComponent* SpawnAreaSphere = Cast<USphereComponent>(SpawnAreaComponent))
			{
				// Random distance from the center location.
				float SpawnAreaRadius = SpawnAreaSphere->GetScaledSphereRadius();
				float ThisRandRadius = FMath::FRandRange(0.0f, SpawnAreaRadius);
				float ThisRandAngle = FMath::FRandRange(0.0f, 2.0f * PI);
				float ThisRandX = ThisRandRadius * FMath::Cos(ThisRandAngle);
				float ThisRandY = ThisRandRadius * FMath::Sin(ThisRandAngle);

				// Give some random shuffling on spawn location.
				SpawnLocation = SpawnLocation + FVector(ThisRandX, ThisRandY, 0.0f);
			}
			else if (UCapsuleComponent* SpawnAreaCapsule = Cast<UCapsuleComponent>(SpawnAreaComponent))
			{
				// Almost like sphere component..
				float SpawnAreaRadius = SpawnAreaCapsule->GetScaledCapsuleRadius();
				float ThisRandRadius = FMath::FRandRange(0.0f, SpawnAreaRadius);
				float ThisRandAngle = FMath::FRandRange(0.0f, 2.0f * PI);
				float ThisRandX = ThisRandRadius * FMath::Cos(ThisRandAngle);
				float ThisRandY = ThisRandRadius * FMath::Sin(ThisRandAngle);

				// Give some random shuffling on spawn location.
				SpawnLocation = SpawnLocation + FVector(ThisRandX, ThisRandY, 0.0f);
			}
		}
	}

	return FTransform(SpawnRotation, SpawnLocation);
}

void AB2MonsterSpawnPool::OnSingleWaveStarted(int32 WaveNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::OnSingleWaveStarted"));

	ExecuteWaveEventsCommonInternal(WaveNum, EWaveCustomEventTiming::EWCET_WaveStart); // 각종 부가 이벤트들

	// 헥헥 여기도 이벤트 통지
	SpawnPoolSingleWaveStartClass<AB2MonsterSpawnPool*, int32>::GetInstance().Signal(this, WaveNum);

	ExtraOnSingleWaveStarted_BP(WaveNum);

	if (WaveNum < RuntimeWaveStates.Num()) // This must be true. We might check on it.
	{
		if (RuntimeWaveStates[WaveNum].FellowsInThisWave.Num() == 0)
		{
			// This means no monsters were actually spawned even after a normal spawning procedure.
			// There can be various reasons, but we can expect this can happen in normal circumstances.
			// So, ensure it proceed to other wave, if any.

			// Handle it like a spawned monster gets destroyed, while there were nobody actually.
			// It will handle complete check and next wave stuff anyway.
			// 실제로 DyingMonster 가 없는 특수 케이스이므로 다른 곳을 거치지 않고 OnSpawnedMonsterDead 를 직접 call 하는데 만일 필요하다면 SpawnPoolWaveMobDeadClass 를 사용하여 이벤트 broadcast 를 하도록.
			OnSpawnedMonsterDead(NULL, WaveNum, -1, false);
		}
	}
}
void AB2MonsterSpawnPool::ExecuteWaveEventsCommonInternal(int32 WaveNum, EWaveCustomEventTiming InTiming)
{
	// 설정한 Component 들 On/Off, 액터나 레벨로의 신호 등..
	// Wave 시작 혹은 완료 시

	TArray<FMonsterSpawnPoolWaveLevelStreamingSettings> LevelStreamingSettings;
	GetWaveLevelStreamingSettings(LevelStreamingSettings, InTiming, WaveNum);
	for (FMonsterSpawnPoolWaveLevelStreamingSettings& ThisSetting : LevelStreamingSettings)
	{
		// 설정에 따라 즉석에서 Async 로딩 Flush 가 될 수 있음. 아니면 WaveLevelStreamingFlushCB 에서.
		ThisSetting.StreamingSetting.ExecuteStreaming(GetWorld(), ThisSetting.StreamingSettingFlushTH, FTimerDelegate::CreateUObject(this, &AB2MonsterSpawnPool::WaveLevelStreamingFlushCB, ThisSetting.StreamingSettingFlushTH));
	}

	TArray<FMonsterSpawnPoolWaveComponentOnOffSettings> CompOnOffSettings;
	GetWaveCompOnOffSettings(CompOnOffSettings, InTiming, WaveNum);
	for (int32 CI = 0; CI < CompOnOffSettings.Num(); ++CI)
	{
		FMonsterSpawnPoolWaveComponentOnOffSettings& ThisSetting = CompOnOffSettings[CI];
		if (ThisSetting.bCheckForOnOrOff)
		{
			WaveEnableSingleComponent(ThisSetting.ComponentName);
		}
		else
		{
			WaveDisableSingleComponent(ThisSetting.ComponentName);
		}
	}

	TArray<FName> EventCE;
	GetWaveLevelCustomEventName(EventCE, InTiming, WaveNum);
	for (int32 EI = 0; EI < EventCE.Num(); ++EI)
	{
		FName ThisEventCE = EventCE[EI];
		if (ThisEventCE != NAME_None)
		{
			// For any other custom handling. Can use this in the level blueprint
			// Engine command "CE **"
			GEngine->HandleCeCommand(GetWorld(), *(ThisEventCE.ToString()), *GLog);
		}
	}

	TArray<FMonsterSpawnPoolWaveSignalActorSettings> SignalActorSettings;
	GetWaveSignalActorSettings(SignalActorSettings, InTiming, WaveNum);
	for (int32 SAI = 0; SAI < SignalActorSettings.Num(); ++SAI)
	{
		FMonsterSpawnPoolWaveSignalActorSettings& ThisSetting = SignalActorSettings[SAI];
		if (ThisSetting.SignalingActor)
		{
			//ThisSetting.SignalingActor->NotifyActorCustomEvent(ThisSetting.OptionalEventName, this);
		}
	}

	TArray<WaveDialogSettings_OutParam> DialogSettings;
	GetWaveDialogCodeName(DialogSettings, InTiming, WaveNum);
	for (WaveDialogSettings_OutParam& ThisDialogSetting : DialogSettings)
	{
		OpenDialogClass<FName, bool>::GetInstance().Signal(ThisDialogSetting.DialogCode, (bool)ThisDialogSetting.bMaintainBattleUIs);
	}
}

void AB2MonsterSpawnPool::OnSpawnedMonsterDead(ABladeIICharacter* DyingMonster, int32 DeadMobWaveNum, int32 DeadMobObjIndex, bool bIsSummonedMob)
{
	checkSlow(!DyingMonster || DyingMonster->IsValidObj());

	if (bIsSummonedMob)
	{
		for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
		{
			FMonsterSpawnPoolSummonedFellowState& CurrState = SummonedFellows[SI];

			if (CurrState.Summonee == DyingMonster)
			{
				checkSlow(CurrState.Summonee && CurrState.Summonee->IsValidObj());
				SummonedFellows.RemoveAt(SI); // 소환 몹 리스트는 따로. 사실 Summonee 는 unique 해야 함.
				--SI;
			}
		}
	}
	else
	{
#if !UE_BUILD_SHIPPING
		if (DyingMonster) // DyingMonster 자체는 NULL 일 수 있으니 거기에 check 를 걸지는 않는다.
		{
			checkSlow(DyingMonster->IsValidObj());
			bool bFoundInFellowList = false;
			for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI)
			{
				bFoundInFellowList |= (RuntimeWaveStates[WI].FellowsInThisWave.Find(DyingMonster) != INDEX_NONE);
			}
			checkSlow(bFoundInFellowList);
		}
#endif

		// DeadMobWaveNum 을 볼 수도 있겠지만 모두 돌아주자.
		for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI)
		{
			RuntimeWaveStates[WI].FellowsInThisWave.Remove(DyingMonster);
		}

		if (DyingMonster)
		{
			TArray<ABladeIICharacter*> RelevantSummoneeList; // Summoner 중 하나가 죽었을 때 거기서 소환된 Summonee 들을 처리하기 위한 리스트. Summonee 처리 도중 SummonedFellows 가 비워질 것이므로 따로 복사.
			for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
			{
				FMonsterSpawnPoolSummonedFellowState& CurrState = SummonedFellows[SI];
				if (CurrState.Summoner == DyingMonster)
				{
					// 해당 Summoner 에서 소환된 Summonee 리스트를 따로 취합해서 제거.
					RelevantSummoneeList.Add(CurrState.Summonee);
				}
			}

			for (ABladeIICharacter* CurrSummonee : RelevantSummoneeList)
			{
				checkSlow(CurrSummonee && CurrSummonee->IsValidObj());
				{
#if !UE_BUILD_SHIPPING
					UE_LOG(LogBladeII, Log, TEXT("Suicide due to Summoner's death : Summoner %s, Summonee %s"), *(DyingMonster->GetName()), *(CurrSummonee->GetName()));
#endif
					CurrSummonee->Suicide(); // 이걸 통해서도 OnSpawnedMonsterDead 가 불리면서 상단 bIsSummonedMob 쪽으로 처리될 것.
				}
			}
		}
	}

	EnsureCleanFellowList();

	if (DyingMonster && DyingMonster->IsSpawnedAsBossMob())
	{
		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
		if (DocBattle) // BossMob 관련 UI Doc 상태 리셋.
		{
			DocBattle->SetbCurrentlyBossAppeared(false);
		}
	}

	// Don't do the completion check and spawn handling if we are clearing, as that can possibly cause recursive call.
	// 소환된 몹의 경우도 CheckWaveCompletion 을 하지 않는데, 코드 패쓰가 복잡해 질 수 있음. 딱히 이상동작을 하지는 않지만 ForceClearWaves 가 불릴 상황이라면 소환몹을 죽이는 시도를 두 번 할 수 있어서 헷갈림.
	// 소환자가 죽을 때 소환한 몹 처리를 바로 위에서 하므로 CheckWaveCompletion 이 들어는 갈 것이다.
	if (!bForceClearing && !bIsSummonedMob)
	{
		// Let's see if we have kicked shit out of here, as well as check for new wave start.
		CheckWaveCompletion();
	}

#if !UE_BUILD_SHIPPING
	extern bool GCheat_HeroineMode;
	if (GCheat_HeroineMode && !bForceClearing)
	{ // 현재 spawn 된 모든 몹들 죄다 쓸기

		// ForceClearWaves 앞뒤로 wave 시작 상태를 저장했다가 복구한다. Heroine 모드의 목적은 wave 를 끝내는 게 아니라 현재 mob 들을 죽이는 것이므로.
		TArray<FMonsterSpawnPoolRuntimeWaveState> WaveStateCopy = RuntimeWaveStates;
		ForceClearWaves(-1, false); // MarkCompleted 를 해선 안됨.
		for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI)
		{
			RuntimeWaveStates[WI].bWaveStarted = WaveStateCopy[WI].bWaveStarted;
		}
		bForceClearing = false;
		CheckWaveCompletion(); // 남은 애들 쓸어담는 와중에는 CheckWaveCompletion 처리가 누락될 것.
	}
#endif
}

void AB2MonsterSpawnPool::CheckWaveCompletion()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::CheckWaveCompletion"));

	bool bAllCompleted = true;

	checkSlow(GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel) == RuntimeWaveStates.Num());

	for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI)
	{
		FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];
		const FMonsterSpawnPoolWaveSettings* WaveSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WI);
		checkSlow(WaveSetting);

		if (CurrState.bWaveStarted == false)
		{
			// If we got any wave not even started, not completed yet.
			bAllCompleted = false;
		}
		else if (CurrState.FellowsInThisWave.Num() > 0)
		{
#if !UE_BUILD_SHIPPING
			for (ABladeIICharacter* CheckFellow : CurrState.FellowsInThisWave)
			{
				checkSlow(CheckFellow && CheckFellow->IsValidObj());
			}
#endif
			// Active now.
			bAllCompleted = false;
		}
		else if (CurrState.bWaveCompleted == false)
		{
			OnSingleWaveComplete(WI);

			if (WaveSetting->bCanReuseWave && bForceClearing == false)
			{
				// Let it able to start again.
				CurrState.bWaveStarted = false;
				CurrState.bWaveCompleted = false;
				bAllCompleted = false;
			}
			else
			{
				CurrState.bWaveCompleted = true;
			}

			CheckNewWaveSpawnOtherWaveComplete(WI); // Any new wave depend on completed one?
		}
	}

	if (SummonedFellows.Num() > 0) // 소환몹들도 모두 처리해야 함. ㅋ
	{
		bAllCompleted = false;
	}

	if (bForceClearing) // Special case..
	{
		bAllCompleted = true;
	}

	if (bAllCompleted)
	{
		// We did really blow this shit up.
		if (bAllWaveDoneForCurrentStage == false)
		{
			bAllWaveDoneForCurrentStage = true;
			OnAllWaveComplete();
		}
	}
}

void AB2MonsterSpawnPool::EnsureCleanFellowList()
{
	// 흔한 일은 아니지만 개발 도중의 사정 혹은 잘 알지 못하는 문제로 인해 invalid 한 상태로 리스트에 있는 경우가 있다..
	// Assert 및 안전확보 차원에서 리스트에서 비움.
	for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
	{
		FMonsterSpawnPoolSummonedFellowState& CurrState = SummonedFellows[SI];
		if (!IsValid(CurrState.Summonee))
		{ // 내부적으로는 알 수 있게.
			ensureMsgf(0, TEXT("Invalid Mob has been detected from spawn pool summoned list!"));
			SummonedFellows.RemoveAt(SI);
			--SI;
		}
	}
	for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI)
	{
		FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];
		for (int32 MI = 0; MI < CurrState.FellowsInThisWave.Num(); ++MI)
		{
			ABladeIICharacter* ThisFellow = CurrState.FellowsInThisWave[MI];
			if (!IsValid(ThisFellow))
			{
				ensureMsgf(0, TEXT("Invalid Mob has been detected from spawn pool fellow list! Wave %d, Index %d"), WI, MI);
				CurrState.FellowsInThisWave.RemoveAt(MI);
				--MI;
			}
		}
	}
}

void AB2MonsterSpawnPool::OnSingleWaveComplete(int32 WaveNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::OnSingleWaveComplete"));

	// Enable or disable components as specified
	const FMonsterSpawnPoolWaveSettings* CompletedWaveSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WaveNum);
	if (CompletedWaveSetting)
	{
		ExecuteWaveEventsCommonInternal(WaveNum, EWaveCustomEventTiming::EWCET_WaveComplete); // 각종 부가 이벤트들

		// 헥헥 여기도 이벤트 통지
		SpawnPoolSingleWaveCompleteClass<AB2MonsterSpawnPool*, int32>::GetInstance().Signal(this, WaveNum);

		if (CompletedWaveSetting->WaveCompl_TerminateAll)
		{
			ForceClearWaves(); // Finish all
		}
	}

	ExtraOnSingleWaveComplete_BP(WaveNum);
}

void AB2MonsterSpawnPool::WaveEnableSingleComponent(FName ComponentName)
{
	UPrimitiveComponent* ComponentTobeEnable = FindComponentByName(ComponentName);

	if (ComponentTobeEnable == NULL)
	{
		return;
	}

	UShapeComponent* CastedShapeComp = Cast<UShapeComponent>(ComponentTobeEnable);
	UMeshComponent* CastedMeshComp = Cast<UMeshComponent>(ComponentTobeEnable);

	if (CastedShapeComp == NULL)
	{
		// MeshComponent 인 경우라면 눈에 보이는 wall 로 간주
		if (CastedMeshComp)
		{
			CastedMeshComp->SetCollisionProfileName(CollisionProfileName_BlockAll);
		}

		ComponentTobeEnable->SetVisibility(true); // 이 경우는 모두 visibility 를 켜 줌.
		ComponentTobeEnable->SetHiddenInGame(false); // 이게 세팅되었을수도 있으니..
	}
	else // 트리거 용 Collision Component 로 간주.
	{
		ComponentTobeEnable->SetCollisionProfileName(CollisionProfileName_Trigger);
	}
}

void AB2MonsterSpawnPool::WaveDisableSingleComponent(FName ComponentName)
{
	UPrimitiveComponent* ComponentTobeDisable = FindComponentByName(ComponentName);
	if (ComponentTobeDisable == NULL)
	{
		return;
	}

	// 일단 공통적으로 no collision 에 invisible
	ComponentTobeDisable->SetCollisionProfileName(CollisionProfileName_NoCollision);
	ComponentTobeDisable->SetVisibility(false);
}

void AB2MonsterSpawnPool::ForceClearWaves(int32 DesiredFinishWaveNum /*= -1*/, bool bMarkCompleted /*= true*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::ForceClearWaves"));

	checkSlow(GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel) == RuntimeWaveStates.Num());

	bForceClearing = true;

	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		if (DesiredFinishWaveNum >= 0 && WI > DesiredFinishWaveNum)
		{
			continue;
		}

		const FMonsterSpawnPoolWaveSettings* CurrSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WI);
		FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];

		// During the Suicide call, the element will be removed from FellowsInThisWave array. So do this on copied array.
		TArray<class ABladeIICharacter*> FellowsInThisWaveCopy = CurrState.FellowsInThisWave;
		for (int32 MI = 0; MI < FellowsInThisWaveCopy.Num(); ++MI)
		{
			checkSlow(FellowsInThisWaveCopy[MI] && FellowsInThisWaveCopy[MI]->IsValidObj());

#if !UE_BUILD_SHIPPING
			UE_LOG(LogBladeII, Log, TEXT("Suicide by SpawnPool's ForceClearWave : Wave Fellow %s"), *(FellowsInThisWaveCopy[MI]->GetName()));
#endif
			// OnSpawnedMonsterDead will be called by this, but not any more spawn check stuff.
			FellowsInThisWaveCopy[MI]->Suicide();
		}
		checkSlow(CurrState.FellowsInThisWave.Num() == 0); // See OnSpawnedMonsterDead
		FellowsInThisWaveCopy.Empty();

		CurrState.bWaveStarted = true;
		if (bMarkCompleted)
		{
			CurrState.bWaveCompleted = true;
		}
		// We might want to call OnSingleWaveComplete here if wave is not completed yet, but guess that causes endless recursive call.. and other possible issues..?
	}

	// 소환몹들은 DesiredFinishWaveNum 과는 상관없이 모두 날림. DesiredFinishWaveNum 이 사실상 개발용 기능이기도 하고, 이후 wave 의 소환몹이 나오는 것도 말이 안됨 낄낄
	TArray<FMonsterSpawnPoolSummonedFellowState> SummonedFellowsCopy = SummonedFellows;
	for (FMonsterSpawnPoolSummonedFellowState& SummonState : SummonedFellowsCopy)
	{
		if (SummonState.Summonee && SummonState.Summonee->IsAlive())
		{
#if !UE_BUILD_SHIPPING
			UE_LOG(LogBladeII, Log, TEXT("Suicide by SpawnPool's ForceClearWave : Summonee %s"), *(SummonState.Summonee->GetName()));
#endif
			SummonState.Summonee->Suicide();
		}
	}
	checkSlow(SummonedFellows.Num() == 0); // Suicide 에서 OnSpawnedMonsterDead 거치면서 비워졌을 것.
	SummonedFellowsCopy.Empty();

	// Let this to be set in CheckWaveCompletion
	//bAllWaveDoneForCurrentStage = true;
}

void AB2MonsterSpawnPool::GetWaveLevelStreamingSettings(TArray<FMonsterSpawnPoolWaveLevelStreamingSettings>& OutSettings, EWaveCustomEventTiming EventTiming, int32 WaveNum)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (StageSetting)
	{
		const TArray<FMonsterSpawnPoolWaveLevelStreamingSettings>& ThisSetting = StageSetting->WaveLevelStreamingSettings;
		for (int32 EI = 0; EI < ThisSetting.Num(); ++EI)
		{
			if (ThisSetting[EI].EventTiming == EventTiming && ThisSetting[EI].WaveNum == WaveNum)
			{
				OutSettings.Add(ThisSetting[EI]);
			}
		}
	}
}

void AB2MonsterSpawnPool::GetWaveCompOnOffSettings(TArray<FMonsterSpawnPoolWaveComponentOnOffSettings>& OutSelected, EWaveCustomEventTiming EventTiming, int32 WaveNum)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (StageSetting)
	{
		const TArray<FMonsterSpawnPoolWaveComponentOnOffSettings>& CompOnOffSetting = StageSetting->WaveCompOnOffSettings;

		for (int32 EI = 0; EI < CompOnOffSetting.Num(); ++EI)
		{
			if (CompOnOffSetting[EI].EventTiming == EventTiming && CompOnOffSetting[EI].WaveNum == WaveNum)
			{
				OutSelected.Add(CompOnOffSetting[EI]);
			}
		}
	}
}

void AB2MonsterSpawnPool::GetWaveLevelCustomEventName(TArray<FName>& OutEvents, EWaveCustomEventTiming EventTiming, int32 WaveNum)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (StageSetting)
	{
		const TArray<FMonsterSpawnPoolWaveLevelCustomEventSettings>& CESetting = StageSetting->WaveLevelCustomEvents;

		for (int32 EI = 0; EI < CESetting.Num(); ++EI)
		{
			if (CESetting[EI].EventTiming == EventTiming && CESetting[EI].WaveNum == WaveNum)
			{
				OutEvents.Add(CESetting[EI].CustomEventName); // We don't really need to have two custom events having the same condition, but better not miss it if we got.
			}
		}
	}
}

void AB2MonsterSpawnPool::GetWaveSignalActorSettings(TArray<FMonsterSpawnPoolWaveSignalActorSettings>& OutSelected, EWaveCustomEventTiming EventTiming, int32 WaveNum)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (StageSetting)
	{
		const TArray<FMonsterSpawnPoolWaveSignalActorSettings>& CESetting = StageSetting->WaveActorCustomEvents;

		for (int32 EI = 0; EI < CESetting.Num(); ++EI)
		{
			if (CESetting[EI].EventTiming == EventTiming && CESetting[EI].WaveNum == WaveNum)
			{
				OutSelected.Add(CESetting[EI]); // In this case, we might need multiple actors to signal, so put every encountered.
			}
		}
	}
}

void AB2MonsterSpawnPool::GetWaveDialogCodeName(TArray<WaveDialogSettings_OutParam>& OutDialogParams, EWaveCustomEventTiming EventTiming, int32 WaveNum)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (StageSetting)
	{
		const TArray<FMonsterSpawnPoolWaveDialogSettings>& CESetting = StageSetting->WaveDialogEvents;

		for (int32 EI = 0; EI < CESetting.Num(); ++EI)
		{
			const FMonsterSpawnPoolWaveDialogSettings& CurrSetting = CESetting[EI];

			if (CurrSetting.EventTiming == EventTiming && CurrSetting.WaveNum == WaveNum &&
				(AB2StageEventDirector::CheckSkipForManageMode(this, CurrSetting.ManageMode, false) == EStageEventSkipType::SEST_Play))
			{
				WaveDialogSettings_OutParam ThisParam;
				ThisParam.DialogCode = CurrSetting.DialogCodeName;
				ThisParam.bMaintainBattleUIs = CurrSetting.bMaintainBattleUIs;
				OutDialogParams.Add(ThisParam); // We don't really need to have two custom events having the same condition, but better not miss it if we got.
			}
		}
	}
}

void AB2MonsterSpawnPool::OnAllWaveComplete()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MonsterSpawnPool::OnAllWaveComplete"));

	SpawnPoolCompleteClass<AB2MonsterSpawnPool*>::GetInstance().Signal(this);

	ExtraOnAllWaveComplete_BP();
}

void AB2MonsterSpawnPool::WaveLevelStreamingFlushCB(FTimerHandle InUsedTimerHandle)
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		UGameplayStatics::FlushLevelStreaming(this); // 이거 하려고 타이머 걸어놓은 거.
		TheWorld->GetTimerManager().ClearTimer(InUsedTimerHandle);
	}
}

void AB2MonsterSpawnPool::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNewWaveSpawnCollisionTrigger(OtherActor);
}

void AB2MonsterSpawnPool::OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Just handle it as same as BeginOverlap
	CheckNewWaveSpawnCollisionTrigger(OtherActor);
}

void AB2MonsterSpawnPool::OnComponentForDialogBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* Player = Cast<ABladeIIPlayer>(OtherActor);
	if (Player)
	{
		TArray<UPrimitiveComponent*> PlayerOverappingComps;
		Player->GetOverlappingComponents(PlayerOverappingComps);

		for (auto* Comp : PlayerOverappingComps)
		{
			WaveDialogSettings_OutParam DialogCodeParam = GetDialogCodeParamByTriggeringComponent(Comp);
			if (DialogCodeParam.DialogCode != NAME_None)
			{
				OpenDialogClass<FName, bool>::GetInstance().Signal(DialogCodeParam.DialogCode, (bool)DialogCodeParam.bMaintainBattleUIs);
				Comp->OnComponentBeginOverlap.RemoveDynamic(this, &AB2MonsterSpawnPool::OnComponentForDialogBeginOverlapCallback);
				break;
			}
		}
	}
}

void AB2MonsterSpawnPool::RegisterComponentOverlapCallback()
{
	TArray<UActorComponent*> AllComps;
	GetComponents(AllComps);
	for (int32 CI = 0; CI < AllComps.Num(); ++CI)
	{
		UPrimitiveComponent* CurrComp = Cast<UPrimitiveComponent>(AllComps[CI]);
		if (CurrComp == NULL)
			continue;
		if (IsComponentForTriggerPurpose(CurrComp))
		{
			CurrComp->OnComponentBeginOverlap.AddDynamic(this, &AB2MonsterSpawnPool::OnComponentBeginOverlapCallback);
			CurrComp->OnComponentEndOverlap.AddDynamic(this, &AB2MonsterSpawnPool::OnComponentEndOverlapCallback);
		}

		if (IsComponentForTriggerToOpenDialogPurpose(CurrComp))
		{
			CurrComp->OnComponentBeginOverlap.AddDynamic(this, &AB2MonsterSpawnPool::OnComponentForDialogBeginOverlapCallback);
		}
	}
}

void AB2MonsterSpawnPool::RemoveComponentOverlapCallback()
{
	TArray<UActorComponent*> AllComps;
	GetComponents(AllComps);
	for (int32 CI = 0; CI < AllComps.Num(); ++CI)
	{
		UPrimitiveComponent* CurrComp = Cast<UPrimitiveComponent>(AllComps[CI]);

		// Simply remove for all..
		if (CurrComp)
		{
			CurrComp->OnComponentBeginOverlap.RemoveDynamic(this, &AB2MonsterSpawnPool::OnComponentBeginOverlapCallback);
			CurrComp->OnComponentBeginOverlap.RemoveDynamic(this, &AB2MonsterSpawnPool::OnComponentForDialogBeginOverlapCallback);
			CurrComp->OnComponentEndOverlap.RemoveDynamic(this, &AB2MonsterSpawnPool::OnComponentEndOverlapCallback);
		}
	}
}

bool AB2MonsterSpawnPool::TryGetStartTalkSetting(int32& OutGroupID)
{
	OutGroupID = -1;
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (StageSetting && StageSetting->TalkDialogSettings.Num())
	{
		for (const auto& Each : StageSetting->TalkDialogSettings)
		{
			if (Each.EventTiming == ETalkEventTiming::ETET_StageBegin)
			{
				OutGroupID = Each.GroupID;
				return true;
			}
		}
	}

	return false;
}

bool AB2MonsterSpawnPool::TryGetCompleteTalkSetting(int32& OutGroupID)
{
	OutGroupID = -1;
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (StageSetting && StageSetting->TalkDialogSettings.Num())
	{
		for (const auto& Each : StageSetting->TalkDialogSettings)
		{
			if (Each.EventTiming == ETalkEventTiming::ETET_StageFinish)
			{
				OutGroupID = Each.GroupID;
				return true;
			}
		}
	}

	return false;
}

void AB2MonsterSpawnPool::CheckNewWaveSpawnBeginning()
{
	TArray<int32> SpawnedWaves;
	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		const FMonsterSpawnPoolWaveSettings* WaveSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WI);
		if (WaveSetting && WaveSetting->WaveExec_FromBeginning)
		{
			if (InternalSpawnWaveOrDelayCommon(WI, WaveSetting))
			{
				// 즉석에서 SpawnWave 를 한 경우 OnSingleWaveStarted 를 모아서 처리하기 위해 SpawnedWaves 에 번호 추가.
				SpawnedWaves.Add(WI);
			}
		}
	}

	for (int32 SWI = 0; SWI < SpawnedWaves.Num(); ++SWI)
	{
		OnSingleWaveStarted(SpawnedWaves[SWI]); // SpawnWave 호출시 지연시켰던 OnSingleWaveStarted 일괄 처리
	}
}

void AB2MonsterSpawnPool::CheckNewWaveSpawnOtherWaveComplete(int32 JustCompletedWave)
{
	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	checkSlow(StageWaveNum == RuntimeWaveStates.Num());

	TArray<int32> SpawnedWaves;
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		const FMonsterSpawnPoolWaveSettings* CurrSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WI);
		FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];

		if (CurrSetting)
		{
			// For any unstarted wave with matching PostCompleteWaveNum setting.
			if (CurrState.bWaveStarted == false && CurrSetting->WaveExec_PostCompleteWaveNum == JustCompletedWave)
			{
				if (InternalSpawnWaveOrDelayCommon(WI, CurrSetting))
				{
					// 즉석에서 SpawnWave 를 한 경우 OnSingleWaveStarted 를 모아서 처리하기 위해 SpawnedWaves 에 번호 추가.
					SpawnedWaves.Add(WI);
				}
			}
		}
	}
	for (int32 SWI = 0; SWI < SpawnedWaves.Num(); ++SWI)
	{
		OnSingleWaveStarted(SpawnedWaves[SWI]); // SpawnWave 호출시 지연시켰던 OnSingleWaveStarted 일괄 처리
	}
}

void AB2MonsterSpawnPool::DelayedNewWaveSpawn(int32 WaveNum)
{
	// 타이머 클리어
	FTimerManager& TheTimerManager = GetWorldTimerManager();
	FTimerHandle* ExistingTHForThisWave = SpawnWaveDelayTHMap.Find(WaveNum);
	if (ExistingTHForThisWave)
	{
		TheTimerManager.ClearTimer(*ExistingTHForThisWave);
		SpawnWaveDelayTHMap.Remove(WaveNum);
	}

	// This callback is from the timer set at CheckNewWaveSpawnOtherWaveComplete
	SpawnWave(WaveNum);
}

bool AB2MonsterSpawnPool::InternalSpawnWaveOrDelayCommon(int32 ThisWaveIndex, const FMonsterSpawnPoolWaveSettings* ThisWaveSetting)
{ // SpawnDelay 에 따라 DelayedNewWaveSpawn 예약, 혹은 SpawnWave 호출을 하는 공통된 코드 블록.
	// 즉석에서 SpawnWave 를 하게 되면 return true 해서 이번 WaveIndex 에 대해 모종의 처리를 하도록 함.
	if (ThisWaveSetting)
	{
		const float EffectiveSpawnDelay = ThisWaveSetting->GetFinalSpawnDelay(CachedStageManager ? CachedStageManager->IsScenarioMode() : false);
		if (EffectiveSpawnDelay > 0.0f)
		{
			FTimerManager& TheTimerManager = GetWorldTimerManager();
			// 기존에 혹여라도 설치되어 있는 타이머가 있다면 제거.
			FTimerHandle* ExistingTHForThisWave = SpawnWaveDelayTHMap.Find(ThisWaveIndex);
			if (ExistingTHForThisWave)
			{
				TheTimerManager.ClearTimer(*ExistingTHForThisWave);
				SpawnWaveDelayTHMap.Remove(ThisWaveIndex);
			}

			// Set a timer to call SpawnWave later with WaveNum parameter.
			FTimerHandle NewTimerHandle;
			TheTimerManager.SetTimer(NewTimerHandle, FTimerDelegate::CreateUObject(this, &AB2MonsterSpawnPool::DelayedNewWaveSpawn, ThisWaveIndex), EffectiveSpawnDelay, false);
			SpawnWaveDelayTHMap.Add(ThisWaveIndex, NewTimerHandle);
		}
		else
		{
			// Immediate call here.
			SpawnWave(ThisWaveIndex, true);
			return true;  // 리턴값은 단지 SpawnWave 콜 시에 WaveIndex 들 모아서 OnSingleWaveStarted 처리 하려는 거.
		}
	}
	return false;
}

void AB2MonsterSpawnPool::CheckNewWaveSpawnCollisionTrigger(AActor* BeginOverlapActor)
{
	ABladeIIPlayer* OverlappingPlayer = Cast<ABladeIIPlayer>(BeginOverlapActor);
	ABladeIICharacter* OverlappingChar = Cast<ABladeIICharacter>(BeginOverlapActor);
	bool bFromQTEVehicle = false;

	if (OverlappingPlayer == NULL)
	{
		if (OverlappingChar && OverlappingChar->IsControlledByQTEPlayer())
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			ABladeIIPlayer* Player = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;
			if (Player && Player->QTEActor == OverlappingChar)
			{
				bFromQTEVehicle = true;
				OverlappingPlayer = Player;
			}
		}
	}

	if (OverlappingPlayer)
	{
		// No component information from NotifyActorBeginOverlap, so manually get it here.
		TArray<UPrimitiveComponent*> PlayerOverlappingComps;

		if (bFromQTEVehicle)
			OverlappingChar->GetOverlappingComponents(PlayerOverlappingComps);
		else
			OverlappingPlayer->GetOverlappingComponents(PlayerOverlappingComps);

		const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
		checkSlow(StageWaveNum == RuntimeWaveStates.Num());

		TArray<int32> SpawnedWaves;
		for (int32 WI = 0; WI < StageWaveNum; ++WI)
		{
			const FMonsterSpawnPoolWaveSettings* CurrSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WI);
			FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];

			// For any wave settings with valid trigger component name.
			if (CurrState.bWaveStarted == false && CurrSetting && CurrSetting->WaveExec_TriggerComponent != NAME_None)
			{
				for (int32 CI = 0; CI < PlayerOverlappingComps.Num(); ++CI)
				{
					UPrimitiveComponent* CurrComp = PlayerOverlappingComps[CI];

					// See if player is touching a component working as our trigger.
					if (CurrComp->GetOwner() == this && CurrComp->GetName() == CurrSetting->WaveExec_TriggerComponent.ToString())
					{
						if (InternalSpawnWaveOrDelayCommon(WI, CurrSetting))
						{
							// 즉석에서 SpawnWave 를 한 경우 OnSingleWaveStarted 를 모아서 처리하기 위해 SpawnedWaves 에 번호 추가.
							SpawnedWaves.Add(WI);
						}
						break;
					}
				}
			}
		}
		for (int32 SWI = 0; SWI < SpawnedWaves.Num(); ++SWI)
		{
			OnSingleWaveStarted(SpawnedWaves[SWI]); // SpawnWave 호출시 지연시켰던 OnSingleWaveStarted 일괄 처리
		}
	}
}

class UPrimitiveComponent* AB2MonsterSpawnPool::FindComponentByName(const FName& InName)
{
	TArray<UActorComponent*> AllComps;
	GetComponents(AllComps);

	for (int32 CI = 0; CI < AllComps.Num(); ++CI)
	{
		if (AllComps[CI]->GetName() == InName.ToString())
		{
			UPrimitiveComponent* CastedComp = Cast<UPrimitiveComponent>(AllComps[CI]);

			if (CastedComp)
			{
				return CastedComp;
			}
		}
	}

	return NULL;
}

bool AB2MonsterSpawnPool::IsComponentForTriggerPurpose(UActorComponent* InComp)
{
	if (Cast<UPrimitiveComponent>(InComp) && InComp->GetOwner() == this)
	{
		const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
		for (int32 WI = 0; WI < StageWaveNum; ++WI)
		{
			const FMonsterSpawnPoolWaveSettings* CurrSetting = GetStageWaveSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, WI);

			// Found one that has matching WaveExec name condition
			if (CurrSetting->WaveExec_TriggerComponent.ToString() == InComp->GetName())
			{
				return true;
			}
		}
	}

	return false;
}

bool AB2MonsterSpawnPool::IsComponentForTriggerToOpenDialogPurpose(UActorComponent* InComp)
{
	if (Cast<UPrimitiveComponent>(InComp) && InComp->GetOwner() == this)
	{
		const auto* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
		if (StageSetting)
		{
			for (const auto& DialogEvent : StageSetting->WaveDialogEvents)
			{
				if (DialogEvent.TriggerComponentName == InComp->GetFName() &&
					(AB2StageEventDirector::CheckSkipForManageMode(this, DialogEvent.ManageMode, false) == EStageEventSkipType::SEST_Play))
					return true;
			}
		}
	}

	return false;
}

AB2MonsterSpawnPool::WaveDialogSettings_OutParam AB2MonsterSpawnPool::GetDialogCodeParamByTriggeringComponent(UPrimitiveComponent* InComp)
{
	if (InComp && InComp->GetOwner() == this)
	{
		const auto* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
		if (StageSetting)
		{
			for (const FMonsterSpawnPoolWaveDialogSettings& DialogEvent : StageSetting->WaveDialogEvents)
			{
				if (DialogEvent.TriggerComponentName == InComp->GetFName() &&
					(AB2StageEventDirector::CheckSkipForManageMode(this, DialogEvent.ManageMode, false) == EStageEventSkipType::SEST_Play))
				{
					WaveDialogSettings_OutParam RetParam;
					RetParam.DialogCode = DialogEvent.DialogCodeName;
					RetParam.bMaintainBattleUIs = DialogEvent.bMaintainBattleUIs;
					return RetParam;
				}
			}
		}
	}

	return WaveDialogSettings_OutParam();
}

bool AB2MonsterSpawnPool::IsComponentEnabledLater(UActorComponent* InComp)
{
	const FMonsterSpawnPoolPerStageSettings* StageSetting = GetDesiredStageSetting(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	if (Cast<UPrimitiveComponent>(InComp) && InComp->GetOwner() == this && StageSetting)
	{
		for (int32 SI = 0; SI < StageSetting->WaveCompOnOffSettings.Num(); ++SI)
		{
			const FMonsterSpawnPoolWaveComponentOnOffSettings& ThisSetting = StageSetting->WaveCompOnOffSettings[SI];

			if (
				ThisSetting.bCheckForOnOrOff == true && ThisSetting.ComponentName == FName(*InComp->GetName()) // 켜지도록 되어 있는 것만.
				)
			{
				return true;
			}
		}
	}

	return false;
}

void AB2MonsterSpawnPool::SetStageManager(class AB2StageManager* InManager)
{
	CachedStageManager = InManager;

	CurrentClientStageId = GetCurrentClientStageId();
	CurrentDifficultyLevel = GetCurrentStageDifficulty();
}

int32 AB2MonsterSpawnPool::GetCurrentClientStageId()
{
	if (CurrentGameMode)
	{
		switch (CurrentGameMode->GetB2GameModeType())
		{
		case EB2GameMode::Raid:
		{
			if (auto* RaidGameMode = Cast<AB2RaidGameMode>(CurrentGameMode))
				return RaidGameMode->GetClientStageId();
		}
		break;
		case EB2GameMode::Tutorial:
		{
			if (auto* TutorialGameMode = Cast<AB2TutorialGameMode>(CurrentGameMode))
				return TutorialGameMode->GetClientStageId();
		}
		break;
		}
	}

	if (CachedStageManager)
		return CachedStageManager->GetCurrentClientStageId();

	return 1;
}

int32 AB2MonsterSpawnPool::GetCurrentStageDifficulty()
{
	if (CurrentGameMode)
	{
		switch (CurrentGameMode->GetB2GameModeType())
		{
			/*
					case EB2GameMode::Raid:
					{
						if (auto* RaidGameMode = Cast<AB2RaidGameMode>(CurrentGameMode))
							return RaidGameMode->GetStageDifficulty();
					}
					break;*/
		case EB2GameMode::Tutorial:
		{
			if (auto* TutorialGameMode = Cast<AB2TutorialGameMode>(CurrentGameMode))
				return (int32)TutorialGameMode->GetStageDifficulty();
		}
		break;
		}
	}

	if (CachedStageManager)
		return (int32)CachedStageManager->GetStageDifficultyLevel();

	return (int32)EStageDifficulty::ESD_Normal;
}

FMonsterSpawnPoolRuntimeWaveState* AB2MonsterSpawnPool::GetRuntimeWaveState(int32 WaveNum)
{
	if (WaveNum >= 0 && WaveNum < RuntimeWaveStates.Num())
	{
		return &RuntimeWaveStates[WaveNum];
	}
	return NULL;
}

void AB2MonsterSpawnPool::GetWaveMobList(int32 WaveNum, TArray<class ABladeIICharacter*>& OutLiveMobs, bool bOnlyLiveMobs /*= true*/)
{
	FMonsterSpawnPoolRuntimeWaveState* WaveState = GetRuntimeWaveState(WaveNum);

	if (WaveState)
	{
		for (int32 FI = 0; FI < WaveState->FellowsInThisWave.Num(); ++FI)
		{
			ABladeIICharacter* CurrMob = WaveState->FellowsInThisWave[FI];
			// bOnlyLiveMobs 가 false 인 경우라도 실제로 Dying 상태인 것을 가져올 수 있는 타이밍은 많지 않다.
			// OnSpawnedMonsterDead 참고.
			if (IsValid(CurrMob) && (bOnlyLiveMobs == false || CurrMob->IsAlive()))
			{
				OutLiveMobs.Add(WaveState->FellowsInThisWave[FI]);
			}
		}
	}
}

void AB2MonsterSpawnPool::GetAllWaveMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		TArray<ABladeIICharacter*> CurrWaveMobList;
		GetWaveMobList(WI, CurrWaveMobList, bOnlyLiveMobs);
		OutList.Append(CurrWaveMobList);
	}
}

void AB2MonsterSpawnPool::GetAllSummonedMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
	for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
	{
		ABladeIICharacter* ThisSummonee = SummonedFellows[SI].Summonee;

		if (IsValid(ThisSummonee) && (ThisSummonee->IsAlive() || bOnlyLiveMobs == false))
		{
			OutList.Add(ThisSummonee);
		}
	}
}

void AB2MonsterSpawnPool::SwapMonsterForPhaseSpawn(class ABladeIICharacter* OldOne, class ABladeIICharacter* NewOne)
{
	checkSlow(OldOne && NewOne);

	// 몹 리스트의 OldOne 에 해당하는 요소를 NewOne 으로 바꿔침.
	if (OldOne->IsSummonedMob())
	{
		// Summoner 든, Summonee 든 대상이 될 수 있겠다.
		for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
		{
			if (SummonedFellows[SI].Summonee == OldOne)
			{
				SummonedFellows[SI].Summonee = NewOne;
			}
			else if (SummonedFellows[SI].Summoner == OldOne)
			{
				SummonedFellows[SI].Summoner = NewOne;
			}
		}
	}
	else
	{
		const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
		for (int32 WI = 0; WI < StageWaveNum; ++WI)
		{
			FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];

			for (int32 MI = 0; MI < CurrState.FellowsInThisWave.Num(); ++MI)
			{
				if (CurrState.FellowsInThisWave[MI] == OldOne)
				{
					CurrState.FellowsInThisWave[MI] = NewOne;
					// 여기서 걍 break 걸어 버려도 되겠지만..
				}
			}
		}
	}

	// 추가로 NewOne 에 대한 SetBirthPlace 처리. StageManager 를 거치지 않는 직접 처리인데 필요하게 되면 교체..
	NewOne->SetBirthPlace(FMobSpawnedBirthplaceInfo(this, OldOne->GetSpawnPoolWaveNum(), OldOne->GetSpawnPoolWaveObjIndex(), OldOne->IsSpawnedAsBossMob(), OldOne->IsSummonedMob()));
}

int32 AB2MonsterSpawnPool::GetNumOfMobsStageLeftWave(int32 InClientStageId, EStageDifficulty DifficultyLevel, bool bIncludeSummoned)
{
	int32 Counter = 0;
	const int32 StageWaveNum = GetStageWaveNum(InClientStageId, DifficultyLevel);
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		const FMonsterSpawnPoolRuntimeWaveState& WaveState = RuntimeWaveStates[WI];
		if (WaveState.bWaveStarted == false)
			Counter += WaveState.FinalSpawnNum;
		else if (WaveState.bWaveCompleted == false)
		{
			for (int32 MobIdx = 0; MobIdx < WaveState.FellowsInThisWave.Num(); ++MobIdx)
			{
				if (WaveState.FellowsInThisWave[MobIdx] && WaveState.FellowsInThisWave[MobIdx]->IsAlive())
					Counter++;
			}
		}
	}

	if (bIncludeSummoned)
	{
		for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
		{
			if (SummonedFellows[SI].Summonee && SummonedFellows[SI].Summonee->IsAlive())
			{
				++Counter;
			}
		}
	}

	return Counter;
}

int32 AB2MonsterSpawnPool::GetLeftGoldWeight(int32 InClientStageId, EStageDifficulty DifficultyLevel)
{
	int32 WeightCounter = 0;
	const int32 StageWaveNum = GetStageWaveNum(InClientStageId, DifficultyLevel);
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		const FMonsterSpawnPoolWaveSettings* WaveSettings = GetStageWaveSetting(InClientStageId, DifficultyLevel, WI);
		if (WaveSettings)
		{
			const FMonsterSpawnPoolRuntimeWaveState& WaveState = RuntimeWaveStates[WI];
			if (WaveState.bWaveStarted == false)
			{//GetSpawnClassFromSetting()의 랜덤성에 의해 계산값은 매번 바뀔 수 있다. 예상치 용도라서 큰 문제는 없을 듯.
				for (int32 spawnIdx = 0; spawnIdx < WaveState.FinalSpawnNum; ++spawnIdx)
				{
					FSpawnClassFromSetting_OutParams OutParams;
					GetSpawnClassFromSetting(OutParams, WaveSettings, NULL, spawnIdx); //다른 쉬운 인터페이스는? -> 없습니다 고객님
					WeightCounter += OutParams.OutWeightRewardGold;
				}
			}
			else if (WaveState.bWaveCompleted == false)
			{
				for (int32 MobIdx = 0; MobIdx < WaveState.FellowsInThisWave.Num(); ++MobIdx)
				{
					if (WaveState.FellowsInThisWave[MobIdx] && WaveState.FellowsInThisWave[MobIdx]->IsAlive())
					{
						WeightCounter += WaveState.FellowsInThisWave[MobIdx]->GetWeightRewardGold();
					}
				}
			}
		}
	}

	return WeightCounter;
}

#if !UE_BUILD_SHIPPING
void AB2MonsterSpawnPool::Cheat_FinishUpToCertainWave(int32 DesiredFinishWaveNum)
{
	if (DesiredFinishWaveNum < 0)
	{
		return; // ForceClearWaves 와는 다름.
	}

	// DesiredFinishWaveNum 까지 시작하지 않은 wave 가 있다면 시작.
	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		if (WI > DesiredFinishWaveNum)
		{
			continue;
		}

		FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];

		if (CurrState.bWaveStarted == false)
		{
			SpawnWave(WI);
		}
	}

	// ForceClearWaves 에서 그냥 죽이기만 하고 여타 처리들은 CheckWaveCompletion 을 통해서..
	ForceClearWaves(DesiredFinishWaveNum, false);
	bForceClearing = false; // 으이구.. 에디터 기능이니 이 허접함을 넘긴다..

	CheckWaveCompletion(); // 걍 이거 한번 불러주면 OnSingleWaveComplete 처리도 될 듯.
}

void AB2MonsterSpawnPool::ProceedOnEventIgnoringCheat()
{ // 들어오는 상황은 이벤트 없이 애들을 따로 죽인 다음 wave 끝났는지 체크하기 위함.
	// OnSpawnedMonsterDead 의 필요 처리를 직접.
	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel);
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];
		if (CurrState.bWaveStarted)
		{
			for (int32 FI = 0; FI < CurrState.FellowsInThisWave.Num(); ++FI)
			{
				ABladeIICharacter* ThisFellow = CurrState.FellowsInThisWave[FI];
				if (ThisFellow && !ThisFellow->IsAlive())
				{
					CurrState.FellowsInThisWave.RemoveAt(FI);
					--FI;
				}
			}
		}
	}

	SummonedFellows.Empty(); // 소환 리스트는 걍 모두 비움.

	CheckWaveCompletion();
}

#endif

#if WITH_EDITOR && WITH_EDITORONLY_DATA

void AB2MonsterSpawnPool::RegenerateEditingActors()
{
	UPackage* OutermostPkg = GetOutermost();
	// 맵인 경우만 생성해야 함. 블루프린트로 저장한 애도 처음엔 맵으로 인식될 것.
	if (OutermostPkg == NULL || FEditorFileUtils::IsMapPackageAsset(OutermostPkg->GetName()) == false || !AmICurrentEditorActive(this))
	{
		DestroyAllEditingActors();
		return;
	}

	for (int32 EI = 0; EI < WaveEditingActors.Num(); ++EI)
	{
		if (WaveEditingActors[EI])
		{
			// Ensure to cache the owner pool.
			WaveEditingActors[EI]->SetOwnerPool(this);
		}
		else
		{
			WaveEditingActors.RemoveAt(EI);
			--EI;
		}
	}

	ABladeIIWorldSettings* B2WS = Cast<ABladeIIWorldSettings>(GetWorldSettings());
	const int32 EditorStageNum = B2WS ? B2WS->GetEditorStageNumber() : 1;
	const EStageDifficulty EditorDifficultyLevel = B2WS ? (EStageDifficulty)B2WS->GetEditorStageDifficulty() : EStageDifficulty::ESD_Normal;

	// Create WaveEditingInfo if not create yet, according to the wave number
	const int32 CurrStageWaveNum = GetStageWaveNum(EditorStageNum, EditorDifficultyLevel);
	if (CurrStageWaveNum == 0)
	{
		DestroyAllEditingActors();

		// We might generate at least one anyway for ComponentNameDisplay, but it seems like tricky for some reason..
		// By just not allowing EditingActor at the first place, we can avoid some trouble.
		//AB2SpawnPoolEditingActor* NewEditingActor = CreateSingleWaveEditingActor(EditorStageNum, EditorDifficultyLevel, 0);
		//if(NewEditingActor)
		//{
		//	WaveEditingActors.Add(NewEditingActor);
		//}
	}
	else if (WaveEditingActors.Num() > CurrStageWaveNum)
	{
		// Remove unnecessary
		for (int32 EI = CurrStageWaveNum; EI < WaveEditingActors.Num(); ++EI)
		{
			if (WaveEditingActors[EI])
			{
				WaveEditingActors[EI]->Destroy();
			}
		}

		WaveEditingActors.RemoveAt(CurrStageWaveNum, WaveEditingActors.Num() - CurrStageWaveNum);
	}
	else if (WaveEditingActors.Num() < CurrStageWaveNum)
	{
		const int32 NumBefore = WaveEditingActors.Num();
		// Add more
		for (int32 EI = NumBefore; EI < CurrStageWaveNum; ++EI)
		{
			AB2SpawnPoolEditingActor* NewEditingActor = CreateSingleWaveEditingActor(EditorStageNum, EditorDifficultyLevel, EI);
			if (NewEditingActor)
			{
				WaveEditingActors.Add(NewEditingActor);
			}
		}
	}

	// Double check for GIsEditor
	if (GIsEditor)
	{
		const int32 StageWaveNum = GetStageWaveNum(EditorStageNum, EditorDifficultyLevel);
		for (int32 WI = 0; WI < StageWaveNum; ++WI)
		{
			AB2SpawnPoolEditingActor* CurrEditingActor = (WI < WaveEditingActors.Num()) ? WaveEditingActors[WI] : NULL;
			const FMonsterSpawnPoolWaveSettings* CurrSetting = GetStageWaveSetting(EditorStageNum, EditorDifficultyLevel, WI);

			if (CurrEditingActor && CurrSetting)
			{
				// Relocating the editing actor is not that necessary, but for the sake of cleanness..
				// After the root component gets relocated, other child representing sk components will get their location below by SetRepresentingInfo.
				// SpawnAreaComp 는 ShapeComponent
				if (UShapeComponent* SpawnAreaComp = Cast<UShapeComponent>(FindComponentByName(CurrSetting->SpawnAreaComponentName)))
				{
					CurrEditingActor->SetActorLocation(SpawnAreaComp->GetComponentLocation());
				}
				else
				{
					CurrEditingActor->SetActorLocation(GetActorLocation());
				}

				// Before we go on, remove any unnecessary editing info components..
				CurrEditingActor->RemoveEditingInfo_AllButOneWave(WI);
				CurrEditingActor->RemoveEditingInfo_UnnecessaryObj(WI, CurrSetting->SpawnNum);

				for (int32 AI = 0; AI < CurrSetting->SpawnNum; ++AI)
				{
					// To get the type information for current spawn object
					// Those are from old code, we probably don't need them anymore.
					ECharacterType TypeForThisObject = ECharacterType::ECT_MeleeMob;
					bool bSpecifiedClassForThisObject = false;

					FSpawnClassFromSetting_OutParams SpawnClassParam;
					GetSpawnClassFromSetting(SpawnClassParam, CurrSetting, NULL, AI);

					// Either ClassEnum or ClassBP should be valid,
					// but let it proceed to call SetRepresentingInfo anyway. It will display some error info.
					//if(SpawnClassParam.OutClassEnum != ENPCClass::ENC_End || SpawnClassParam.OutClassBP != NULL) 
					{
						FTransform SpawnTransform = GetSpawnTransform(CurrSetting, AI);

						// Add or update representing info (components) to the EditingActor
						CurrEditingActor->SetRepresentingInfo(EditorStageNum, EditorDifficultyLevel, WI, AI, SpawnClassParam.OutClassEnum, SpawnClassParam.OutClassVariation, SpawnClassParam.OutClassBP,
							CurrSetting->bRandomSpawnLocation, SpawnTransform, SpawnClassParam.MaterialOverrides);
					}
				}
			}
		}
	}
}

void AB2MonsterSpawnPool::DestroyAllEditingActors()
{
	for (int32 EI = 0; EI < WaveEditingActors.Num(); ++EI)
	{
		if (WaveEditingActors[EI])
		{
			WaveEditingActors[EI]->Destroy();
		}
	}
	WaveEditingActors.Empty();
}

class AB2SpawnPoolEditingActor* AB2MonsterSpawnPool::CreateSingleWaveEditingActor(int32 InClientStageId, EStageDifficulty DifficultyLevel, int32 WaveNum)
{
	if (GetWorld())
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.bDeferConstruction = false;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnTransform(GetActorLocation());

		// We might allow to create EditingActor for invalid WaveNum index. See RegenerateEditingActors, there could be a chance for that use case.
		// Right now, not allowing it.
		const FMonsterSpawnPoolWaveSettings* CurrSetting = GetStageWaveSetting(InClientStageId, DifficultyLevel, WaveNum);
		if (CurrSetting)
		{
			if (CurrSetting->SpawnAreaComponentName != NAME_None)
			{
				// SpawnAreaComp 는 ShapeComponent
				UShapeComponent* SpawnAreaComp = Cast<UShapeComponent>(FindComponentByName(CurrSetting->SpawnAreaComponentName));
				if (SpawnAreaComp)
				{
					SpawnTransform = FTransform(SpawnAreaComp->GetComponentLocation());
				}
			}

			AB2SpawnPoolEditingActor* NewCreated = Cast<AB2SpawnPoolEditingActor>(GetWorld()->SpawnActor(AB2SpawnPoolEditingActor::StaticClass(), &SpawnTransform, SpawnInfo));

			if (NewCreated)
			{
				NewCreated->SetOwnerPool(this); // this must be the Owner anyway..
				return NewCreated;
			}
		}
	}
	return NULL;
}

void AB2MonsterSpawnPool::EnsureHaveFixedSpawnTransforms()
{
	// Let it iterate for all stage settings.

	for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
	{
		for (int32 WI = 0; WI < PerStageSettings[SI].WaveSettings.Num(); ++WI)
		{
			FMonsterSpawnPoolWaveSettings& CurrWaveSetting = PerStageSettings[SI].WaveSettings[WI];

			if (CurrWaveSetting.bRandomSpawnLocation == true)
			{
				// We might leave the location values which probably edited before, but I would not like to allow unnecessary memory consumption.
				CurrWaveSetting.FixedSpawnTransforms.Empty();
			}
			else
			{
				// Make it to have the same number of elements.
				if (CurrWaveSetting.FixedSpawnTransforms.Num() < CurrWaveSetting.SpawnNum)
				{
					for (int32 SLI = CurrWaveSetting.FixedSpawnTransforms.Num(); SLI < CurrWaveSetting.SpawnNum; ++SLI)
					{
						// GetSpawnTransform will return random location if not previously set.
						CurrWaveSetting.FixedSpawnTransforms.Add(GetSpawnTransform(&CurrWaveSetting, SLI));
					}
				}
				else if (CurrWaveSetting.FixedSpawnTransforms.Num() > CurrWaveSetting.SpawnNum)
				{
					CurrWaveSetting.FixedSpawnTransforms.RemoveAt(
						CurrWaveSetting.SpawnNum,
						CurrWaveSetting.FixedSpawnTransforms.Num() - CurrWaveSetting.SpawnNum
					);
				}
			}
		}
	}
}

void AB2MonsterSpawnPool::ExplictSetSpawnTransform(int32 InClientStageId, EStageDifficulty DifficultyLevel, int32 WaveSettingNum, int32 ObjNum, FVector InLocation, FRotator InRotation)
{
	FMonsterSpawnPoolWaveSettings* WaveSetting = const_cast<FMonsterSpawnPoolWaveSettings*>(GetStageWaveSetting(InClientStageId, DifficultyLevel, WaveSettingNum));

	if (WaveSetting)
	{
		if (ObjNum >= 0 && ObjNum < WaveSetting->FixedSpawnTransforms.Num())
		{
			WaveSetting->FixedSpawnTransforms[ObjNum] = FTransform(InRotation, InLocation);
		}
	}
}

void AB2MonsterSpawnPool::RegenerateComponentNameDisplay()
{
	// Use the first element of WaveEditingActors array for ComponentNameDisplay. Cannot separate the ComponentNameDisplay according to the WaveNum index.
	// Therefore, no ComponentNameDisplay when there is no wave setting.
	if (GIsEditor && WaveEditingActors.Num() && WaveEditingActors[0] != NULL) // Double check for GIsEditor.
	{
		TArray<UActorComponent*> AllComps;
		GetComponents(AllComps);

		// Before we go on, remove previously created text render components..
		// For this, it is a little bit harder to remove only unnecessary stuff, so just remove all and recreate..
		for (int32 EI = 0; EI < WaveEditingActors.Num(); ++EI)
		{
			if (WaveEditingActors[EI])
			{
				WaveEditingActors[EI]->RemoveAllComponentDisplay();
			}
		}

		if (AmICurrentEditorActive(this))
		{
			// Generate name display actor for all primitive component.
			for (int32 CI = 0; CI < AllComps.Num(); ++CI)
			{
				UPrimitiveComponent* CurrComp = Cast<UPrimitiveComponent>(AllComps[CI]);

				if (Cast<UBillboardComponent>(CurrComp))
				{
					continue; // 에디터 아이콘 스프라이트 이름 뜨는 거 필요도 없고 괜히 거슬림.
				}

				WaveEditingActors[0]->SetComponentToDisplayInfo(CurrComp);
			}
		}
	}
}

bool AB2MonsterSpawnPool::EditorGetSpawnClassOfWaveMob(ENPCClass& OutClassEnum, ENPCClassVariation& OutClassVariation, TSubclassOf<class ABladeIICharacter>& OutClassBP, int32 InWaveNum, int32 InObjIndex)
{
	// 이건 사실상 에디터 용도라 EditorStageNumber 를 사용
	ABladeIIWorldSettings* B2WS = Cast<ABladeIIWorldSettings>(GetWorldSettings());
	const int32 EditorStageNum = B2WS ? B2WS->GetEditorStageNumber() : 1;
	const EStageDifficulty EditorDifficultyLevel = B2WS ? (EStageDifficulty)B2WS->GetEditorStageDifficulty() : EStageDifficulty::ESD_Normal;

	const FMonsterSpawnPoolWaveSettings* WaveSetting = GetStageWaveSetting(EditorStageNum, EditorDifficultyLevel, InWaveNum);

	if (WaveSetting)
	{
		ECharacterType LocalCharacterType;
		FSpawnClassFromSetting_OutParams SpawnClassParam(OutClassEnum, OutClassVariation, OutClassBP, LocalCharacterType);

		if (GetSpawnClassFromSetting(SpawnClassParam, WaveSetting, NULL, InObjIndex))
		{
			if (SpawnClassParam.OutClassEnum != ENPCClass::ENC_End || SpawnClassParam.OutClassBP != NULL) // 위에서 true 리턴이면 이래야 하겠지만 한번 더 체크
			{
				OutClassEnum = SpawnClassParam.OutClassEnum;
				OutClassVariation = SpawnClassParam.OutClassVariation;
				OutClassBP = SpawnClassParam.OutClassBP;
				return true;
			}
		}
	}

	return false;
}

void AB2MonsterSpawnPool::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_PerStageSettings = GET_MEMBER_NAME_CHECKED(AB2MonsterSpawnPool, PerStageSettings);
	const FName Name_RandomSpawnLocation = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolWaveSettings, bRandomSpawnLocation);
	const FName Name_SpawnNum = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolWaveSettings, SpawnNum);
	const FName Name_MinSpawnObjIndex = GET_MEMBER_NAME_CHECKED(FMonsterSpawnTypeSpecific, MinSpawnObjIndex);
	const FName Name_MaxSpawnObjIndex = GET_MEMBER_NAME_CHECKED(FMonsterSpawnTypeSpecific, MaxSpawnObjIndex);
	const FName Name_SpawnNumRandomSlack = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolWaveSettings, SpawnNumRandomSlack);
	const FName Name_TerminateAll = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolWaveSettings, WaveCompl_TerminateAll);
	const FName Name_DisableAllSpawn = GET_MEMBER_NAME_CHECKED(AB2MonsterSpawnPool, bDisableAllSpawn);
	const FName Name_TargetStageNum = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolPerStageSettings, TargetStageNum);
	const FName Name_CompOnOffComponentName = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolWaveComponentOnOffSettings, ComponentName);
	const FName Name_CompOnOffCheckForOnOff = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolWaveComponentOnOffSettings, bCheckForOnOrOff);
	const FName Name_BossWave = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolWaveSettings, bBossWave);
	const FName Name_TargetDifficultyLevel = GET_MEMBER_NAME_CHECKED(FMonsterSpawnPoolPerStageSettings, TargetDifficultyLevel);
	const FName Name_ClassVariaion = GET_MEMBER_NAME_CHECKED(FMonsterSpawnClassInfo, ClassVariation);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == Name_RandomSpawnLocation || PropertyName == Name_SpawnNum)
	{
		// Don't have to call this here as long as we call it at OnConstruction.
		//EnsureHaveFixedSpawnTransforms();
	}

	if (PropertyName == Name_MinSpawnObjIndex || PropertyName == Name_MaxSpawnObjIndex)
	{
		// Simply let MinSpawnObjIndex and MaxSpawnObjIndex over the array bound is not really harmful,
		// but just to help setting proper value..
		for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
		{
			for (int32 WI = 0; WI < PerStageSettings[SI].WaveSettings.Num(); ++WI)
			{
				FMonsterSpawnPoolWaveSettings& CurrWaveSetting = PerStageSettings[SI].WaveSettings[WI];

				for (int32 STI = 0; STI < CurrWaveSetting.SpawnCharacterTypes.Num(); ++STI)
				{
					FMonsterSpawnTypeSpecific& CurrSTInfo = CurrWaveSetting.SpawnCharacterTypes[STI];
					CurrSTInfo.MinSpawnObjIndex = FMath::Min(CurrSTInfo.MinSpawnObjIndex, CurrWaveSetting.SpawnNum - 1);
					CurrSTInfo.MaxSpawnObjIndex = FMath::Min(CurrSTInfo.MaxSpawnObjIndex, CurrWaveSetting.SpawnNum - 1);
				}
			}
		}
	}

	if (PropertyName == Name_SpawnNumRandomSlack)
	{
		for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
		{
			for (int32 WI = 0; WI < PerStageSettings[SI].WaveSettings.Num(); ++WI)
			{
				FMonsterSpawnPoolWaveSettings& CurrWaveSetting = PerStageSettings[SI].WaveSettings[WI];
				CurrWaveSetting.SpawnNumRandomSlack = FMath::Clamp(CurrWaveSetting.SpawnNumRandomSlack, 0, CurrWaveSetting.SpawnNum);
			}
		}
	}

	if (PropertyName == Name_TerminateAll)
	{
		bool bAtLeastOneTerminateAll = false;
		for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
		{
			for (int32 WI = 0; WI < PerStageSettings[SI].WaveSettings.Num(); ++WI)
			{
				FMonsterSpawnPoolWaveSettings& CurrWaveSetting = PerStageSettings[SI].WaveSettings[WI];
				if (CurrWaveSetting.WaveCompl_TerminateAll == true)
				{
					bAtLeastOneTerminateAll = true;
					break; // Does not break from outer loop, but no matter how..
				}
			}
		}
		if (bAtLeastOneTerminateAll)
		{
#if !PLATFORM_MAC
			// Just a note or warning according to the current implementation limit of ForceClearWaves.
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("[B2MonsterSpawnPool] [NOTE] 주의사항:\nWaveCompl_TerminateAll 에 따라 다른 wave 가 강제 종료될 때에는 wave 종료에 따른 이벤트 호출이 생략됩니다.")));
#endif
		}
	}

	if (PropertyName == Name_DisableAllSpawn)
	{
		if (bDisableAllSpawn == true)
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("[B2MonsterSpawnPool] DisableAllSpawn 은 테스트 용도로 제공됩니다. 최종 배포판 빌드에서는 동작하지 않을 것입니다.")));
#endif
		}
	}

	if (/*PropertyName == Name_PerStageSettings || */PropertyName == Name_TargetStageNum || PropertyName == Name_TargetDifficultyLevel)
	{
		// 나를 제외한 SpawnPool 리스트 수집. 다른 애들것도 체크해야.
		UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : NULL;
		TArray<AB2MonsterSpawnPool*> AllSPExceptMe;
		for (FActorIterator ActorIt(EditorWorld); ActorIt; ++ActorIt)
		{
			AB2MonsterSpawnPool* MSP = Cast<AB2MonsterSpawnPool>(*ActorIt);
			if (MSP && MSP != this)
			{
				AllSPExceptMe.Add(MSP);
			}
		}

#if !PLATFORM_MAC
		int32 AllFoundDuplicatedNum = 0;
		FString WarnMsgStr(TEXT("[Warning] 중복된 TargetStageNum / DifficultyLevel 세팅:\n\n"));
		for (int32 SIA = 0; SIA < PerStageSettings.Num(); ++SIA)
		{
			int32 CurrTargetStageNum = PerStageSettings[SIA].TargetStageNum;
			int32 CurrDifficultyLevel = PerStageSettings[SIA].TargetDifficultyLevel;

			for (int32 SIB = SIA + 1; SIB < PerStageSettings.Num(); ++SIB)
			{
				if (CurrTargetStageNum == PerStageSettings[SIB].TargetStageNum && CurrDifficultyLevel == PerStageSettings[SIB].TargetDifficultyLevel)
				{
					WarnMsgStr += FString::Printf(TEXT("SpawnPool %s, 인덱스 %d and %d, TargetStage %d, TargetDifficulty %d\n"), *GetName(), SIA, SIB, CurrTargetStageNum, (int32)CurrDifficultyLevel);
					++AllFoundDuplicatedNum;
				}
			}

			// 그리고 다른 SpawnPool 에서도..
			for (AB2MonsterSpawnPool* OtherOne : AllSPExceptMe)
			{
				for (int32 SIB = 0; SIB < OtherOne->PerStageSettings.Num(); ++SIB)
				{
					if (CurrTargetStageNum == OtherOne->PerStageSettings[SIB].TargetStageNum && CurrDifficultyLevel == OtherOne->PerStageSettings[SIB].TargetDifficultyLevel)
					{
						WarnMsgStr += FString::Printf(TEXT("SpawnPool %s, 인덱스 %d and %d, TargetStage %d, TargetDifficulty %d\n"), *OtherOne->GetName(), SIA, SIB, CurrTargetStageNum, (int32)CurrDifficultyLevel);
						++AllFoundDuplicatedNum;
					}
				}
			}
		}

		if (AllFoundDuplicatedNum > 0)
		{
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMsgStr));
		}
#endif
	}

	if (PropertyName == Name_ClassVariaion)
	{
#if !PLATFORM_MAC
		int32 AllFoundNotAllowedVariation = 0;
		FString WarnMsgStr(TEXT("[Warning] 허용되지 않는 ClassVariation 세팅:\r\n\r\n"));
		for (int32 SIA = 0; SIA < PerStageSettings.Num(); ++SIA)
		{
			for (int32 WI = 0; WI < PerStageSettings[SIA].WaveSettings.Num(); ++WI)
			{
				FMonsterSpawnPoolWaveSettings& ThisWaveSetting = PerStageSettings[SIA].WaveSettings[WI];

				for (int32 SCI = 0; SCI < ThisWaveSetting.SpawnClasses.Num(); ++SCI)
				{
					FMonsterSpawnClassInfo& ThisSpawnClassinfo = ThisWaveSetting.SpawnClasses[SCI];
					if (ThisSpawnClassinfo.ClassVariation == ENPCClassVariation::ENCV_CounterDungeon)
					{
						++AllFoundNotAllowedVariation;
						WarnMsgStr += FString::Printf(TEXT("%d. TargetStage: %d, Difficulty: %d, Wave %d, ClassVariation ENCV_CounterDungeon\r\n"),
							AllFoundNotAllowedVariation,
							PerStageSettings[SIA].TargetStageNum,
							PerStageSettings[SIA].TargetDifficultyLevel,
							WI);
					}

					// 일단 CounterDungeon 넣지 않게 하려고 한 건데 기타 다른 걸 추가할 수도..
				}
			}
		}

		if (AllFoundNotAllowedVariation > 0)
		{
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMsgStr));
		}
#endif

	}

	if (PropertyName == Name_CompOnOffComponentName || PropertyName == Name_CompOnOffCheckForOnOff)
	{
#if !PLATFORM_MAC
		// Wave 시작/종료 시 컴포넌트 Enable 시키는 설정은 좀 조심해야 함. 잘못하면 길이 막히기 때문. 이에 따른 경고

		TArray<FString> UndesiredBeginEnableCompsSetting;
		TArray<FString> UndesiredComplEnableCompsSetting;

		for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
		{
			for (int32 WI = 0; WI < PerStageSettings[SI].WaveCompOnOffSettings.Num(); ++WI)
			{
				FMonsterSpawnPoolWaveComponentOnOffSettings& CurrCompSetting = PerStageSettings[SI].WaveCompOnOffSettings[WI];

				FMonsterSpawnPoolWaveSettings* TargetWaveSetting = (CurrCompSetting.WaveNum >= 0 && CurrCompSetting.WaveNum < PerStageSettings[SI].WaveSettings.Num()) ? &(PerStageSettings[SI].WaveSettings[CurrCompSetting.WaveNum]) : NULL;

				UMeshComponent* MeshComp = Cast<UMeshComponent>(FindComponentByName(CurrCompSetting.ComponentName));

				// Wave 시작시에 MeshComponent 를 켜는 설정이라면 WaveExec_TriggerComponent 하고만 사용되는 것이 바람직하다. 그래야 wave 시작 시 위치를 강제할 수 있으므로.
				if (CurrCompSetting.EventTiming == EWaveCustomEventTiming::EWCET_WaveStart && CurrCompSetting.bCheckForOnOrOff == true && MeshComp && TargetWaveSetting &&
					(TargetWaveSetting->WaveExec_TriggerComponent == NAME_None || TargetWaveSetting->WaveExec_PostCompleteWaveNum >= 0)
					)
				{
					UndesiredBeginEnableCompsSetting.Add(FString::Printf(TEXT("Stage %d, WaveComp %d"), SI, WI));
				}

				// 어떤 경우든 wave 종료 시에 MeshComponent 를 켜도록 지정하는 게 바람직하지 않다. 어디서 wave 가 끝날지 모르므로.
				if (CurrCompSetting.EventTiming == EWaveCustomEventTiming::EWCET_WaveComplete && CurrCompSetting.bCheckForOnOrOff == true && MeshComp)
				{
					UndesiredComplEnableCompsSetting.Add(FString::Printf(TEXT("Stage %d, WaveComp %d"), SI, WI));
				}
			}
		}

		if (UndesiredBeginEnableCompsSetting.Num() > 0)
		{
			//FString WarnMessage = TEXT("WaveCompOnOffSettings 에 잘못된 플레이를 유발할 수 있는 설정이 있습니다. 아래 인덱스의 설정(들)을 확인해 주세요.\n\n");
			//for (int32 CI = 0; CI < UndesiredBeginEnableCompsSetting.Num(); ++CI)
			//{
			//	WarnMessage += UndesiredBeginEnableCompsSetting[CI] + TEXT("\n");
			//}
			//WarnMessage += FString(TEXT("\nWave 시작 시 MeshComponent 를 enable 시키는 설정에서는 WaveExec_TriggerComponent 만 사용하는 것이 좋습니다."));
			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage));
		}
		if (UndesiredComplEnableCompsSetting.Num() > 0)
		{
			//FString WarnMessage = TEXT("WaveCompOnOffSettings 에 잘못된 플레이를 유발할 수 있는 설정이 있습니다. 아래 인덱스의 설정(들)을 확인해 주세요.\n\n");
			//for (int32 CI = 0; CI < UndesiredComplEnableCompsSetting.Num(); ++CI)
			//{
			//	WarnMessage += UndesiredComplEnableCompsSetting[CI] + TEXT("\n");
			//}
			//WarnMessage += FString(TEXT("\nWave 종료 시 MeshComponent 를 enable 시키는 것은 플레이 루트를 막을 가능성이 있습니다."));
			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage));
		}
#endif
	}

	if (PropertyName == Name_SpawnNum || PropertyName == Name_BossWave)
	{
#if !PLATFORM_MAC
		FString BossWaveWarnString = TEXT("BossWave 설정을 한 wave 에는 단 하나의 몹만을 spawn 시켜야 합니다. 다음 wave 세팅들을 확인하세요.\n");
		bool bNeedWarning = false;
		for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
		{
			for (int32 WI = 0; WI < PerStageSettings[SI].WaveSettings.Num(); ++WI)
			{
				FMonsterSpawnPoolWaveSettings& CurrWaveSetting = PerStageSettings[SI].WaveSettings[WI];
				// BossWave 에 2마리 이상 spawn 하면 개곤란. 왜냐면 BossHUD 가 중첩되어 뜰 것이기 때문.
				if (CurrWaveSetting.bBossWave && CurrWaveSetting.SpawnNum > 1)
				{
					bNeedWarning = true;
					BossWaveWarnString += FString::Printf(TEXT(" - TargetStage : %d / Difficulty : %d / Wave : %d\n"), PerStageSettings[SI].TargetStageNum, PerStageSettings[SI].TargetDifficultyLevel, WI);
				}
			}
		}
		if (bNeedWarning)
		{
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(BossWaveWarnString));
		}
#endif
	}

	if (PropertyName == Name_TargetDifficultyLevel)
	{
		for (FMonsterSpawnPoolPerStageSettings& ThisSetting : PerStageSettings)
		{
			ThisSetting.TargetDifficultyLevel = FMath::Clamp(ThisSetting.TargetDifficultyLevel, (int32)EStageDifficulty::ESD_Normal, (int32)EStageDifficulty::ESD_Hell);
		}
	}

	// Do this in any case.. -> Don't have to call this here as long as we call it at OnConstruction.
	//RegenerateEditingActors();
}

void AB2MonsterSpawnPool::PostEditUndo()
{
	Super::PostEditUndo();

	// Not likely need to call this here as long as we call it at OnConstruction.
	//RegenerateEditingActors();
}
void AB2MonsterSpawnPool::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		// In this case, components in editing actor will be just re-located.
		RegenerateEditingActors();
		// NameDisplays are regenerated anyhow.
		RegenerateComponentNameDisplay();
	}
}
#endif

void ParseForWaveMobSelectKeyword(FString InKeyword, int32& OutWaveNum, int32& OutMobIndex)
{
	OutWaveNum = -1;
	OutMobIndex = 0; // MobIndex 는 지정하지 않을 경우 기본 0번으로 하도록 해 보자..

	const TCHAR* KeywordStr = *InKeyword;
	FString NextToken;
	if (FParse::Token(KeywordStr, NextToken, false))
	{
		OutWaveNum = FCString::Atoi(*NextToken);

		if (FParse::Token(KeywordStr, NextToken, false))
		{
			OutMobIndex = FCString::Atoi(*NextToken);
		}
	}
}

AB2MonsterSpawnPool* GetDesiredSpawnPoolOfSettingFromList(TArray<AB2MonsterSpawnPool*>& InSPs, int32 InTargetStageNum, EStageDifficulty InTargetDifficulty, bool bFallBackToLowerDifficulty)
{ // 월드에 배치된 여러개의 SpawnPool 중에서 원하는 스테이지에 필요한 세팅을 담고 있는 하나를 찾는 데에 사용됨.
	for (AB2MonsterSpawnPool* ThisSP : InSPs)
	{
		if (ThisSP && ThisSP->HasSetupFor(InTargetStageNum, InTargetDifficulty)) // 일단 정확히 일치하는 세팅이 있는 걸 찾는다.
		{
			return ThisSP;
		}
	}

	if (bFallBackToLowerDifficulty && InTargetDifficulty > EStageDifficulty::ESD_Normal)
	{
		// 높은 난이도로는 폴백 못하고 한 단계씩 낮은 난이도로 테스트. (AB2MonsterSpawnPool::GetDesiredStageSetting)
		EStageDifficulty FallbackTestDifficulty = GetOneEasierStageDifficulty(InTargetDifficulty);

		while (true)
		{
			bool bFoundFallback = false;
			for (AB2MonsterSpawnPool* ThisSP : InSPs)
			{
				if (ThisSP && ThisSP->HasSetupFor(InTargetStageNum, FallbackTestDifficulty))
				{
					return ThisSP;
				}
			}

			if (FallbackTestDifficulty <= EStageDifficulty::ESD_Normal)
			{
				break; // 이 이하는 없음.
			}

			FallbackTestDifficulty = GetOneEasierStageDifficulty(FallbackTestDifficulty);
		}
	}
	// 추가로 여기로 왔을 때 처음 것을 리턴하는 옵션도 생각해 볼 만하다.
	return NULL;
}

void SetForcedPerStageSettings(TArray<AB2MonsterSpawnPool*>& InSPs, int32 InClientStageNum, int32 InTargetDifficulty)
{
	for (AB2MonsterSpawnPool* ThisSP : InSPs)
	{
		if (ThisSP)
		{
			ThisSP->SetForcedPerStageSettings(InClientStageNum, InTargetDifficulty);
		}
	}
}

#if WITH_EDITOR
AB2MonsterSpawnPool* EditorGetCurrentActiveSpawnPool(bool bFallBackToLowerDifficulty)
{
	if (GIsEditor == false) // 인게임에선 다른 루트를 통하도록. 대체로 GameMode 의 GetActiveSpawnPool
	{
		return NULL;
	}

	UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : NULL;
	ABladeIIWorldSettings* B2WS = EditorWorld ? Cast<ABladeIIWorldSettings>(EditorWorld->GetWorldSettings()) : NULL;

	if (EditorWorld && B2WS)
	{
		// 일단 리스트부터 수집
		TArray<AB2MonsterSpawnPool*> AllFoundSP;
		for (FActorIterator ActorIt(EditorWorld); ActorIt; ++ActorIt)
		{
			AB2MonsterSpawnPool* MSP = Cast<AB2MonsterSpawnPool>(*ActorIt);
			if (MSP)
			{
				AllFoundSP.Add(MSP);
			}
		}
		return GetDesiredSpawnPoolOfSettingFromList(AllFoundSP, B2WS->GetEditorStageNumber(), static_cast<EStageDifficulty>(B2WS->GetEditorStageDifficulty()), bFallBackToLowerDifficulty);
	}
	return NULL;
}


#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
// For SpawnPoolEditingActor below..
////////////////////////////////////////////////////////////////////////////////////////////////////


UB2SpawnPoolEditingSKComponent::UB2SpawnPoolEditingSKComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR

void UB2SpawnPoolEditingSKComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// EditingActor 내 단일 컴포넌트 움직인다고 EditingActor 쪽으로 신호가 가지 않으므로 이렇게 신호를 보내준다
	// 근데 이것도 어차피 이동할 땐 안불리고.. 뾰족한 수가 없군. Modify 를 사용할 수도 있는데 그건 좀 위험한 거 같다.
	if (PropertyChangedEvent.Property &&
		(PropertyChangedEvent.Property->GetName().Contains(TEXT("Location")) || PropertyChangedEvent.Property->GetName().Contains(TEXT("Rotation")))
		)
	{
		AB2SpawnPoolEditingActor* OwnerEditingActor = Cast<AB2SpawnPoolEditingActor>(GetOwner());
		if (OwnerEditingActor)
		{
			OwnerEditingActor->SyncEditingActorLocationToSpawnPool();
		}
	}
}
#endif

float AB2SpawnPoolEditingActor::MeshSizeLimit = 200.0f;

AB2SpawnPoolEditingActor::AB2SpawnPoolEditingActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	NetUpdateFrequency = 10.0f;
	SetReplicateMovement(false);
	SetCanBeDamaged(false);

	// Just being a root to attach the dynamically loaded skeletalmeshcomponents.
	UBoxComponent* SimpleRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SimpleRootComponent"), true); // To be transient
	SetRootComponent(SimpleRootComponent);
	SimpleRootComponent->InitBoxExtent(FVector(10.0f, 10.0f, 10.0f));
	SimpleRootComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

#if WITH_EDITOR
	if (HasAnyFlags(RF_ClassDefaultObject) && GConfig) // 한번만 실행하도록.
	{
		GConfig->GetFloat(TEXT("MonsterSpawnPool"), TEXT("EditingActorSizeLimit"), MeshSizeLimit, GEditorIni);
	}
#endif

	CurrEditingClientStageId = 1;
	CurrEditingDifficultyLevel = EStageDifficulty::ESD_Normal;

	OwnerPool = NULL;
}

//#include "Components/SkeletalMeshComponent"




void AB2SpawnPoolEditingActor::BeginDestroy()
{
	Super::BeginDestroy();

	// Need to manually detach and remove them..?
	for (int TI = 0; TI < FakeSKCompArray.Num(); ++TI)
	{
		USkeletalMeshComponent* CurrComp = FakeSKCompArray[TI];
		if (CurrComp)
		{
			CurrComp->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
			CurrComp->UnregisterComponent();
			CurrComp->ConditionalBeginDestroy();
		}
	}
	FakeSKCompArray.Empty();
}

void AB2SpawnPoolEditingActor::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
#if WITH_EDITOR // Guess calling this only at PreSave could be enough? -> No.. better handle it when it is moved too..
	SyncEditingActorLocationToSpawnPool();
#endif
}

#if WITH_EDITOR

FString AB2SpawnPoolEditingActor::GetEditingSKMeshComponentName(int32 WaveNum, int32 SpawnObjNum)
{
	return FString::Printf(TEXT("EditingSkeletalMeshComponent_%d_%d"), WaveNum, SpawnObjNum);
}

FString AB2SpawnPoolEditingActor::GetExtraInfoTextComponentName(int32 WaveNum, int32 SpawnObjNum)
{
	return FString::Printf(TEXT("ExtraInfoTextRenderComponent_%d_%d"), WaveNum, SpawnObjNum);
}

static const FString ComponentNameDisplayComponentNamePrefix("ComponentNameDisplay");
FString AB2SpawnPoolEditingActor::GetComponentNameDisplayComponentName(FString ComponentToDisplayName)
{
	return FString::Printf(TEXT("%s_%s"), *ComponentNameDisplayComponentNamePrefix, *ComponentToDisplayName);
}

USkeletalMeshComponent* AB2SpawnPoolEditingActor::FindRelevantEditingSKMeshComp(int32 WaveNum, int32 SpawnObjNum)
{
	if (RootComponent)
	{
		const TArray<USceneComponent*>& RootAttachedChildren = RootComponent->GetAttachChildren();
		for (int32 SI = 0; SI < RootAttachedChildren.Num(); ++SI)
		{
			USkeletalMeshComponent* CastedChild = Cast<USkeletalMeshComponent>(RootAttachedChildren[SI]);

			if (CastedChild)
			{
				if (CastedChild->GetName() == GetEditingSKMeshComponentName(WaveNum, SpawnObjNum))
				{
					return CastedChild;
				}
			}
		}
	}
	return NULL;
}

UTextRenderComponent* AB2SpawnPoolEditingActor::FindRelevantExtraInfoTextRenderComp(int32 WaveNum, int32 SpawnObjNum)
{
	USkeletalMeshComponent* SKComp = FindRelevantEditingSKMeshComp(WaveNum, SpawnObjNum);
	if (SKComp)
	{
		const TArray<USceneComponent*>& SKAttachedChildren = SKComp->GetAttachChildren();
		for (int32 SI = 0; SI < SKAttachedChildren.Num(); ++SI)
		{
			UTextRenderComponent* CastedChild = Cast<UTextRenderComponent>(SKAttachedChildren[SI]);

			if (CastedChild)
			{
				if (CastedChild->GetName() == GetExtraInfoTextComponentName(WaveNum, SpawnObjNum))
				{
					return CastedChild;
				}
			}
		}
	}
	return NULL;
}

class UTextRenderComponent* AB2SpawnPoolEditingActor::FindRelevantComponentNameDisplayComp(FString ComponentToDisplayName)
{
	if (RootComponent)
	{
		const TArray<USceneComponent*>& RootAttachedChildren = RootComponent->GetAttachChildren();
		for (int32 SI = 0; SI < RootAttachedChildren.Num(); ++SI)
		{
			UTextRenderComponent* CastedChild = Cast<UTextRenderComponent>(RootAttachedChildren[SI]);

			if (CastedChild)
			{
				if (CastedChild->GetName() == GetComponentNameDisplayComponentName(ComponentToDisplayName))
				{
					return CastedChild;
				}
			}
		}
	}
	return NULL;
}

FSpawnPoolEditingWaveObjectSimpleInfo* AB2SpawnPoolEditingActor::FindEditingWaveObjectSimpleInfo(int32 WaveNum, int32 SpawnObjNum)
{
	for (int32 CI = 0; CI < EditingSimpleInfo.Num(); ++CI)
	{
		if (EditingSimpleInfo[CI].WaveNum == WaveNum && EditingSimpleInfo[CI].SpawnObjNum == SpawnObjNum)
		{
			return &(EditingSimpleInfo[CI]);
		}
	}
	return NULL;
}

void AB2SpawnPoolEditingActor::SetOwnerPool(class AB2MonsterSpawnPool* InOwnerPool)
{
	OwnerPool = InOwnerPool;
}

void AB2SpawnPoolEditingActor::SetRepresentingInfo(int32 InClientStageId, EStageDifficulty InDifficultyLevel, int32 InWaveSettingNum, int32 InSpawnObjNum,
	ENPCClass CharacterClassEnum, ENPCClassVariation ClassVariation, TSubclassOf<class ABladeIICharacter> CharacterClassBP,
	bool bInRandomLocation, FTransform ObjWorldTransform, const TArray<UMaterialInterface*>& InOptionalWaveMtrlOverride)
{
	// SetRepresentingInfo call from MonsterSpawnPool always reflect currently active editing stage, so just get this.
	CurrEditingClientStageId = InClientStageId;
	CurrEditingDifficultyLevel = InDifficultyLevel;

	FSpawnPoolEditingWaveObjectSimpleInfo* SimpleInfoThis = FindEditingWaveObjectSimpleInfo(InWaveSettingNum, InSpawnObjNum);
	if (SimpleInfoThis == NULL)
	{
		// Add a new simple editing info if not found.
		FSpawnPoolEditingWaveObjectSimpleInfo NewInfo;
		NewInfo.WaveNum = InWaveSettingNum;
		NewInfo.SpawnObjNum = InSpawnObjNum;
		NewInfo.bRandomLocation = bInRandomLocation;
		EditingSimpleInfo.Add(NewInfo);
	}
	else
	{
		// Or update the necessary information.
		SimpleInfoThis->bRandomLocation = bInRandomLocation;
	}

	USkeletalMeshComponent* EditingMeshComp = FindRelevantEditingSKMeshComp(InWaveSettingNum, InSpawnObjNum);

	// Create a sub component to do the actual job.
	if (EditingMeshComp == NULL)
	{ // 생성을 UB2SpawnPoolEditingSKComponent 로 해 주어야 컴포넌트 개별 이동 시 신호가 간다.
		EditingMeshComp = NewObject<UB2SpawnPoolEditingSKComponent>(this, *GetEditingSKMeshComponentName(InWaveSettingNum, InSpawnObjNum), RF_Transient);
	}

	if (EditingMeshComp)
	{
		// Ensure them to be in the fake array.
		if (FakeSKCompArray.Find(EditingMeshComp) == INDEX_NONE)
		{
			FakeSKCompArray.Add(EditingMeshComp);
		}

		// Mesh component is attached to the root.
		EditingMeshComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		EditingMeshComp->RegisterComponent();
		EditingMeshComp->SetWorldLocationAndRotation(ObjWorldTransform.GetLocation(), ObjWorldTransform.GetRotation()); // Location 은 Mesh 랑 크기 세팅된 다음 offset 반영해서 아래에서 다시.
		EditingMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EditingMeshComp->SetVisibility(true);
		EditingMeshComp->SetHiddenInGame(true);

		UTextRenderComponent* ExtraInfoTextComp = FindRelevantExtraInfoTextRenderComp(InWaveSettingNum, InSpawnObjNum);

		if (ExtraInfoTextComp == NULL)
		{
			ExtraInfoTextComp = NewObject<UTextRenderComponent>(this, *GetExtraInfoTextComponentName(InWaveSettingNum, InSpawnObjNum), RF_Transient);
		}

		if (ExtraInfoTextComp)
		{
			FString ExtraInfoString;

			// InfoText is attached to the mesh and will have some z offset depend on the mesh size. See SetRepresentativeMesh
			ExtraInfoTextComp->AttachToComponent(EditingMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			ExtraInfoTextComp->RegisterComponent();
			ExtraInfoTextComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ExtraInfoTextComp->SetVisibility(true);
			ExtraInfoTextComp->SetHiddenInGame(true);
			ExtraInfoTextComp->SetTextRenderColor(FColor(0, 255, 0));
			ExtraInfoTextComp->SetHorizontalAlignment(EHTA_Center);
			ExtraInfoTextComp->SetWorldSize(25.0f);

			if (bInRandomLocation)
			{
				ExtraInfoString = FString::Printf(TEXT("W%d RAND"), InWaveSettingNum);
			}
			else
			{
				// Give index information
				ExtraInfoString = FString::Printf(TEXT("W%d %d"), InWaveSettingNum, InSpawnObjNum);
			}

			// Display additional error info for invalid class..
			if (CharacterClassEnum == ENPCClass::ENC_End && CharacterClassBP == NULL)
			{
				// 필요에 따라 추가 정보를 확인하든지..
				ExtraInfoString = TEXT("INVALID CLASS!\n") + ExtraInfoString;
				ExtraInfoTextComp->SetTextRenderColor(FColor(255, 0, 255));
			}

			ExtraInfoTextComp->SetText(FText::FromString(ExtraInfoString));
		}

		SetRepresentativeMesh(InWaveSettingNum, InSpawnObjNum, CharacterClassEnum, ClassVariation, CharacterClassBP);

		// NPCClassInfo 에 의한 Mesh 및 Material 세팅은 SetRepresentativeMesh 에서 처리하고 SpawnPool 자체적으로 세팅하는 추가 material override 처리.
		AB2MonsterSpawnPool::OverrideMaterialForNewSpawned(nullptr, InOptionalWaveMtrlOverride, EditingMeshComp);

		// SyncEditingActorLocationToSpawnPool 에서 바운드 크기만큼 올린 값을 사용하므로 여기서는 그만큼 내려준다.
		EditingMeshComp->SetWorldLocation(ObjWorldTransform.GetLocation() - GetEditingCompToSpawnLocOffset(EditingMeshComp));
	}
}

void AB2SpawnPoolEditingActor::SetRepresentativeMesh(int32 InWaveSettingNum, int32 InSpawnObjNum, ENPCClass CharacterClassEnum, ENPCClassVariation ClassVariation, TSubclassOf<class ABladeIICharacter> CharacterClassBP)
{
	USkeletalMeshComponent* EditingMeshComp = FindRelevantEditingSKMeshComp(InWaveSettingNum, InSpawnObjNum);
	if (EditingMeshComp && (CharacterClassEnum != ENPCClass::ENC_End || CharacterClassBP != NULL))
	{
		USkeletalMesh* SKMeshFromClass = NULL;
		TArray<UMaterialInterface*> MtrlOverrideFromClass;

		// Same priority between enum class and BP class as AB2MonsterSpawnPool::SpawnWave
		if (CharacterClassEnum != ENPCClass::ENC_End)
		{
			// Cannot get the Class Info from BladeIIGameMode instance, as this is in an editor viewport, not a game.
			UB2NPCClassInfoBox* ClassInfoBox = StaticFindMobClassInfoBox();
			if (ClassInfoBox)
			{
				UB2NPCClassInfoBox::NPCClassPreviewAssetInfo PreviewAssetInfo = ClassInfoBox->GetPreviewAssetInfoFromClass(CharacterClassEnum, ClassVariation);
				SKMeshFromClass = PreviewAssetInfo.SKMesh;
				MtrlOverrideFromClass = PreviewAssetInfo.MaterialOverrides; // NPCClassInfo 에 의한 기본적인 MaterialOverride 인데, 이것 이후에 SpawnPool WaveSetting 에 의한 Material Override 도 있을 수 있다.
			}
		}
		else if (CharacterClassBP)
		{
			ABladeIICharacter* CharacterObj = Cast<ABladeIICharacter>(CharacterClassBP->GetDefaultObject());
			if (CharacterObj && CharacterObj->GetMesh())
			{
				//SKMeshFromClass = CharacterObj->GetMesh()->Get;
			}
		}

		//if (SKMeshFromClass) NULL 이면 NULL 로 세팅. 그래야 잘못된 걸 알겠지..
		{
			for (int32 MI = 0; MI < EditingMeshComp->GetNumMaterials(); ++MI)
			{ // 컴포넌트 재활용 할 수 있으므로 기존 꺼 클리어 해야.
				EditingMeshComp->SetMaterial(MI, nullptr);
			}
			EditingMeshComp->SetSkeletalMesh(SKMeshFromClass);
			for (int32 MI = 0; MI < MtrlOverrideFromClass.Num(); ++MI)
			{
				if (MtrlOverrideFromClass[MI])
				{
					EditingMeshComp->SetMaterial(MI, MtrlOverrideFromClass[MI]);
				}
			}

			float MeshScale = 1.0f;

			// Need to reset the scale before we do the proper MeshScale calculation. Looks like bound get updated with SetRelativeScale call?
			EditingMeshComp->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
			// See the height of the mesh component and give some scale to limit it.
			float MeshHeight = EditingMeshComp->Bounds.BoxExtent.Z * 2.0f;
			if (MeshHeight > MeshSizeLimit)
			{
				MeshScale = MeshSizeLimit / MeshHeight;
			}
			EditingMeshComp->SetRelativeScale3D(FVector(MeshScale, MeshScale, MeshScale));

			UTextRenderComponent* ExtraInfoText = FindRelevantExtraInfoTextRenderComp(InWaveSettingNum, InSpawnObjNum);

			if (ExtraInfoText)
			{
				// ExtraInfoText is child of MeshComp, so scale with inverse to maintain its size.
				const float InvMeshScale = 1.0f / MeshScale;
				ExtraInfoText->SetRelativeScale3D(FVector(InvMeshScale, InvMeshScale, InvMeshScale));

				// Most SkeletalMesh assets have their root at their feet, so multiply the box extent by 2.
				// Relative location takes a value before scaling, so give InvMeshScale.
				const float TextRenderZOffset = (EditingMeshComp->Bounds.BoxExtent.Z * 2.0f + 20.0f) * InvMeshScale;

				FVector TextRelativeLocation = ExtraInfoText->GetRelativeTransform().GetLocation();
				TextRelativeLocation.Z = TextRenderZOffset;
				ExtraInfoText->SetRelativeLocation(TextRelativeLocation);
			}
		}
	}
}

FVector AB2SpawnPoolEditingActor::GetEditingCompToSpawnLocOffset(class USkeletalMeshComponent* EditingComp)
{ // EditingComponent 의 반쪽 크기를 감안하는 거.
	// EditingComponent 들은 원점이 발바닥인데 실제 spawn 할 BladeIICharacter 들은 원점이 중심이므로.. 정확하지는 않지만 어느 정도 감안해서 보정해 주기 위함.
	// 어차피 spawn 시 바닥에 snap 시킬 것이므로 위치를 약간 높게 선정할 필요가 있다.
	return EditingComp ? FVector(0.0f, 0.0f, EditingComp->Bounds.BoxExtent.Z * EditingComp->BoundsScale) : FVector(0.0f, 0.0f, 0.0f);
}

void AB2SpawnPoolEditingActor::RemoveEditingInfo_UnnecessaryWave(int32 AllWaveNum)
{
	// Need to do things with copied array, because RemoveEditingInfo_UnnecessaryObj can remove some element from EditingSimpleInfo.
	TArray<FSpawnPoolEditingWaveObjectSimpleInfo> EditingSimpleInfoCopy = EditingSimpleInfo;

	for (int32 EI = 0; EI < EditingSimpleInfoCopy.Num(); ++EI)
	{
		FSpawnPoolEditingWaveObjectSimpleInfo& CurrInfo = EditingSimpleInfoCopy[EI];

		if (CurrInfo.WaveNum >= AllWaveNum)
		{
			RemoveEditingInfo_UnnecessaryObj(CurrInfo.WaveNum, -1); // Sending -1 to RemoveEditingInfo_UnnecessaryObj will remove all entry in the wave..
		}
	}
}

void AB2SpawnPoolEditingActor::RemoveEditingInfo_AllButOneWave(int32 RemainWaveNum)
{
	// Need to do things with copied array, because RemoveEditingInfo_UnnecessaryObj can remove some element from EditingSimpleInfo.
	TArray<FSpawnPoolEditingWaveObjectSimpleInfo> EditingSimpleInfoCopy = EditingSimpleInfo;

	for (int32 EI = 0; EI < EditingSimpleInfoCopy.Num(); ++EI)
	{
		FSpawnPoolEditingWaveObjectSimpleInfo& CurrInfo = EditingSimpleInfoCopy[EI];

		if (CurrInfo.WaveNum != RemainWaveNum)
		{
			RemoveEditingInfo_UnnecessaryObj(CurrInfo.WaveNum, -1); // Sending -1 to RemoveEditingInfo_UnnecessaryObj will remove all entry in the wave..
		}
	}
}

void AB2SpawnPoolEditingActor::RemoveEditingInfo_UnnecessaryObj(int32 WaveNum, int32 AllObjNumInWave)
{
	for (int32 EI = 0; EI < EditingSimpleInfo.Num(); ++EI)
	{
		FSpawnPoolEditingWaveObjectSimpleInfo& CurrInfo = EditingSimpleInfo[EI];

		if (CurrInfo.WaveNum == WaveNum && CurrInfo.SpawnObjNum >= AllObjNumInWave)
		{
			// Remove and destroy the relevant components

			UTextRenderComponent* TextCompToRemove = FindRelevantExtraInfoTextRenderComp(CurrInfo.WaveNum, CurrInfo.SpawnObjNum);
			if (TextCompToRemove)
			{
				TextCompToRemove->RemoveFromRoot();
				TextCompToRemove->UnregisterComponent();
				TextCompToRemove->ConditionalBeginDestroy();
			}
			// Is just removing SKComp enough..? TextRenderComp is attached to this..
			USkeletalMeshComponent* SKCompToRemove = FindRelevantEditingSKMeshComp(CurrInfo.WaveNum, CurrInfo.SpawnObjNum);
			if (SKCompToRemove)
			{
				SKCompToRemove->RemoveFromRoot();
				SKCompToRemove->UnregisterComponent();
				SKCompToRemove->ConditionalBeginDestroy();
				FakeSKCompArray.Remove(SKCompToRemove);
			}

			// And.. finally remove the info too.
			EditingSimpleInfo.RemoveAt(EI);
			--EI;
		}
	}
}

void AB2SpawnPoolEditingActor::SetComponentToDisplayInfo(class UPrimitiveComponent* InComp)
{
	if (InComp)
	{
		UTextRenderComponent* DisplayTextRenderComp = FindRelevantComponentNameDisplayComp(InComp->GetName());

		// Create if not exist yet.
		if (DisplayTextRenderComp == NULL)
		{
			DisplayTextRenderComp = NewObject<UTextRenderComponent>(this, *GetComponentNameDisplayComponentName(InComp->GetName()), RF_Transient);
		}

		if (DisplayTextRenderComp)
		{
			DisplayTextRenderComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			DisplayTextRenderComp->RegisterComponent();
			DisplayTextRenderComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DisplayTextRenderComp->SetVisibility(true);
			DisplayTextRenderComp->SetHiddenInGame(true);
			DisplayTextRenderComp->SetTextRenderColor(FColor(255, 0, 0));
			DisplayTextRenderComp->SetHorizontalAlignment(EHTA_Center);
			DisplayTextRenderComp->SetWorldSize(35.0f);

			DisplayTextRenderComp->SetText(FText::FromString(InComp->GetName()));

			// A little above from the component ceiling.
			DisplayTextRenderComp->SetWorldLocation(InComp->GetComponentLocation() + FVector(0.0f, 0.0f, InComp->Bounds.BoxExtent.Z + 100.0f));
		}
	}
}

void AB2SpawnPoolEditingActor::RemoveAllComponentDisplay()
{
	if (RootComponent)
	{
		TArray<UTextRenderComponent*> ToBeRemoved;

		// TextRenderComponent for component name display are all attached directly to the root, and not for others..
		const TArray<USceneComponent*>& RootAttachedChildren = RootComponent->GetAttachChildren();
		for (int32 TI = 0; TI < RootAttachedChildren.Num(); ++TI)
		{
			UTextRenderComponent* CurrTextRenderComp = Cast<UTextRenderComponent>(RootAttachedChildren[TI]);

			if (CurrTextRenderComp)
			{
				if (CurrTextRenderComp->GetName().Contains(ComponentNameDisplayComponentNamePrefix))
				{
					// Now we are pretty sure that this text render component is for component name display.
					ToBeRemoved.Add(CurrTextRenderComp);
				}
			}
		}

		for (int32 TI = 0; TI < ToBeRemoved.Num(); ++TI)
		{
			UTextRenderComponent* CurrComp = ToBeRemoved[TI];
			CurrComp->RemoveFromRoot();
			CurrComp->UnregisterComponent();
			CurrComp->ConditionalBeginDestroy();
		}
	}
}

void AB2SpawnPoolEditingActor::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	SyncEditingActorLocationToSpawnPool();
}

void AB2SpawnPoolEditingActor::SyncEditingActorLocationToSpawnPool()
{
	if (OwnerPool)
	{
		for (int32 SI = 0; SI < EditingSimpleInfo.Num(); ++SI)
		{
			// This is probably the main role of EditingSimpleInfo.. To know necessary WaveNum and SpawnObjNum
			USkeletalMeshComponent* SKMeshComp = FindRelevantEditingSKMeshComp(EditingSimpleInfo[SI].WaveNum, EditingSimpleInfo[SI].SpawnObjNum);

			if (SKMeshComp && EditingSimpleInfo[SI].bRandomLocation == false)
			{
				// 원점이 발바닥인 EditingComponent 들에 대해 원점이 중심인 BladeIICharacter 들을 고려해서 조금이나만 원점 보정을.. 어차피 spawn 시 바닥에 snap 시키므로 좀 높여줘도 됨.
				FVector HeightAdjustedLocation = SKMeshComp->GetComponentLocation() + GetEditingCompToSpawnLocOffset(SKMeshComp);

				OwnerPool->ExplictSetSpawnTransform(CurrEditingClientStageId, CurrEditingDifficultyLevel, EditingSimpleInfo[SI].WaveNum, EditingSimpleInfo[SI].SpawnObjNum,
					HeightAdjustedLocation, SKMeshComp->GetComponentRotation());
			}
		}
		OwnerPool->MarkPackageDirty();
	}
}
#endif
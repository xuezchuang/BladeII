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
		{ // Normal ³­ÀÌµµ·Î Æú¹é ½Ãµµ
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
	{ // »õ·Î Ãß°¡ÇÑ µ¥ÀÌÅÍ ¾øÀ» ¶§ ±âº» ¼³Á¤Çß´ø °É·Î ¸ðµç ³­ÀÌµµ µ¥ÀÌÅÍ Ã¤¿ö³Ö±â
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
			WrongKeyWarnStr = FString::Printf(TEXT("SummonSettingAsset %s ¿¡ Á¸ÀçÇÏÁö ¾Ê´Â ³­ÀÌµµ ¼ýÀÚ¸¦ Å° °ªÀ¸·Î °®´Â element °¡ ÀÖÀ½\r\nÇØ´ç Å° °ª: "), *this->GetName())
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
	// ¿¾³¯ ¿¾Àû¿¡ InfoAsset Á¢±Ù ÄÚµåµéÀÌ Á¤¸® ¾ÈµÇ¾î ÀÖ´ø ½ÃÀý¿¡ ÄÚµå°¡ Á» ´õ ±æ¾ú¾î¼­ ÀÌ·¸°Ô µû·Î ºüÁ® ÀÖ´Â °Å ¤Ñ¤Ñ;
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
		// È¤½Ã¶óµµ ·¹º§ ÀÌ¸§ ¹Ù²Ù¸é redirector Ã³¸®°¡ ¾ÈµÇ¹Ç·Î ¿©±â¼­µµ °Ë»ç.
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
	// GetDesiredSpawnPoolOfSettingFromList ¿¡¼­µµ ºñ½ÁÇÑ ±â´ÉÀ» ÇÔ. AB2MonsterSpawnPool ¾×ÅÍ¸¦ ´ë»óÀ¸·Î

	TArray<FMonsterSpawnPoolPerStageSettings*> FoundSettingOfClientStageId; // StageNum ¸¸ ¸Â´Â °Å ¸ÕÀú °É·¯³¿.
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
		// ³ôÀº ³­ÀÌµµºÎÅÍ iteration ÇÏµµ·Ï Á¤·Ä
		FoundSettingOfClientStageId.Sort([](const FMonsterSpawnPoolPerStageSettings& A, const FMonsterSpawnPoolPerStageSettings& B) { return A.TargetDifficultyLevel > B.TargetDifficultyLevel; });

		for (FMonsterSpawnPoolPerStageSettings* ThisSetting : FoundSettingOfClientStageId)
		{
			if (
				((EStageDifficulty)ThisSetting->TargetDifficultyLevel == DifficultyLevel) ||
				(bFallBackToLowerDifficulty && (EStageDifficulty)ThisSetting->TargetDifficultyLevel < DifficultyLevel) // Á¤È®È÷ ¶È°°Àº ³­ÀÌµµ ¼¼ÆÃÀÌ ¾øÀ» ¶§ °¡Àå °¡±î¿î º¸´Ù ³·Àº ³­ÀÌµµ·Î Æú¹é.
				)
			{
				return ThisSetting;
			}
		}
		// ¾ÆÁ÷µµ ¸®ÅÏÀ» ¸øÇß´Ù¸é ´õ ³ôÀº ³­ÀÌµµ ¼¼ÆÃ¸¸ ÀÖ´ø °ÍÀÎµ¥ ±×°É·Î¶óµµ °¡Àå °¡±î¿î °É·Î Æú¹é ½Ãµµ.. ÇÏ´Â °Ç ÀÏ´Ü º¸·ùÇÏÀÚ.. 
		// Lower ¿¡¸¸ Àû¿ë. ´Ù¸¥ °÷¿¡µµ lower ·Î¸¸ fallback ÇÏ´Â ÄÚµå°¡ ÀÖ±âµµ ÇÏ°í.
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
	// ÀÌ¹ø ½ºÅ×ÀÌÁö¿¡¼­ Æ¯º°È÷ ¿À¹ö¶óÀÌµùÇÑ ¼¼ÆÃ ¸ÕÀú
	if (StageSetting)
	{
		for (int32 SI = 0; SI < StageSetting->SummonSettings.Num(); ++SI)
		{
			if (StageSetting->SummonSettings[SI].SummonIndex == SummonIndex)
			{
				FMonsterSpawnPoolSummonSettings NewSetting = StageSetting->SummonSettings[SI];
				NewSetting.TransientMySettingIndex = SI; // ¼ÒÈ¯ ¸÷ ¼ö Á¦ÇÑ¿¡ Á» »ç¿ëÇÒ ÀÏÀÌ ÀÖ¾î¼­ ¼¼ÆÃ ÀÎµ¦½º¸¦ Àá±ñ ÀúÀåÇØ µÐ´Ù..
				OutMatchingSettings.Add(NewSetting);
			}
		}
	}

	// ¸¸ÀÏ ½ºÅ×ÀÌÁö ¼¼ÆÃ¿¡¼­ Ã£Àº °Ô ¾ø´Ù¸é °øÅë ¿¡¼Â ¼¼ÆÃ¿¡¼­ Ã£´Â´Ù.
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
					NewSetting.TransientMySettingIndex = SI; // ¼ÒÈ¯ ¸÷ ¼ö Á¦ÇÑ¿¡ Á» »ç¿ëÇÒ ÀÏÀÌ ÀÖ¾î¼­ ¼¼ÆÃ ÀÎµ¦½º¸¦ Àá±ñ ÀúÀåÇØ µÐ´Ù..
					OutMatchingSettings.Add(NewSetting);
				}
			}
		}
	}
}

int32 AB2MonsterSpawnPool::GetStageWaveNumBySettingIndex(int32 SettingIndex)
{
	// °Á ÀÎµ¦½º ¸ÂÃã
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
	// Á¤È®È÷ InTestStageNum °ú InTestDifficulty ¿¡ ÇØ´çÇÏ´Â ¼¼ÆÃÀÌ ÀÖ´ÂÁö °Ë»ç
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

	// ¿©±â Ã³À½ spawn ÇÏ¸é¼­ Async ·Îµù ¿Ï·áµÇ´Â °Íµµ ÀÖ°í ÇÒ °Í.
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
		{// ÀÌ¹ø ½ºÅ×ÀÌÁö¿¡ ¾²ÀÏ °Å ¾Æ´Ï¸é ¸ðµÎ ºñÈ°¼ºÈ­.
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
				RuntimeWaveStates.Add(FMonsterSpawnPoolRuntimeWaveState()); //RuntimeWaveStates.Num()ÀÌ GetStageWaveNum()ÀÌ¶û ´Ù¸¦ ¼ö ¾øÀ½. DestroyAllActiveWaves()¿¡¼­ RuntimeWaveStates array¸¦ ÃÊ±âÈ­.
	
				//SpawnWave() ¿¡¼­ FinalSpawnNum°ªÀ» °áÁ¤ÇÏ´ø °ÍÀ» ÀÌÂÊ¿¡¼­ ¹Ì¸® ±¸ÇØ ³õ´Â´Ù.
				//Wave¿¡ µîÀåÇÒ ¸÷ÀÇ ¼ö¸¦ ÆíÇÏ°Ô ±¸ÇÏ±â À§ÇÔ.
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
			
			ConditionalBeginWaveCheck(); // Beginning Á¶°ÇÀÇ wave ½ÃÀÛ. ÀÌ°É º°µµ·Î call ÇÏ°Ô µÇ´Â °æ¿ì BeginPlay ´Â Áö³­ ÈÄ¿¡ ÇØ¾ß.
		}
}

void AB2MonsterSpawnPool::ConditionalBeginWaveCheck()
{// ÀÏ¹ÝÀûÀ¸·Î´Â ±âº»ÀûÀÎ BeginPlay Á÷ÈÄ¿¡ Ã¹ wave ½ÇÇàÀ» °Ë»çÇÏ°Ô µÈ´Ù. ¾îÂ¼´Ù ÇÊ¿äÇÑ ÀûÀÌ ÀÖ¾î¼­ Áßº¹ ÁøÀÔÇÒ ¼ö ÀÖ°Ô ÇØ µÒ.
	if (AmIActiveForCurrentWorld() && !bCheckedForBeginWave)
	{
		bCheckedForBeginWave = true;
		CheckNewWaveSpawnBeginning();
	}
}

bool AB2MonsterSpawnPool::AmIActiveForCurrentWorld()
{
	// PreBeginPlay ÀÌÈÄ ½ÃÁ¡¿¡ »ç¿ë °¡´É.
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

void AB2MonsterSpawnPool::DestroyAllActiveWaves() // ForceClearWaves ¿Í´Â ´Ù¸¥ »óÅÂ Reset ¸ñÀûÀ¸·Î »ç¿ëÇÏ´Â °Í.
{
	FMobSpawnedBirthplaceInfo NullBirthplaceInfo;

	for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI)
	{
		FMonsterSpawnPoolRuntimeWaveState& CurrState = RuntimeWaveStates[WI];
		for (int32 FI = 0; FI < CurrState.FellowsInThisWave.Num(); ++FI)
		{
			ABladeIICharacter* ThisFellow = CurrState.FellowsInThisWave[FI];
			if (ThisFellow)
			{ // ÀÌÂÊÀ¸·Î ½ÅÈ£ ¾È ¿Àµµ·Ï ÇÑ ÈÄ¿¡ Á×ÀÓ.
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

	// GetAsyncRequestInfoFromNPCIds ¿¡¼­´Â AddUnique ¸¦ ÇÏÁö ¾Ê´Â´Ù. 
	// ¸¸ÀÏ ÀÌ°É ¹Ýº¹ »ç¿ëÇÏ¸é¼­ RequestAsset µéÀ» ¸ðÀ¸·Á´Â ÀÇµµÀÌ¸é ÀÌ°É »ç¿ëÇÏ´Â ÂÊ¿¡¼­ Áßº¹µÇÁö ¾Êµµ·Ï °É·¯³»µµ·Ï

	GetWaveNPCIDs(WaveIndex, NPCClassIDs);
	if (UB2NPCClassInfoBox* NPCInfoBox = GetMobClassInfoBox())
	{
		NPCInfoBox->GetAsyncRequestInfoFromNPCIds(NPCClassIDs, OutRequestAssets);
	}
}

/** ·¹º§ ½ÃÀÛ ÀÌÈÄ¿¡ Async ·ÎµùÀ» ÇØµµ µÇ´Â °Í ÀÌ¿ÜÀÇ.. Ã³À½ºÎÅÍ ·ÎµùÇØ¾ß ÇÏ´Â °Å ¸ðÀ½. */
void AB2MonsterSpawnPool::GetEssentialPreloadNPCIDs(TArray<FCombinedNPCClassID>& OutNPCIDs)
{
	const EStageDifficulty Difficulty = static_cast<EStageDifficulty>(CurrentDifficultyLevel);
	const int32 StageWaveNum = GetStageWaveNum(CurrentClientStageId, Difficulty);

	TArray<ECharacterType> AllUsedTypes; // Class Á÷Á¢ ÁöÁ¤ ÀÌ¿ÜÀÇ °Íµé.

	//
	// °¢ Wave º°·Î Class ¸¦ Á÷Á¢ ¸í½ÃÇÑ °ÍµéÀº °ÔÀÓ ½ÃÀÛ ÀÌÈÄÀÇ Async ·ÎµùÀ» À§ÇØ GetWaveNPCIDs ·Î ¿Å°Ü°£ »óÅÂ.
	// TypeMap Á¤µµ´Â Æ¯Á¤ Wave ÀÌÈÄÀÇ °ÍÀ» ¸ðÀ» ¼öµµ ÀÖÀ» µí.
	// SummonSetting Àº Ã³À½ ÇÑÅ¥¿¡ ·ÎµùÇØ¾ß ÇÔ.
	//

	// ¼ÒÈ¯ ¼¼ÆÃÀº Wave ±¸ºÐ ¾øÀ½. ¸ðµÎ Ã¹¹æ¿¡ ·ÎµùÇØ¾ß
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


	// WaveSettings ¿¡¼­ Type ¸¸ ¸ðÀ½. ½ÇÁ¦ Spawn ¼¼ÆÃ¿¡ »ç¿ëÇÑ Type µéÀ» ±Ü¾î¿À±â À§ÇØ. Type ¿¡ ¸ÊÇÎµÇ´Â Å¬·¡½º´Â ¸¹¾Æµµ Type ÀÚÃ¼´Â ¸¹Áö ¾Ê´Ù.
	for (int32 WI = 0; WI < StageWaveNum; ++WI)
	{
		TArray<FCombinedNPCClassID> LocalTempIDs;
		GatherAllClassAndTypesFromSetting(LocalTempIDs, AllUsedTypes, GetStageWaveSetting(CurrentClientStageId, Difficulty, WI), NULL);
	}

	// ClassEnum ¿Ü¿¡ Type À¸·Î ÁöÁ¤ÇÑ °Íµé¿¡ ´ëÇØ ÇÊ¿äÇÑ °Íµé Ã£¾Æ³»±â.
	UB2MonsterClassTypeMap* TypeMapAsset = TypeMapClassAsset ? Cast<UB2MonsterClassTypeMap>(TypeMapClassAsset->GetDefaultObject()) : NULL;
	if (bUseOwnTypeMapInfo || TypeMapAsset)
	{
		TArray<FMonsterTypeMapSingleInfo>& FinalTypeMapInfoArray = bUseOwnTypeMapInfo ? OwnTypeMapInfo : TypeMapAsset->InfoArray;

		for (int32 TMI = 0; TMI < FinalTypeMapInfoArray.Num(); ++TMI)
		{
			FMonsterTypeMapSingleInfo& CurrTypeMapInfo = FinalTypeMapInfoArray[TMI];

			// TypeMap ¿¡ µî·ÏµÈ °Íµé Áß ½ÇÁ¦·Î spawn pool ¿¡ ¼¼ÆÃÈù type ÀÎ °æ¿ì¸¸. TypeMap ¿¡ µî·ÏµÈ ÀüÃ¼¸¦ °¡Á®¿À´Â °Å³ª ´ëÃ¼·Î´Â Â÷ÀÌ ¾ø°ÚÁö¸¸.
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

/** MonsterSpawnPool À» ÅëÇÑ NPCClass ÀÇ Preload ½ÃÁ¡ ÀÌÈÄ Async ·Îµù °³¼ö Á¦ÇÑ ¼³Á¤.
 * ÀÌ ¼³Á¤À» DeviceProfile ¿¡ ³ÖÀ» ¼ö ÀÖÀ½. */
static TAutoConsoleVariable<int32> CVarPostGameLoadNPCAsyncLoadLimit(
	TEXT("r.PostGameLoadNPCAsyncLoadLimit"),
	//////////
	3, // ¼³Á¤¿¡ ´ëÇÑ Å×½ºÆ® °á°ú http://172.168.2.105:8090/pages/viewpage.action?pageId=4822058
	//////////
	TEXT("Limit the maximum number of NPCClass data that can be loaded after world BeginPlay, in asynchronous manner."),
	ECVF_Default);

void AB2MonsterSpawnPool::PreloadPoolMonsters()
{
	// ¸Ê ·Îµù ½ÃÁ¡¿¡ ¹Ì¸® ·ÎµùÇØ ³õÀ» °Íµé.
	// ¿©±â¼­ ·Îµù ¾È ÇÑ °ÍµéÀº °ÔÀÓ ½ÃÀÛ ÀÌÈÄ ¹é±×¶ó¿îµå ·ÎµùÀ» ÇÏ°íÀÚ ÇÏ´Â ÀÇµµ.

	TArray<FCombinedNPCClassID> PreloadNPCIDs;

	GetEssentialPreloadNPCIDs(PreloadNPCIDs);

	// Àü¿ª ¼³Á¤À¸·Î Async ·ÎµùÀ» ÇÒ ÃÖ¼Ò wave index ÀÌÀü±îÁö ±Ü¾î¿À°í.. 0 Àº Ã·ºÎÅÍ ÇÊ¿äÇÒ Å×´Ï ±âº» Æ÷ÇÔ
	const int32 BasicAsyncBeginWave = FMath::Max(GetBasicAsyncLoadBeginWave(), 1);
	for (int32 Wave = 0; Wave < BasicAsyncBeginWave; Wave++)
		GetWaveNPCIDs(Wave, PreloadNPCIDs);

	// ¿©±â±îÁö°¡ ±âº»ÀûÀ¸·Î ±Ü¾î¸ðÀº Preload Å¬·¡½ºµé
	const TArray<FCombinedNPCClassID> BasicAsummedPreloadIDs = PreloadNPCIDs;

	//
	// Ãß°¡ ±âÁØÀ» ÅëÇØ Async ¹é±×¶ó¿îµå ·ÎµùÀ» ÇØ¾ß ÇÏ´Â °Ô ±âÁØ ÀÌ»óÀ¸·Î ¸¹À¸¸é ±× ÀÌÀü±îÁö´Â Preload ¿¡ Æ÷ÇÔ½ÃÅ²´Ù.
	//

	// ÀÏ´Ü ³¡±îÁö ÇÊ¿äÇÑ °Ô ¾ó¸¶³ª µÇ´ÂÁö ±Ü¾î¿È. ÈÄ¹Ý wave ¿¡ ÇÊ¿äÇÑ °ÍÀÏ¼ö·Ï ±Ü¾î¿Â ¸®½ºÆ®ÀÇ µÚ¿¡ À§Ä¡.
	const int32 MaxWaveNum = GetStageWaveNum(CurrentClientStageId, static_cast<EStageDifficulty>(CurrentDifficultyLevel));
	for (int32 Wave = BasicAsyncBeginWave; Wave < MaxWaveNum; Wave++)
		GetWaveNPCIDs(Wave, PreloadNPCIDs);

	IConsoleVariable* CVarAsyncLoadLimit = IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostGameLoadNPCAsyncLoadLimit"));
	const int32 AsyncLoadLimit = CVarAsyncLoadLimit ? FMath::Max(CVarAsyncLoadLimit->GetInt(), 0) : 0;

	// CVar Á¦ÇÑ ÀÌÀüÀÇ async ´ë»ó °³¼ö
	const int32 BasicAssumedAsyncNum = PreloadNPCIDs.Num() - BasicAsummedPreloadIDs.Num();
	// CVar Á¦ÇÑÀÌ °É¸° async ´ë»ó °³¼ö
	int32 FinalAsyncReserveNum = FMath::Min(BasicAssumedAsyncNum, AsyncLoadLimit);

	// ÀÏ´Ü ·¹ÀÌµå ÁøÀÔÀÌ ¾ÈµÇ´Â ¹®Á¦ ¶§¹®¿¡ ÀÓ½Ã ÁÖ¼®Ã³¸®.
	// ²À ¹°¾îºÁ¾ßÇÔ.
	//check(PreloadNPCIDs.Num() > FinalAsyncReserveNum);

	ReservedNPCClassIDsForAsyncLoad.Empty();
	// Çã°¡µÈ °¹¼ö¸¸Å­ µÚ¿¡¼­ºÎÅÍ Â÷·Ê´ë·Î Reserved ¸®½ºÆ®¿¡ ³Ö´Âµ¥ Ã¹ ¿¬Ãâ µ¹¾Æ°¡´Â ¿ÍÁß¿¡ ¿Ï·á¸¦ ¾î·Æ°Ô ÇÏ´Â (¹«°Å¿î) Blacklist ´Â Á¦¿Ü½ÃÅ²´Ù.
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

	// ÃÖÁ¾ÀûÀ¸·Î ÃëÇÕµÈ Preload ¸®½ºÆ®·Î ·Îµù ½ÃÀÛ.
	TArray<FB2AsyncRequestInfo> PreloadAssets;
	if (UB2NPCClassInfoBox* NPCInfoBox = GetMobClassInfoBox())
	{
		NPCInfoBox->GetAsyncRequestInfoFromNPCIds(PreloadNPCIDs, PreloadAssets);
		// TryAsyncLoad ¸¦ ÇÏÁö¸¸ ÀÌ°É »ç¿ëÇÏ´Â °Ô ¸Ê ·Îµù ½ÃÁ¡ÀÌ¶ó °á±¹ Flush ³»Áö´Â ºñ½º¹«¸®ÇÏ°Ô µÇ¾î ½ÃÀÛ Àü¿¡ ·ÎµùµÉ °Í.
		NPCInfoBox->TryAsyncLoad(TEXT("MonsterPool Preload"), PreloadAssets);
	}
}

void AB2MonsterSpawnPool::TryAsyncLoadReservedMonsters()
{
	// PreloadPoolMonsters ¿¡¼­ ·Îµù ¸øÇÑ NPCClass µéÀ» °ÔÀÓ ½ÃÀÛ ÀÌÈÄ¿¡ Async ¿äÃ»ÇÒ ¸ñÀû
	UB2NPCClassInfoBox* NPCInfoBox = GetMobClassInfoBox();
	if (NPCInfoBox && ReservedNPCClassIDsForAsyncLoad.Num() > 0)
	{
		TArray<FB2AsyncRequestInfo> AsyncLoadAssets;
		NPCInfoBox->GetAsyncRequestInfoFromNPCIds(ReservedNPCClassIDsForAsyncLoad, AsyncLoadAssets);
		const FString RequestName = FString::Printf(TEXT("MonsterPool ReservedAsyncReq %s"), *GetName());
		NPCInfoBox->TryAsyncLoad(RequestName, AsyncLoadAssets);
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
		NPCInfoBox->DevTestMarkLastAsyncReqTime(RequestName); // ´Ù¼öÀÇ ¿äÃ»¿¡ ´ëÇØ Á¦°¢°¢ ½Ã°£ ÃøÁ¤À» ÇÏÁö ¸øÇÏ¹Ç·Î ½ÇÁ¦·Î ½Ã°£ ÃøÁ¤À» ÇÏ°íÀÚ ÇÏ´Â ½ºÆÌ¿¡¼­¸¸ »ç¿ë
#endif
		ReservedNPCClassIDsForAsyncLoad.Empty();
	}
}

void AB2MonsterSpawnPool::TryAsyncLoadMonsters(int32 BeginWave, int32 EndWave /* = INDEX_NONE (== MaxWave) */)
{
	// PostGameLoadNPCAsyncLoadLimit µµÀÔ ÀÌÀüÀÇ ÄÚµåÀÎµ¥, È¤½Ã¶óµµ Wave ¹üÀ§¸¦ ÄÛ Âï¾î¼­ async ·ÎµùÀ» ÇÒ ÇÊ¿ä°¡ »ý±â¸é..

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
		// GetSpawnNPCAssets ¼º°Ý»ó ±×ÂÊ¼­ AddUnique ÇÏ´Â °Ç ¾Æ´Ñ °Å °°¾Æ ¿©±â¼­ ¸Å Wave ¸¶´Ù AddUnique.
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
	// ±âº»ÀûÀ¸·Î NPCClass µ¥ÀÌÅÍÀÇ Async ·ÎµùÀ» Àû¿ëÇÏ°íÀÚ ÇÏ´Â ÃÖÃÊ wave ³Ñ¹ö. ÀÌ ÀÌÀü wave ¶û typemap, summon µîÀº ÀÏÂ÷ÀûÀÎ Preload ´ë»óÀÌ µÈ´Ù.
	// ÀÌ°Í°ú ÀÌÈÄ wave °¡ ¸ðµÎ async ·ÎµùÀÌ µÇ´Â °Ç ¾Æ´Ï°í PostGameLoadNPCAsyncLoadLimit ¿¡ ÀÇÇØ Ãß°¡·Î Preload ¸¦ ÇÏ°í ³²Àº ³ª¸ÓÁö°¡ async ´ë»óÀÌ µÈ´Ù.
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

	// WaveSetting È¤Àº SummonSetting µÑ Áß ÇÏ³ª¿¡¼­ ÃÖÁ¾ ¼³Á¤µéÀ» °¡Á®¿Â´Ù.
	const TArray<FMonsterSpawnClassInfo>& SpawnClasses = WaveSetting ? WaveSetting->SpawnClasses : SummonSetting->SpawnClasses;
	const TArray<FMonsterSpawnTypeSpecific>& SpawnTypes = WaveSetting ? WaveSetting->SpawnCharacterTypes : SummonSetting->SpawnCharacterTypes;

	for (int32 SI = 0; SI < SpawnClasses.Num(); ++SI)
	{
		const FMonsterSpawnClassInfo& ThisSpawnClassInfo = SpawnClasses[SI];

		if (ThisSpawnClassInfo.CharacterClassEnum != ENPCClass::ENC_End)
		{ // CharacterClassEnum ¼³Á¤µÈ °ÍÀº ¹Ù·Î ¸®ÅÏ°ª¿¡ Ãß°¡.
			OutCombinedIDs.AddUnique(FCombinedNPCClassID(ThisSpawnClassInfo.CharacterClassEnum, ThisSpawnClassInfo.ClassVariation));
		}
	}

	if (SpawnClasses.Num() == 0 && SpawnTypes.Num() > 0)
	{
		// ÀÌ °æ¿ì TypeMap À» ¾²°Ô µÇ´Âµ¥ »ç¿ëÇÏ´Â Å¸ÀÔÀ» ¸ðµÎ ¸ð¾Æ³õ´Â´Ù.
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
				// »ç¿ëÇÏ´Â »óÈ²¿¡¼­ ´Ù¸¥ µ¥¿¡¼­ ±Ü¾î¿Â °ÍµéÀ» ÇÑµ¥ ÇÕÄ¥ ¼ö ÀÖÀ¸¹Ç·Î AddUnique ÇÔ.
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

// ¿ùµå Àç·Îµù ¾øÀÌ SpawnPool À» Ã³À½ºÎÅÍ ´Ù½Ã »ç¿ë °¡´ÉÇÏµµ·Ï. ´Ü, ÀÌ°Í¸¸ ÇÑ´Ù°í ÇØ¼­ ·¹º§ ÀüÃ¼°¡ ¸®¼ÂµÇ´Â °Ç ¾Æ´Ô. ´Ù¸¥ °Íµéµµ Ã³¸®ÇØ¾ß ÇÒ °Í.
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

	TArray<FMonsterSpawnPoolSummonSettings> MatchingSettings; // SummonIndex ¿¡ ´ëÇÑ ¸ðµç ¼¼ÆÃ ¸®½ºÆ®
	GetAllStageSummonSetting(MatchingSettings, CurrentClientStageId, (EStageDifficulty)CurrentDifficultyLevel, SummonIndex);

	if (MatchingSettings.Num() == 0 || Summoner == NULL || bAllWaveDoneForCurrentStage)
	{
		return false;
	}

	for (int32 SI = 0; SI < MatchingSettings.Num(); ++SI)
	{
		FMonsterSpawnPoolSummonSettings& CurrSetting = MatchingSettings[SI];
		// TransientMySettingIndex ¿¡ ¿ø·¡ SummonSettings ³»¿¡¼­ÀÇ ÀÎµ¦½º°¡ µé¾î°¡ ÀÖÀ½.
		check(CurrSetting.TransientMySettingIndex >= 0)

		for (int32 MI = 0; MI < CurrSetting.SpawnNum; ++MI) // RandomSlack °°Àº °Ô µé¾î°¥ ¼öµµ..
		{			
			if (CountSummonedMobsBySetting(CurrSetting.TransientMySettingIndex) >= CurrSetting.MaxObjectByThisSetting){
				continue; // Çö ¼¼ÆÃ¿¡¼­ÀÇ ÃÖ´ë summon ÇÑµµ¿¡ µµ´ÞÇÑ °æ¿ì.
			}

			ABladeIICharacter* NewSummoned = SummonSingleMob(Summoner, CurrSetting, MI, MaxDistance);
			if (NewSummoned == NULL){
				continue;
			}

			// (¾Æ¸¶µµ StageManager ÂÊÀ¸·Î) ÅëÁö. ¼ÒÈ¯µÈ °æ¿ì´Â WaveNum °°Àº °Ô ÀÇ¹Ì¾ø°í bIsSummoned Á¤µµ¸¸. ¹°·Ð boss ÀÏ¸®µµ ¾ø°ÚÁö. ¤»
			SpawnPoolMobSpawnClass<ABladeIICharacter*, const FMobSpawnedBirthplaceInfo&>::GetInstance().Signal(NewSummoned, FMobSpawnedBirthplaceInfo(this, -1, -1, false, true));

			FMonsterSpawnPoolSummonedFellowState NewState;
			NewState.SummonSettingIndex = CurrSetting.TransientMySettingIndex; // ¸î¹ø ¼¼ÆÃ¿¡ ÀÇÇØ spawn µÇ¾ú´ÂÁö ±â·ÏÇØ¼­ ÃßÈÄ summon Á¦ÇÑ ½Ã »ç¿ë.
			NewState.Summoner = Summoner;
			NewState.Summonee = NewSummoned;
			SummonedFellows.Add(NewState);
			
			if (!NewSummoned->IsAlive())
			{ // SpawningNoDamageGuard ¸¦ ÇÔ¿¡µµ ºÒ±¸ÇÏ°í Ãâ»ýµî·Ï ÀÌÀü¿¡ Á×´Â °æ¿ì°¡ ¹ß»ýÇÏ´Â °Å °°À½. ÀÌ ½ÃÁ¡¿¡ Á×¾î ÀÖÀ¸¸é °­Á¦ Á¦°Å.
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

	// SummonerLocation ÁÖº¯À¸·Î ¹«ÀÛÀ§ ¹æÇâ¿¡, MaxDistance °Å¸® ÀÌ³»·Î Spawn location À» Á¤ÇÔ.
	FVector2D RandSpawnDir(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));
	if (FMath::Abs(RandSpawnDir.X) <= KINDA_SMALL_NUMBER && FMath::Abs(RandSpawnDir.Y) <= KINDA_SMALL_NUMBER)
	{
		// ÀÌ·± °æ¿ì°¡ ¾ó¸¶³ª ÀÚÁÖ ³ª¿ÃÁö ¸ð¸£°ÚÁö¸¸ ÈçÇÒ °Å °°Áø ¾ÊÀ¸´Ï Àû´çÈ÷ ¿¹¿ÜÃ³¸®.
		RandSpawnDir.X = FMath::FRandRange(0.1f, 1.0f);
	}
	RandSpawnDir.Normalize();
	RandSpawnDir *= FMath::RandRange(MaxDistance * 0.1f, MaxDistance);

	FTransform SpawnTransform(Summoner->GetActorLocation() + FVector(RandSpawnDir.X, RandSpawnDir.Y, 100.0f)); // È¤½Ã¶óµµ ³¢ÀÏ ¼ÒÁö¸¦ ÁÙÀÌ±â À§ÇØ ¾à°£ ³ô°Ô spawn ½ÃÅ² ÈÄ ¶¥¿¡ snap.

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
		// °â»ç°â»ç Ã¼Å©Á»..
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
							// ·¹º§ È¯°æ¿¡ µû¶ó µµÀúÈ÷ (modulated) dynamic shadow ¸¦ ¸ø ¾µ È¯°æÀÌ¸é ²¨ ÁØ´Ù.. ¾Æ½±Áö¸¸.
							TurnOffMeshComponentsDyamicShadowForModulated(NewMonster);
						}
	
						// (¾Æ¸¶µµ StageManager ÂÊÀ¸·Î) ÅëÁö.
						// ¿¡µðÅÍ¿¡¼­ º» ¿ÀºêÁ§Æ® ¹øÈ£¸¦ ÀÎ°ÔÀÓ¿ë ¼³Á¤¿¡¼­ »ç¿ëÇÏ´Â »óÈ²À¸·Î ÀÎÇØ ¿¡µðÅÍ¿ë SetRepresentingInfo È£Ãâ½Ã¿Í °°Àº ¼ø¼­ÀÇ spawn object ÀÎµ¦½º¸¦ ³Ö¾îÁÜ.
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
							// NavMesh °¡ Á¦´ë·Î ºôµåµÇ¾ú´õ¶óµµ ¸÷ÀÌ ³Ê¹« Å©°Å³ª ÇÏ¸é ¿©ÇÏ°£ ½ÇÆÐÇÏ±â ÁÁ´Ù. Áß¿äÇÒ °¡´É¼ºÀÌ ÀÖ´Â ¼³Á¤ÀÌ¶ó¸é spawn À» °è¼Ó.
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
							{ // SpawningNoDamageGuard ¸¦ ÇÔ¿¡µµ ºÒ±¸ÇÏ°í Ãâ»ýµî·Ï ÀÌÀü¿¡ Á×´Â °æ¿ì°¡ ¹ß»ýÇÏ´Â °Å °°À½. ÀÌ ½ÃÁ¡¿¡ Á×¾î ÀÖÀ¸¸é °­Á¦ Á¦°Å.
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
					// ¿©±â¼­ ÇÏÁö ¾ÊÀ¸¸é ¹Û¿¡¼­ µû·Î È£ÃâÇØ¾ß ÇÔ
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
	SpawnInfo.bDeferConstruction = true; // Need deferred construction and manually call FinishSpawning. AutoPossessAI ¼³Á¤ ¹× NPCClassInfo Àû¿ëÀ» À§ÇØ DeferConstruction ÀÌ ÇÊ¿ä.
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
		NewMonster->BeginSpawningNoDamageGuard(); // ÇÁ·ÎÁ§Å¸ÀÏÀÌ ±× ÀÚ¸®¿¡ ÀÖ´Ù°Å³ª ÇÏ¸é FinishSpawning µµÁß¿¡ ¸Â¾Æ¼­ Á×À» ¼ö ÀÖ°í ±×·¸°Ô µÇ¸é death ½Ã Ã³¸®°¡ ¾ÈµÇ¹Ç·Î ¸·´Â´Ù.
		if (InClassParam.bOutBossMob)
		{
			NewMonster->CharacterType = ECharacterType::ECT_BossMob; // µüÈ÷ ÀÇ¹Ì°¡ ÀÖÀ¸·Á³ª ¸ð¸£°ÚÁö¸¸ ¤»
		}
		else if (InClassParam.OutCharacterType != ECharacterType::ECT_End)
		{
			// NPCClassInfo ¸¦ ÅëÇØ ¼¼ÆÃÇÑ ±âº» CharacterType °ú MonsterClassTypeMap À» ÅëÇÑ ¸ÊÇÎÀº ´Ù¸¦ ¼ö ÀÖÀ¸¹Ç·Î ÃÖÁ¾ÀûÀ¸·Î spawn ¿¡ »ç¿ëÇÑ MonsterClassTypeMap ¸ÊÇÎ°ªÀ» ÇÒ´ç.
			NewMonster->CharacterType = InClassParam.OutCharacterType;
		}
		if (InClassParam.bOutFixedLocation)
		{
			UCharacterMovementComponent* CharMoveComp = NewMonster->GetCharacterMovement();
			if (CharMoveComp)
			{
				// ½ÇÉ«³¯ÏòÓÉ¿ØÖÆÆ÷¿ØÖÆ£¬¶ø²»ÊÇÒÆ¶¯·½Ïò
				CharMoveComp->bOrientRotationToMovement = false;
				NewMonster->bUseControllerRotationYaw = true;
			}
		}

		// NPCClassInfo ¿¡¼­µµ ÇÏ´Â °Çµ¥ ¿©±â¼­µµ material override.
		OverrideMaterialForNewSpawned(NewMonster, InClassParam.MaterialOverrides);

		// Need to set this to let SpawnDefaultController be called. Ãß°¡·Î ÀÌ·¸°Ô FinishSpawning À» delay ½ÃÅ´À¸·Î¼­ NPCClassInfo ¼¼ÆÃ ÈÄ BeginPlay °¡ ºÒ¸®´Â È¿°ú°¡ ÀÖÀ½.
		NewMonster->AutoPossessAI = EAutoPossessAI::Spawned;
		NewMonster->FinishSpawning(InSpawnTransform);

		if (InClassParam.bOutBossMob && InClassParam.OutClassEnum != ENPCClass::ENC_End)
		{// BossMob ¼¼ÆÃÀÎ °æ¿ì´Â UI Doc ¿¡µµ ¾÷µ¥ÀÌÆ® ÇØ ÁÙ °ÍÀÌ Á» ÀÖÀ½.
			UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
			if (DocBattle)
			{
				// SetbCurrentlyBossAppeared ¸¦ ÇÏ±â Àü¿¡ MobClass ¿Í Variation À» ¸ÕÀú ¼¼ÆÃÇØ¾ß ¾û¶×ÇÑ NPCClassInfo ·ÎµùÀ» ÇÏÁö ¾Ê´Â´Ù.
				DocBattle->SetBossMobClass(InClassParam.OutClassEnum);
				DocBattle->SetBossMobClassVariation(InClassParam.OutClassVariation);
				DocBattle->SetbCurrentlyBossAppeared(true); // Á×À» ¶§ ´Ù½Ã ¸®¼ÂÇØ¾ß ÇÔ.
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
		check(!InEditorMeshComp); // ¿À¿ë ¹æÁö.
	}
#endif

	USkeletalMeshComponent* MeshComp =
#if WITH_EDITOR
		InEditorMeshComp ? InEditorMeshComp : // ¿¡µðÅÍ ÇÁ¸®ºä ¸ñÀûÀ¸·Î Component ¸¦ Á÷Á¢ Àü´ÞÇÏ´Â °æ¿ì ±×°É »ç¿ë.
#endif
		(NewSpawned ? NewSpawned->GetBaseMesh() : NULL); // Mob Àº Æ¯¼öÇÑ attachment ¿Ü¿¡ ±× ±âº»ÀÌ multi-component ±¸¼ºÀÌ ¾Æ´Ñ ÇÑÀº BaseMesh ¸¸ »ç¿ë.

	if (!MeshComp || MaterialOverrides.Num() == 0)
		return;

	int32 BaseMeshMaterialNum = MeshComp->GetNumMaterials();

	int32 ApplyMIIndex = 0;
	for (; ApplyMIIndex < FMath::Min(MaterialOverrides.Num(), BaseMeshMaterialNum); ++ApplyMIIndex)
	{
		if (MaterialOverrides[ApplyMIIndex]) // Null ¼¼ÆÃÀ» ÅëÇØ Æ¯Á¤ ¼½¼ÇÀ» ¶Ù¾î³ÑÀ» ¼ö ÀÖÀ½.
		{
			MeshComp->SetMaterial(ApplyMIIndex, MaterialOverrides[ApplyMIIndex]);
		}
	}

	// ±âº» element °³¼öº¸´Ù MaterialOverrides ¸¦ ¸¹ÀÌ ³Ö¾ú´Ù¸é ¼ø¼­´ë·Î attach µÈ ÄÄÆ÷³ÍÆ®¿¡ Àû¿ë.
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
				{ // ¿ª½Ã³ª NULL ·Î µÎ¸é Æ¯Á¤ attach ´Â ¶Ù¾î³ÑÀ» ¼ö ÀÖÀ½.
// Attach µÈ °ÍÀº ±»ÀÌ element ÀÎµ¦½º ±îÁö °¡Áö ¾Ê´Â´Ù. ±×°Í±îÁö ÇÊ¿äÇÏ´Ù¸é Áö³ªÄ£ ¿ä±¸ÀÏ µí.
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
	OutParams.OutCharacterType = ECharacterType::ECT_End; // CharacterType À¸·Î spawn ÇÏµµ·Ï ÇÑ °æ¿ì¿¡¸¸ °ªÀÌ ¼¼ÆÃµÉ °Í.

	if (InWaveSetting == NULL && InSummonSetting == NULL)
	{
		return false;
	}

	// WaveSetting È¤Àº SummonSetting ¿¡¼­ ¼³Á¤À» »© ¿È. WaveSetting ÀÌ ¿ì¼±.
	const TArray<FMonsterSpawnClassInfo>& SpawnClassSetting = InWaveSetting ? InWaveSetting->SpawnClasses : InSummonSetting->SpawnClasses;
	const TArray<FMonsterSpawnTypeSpecific>& SpawnTypeSetting = InWaveSetting ? InWaveSetting->SpawnCharacterTypes : InSummonSetting->SpawnCharacterTypes;

	// Àç»ç¿ë °¡´ÉÇÑ wave ÀÎ °æ¿ì, °ñµå¸¦ µå¶øÇÏÁö ¾Ê°ÔÇÑ´Ù. ¼ÒÈ¯ÀÎ °æ¿ìµµ ¸¶Âù°¡Áö.
	const bool bNonGoldDropSetting = (InWaveSetting && InWaveSetting->bCanReuseWave) || (InWaveSetting == NULL && InSummonSetting != NULL);

	if (SpawnClassSetting.Num()) // Class Á÷Á¢ ÁöÁ¤ÀÌ ¿ì¼±.
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
			//Àç»ç¿ë °¡´ÉÇÑ ½ºÆùÇ®ÀÎ °æ¿ì, °ñµå¸¦ µå¶øÇÏÁö ¾Ê°ÔÇÑ´Ù. ¼ÒÈ¯ÀÎ °æ¿ìµµ ¸¶Âù°¡Áö.
			OutParams.OutWeightRewardGold = bNonGoldDropSetting ? 0 : FinalInfo.RewardGoldWeight;
			OutParams.OutCanDropItem = FinalInfo.bCanDropItem;
			OutParams.OutNetPlay = FinalInfo.bNetPlay;
			OutParams.MaterialOverrides = FinalInfo.MaterialOverrides;
			OutParams.bOutFixedLocation = FinalInfo.bFixedLocation;

			// ÀÌ°Ç Å¬·¡½º ¼¼ÆÃ¿¡ µé¾î°£ °Ç ¾Æ´Ñµ¥ ¿©ÇÏ°£ ¿©±â¿¡ ³Ö¾îµÎ¸é ½á¸Ô±â ÁÁ¾Æ¼­. ±Ùµ¥ bBossWave ÀÚÃ¼°¡ WaveSetting º¸´Ù´Â Å¬·¡½º ¼¼ÆÃ¿¡ ÀÖ¾î¾ß ¾î¿ï¸± ¸¸ÇÑ ¼º°ÝÀÌ±âµµ ÇÏ´Ù.
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
					OutParams.OutCharacterType = FinalInfo.CharacterType; // Spawn ÈÄ CharacterType °ªÀ» µ¤¾î¾²µµ·Ï °á°ú ÆÄ¶ó¹ÌÅÍ·Î Àü´Þ.
					//Àç»ç¿ë °¡´ÉÇÑ ½ºÆùÇ®ÀÎ °æ¿ì, °ñµå¸¦ µå¶øÇÏÁö ¾Ê°ÔÇÑ´Ù.
					OutParams.OutWeightRewardGold = bNonGoldDropSetting ? 0 : SpawnCharacterTypeForThis->RewardGoldWeight;
					OutParams.OutCanDropItem = SpawnCharacterTypeForThis->bCanDropItem;
					OutParams.MaterialOverrides = FinalInfo.MaterialOverrides;
					OutParams.bOutBossMob = InWaveSetting ? InWaveSetting->bBossWave : false; // ÀÌ°Ç Å¬·¡½º ¼¼ÆÃ¿¡ µé¾î°£ °Ç ¾Æ´Ñµ¥ ¿©ÇÏ°£ ¿©±â¿¡ ³Ö¾îµÎ¸é ½á¸Ô±â ÁÁ¾Æ¼­
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
		// SpawnArea ´Â ShapeComponent ¿©¾ß ÇÔ.
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

	ExecuteWaveEventsCommonInternal(WaveNum, EWaveCustomEventTiming::EWCET_WaveStart); // °¢Á¾ ºÎ°¡ ÀÌº¥Æ®µé

	// ÇíÇí ¿©±âµµ ÀÌº¥Æ® ÅëÁö
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
			// ½ÇÁ¦·Î DyingMonster °¡ ¾ø´Â Æ¯¼ö ÄÉÀÌ½ºÀÌ¹Ç·Î ´Ù¸¥ °÷À» °ÅÄ¡Áö ¾Ê°í OnSpawnedMonsterDead ¸¦ Á÷Á¢ call ÇÏ´Âµ¥ ¸¸ÀÏ ÇÊ¿äÇÏ´Ù¸é SpawnPoolWaveMobDeadClass ¸¦ »ç¿ëÇÏ¿© ÀÌº¥Æ® broadcast ¸¦ ÇÏµµ·Ï.
			OnSpawnedMonsterDead(NULL, WaveNum, -1, false);
		}
	}
}
void AB2MonsterSpawnPool::ExecuteWaveEventsCommonInternal(int32 WaveNum, EWaveCustomEventTiming InTiming)
{
	// ¼³Á¤ÇÑ Component µé On/Off, ¾×ÅÍ³ª ·¹º§·ÎÀÇ ½ÅÈ£ µî..
	// Wave ½ÃÀÛ È¤Àº ¿Ï·á ½Ã

	TArray<FMonsterSpawnPoolWaveLevelStreamingSettings> LevelStreamingSettings;
	GetWaveLevelStreamingSettings(LevelStreamingSettings, InTiming, WaveNum);
	for (FMonsterSpawnPoolWaveLevelStreamingSettings& ThisSetting : LevelStreamingSettings)
	{
		// ¼³Á¤¿¡ µû¶ó Áï¼®¿¡¼­ Async ·Îµù Flush °¡ µÉ ¼ö ÀÖÀ½. ¾Æ´Ï¸é WaveLevelStreamingFlushCB ¿¡¼­.
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
					SummonedFellows.RemoveAt(SI); // ¼ÒÈ¯ ¸÷ ¸®½ºÆ®´Â µû·Î. »ç½Ç Summonee ´Â unique ÇØ¾ß ÇÔ.
					--SI;
				}
			}
		}
		else
		{
	#if !UE_BUILD_SHIPPING
			if (DyingMonster) // DyingMonster ÀÚÃ¼´Â NULL ÀÏ ¼ö ÀÖÀ¸´Ï °Å±â¿¡ check ¸¦ °ÉÁö´Â ¾Ê´Â´Ù.
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
	
			// DeadMobWaveNum À» º¼ ¼öµµ ÀÖ°ÚÁö¸¸ ¸ðµÎ µ¹¾ÆÁÖÀÚ.
			for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI)
			{
				RuntimeWaveStates[WI].FellowsInThisWave.Remove(DyingMonster);
			}
	
			if (DyingMonster)
			{
				TArray<ABladeIICharacter*> RelevantSummoneeList; // Summoner Áß ÇÏ³ª°¡ Á×¾úÀ» ¶§ °Å±â¼­ ¼ÒÈ¯µÈ Summonee µéÀ» Ã³¸®ÇÏ±â À§ÇÑ ¸®½ºÆ®. Summonee Ã³¸® µµÁß SummonedFellows °¡ ºñ¿öÁú °ÍÀÌ¹Ç·Î µû·Î º¹»ç.
				for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
				{
					FMonsterSpawnPoolSummonedFellowState& CurrState = SummonedFellows[SI];
					if (CurrState.Summoner == DyingMonster)
					{
						// ÇØ´ç Summoner ¿¡¼­ ¼ÒÈ¯µÈ Summonee ¸®½ºÆ®¸¦ µû·Î ÃëÇÕÇØ¼­ Á¦°Å.
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
						CurrSummonee->Suicide(); // ÀÌ°É ÅëÇØ¼­µµ OnSpawnedMonsterDead °¡ ºÒ¸®¸é¼­ »ó´Ü bIsSummonedMob ÂÊÀ¸·Î Ã³¸®µÉ °Í.
					}
				}
			}
		}
	
		EnsureCleanFellowList();
	
		if (DyingMonster && DyingMonster->IsSpawnedAsBossMob())
		{
			UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
			if (DocBattle) // BossMob °ü·Ã UI Doc »óÅÂ ¸®¼Â.
			{
				DocBattle->SetbCurrentlyBossAppeared(false);
			}
		}
	
		// Don't do the completion check and spawn handling if we are clearing, as that can possibly cause recursive call.
		// ¼ÒÈ¯µÈ ¸÷ÀÇ °æ¿ìµµ CheckWaveCompletion À» ÇÏÁö ¾Ê´Âµ¥, ÄÚµå ÆÐ¾²°¡ º¹ÀâÇØ Áú ¼ö ÀÖÀ½. µüÈ÷ ÀÌ»óµ¿ÀÛÀ» ÇÏÁö´Â ¾ÊÁö¸¸ ForceClearWaves °¡ ºÒ¸± »óÈ²ÀÌ¶ó¸é ¼ÒÈ¯¸÷À» Á×ÀÌ´Â ½Ãµµ¸¦ µÎ ¹ø ÇÒ ¼ö ÀÖ¾î¼­ Çò°¥¸².
		// ¼ÒÈ¯ÀÚ°¡ Á×À» ¶§ ¼ÒÈ¯ÇÑ ¸÷ Ã³¸®¸¦ ¹Ù·Î À§¿¡¼­ ÇÏ¹Ç·Î CheckWaveCompletion ÀÌ µé¾î´Â °¥ °ÍÀÌ´Ù.
		if (!bForceClearing && !bIsSummonedMob)
		{
			// Let's see if we have kicked shit out of here, as well as check for new wave start.
			CheckWaveCompletion();
		}
	
	#if !UE_BUILD_SHIPPING
		extern bool GCheat_HeroineMode;
		if (GCheat_HeroineMode && !bForceClearing)
		{ // ÇöÀç spawn µÈ ¸ðµç ¸÷µé ÁË´Ù ¾µ±â
	
			// ForceClearWaves ¾ÕµÚ·Î wave ½ÃÀÛ »óÅÂ¸¦ ÀúÀåÇß´Ù°¡ º¹±¸ÇÑ´Ù. Heroine ¸ðµåÀÇ ¸ñÀûÀº wave ¸¦ ³¡³»´Â °Ô ¾Æ´Ï¶ó ÇöÀç mob µéÀ» Á×ÀÌ´Â °ÍÀÌ¹Ç·Î.
			TArray<FMonsterSpawnPoolRuntimeWaveState> WaveStateCopy = RuntimeWaveStates;
			ForceClearWaves(-1, false); // MarkCompleted ¸¦ ÇØ¼± ¾ÈµÊ.
			for (int32 WI = 0; WI < RuntimeWaveStates.Num(); ++WI){
				RuntimeWaveStates[WI].bWaveStarted = WaveStateCopy[WI].bWaveStarted;
			}
			bForceClearing = false;
			CheckWaveCompletion(); // ³²Àº ¾Öµé ¾µ¾î´ã´Â ¿ÍÁß¿¡´Â CheckWaveCompletion Ã³¸®°¡ ´©¶ôµÉ °Í.
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
				for (ABladeIICharacter* CheckFellow : CurrState.FellowsInThisWave){
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
	
		if (SummonedFellows.Num() > 0) // ¼ÒÈ¯¸÷µéµµ ¸ðµÎ Ã³¸®ÇØ¾ß ÇÔ. ¤»
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
	// ÈçÇÑ ÀÏÀº ¾Æ´ÏÁö¸¸ °³¹ß µµÁßÀÇ »çÁ¤ È¤Àº Àß ¾ËÁö ¸øÇÏ´Â ¹®Á¦·Î ÀÎÇØ invalid ÇÑ »óÅÂ·Î ¸®½ºÆ®¿¡ ÀÖ´Â °æ¿ì°¡ ÀÖ´Ù..
	// Assert ¹× ¾ÈÀüÈ®º¸ Â÷¿ø¿¡¼­ ¸®½ºÆ®¿¡¼­ ºñ¿ò.
	for (int32 SI = 0; SI < SummonedFellows.Num(); ++SI)
	{
		FMonsterSpawnPoolSummonedFellowState& CurrState = SummonedFellows[SI];
		if (!IsValid(CurrState.Summonee))
		{ // ³»ºÎÀûÀ¸·Î´Â ¾Ë ¼ö ÀÖ°Ô.
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
		ExecuteWaveEventsCommonInternal(WaveNum, EWaveCustomEventTiming::EWCET_WaveComplete); // °¢Á¾ ºÎ°¡ ÀÌº¥Æ®µé

		// ÇíÇí ¿©±âµµ ÀÌº¥Æ® ÅëÁö
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
		// MeshComponent ÀÎ °æ¿ì¶ó¸é ´«¿¡ º¸ÀÌ´Â wall ·Î °£ÁÖ
		if (CastedMeshComp)
		{
			CastedMeshComp->SetCollisionProfileName(CollisionProfileName_BlockAll);
		}

		ComponentTobeEnable->SetVisibility(true); // ÀÌ °æ¿ì´Â ¸ðµÎ visibility ¸¦ ÄÑ ÁÜ.
		ComponentTobeEnable->SetHiddenInGame(false); // ÀÌ°Ô ¼¼ÆÃµÇ¾úÀ»¼öµµ ÀÖÀ¸´Ï..
	}
	else // Æ®¸®°Å ¿ë Collision Component ·Î °£ÁÖ.
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

	// ÀÏ´Ü °øÅëÀûÀ¸·Î no collision ¿¡ invisible
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
	
		// ¼ÒÈ¯¸÷µéÀº DesiredFinishWaveNum °ú´Â »ó°ü¾øÀÌ ¸ðµÎ ³¯¸². DesiredFinishWaveNum ÀÌ »ç½Ç»ó °³¹ß¿ë ±â´ÉÀÌ±âµµ ÇÏ°í, ÀÌÈÄ wave ÀÇ ¼ÒÈ¯¸÷ÀÌ ³ª¿À´Â °Íµµ ¸»ÀÌ ¾ÈµÊ ³¥³¥
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
		checkSlow(SummonedFellows.Num() == 0); // Suicide ¿¡¼­ OnSpawnedMonsterDead °ÅÄ¡¸é¼­ ºñ¿öÁ³À» °Í.
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
		UGameplayStatics::FlushLevelStreaming(this); // ÀÌ°Å ÇÏ·Á°í Å¸ÀÌ¸Ó °É¾î³õÀº °Å.
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
				// Áï¼®¿¡¼­ SpawnWave ¸¦ ÇÑ °æ¿ì OnSingleWaveStarted ¸¦ ¸ð¾Æ¼­ Ã³¸®ÇÏ±â À§ÇØ SpawnedWaves ¿¡ ¹øÈ£ Ãß°¡.
				SpawnedWaves.Add(WI);
			}
		}
	}

	for (int32 SWI = 0; SWI < SpawnedWaves.Num(); ++SWI)
	{
		OnSingleWaveStarted(SpawnedWaves[SWI]); // SpawnWave È£Ãâ½Ã Áö¿¬½ÃÄ×´ø OnSingleWaveStarted ÀÏ°ý Ã³¸®
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
					// Áï¼®¿¡¼­ SpawnWave ¸¦ ÇÑ °æ¿ì OnSingleWaveStarted ¸¦ ¸ð¾Æ¼­ Ã³¸®ÇÏ±â À§ÇØ SpawnedWaves ¿¡ ¹øÈ£ Ãß°¡.
					SpawnedWaves.Add(WI);
				}
			}
		}
	}
	for (int32 SWI = 0; SWI < SpawnedWaves.Num(); ++SWI)
	{
		OnSingleWaveStarted(SpawnedWaves[SWI]); // SpawnWave È£Ãâ½Ã Áö¿¬½ÃÄ×´ø OnSingleWaveStarted ÀÏ°ý Ã³¸®
	}
}

void AB2MonsterSpawnPool::DelayedNewWaveSpawn(int32 WaveNum)
{
	// Å¸ÀÌ¸Ó Å¬¸®¾î
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
{ // SpawnDelay ¿¡ µû¶ó DelayedNewWaveSpawn ¿¹¾à, È¤Àº SpawnWave È£ÃâÀ» ÇÏ´Â °øÅëµÈ ÄÚµå ºí·Ï.
	// Áï¼®¿¡¼­ SpawnWave ¸¦ ÇÏ°Ô µÇ¸é return true ÇØ¼­ ÀÌ¹ø WaveIndex ¿¡ ´ëÇØ ¸ðÁ¾ÀÇ Ã³¸®¸¦ ÇÏµµ·Ï ÇÔ.
	if (ThisWaveSetting)
	{
		const float EffectiveSpawnDelay = ThisWaveSetting->GetFinalSpawnDelay(CachedStageManager ? CachedStageManager->IsScenarioMode() : false);
		if (EffectiveSpawnDelay > 0.0f)
		{
			FTimerManager& TheTimerManager = GetWorldTimerManager();
			// ±âÁ¸¿¡ È¤¿©¶óµµ ¼³Ä¡µÇ¾î ÀÖ´Â Å¸ÀÌ¸Ó°¡ ÀÖ´Ù¸é Á¦°Å.
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
			return true;  // ¸®ÅÏ°ªÀº ´ÜÁö SpawnWave ÄÝ ½Ã¿¡ WaveIndex µé ¸ð¾Æ¼­ OnSingleWaveStarted Ã³¸® ÇÏ·Á´Â °Å.
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

		if(bFromQTEVehicle)
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
							// Áï¼®¿¡¼­ SpawnWave ¸¦ ÇÑ °æ¿ì OnSingleWaveStarted ¸¦ ¸ð¾Æ¼­ Ã³¸®ÇÏ±â À§ÇØ SpawnedWaves ¿¡ ¹øÈ£ Ãß°¡.
							SpawnedWaves.Add(WI);
						}
						break;
					}
				}
			}
		}
		for (int32 SWI = 0; SWI < SpawnedWaves.Num(); ++SWI)
		{
			OnSingleWaveStarted(SpawnedWaves[SWI]); // SpawnWave È£Ãâ½Ã Áö¿¬½ÃÄ×´ø OnSingleWaveStarted ÀÏ°ý Ã³¸®
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
				ThisSetting.bCheckForOnOrOff == true && ThisSetting.ComponentName == FName(*InComp->GetName()) // ÄÑÁöµµ·Ï µÇ¾î ÀÖ´Â °Í¸¸.
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
			// bOnlyLiveMobs °¡ false ÀÎ °æ¿ì¶óµµ ½ÇÁ¦·Î Dying »óÅÂÀÎ °ÍÀ» °¡Á®¿Ã ¼ö ÀÖ´Â Å¸ÀÌ¹ÖÀº ¸¹Áö ¾Ê´Ù.
			// OnSpawnedMonsterDead Âü°í.
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

	// ¸÷ ¸®½ºÆ®ÀÇ OldOne ¿¡ ÇØ´çÇÏ´Â ¿ä¼Ò¸¦ NewOne À¸·Î ¹Ù²ãÄ§.
	if (OldOne->IsSummonedMob())
	{
		// Summoner µç, Summonee µç ´ë»óÀÌ µÉ ¼ö ÀÖ°Ú´Ù.
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
					// ¿©±â¼­ °Á break °É¾î ¹ö·Áµµ µÇ°ÚÁö¸¸..
				}
			}
		}
	}

	// Ãß°¡·Î NewOne ¿¡ ´ëÇÑ SetBirthPlace Ã³¸®. StageManager ¸¦ °ÅÄ¡Áö ¾Ê´Â Á÷Á¢ Ã³¸®ÀÎµ¥ ÇÊ¿äÇÏ°Ô µÇ¸é ±³Ã¼..
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
			{//GetSpawnClassFromSetting()ÀÇ ·£´ý¼º¿¡ ÀÇÇØ °è»ê°ªÀº ¸Å¹ø ¹Ù²ð ¼ö ÀÖ´Ù. ¿¹»óÄ¡ ¿ëµµ¶ó¼­ Å« ¹®Á¦´Â ¾øÀ» µí.
				for (int32 spawnIdx = 0; spawnIdx < WaveState.FinalSpawnNum; ++spawnIdx)
				{
					FSpawnClassFromSetting_OutParams OutParams;
					GetSpawnClassFromSetting(OutParams, WaveSettings, NULL, spawnIdx); //´Ù¸¥ ½¬¿î ÀÎÅÍÆäÀÌ½º´Â? -> ¾ø½À´Ï´Ù °í°´´Ô
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
		return; // ForceClearWaves ¿Í´Â ´Ù¸§.
	}

	// DesiredFinishWaveNum ±îÁö ½ÃÀÛÇÏÁö ¾ÊÀº wave °¡ ÀÖ´Ù¸é ½ÃÀÛ.
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

	// ForceClearWaves ¿¡¼­ ±×³É Á×ÀÌ±â¸¸ ÇÏ°í ¿©Å¸ Ã³¸®µéÀº CheckWaveCompletion À» ÅëÇØ¼­..
	ForceClearWaves(DesiredFinishWaveNum, false);
	bForceClearing = false; // À¸ÀÌ±¸.. ¿¡µðÅÍ ±â´ÉÀÌ´Ï ÀÌ ÇãÁ¢ÇÔÀ» ³Ñ±ä´Ù..

	CheckWaveCompletion(); // °Á ÀÌ°Å ÇÑ¹ø ºÒ·¯ÁÖ¸é OnSingleWaveComplete Ã³¸®µµ µÉ µí.
}

void AB2MonsterSpawnPool::ProceedOnEventIgnoringCheat()
{ // µé¾î¿À´Â »óÈ²Àº ÀÌº¥Æ® ¾øÀÌ ¾ÖµéÀ» µû·Î Á×ÀÎ ´ÙÀ½ wave ³¡³µ´ÂÁö Ã¼Å©ÇÏ±â À§ÇÔ.
	// OnSpawnedMonsterDead ÀÇ ÇÊ¿ä Ã³¸®¸¦ Á÷Á¢.
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

	SummonedFellows.Empty(); // ¼ÒÈ¯ ¸®½ºÆ®´Â °Á ¸ðµÎ ºñ¿ò.

	CheckWaveCompletion();
}

#endif

#if WITH_EDITOR && WITH_EDITORONLY_DATA

void AB2MonsterSpawnPool::RegenerateEditingActors()
{
	UPackage* OutermostPkg = GetOutermost();
	// ¸ÊÀÎ °æ¿ì¸¸ »ý¼ºÇØ¾ß ÇÔ. ºí·çÇÁ¸°Æ®·Î ÀúÀåÇÑ ¾Öµµ Ã³À½¿£ ¸ÊÀ¸·Î ÀÎ½ÄµÉ °Í.
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
				// SpawnAreaComp ´Â ShapeComponent
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
				// SpawnAreaComp ´Â ShapeComponent
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
	if(GIsEditor && WaveEditingActors.Num() && WaveEditingActors[0] != NULL) // Double check for GIsEditor.
	{
		TArray<UActorComponent*> AllComps;
		GetComponents(AllComps);

		// Before we go on, remove previously created text render components..
		// For this, it is a little bit harder to remove only unnecessary stuff, so just remove all and recreate..
		for(int32 EI = 0; EI < WaveEditingActors.Num(); ++EI)
		{
			if(WaveEditingActors[EI])
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
					continue; // ¿¡µðÅÍ ¾ÆÀÌÄÜ ½ºÇÁ¶óÀÌÆ® ÀÌ¸§ ¶ß´Â °Å ÇÊ¿äµµ ¾ø°í ±¦È÷ °Å½½¸².
				}

				WaveEditingActors[0]->SetComponentToDisplayInfo(CurrComp);
			}
		}
	}
}

bool AB2MonsterSpawnPool::EditorGetSpawnClassOfWaveMob(ENPCClass& OutClassEnum, ENPCClassVariation& OutClassVariation, TSubclassOf<class ABladeIICharacter>& OutClassBP, int32 InWaveNum, int32 InObjIndex)
{
	// ÀÌ°Ç »ç½Ç»ó ¿¡µðÅÍ ¿ëµµ¶ó EditorStageNumber ¸¦ »ç¿ë
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
			if (SpawnClassParam.OutClassEnum != ENPCClass::ENC_End || SpawnClassParam.OutClassBP != NULL) // À§¿¡¼­ true ¸®ÅÏÀÌ¸é ÀÌ·¡¾ß ÇÏ°ÚÁö¸¸ ÇÑ¹ø ´õ Ã¼Å©
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
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("[B2MonsterSpawnPool] [NOTE] ÁÖÀÇ»çÇ×:\nWaveCompl_TerminateAll ¿¡ µû¶ó ´Ù¸¥ wave °¡ °­Á¦ Á¾·áµÉ ¶§¿¡´Â wave Á¾·á¿¡ µû¸¥ ÀÌº¥Æ® È£ÃâÀÌ »ý·«µË´Ï´Ù.")));
#endif
		}
	}

	if (PropertyName == Name_DisableAllSpawn)
	{
		if (bDisableAllSpawn == true)
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("[B2MonsterSpawnPool] DisableAllSpawn Àº Å×½ºÆ® ¿ëµµ·Î Á¦°øµË´Ï´Ù. ÃÖÁ¾ ¹èÆ÷ÆÇ ºôµå¿¡¼­´Â µ¿ÀÛÇÏÁö ¾ÊÀ» °ÍÀÔ´Ï´Ù.")));
#endif
		}
	}

	if (/*PropertyName == Name_PerStageSettings || */PropertyName == Name_TargetStageNum || PropertyName == Name_TargetDifficultyLevel)
	{
		// ³ª¸¦ Á¦¿ÜÇÑ SpawnPool ¸®½ºÆ® ¼öÁý. ´Ù¸¥ ¾Öµé°Íµµ Ã¼Å©ÇØ¾ß.
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
		FString WarnMsgStr(TEXT("[Warning] Áßº¹µÈ TargetStageNum / DifficultyLevel ¼¼ÆÃ:\n\n"));
		for (int32 SIA = 0; SIA < PerStageSettings.Num(); ++SIA)
		{
			int32 CurrTargetStageNum = PerStageSettings[SIA].TargetStageNum;
			int32 CurrDifficultyLevel = PerStageSettings[SIA].TargetDifficultyLevel;

			for (int32 SIB = SIA + 1; SIB < PerStageSettings.Num(); ++SIB)
			{
				if (CurrTargetStageNum == PerStageSettings[SIB].TargetStageNum && CurrDifficultyLevel == PerStageSettings[SIB].TargetDifficultyLevel)
				{
					WarnMsgStr += FString::Printf(TEXT("SpawnPool %s, ÀÎµ¦½º %d and %d, TargetStage %d, TargetDifficulty %d\n"), *GetName(), SIA, SIB, CurrTargetStageNum, (int32)CurrDifficultyLevel);
					++AllFoundDuplicatedNum;
				}
			}

			// ±×¸®°í ´Ù¸¥ SpawnPool ¿¡¼­µµ..
			for (AB2MonsterSpawnPool* OtherOne : AllSPExceptMe)
			{
				for (int32 SIB = 0; SIB < OtherOne->PerStageSettings.Num(); ++SIB)
				{
					if (CurrTargetStageNum == OtherOne->PerStageSettings[SIB].TargetStageNum && CurrDifficultyLevel == OtherOne->PerStageSettings[SIB].TargetDifficultyLevel)
					{
						WarnMsgStr += FString::Printf(TEXT("SpawnPool %s, ÀÎµ¦½º %d and %d, TargetStage %d, TargetDifficulty %d\n"), *OtherOne->GetName(), SIA, SIB, CurrTargetStageNum, (int32)CurrDifficultyLevel);
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
		FString WarnMsgStr(TEXT("[Warning] Çã¿ëµÇÁö ¾Ê´Â ClassVariation ¼¼ÆÃ:\r\n\r\n"));
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

					// ÀÏ´Ü CounterDungeon ³ÖÁö ¾Ê°Ô ÇÏ·Á°í ÇÑ °Çµ¥ ±âÅ¸ ´Ù¸¥ °É Ãß°¡ÇÒ ¼öµµ..
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
		// Wave ½ÃÀÛ/Á¾·á ½Ã ÄÄÆ÷³ÍÆ® Enable ½ÃÅ°´Â ¼³Á¤Àº Á» Á¶½ÉÇØ¾ß ÇÔ. Àß¸øÇÏ¸é ±æÀÌ ¸·È÷±â ¶§¹®. ÀÌ¿¡ µû¸¥ °æ°í

		TArray<FString> UndesiredBeginEnableCompsSetting;
		TArray<FString> UndesiredComplEnableCompsSetting;

		for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
		{
			for (int32 WI = 0; WI < PerStageSettings[SI].WaveCompOnOffSettings.Num(); ++WI)
			{
				FMonsterSpawnPoolWaveComponentOnOffSettings& CurrCompSetting = PerStageSettings[SI].WaveCompOnOffSettings[WI];

				FMonsterSpawnPoolWaveSettings* TargetWaveSetting = (CurrCompSetting.WaveNum >= 0 && CurrCompSetting.WaveNum < PerStageSettings[SI].WaveSettings.Num()) ? &(PerStageSettings[SI].WaveSettings[CurrCompSetting.WaveNum]) : NULL;

				UMeshComponent* MeshComp = Cast<UMeshComponent>(FindComponentByName(CurrCompSetting.ComponentName));

				// Wave ½ÃÀÛ½Ã¿¡ MeshComponent ¸¦ ÄÑ´Â ¼³Á¤ÀÌ¶ó¸é WaveExec_TriggerComponent ÇÏ°í¸¸ »ç¿ëµÇ´Â °ÍÀÌ ¹Ù¶÷Á÷ÇÏ´Ù. ±×·¡¾ß wave ½ÃÀÛ ½Ã À§Ä¡¸¦ °­Á¦ÇÒ ¼ö ÀÖÀ¸¹Ç·Î.
				if (CurrCompSetting.EventTiming == EWaveCustomEventTiming::EWCET_WaveStart && CurrCompSetting.bCheckForOnOrOff == true && MeshComp && TargetWaveSetting &&
					(TargetWaveSetting->WaveExec_TriggerComponent == NAME_None || TargetWaveSetting->WaveExec_PostCompleteWaveNum >= 0)
					)
				{
					UndesiredBeginEnableCompsSetting.Add(FString::Printf(TEXT("Stage %d, WaveComp %d"), SI, WI));
				}

				// ¾î¶² °æ¿ìµç wave Á¾·á ½Ã¿¡ MeshComponent ¸¦ ÄÑµµ·Ï ÁöÁ¤ÇÏ´Â °Ô ¹Ù¶÷Á÷ÇÏÁö ¾Ê´Ù. ¾îµð¼­ wave °¡ ³¡³¯Áö ¸ð¸£¹Ç·Î.
				if (CurrCompSetting.EventTiming == EWaveCustomEventTiming::EWCET_WaveComplete && CurrCompSetting.bCheckForOnOrOff == true && MeshComp)
				{
					UndesiredComplEnableCompsSetting.Add(FString::Printf(TEXT("Stage %d, WaveComp %d"), SI, WI));
				}
			}
		}

		if (UndesiredBeginEnableCompsSetting.Num() > 0)
		{
			//FString WarnMessage = TEXT("WaveCompOnOffSettings ¿¡ Àß¸øµÈ ÇÃ·¹ÀÌ¸¦ À¯¹ßÇÒ ¼ö ÀÖ´Â ¼³Á¤ÀÌ ÀÖ½À´Ï´Ù. ¾Æ·¡ ÀÎµ¦½ºÀÇ ¼³Á¤(µé)À» È®ÀÎÇØ ÁÖ¼¼¿ä.\n\n");
			//for (int32 CI = 0; CI < UndesiredBeginEnableCompsSetting.Num(); ++CI)
			//{
			//	WarnMessage += UndesiredBeginEnableCompsSetting[CI] + TEXT("\n");
			//}
			//WarnMessage += FString(TEXT("\nWave ½ÃÀÛ ½Ã MeshComponent ¸¦ enable ½ÃÅ°´Â ¼³Á¤¿¡¼­´Â WaveExec_TriggerComponent ¸¸ »ç¿ëÇÏ´Â °ÍÀÌ ÁÁ½À´Ï´Ù."));
			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage));
		}
		if (UndesiredComplEnableCompsSetting.Num() > 0)
		{
			//FString WarnMessage = TEXT("WaveCompOnOffSettings ¿¡ Àß¸øµÈ ÇÃ·¹ÀÌ¸¦ À¯¹ßÇÒ ¼ö ÀÖ´Â ¼³Á¤ÀÌ ÀÖ½À´Ï´Ù. ¾Æ·¡ ÀÎµ¦½ºÀÇ ¼³Á¤(µé)À» È®ÀÎÇØ ÁÖ¼¼¿ä.\n\n");
			//for (int32 CI = 0; CI < UndesiredComplEnableCompsSetting.Num(); ++CI)
			//{
			//	WarnMessage += UndesiredComplEnableCompsSetting[CI] + TEXT("\n");
			//}
			//WarnMessage += FString(TEXT("\nWave Á¾·á ½Ã MeshComponent ¸¦ enable ½ÃÅ°´Â °ÍÀº ÇÃ·¹ÀÌ ·çÆ®¸¦ ¸·À» °¡´É¼ºÀÌ ÀÖ½À´Ï´Ù."));
			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage));
		}
#endif
	}

	if (PropertyName == Name_SpawnNum || PropertyName == Name_BossWave)
	{
#if !PLATFORM_MAC
		FString BossWaveWarnString = TEXT("BossWave ¼³Á¤À» ÇÑ wave ¿¡´Â ´Ü ÇÏ³ªÀÇ ¸÷¸¸À» spawn ½ÃÄÑ¾ß ÇÕ´Ï´Ù. ´ÙÀ½ wave ¼¼ÆÃµéÀ» È®ÀÎÇÏ¼¼¿ä.\n");
		bool bNeedWarning = false;
		for (int32 SI = 0; SI < PerStageSettings.Num(); ++SI)
		{
			for (int32 WI = 0; WI < PerStageSettings[SI].WaveSettings.Num(); ++WI)
			{
				FMonsterSpawnPoolWaveSettings& CurrWaveSetting = PerStageSettings[SI].WaveSettings[WI];
				// BossWave ¿¡ 2¸¶¸® ÀÌ»ó spawn ÇÏ¸é °³°ï¶õ. ¿Ö³Ä¸é BossHUD °¡ ÁßÃ¸µÇ¾î ¶ã °ÍÀÌ±â ¶§¹®.
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
	OutMobIndex = 0; // MobIndex ´Â ÁöÁ¤ÇÏÁö ¾ÊÀ» °æ¿ì ±âº» 0¹øÀ¸·Î ÇÏµµ·Ï ÇØ º¸ÀÚ..

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
{ // ¿ùµå¿¡ ¹èÄ¡µÈ ¿©·¯°³ÀÇ SpawnPool Áß¿¡¼­ ¿øÇÏ´Â ½ºÅ×ÀÌÁö¿¡ ÇÊ¿äÇÑ ¼¼ÆÃÀ» ´ã°í ÀÖ´Â ÇÏ³ª¸¦ Ã£´Â µ¥¿¡ »ç¿ëµÊ.
	for (AB2MonsterSpawnPool* ThisSP : InSPs)
	{
		if (ThisSP && ThisSP->HasSetupFor(InTargetStageNum, InTargetDifficulty)) // ÀÏ´Ü Á¤È®È÷ ÀÏÄ¡ÇÏ´Â ¼¼ÆÃÀÌ ÀÖ´Â °É Ã£´Â´Ù.
		{
			return ThisSP;
		}
	}

	if (bFallBackToLowerDifficulty && InTargetDifficulty > EStageDifficulty::ESD_Normal)
	{
		// ³ôÀº ³­ÀÌµµ·Î´Â Æú¹é ¸øÇÏ°í ÇÑ ´Ü°è¾¿ ³·Àº ³­ÀÌµµ·Î Å×½ºÆ®. (AB2MonsterSpawnPool::GetDesiredStageSetting)
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
				break; // ÀÌ ÀÌÇÏ´Â ¾øÀ½.
			}

			FallbackTestDifficulty = GetOneEasierStageDifficulty(FallbackTestDifficulty);
		}
	}
	// Ãß°¡·Î ¿©±â·Î ¿ÔÀ» ¶§ Ã³À½ °ÍÀ» ¸®ÅÏÇÏ´Â ¿É¼Çµµ »ý°¢ÇØ º¼ ¸¸ÇÏ´Ù.
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
	if (GIsEditor == false) // ÀÎ°ÔÀÓ¿¡¼± ´Ù¸¥ ·çÆ®¸¦ ÅëÇÏµµ·Ï. ´ëÃ¼·Î GameMode ÀÇ GetActiveSpawnPool
	{
		return NULL;
	}

	UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : NULL;
	ABladeIIWorldSettings* B2WS = EditorWorld ? Cast<ABladeIIWorldSettings>(EditorWorld->GetWorldSettings()) : NULL;

	if (EditorWorld && B2WS)
	{
		// ÀÏ´Ü ¸®½ºÆ®ºÎÅÍ ¼öÁý
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

	// EditingActor ³» ´ÜÀÏ ÄÄÆ÷³ÍÆ® ¿òÁ÷ÀÎ´Ù°í EditingActor ÂÊÀ¸·Î ½ÅÈ£°¡ °¡Áö ¾ÊÀ¸¹Ç·Î ÀÌ·¸°Ô ½ÅÈ£¸¦ º¸³»ÁØ´Ù
	// ±Ùµ¥ ÀÌ°Íµµ ¾îÂ÷ÇÇ ÀÌµ¿ÇÒ ¶© ¾ÈºÒ¸®°í.. »ÏÁ·ÇÑ ¼ö°¡ ¾ø±º. Modify ¸¦ »ç¿ëÇÒ ¼öµµ ÀÖ´Âµ¥ ±×°Ç Á» À§ÇèÇÑ °Å °°´Ù.
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
	if (HasAnyFlags(RF_ClassDefaultObject) && GConfig) // ÇÑ¹ø¸¸ ½ÇÇàÇÏµµ·Ï.
	{
		GConfig->GetFloat(TEXT("MonsterSpawnPool"), TEXT("EditingActorSizeLimit"), MeshSizeLimit, GEditorIni);
	}
#endif

	CurrEditingClientStageId = 1;
	CurrEditingDifficultyLevel = EStageDifficulty::ESD_Normal;

	OwnerPool = NULL;
}

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
	{ // »ý¼ºÀ» UB2SpawnPoolEditingSKComponent ·Î ÇØ ÁÖ¾î¾ß ÄÄÆ÷³ÍÆ® °³º° ÀÌµ¿ ½Ã ½ÅÈ£°¡ °£´Ù.
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
		EditingMeshComp->SetWorldLocationAndRotation(ObjWorldTransform.GetLocation(), ObjWorldTransform.GetRotation()); // Location Àº Mesh ¶û Å©±â ¼¼ÆÃµÈ ´ÙÀ½ offset ¹Ý¿µÇØ¼­ ¾Æ·¡¿¡¼­ ´Ù½Ã.
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
				// ÇÊ¿ä¿¡ µû¶ó Ãß°¡ Á¤º¸¸¦ È®ÀÎÇÏµçÁö..
				ExtraInfoString = TEXT("INVALID CLASS!\n") + ExtraInfoString;
				ExtraInfoTextComp->SetTextRenderColor(FColor(255, 0, 255));
			}

			ExtraInfoTextComp->SetText(FText::FromString(ExtraInfoString));
		}

		SetRepresentativeMesh(InWaveSettingNum, InSpawnObjNum, CharacterClassEnum, ClassVariation, CharacterClassBP);

		// NPCClassInfo ¿¡ ÀÇÇÑ Mesh ¹× Material ¼¼ÆÃÀº SetRepresentativeMesh ¿¡¼­ Ã³¸®ÇÏ°í SpawnPool ÀÚÃ¼ÀûÀ¸·Î ¼¼ÆÃÇÏ´Â Ãß°¡ material override Ã³¸®.
		AB2MonsterSpawnPool::OverrideMaterialForNewSpawned(nullptr, InOptionalWaveMtrlOverride, EditingMeshComp);

		// SyncEditingActorLocationToSpawnPool ¿¡¼­ ¹Ù¿îµå Å©±â¸¸Å­ ¿Ã¸° °ªÀ» »ç¿ëÇÏ¹Ç·Î ¿©±â¼­´Â ±×¸¸Å­ ³»·ÁÁØ´Ù.
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
				MtrlOverrideFromClass = PreviewAssetInfo.MaterialOverrides; // NPCClassInfo ¿¡ ÀÇÇÑ ±âº»ÀûÀÎ MaterialOverride ÀÎµ¥, ÀÌ°Í ÀÌÈÄ¿¡ SpawnPool WaveSetting ¿¡ ÀÇÇÑ Material Override µµ ÀÖÀ» ¼ö ÀÖ´Ù.
			}
		}
		else if (CharacterClassBP)
		{
			ABladeIICharacter* CharacterObj = Cast<ABladeIICharacter>(CharacterClassBP->GetDefaultObject());
			if (CharacterObj && CharacterObj->GetMesh())
			{
				SKMeshFromClass = CharacterObj->GetMesh()->SkeletalMesh;
			}
		}

		//if (SKMeshFromClass) NULL ÀÌ¸é NULL ·Î ¼¼ÆÃ. ±×·¡¾ß Àß¸øµÈ °É ¾Ë°ÚÁö..
		{
			for (int32 MI = 0; MI < EditingMeshComp->GetNumMaterials(); ++MI)
			{ // ÄÄÆ÷³ÍÆ® ÀçÈ°¿ë ÇÒ ¼ö ÀÖÀ¸¹Ç·Î ±âÁ¸ ²¨ Å¬¸®¾î ÇØ¾ß.
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
{ // EditingComponent ÀÇ ¹ÝÂÊ Å©±â¸¦ °¨¾ÈÇÏ´Â °Å.
	// EditingComponent µéÀº ¿øÁ¡ÀÌ ¹ß¹Ù´ÚÀÎµ¥ ½ÇÁ¦ spawn ÇÒ BladeIICharacter µéÀº ¿øÁ¡ÀÌ Áß½ÉÀÌ¹Ç·Î.. Á¤È®ÇÏÁö´Â ¾ÊÁö¸¸ ¾î´À Á¤µµ °¨¾ÈÇØ¼­ º¸Á¤ÇØ ÁÖ±â À§ÇÔ.
	// ¾îÂ÷ÇÇ spawn ½Ã ¹Ù´Ú¿¡ snap ½ÃÅ³ °ÍÀÌ¹Ç·Î À§Ä¡¸¦ ¾à°£ ³ô°Ô ¼±Á¤ÇÒ ÇÊ¿ä°¡ ÀÖ´Ù.
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
				// ¿øÁ¡ÀÌ ¹ß¹Ù´ÚÀÎ EditingComponent µé¿¡ ´ëÇØ ¿øÁ¡ÀÌ Áß½ÉÀÎ BladeIICharacter µéÀ» °í·ÁÇØ¼­ Á¶±ÝÀÌ³ª¸¸ ¿øÁ¡ º¸Á¤À».. ¾îÂ÷ÇÇ spawn ½Ã ¹Ù´Ú¿¡ snap ½ÃÅ°¹Ç·Î Á» ³ô¿©Áàµµ µÊ.
				FVector HeightAdjustedLocation = SKMeshComp->GetComponentLocation() + GetEditingCompToSpawnLocOffset(SKMeshComp);

				OwnerPool->ExplictSetSpawnTransform(CurrEditingClientStageId, CurrEditingDifficultyLevel, EditingSimpleInfo[SI].WaveNum, EditingSimpleInfo[SI].SpawnObjNum,
					HeightAdjustedLocation, SKMeshComp->GetComponentRotation());
			}
		}
		OwnerPool->MarkPackageDirty();
	}
}
#endif
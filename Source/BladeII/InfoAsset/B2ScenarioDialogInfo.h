#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "BladeIIPlayer.h"
#include "B2ScenarioDialogInfo.generated.h"

UENUM(BlueprintType)
enum class EPortraitType : uint8
{
	EPT_Default,
	EPT_Happy,
	EPT_Angry,
	EPT_End
};

struct FDialogLine
{
	FDialogLine(struct FScenarioDialogInfoData*);

	FName			SpeakerCodeName;
	EPortraitType	PortraitType;
	FName			DialogLineCodeName;
	FName			DialogSoundCodeName;
	float			ShowingTime;
	float			DelayToNextLine;
};

//Native only
struct FDialogInfoInstance
{
	FDialogInfoInstance(struct FScenarioDialogInfoData*);

	FName				DialogCodeName;	
	TArray<FDialogLine>	DialogSequence;
	bool				bNeedPauseGame;
};

USTRUCT(BlueprintType)
struct FDialogPortraitInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPortraitType				PortraitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UMaterialInstance>	Portrait;
};

USTRUCT()
struct FDialogCharacterInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FDialogCharacterInfoLoadedRefHolder()
	{
		LoadedWholeBodyPortrait = NULL;
	}

	UPROPERTY(Transient)
	TArray<class UMaterialInstance*> LoadedPortraitMtrls;
	UPROPERTY(Transient)
	class UTexture2D* LoadedWholeBodyPortrait;
};

USTRUCT(BlueprintType)
struct FDialogCharacterInfo
{
	GENERATED_USTRUCT_BODY()
	
	FDialogCharacterInfo(){ SpecifyPCClass = EPCClass::EPC_End; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName						CharacterCodeName;

	UPROPERTY(EditAnywhere)
	EPCClass					SpecifyPCClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName						CharacterNameKey;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogPortraitInfo>	 CharacterPortrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D>		WholeBodyPortraitPtr;
public:

	/** You must use the one provided at UB2DialogCharacterInfoSet, if you cannot provide LoadedRefHolder */
	UMaterialInstance* GetPortraitMtrlForType(FStreamableManager& InLoadManager, EPortraitType InType, FDialogCharacterInfoLoadedRefHolder& InOutLoadedRefHolder);
	/** You must use the one provided at UB2DialogCharacterInfoSet, if you cannot provide LoadedRefHolder */
	UTexture2D* GetWholeBodyPortrait(FStreamableManager& InLoadManager, FDialogCharacterInfoLoadedRefHolder& InOutLoadedRefHolder);
	
	void UnloadAll(FStreamableManager& InLoadManager, FDialogCharacterInfoLoadedRefHolder& InOutLoadedRefHolder);
#if WITH_EDITOR
	void EditorLoadAll(FStreamableManager& InLoadManager, FDialogCharacterInfoLoadedRefHolder& InOutLoadedRefHolder);
#endif
};

UCLASS(Blueprintable)
class BLADEII_API UB2DialogCharacterInfoSet : public UObject
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	TArray<FDialogCharacterInfo>	DialogCharacterInfoArray;

	FStreamableManager InfoLoadManager;

public:
	/* Construct DialogCharacterMap */
	virtual void PostLoad() override;

	int32 GetInfoArrayIndexOfCharCode(FName CharacterCodeName, UObject* WorldContextObject);

	FDialogCharacterInfo* GetDialogCharacterInfo(FName CharacterCodeName, UObject* WorldContextObject);

	UMaterialInstance* GetDialogCharacterPortraitMtrl(FName CharacterCodeName, EPortraitType InType, UObject* WorldContextObject);
	UTexture2D* GetWholeBodyPortrait(FName CharacterCodeName, UObject* WorldContextObject);
	
	//FORCEINLINE FStreamableManager& GetInfoLoadManager() { return InfoLoadManager; }

	void UnloadAll();
#if WITH_EDITOR
	/** For safe use of TSoftObjectPtr in editor. */
	void EditorLoadAll();
#endif
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
private:	
	TMap<FName, int32>				DialogCharacterMap;
	TMap<EPCClass, int32>			DialogPlayerClassMap;

	/** Contains all loaded resources of DialogCharacterInfoArray */
	UPROPERTY(Transient)
	TMap<int32, FDialogCharacterInfoLoadedRefHolder> LoadedResourceHolder;
};

#define DIALOG_CHAR_USERMAINCLASS FName(TEXT("MainPlayerClass"))
#define DIALOG_CHAR_USERSUBCLASS FName(TEXT("SubPlayerClass"))

//======================================================================
// DialogSoundInfo, as separate lazy-loaded info asset table
//======================================================================

/** Struct to contain loaded TAsset references of a single DialogSoundInfo entry to prevent it get GCd. */
USTRUCT()
struct FSingleDialogSoundInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FSingleDialogSoundInfoLoadedRefHolder()
	{
		LoadedSoundPtr = NULL;
	}

	UPROPERTY(Transient)
	USoundCue* LoadedSoundPtr;
};

USTRUCT(BlueprintType)
struct FDialogSoundInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FDialogSoundInfo()
	{
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USoundCue> AssetPtr;
public:

	/** An utility to get the actual SoundCue asset defined by AssetPtr.
	 * You must use the one provided at UB2DialogSoundInfoSet, if you cannot provide LoadedRefHolder */
	USoundCue* GetSoundCue(FStreamableManager& InLoadManager, FSingleDialogSoundInfoLoadedRefHolder& InOutLoadedRefHolder);
	void Unload(FStreamableManager& InLoadManager, FSingleDialogSoundInfoLoadedRefHolder& InOutLoadedRefHolder);
};

UCLASS(Blueprintable)
class BLADEII_API UB2DialogSoundInfoSet : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	class UDataTable* TheData;

	UPROPERTY(Transient)
	TMap<FName, FSingleDialogSoundInfoLoadedRefHolder> LoadedResourceHolder; 

	FStreamableManager InfoLoadManager;

public:
	UB2DialogSoundInfoSet(const FObjectInitializer& ObjectInitializer);

	void SetDataTable(UDataTable* InDataTable);

	/** InRawSoundCode is used without any modification except for player character, which requires some identification using WorldContextObject. */
	USoundCue* GetInfoSound(FName InCharacterCodeName, FName InRawSoundCode, UObject* WorldContextObject);
	
	void UnloadAll();
#if WITH_EDITOR
	/** For safe use of TSoftObjectPtr in editor. This is DataTable-based, so it is dead necessary. */
	void EditorLoadAll();
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
#endif
};

//======================================================================
// The final dialog info, putting things together.
//======================================================================

USTRUCT(BlueprintType)
struct FScenarioDialogInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FScenarioDialogInfoData()
	{
		PortraitType = EPortraitType::EPT_Default;
		ShowingTime = 0.0f;
		DelayToNextLine = 0.0f;
		NeedPauseGame = false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName			DialogCodeName;

	/*
	 * If want to set the player's current class, write 'CurrentMainPlayerClass' or 'CurrentSubPlayerClass'
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName			SpeakerCodeName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName			DialogLineCodeName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName			DialogSoundCodeName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPortraitType	PortraitType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float			ShowingTime;
	// Required only for successive dialog line, which has duplicated DialogCodeName.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float			DelayToNextLine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32			NeedPauseGame:1;
};

UCLASS()
class BLADEII_API UB2ScenarioDialogInfo : public UObject
{
	GENERATED_UCLASS_BODY()	
public:
	/* Construct DialogInfoMap */
	void SetDataTable(UDataTable* InDataTable);

	const FDialogInfoInstance* GetDialogInfo(FName DialogCodeName);

	void PreloadAssetsForStageGame(const TArray<FName>& InPreloadDialogNames, UObject* WorldContextObject, bool bIncludeWholeBodyPortrait);

private:
	TMap<FName, FDialogInfoInstance>	DialogInfoMap;

	UPROPERTY()
	class UDataTable*				DialogInfoDataTable;
};
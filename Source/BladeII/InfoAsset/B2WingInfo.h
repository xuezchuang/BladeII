// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "CommonStruct.h"
#include "BladeIIPlayer.h"
#include "Engine/StreamableManager.h"
#include "B2WingInfo.generated.h"

/** All factors decide which set of wing info resource to use */
USTRUCT()
struct FSingleWingInfoID
{
	GENERATED_USTRUCT_BODY()

	FSingleWingInfoID()
	{
		PCClassEnum = EPCClass::EPC_End;
		EvolutionGrade = 0;
	}
	FSingleWingInfoID(const FB2Wing& InWingData)
	{
		PCClassEnum = IntToPCClass(InWingData.OwnerPCClass);
		EvolutionGrade = InWingData.EvolutionGrade;
	}
	
	// Data here are not for editing anymore. They will be set automatically in the Editor.

	UPROPERTY(VisibleAnywhere)
	EPCClass PCClassEnum;

	UPROPERTY(VisibleAnywhere)
	int32 EvolutionGrade;

	bool operator==(const FSingleWingInfoID& InOther) const
	{
		return (this->PCClassEnum == InOther.PCClassEnum && this->EvolutionGrade == InOther.EvolutionGrade);
	}

	// Get int32 ID from data member here, for use of LoadedPtrMap
	FORCEINLINE int32 GetIntID() const { return (PCClassToInt(PCClassEnum) * 100) + EvolutionGrade; }
};

USTRUCT()
struct FSingleWingInfoData
{
	GENERATED_USTRUCT_BODY()

	FSingleWingInfoData()
	{
		
	}

	UPROPERTY(EditAnywhere)
	FSingleWingInfoID MyID;

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class USkeletalMesh> WingMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> MtrlOverride;
public:
	class USkeletalMesh* GetWingMesh();
	class UMaterialInterface* GetMtrlOverride();

	UPROPERTY(EditAnywhere)
	TArray<FCHSKCompAttachParticleSystemInfo> AttachEffectInfoArray;

#if WITH_EDITOR
	void EditorLoadAllTAsset(); // Provided for safe editing work
#endif
};

/** For PropOptionIcon material setup */
USTRUCT()
struct FWingPropOptionIconSetup
{
	GENERATED_USTRUCT_BODY()

	FWingPropOptionIconSetup()
	{
		PropOption = EItemOption::EIO_End;
	}

	UPROPERTY(EditAnywhere)
	EItemOption PropOption;

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> IconMtrl;
public:
	class UMaterialInterface* GetIconMtrl();

#if WITH_EDITOR
	void EditorLoadAllTAsset(); // Provided for safe editing work
#endif
	// Get int32 ID from data member here, for use of LoadedPropOptionIconMap
	FORCEINLINE int32 GetIntID() const { return static_cast<int32>(PropOption); }
};


/**
 * Info table containing wing resources
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2WingInfo : public UObject
{
	GENERATED_BODY()
	
protected:
		
	FStreamableManager InfoLoadManager; // To use TSoftObjectPtr

	/** Info data array was once merged as single array but now separated per character class to identify setup more or less easily. */
	UPROPERTY(EditAnywhere)
	TArray<FSingleWingInfoData> MainInfoArray_Gladiator;
	UPROPERTY(EditAnywhere)
	TArray<FSingleWingInfoData> MainInfoArray_Assassin;
	UPROPERTY(EditAnywhere)
	TArray<FSingleWingInfoData> MainInfoArray_Wizard;
	UPROPERTY(EditAnywhere)
	TArray<FSingleWingInfoData> MainInfoArray_Fighter;
	
	
	UPROPERTY(EditAnywhere)
	TArray<FWingPropOptionIconSetup> PropOptionIconSetup;

	UPROPERTY(EditAnywhere)
	TMap<int32, TSoftObjectPtr<UMaterialInterface>> PerGradeWingMainIconMtrl;
	
public:
	UB2WingInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;

	TArray<FSingleWingInfoData>* GetMainInfoArrayPtrForChar(EPCClass InCharClass);

	/** Get single info data by WingInfoID, NULL if not found. It loads all lazy-loaded TSoftObjectPtr of found info. */
	FSingleWingInfoData* GetInfoData(const FSingleWingInfoID& InID);
	FSingleWingInfoData* GetInfoData(const FB2Wing& InWing);

	FWingPropOptionIconSetup* GetPropOptionIconSetup(EItemOption InOption);

	UMaterialInterface* GetWingMainIconMtrlForGrade(int32 InEvolveGrade);

#if WITH_EDITOR
	/** For safe use of TSoftObjectPtr in editor. */
	void EditorLoadAll();
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	friend struct FSingleWingInfoData;
};

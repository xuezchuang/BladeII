// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommonStruct.h"
#include "B2EtherSetEffect.h"
#include "../UI/Doc/B2UIDocBattle.h"
#include "B2EtherContainer.generated.h"


UCLASS(Blueprintable)
class BLADEII_API UB2EtherSetInfoTable : public UObject
{
public:
	GENERATED_BODY()

	TSubclassOf<UB2EtherSetEffect> GetEtherSetEffectClass(int32 EtherSetID);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	TMap<int32, TSubclassOf<UB2EtherSetEffect>> EtherSetClasses;
};
/**
 * 
 */
UCLASS(Blueprintable)
class BLADEII_API UB2EtherContainer : public UObject
{
public:
	GENERATED_BODY()

	void InitializeContainer(class ABladeIIPlayer* EtherOwner);
	void InitializeDocBattle(UB2UIDocBattle* DocBattle);
	void SetOwnerPlayer(class ABladeIIPlayer* EtherOwner);
	void RealtimeUpdateUIDoc(UB2UIDocBattle* DocBattle);
	void UpdateCoolTime(float DeltaSeconds);

	UB2EtherSetEffect* GetEtherSetEffect(EEtherSetType EtherSetType);
	UB2EtherSetEffect* GetEtherSetEffect(const int32 EtherSetdID);
	FORCEINLINE UB2EtherSetEffect* GetOffenseEtherSet() { return OffenseEther; }
	FORCEINLINE UB2EtherSetEffect* GetDefenseEtherSet() { return DefenseEther; }

	bool AddEtherSetEffect(const int32 EtherSetID, float TriggerRate, float CoolTime);
	void RemoveEtherSetEffect(EEtherSetType EtherSetType);

protected:

	UB2EtherSetEffect*	InitEtherSetEffect(ABladeIIPlayer* EtherOwner, EEtherSetType EtherSetType);
	UB2EtherSetEffect*	CreateEtherSetEffect(EEtherSetType EtherSetType, int32 EtherID);

	class ABladeIIPlayer*	OwnerPlayer;

	UPROPERTY(Transient)
	UB2EtherSetEffect*		OffenseEther;

	UPROPERTY(Transient)
	UB2EtherSetEffect*		DefenseEther;
};


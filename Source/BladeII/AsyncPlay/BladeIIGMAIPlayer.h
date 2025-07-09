#pragma once

#include "BladeIITMAIPlayer.h"
#include "B2GuildGameMode.h"
#include "BladeIIGMAIPlayer.generated.h"

UCLASS()
class ABladeIIGMAIPlayer : public ABladeIITMAIPlayer, public IGuildPawnInterface
{
	GENERATED_BODY()
public:
	ABladeIIGMAIPlayer(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeCombatStats() final;

	void InitCombateStatsForParam(const FCombatStatParam& CombatStatParam);
	virtual class ICharacterDataStore* GetCharacterDataStore() override;

	void InitializePCClassInfoSpawn(ABladeIIGameMode* InGameMode, UB2PCClassInfo* PCClassInfo);

	void SetPlayerInfo(const FGuildPlayerInfo& InPlayerInfo);
	const FGuildPlayerInfo& GetPlayerInfo();

public:
	virtual const FGuildPlayerInfo& GetGuildPlayerInfo() override;
	virtual bool PawnIsAlive() override;
	virtual class ABladeIICharacter* GetPawnInstance() override;
	virtual FVector GetLocation() const override;
	virtual AController* GetPawnController() override;
	virtual FString GetPlayerStartName() override;

private:
	FGuildPlayerInfo CacheGuildPlayerInfo;
};
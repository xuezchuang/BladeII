#pragma once

#include "GameFramework/Actor.h"
#include "B2GuildCastleActor.generated.h"


UENUM(BlueprintType)
enum class EUIGuildSelectEffect : uint8
{
	Rival			UMETA(DisplayName = "Rival"),
	Team			UMETA(DisplayName = "Team"),
	MyCastle		UMETA(DisplayName = "MyCastle"),
	Temp			UMETA(DisplayName = "Temp"),
	End				UMETA(DisplayName = "End"),

};
UCLASS()
class BLADEII_API AB2GuildCastleActor : public AActor
{
	GENERATED_UCLASS_BODY()

UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRootComponent;

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickCastleActer();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void SetCastleStaticMesh(UStaticMeshComponent* MeshComponent,UStaticMesh* Castle1, UStaticMesh* Castle2, UStaticMesh* Castle3, UStaticMesh* Castle4);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void SetCastleEffect(UParticleSystemComponent* Effect1, UParticleSystemComponent* Effect2);

	UFUNCTION(BlueprintImplementableEvent)
		void ClickPlaySound();


	virtual void NotifyActorOnReleased(FKey ButtonReleased = EKeys::LeftMouseButton) override;
	virtual void NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex) override;
	void SetGuildInfo(int32 Index, bool GuildInfoState = true);
private:
	int32 InfoIndex;
	bool MyGuildInfo;
	TArray<UStaticMesh*>		CastleMesh;
	UStaticMeshComponent*		CastleMeshComponent;

	UParticleSystemComponent*				NoMedalEffect;
	UParticleSystemComponent*				BattleEffect;


};
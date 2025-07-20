#pragma once
//#include "BladeII.h"
#include "B2AIUtil.generated.h"

class ABladeIICharacter;
namespace B2AIUtil
{
	FVector GetFleeLocation(ABladeIICharacter* CurrentCharacter, TArray<ABladeIICharacter*>& FleeToChracters, int32 TestNumLocation, float Radius);
};

UCLASS(Blueprintable)
class BLADEII_API UB2CharacterContainer : public UObject
{
public:
	GENERATED_BODY()

	TArray<ABladeIICharacter*> Chracters;
};
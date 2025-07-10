#pragma once
//#include "BladeII.h"
#include "B2AIUtil.generated.h"

namespace B2AIUtil
{
	FVector GetFleeLocation(class ABladeIICharacter* CurrentCharacter, TArray<class ABladeIICharacter*>& FleeToChracters, int32 TestNumLocation, float Radius);
};

UCLASS(Blueprintable)
class BLADEII_API UB2CharacterContainer : public UObject
{
public:
	GENERATED_BODY()

	TArray<class ABladeIICharacter*> Chracters;
};
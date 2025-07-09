#include "BladeII.h"
#include "B2AIUtil.h"
#include "BladeIIUtil.h"


FVector B2AIUtil::GetFleeLocation(ABladeIICharacter* CurrentCharacter, TArray<ABladeIICharacter*>& FleeToChracters, int32 TestNumLocation, float Radius)
{
	if (!CurrentCharacter || FleeToChracters.Num() == 0)
		return FVector();

	struct FLocationScore
	{
		FVector Location;
		float Score;
	};

	TArray<FLocationScore> TestLocations;
	TestLocations.AddZeroed(TestNumLocation);

	const FVector& ActorLocation = CurrentCharacter->GetActorLocation();

	for (auto& Location : TestLocations)
	{
		Location.Location = GetRandomReachablePointInRadiusWithCurrNavSys(CurrentCharacter->GetWorld(), ActorLocation, Radius);
		Location.Score = 0.f;
	}

	//우선 단순하게 거리순으로.
	//플레이어에게서 거리가 멀수록 좋은 지점 100uu당 1점씩
	for (auto& Location : TestLocations)
	{
		//for (ABladeIICharacter* Character : FleeToChracters)
		//{
		//	if (Character && Character->IsValidObj() && Character->IsAlive())
		//		Location.Score += (Location.Location - Character->GetActorLocation()).SizeSquared() * 0.0001f;
		//}
	}


	TestLocations.Sort([](const FLocationScore& A, const FLocationScore& B)->bool
	{
		return A.Score > B.Score;
	});

	//Blocking volume등으로 이동못하는 경우가 생길 수 있다.
	FCollisionQueryParams TraceParams(NAME_None, false, CurrentCharacter);
	for (auto Location : TestLocations)
	{
		FHitResult Hit;

		if (CurrentCharacter->GetWorld()->LineTraceSingleByChannel(Hit, ActorLocation, Location.Location, ECC_WorldDynamic, TraceParams))
			continue;

		return Location.Location;
	}

	return ActorLocation;
}

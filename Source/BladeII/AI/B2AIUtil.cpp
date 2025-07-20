#include "B2AIUtil.h"


#include "BladeIIUtil.h"
#include "BladeIICharacter.h"


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

	//快急 窜鉴窍霸 芭府鉴栏肺.
	//敲饭捞绢俊霸辑 芭府啊 钢荐废 亮篮 瘤痢 100uu寸 1痢究
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

	//Blocking volume殿栏肺 捞悼给窍绰 版快啊 积辨 荐 乐促.
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

#include "B2TargetSelector.h"
#include "BladeIIGameMode.h"



B2TargetSelector & B2TargetSelector::GetInstance()
{
	static B2TargetSelector BuffLogic;
	return BuffLogic;
}

B2TargetSelector::B2TargetSelector()
{
	//container의 메모리가 부족해서 메모리 늘릴때 lambda가 날아감 
	TargetFilters.Reserve(10);

	//Add 플래그 부터 순서대로 넣어줘야 전부 Add하고 나서 Filter로 거르게 됨...
	AddFilter(ETargetSelectType::ETST_AddAllyTeam,
		[this](TArray<class ABladeIICharacter*>& OutFoundCharacters, ABladeIICharacter * CurrentChracter)
	{
		ABladeIIGameMode* GameMode = nullptr;
		if (!this->CheckValidParameter(CurrentChracter, &GameMode))
			return;

		TArray<ABladeIICharacter*> TeamCharacters;
		GameMode->GetAllMyTeamCharList(TeamCharacters, CurrentChracter->GetTeamNum(), true);
		OutFoundCharacters.Append(TeamCharacters);
	});

	AddFilter(ETargetSelectType::ETST_AddEnemyTeam,
		[this](TArray<class ABladeIICharacter*>& OutFoundCharacters, ABladeIICharacter * CurrentChracter)
	{
		ABladeIIGameMode* GameMode = nullptr;
		if (!this->CheckValidParameter(CurrentChracter, &GameMode))
			return;

		int32 CurrentPlayerTeamNum = CurrentChracter->GetTeamNum();
		TArray<ABladeIICharacter*> TeamCharacters;

		GameMode->GetAllCharacterList(TeamCharacters);
		
		TeamCharacters.RemoveAll([CurrentPlayerTeamNum](ABladeIICharacter* Character) 
		{
			return Character == nullptr || !Character->IsAlive() || (Character->GetTeamNum() == CurrentPlayerTeamNum);
		});
		OutFoundCharacters.Append(TeamCharacters);
	});

	AddFilter(ETargetSelectType::ETST_FilterMercenary,
		[](TArray<class ABladeIICharacter*>& OutFoundCharacters, ABladeIICharacter * CurrentChracter)
	{
		OutFoundCharacters.RemoveAll([](ABladeIICharacter* Character)
		{
			return Character == nullptr || Character->CharacterType != ECharacterType::ECT_Mercenary;
		});
	});

	AddFilter(ETargetSelectType::ETST_FilterPlayer,
		[](TArray<class ABladeIICharacter*>& OutFoundCharacters, ABladeIICharacter * CurrentChracter)
	{
		OutFoundCharacters.RemoveAll([](ABladeIICharacter* Character)
		{
			return Character == nullptr || UB2PCClassInfo::FromCharacterTypeToPCClass(Character->CharacterType) == EPCClass::EPC_End;
		});
	});

	AddFilter(ETargetSelectType::ETST_FilterSelf,
		[](TArray<class ABladeIICharacter*>& OutFoundCharacters, ABladeIICharacter * CurrentChracter)
	{
		OutFoundCharacters.RemoveAll([CurrentChracter](ABladeIICharacter* Character)
		{
			return Character == nullptr || CurrentChracter != Character;
		});
	});
}

void B2TargetSelector::AddFilter(ETargetSelectType SelectType, TargetFilterFunction && FilterFunction)
{
	TargetFilters.Emplace(SelectType, std::forward<TargetFilterFunction>(FilterFunction));
}

void B2TargetSelector::FindTargetSelectFlag(TArray<class ABladeIICharacter*>& OutFoundCharacters, EnumMaskValue<ETargetSelectType> TargetSelectType, ABladeIICharacter * CurrentChracter)
{
	OutFoundCharacters.Empty();

	for (auto& TargetFilterPair : TargetFilters)
	{
		if (TargetSelectType.Test(TargetFilterPair.Key))
			TargetFilterPair.Value(OutFoundCharacters, CurrentChracter);
	}
}
void B2TargetSelector::FilterDistance(TArray<class ABladeIICharacter*>& OutFoundCharacters, ABladeIICharacter * CurrentChracter, float Radius)
{
	if (CurrentChracter == nullptr)
		return;

	if (CurrentChracter->GetCapsuleComponent() && Radius < CurrentChracter->GetCapsuleComponent()->GetScaledCapsuleRadius())
		return;

	OutFoundCharacters.RemoveAll([CurrentChracter, Radius](ABladeIICharacter* Character)
	{
		return Character != NULL &&
			   Character != CurrentChracter &&
			   FVector::DistSquared(Character->GetActorLocation(), CurrentChracter->GetActorLocation()) >= FMath::Square(Radius);
	});
}

void B2TargetSelector::FilterMaxNum(TArray<class ABladeIICharacter*>& OutFoundCharacters, int32 MaxNum)
{
	if (OutFoundCharacters.Num() <= MaxNum)
		return;

	OutFoundCharacters.RemoveAt(MaxNum, OutFoundCharacters.Num() - MaxNum);
}

bool B2TargetSelector::IsExceptSelf(EnumMaskValue<ETargetSelectType> TargetSelectType)
{
	return TargetSelectType.Test(ETargetSelectType::ETST_ExceptSelf);
}

bool B2TargetSelector::CheckValidParameter(ABladeIICharacter * CurrentChracter, ABladeIIGameMode ** OutGameMode)
{
	if (CurrentChracter == nullptr)
		return false;

	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(CurrentChracter));
	if (GameMode == nullptr)
		return false;

	(*OutGameMode) = GameMode;
	return true;
}

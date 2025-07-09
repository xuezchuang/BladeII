#pragma once
#include "BladeIIDamageType.h"
#include <type_traits>

template<typename EnumValue>
class EnumMaskValue
{
	static_assert(std::is_enum<EnumValue>::value, "Type for EnumMask must be an Enum");

private:
	uint8 SetMaskValue;

public:
	EnumMaskValue() { SetMaskValue = 0;	}
	EnumMaskValue(uint8 MaskValue) { SetMaskValue = MaskValue; }

	bool Test(EnumValue Compare)
	{
		return (SetMaskValue & (1 << static_cast<uint8>(Compare))) > 0;
	}
};

class B2TargetSelector
{
public:
	using TargetFilterFunction = std::function<void(TArray<class ABladeIICharacter*>&, class ABladeIICharacter*)>;

public:
	static B2TargetSelector& GetInstance();
	
	void FindTargetSelectFlag(TArray<class ABladeIICharacter*>& OutFoundCharacters, EnumMaskValue<ETargetSelectType> TargetSelectType, class ABladeIICharacter* CurrentChracter);
	void FilterDistance(TArray<class ABladeIICharacter*>& OutFoundCharacters, class ABladeIICharacter* CurrentChracter, float Radius);
	void FilterMaxNum(TArray<class ABladeIICharacter*>& OutFoundCharacters, int32 MaxNum);
	bool IsExceptSelf(EnumMaskValue<ETargetSelectType> TargetSelectType);
	bool CheckValidParameter(ABladeIICharacter * CurrentChracter, ABladeIIGameMode** OutGameMode);

private:
	B2TargetSelector();

	//Use only reference variable.
	B2TargetSelector(B2TargetSelector const&) = delete;
	B2TargetSelector& operator=(B2TargetSelector const&) = delete;


	void AddFilter(ETargetSelectType SelectType, TargetFilterFunction&& FilterFunction);

private:
	TMap<ETargetSelectType, TargetFilterFunction> TargetFilters;
};

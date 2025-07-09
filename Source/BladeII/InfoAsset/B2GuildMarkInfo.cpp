#include "BladeII.h"
#include "B2GuildMarkInfo.h"



UB2GuildMarkInfo::UB2GuildMarkInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UMaterialInstance* UB2GuildMarkInfo::GetPatternImage(int32 Index)
{
	if (!PatternImage.IsValidIndex(Index))
		return nullptr;

	TSoftObjectPtr<UMaterialInstance>& WantedImageAssetPtr = PatternImage[Index];
	if (!WantedImageAssetPtr.IsValid())
	{
		WantedImageAssetPtr.LoadSynchronous();
	}

	return WantedImageAssetPtr.Get();
}

FLinearColor UB2GuildMarkInfo::GetPatternColor(int32 Index)
{
	if (!PatternColor.IsValidIndex(Index))
		return FLinearColor(0.f, 0.f, 0.f, 0.f);

	return PatternColor[Index];
}

UMaterialInstance* UB2GuildMarkInfo::GetBGImage(int32 Index)
{
	if (!BGImage.IsValidIndex(Index))
		return nullptr;
	
	TSoftObjectPtr<UMaterialInstance>& WantedImageAssetPtr = BGImage[Index];
	if (!WantedImageAssetPtr.IsValid())
	{
		WantedImageAssetPtr.LoadSynchronous();
	}

	return WantedImageAssetPtr.Get();
}

FLinearColor UB2GuildMarkInfo::GetBGColor(int32 Index)
{
	if (!BGColor.IsValidIndex(Index))
		return FLinearColor(0.f, 0.f, 0.f, 0.f);

	return BGColor[Index];
}
UMaterialInstance* UB2GuildMarkInfo::GetBGBorder(int32 Index)
{
	if (!BGBorder.IsValidIndex(Index))
		return nullptr;

	TSoftObjectPtr<UMaterialInstance>& WantedImageAssetPtr = BGBorder[Index];
	if (!WantedImageAssetPtr.IsValid())
	{
		WantedImageAssetPtr.LoadSynchronous();
	}

	return WantedImageAssetPtr.Get();
}
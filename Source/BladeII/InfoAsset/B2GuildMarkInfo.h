#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Materials/MaterialInstance.h"
#include "B2GuildMarkInfo.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2GuildMarkInfo : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<TSoftObjectPtr<UMaterialInstance>> PatternImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FLinearColor> PatternColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<TSoftObjectPtr<UMaterialInstance>> BGImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FLinearColor> BGColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<TSoftObjectPtr<UMaterialInstance>> BGBorder;


public:
	FORCEINLINE int32 GetPatternImageNum() { return PatternImage.Num(); }
	FORCEINLINE int32 GetPatternColorNum() { return PatternColor.Num(); }
	FORCEINLINE int32 GetBGImageNum() { return BGImage.Num(); }
	FORCEINLINE int32 GetBGColorNum() { return BGColor.Num(); }
	FORCEINLINE int32 GetBGBorderNum() { return BGBorder.Num(); }


	UMaterialInstance* GetPatternImage(int32 Index);
	FLinearColor GetPatternColor(int32 Index);
	UMaterialInstance* GetBGImage(int32 Index);
	FLinearColor GetBGColor(int32 Index);
	UMaterialInstance* GetBGBorder(int32 index);


	UB2GuildMarkInfo(const FObjectInitializer& ObjectInitializer);

};
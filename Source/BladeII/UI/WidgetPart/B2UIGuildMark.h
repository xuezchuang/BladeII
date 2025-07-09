#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIGuildMark.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildMark : public UB2UIWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	void SetGuildMark(int32 InPatternIndex, int32 InPetternColor, int32 InBGImage, int32 InBGColor);

protected:
	TWeakObjectPtr<UB2Image>		IMG_Pattern;
	TWeakObjectPtr<UB2Image>		IMG_BG;
	TWeakObjectPtr<UB2Image>		IMG_Border;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		FLinearColor				PatternColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		FLinearColor				BGColor;

};
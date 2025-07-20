// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UICharacterRectIcon.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UICharacterRectIcon : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void SelectCharacterClass(EPCClass CharType);
	virtual void Init() override;

protected:
	virtual void CacheAssets() override;

private:
	EPCClass						EntryPCClass;
	TWeakObjectPtr<UImage>			IMG_Character;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<UMaterialInterface*>		CharacterTexture;
};

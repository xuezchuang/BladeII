// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/B2UICommonTab.h"
#include "B2UIPSubQuestTab.generated.h"

/**
 * 
 */
UCLASS()
class UB2UIPSubQuestTab : public UB2UICommonTab
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TMap<int32, UMaterialInstance*> SelectedImanges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TMap<int32, UMaterialInstance*> UnselectedImanges;

	void SetIcon(int32 InDifficulty);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UB2Image> IMG_IconSelected;
	TWeakObjectPtr<UB2Image> IMG_IconUnselected;
};

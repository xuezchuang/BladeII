// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "../Widget/B2Image.h"
#include "B2UIPMainQuestTab.generated.h"

/**
 *
 */
UCLASS()
class BLADEII_API UB2UIPMainQuestTab : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, TSoftObjectPtr<UTexture2D>>	TabNormalBG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, TSoftObjectPtr<UTexture2D>>	TabDisableBG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor	TitleSelectColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor	CountSelectColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor	TitleNormalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor	CountNormalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor	TitleDisableColor;

	void Update(int32 InActID, bool InIsLock, int32 QuestID);
	void SetSelected(bool InValue);

protected:
	UFUNCTION()
	void OnClickBTN_Tab();

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	TWeakObjectPtr<UB2Image> IMG_TabBG_Normal;
	TWeakObjectPtr<UB2Image> IMG_TabBG_Disabled;
	TWeakObjectPtr<UB2Image> IMG_SelectFrame;
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_Count;
	TWeakObjectPtr<UB2Image> IMG_Lock;
	TWeakObjectPtr<UB2Button> BTN_Tab;
	int32 ActID;
	bool IsLock;
};

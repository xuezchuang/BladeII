// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "../B2RichTextBlock.h"
#include "B2UICostumeItemDetailSetEffect.generated.h"

USTRUCT(BlueprintType)
struct FCostumeSetItemIconInfo
{
	GENERATED_USTRUCT_BODY()

	FCostumeSetItemIconInfo() : ItemClass(EItemClass::EIC_End), ItemSimpleIconOn(nullptr), ItemSimpleIconOff(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemClass ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ItemSimpleIconOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ItemSimpleIconOff;

	TWeakObjectPtr<UB2Image>	IMG_Icon;
};

UCLASS()
class BLADEII_API UB2UICostumeItemDetailSetEffect : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

public:
	void InitCostumeItemDetailSetEffect(const FB2Item& InItem);

private:
	void UpdatePrefixText(const int32& iSetGroupID);
	void UpdateStarGradePart(const FB2Item& InItem);
	void UpdateSetEffectIcon(const EPCClass& InPCClass, const FMD_SetInfo* pSetItemInfo);
	void UpdateSetEffectText(const EPCClass& InPCClass, const int32 iSetOptionID, const FMD_SetInfo* pSetItemInfo);

private:
	void SetItemIconImg(const EItemClass& InItemClass, const bool& IsEquipped);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeSetItemIconInfo")
	TArray<FCostumeSetItemIconInfo>				ArrayCostumeSetItemIconInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor ApplySetTextColor;

private:
	TWeakObjectPtr<UB2RichTextBlock>			RTB_SetOptionTitle;

	TWeakObjectPtr<UB2RichTextBlock>			RTB_Prefix;

	TWeakObjectPtr<class UB2UIStarGradePart>	UIP_StarGradePart;

	TArray<TWeakObjectPtr<class UB2DynText_Multiple>>	ArraySetEffectText;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2FloatingWidget.h"
#include "B2UIEnum.h"
#include "B2FloatingAbnormalEffect.generated.h"

UENUM(BlueprintType)
enum class EFloatingAbnormalEffectArrowType : uint8
{
	FAXAT_None,
	FAXAT_Up,
	FAXAT_Down
};

/** Intended to be used for setting up lazy-loaded abnormal effect text asset. */
USTRUCT(BlueprintType)
struct FFloatingAbnormalEffectSetupInfo
{
	GENERATED_USTRUCT_BODY()

	FFloatingAbnormalEffectSetupInfo()
	{
		ArrowType = EFloatingAbnormalEffectArrowType::FAXAT_None;
		AdditionalZOffset = 0.0f;
	}
	
protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UTexture2D> TextEffectTexture; // It is just provided as texture, not much reason here.

public:
	UPROPERTY(EditAnywhere)
	EFloatingAbnormalEffectArrowType ArrowType;

	/** Additional to the AddtionalZOffset of FloatingAbnormalEffect widget */
	UPROPERTY(EditAnywhere)
	float AdditionalZOffset;

#if WITH_EDITOR
	// Provided only for EditorLoadAll function.
	void EditorOnlySimpleLoad()
	{
		TextEffectTexture.LoadSynchronous();
	}
#endif
	bool MarkLoadedAssetRootSet()
	{
		if (TextEffectTexture.IsValid()) // Not loading here, must be loaded already.
		{
			TextEffectTexture->AddToRoot();
			return true;
		}
		return false;
	}

	friend struct FLoadedFloatingAbnormalEffectSetupInfo;
};

/** Contains loaded asset information of lazy-loaded FFloatingAbnormalEffectSetupInfo */
USTRUCT(BlueprintType)
struct FLoadedFloatingAbnormalEffectSetupInfo
{
	GENERATED_USTRUCT_BODY()

	FLoadedFloatingAbnormalEffectSetupInfo()
	{
		LoadedTextEffectTexture = NULL;
		ArrowType = EFloatingAbnormalEffectArrowType::FAXAT_None;
		AdditionalZOffset = 0.0f;
	}

	FLoadedFloatingAbnormalEffectSetupInfo(FFloatingAbnormalEffectSetupInfo& InSetup, FStreamableManager& InStreamManager)
	{
		if (!InSetup.TextEffectTexture.IsValid() && !InSetup.TextEffectTexture.IsNull())
		{
			// Simple InSetup.TextEffectTexture.LoadSynchronous() can cause FlushAsyncLoading,
			// which is not desired especially because this will be used at anytime during game-play
			InStreamManager.LoadSynchronous(InSetup.TextEffectTexture);
		}

		LoadedTextEffectTexture = InSetup.TextEffectTexture.Get();
		ArrowType = InSetup.ArrowType;
		AdditionalZOffset = InSetup.AdditionalZOffset;
	}

	UPROPERTY()
	class UTexture2D* LoadedTextEffectTexture; 
		
	EFloatingAbnormalEffectArrowType ArrowType;

	float AdditionalZOffset;

	void ClearLoaded() 
	{
		LoadedTextEffectTexture = nullptr;
	}
};

/**
 * When an abnormal condition occurs notifying purpose. (Critical, Fury, and the abnormal states. )
 */
UCLASS()
class BLADEII_API UB2FloatingAbnormalEffect : public UB2FloatingWidget
{
	GENERATED_BODY()
	
protected:

	/** Visible duration from created moment. It is also the lifetime of an instance of this widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float VisibleDuration;

	/** Its LocOffsetFromOwner will be automatically set based on owner character's height, and set this if you want give additional z offset.. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float AdditionalZOffset;

	/** Based on this distance, X and Y of LocOffsetFromOwner will be randomly calculated. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float RandomPlaceDistanceFromOwner;

	// For some abnormal effect requiring inc/dec arrow sign.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UTexture2D* UpArrowTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UTexture2D* DownArrowTexture;

	/** It is initially calculated, and now being the final static screen position while it exists, instead of following owner character. */
	FVector2D InitialProjectedLocation;

	TWeakObjectPtr<UImage> IMG_AbnormalEffectText;
	TWeakObjectPtr<UImage> IMG_Arrow;

public:

	UB2FloatingAbnormalEffect(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void SetOwnerCharacter(class ABladeIICharacter* InOwner) override;

	void NotifyAbnormalEffect(const FLoadedFloatingAbnormalEffectSetupInfo& InEffectSetupInfo);

	/** For additional blueprint abnormal effect event handling */
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyAbnormalEffect_BP(EFloatingAbnormalEffectArrowType InArrowType);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool IsOwnerPC();

	FORCEINLINE float GetVisibleDuration() { return VisibleDuration; }

private:
	/** Update LocOffsetFromOwner based on the character bound size and additional parameter. */
	void UpdateFloatingOffset();
};

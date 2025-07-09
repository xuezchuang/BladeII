// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "FairyManager.h"
#include "B2UIFairyButton.generated.h"

DECLARE_DELEGATE_OneParam(FFairyButtonOnClick, EFairyType)

namespace EFairyButtonStatus
{
	enum Type : uint8
	{
		NONE,
		LOCK,
		DISABLED,
		ENABLED,
	};
}

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyButton : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetFairyBtnHandler(const FFairyButtonOnClick& InBtnHandler);

	void SetFairyInfo(FB2FairyStatusInfo &Status);
	void SetFairyButtonStatus(EFairyButtonStatus::Type Status);
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;

	void ChangeButtonState(EFairyButtonStatus::Type Status);

	void SetFairyType(EFairyType FairyType) { CurFairyType = FairyType; }

	UFUNCTION()
	void OnClickBTN_Fairy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TMap<EFairyType, TSoftObjectPtr<class UMaterialInterface>>		NPCImage;

private:
	FFairyButtonOnClick OnClickBtnHandler;

	EFairyButtonStatus::Type CachedButtonStatus;
	EFairyType CurFairyType;

	TWeakObjectPtr<UImage> IMG_FairyLcok;
	TWeakObjectPtr<UImage> IMG_FairyUnlcok;
	TWeakObjectPtr<UImage> IMG_Lock;

	TWeakObjectPtr<UB2Button> BTN_Fairy;
	TWeakObjectPtr<UWidgetSwitcher> WS_Image;
	TWeakObjectPtr<UTextBlock> TB_LockText;
	
};

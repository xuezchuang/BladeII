// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIWidgetBase.h"
#include "B2UITutorialLockContents.generated.h"

class TutorialManager;

USTRUCT(BlueprintType)
struct FCustomImageInfos
{
	GENERATED_USTRUCT_BODY()

	FCustomImageInfos() : IsVisibleImage(false)	 {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool			IsVisibleImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush		ImageBrush;	
};

UENUM(BlueprintType)
enum class ETutorialLockType : uint8
{
	EOverrideLock,			// 譬配府倔捞 努府绢登瘤 臼疽栏搁 Lock捞固瘤(滚瓢)甫 畴免 (滚瓢阑 穿福瘤给窍霸 阜阑锭 荤侩)
	EParentHidden,			// 譬配府倔捞 努府绢登瘤 臼疽栏搁 何葛甫 Hidden贸府窃. (滚瓢阑 酒抗 救焊咯林扁困秦辑 荤侩)
};

UCLASS()
class BLADEII_API UB2UITutorialLockContents : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UITutorialLockContents(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	
public:
	virtual void SynchronizeProperties() override;
	virtual void Init() override;
#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void UpdateDynamicWidgets();
	void UpdateTutorialLockText();
	void UpdateLockImage();
	void CheckDestroySelfByOpenedTutorial();
	void UnBindTutorialDoc();

public:
	UFUNCTION()
	void OnBTNClickLockContents();

	void OnLastTutorialId(class TutorialManager* Sender, int32 CurTutorialId, int32 PreTutorialId);
	void OnChanagedTutorialId(class TutorialManager* Sender, int32 CurTutorialId, int32 PreTutorialId);
	void OnChanagedTutorialStepId(class TutorialManager* Sender, int32 CurTutorialStepId, int32 PreTutorialStepId);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial")
	int32 TutorialID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial")
	int32 TutorialStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial Image")
	ETutorialLockType LockType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial Image")
	FCustomImageInfos	CustomBackGroundImageInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial Image")
	FCustomImageInfos	CustomLockImageInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial Image")
	FVector2D LockImageOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial Image")
	FVector2D LockImageScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial Text")
	bool IsVisibilityLockTitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial Text")
	bool IsVisibilityLockDescText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Totorial Text")
	FVector2D LockTextOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII Totorial Text")
	FSlateColor OverrideLockTitleColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII Totorial Text")
	FSlateColor OverrideLockDescColor;

private:
	TWeakObjectPtr<UButton> BTN_Click;
	TWeakObjectPtr<UImage> IMG_BG;
	TWeakObjectPtr<UImage> IMG_DefaultBG;
	TWeakObjectPtr<UImage> IMG_Lock;
	TWeakObjectPtr<UImage> IMG_DefaultLock;

	TWeakObjectPtr<UOverlay> X_O_Padding;
	TWeakObjectPtr<UOverlay> X_O_ConditionText;
	TWeakObjectPtr<UTextBlock> TB_Condition1;
	TWeakObjectPtr<UTextBlock> TB_Condition2;
};

// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "BladeIIUtil.h"
#include "B2AssetLoader.h"
#include "B2UIBattleTagAnim.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUIBattleTagAnimExpire, UB2UIBattleTagAnim*, ExpiredUIP);

/**
 * Resource setup.. tried to make it unified as much as we can..
 */
USTRUCT()
struct FUITagAnimSetupPerPCClass
{
	GENERATED_USTRUCT_BODY()

	FUITagAnimSetupPerPCClass()
	{

	}
	
protected:
	// The "major" tag anim pictures are 1 or 2 depend on the character.
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> MainPic_1;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> MainPic_2; // Might not being set depend on character class.

	// There are other textures but let them just being set on widget BP, unless they are in significant size..

public:
	class UMaterialInterface* GetMainPic_1();
	class UMaterialInterface* GetMainPic_2();
};

/** 
 * UMG widget set per PC class, their image brush to be set by its relevant FUITagAnimSetupPerPCClass
 */
USTRUCT()
struct FUITagAnimWidgetCollectionPerPCClass
{
	GENERATED_USTRUCT_BODY()

	FUITagAnimWidgetCollectionPerPCClass()
	{

	}

	// Those UImages below supposed to be empty without actual image not being setup
	// The required brush material will be dynamically loaded from relevant FUITagAnimSetupPerPCClass struct.

	TWeakObjectPtr<UImage> IMG_Main_01;
	TWeakObjectPtr<UImage> IMG_Main_02; // Might not be available depend on character class.
	TWeakObjectPtr<UTextBlock> TB_CharName;

	void CacheAssetsForChar(UB2UIWidget* InOwnerWidget, EPCClass InCharClass);

	FORCEINLINE FString GetWidgetNamePostFix(EPCClass InCharClass) const {
		if (InCharClass != EPCClass::EPC_End)
		{ // Widgets placed on BP must follow these naming rule.
			return (InCharClass == EPCClass::EPC_Gladiator) ? TEXT("Gladiator") :
				((InCharClass == EPCClass::EPC_Assassin) ? TEXT("Assassin") :
				((InCharClass == EPCClass::EPC_Wizard) ? TEXT("Wizard") : TEXT("Fighter")));
		}
		return TEXT("Unknown");
	}

	void UpdateWidgets(EPCClass InCharClass, FUITagAnimSetupPerPCClass* InResoureSetup);
};

/** 
 * UI Tag Animation commonly used for several battle UI and TAsset loaded..
 */
UCLASS()
class BLADEII_API UB2UIBattleTagAnim : public UB2UIWidget
{
	GENERATED_BODY()

protected:

	/** Dynamically created instance of this need to be destroyed after this time.
	 * It is about saving memory, no need to be that precise.
	 * Counted on animation beginning. */
	UPROPERTY(EditAnywhere)
	float TotalRunningPeriod;

	/** Main resource setup per character class. */
	UPROPERTY(EditAnywhere)
	FUITagAnimSetupPerPCClass ResourceSetup_Gladiator;
	UPROPERTY(EditAnywhere)
	FUITagAnimSetupPerPCClass ResourceSetup_Assassin;
	UPROPERTY(EditAnywhere)
	FUITagAnimSetupPerPCClass ResourceSetup_Wizard;
	UPROPERTY(EditAnywhere)
	FUITagAnimSetupPerPCClass ResourceSetup_Fighter;
public:
	/** Main resource setup accessor util. */
	FUITagAnimSetupPerPCClass* GetResourceSetup(EPCClass InCharClass);

protected:
	
	/** The outer widget panel containing widgets for all pc classes. The ActiveWidgetIndex is to be set for PCClass */
	TWeakObjectPtr<UWidgetSwitcher> WS_TagEffectPerPCSet;

	/** Structs containing widgets per character. Each of them is in a panel within WidgetSwitcher, to be activated by setting index.
	 * Could not entirely unify them due to a little differences per class. */
	TArray<FUITagAnimWidgetCollectionPerPCClass> CachedWidgetSets;
	FUITagAnimWidgetCollectionPerPCClass* GetWidgetSetForChar(EPCClass InCharClass);

public:
	UB2UIBattleTagAnim(const FObjectInitializer& ObjectInitializer);

	// Delegate to the owner that create this instance
	FOnUIBattleTagAnimExpire OnTotalPeriodExpireDelegate;

	// For calling main operation here..
	void BeginTagAnim(EPCClass InCharClass, bool bIsTagAttack);

protected:
	// For actual Blueprint anim invoking
	UFUNCTION(BlueprintImplementableEvent, Category="BladeII")
	void BeginTagAnimBP(EPCClass InCharClass, bool bIsTagAttack);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
private:
	bool bEventsSubscribed;
	void SubscribeEvents();
	void UnsubscribeEvents();

	uint32 StartPauseMenuTicket;
	uint32 StopPauseMenuTicket;

	uint32 StartVisibleChatPopupTicket;
	uint32 StopVisibleChatPopupTicket;

	void OnPause(bool bPaused);

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	FTimerHandle TotalPeriodExpireTH;
	void TotalPeriodExpireCB();
	
};

/** Common util to use UB2UIBattleTagAnim as intended. */
template< class OwnerWidgetClass >
UB2UIBattleTagAnim* LoadAndBeginTAssetBattleTagAnimUIP(OwnerWidgetClass* InOwnerWidget, TSoftClassPtr<UB2UIBattleTagAnim>& InUIPTAssetToLoad, TSubclassOf<UB2UIBattleTagAnim>* OutLoadedPtrCache,
	EPCClass NewTaggedClass, bool bIsTagAttack,
	int32 ViewportZOrder = BII_WIDGET_ZORDER_SEPARATE_ABOVE_UIMAIN)
{
	UB2UIBattleTagAnim* CreatedTagAnimUIP = NULL;
	if (InOwnerWidget && OutLoadedPtrCache)
	{
		GetSomeLazyLoadedClassAssetCommon<UB2UIBattleTagAnim>(UB2AssetLoader::GetStreamableManager(), InUIPTAssetToLoad, OutLoadedPtrCache);
		if (OutLoadedPtrCache)
		{
			CreatedTagAnimUIP = CreateWidget<UB2UIBattleTagAnim>(InOwnerWidget->GetOwningPlayer(), *OutLoadedPtrCache);
			if (CreatedTagAnimUIP)
			{
				// It won't be hidden if all other UIs are hidden by cheat, cinematic or anything else.. because it is not a child of other managed UI widget. 
				// Currently not much problem. but better aware of it.
				CreatedTagAnimUIP->AddToViewport(ViewportZOrder); // Could not do the same as what UIManager doing AddChild.
				CreatedTagAnimUIP->Init();
				CreatedTagAnimUIP->BeginTagAnim(NewTaggedClass, bIsTagAttack);

				// Register callback to be called on expire.. mainly to destroy created one here.. 
				// OwnerWidgetClass must declare member method OnBattleTagAnimUIPExpired as public in format can be bound to OnTotalPeriodExpireDelegate
				CreatedTagAnimUIP->OnTotalPeriodExpireDelegate.BindDynamic(InOwnerWidget, &OwnerWidgetClass::OnBattleTagAnimUIPExpired);
			}
		}
	}
	return CreatedTagAnimUIP;
}

FORCEINLINE void UnloadTAssetBattleTagAnimUIP(TSoftClassPtr<UB2UIBattleTagAnim>& InUIPTAssetToLoad, TSubclassOf<UB2UIBattleTagAnim>* OutLoadedPtrCache)
{ // Force to use the same StreamableManager
	UnloadSomeLazyLoadedClassAssetCommon<UB2UIBattleTagAnim>(UB2AssetLoader::GetStreamableManager(), InUIPTAssetToLoad, OutLoadedPtrCache);
}
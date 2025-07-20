#pragma once

#include "B2UIWidget.h"
#include "BladeIIUserWidget.h"
#include "B2UIMailSelectiveLotteryResult.generated.h"

UCLASS()
class BLADEII_API UB2UIMailSelectiveLotteryResult : public UB2UIWidget
{
	GENERATED_BODY()

	FB2Item ItemToShow; // After enhancement

public:
	void OpenPopup(const FB2Item& InItem); // To update target item icon.

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void OnOpen(bool RightNow = false) override;

protected:

	//================================================================================
	// For UMG item icon creation stuff

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_ItemOpScene> ItemIconWidgetClass;

	/** Enhance target item icon created inside CP_ItemIconSet */
	UPROPERTY(Transient)
	class UB2DynItemIcon_ItemOpScene* CreatedTargetItemIcon;
	/** Center pos of created target item icon in screen space. */
	FVector2D TargetItemIconScreenPos;
	//================================================================================


	//================================================================================
	// For 3D effect stuff


	/** Global time dilation to be applied during this page. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	float OverallPlayRate;

	/** Setup for overall background. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FSingleUMGParticleSetup OverallBGFxSetup;
	UPROPERTY(Transient)
	UParticleSystemComponent* CreatedOverallBGFx;

	/** Setup for blowing effect filling up screen. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FSingleUMGParticleSetup EnhanceBlowupFxSetup;
	UPROPERTY(Transient)
	UParticleSystemComponent* CreatedEnhanceBlowupFx;

	/** Setup for background effect behind target item icon. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FSingleUMGParticleSetup TargetItemBGFxSetup;
	UPROPERTY(Transient)
	UParticleSystemComponent* CreatedTargetItemBGFx;

	/** Setup for final effect showing glorious enhancement success. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FSingleUMGParticleSetup FinalEnhanceSuccessFxSetup;
	UPROPERTY(Transient)
	UParticleSystemComponent* CreatedFinalEnhanceSuccessFx;

	//================================================================================	
private:

	// Names defined for NamedMaterialSlots of particle system template.
	const static FName Name_FxMID_ItemBGPanel;
	const static FName Name_FxMID_ItemIcon;
	// They are just GetFxNamedMID, for some defined names..
	class UMaterialInstanceDynamic* GetFxMID_ItemBGPanel(class UParticleSystemComponent* InPSComp);
	class UMaterialInstanceDynamic* GetFxMID_ItemIcon(class UParticleSystemComponent* InPSComp);

protected:

	void DestroyTargetItemIcon();

	void DestroyOverallBGFx();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void DestroyEnhanceBlowupFx();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void DestroyTargetItemBGFx();
	void DestroyFinalEnhanceSuccessFxComp();

protected:

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowOverallBGFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowEnhanceBlowupFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowTargetItemBGFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowFinalEnhanceSuccessFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetTargetItemIconVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowFinalResultPage();


	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartDirecting();

private:

	FVector GetTargetIconDesiredWorldPos(float Depth);

	UFUNCTION()
	void FinishAndProceedToResult();

private:
	TWeakObjectPtr<UCanvasPanel> CP_TargetItemIconSet; // Where the target item icon is placed.
	TWeakObjectPtr<UCanvasPanel> CP_EnhanceIngredIconPanel; // Where the ingredient item icons are placed and aligned
	TWeakObjectPtr<class UB2LobbyUI_ItemDetailPostOp> ItemDetailPopup;
};

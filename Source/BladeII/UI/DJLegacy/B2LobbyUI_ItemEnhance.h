// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_ItemEnhance.generated.h"

const int32 ITEM_ENHANCE_MENU_INGREDIENT_ICON_PER_ROW = 2;

/**
 * The first (in fact, the main) step window of Enhnace item feature.
 */

USTRUCT(BlueprintType)
struct BLADEII_API FB2LobbyItemEnhanceInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 CompareValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FColor ViewColor;
};

UCLASS()
class BLADEII_API UB2LobbyUI_ItemEnhance : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	/** The item data to be the target of Enhance. It is not supposed to be changed once pop-up this page. */
	FB2Item NativeTargetItem;
	FB2Item NativeTargetEnhanceLevelUpItem;

protected:
	TWeakObjectPtr<UTextBlock> TB_EnhanceTitle;		
	TWeakObjectPtr<UTextBlock> TB_EnhanceCommitBtn;
	TWeakObjectPtr<UB2Button> BTN_EnhanceCommit;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UTextBlock> TB_EnhanceCostNum;
	TWeakObjectPtr<UCanvasPanel> CP_EnhanceTargetItem;
	TWeakObjectPtr<UCanvasPanel> CP_EnhanceIngredItem;
	TWeakObjectPtr<UCanvasPanel> CP_SelectItemExplain;
	TWeakObjectPtr<UCanvasPanel> CP_EnhanceExplain;
	
	TWeakObjectPtr<UB2Button> BTN_EnhanceIngredItem;

	TWeakObjectPtr<UTextBlock> TB_NeedIngredItem;
	TWeakObjectPtr<UTextBlock> TB_NeedIngredItemCount;
	TWeakObjectPtr<UB2RichTextBlock> RTB_IngredItemCount;

	TWeakObjectPtr<UTextBlock> TB_SelectItemExplainTitle;
	TWeakObjectPtr<UB2RichTextBlock> RTB_SelectItemExplain;	
	TWeakObjectPtr<UTextBlock> TB_ProtectionItem;
	TWeakObjectPtr<UB2RichTextBlock> RTB_ProtectionItemCount;
	TWeakObjectPtr<UTextBlock> TB_ProtectionItemNotice;

	TWeakObjectPtr<UTextBlock> TB_StatValueOld;
	TWeakObjectPtr<UTextBlock> TB_StatValueNew;
	TWeakObjectPtr<UTextBlock> TB_StatIncreaseValue;
	TWeakObjectPtr<UTextBlock> TB_StatKey;
	TWeakObjectPtr<UB2RichTextBlock> RTB_ProbabilityOfSuccess;
	TWeakObjectPtr<UB2RichTextBlock> RTB_EnhanceFailNotice;
	TWeakObjectPtr<UCheckBox>	CB_UseProtectionItem;

protected:
	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;

	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedTargetItemIcon;

	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedIngredientItemIcon;

	/** To sync Enhance commit animation and the real enhancement operation start. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float EnhanceCommitAnimLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Enhance")
	FColor EnhanceEnable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Enhance")
	FColor EnhanceDisable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Enhance")
	TArray<FB2LobbyItemEnhanceInfo> EnhanceConditions;
public:
	UB2LobbyUI_ItemEnhance(const FObjectInitializer& ObjectInitializer);
	virtual void CacheAssets() override;
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	void DestroySelf() override;

	/** Mainly to be called when Enhance ingredient selection is changed. */
	void UpdateOnEnhanceStateChange();

private:
	void UpdateStaticTexts();
	void UpdateDynamicTexts();
	void UpdateEnhanceIngredientItemIcon(); // To be updated whenever enhance ingredients are changed.
	void UpdateEnhanceTargetItemIcon();
	void UpdateEnhanceExplain(); // To be updated whenever enhance ingredients are changed.
	void UpdateItemEnhanceCommitBtnState();	
	void UpdateIsProtectionItemCheckBoxState();

	FText GetProbabilityOfSuccessText();
	FText GetIngredColorFormat(const int32 PossessionCount, const int32 NeedCount, const FText &AppendText);
public:
	UFUNCTION()
	void OnEnhanceCommit();

	UFUNCTION()
	void OnCloseButtonPressed();

	UFUNCTION()
	void OnClickEnhanceIngredItem();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnChangeStateIsProtectionItemCheckBox(bool IsCheck);

private:
	void CloseAndQuitItemOpMode();
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void BeginEnhanceCommitAnim();

	FTimerHandle ItemEnhanceCommintTH;
	void FinalItemEnhanceCommit();

	virtual void CloseWidgetDelegate() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "B2LobbyUI_ItemEnhanceChangeMain.generated.h"

/**
 * 
 */
class issue;
using issue_ptr = std::shared_ptr<issue>;

UCLASS()
class BLADEII_API UB2LobbyUI_ItemEnhanceChangeMain : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
	enum DescriptionState
	{
		BEFRE,
		AFTER,
		RESULT,
	};

public:
	UFUNCTION()
	void OnCloseButtonPressed();

	UFUNCTION()
	void OnClickIngredUnselectBtn();

	UFUNCTION()
	void OnClickEnhanceChangeBtn();

	UFUNCTION()
	void OnClickSkipResultBtn();

	UFUNCTION(BlueprintCallable)
	void OnFinishEnhanceChange();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimationSound();

	virtual void CacheAssets() override;
	virtual void DestroySelf() override;
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void UpdateStateChange();

private:
	void CloseAndQuitItemOpMode();
	void DestroyTargetItemIcon();
	void DestroyIngredItemIcon();

	void UpdateStaticText();
	void UpdateIngredItemIcon();
	void UpdateItemIcons();

	void UpdateDescriptionState(DescriptionState InState);
	void UpdateUnselectDescription();
	void UpdateSelectDescription();
	void UpdateResultDescription();

	void FindIngredItem();
	void RequestEnhanceChange();
	void PlayEnhanceChangeAnimation();

	void DeliveryItemEnhanceLevel(FB2ItemEnhanceLevel InEnhanceLevelInfo);

protected:
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
		class UB2LobbyInventory* CachedInventory;

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
		TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;

	UPROPERTY(Transient)
		class UB2DynItemIcon_LobbyInven* CreatedTargetItemIcon;

	UPROPERTY(Transient)
		class UB2DynItemIcon_LobbyInven* CreatedIngredItemIcon;

	TArray<issue_ptr>	Issues;

	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_IngredUnselect;
	TWeakObjectPtr<UB2Button> BTN_EnhanceChange;
	TWeakObjectPtr<UB2Button> BTN_SkipResult;

	TWeakObjectPtr<UCanvasPanel> CP_TargetItemIconSet;
	TWeakObjectPtr<UCanvasPanel> CP_IngredItemIconSet;

	TWeakObjectPtr<UPanelWidget> P_SelectDescription;
	TWeakObjectPtr<UPanelWidget> P_UnselectDescription;
	TWeakObjectPtr<UPanelWidget> P_ResultDescription;

	TWeakObjectPtr<UTextBlock> TB_BeforeLeftEnhance;
	TWeakObjectPtr<UTextBlock> TB_BeforeEnhance;
	TWeakObjectPtr<UTextBlock> TB_BeforeRightEnhance;

	TWeakObjectPtr<UTextBlock> TB_AfterLeftEnhance;
	TWeakObjectPtr<UTextBlock> TB_AfterRightEnhance;
	TWeakObjectPtr<UTextBlock> TB_AfterEnhance;

	TWeakObjectPtr<UTextBlock> TB_LeftResultEnhance;
	TWeakObjectPtr<UTextBlock> TB_LeftResultEnhanceBefore;
	TWeakObjectPtr<UTextBlock> TB_CurrentEnhance;
	TWeakObjectPtr<UTextBlock> TB_RightResultEnhance;
	TWeakObjectPtr<UTextBlock> TB_RightResultEnhanceBefore;

	TWeakObjectPtr<UTextBlock> TB_LeftResultAttack;
	TWeakObjectPtr<UTextBlock> TB_LeftResultAttackBefore;
	TWeakObjectPtr<UTextBlock> TB_CurrentAttack;
	TWeakObjectPtr<UTextBlock> TB_RightResultAttack;
	TWeakObjectPtr<UTextBlock> TB_RightResultAttackBefore;

	TWeakObjectPtr<UTextBlock> TB_EnhanceChangeCost;
	TWeakObjectPtr<UTextBlock> TB_EnhanceChangeTitle;
	TWeakObjectPtr<UTextBlock> TB_EnhanceChangeBtn;
	TWeakObjectPtr<UTextBlock> TB_NoIngredMessage;
	TWeakObjectPtr<UTextBlock> TB_ComposeDescription;

	TWeakObjectPtr<UImage>  IMG_BackGround;
	TWeakObjectPtr<UImage>	IMG_EnhanceChangeIcon;
	TWeakObjectPtr<UWidgetAnimation> ANIM_EnhanceChange;

	FB2Item TargetItem;
	FB2Item SourceItem;

	int32 TargetItemEnhanceLevel;
	int32 TargetItemAttackPower;

	int32 SourceItemEnhanceLevel;
	int32 SourceItemAttackPower;

	TArray<int64> CurrIngredIDs;
};

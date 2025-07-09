// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "BladeIIPlayer.h"
#include "B2LobbyUI_ItemEnhUpgCommon.generated.h"

/**
 * Common part of item enhancement and upgrade UI.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemEnhUpgCommon : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

protected:

	/** The item data to be the target of Enhancement or Upgrade. It is not supposed to be changed once pop-up this page. */
	FB2Item NativeTargetData;
	
	TWeakObjectPtr<UCanvasPanel> X_CP_ItemIconSet;

	TWeakObjectPtr<UCanvasPanel> P_StatSet;
	TWeakObjectPtr<UTextBlock> TB_PrimPointType;
	TWeakObjectPtr<UTextBlock> TB_CurrentPrimPoint;
	TWeakObjectPtr<UTextBlock> TB_PostOpPrimPoint;
	TWeakObjectPtr<UVerticalBox> VB_OptionStats;
	TWeakObjectPtr<UVerticalBox> VB_OptionStats_Upgrade;

	/** For enhance menu if true, upgrade menu if false. */
	uint32 bIsForEnhanceMenu : 1;

	//////////////////////////////////////////////////////////////////////
	// For item icon

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;

	/** Enhance/Upgrade target item icon created inside CP_ItemIconSet */
	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedTargetItemIcon;

	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// For option texts. Enhance menu displays random options, upgrade menu displays both options.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2DynText_Multiple> IntrinsicOptionTitleClass;
	UPROPERTY(Transient)
	class UB2DynText_Multiple* CreatedIntrinsicOptionTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2DynText_Multiple> RandomOptionTitleClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2DynText_Multiple> RandomOptionNewClass;

	UPROPERTY(Transient)
	class UB2DynText_Multiple* CreatedRandomOptionTitle;
	UPROPERTY(Transient)
	class UB2DynText_Multiple* CreatedRandomOptionNew;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2DynText_Multiple> IntrinsicOptionDisplayClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2DynText_Multiple> RandomOptionDisplayClass;

	// It is for both intrinsic and random options
	UPROPERTY(Transient)
	TArray<class UB2DynText_Multiple*> CreatedOptionStatTexts;

	//////////////////////////////////////////////////////////////////////

public:

	UB2LobbyUI_ItemEnhUpgCommon(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	/** Designate if it is for enhance menu or upgrade menu, as well as the target item data. */
	void SetItemOpTargetData(FB2Item& InItem, bool bInIsForEnhanceMenu);

	void DestroySelf() override;
	
private:
	void UpdateStaticTexts();

	void UpdateTargetItemIcon(); // Dynamically create enhance/upgrade target item icon, needed for the first time pop-up.
	
public:
	void UpdateOptionStatTexts(); // To be updated whenever enhance ingredients are changed.
private:
	/** Internal functionality of update current/post option stats text */
	void CreateSingleItemOptionStatText(class UVerticalBox* VBToCreateIn, TArray<class UB2DynText_Multiple*>& ArrayToPutCreatedText, int32 RowIndexInVB, bool bIsForIntrinsicOption, EItemOption InOptionType, float RawOptionValue_Current, float RawOptionValue_PostOp, EPCClass InDisplayPCClass);
	void CreateRandomOptionPreviewStatText(class UVerticalBox* VBToCreateIn, TArray<class UB2DynText_Multiple*>& ArrayToPutCreatedText, int32 RowIndexInVB, bool bIsForIntrinsicOption, EItemOption InOptionType, float RawOptionValue_Current, EPCClass InDisplayPCClass);

	void CreateIntrinsicOptionNew(class UVerticalBox* VBToCreateIn, TArray<class UB2DynText_Multiple*>& ArrayToPutCreatedText, int32 RowIndexInVB, bool bIsForIntrinsicOption, EItemOption InOptionType, float RawOptionValue_Current, float RawOptionValue_PostOp, EPCClass InDisplayPCClass);
	void CreateRandomOptionNew(int32 itemRef, int32 CurrentCount, bool bCreateTitle);
	void CreateLobbyItemAfterOptionPopup(UObject* WorldContextObject, FB2Item& InItemData);

	void DestroyTargetItemIcon();

	void DestroyOptionStatTexts();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void BeginEnhanceCommitAnim();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void BeginUpgradeCommitAnim();

	UFUNCTION()
	void OnClickedOptionInfo();
};

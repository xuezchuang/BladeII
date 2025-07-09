// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "BladeIIPlayer.h"
#include "B2LobbyUI_HeroMgmtCommon.generated.h"

/**
* Common part of several lobby UI, inventory, level-up, skill, etc.
*/

UCLASS()
class BLADEII_API UB2LobbyUI_HeroMgmtCommon : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
	EPCClass SelectedHeroMgmtPCClass; // First for display here, and to be set to LobbyGM
	
	void CyclePCSelection(bool bForward);
public:
	UPROPERTY(EditAnywhere)
	EHeroMgmtSubMode CurrentHeroMgmtSubMode; // Like SelectedHeroMgmtPCClass, to be set to LobbyGM

	EInventoryPresetMode CurrentPreserMode;

	uint32 bEventSubscribed : 1;
	uint32 SelectPresetSlotTicket;
protected:

	TWeakObjectPtr<class UB2UIHeader> UIP_CommonHeader; // A part to share the common functionalities with separated system.
	
	TWeakObjectPtr<UB2Button> BTN_EnhanceItemSet;
	TWeakObjectPtr<class UB2LobbyUI_HeroMgmtShowWingSet> UIP_ShowWingSet;
	TWeakObjectPtr<UCheckBox> CB_ShowHead;

	TWeakObjectPtr<UTextBlock> TB_GoldAmount;
	TWeakObjectPtr<UTextBlock> TB_GemAmount;

	TWeakObjectPtr<UTextBlock> TB_CharLevel;
	TWeakObjectPtr<UProgressBar> PB_CharExpProgress;
	TWeakObjectPtr<UTextBlock> TB_CharExpText;

	TWeakObjectPtr<UTextBlock> TB_HealthLabel;
	TWeakObjectPtr<UTextBlock> TB_HealthPoint;

	TWeakObjectPtr<UTextBlock> TB_AttackLabel;
	TWeakObjectPtr<UTextBlock> TB_AttackPoint;

	TWeakObjectPtr<UTextBlock> TB_DefenseLabel;
	TWeakObjectPtr<UTextBlock> TB_DefensePoint;

	TWeakObjectPtr<UTextBlock>	TB_CombatLable;
	TWeakObjectPtr<UTextBlock>	TB_CombatPower;


	// Outer panel of each character selection set.
	TWeakObjectPtr<UPanelWidget>		P_HeroGladiator;
	TWeakObjectPtr<UPanelWidget>		P_HeroAssassin;
	TWeakObjectPtr<UPanelWidget>		P_HeroWizard;
	TWeakObjectPtr<UPanelWidget>		P_HeroFighter;
	// Other images and/or buttons for each character are under the panel for each character.
	TWeakObjectPtr<UImage>			IMG_Select_Gladiator;
	TWeakObjectPtr<UImage>			IMG_Select_Assassin;
	TWeakObjectPtr<UImage>			IMG_Select_Wizard;
	TWeakObjectPtr<UImage>			IMG_Select_Fighter;	
	TWeakObjectPtr<UImage>			IMG_LevelMax;
	TWeakObjectPtr<UB2Button>		BTN_SelectGladiator;
	TWeakObjectPtr<UB2Button>		BTN_SelectAssassin;
	TWeakObjectPtr<UB2Button>		BTN_SelectWizard;
	TWeakObjectPtr<UB2Button>		BTN_SelectFighter;
	TWeakObjectPtr<UTextBlock>		TB_GladiatorFull;
	TWeakObjectPtr<UTextBlock>		TB_AssassinFull;
	TWeakObjectPtr<UTextBlock>		TB_WizardFull;
	TWeakObjectPtr<UTextBlock>		TB_FighterFull;

	TWeakObjectPtr<UB2Button>		BTN_DetailedInfo;
	TWeakObjectPtr<UB2Button>		BTN_DetailedInfoExtra;

	TWeakObjectPtr<UPanelWidget>		HB_EquipPreset;
	TWeakObjectPtr<UPanelWidget>		HB_SkillPreset;
	
	TArray<TWeakObjectPtr<class UB2UIPHeroMgmtPreset>> EquipPresets;
	TArray<TWeakObjectPtr<class UB2UIPHeroMgmtPreset>> SkillPresets;

	TWeakObjectPtr<UCanvasPanel>	X_LeftPopupPanel;
	TWeakObjectPtr<UCanvasPanel>	X_RightPopupPanel;

	TWeakObjectPtr<UOverlay>	X_FullPopupPanel;

	TWeakObjectPtr<class UB2UIWidgetBase> IMG_MarkForNew_Gladiator;
	TWeakObjectPtr<class UB2UIWidgetBase> IMG_MarkForNew_Assassin;
	TWeakObjectPtr<class UB2UIWidgetBase> IMG_MarkForNew_Wizard;
	TWeakObjectPtr<class UB2UIWidgetBase> IMG_MarkForNew_Fighter;
public:

	UB2LobbyUI_HeroMgmtCommon(const FObjectInitializer& ObjectInitializer);
	
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void CacheAssets() override;
	virtual void RegisterUIMarkForRedDot() final;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void DestroySelf() override;

	void SelectPcClass(EPCClass pc_class);
	virtual void OnHeroPCChanged(EPCClass UpdatedClass);

	void ShowNewMark(EPCClass PCClass, bool bShow);

	void ShowCharacterButtons(bool bShow = false);
	void ShowEnhanceItemSetButton(bool bShow = false);
	void ShowWingButton(bool bShow = false);
	void ShowHeadButton(bool bShow = false);
	void ShowDetailedInfoExtraButtons(bool bShow = false);

	/** For temporarily disable buttons for any reason like some auto execution task.. */
	void SetExtraDisableButtons(bool bInDisable);

	EPCClass GetCurrentPCClass() const { return SelectedHeroMgmtPCClass; }
	
	void SwitchPresetState(EItemPresetSlotType SelectPreset);
	void SetPresetMode(EInventoryPresetMode PresetMode);
	void SetHidePresetControls();
	void SetInventoryFullText();
	// 
	void HidePresetWidget(EInventoryPresetMode PresetMode);

	void HeaderBackButton();

protected:
	virtual void InternalSubscribeEvents();
	virtual void InternalUnsubscribeEvents();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	void UpdateStaticTexts();
	void UpdateDynamicTexts();
		
	void SetCharacterTextColor(EPCClass eclass);
	void GetDetailPopupWidgetInfo(EHeroMgmtSubMode InHeroMgmtSubMode, UB2LobbyUserWidget** OutLobbyUserWIdget, UPanelWidget** OutPopupWidget);
	
protected:
	UFUNCTION()
	void OnGladiatorSelectButtonPressed();
	UFUNCTION()
	void OnAssassinSelectButtonPressed();
	UFUNCTION()
	void OnWizardSelectButtonPressed();
	UFUNCTION()
	void OnFighterSelectButtonPressed();

	UFUNCTION()
	void OnEnhanceItemSetButtonPressed();
	UFUNCTION()
	void OnDetailedInfoButtonPressed();
	UFUNCTION()
	void OnShowHeadButtonPressed(bool IsCheck);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	FText GetHeroMgmtText() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	FText GetNickNameText() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	FText GetDetailedInfoButtonText() const;

	/** To be called on charcter class change tab button, to change some stuff at BP. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void EventChangedHeroMgmtPCClass(EPCClass InSelectedClass);

	UFUNCTION()
	bool RedDotCondition_Gladiator();
	UFUNCTION()
	bool RedDotCondition_Assassin();
	UFUNCTION()
	bool RedDotCondition_Wizard();
	UFUNCTION()
	bool RedDotCondition_Fighter();

	bool RedDotCondition_BySubMode(EHeroMgmtSubMode InSubMode, EPCClass PCClass);

	virtual bool RedDotCondition_ByCostumeMode(EPCClass PCClass);

public:
	void RedDotCondition_ClassAllUpdate(EHeroMgmtSubMode InSubMode);

	FORCEINLINE UCanvasPanel* GetInfoPopupParentPanel(bool bOfLeftSide) const { return bOfLeftSide ? X_LeftPopupPanel.Get() : X_RightPopupPanel.Get(); }
	FORCEINLINE UOverlay* GetInfoPopupParentFullPanel() const { return X_FullPopupPanel.Get(); }
};

/** This is another commonly used (small) part of hero management mode, 
 * separated because layout seemed to be different for each sub-mode.. but just the same now..? */
UCLASS()
class BLADEII_API UB2LobbyUI_HeroMgmtShowWingSet : public UB2LobbyUserWidget
{
	GENERATED_BODY()

public:

	UB2LobbyUI_HeroMgmtShowWingSet(const FObjectInitializer& ObjectInitializer);

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void CacheAssets() override;

	virtual void DestroySelf() override;

protected:

	TWeakObjectPtr<UB2Button> BTN_ShowWing;
	TWeakObjectPtr<UImage> IMG_ShowWing_BGOn;
	TWeakObjectPtr<UImage> IMG_ShowWing_IconOn;
	TWeakObjectPtr<UImage> IMG_ShowWing_BGOff;
	TWeakObjectPtr<UImage> IMG_ShowWing_IconOff;
	TWeakObjectPtr<UTextBlock> TB_ShowWing;

	UFUNCTION()
	void OnClickedShowWingBtn();
public:
	void UpdateShowWingBtnImage();
};
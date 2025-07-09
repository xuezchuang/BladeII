// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "BladeIIPlayer.h"
#include "CommonStruct.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_CharStatDetail.generated.h"

DECLARE_DELEGATE(FDelegateCharStatDetailClose);


USTRUCT()
struct FStatTextInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FStatTextInfo();
	FStatTextInfo(EUnitedCombatOptions	InItemOption, float InStatValue, bool InEmptyOption, TWeakObjectPtr<class UB2DynText_Multiple> InCreatedStatTextBlock);
	~FStatTextInfo();

public:
	bool IsEmpty();
	bool IsEmptyValue();
	bool GetIsEmptyOption();

	float GetConvertValue();
public:
	EUnitedCombatOptions    ItemOption;
	float					StatValue;
	bool					IsEmptyOption;

public:
	UPROPERTY(Transient)
	TWeakObjectPtr<class UB2DynText_Multiple>	CreatedStatTextBlock;
};


/**
 * Displaying a player character's various stats influenced by item equipment.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_CharStatDetail : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CloseWidgetDelegate() override;

	/** Vertical box panel listing offensive stats. */
	TWeakObjectPtr<UVerticalBox> VB_OffensiveStats;
	/** Vertical box panel listing defensive stats. */
	TWeakObjectPtr<UVerticalBox> VB_DefensiveStats;
	/** Vertical box panel listing health stats. */
	TWeakObjectPtr<UVerticalBox> VB_HealthStats;
	/** Vertical box panel listing adventure stats. */
	TWeakObjectPtr<UVerticalBox> VB_AdventureStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharDetail")
	TSubclassOf<class UB2DynText_Multiple> StatTextDisplayClass;

	UPROPERTY(Transient)
	TMap<int32, FStatTextInfo>	CreatedStatTextBlockInfos;

	TWeakObjectPtr<UTextBlock> TB_StatDetailTitle;
	TWeakObjectPtr<UTextBlock> TB_AttackStatTitle;
	TWeakObjectPtr<UTextBlock> TB_DefenseStatTitle;
	TWeakObjectPtr<UTextBlock> TB_HealthStatTitle;
	TWeakObjectPtr<UTextBlock> TB_AdventureStatTitle;
	
	TWeakObjectPtr<UB2Button>	BTN_Close;
	FDelegateCharStatDetailClose OnClose;

	EPCClass					CurPCClass;
	int32						OldSelectedSlotNum;
	
public:

	UB2LobbyUI_CharStatDetail(const FObjectInitializer& ObjectInitializer);
	
	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	
	virtual void DestroySelf() override;

	void UpdateStaticTexts();

	/** Main update function here, to update all displaying texts. */
	void UpdateStats();
	void UpdateStats(EPCClass PCClassToShow, bool bFindAccountInfo = false);
	void SetDelegateOnClose(const FDelegateCharStatDetailClose& Delegate);

private:
	/** To reduce duplicated code just a little bit. */
	void CreateSingleStatBasicPoint(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, EItemPrimaryPointType InPrimaryPointType, const FText& InStatLabelText, int32 InStatValue);
	void CreateSingleStatItemOption(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, EUnitedCombatOptions InUnitedOptionType, EPCClass InDisplayPCClas, bool bFindAccountInfo);
	void OnClickedStatSlot(int32 InSlotNum, FVector2D ClickPosition);
	void OnCloseDetailPopup();

public:
	void DestroyStatTexts();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonClicked();


	// For in game debug display of stats.. the same kind of stats displayed here
#if !UE_BUILD_SHIPPING
private:
	/** 0 or less shows nothing. 1 or above shows pre-defined set of player stat according to the number. */
	static int32 DebugDrawPlayerStatMode;
	static EPCClass DebugDrawPlayerStatClass; // It is for some network game mode, and will be overridden for local play mode
	static void DebugDrawPlayerPrimStatOnScreen(EPCClass InLocalMainCharClass, EPCClass InLocalSubCharClass, EItemPrimaryPointType InPrimPointType, float InStartX, float InStartY);
	static void DebugDrawPlayerSingleOptionOnScreen(EPCClass InLocalCharClass, EUnitedCombatOptions DisplayOption, float InStartX, float InStartY);
public:
	/** Draw on screen text showing local player combat stats. Most stats are for MainCharClass, SubCharClass is referred to display united prim stat value. */
	static void DebugDrawPlayerStatOnScreen(EPCClass InMainCharClassOverride = EPCClass::EPC_End, EPCClass InSubCharClass = EPCClass::EPC_End);
	/** Select which set of stat to display.. start from 1. 0 or less turns it off. */
	static void SetDebugDrawPlayerStatMode(int32 InSetNumber);
	static void SetDebugDrawPlayerStatClass(EPCClass InClass);
#endif
};

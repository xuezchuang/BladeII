// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "B2UIDocUICondition.generated.h"

/* This purpose is to hold miscellaneous UI-related data. */
UCLASS()
class BLADEII_API UB2UIDocUICondition : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocUICondition(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocUICondition();


	DECLARE_UIDOC_FIELD(int32, Test)

	/* The title text of the header UI. */
	DECLARE_UIDOC_FIELD(FText, HeaderTitle)

	DECLARE_UIDOC_FIELD(EUIScene, CurrUIScene)

	DECLARE_UIDOC_FIELD(bool, HideAllLobbyMainUIs)

	/** Ignore pre-combat Arin consulting if this is positive value, expected to be 0 if user did not ignored Arin consulting. */
	DECLARE_UIDOC_FIELD(int32, PreCombatArinConsultIgnoreCount)

	/** Whether the user checked skip story check box which can be seen once after clear the stage of scenario mode. */
	DECLARE_UIDOC_FIELD(bool, bUserSelectedSkipStory)

	DECLARE_UIDOC_FIELD(bool, IsCombineMenu)

	DECLARE_UIDOC_FIELD(bool, ForcedOpenEventAttendance)

public:
	FORCEINLINE bool ShouldIgnorePreCombatArinConsulting() const { return (PreCombatArinConsultIgnoreCount > 0); }
};

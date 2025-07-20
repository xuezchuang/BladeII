// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "Materials/MaterialInterface.h"
#include "B2UIDocScenarioDialog.generated.h"

/* Data indicating the dialog involved in the scenario. */
UCLASS()
class BLADEII_API UB2UIDocScenarioDialog : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocScenarioDialog(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocScenarioDialog();

	/* Whether the dialog is open, whether closed? */
	//DECLARE_UIDOC_FIELD(bool, DialogOpened)   //Do not use this. Please use "UB2UIManager::GetInstance()->OpenUI(UIFName::ScenarioDialog);" instead.
	/* the duration of Dialog ==>(Without counting on the UI, it is calculated from the Client) */
	//DECLARE_UIDOC_FIELD(int32, AutoHideTime)

	/* Speaker Information. This Material reference need to be reset on level unload */
	DECLARE_UIDOC_FIELD(UMaterialInterface*, SpeakerPortrait)
	DECLARE_UIDOC_FIELD(FText, SpeakerName)

	/* Dialog script */
	DECLARE_UIDOC_FIELD(FText, DialogText)

	/** Not for dialog timer, for some reference.. */
	DECLARE_UIDOC_FIELD(float, ThisDialogShowingTime)

	/* That have a button to move to the next? */
	DECLARE_UIDOC_FIELD(bool, ShowNextButton)

	DECLARE_UIDOC_FIELD(bool, bLastBattleUIHidden)

public:
	virtual void Init() override;

	void ReleaseResources();
};

// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "CommonStruct.h"
#include "../B2RichTextBlock.h"
#include "B2UICharacterNickNameEdit.generated.h"

/*
 * Editing per-character nickname at CharacterObserver scene.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICharacterNickNameEdit : public UB2UIWidgetBase
{
	GENERATED_BODY()

	EPCClass EditingPCClass;

protected:

	TWeakObjectPtr<UEditableText> ETB_NickName;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UB2RichTextBlock> RTB_Desc;
	TWeakObjectPtr<UTextBlock> TB_EditConfirmBtn;
	TWeakObjectPtr<UTextBlock> TB_CancelBtn;

	TWeakObjectPtr<UB2Button> BTN_EditConfirm;
	TWeakObjectPtr<UB2Button> BTN_Cancel;

	// To handle max length clamp.. 
	bool bClampedEditNickNamePending;
	FText PendingClampedEditNickName;

public:
	UB2UICharacterNickNameEdit(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BeginEditPopup(EPCClass InEditClass); // Just to start from CharacterObserve UI.

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	bool CheckForProhibitedNickName(const FText& InNickName);
	bool CheckForWrongNickName(const FText& InNickName);
	bool CheckForDuplicatedNickName(const FText& InNickName);
	
	UFUNCTION()
	void OnNickNameEditChanged(const FText& InText);

	UFUNCTION()
	void OnClickEditConfirmBtn();
	UFUNCTION()
	void OnClickCancelBtn();

public:
	void CloseMe();
};

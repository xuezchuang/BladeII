// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIControlChatPanelMessage.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlChatPanelMessage : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	UB2UIControlChatPanelMessage(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBtnSend();
	
public:
	void AddControlChatPanelMessageItem(EControlChatType Type, int32 Index, FString TextKey, UMaterialInterface* MI);

private:
	TWeakObjectPtr<UB2Button> Btn_Send;
	//TWeakObjectPtr<UB2Image> IMG_Icon;
	TWeakObjectPtr<UTextBlock> TB_Message;

	//TWeakObjectPtr<UB2Image> IMG_BG_Dark;
	//TWeakObjectPtr<UB2Image> IMG_BG_Bright;

private:
	EControlChatType ControlMessageType;
	int32 ControlMessageIndex;

	
};

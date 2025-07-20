// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIControlChatPanel.generated.h"



/**
 * 
 */

UCLASS()
class BLADEII_API UB2UIControlChatPanel : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	UB2UIControlChatPanel(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;
	
private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBtnMsgTab();

	UFUNCTION()
	void OnClickBtnEmotionTab();

	void ResetPanelContents();
	
public:
	void SetChatTab(EControlChatType TabType);

	void AddControlChatPanelMessageItem(EControlChatType Type, int32 Index, FString TextKey, UMaterialInterface* MI);

private:
	TWeakObjectPtr<UOverlay> O_BTN_Msg_DeAct;
	TWeakObjectPtr<UOverlay> O_BTN_Msg_Act;
	TWeakObjectPtr<UOverlay> O_Contents_Msg;

	TWeakObjectPtr<UOverlay> O_BTN_Emotion_DeAct;
	TWeakObjectPtr<UOverlay> O_BTN_Emotion_Act;
	TWeakObjectPtr<UOverlay> O_Contents_Emotion;

	TWeakObjectPtr<UVerticalBox> VB_ChatText;
	TWeakObjectPtr<UUniformGridPanel> GP_ChatEmoticon;

	TWeakObjectPtr<UB2Button> BTN_Msg_Tab;
	TWeakObjectPtr<UB2Button> BTN_Emotion_Tab;
	
	EControlChatType CurrentTab;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MsgItem")
	TSubclassOf<class UB2UIControlChatPanelMessage> MessageItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MsgItem")
	TSubclassOf<class UB2UIControlChatPanelMessage> EmoticonItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MsgItem")
	int32 GridPanelColumnCount;
};

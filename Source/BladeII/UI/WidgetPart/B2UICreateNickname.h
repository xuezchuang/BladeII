// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UICreateNickname.generated.h"

enum class ECreateState : uint8
{
	Create = 0,
	Change,
	Confirm,
	WrongNick,
	UseNick,
	SameNick,
	_Max,
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICreateNickname : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

public:
	
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
private:

	TWeakObjectPtr<UOverlay> O_Content;
	TWeakObjectPtr<UOverlay> O_ContentInput;
	TWeakObjectPtr<UOverlay> O_Btn_Create;
	TWeakObjectPtr<UOverlay> O_Btn_Two;
	TWeakObjectPtr<UOverlay> O_Btn_Confirm;
	TWeakObjectPtr<UOverlay> O_Btn_Changes;
	TWeakObjectPtr<UOverlay> O_Warning;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_FirstContent;
	TWeakObjectPtr<UTextBlock> TB_SecondContent;
	TWeakObjectPtr<UTextBlock> TB_InputContent;
	TWeakObjectPtr<UTextBlock> TB_CreateBtn;
	TWeakObjectPtr<UTextBlock> TB_OKBtn;
	TWeakObjectPtr<UTextBlock> TB_CancleBtn;
	TWeakObjectPtr<UTextBlock> TB_ConfirmBtn;
	TWeakObjectPtr<UTextBlock> TB_ChangeOkBtn;
	TWeakObjectPtr<UTextBlock> TB_ChangeNoBtn;

	TWeakObjectPtr<UB2Button> Btn_Create;
	TWeakObjectPtr<UB2Button> Btn_Ok;
	TWeakObjectPtr<UB2Button> Btn_Cancle;
	TWeakObjectPtr<UB2Button> Btn_Confirm;

	TWeakObjectPtr<UB2Button> Btn_Change_Ok;
	TWeakObjectPtr<UB2Button> Btn_Change_No;

	TWeakObjectPtr<UEditableText> ETB_Input;

	TWeakObjectPtr<UB2RichTextBlock> RTB_Warning;

	uint32 DeliveryCreateNickNameTicket;
	uint32 DeliveryChangeNickNameTicket;
	uint32 HandleErrorUseNickNameTicket;
	uint32 HandleErrorWorngNickNameTicket;

	FString m_fstrNickName;
	ECreateState CurrentState;
	int32 NicknameChangeItemID;

public:
	UFUNCTION()
	void OnClickBtn_Create();
	UFUNCTION()
	void OnClickBtn_Ok();
	UFUNCTION()
	void OnClickBtn_Cancle();
	UFUNCTION()
	void OnClickBtn_Confirm();
	UFUNCTION()
	void OnClickBtn_ChangeOk();
	UFUNCTION()
	void OnClickBtn_ChangeNo();

public:
	void SetStateMode(ECreateState eState);
	void SetInputText(const FText& fText);
	FText GetInputText();
	void ReQuestCreateNickName(FString fstr);
	void ReQuestChangeNickName(FString fstr);

	void SetModeChangeNickName(int32 itemID);

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	void SettingCreate();
	void SettingChange();
	void SettingConfirm();
	void SettingUse();
	void SettingWrong();
	void SettingSame();

	bool IsCheckWrong(const FText& fTxtNick);
	bool IsCheckUse(const FText& fTxtNick);
	bool IsCheckSame();

	void ResponCraeteNickName();
	void ResponChangeNickName(const FB2UpdateNicknamePtr& response);
	void ErrorUseNickName();
	void ErrorWrongNickName();

	bool bIsChange;
};

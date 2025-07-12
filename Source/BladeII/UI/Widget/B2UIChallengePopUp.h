// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2Button.h"

#include "B2UIChallengePopUp.generated.h"

DECLARE_DELEGATE_OneParam(ChallengePopUpOnClick, ELobbyModSceneElem);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChallengePopUp : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetPopupInfo(ELobbyModSceneElem InModEnum, bool bLock);
	void ClosePopUp();
private:
	void SettingInfo(const FString& Title, const FString& Explan, const FString& Confrim);
protected:
	UFUNCTION()
	void OnClickBTN_Confirm();
	UFUNCTION()
	void OnClickBTN_Close();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetPopUpInfo_Bp(ELobbyModSceneElem InModEnum);

private:

	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_Confirm;
	TWeakObjectPtr<UTextBlock> TB_Explanation;
	TWeakObjectPtr<UTextBlock> TB_IconReward;
	TWeakObjectPtr<UTextBlock> TB_IconName;

private:
	ELobbyModSceneElem MyMod;
	bool bOpenLock;

public:
	ChallengePopUpOnClick OnClickDelegate;
};

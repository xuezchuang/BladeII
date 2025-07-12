// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../Widget/B2Image.h"
#include "B2UISecession.generated.h"


UCLASS()
class BLADEII_API UB2UISecession : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	UFUNCTION()
	void OnClickB2Button_4();
	UFUNCTION()
	void OnClickBTN_Exit();
	UFUNCTION()
	void OnClickB2Button_0();
	UFUNCTION()
	void OnClickBTN_Secession();

private:
	TWeakObjectPtr<UB2Button> B2Button_4;
	TWeakObjectPtr<UB2Image> B2Image_3;
	TWeakObjectPtr<UB2Image> B2Image_20;
	TWeakObjectPtr<UImage> TitleBGImage;
	TWeakObjectPtr<UTextBlock> TXT_SettingTitle;
	TWeakObjectPtr<UB2Button> BTN_Exit;
	TWeakObjectPtr<UOverlay> Overlay_0;
	TWeakObjectPtr<UTextBlock> TXT_Body;
	TWeakObjectPtr<UB2Image> B2Image_2;
	TWeakObjectPtr<UTextBlock> TXT_NickName;
	TWeakObjectPtr<UTextBlock> TXT_03;
	TWeakObjectPtr<UB2Button> B2Button_0;
	TWeakObjectPtr<UTextBlock> TXT_OK1;
	TWeakObjectPtr<UOverlay> Overlay_1;
	TWeakObjectPtr<UTextBlock> TXT_Body2;
	TWeakObjectPtr<UB2Button> BTN_Secession;
	TWeakObjectPtr<UTextBlock> TXT_OK2;
};

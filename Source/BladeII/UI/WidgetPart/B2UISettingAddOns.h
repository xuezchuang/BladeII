// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UISettingAddOns.generated.h"

enum class EAddonsOption : uint8
{
	EAddons_AllPush = 0,
	EAddons_NoticePush,
	EAddons_GamePush,
	EAddons_NightPush,
	EAddons_Vibration,
	EAddons_ScreenN,
	EAttendanceTab_Max
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISettingAddOns : public UB2UIWidget
{
	GENERATED_BODY()

private:
	virtual void CacheAssets() override;	
	virtual void BindDelegates() override;
public:
	UB2UISettingAddOns(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	UFUNCTION()
	void OnClickBTN_AllPush();
	UFUNCTION()
	void OnClickBTN_NoticePush();
	UFUNCTION()
	void OnClickBTN_GamePush();
	UFUNCTION()
	void OnClickBTN_NightPush();
	/*
	UFUNCTION()
	void OnClickBTN_Vibration();
	UFUNCTION()
	void OnClickBTN_ScreenN();
	*/
private:

	TWeakObjectPtr<UButton> BTN_AllPush;
	TWeakObjectPtr<UImage> IMG_Unselect_AllPush;
	TWeakObjectPtr<UImage> IMG_Select_AllPush;
	TWeakObjectPtr<UTextBlock> TXT_AllPush_Off;
	TWeakObjectPtr<UTextBlock> TXT_AllPush_On;

	TWeakObjectPtr<UButton> BTN_NoticePush;
	TWeakObjectPtr<UImage> IMG_Unselect_NoticePush;
	TWeakObjectPtr<UImage> IMG_Select_NoticePush;
	TWeakObjectPtr<UTextBlock> TXT_NoticePush_Off;
	TWeakObjectPtr<UTextBlock> TXT_NoticePush_On;
	
	TWeakObjectPtr<UButton> BTN_GamePush;
	TWeakObjectPtr<UImage> IMG_Unselect_GamePush;
	TWeakObjectPtr<UImage> IMG_Select_GamePush;
	TWeakObjectPtr<UTextBlock> TXT_GamePush_Off;
	TWeakObjectPtr<UTextBlock> TXT_GamePush_On;

	TWeakObjectPtr<UButton> BTN_NightPush;
	TWeakObjectPtr<UImage> IMG_Unselect_NightPush;
	TWeakObjectPtr<UImage> IMG_Select_NightPush;
	TWeakObjectPtr<UTextBlock> TXT_NightPush_Off;
	TWeakObjectPtr<UTextBlock> TXT_NightPush_On;

	/*
	TWeakObjectPtr<UButton> BTN_Vibration;
	TWeakObjectPtr<UImage> IMG_Unselect_Vibration;
	TWeakObjectPtr<UImage> IMG_Select_Vibration;
	TWeakObjectPtr<UTextBlock> TXT_Vibration_Off;
	TWeakObjectPtr<UTextBlock> TXT_Vibration_On;

	TWeakObjectPtr<UButton> BTN_ScreenN;
	TWeakObjectPtr<UImage> IMG_Unselect_ScreenN;
	TWeakObjectPtr<UImage> IMG_Select_ScreenN;
	TWeakObjectPtr<UTextBlock> TXT_ScreenN_Off;
	TWeakObjectPtr<UTextBlock> TXT_ScreenN_On;
	*/
	struct SettingAddOnsBtn
	{		
		TWeakObjectPtr<UImage>		SelectedImage;
		TWeakObjectPtr<UImage>		UnselectedImage;
		TWeakObjectPtr<UTextBlock>	SelectedText;
		TWeakObjectPtr<UTextBlock>	UnselectedText;
	};

	TArray<SettingAddOnsBtn> m_SettingAddOnsBtn;
	SettingAddOnsBtn m_TempSettingAddOns;

	void SetData();
	void SelectBtn(EAddonsOption option);

	void SaveSettingData();
	void LoadSettingGameData();

};

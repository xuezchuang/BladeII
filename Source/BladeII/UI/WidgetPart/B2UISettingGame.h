// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "BladeIIScalabilityControl.h"
#include "B2UISettingGame.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISettingGame : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UB2UISettingGame(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	virtual void CacheAssets() override;	
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void SaveSettingData();
	void LoadSettingGameData();
#if !UE_BUILD_SHIPPING
	bool bLoadSettingGameDataCalledOnce;
#endif
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII_Setting")
		void OnInitBP();

private:
	UFUNCTION()
	void OnClick_BackToSuggested();

	UFUNCTION()
	void OnClick_Graphic_High();
	UFUNCTION()
	void OnClick_Graphic_Mid();
	UFUNCTION()
	void OnClick_Graphic_Low();
	void OnClick_Graphic_Common(EB2GraphicsLevel SelectedLevel);
	void SetGraphicsButtonSelectedState(EB2GraphicsLevel SelectedLevel);

	UFUNCTION()
	void OnClick_FrameLimit_High();
	UFUNCTION()
	void OnClick_FrameLimit_Mid();
	UFUNCTION()
	void OnClick_FrameLimt_Low();
	void OnClick_FrameLimt_Common(EB2FrameLimitLevel SelectedLevel);
	void SetFrameLimitButtonSelectedState(EB2FrameLimitLevel SelectedLevel);
	void SetFrameLimitButtonEnableState();

	UFUNCTION()
	void OnClick_ResAA_High();
	UFUNCTION()
	void OnClick_ResAA_Mid();
	UFUNCTION()
	void OnClick_ResAA_Low();
	void OnClick_ResAA_Common(EB2ResolutionLevel SelectedLevel);
	void SetResAAButtonSelectedState(EB2ResolutionLevel SelectedLevel);
	void SetResAAButtonEnableState();

	void RefreshAllButtonSelectedState();

public:
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	EB2GraphicsLevel OnGetGraphicType();

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	float OnBGMVolume_Set(bool bUpDown, float fCur_Value);

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	float OnFXSoundVolume_Set(bool bUpDown, float fCur_Value);

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	int32 OnValue_TextView(float fValue);

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void OnSetBGM(float fVolume);

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void OnSetFXSound(float fVolume);

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	float OnGetBGM_Volume();

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	float OnGetFXSound_Volume();
		
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void OnsetVibration();

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void OnsetScreenLock();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
	void OnsetVoice();
protected:
	
	TWeakObjectPtr<UB2Button> BTN_BackToSuggested;
	TWeakObjectPtr<UTextBlock> TB_BackToSuggested;

	TWeakObjectPtr<UB2Button> BTN_Graphics_High;
	TWeakObjectPtr<UB2Button> BTN_Graphics_Mid;
	TWeakObjectPtr<UB2Button> BTN_Graphics_Low;

	TWeakObjectPtr<UTextBlock> TB_Graphics_Label;

	TWeakObjectPtr<UImage> IMG_Graphics_High_Select;
	TWeakObjectPtr<UImage> IMG_Graphics_High_Unselect;
	TWeakObjectPtr<UTextBlock> TB_Graphics_High_Select;
	TWeakObjectPtr<UTextBlock> TB_Graphics_High_Unselect;

	TWeakObjectPtr<UImage> IMG_Graphics_Mid_Select;
	TWeakObjectPtr<UImage> IMG_Graphics_Mid_Unselect;
	TWeakObjectPtr<UTextBlock> TB_Graphics_Mid_Select;
	TWeakObjectPtr<UTextBlock> TB_Graphics_Mid_Unselect;

	TWeakObjectPtr<UImage> IMG_Graphics_Low_Select;
	TWeakObjectPtr<UImage> IMG_Graphics_Low_Unselect;
	TWeakObjectPtr<UTextBlock> TB_Graphics_Low_Select;
	TWeakObjectPtr<UTextBlock> TB_Graphics_Low_Unselect;



	TWeakObjectPtr<UB2Button> BTN_FrameLimit_High;
	TWeakObjectPtr<UB2Button> BTN_FrameLimit_Mid;
	TWeakObjectPtr<UB2Button> BTN_FrameLimit_Low;

	TWeakObjectPtr<UTextBlock> TB_FrameLimit_Label;

	TWeakObjectPtr<UImage> IMG_FrameLimit_Low_Select;
	TWeakObjectPtr<UImage> IMG_FrameLimit_Low_Unselect;
	TWeakObjectPtr<UTextBlock> TB_FrameLimit_Low_Select;
	TWeakObjectPtr<UTextBlock> TB_FrameLimit_Low_Unselect;

	TWeakObjectPtr<UImage> IMG_FrameLimit_Mid_Select;
	TWeakObjectPtr<UImage> IMG_FrameLimit_Mid_Unselect;
	TWeakObjectPtr<UTextBlock> TB_FrameLimit_Mid_Select;
	TWeakObjectPtr<UTextBlock> TB_FrameLimit_Mid_Unselect;

	TWeakObjectPtr<UImage> IMG_FrameLimit_High_Select;
	TWeakObjectPtr<UImage> IMG_FrameLimit_High_Unselect;
	TWeakObjectPtr<UTextBlock> TB_FrameLimit_High_Select;
	TWeakObjectPtr<UTextBlock> TB_FrameLimit_High_Unselect;


	TWeakObjectPtr<UB2Button> BTN_ResAA_High;
	TWeakObjectPtr<UB2Button> BTN_ResAA_Mid;
	TWeakObjectPtr<UB2Button> BTN_ResAA_Low;

	TWeakObjectPtr<UTextBlock> TB_ResAA_Label;

	TWeakObjectPtr<UImage> IMG_ResAA_Low_Select;
	TWeakObjectPtr<UImage> IMG_ResAA_Low_Unselect;
	TWeakObjectPtr<UTextBlock> TB_ResAA_Low_Select;
	TWeakObjectPtr<UTextBlock> TB_ResAA_Low_Unselect;

	TWeakObjectPtr<UImage> IMG_ResAA_Mid_Select;
	TWeakObjectPtr<UImage> IMG_ResAA_Mid_Unselect;
	TWeakObjectPtr<UTextBlock> TB_ResAA_Mid_Select;
	TWeakObjectPtr<UTextBlock> TB_ResAA_Mid_Unselect;

	TWeakObjectPtr<UImage> IMG_ResAA_High_Select;
	TWeakObjectPtr<UImage> IMG_ResAA_High_Unselect;
	TWeakObjectPtr<UTextBlock> TB_ResAA_High_Select;
	TWeakObjectPtr<UTextBlock> TB_ResAA_High_Unselect;



	TWeakObjectPtr<UTextBlock> TB_BGM_Label;
	TWeakObjectPtr<UTextBlock> TB_FXSound_Label;
	
	TWeakObjectPtr<UPanelWidget> P_ScreenLock_Vib_Set;

	TWeakObjectPtr<UTextBlock> TB_ScreenLock_Label;
	TWeakObjectPtr<UImage> IMG_ScreenLock_On;
	TWeakObjectPtr<UImage> IMG_ScreenLock_Off;
	TWeakObjectPtr<UTextBlock> TB_ScreenLock_On;
	TWeakObjectPtr<UTextBlock> TB_ScreenLock_Off;

	TWeakObjectPtr<UTextBlock> TB_Vib_Label;
	TWeakObjectPtr<UImage> IMG_Vib_On;
	TWeakObjectPtr<UImage> IMG_Vib_Off;
	TWeakObjectPtr<UTextBlock> TB_Vib_On;
	TWeakObjectPtr<UTextBlock> TB_Vib_Off;

	TWeakObjectPtr<class UB2UISettingGlobalSoundBtn> UIP_Setting_GlobalSoundBtn;


	// Once had a plan for voice-only on/off ?
	//TWeakObjectPtr<UImage>		IMG_Voice_On;
	//TWeakObjectPtr<UImage>		IMG_Voice_Off;
	//TWeakObjectPtr<UTextBlock>		TB_Voice_On;
	//TWeakObjectPtr<UTextBlock>		TB_Voice_Off;

	/** Button and text bundle set having on/off state. */
	struct FOnOffBtnBundle
	{
		TWeakObjectPtr<UImage> IMG_On;
		TWeakObjectPtr<UImage> IMG_Off;
		TWeakObjectPtr<UTextBlock> TB_On;
		TWeakObjectPtr<UTextBlock> TB_Off;

		void SetOnOff(bool bOn);
		void SetEnable(bool bEnable);
	};
	FOnOffBtnBundle Graphics_High_Bundle;
	FOnOffBtnBundle Graphics_Mid_Bundle;
	FOnOffBtnBundle Graphics_Low_Bundle;
	FOnOffBtnBundle FrameLimit_High_Bundle;
	FOnOffBtnBundle FrameLimit_Mid_Bundle;
	FOnOffBtnBundle FrameLimit_Low_Bundle;
	FOnOffBtnBundle ResAA_High_Bundle;
	FOnOffBtnBundle ResAA_Mid_Bundle;
	FOnOffBtnBundle ResAA_Low_Bundle;
	FOnOffBtnBundle ScreenLock_Bundle;
	FOnOffBtnBundle Vib_Bundle;
	

	int32 m_nGraphicLevel;
	int32 m_nFrameLimitLevel;

	int32 m_nSafeResolutionLevel; // "Safe" resolution level means that reserved state is not shown. The one that applied internally this time.
	EB2ResolutionLevel GetContextDesiredResolutionLevel(bool bIsForUI); // It either simply returns m_nSafeResolutionLevel or reserved one, according to argument and current state.

	float m_fBGM_Volume;
	float m_fFXSound_Volume;
	bool m_bVibration;
	bool m_bScreenLock;
};



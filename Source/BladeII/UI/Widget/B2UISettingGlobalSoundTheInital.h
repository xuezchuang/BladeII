// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UISettingGlobalSoundTheInital.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UISettingGlobalSoundTheInital : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnCloseComplete() override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

protected:
	UFUNCTION()
	void OnClickBTN_Korea();
	UFUNCTION()
	void OnClickBTN_English();

public:
	UFUNCTION(BlueprintCallable, Category = "BladeII SettingGlobalSoundTheInital")
	void OnCloseAnimEnd_BP();

protected:
	void CloseMySelf();
	void LocalSaveSoundLocType();
	void UpdatePopupState(const int32 iSoundLocType);
	void OpenConfirmNotifyPopup();

public:
	void TheInitialSoundLocType();

protected:
	TWeakObjectPtr<UB2Button>	BTN_Confirm;
	TWeakObjectPtr<UB2Button>	BTN_Korea;
	TWeakObjectPtr<UB2Button>	BTN_English;
	TWeakObjectPtr<UTextBlock>	TB_TitleGlobalSelect;
	TWeakObjectPtr<UTextBlock>	TB_ExplainGlobalSelect;
	TWeakObjectPtr<UTextBlock>	TB_VoiceChoiceText;

	TWeakObjectPtr<UTextBlock>	TB_UnSelectedKorea;
	TWeakObjectPtr<UTextBlock>	TB_SelectedKorea;
	TWeakObjectPtr<UTextBlock>	TB_UnSelectedEnglish;
	TWeakObjectPtr<UTextBlock>	TB_SelectedEnglish;

	TWeakObjectPtr<UWidgetSwitcher>	WS_Korea;
	TWeakObjectPtr<UWidgetSwitcher>	WS_English;

	TWeakObjectPtr<UWidgetAnimation> Anim_Close;
protected:
	int32	CachedSoundLocType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChangeNotiyEndTime;
};

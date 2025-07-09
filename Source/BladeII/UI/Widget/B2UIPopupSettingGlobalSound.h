// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIPopupSettingGlobalSound.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPopupSettingGlobalSound : public UB2UIWidget
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

protected:
	UFUNCTION()
	void OnClickBTN_Cancel();
	UFUNCTION()
	void OnClickBTN_Confirm();
	UFUNCTION()
	void OnClickBTN_Korea();
	UFUNCTION()
	void OnClickBTN_English();

protected:
	void CloseMySelf();
	void LocalSaveSoundLocType();
	void UpdatePopupState(const int32 iSoundLocType);
	void OpenConfirmNotifyPopup();
	
protected:
	TWeakObjectPtr<UB2Button>	BTN_Cancel;
	TWeakObjectPtr<UB2Button>	BTN_Confirm;
	TWeakObjectPtr<UB2Button>	BTN_Korea;
	TWeakObjectPtr<UB2Button>	BTN_English;
	TWeakObjectPtr<UTextBlock>	TB_TitleGlobalSelect;
	TWeakObjectPtr<UTextBlock>	TB_ExplainGlobalSelect;

	TWeakObjectPtr<UTextBlock>	TB_UnSelectedKorea;
	TWeakObjectPtr<UTextBlock>	TB_SelectedKorea;
	TWeakObjectPtr<UTextBlock>	TB_UnSelectedEnglish;
	TWeakObjectPtr<UTextBlock>	TB_SelectedEnglish;

	TWeakObjectPtr<UWidgetSwitcher>	WS_Korea;
	TWeakObjectPtr<UWidgetSwitcher>	WS_English;


protected:
	int32	CachedSoundLocType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChangeNotiyEndTime;
};

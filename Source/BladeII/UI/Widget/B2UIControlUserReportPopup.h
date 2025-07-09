// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIControlUserReportPopup.generated.h"

/**
 * 
 */

UCLASS()
class BLADEII_API UB2UIControlUserReportPopup : public UB2UIWidget
{

	GENERATED_BODY()
public:
	UB2UIControlUserReportPopup(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetControlUserReportPopup();
	void SlotSetting();
	void EnAlbeSettting();
	void UserReportComplete();

	UFUNCTION()
		void OnClickClose();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

protected:
	TWeakObjectPtr<UB2Button>		BTN_Close;

	TArray<TWeakObjectPtr<class UB2UIControlUserReportPopupSlot>>		RedTeam;
	TArray<TWeakObjectPtr<class UB2UIControlUserReportPopupSlot>>		BlueTeam;

	TWeakObjectPtr<UTextBlock>	TB_Title;

	uint32 UserReportTicket;
	TMap<int64, int64>									UseRepotUser;
};
UCLASS()
class BLADEII_API UB2UIControlUserReportPopupSlot : public UB2UIWidget
{

	GENERATED_BODY()
public:
	UB2UIControlUserReportPopupSlot(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<UMaterialInterface*>	CharterTexture;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

public:
	void SetUserReportSlot(EPCClass InClass, int64 InAccountID, FText NickName);
	void SetIsEnable(bool InEnable);
	int64 GetAccount() { return AccountID; }

	UFUNCTION()
		void OnClickSelect();


protected:
	TWeakObjectPtr<UImage>	IMG_Character;
	TWeakObjectPtr<UTextBlock>	TB_NickName;
	TWeakObjectPtr<UB2Button>	BTN_Select;

	int64 AccountID;
};

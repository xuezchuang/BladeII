// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "B2UIWealthPopup.generated.h"


UCLASS()
class BLADEII_API UB2UIWealthPopupSlotHB : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	void AddChildWidget(UWidget* Content);

private:
	TWeakObjectPtr<UHorizontalBox>	HorizontalBox;
};


USTRUCT(BlueprintType)
struct FWealthInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWEALTH_TYPE					WealthType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface>	WealthMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32							MaxCount;
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIWealthPopupSlotInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FWealthInfo>	ArrayWealthInfo;

public:
	const TArray<FWealthInfo>& GetArrayWealthInfo() { return ArrayWealthInfo; }
};

UCLASS()
class BLADEII_API UB2UIWealthPopup : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

private:
	UFUNCTION()
	void OnClickBTN_Close();

	UFUNCTION()
	void OnChangedHideAllLobbyMainUIs(class UB2UIDocBase* Sender, bool HideAllLobbyMainUIs, bool PrevHideAllLobbyMainUIs);
	
public:
	void InitWealthPopup(FVector2D v2WealthPopupSlotPostion);

private:
	void CreateWealthPopupSlot();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32	MAX_Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIWealthPopupSlotHB>	WealthSlotHBClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIWealthPopupSlot>		WealthPopupSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIWealthPopupSlotInfo>	WealthPopupSlotInfo;

private:
	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UVerticalBox>	VB_WealthSlotList;
	TWeakObjectPtr<UImage>			IMG_WealthSlotListBG;
};

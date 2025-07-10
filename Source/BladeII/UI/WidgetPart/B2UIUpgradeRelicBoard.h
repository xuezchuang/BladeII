// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIWidget.h"
#include "B2UIUpgradeRelicBoard.generated.h"

UCLASS()
class BLADEII_API UB2UIUpgradeRelicBoardBar : public UB2UIWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetProgressValue(float nPercent);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimation_BP(UB2UIUpgradeRelicBoard* parentWidget, bool bIsFail);
private:
	TWeakObjectPtr<UProgressBar> ProgressBar;
	TWeakObjectPtr<UProgressBar> FX_ProgressBar_01;
	TWeakObjectPtr<UProgressBar> FX_ProgressBar_02;
};

UCLASS()
class BLADEII_API UB2UIUpgradeRelicBoardSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetSlotText(int32 nSlotIndex);
	void SetSlotVisibleState(int32 nType);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimation_BP(UB2UIUpgradeRelicBoard* parentWidget);
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimation2_BP(UB2UIUpgradeRelicBoard* parentWidget);

public:
	UPROPERTY(BlueprintReadOnly, Category = BladeIIGame)
	int32 m_nSlotIndex;

private:
	TWeakObjectPtr<UTextBlock> TB_Locked;
	TWeakObjectPtr<UPanelWidget> P_Locked;
	TWeakObjectPtr<UTextBlock> TB_UnLocked;
	TWeakObjectPtr<UPanelWidget> P_UnLocked;
	TWeakObjectPtr<UTextBlock> TB_Selected;
	TWeakObjectPtr<UPanelWidget> P_Selected;

	TWeakObjectPtr<UB2Image> IMG_Locked;
	TWeakObjectPtr<UB2Image> IMG_UnLocked;
	TWeakObjectPtr<UB2Image> IMG_Selected;
};

UCLASS()
class BLADEII_API UB2UIUpgradeRelicBoard : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetBoardInfo(int32 nLevel = 0, int32 nRelicId = 0, int32 nRelicGrade = 0);

	void OnPlayAnimation_Levelup(int32 nPlayIndex);
	void OnPlayAnimation_Upgrade(bool bIsFail);
	void OnPlayFailAnimation_Levelup(int32 nPlayIndex);
	void OnPlayFailAnimation_Upgrade();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnEndSlotAnimation_BP(int32 nSlotIndex);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnEndBarAnimation_BP(bool bIsFail);

	void SetParentClass(class UB2LobbyUI_RelicManagement* parentClass);

protected:

public:
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_0;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_1;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_2;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_3;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_4;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_5;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_6;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_7;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar> ProgressBar_8;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_0;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_1;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_2;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_3;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_4;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_5;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_6;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_7;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_8;
	TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot> item_9;

private:
	TArray<TWeakObjectPtr<class UB2UIUpgradeRelicBoardSlot>> ItemArray;
	TArray<TWeakObjectPtr<class UB2UIUpgradeRelicBoardBar>> BarArray;

	int32 AnimationPlayIndex = 0;
	bool AnimationIsLevelUp = false;
	bool bIsUpgradeFail = false;

	int32 TempRelicID = 0;
	int32 TempRelicLevel = 0;
	int32 TempRelicGrade = 0;
	class UB2LobbyUI_RelicManagement* m_parentClass;
};

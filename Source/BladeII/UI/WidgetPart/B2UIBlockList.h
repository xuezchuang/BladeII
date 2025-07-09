// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBlockList.generated.h"


UCLASS()
class BLADEII_API UB2UIBlockList : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetBlockUserText();
	void RemoveBlockUser(int64 InAccountId);

protected:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickClose();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
		TSubclassOf<class UB2UIBlockUser> BlockUserSlotClass;

private:
	TWeakObjectPtr<UTextBlock> TB_BlockUserList;
	TWeakObjectPtr<UTextBlock> TB_BlockUserCount;
	TWeakObjectPtr<UTextBlock> TB_BlockUserCoutValue;
	TWeakObjectPtr<UTextBlock> TB_MaxBlockUserCount;
	TWeakObjectPtr<UTextBlock> TB_NobodyBlockUser;

	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2ScrollBox> SB_BlockScrollBox;
};

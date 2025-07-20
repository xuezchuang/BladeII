// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBlockUser.generated.h"


UCLASS()
class BLADEII_API UB2UIBlockUser : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void UpdateStaticText();
	void SetInfo(const FString& InUserNickName, int64 InUserAccountId);

	int64 GetAccountId() const { return UserAccountId; }
protected:
	UFUNCTION()
	void OnClickUnBlock();

private:
	TWeakObjectPtr<UB2Button> BTN_UnBlock;
	TWeakObjectPtr<UTextBlock> TB_UnBlock;
	TWeakObjectPtr<UTextBlock> TB_UserID;

	FString UserNickName;
	int64 UserAccountId;
};

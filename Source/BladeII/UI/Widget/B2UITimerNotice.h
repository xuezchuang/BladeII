// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UITimerNotice.generated.h"

DECLARE_DELEGATE(FTimerNoticeOnFinish)

UCLASS()
class BLADEII_API UB2UITimerNotice : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetContentText(FText InText);
	void SetNoticeTimer(float InTimer, const FTimerNoticeOnFinish& handler);

	void RefreshTimerUI();
public:
	TWeakObjectPtr<UTextBlock>	TB_ContentText;
	TWeakObjectPtr<UTextBlock>	TB_RemainTime;

	FTimerNoticeOnFinish OnFinishHandler;

private:
	float m_fRemainTime;
};

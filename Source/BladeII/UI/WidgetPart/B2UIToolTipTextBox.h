// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UI/B2UIWidget.h"
#include "B2UIToolTipTextBox.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIToolTipTextBox : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetContentText(FText txtContent);
	//void SetContentTextFontSize(uint16 FontSize);

	// ClockDirection 0 ~ 12 , 0 = 12
	void SetTargetInfo(UB2UIWidgetBase* TargetUB2UIWidget, UWidget* Target, float ClockDirection = 6.0f, float InLifeTime = 5.0f);

private:
	virtual void CacheAssets() override;
	
	void UpdateTooltipBox();

private:
	TWeakObjectPtr<UCanvasPanel> CP_BG;
	TWeakObjectPtr<UB2Image> IMG_BG;
	TWeakObjectPtr<UOverlay> OverRay_Text;
	TWeakObjectPtr<UB2RichTextBlock> TB_Content;	

	UB2UIWidgetBase* m_pTargetUB2UIWidget;
	UWidget* m_pTargetWidget;
	float m_fClockDirectionFromTarget;
	float m_fTimeToInit;
	float m_fLifeTime;
};

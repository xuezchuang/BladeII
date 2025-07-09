// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "B2UIBroadcastSystemNotice.generated.h"

/**
 * 
 */
struct FB2BroadcastMessage
{
	FString Message;
	int32 DisplayTime;
};

UCLASS()
class BLADEII_API UB2UIBroadcastSystemNotice : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	UB2UIBroadcastSystemNotice(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	
	UFUNCTION()
	void SetMessageInfo(const FString &InMessage, const int32 InDisplayTime);

	UFUNCTION()
	void StartBroadcast();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "BladeII")
	void StartBroadcast_BP();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnStartBroadcastWidgetInfo(float InVisibleWidgetWidth, float InMessageWidth, int32 InTextLength);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnFinishedBroadcast();

private:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float ScrollSpeed;

private:
	TArray<FB2BroadcastMessage> ChatStore;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UB2RichTextBlock> TB_Message; 
	TWeakObjectPtr<UPanelWidget> OV_Content;

	bool IsPlayBroadcast;
	bool IsEableScrolling;
	float DisplayTime;

	float WidgetWidth;
	float VisibleWidgetWidth;
	int32 MessageLength;
	FTimerHandle TimeToBroadcast;
};

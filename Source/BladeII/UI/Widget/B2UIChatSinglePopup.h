#pragma once

#include "B2UIWidget.h"
#include "B2Chatting.h"
#include "B2UIChatSinglePopup.generated.h"

/*
 * Single line pop-up to display most recent chatting message in wide range of places.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChatSinglePopup : public UB2UIWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float DisplayTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float DisplayCoolTime;

	/** If message over this length, clamp with sufix ... */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 MaxDisplayTextLen;

	float LastUpdatedTime;

	float CahcedDisplayCoolTime;

	TWeakObjectPtr<UB2RichTextBlock>	TB_Message;

	TWeakObjectPtr<UB2Button>			BTN_Message;

public:
	UB2UIChatSinglePopup(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
		
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void UpdateWithNewChatMessage(const FB2ChatMessage& InMessage); // Update to newly arrived message, and also reset the timer (if it was already open).

	static FText GetMessageHeader(const FB2ChatMessage& InMessage); // Get chatting message header by message type.

	void CloseMe();

protected:
	bool CheckDisplayCoolTime();

	UFUNCTION()
	void OnClickBTN_Message();
};
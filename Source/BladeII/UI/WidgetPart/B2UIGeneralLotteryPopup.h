#pragma once

#include "B2UIWidget.h"
#include "B2UIGeneralLotteryPopup.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGeneralLotteryPopup : public UB2UIWidget
{
	GENERATED_BODY()

public:	
	void SetMailBox(class UB2UIMail* InMailBox) { MailBox = InMailBox; }

	void SetTitleText(const FText& Title);

	void Close() { CloseMe(); }
	virtual void Init() override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void CloseMe();
	UFUNCTION()
		void SelectedGladiator();
	UFUNCTION()
		void SelectedAssassin();
	UFUNCTION()
		void SelectedWizard();
	UFUNCTION()
		void SelectedFighter();
	UFUNCTION()
		void SelectConfirm();

	void SelectedCharacter(EPCClass SelectedCharacter);
	void UpdateCharSlot(EPCClass SelectCharacter);

protected:
	TWeakObjectPtr<UButton> BTN_Gladiator;
	TWeakObjectPtr<UButton> BTN_Assassin;
	TWeakObjectPtr<UButton> BTN_Wizard;
	TWeakObjectPtr<UButton> BTN_Fighter;
	TWeakObjectPtr<UButton> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_Confirm;

	TWeakObjectPtr<UTextBlock> TB_Title;

	TArray<TWeakObjectPtr<UOverlay>> O_OnCharacter;
	TArray<TWeakObjectPtr<UOverlay>> O_OffCharacter;

	class UB2UIMail* MailBox;

private:
	EPCClass		SelectPCType;
};

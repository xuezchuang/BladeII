#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildCreate.generated.h"

DECLARE_DELEGATE_OneParam(FGuildMarkSlotIndex, int32)

UENUM(BlueprintType)
enum class EUIGuildMarkTap : uint8
{
	PATTERNIMAGE		UMETA(DisplayName = "PatternImage"),	
	BGIMAGE				UMETA(DisplayName = "BGImage"),	
	MARKEND				UMETA(DisplayName = "MarkEnd"),
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildCreateSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	void SetMeterial(UMaterialInstance* MaterialImage);
	void SetLinearColor(FLinearColor LinearColor);
	void SetSlotIndex(int32 Index);
	void SetDelegate(FGuildMarkSlotIndex Delegate);
	void SetSelect(bool bInSelect);

	FORCEINLINE int32 GetSlotIndex() { return SlotIndex; }

	UFUNCTION()
		void OnClickSlot();

	
protected:
	TWeakObjectPtr<UB2Image>		IMG_Guild_Slot;
	TWeakObjectPtr<UB2Image>		IMG_Choice_Slot;
	TWeakObjectPtr<UB2Button>		BTN_Slot;

private:
	int32							SlotIndex;
	bool							bSelect;
	FGuildMarkSlotIndex				GuildMarkSlotDelegate;
};



UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildCreate : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void OnOpen(bool RightNow = false) override;
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

private:
	void SetGuildSlot(EUIGuildMarkTap GuildMarkType);
	void SetMaterialGuildMarkSlot(int32 SlotNumber);
	void SetColorGuildMarkSlot(int32 SlotNumber);

	void TapUpdate(EUIGuildMarkTap Intype);

public:
	UFUNCTION()
		void OnClickPattern();
	UFUNCTION()
		void OnClickPatternColor();
	UFUNCTION()
		void OnClickBGImage();
	UFUNCTION()
		void OnClickBGImageColor();
	UFUNCTION()
		void OnClickCreateGuild();
	UFUNCTION()
		void OnClickRandom();
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnClickEdit();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void ChangeCheckBoxState(UCheckBox* checkbox, bool bCheck);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void BP_ChangeTap();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIGuildCreateSlot>			PatternSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIGuildCreateSlot>			ColorSlot;

	void ImageUpdate(int32 index);
	void ColorUpdate(int32 index);

	void ButtonEnableCheck();

protected:
	TWeakObjectPtr<class UB2UIGuildMark>		UIP_LargeGuildMark;
	TWeakObjectPtr<class UB2UIGuildCreateSlot>	UIP_GuildCreateSlot;
	TWeakObjectPtr<UB2Button>					BTN_Random;
	TWeakObjectPtr<UEditableTextBox>			ETB_GuildName;
	TWeakObjectPtr<UCheckBox>					CB_Public;
	TWeakObjectPtr<UCheckBox>					CB_Closed;

	TWeakObjectPtr<UB2Button>					BTN_Pattern;
	TWeakObjectPtr<UB2Button>					BTN_BGImage;
	TWeakObjectPtr<UUniformGridPanel>			UGP_MarkSlotList;
	TWeakObjectPtr<UUniformGridPanel>			UGP_ColorSlotList;
	TWeakObjectPtr<UImage>						IMG_ChoiceSlot;
	TArray<TWeakObjectPtr<UOverlay>>			TapSlotArr;
	TArray<TWeakObjectPtr<UOverlay>>			UnTapSlotArr;

	TWeakObjectPtr<UB2Button>					BTN_CreateGuild;
	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UB2Button>		BTN_Edit;

	TWeakObjectPtr<UB2RichTextBlock>			RTB_Open;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_Private;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_Flag1_1;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_Flag1_2;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_Flag2_1;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_Flag2_2;
	TWeakObjectPtr<UTextBlock>					TB_Flag3;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_Random;
	TWeakObjectPtr<UTextBlock>					TB_GuildCreate;
	TWeakObjectPtr<UTextBlock>					TB_GuildEdit;
	TWeakObjectPtr<UTextBlock>					TB_GuildEdit2;

	TWeakObjectPtr<UTextBlock>					TB_Cost;
	TWeakObjectPtr<UTextBlock>					TB_UpdateCost;



private:
	const int32 SlotWidthNumber = 4;
	const int32 SlowColorWhidthNumber = 3;
	int32	PatternImageIndex;
	int32	PatternColorIndex;
	int32	BGImageIndex;
	int32	BGColorIndex;
	EUIGuildMarkTap TapType;
};
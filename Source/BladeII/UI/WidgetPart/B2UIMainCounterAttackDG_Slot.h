#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIMainCounterAttackDG_Slot.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMainCounterAttackDG_Slot : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void SetNowDay(int32);
	void SetDayType(int32 type, FString DungeonName);
	const FString DayDivision(EUICounterDayType Day);
	FText GetTimeText(int32 beginHour, int32 beginMin, int32 endHour, int32 endMin);

	UFUNCTION()
	void OnClickSelect();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelect")
	void DivisionLockEvent(int32 index);

	FORCEINLINE bool IsOpenDungeon() const { return IsOpened; }

protected:
	TWeakObjectPtr<UTextBlock>					TB_OpenDungeonTitle;
	TWeakObjectPtr<UTextBlock>					TB_CloseDungeonTitle;
	TWeakObjectPtr<UTextBlock>					TB_OpenRewardItem;
	TWeakObjectPtr<UTextBlock>					TB_CloseRewardItem;
	TWeakObjectPtr<UTextBlock>					TB_PossibilityTime;
	TWeakObjectPtr<UTextBlock>					TB_ClosePossibilityTime;

	TWeakObjectPtr<class UB2RichTextBlock>		TB_PossibilityCount;
	TWeakObjectPtr<UButton>						BTN_Select;

	TWeakObjectPtr<UImage>						IMG_BG;
	TWeakObjectPtr<UImage>						IMG_BG2;

	TWeakObjectPtr<UImage>						IMG_Item;
	TWeakObjectPtr<UImage>						IMG_Item2;

	TWeakObjectPtr<UPanelWidget> HB_CloseItemInfo;
	TWeakObjectPtr<UPanelWidget> HB_CloseItemInfo_Hard;
	TWeakObjectPtr<UPanelWidget> HB_OpenItemInfo;
	TWeakObjectPtr<UPanelWidget> HB_OpenItemInfo_Hard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInstance*>					BGTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		bool IsHardCoding;				// 저주 받은 성채 승급석 표시...... 미안합니다.

private:
	EPCClass						AllowedCharClass;
	int32							OpenType;
	bool							IsOpened;
};
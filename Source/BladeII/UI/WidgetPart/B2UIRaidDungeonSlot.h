#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIRaidDungeonSlot.generated.h"

DECLARE_DELEGATE_OneParam(FUB2UIRaidDungeonSlot_OnClickBtnSelect, int32)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRaidDungeonSlot : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
	void OnClickSelect();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelect")
	void SetEnableSlot_BP(bool IsEnable);

public:
	void SetRaidId(int32 RaidId);
	void SetRewardItemAccesory();
	void SetRewardItem(int32 RewardId);
	void SetRaidName(FText InDungeonName);
	void SetEnableSlot(bool IsEnable, FText Contenttext);
	
private:
	void SetBGImage(int32 InDungeonIdx);	
	UMaterialInterface* GetIconMaterial(EEquipCategoryType EquipCategory);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInstance*>					BGTextures;

public:
	FUB2UIRaidDungeonSlot_OnClickBtnSelect		FOnClickBtnSelect;

private:
	int32	RaidMapId;
	int32	RewardItemIdx;

	bool bIsEnableButton;
protected:
	TWeakObjectPtr<UTextBlock>					TB_DungeonTitle;
	TWeakObjectPtr<UTextBlock>					TB_DungeonTitle2;
	TWeakObjectPtr<UTextBlock>					TB_RewardItem;
	TWeakObjectPtr<UTextBlock>					TB_RewardItem2;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_ClosePossibilityTime;
	TWeakObjectPtr<UButton>						BTN_Select;
	TWeakObjectPtr<UImage>						IMG_BG;
	TWeakObjectPtr<UImage>						IMG_BG2;
	TWeakObjectPtr<UImage>						IMG_Item;
	TWeakObjectPtr<UImage>						IMG_Item2;
	TWeakObjectPtr<UHorizontalBox>				H_RewardItem2;
};
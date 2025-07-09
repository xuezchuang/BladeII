// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIAdventureModSlot.generated.h"

UENUM(BlueprintType)
enum class EAdventureModeType : uint8
{
	Tutorial		UMETA(DisplayName = "Tutorial"),
	Scenario		UMETA(DisplayName = "Scenario"),
	Adventure		UMETA(DisplayName = "Adventure"),
	Max,
};

USTRUCT(BlueprintType)
struct FAdventureModeSetting
{
	GENERATED_USTRUCT_BODY()
	
	FAdventureModeSetting()
	: ModeType(EAdventureModeType::Max)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	EAdventureModeType	ModeType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FText				ModeTitle_GeneralTextKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FText				ModeDesc_GeneralTextKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	UMaterialInterface* MainImageMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	UMaterialInterface* DisableMainImageMaterial;
};

DECLARE_DELEGATE_OneParam(UB2UIAdventureModSlot_OnClickBTNSelect, EAdventureModeType)
DECLARE_DELEGATE_OneParam(UB2UIAdventureModSlot_OnClickBTNDifficulty, EAdventureModeType)
DECLARE_DELEGATE_OneParam(UB2UIAdventureModSlot_OnClickBTNLock, EAdventureModeType)

UCLASS()
class BLADEII_API UB2UIAdventureModSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	UFUNCTION()
	void OnClickBTN_Select();
	UFUNCTION()
	void OnClickBTN_Sub();
	UFUNCTION()
	void OnClickBTN_Lock();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void SetAdventureModeType_BP(EAdventureModeType InAdventureModeType);

public:
	void SetAdventureModeInfo(FAdventureModeSetting InAdventureModeInfo);
	void SetNoticeText(EAdventureModeType InAdventureModeType, int32 ChapterNum, int32 SubChapterNum);
	void SetLockSlot(bool IsLock);
	void SetIsCompleteScenarioMode(bool IsComplete);
	void SetVisiblityAdventureDifficultyButton(bool IsVisbility);
	void SetLastStageDifficulty(EStageDifficulty LastStageDifficulty);

private:
	void SetAdventureModeType(EAdventureModeType InAdventureModeType);

private:
	TWeakObjectPtr<UTextBlock>	TB_Title;
	TWeakObjectPtr<UImage>		IMG_Main;
	TWeakObjectPtr<UImage>		IMG_DisableMain;
	TWeakObjectPtr<UTextBlock>	TB_Desc;
	TWeakObjectPtr<UB2Button>	BTN_Select;
	TWeakObjectPtr<UB2Button>	BTN_Lock;
	TWeakObjectPtr<UTextBlock>	TB_Scenario;
	TWeakObjectPtr<UTextBlock>	TB_Scenario2;

	TWeakObjectPtr<UTextBlock>	TB_PlayScenario;
	TWeakObjectPtr<UTextBlock>	TB_PlayAdventure;

	TWeakObjectPtr<UTextBlock>	TB_Adventure;
	TWeakObjectPtr<UTextBlock>	TB_Adventure2;
	TWeakObjectPtr<UOverlay>	O_NoticeScenario;
	TWeakObjectPtr<UOverlay>	O_NoticeScenarioComplete;
	TWeakObjectPtr<UOverlay>	O_NoticeAdventure;
	TWeakObjectPtr<UOverlay>	O_NoticeAdventureDifficulty;
	TWeakObjectPtr<UOverlay>	O_Lock;
	TWeakObjectPtr<UB2Button>	BTN_StageDifficulty;
	TWeakObjectPtr<UB2Image>	IMG_StageDifficulty;
	TWeakObjectPtr<UTextBlock>	TB_StageDifficulty;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInterface*>	AdventureDifficultyIcon;

public:
	UB2UIAdventureModSlot_OnClickBTNSelect		FOnClickBTNSelect;
	UB2UIAdventureModSlot_OnClickBTNDifficulty	FOnClickBTNSub;
	UB2UIAdventureModSlot_OnClickBTNLock		FOnClickBTNLock;

private:
	EAdventureModeType	CurAdventureModeType;
};

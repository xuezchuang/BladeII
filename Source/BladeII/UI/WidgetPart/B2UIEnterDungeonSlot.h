// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIEnterDungeonSlot.generated.h"

DECLARE_DELEGATE_OneParam(FClickDungeonSlot, ELobbyModSceneElem)

USTRUCT(BlueprintType)
struct FSlotRewardImageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FString RewardText;
	UPROPERTY(EditAnywhere)
		UMaterialInstance* RewardImage;
};

UCLASS()
class BLADEII_API UB2UIEnterDungeonSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetCountInfo(int32 LastCount, int32 MaxCount);
	void SetButtonEnable(bool bEnable);
	void SetLockTimeInfo(FText OpenTimeText);
	void SetReward(int32 ImageNumber);

	UFUNCTION()
	void OnClickBTN_Select();

	//View - b2network::B2ContentsModeState
	void SetModeState(int32 ModeState);
	bool IsModeStateCurrentOpen();
	void SetBlockPage_Dimension();

	void SetModeText(int32 ContentMode);

	// return b2network::B2ContentsMode
	int32 GetCurrentModeType(ELobbyModSceneElem LobbyModScene);

private:
	TWeakObjectPtr<UB2Button>					BTN_Select;
	TWeakObjectPtr<UTextBlock>					TB_DungeonName;
	TWeakObjectPtr<UTextBlock>					TB_DungeonCount;
	TWeakObjectPtr<UTextBlock>					TB_DungeonDesc;
	TWeakObjectPtr<UTextBlock>					TB_Reward;

	TWeakObjectPtr<UOverlay>					OV_NoEnter;
	TWeakObjectPtr<UB2RichTextBlock>			TB_NoEnterText;

	TWeakObjectPtr<UOverlay>					OV_RaidWait;
	TWeakObjectPtr<UB2RichTextBlock>			TB_ClosePossibilityTime;
	TWeakObjectPtr<UB2RichTextBlock>			STB_ClosePossible;
	
	TWeakObjectPtr<UOverlay>					OV_ControlMode;
	TWeakObjectPtr<UB2RichTextBlock>			TB_ControlMode;

	TWeakObjectPtr<UImage>						IMG_Reward;

public:
	FClickDungeonSlot							FOnClickDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII EnterDungeonSlot")
	ELobbyModSceneElem							CurrentGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII EnterDungeonSlot")
	TArray<FSlotRewardImageInfo>					RewardArray;

	//CurrentModeState - b2network::B2ContentsModeState
	int32 CurrentModeState;
};

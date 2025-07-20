#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UICounterAttackSelectCharacterSlot.generated.h"

DECLARE_DELEGATE_FourParams(FClickCounterSlot, int32, int32, int32, int32);
DECLARE_DELEGATE_OneParam(FClickDungeonDifficulty, int32);

USTRUCT(BlueprintType)
struct FRaidDifficultyBGImage
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raid")
	int32								RaidId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raid")
	TArray<UMaterialInterface*>			BGImages;
};

UENUM(BlueprintType)
enum class EDungeonLevelState : uint8
{
	EDLS_ON,
	EDLS_OFF,
	EDLS_LOCK,
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIDungeonDifficulty : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	template<typename FunctorType>
	inline void SetDifficultyDelegate(FunctorType&& InFunctor)
	{
		DifficultyDelegate.Unbind();
		DifficultyDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}
	void SetDifficultyText(int32 DifficultyText);
	void SetCurrentDifficulty(int32 InCurDifficulty);

	FORCEINLINE void SetMaxDifficulty(int32 InMaxDifficulty) { MaxDifficulty = InMaxDifficulty; }

	UFUNCTION()
		void OnClickLeft();

	UFUNCTION()
		void OnClickRight();

private:
	TWeakObjectPtr<UB2Button>			BTN_DifficultyLeft1;
	TWeakObjectPtr<UB2Button>			BTN_DifficultyLeft2;
	TWeakObjectPtr<UB2Button>			BTN_DifficultyRight1;
	TWeakObjectPtr<UB2Button>			BTN_DifficultyRight2;

	TWeakObjectPtr<UTextBlock>			TB_DifficultyDesc;
	TWeakObjectPtr<UTextBlock>			TB_Difficulty;
	TWeakObjectPtr<UTextBlock>			TB_Max;

private:
	int32 MaxDifficulty;
	int32 CurrentDifficulty;
	FClickDungeonDifficulty				DifficultyDelegate;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICounterSlot : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void SetImage();
	void SetSlotLine(class UB2UICounterSlotLine* SlotLine);
	void SetSlotState(EDungeonLevelState LevelState);
	void SetSlotVisibilty(UOverlay* SlotPointer);
	void SetDifficulty(int32 InDifficulty);
	void SetRecommendStat(int32 Combat, int32 InRecommendAttack, int32 InRecommendDefense);
	void SetServerStageID(int32 InStageID);


	void SetBestRecordVisibility(bool InVisible);

	FORCEINLINE int32 GetServerStageID() { return ServerStageID; }


public:
	UFUNCTION()
		void OnClickSelect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMGState")
		UMaterialInterface* OnImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMGState")
		UMaterialInterface* OffImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMGState")
		UMaterialInterface* LockImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMGState")
		FSlateColor		OnColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMGState")
		FSlateColor		OffColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMGState")
		FSlateColor		LockColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMGState")
		FName			LineName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMGState")
		int32	DifficultyLv;

public:
	FClickCounterSlot					FOnClickDelegate;

protected:
	TWeakObjectPtr<UOverlay> O_Lock;
	TWeakObjectPtr<UOverlay> O_Off;
	TWeakObjectPtr<UOverlay> O_On;

	TWeakObjectPtr<UImage> IMG_Lock;
	TWeakObjectPtr<UImage> IMG_Off;
	TWeakObjectPtr<UImage> IMG_On;
	TWeakObjectPtr<UTextBlock> TB_Lv;
	TWeakObjectPtr<UTextBlock> STB_RecommendStat;
	TWeakObjectPtr<UTextBlock> TB_RecommendStat;
	TWeakObjectPtr<UB2Button>	BTN_Select;

	TWeakObjectPtr<class UB2UICounterSlotLine> UIP_SlotLine;

	TWeakObjectPtr<UOverlay>	O_BestRecord;
	TWeakObjectPtr<UTextBlock>	TB_BestRecord;

private:
	int32	CurrentRecommandPower = 0;
	int32	RecommandAttack = 0;
	int32	RecommandDefense = 0;
	int32	ServerStageID = 0;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICounterSlotLine : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	void SetLine(bool IsLine);

protected:
	TWeakObjectPtr<UOverlay> O_Off;
	TWeakObjectPtr<UOverlay> O_On;

};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICounterAttackSelectCharacterSlot : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	FORCEINLINE int32 GetTotalAttackPoint() { return TotalAttack; }
	FORCEINLINE int32 GetTotalDefencePoint() { return TotalDefence; }

	UFUNCTION()
	void OnClickButton();

	void SetRecommendStat(int32 Combat);
	//void SetVisibliltySelectOverlay(bool IsVisible);
	void SettingDifficulty(EUIBattleDifficulty InDifficulty);
	void SettingDifficulty(int32 InDifficulty);

	void SetEnableSlot_Raid(bool bEnalble, bool bVisibleRecomStat);
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelect")
	void EventDifficulty(int32 index);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelect")
	void EventSetVisibleSelected(bool bIsSelected);

public:
	FClickCounterSlot					FOnClickDelegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raid")
	TArray<FRaidDifficultyBGImage>		RaidBGImageDatas;

protected:
	TWeakObjectPtr<UOverlay>			O_SelectOverlay;
	TWeakObjectPtr<UImage>				IMG_Select;
	TWeakObjectPtr<UImage>				IMG_Level;
	TWeakObjectPtr<UTextBlock>			TB_TotalAttackPoint;
	TWeakObjectPtr<UTextBlock>			TB_TotalDefencePoint;
	//TWeakObjectPtr<UTextBlock>			TB_CombatPower;
	TWeakObjectPtr<UButton>				BTN_Select;
	TArray<TWeakObjectPtr<UImage>>		IMG_DifficultyWidgets;
	
private:
	int32								Difficulty;
	int32								TotalAttack;
	int32								TotalDefence;
	uint32								CurrentRecommandPower = 0;
};
#pragma once

#include "B2UIRepeatableElements.h"
#include "B2UIGradeRankAndStar.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGradeRankAndStar : public UB2UIRepeatableElements
{
	GENERATED_UCLASS_BODY()

private:
	// for rankup animation
	struct FRankAnimInfo
	{
		FName WidgetName;
		FName InstanceName;
		int32 StarCountAfterAnimation;
		float AnimSpeed;
		TWeakObjectPtr<UWidgetAnimation> RankUpAnimation;
	public:
		FRankAnimInfo(const FName& _InstanceName, const FName& _WidgetName, const int32 StarCount, TWeakObjectPtr<UWidgetAnimation>& _RankUpAnimation, float InAnimSpeed);
		~FRankAnimInfo();
	};

public:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetGradeRank(int32 Rank, int32 StarNum);

	void SetStarNum(int32 Rank, int32 StarNum);
	void SetUseAnimation();
	void StartStarAnimation(int32 BeforeStarNum, int32 StarNum, bool GradeUp);
	void StartRankUpAnimation(int32 BeforeRank, int32 Rank, int32 StarNum);

protected:
	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> RankMATs;

private:
	virtual void CacheAssets() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void PlayStarAnimation(const FName& AnimationName);
	void LoadAnimation(const FName& AnimName, int32 AfterStarCount, float InPlaySpeed);
	void AddStarAnimation(FRankAnimInfo* InAnim);
	void NoAddStarAnimation(FRankAnimInfo* InAnim);

private:
	TWeakObjectPtr<UImage> IMG_GradeRank;
	TWeakObjectPtr<UTextBlock> TB_Rank;

	TWeakObjectPtr<UOverlay> OV_OverGrade;
	TWeakObjectPtr<UTextBlock> TB_NumStarOverGrade;

	TMap<FName, FRankAnimInfo> StarAnims;

	EStateAfterGradeUp NextAnimation;
	bool IsGradeUp;
	bool IsRankUpAnimation;
	bool IsAfterAdding;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII", meta = (UIMin = "1.0"))
		float UpGradeAnimationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII", meta = (UIMin = "1.0"))
		float DownGradeAnimationSpeed;

};

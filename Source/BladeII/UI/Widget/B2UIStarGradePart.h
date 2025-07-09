// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIStarGradePart.generated.h"

USTRUCT()
struct FStarGradeOver6
{
	GENERATED_USTRUCT_BODY()

	FStarGradeOver6()
	{
		StarGrade = 0;
	}

	int32 StarGrade;

	TWeakObjectPtr<UImage> IMG_NormalStar;
	TWeakObjectPtr<UImage> IMG_SurpassStar;
};

UCLASS()
class BLADEII_API UB2UIStarGradePart : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetEnableSurpass(bool InEnableSurpass) { EnbleSurpass = InEnableSurpass;}

	void SetVisibleOver6(const int32 InItemGrade, bool InIsSurpass);
	void SetVisibleUnder7(const int32 InItemGrade, bool InIsSurpass);
	void SetVisibleUnder7Surpass(const int32 InIndex, bool InIsSurpass);

	void UpdateStar(const FB2Item& InItem);
	void UpdateStar(const int32 InItemGrade, bool InIsSurpass);

private:
	TWeakObjectPtr<UPanelWidget> P_Stars;
	TWeakObjectPtr<UPanelWidget> P_StarGradeOver6;

	TArray<TWeakObjectPtr<UImage>> IMG_StarUnder7;
	TArray<FStarGradeOver6> StarOver6;

	bool EnbleSurpass;
};

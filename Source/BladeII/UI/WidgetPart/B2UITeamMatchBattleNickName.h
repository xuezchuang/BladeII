// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "B2UIWidget.h"
#include "B2UITeamMatchBattleNickName.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamMatchBattleNickName : public UB2UIWidget
{
	GENERATED_BODY()

	UB2UITeamMatchBattleNickName(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;

	void SetPlayerInfo(const FString& NickName, int32 Lv, int32 Grade, int32 Star);
	void SetMyPlayerBadge(bool _isVisible);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UTextBlock> TB_NickName;

	TWeakObjectPtr<class UB2UIGradeRankAndStar>	UIP_PVPGrade;
	TWeakObjectPtr<UB2Image> IMG_MyBadge;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBrevetRankUpSuccess.generated.h"

UCLASS()
class BLADEII_API UB2UIBrevetRankUpSuccess : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void InitBrevetRankUpSuccess(class UB2LobbyUI_BrevetMain* pOwnerUI, int32 _iPrevRank, int32 _iCurrentRank);

protected:
	virtual void CacheAssets() override;
	void UpdateStaticText();

	void SetRankWidgets(int32 iRank);
	UMaterialInterface* GetRankIconMatarial(int32 iRank);
	FText GetRankName(int32 iRank);

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_Anim_RankUp();

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
		void OnFinishAnimationEvent_Anim_RankUp();

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
		void ChangeRankPrevAtCurrent();

protected:
	class UB2LobbyUI_BrevetMain* m_pOwnerUI;

	TWeakObjectPtr<UImage> IMG_RankIcon;
	TWeakObjectPtr<UTextBlock> TB_RankName;
	TWeakObjectPtr<UTextBlock> TB_Title;

	int32 iCurrentRank;
};
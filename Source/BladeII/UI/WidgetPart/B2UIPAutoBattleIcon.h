// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIPAutoBattleIcon.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPAutoBattleIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION()
		void OnChangedIconAutoBattleType(class UB2UIDocBase* Sender, int32 bCurrentOn, int32 bPrevOn);
	
	void SetIconAutoBattleType(int32 InIconAutoBattleType);
	void SetIsStageMode(bool Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		FButtonStyle AutoOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		FButtonStyle AttackAutoOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		FButtonStyle SkillAutoOn;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	
private:
	UFUNCTION()
		void OnClickAutoBattle();

	bool IsStageMode = false;
	TWeakObjectPtr<UB2Button> BTN_AutoBattle;
	int32 IconAutoBattleType;
	FDelegateHandle IconAutoBattleHandler;
};

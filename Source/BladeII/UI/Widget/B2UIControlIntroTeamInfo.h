// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIControlIntroTeamInfo.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlIntroTeamInfo : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// B2UIControlIntroTeamInfo
	UFUNCTION(BlueprintImplementableEvent)
	void AddTeamUserInfo(ETeamType teamType, const FText& UserName, UMaterialInterface* CharIcon, int32 Grade, int32 Star);

private:
	void CreateUserInfos();
	void CreateUserInfo(class ICharacterDataStore* pCharData, ETeamType TeamType, int32 BattleGrade, int32 BattleStarCount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
		TSubclassOf<class UUserWidget> TeamInfoItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TMap<EPCClass, TSoftObjectPtr<UMaterialInterface>> FaceMaterials;
	
	
};

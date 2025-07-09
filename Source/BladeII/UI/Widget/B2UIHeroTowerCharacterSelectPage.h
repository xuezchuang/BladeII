// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIHeroTowerCharacterSelectPage.generated.h"

UCLASS()
class BLADEII_API UB2UIHeroTowerCharacterSelectPage : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void UpdateStaticText() override;

	void BattleStart();
	bool CanStartBattle();
	void RequestStartHeroTower(bool bUseGem = false);

	void PageBlock(bool bBlock);

	void SetChangeCombatPower(int32 InValue, int32 CurAttack, int32 CurDefense);

	void CheckStartButton();
	void UpDateHero();
	void SlotDelegateFunction(EPCClass InValue);

	UFUNCTION()
	void OnClickBtnEquipManagement();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void PlayChangeChacterAnimBP();

	UFUNCTION()
	void OnClickBtnChangeMainSub();

	UFUNCTION()
	void OnClickMainButton();

	UFUNCTION()
		void OnClickSubButton();
protected:
	TArray<TWeakObjectPtr<class UB2UICharacterSelectPageSlot>>		HeroArr;
	TWeakObjectPtr<UTextBlock>		STB_MainChar;
	TWeakObjectPtr<UTextBlock>		STB_SubChar;
	TWeakObjectPtr<UTextBlock>		STB_Title;
	TWeakObjectPtr<UTextBlock>		STB_Equip;
	TWeakObjectPtr<UTextBlock>		STB_CombatPower;
	TWeakObjectPtr<UTextBlock>		STB_RecomPower;

	TWeakObjectPtr<UTextBlock>		TB_CombatPower;
	TWeakObjectPtr<UTextBlock>		TB_RecomPower;

	TWeakObjectPtr<UB2Button>		BTN_EquipManagement;

	TWeakObjectPtr<class UB2UIStartButton>	UIP_StartBT;
	TWeakObjectPtr<class UB2UIRecommandPower> UIP_RecommendPower;

	TWeakObjectPtr<UB2Image>		IMG_EmptyMain;
	TWeakObjectPtr<UB2Image>		IMG_EmptySub;
	TWeakObjectPtr<UVerticalBox>	VB_MainCharacter;
	TWeakObjectPtr<UVerticalBox>	VB_SubCharacter;
	TWeakObjectPtr<UB2Button>		BTN_MainHero;
	TWeakObjectPtr<UB2Button>		BTN_SubHero;
	TWeakObjectPtr<UB2Button>		BTN_CharacterChange;

	TWeakObjectPtr<class UB2UIFairyLinkButton> UIP_FairyLinkButton;

	int32							nRecommandCombatPower;
	int32 RecommendAttack = 0;
	int32 RecommendDefense = 0;
	int32							nCurrentCombatPower;
	int32							ChangeMainClass;
	int32							ChangeSubClass;
};

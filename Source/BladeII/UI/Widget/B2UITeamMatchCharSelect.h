#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UITMFormationSelectButton.h"
#include "B2UITMCharacterSelectItem.h"
#include "B2UIFormationPopup.h"
#include "B2UITeamMatchCharSelect.generated.h"

enum class ECharacterIndex : uint8
{
	FirstCharacter = 1,
	SecondCharacter = 2,
	ThirdCharacter = 3
};

UCLASS(BlueprintType, Blueprintable) 
class UB2UITeamMatchCharSelect : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void BindDocAuto() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void UnbindDoc() override;
	virtual void UpdateStaticText() override;
	virtual void OnClose(bool RightNow = false) override;
	
	void OnClickBtnFormaitionSlot(EPCClass PCClass);
	void FormationPlayAnimation(ETMFormation Formation);
	void DelegateEnhanceButtonClick(ETMFormation Formation, int32 Level);

	void OnChangedATKLevel(UB2UIDocBase* Doc, int32 CurrLv, int32 PreLv);
	void OnChangedDEFLevel(UB2UIDocBase* Doc, int32 CurrLv, int32 PreLv);
	void OnChangedNORLevel(UB2UIDocBase* Doc, int32 CurrLv, int32 PreLv);

	UFUNCTION()
		void OnClickBtnHeroSlot(class UB2UICharacterSelectItem* SelectHero);
	UFUNCTION(BlueprintCallable, Category = TeamMatch)
		void OnClickTeamMatchStart();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayFormationSlotClickSound();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayHeroSlotClickSound();

private:
	void SetFormationButtonSlot();
	void CharacterSlotBaseSetting();
	void CharacterSlotModSetting();
	void UpdateFormationInfo(ETMFormation TMFormaiton);
	void UpdateFormationCharacterSlot();
	void SetTotalHerostat(class UB2UIDocTeamMatch* TeamDoc);
	bool CheckBuyTicket();
	void CheckGemForBuyTicket();
	void OnStartGame(bool bUsedAdditional);
	void GetAbility(ETMFormation Formation, int32 Level, TArray<float>& ATKBonus, TArray<float>& DEFBonus);

protected:
	//Static Text
	TWeakObjectPtr<UTextBlock>						TB_TMFormation;
	TWeakObjectPtr<UTextBlock>						TB_FormationManager;

	TWeakObjectPtr<class UB2UITMCharacterSelectItem>			GladiatorSlot;
	TWeakObjectPtr<class UB2UITMCharacterSelectItem>			AssassinSlot;
	TWeakObjectPtr<class UB2UITMCharacterSelectItem>			MageSlot;
	TWeakObjectPtr<class UB2UITMCharacterSelectItem>			FighterSlot;
	
	TWeakObjectPtr<UTextBlock>								TB_BattleStart;
	TWeakObjectPtr<UTextBlock>								TB_CombatPower;

	TArray<TWeakObjectPtr<class UB2UITMFormationSelectButton>>	SelectFormationButtonArr;

	TWeakObjectPtr<class UB2UITMFormation>					FormationInfo;

	TWeakObjectPtr<UB2Button>								BTN_Upgrade;
private:
	struct FormationEntry
	{
		ETMFormation FormationType;
		EPCClass Slot1Char;
		EPCClass Slot2Char;
		EPCClass Slot3Char;
	};

	UB2UITMCharacterSelectItem*	CharacterSlotArr[PCCLASS_ARRAY_SIZE];
	FormationEntry				OldFormation;
	EPCClass					NewFormationClass;
	bool						IsHeroSelect;
	uint32						DeliveryEnhanceFormationTicket;
};